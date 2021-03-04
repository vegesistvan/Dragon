
#include "stdafx.h"
#include "utilities.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include <algorithm>
#include "GA_ascendants.h"
#include <math.h>
#include <cmath>
#include "version.h"
#include "ProgressWnd.h"

bool sortByK(const _int64 &v1, const _int64 &v2); 
bool sortVAByK(const ASCENDANTS &v1, const ASCENDANTS &v2); 
bool sortVAByNameR( const ASCENDANTS &v1, const ASCENDANTS &v2 );

// listák
#define COUPLES		0
#define APAIANYAI	1
#define APAI		2
#define ANYAI		3
#define APAIE		4
#define ANYAIE		5

const CString listStyle[] =
{
	L"none",
	L"disc",
	L"square",
	L"circle",
};

IMPLEMENT_DYNAMIC(CGaAscendants, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaAscendants::CGaAscendants(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaAscendants::IDD, pParent)
	, m_lista(1)
	, m_NN(FALSE)
	, m_bold(FALSE)
	, m_numOfG_S( L"5" )
	, m_code(FALSE)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaAscendants::~CGaAscendants()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_NN, m_NN);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bold);
	DDX_Control(pDX, IDC_COMBO_LISTSTYLE, m_ListStyle);
	DDX_Text(pDX, IDC_EDIT, m_numOfG_S);
	DDX_Radio(pDX, IDC_ANSI, m_code);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGaAscendants, CDialogEx)
ON_BN_CLICKED(IDOK, &CGaAscendants::OnBnClickedOk)
ON_BN_CLICKED(IDC_CHECK_NN, &CGaAscendants::OnClickedCheckNn)
ON_COMMAND(IDC_ANYAIE, &CGaAscendants::OnAnyaie)
ON_COMMAND(IDC_RADIO_APAIE, &CGaAscendants::OnRadioApaie)
ON_BN_CLICKED(IDC_RADIO_HAZASTARSAK, &CGaAscendants::OnClickedRadioHazastarsak)
ON_COMMAND(IDC_RADIO_APAIANYAI, &CGaAscendants::OnRadioApaianyai)
ON_COMMAND(IDC_RADIO_APAI, &CGaAscendants::OnRadioApai)
ON_COMMAND(IDC_RADIO_ANYAI, &CGaAscendants::OnRadioAnyai)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for( int i = 0; i < 4; ++i )
	{
		m_ListStyle.AddString( listStyle[i] );
	}
	m_ListStyle.SetCurSel( 0 );

	// a felmenõk statisztikájának meghatározása
	if( !findAscendants( false ) )	OnOK();  // ha nincsenek szülei az emberünknek

	m_numOfFDA = getPDA( L"father_id" );
	m_numOfMDA = getPDA( L"mother_id" );

	str.Format( L"%d", m_numOfFG );
	GetDlgItem( IDC_NUMOFFG )->SetWindowTextW(  str );
	
	str.Format( L"%d", m_numOfFDA );
	GetDlgItem( IDC_NUMOFFDA )->SetWindowTextW(  str );

	str.Format( L"%d", m_numOfFA );
	GetDlgItem( IDC_NUMOFFA )->SetWindowTextW(  str );
	
	
	str.Format( L"%d", m_numOfMG );
	GetDlgItem( IDC_NUMOFMG )->SetWindowTextW(  str );
	
	str.Format( L"%d", m_numOfMDA );
	GetDlgItem( IDC_NUMOFMDA )->SetWindowTextW(  str );

	str.Format( L"%d", m_numOfMA );
	GetDlgItem( IDC_NUMOFMA )->SetWindowTextW(  str );

//	m_numOfG_S.Format( L"%d", __max( m_numOfFG, m_numOfMG ) );
	GetDlgItem( IDC_EDIT )->SetWindowText( m_numOfG_S );

	m_code = 0;

	str.Format( L"%s felmenõinek listája", m_name );
	SetWindowTextW( str );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaAscendants::getPDA( CString parent_id)
{
	int		i;
	CString rowid;

	rowid = m_rowid;
	for( i = 0; ; ++i )
	{
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", parent_id, rowid );
		if( !query( m_command ) ) return 0;
		rowid =m_recordset.GetFieldString( 0 );
		if( rowid.IsEmpty() || rowid == L"0" || _wtoi(rowid) > NOFATHERID )  break;
	}
	return( i );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	m_numOfG = _wtoi( m_numOfG_S ); 

	m_listStyleIndex = m_ListStyle.GetCurSel();
	m_ulStyle.Format( L"<ul style=list-style-type:%s;><li>", listStyle[m_listStyleIndex] );

	m_fileSpec = openFile();

	CString lista;

	switch( m_lista )
	{
	case COUPLES:
		lista = L"házastársak";
		break;
	case APAIANYAI:
		lista = L"apai-anyai ágak";
		break;
	case APAI:
		lista = L"apai ág";
		break;
	 case ANYAI:
		lista = L"anyai ág";
		break;
	}

	if( m_lista == APAIE )
		directFatherAscendants();
	else if( m_lista == ANYAIE )
	{
		if( m_numOfMDA )
			directMotherAscendants();
		else
		{
			str.Format( L"%s anyja nem ismert!", m_name );
			AfxMessageBox( str );
			fclose( fh1 );
			return;
		}
	}
	else
	{
		if( !findAscendants( true ) )				// m_maxG genrációg keres felmenõket
		{
			fclose( fh1 );	
			return;		
		}

		fwprintf( fh1, L"<pre>" );
		
		str.Format( L"%s %s\n",	L"lista                         :", lista );
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );

		str.Format( L"%s %d\n",	L"listázott generációk          :", m_numOfG );
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );

		fwprintf( fh1, L"</pre>" );

		if( m_lista == COUPLES )
			printCouples();				// a kekulé számok szerint rendezett vA vektort írja ki
		else
		{
			if( m_NN )	print1();		// vKekule-t készít és annak árnyékában írj aki a vA-t
			else		printX();		// a VA-t írj aki eredeti (feltöltés) sorrendben
		}
		printSame();					// a tbbször elõforduló embereket írja ki
	}

	fwprintf( fh1, L"<pre>" );
	str.Format( L"<br><br>Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );


	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
	ShellExecute(NULL, L"open", m_fileSpec,NULL, NULL, SW_SHOWNORMAL);
	CDialogEx::OnOK();	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Többször elõforduló felmenõk listázása
void CGaAscendants::printSame()
{
	if( vA.size() < 2 ) return;
	
	std::sort( vA.begin(), vA.end(), sortVAByNameR );			// sorrend: név és rowid szerint rendezés

	BOOL first = TRUE;
	_int64 kP = -1;
	vK.clear();
	for( int i = 0; i < vA.size()-1 ; ++i )
	{
		if( vA.at(i).rowid.IsEmpty() ) continue;

		if( vA.at(i).rowid != vA.at(i+1).rowid )
		{
			if( vK.size() )
			{
				std::sort( vK.begin(), vK.end(), sortByK );
				for( int j = 0; j < vK.size(); ++j )
				{
					if( vK.at(j) != kP )
					{
						str.Format( L"%10I64d<br>", vK.at(j) );
						if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
						fwprintf( fh1, L"%s", str );

						kP = vK.at(j);
					}
				}
				vK.clear();
			}
		}
		else
		{
			if( (i && vA.at(i).rowid != vA.at(i-1).rowid ) || !i )
			{
				if( first )
				{
					fwprintf( fh1, L"<pre>\n" );
					str.Format( L"Az alábbi emberek többször szerepelnek a családfában. A \"rowid név\" alatt a kekule számok vannak.<br><br>" );
					if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
					fwprintf( fh1, L"%s", str );


					str.Format( L"%10s %s<br>", L"rowid", L"név" );
					if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
					fwprintf( fh1, L"%s", str );

					first = false;
				}
				str.Format( L"%10s %s<br>", vA.at(i).rowid, vA.at(i).name );
				if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
				fwprintf( fh1, L"%s", str );
					
			}
			vK.push_back( vA.at(i).k );
			vK.push_back( vA.at(i+1).k );
		}
	}
	if( vK.size() )
	{
		std::sort( vK.begin(), vK.end(), sortByK );
		for( int j = 0; j < vK.size(); ++j )
		{
			if( vK.at(j) != kP )
			{
				str.Format( L"%10I64d<br>", vK.at(j) );
				if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
				fwprintf( fh1, L"%s", str );
				kP = vK.at(j);
			}
		}
	}
	fwprintf( fh1, L"</pre>" );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza paterna
void CGaAscendants::directFatherAscendants()
{
	FELMENOK	fel;

	CString rowid;
	CString father_id;
	CString name;
	CString ss;
	CString people;
	int		g;
	_int64	k;

	vFel.clear();

	fel.orig		= 1;
	fel.id			= L"";
	fel.rowid		= m_rowid;
	fel.generation	= 0;
	fel.sex_id		= m_sex_id;
	fel.kekule		= 1;
	vFel.push_back( fel );

	CProgressWnd wndProgress(NULL,L"Apák  összegyûjtése...." ); 
	wndProgress.GoModal();
	
	m_cnt_ul = 0;
	for( int i = 0; ; ++i )
	{
		rowid = vFel.at(i).rowid;
		m_command.Format( L"SELECT father_id FROM people WHERE rowid='%s'", rowid );
		if( !query( m_command ) ) return;

		father_id =m_recordset.GetFieldString( 0 );
		if( father_id.IsEmpty() || father_id == L"0" || _wtoi(father_id ) > NOFATHERID )  break;

		fel.orig		= vFel.size() + 1;
		fel.id			= rowid;
		fel.rowid		= father_id;
		fel.generation	= i+1;
		fel.sex_id		= 1;
		fel.kekule		= (_int64)pow( 2, i+1 );
		vFel.push_back( fel );
	}


	fwprintf( fh1, L"<pre>\n" );
	str.Format( L"%s %s\n",		L"lista                  :", L"Egyenes ági apai felmenõk");
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"generációk száma       :", vFel.size() );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	fwprintf( fh1, L"</pre>\n" );
	int gPrev = -1;
	for( UINT i = 0; i< vFel.size(); ++i )
	{
		g = vFel.at(i).generation;
		k = vFel.at(i).kekule;
		ss	= indent( g, gPrev );
		gPrev	= g;

		people = getPeopleString_vFel( i );
		str.Format( L"%s%I64d &diams;%s</li>\n", ss, k, people );
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );
	}
	wndProgress.DestroyWindow();

	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( fh1, L"</ul>\n" );
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza materna
void CGaAscendants::directMotherAscendants()
{
	FELMENOK	fel;

	CString rowid;
	CString father_id;
	CString mother_id;
	CString name;
	CString ss;
	CString people;
	int		g;
	_int64	k;
	_int64 kPrev;


	vFel.clear();

	fel.orig		= 1;
	fel.id			= L"";
	fel.rowid		= m_rowid;
	fel.generation	= 0;
	fel.sex_id		= 0;
	fel.kekule		= 1;
	vFel.push_back( fel );


	m_command.Format( L"SELECT mother_id, last_name, first_name FROM people WHERE rowid='%s'", m_rowid );
	if( !query( m_command ) ) return;
	mother_id = m_recordset.GetFieldString( 0 );
	if( mother_id.IsEmpty() || mother_id == L"0" )
	{
		str.Format( L"%snak nincenek ismert anyai felmenõi!", m_name );
		AfxMessageBox( str );
		return;
	}
	m_name.Format( L"%s %s", m_recordset.GetFieldString( 1 ), m_recordset.GetFieldString( 2 ) );

	fel.orig		= 1;
	fel.id			= L"";
	fel.rowid		= mother_id;
	fel.generation	= 1;
	fel.sex_id		= 0;
	fel.kekule		= 3;
	vFel.push_back( fel );

	CProgressWnd wndProgress(NULL,L"Apák  összegyûjtése...." ); 
	wndProgress.GoModal();

	m_cnt_ul = 0;
	kPrev = 6;
	for( int i = 0; ; ++i )
	{
		rowid = vFel.at(i+1).rowid;
		m_command.Format( L"SELECT father_id FROM people WHERE rowid='%s'", rowid );
		if( !query( m_command ) ) return;

		father_id =m_recordset.GetFieldString( 0 );
		if( father_id.IsEmpty() || father_id == L"0" || _wtoi(father_id ) > NOFATHERID )  break;

		fel.orig		= vFel.size() + 1;
		fel.id			= rowid;
		fel.rowid		= father_id;
		fel.generation	= i+3;
		fel.sex_id		= 1;
		fel.kekule		= kPrev*2;
		vFel.push_back( fel );
		kPrev = fel.kekule;
	}


	fwprintf( fh1, L"<pre>" );

	str.Format( L"%s %s\n",		L"lista                  :", L"Egyenes ági anyai felmenõk");
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"generációk száma       :", vFel.size() );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	fwprintf( fh1, L"</pre>" );
	int gPrev = -1;
	for( UINT i = 0; i< vFel.size(); ++i )
	{
		g = vFel.at(i).generation;
		k = vFel.at(i).kekule;
		ss	= indent( g, gPrev );
		gPrev	= g;

		people = getPeopleString_vFel( i );
		str.Format( L"%s%I64d &diams;%s\n", ss, k, people );
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );
	}
	wndProgress.DestroyWindow();
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( fh1, L"</ul>\n" );
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::indent( int gen, int genPrev )
{
	CString ss;

	if( gen == genPrev )			// azonos generáció, azonos behúzás
		ss = L"<li>";
	else if( gen > genPrev )
	{
		ss = m_ulStyle;
//		ss = L"<ul><li>";			// új generáció, beljebb nyomja, default style
		++m_cnt_ul;
	}
	else if( gen < genPrev )        // régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for( int j = 0; j < ( genPrev - gen ); ++j )  // behúzások törlése
		{
			fwprintf( fh1, L"</ul>" );
		}
		ss = L"<li>";
	}
	return ss;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByK(const _int64 &v1, const _int64 &v2) 
{ 
    return( v1 < v2 ); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortVAByK(const ASCENDANTS &v1, const ASCENDANTS &v2) 
{ 
	return( v1.k < v2.k ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortVAByNameR(const ASCENDANTS &v1, const ASCENDANTS &v2) 
{ 
	if( v1.name == v2.name ) 
		return( v1.rowid < v2.rowid ); 
	return( (v1.name < v2.name ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CGaAscendants::getName( CString rowid )
{
	CString name;
	m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", rowid );
	if( !query3( m_command ) ) return(L"");

	name.Format( L"%s %s", m_recordset3.GetFieldString( 0 ), m_recordset3.GetFieldString( 1 ) );
	return name;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::getPeopleString_vFel( int i )
{
	CString rowid		= vFel.at(i).rowid;
	CString people;

	CString sp(L" ");
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid );
	if( !query3( m_command ) ) return(L"");
	
	CString	str;
	CString str1;
	CString generation; 
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	int		parentIndex;
	int		parentIndexCalc;
	CString comment;
	CString arm;

	generation		= m_recordset3.GetFieldString( PEOPLE_GENERATION );
	title			= m_recordset3.GetFieldString( PEOPLE_TITLE );
	titolo			= m_recordset3.GetFieldString( PEOPLE_TITOLO );
	last_name		= m_recordset3.GetFieldString( PEOPLE_LAST_NAME );
	first_name		= m_recordset3.GetFieldString( PEOPLE_FIRST_NAME );
	birth_place		= m_recordset3.GetFieldString( PEOPLE_BIRTH_PLACE );
	birth_date		= m_recordset3.GetFieldString( PEOPLE_BIRTH_DATE );
	death_place		= m_recordset3.GetFieldString( PEOPLE_DEATH_PLACE );
	death_date		= m_recordset3.GetFieldString( PEOPLE_DEATH_DATE );
	father_id		= m_recordset3.GetFieldString( PEOPLE_FATHER_ID );
	mother_id		= m_recordset3.GetFieldString( PEOPLE_MOTHER_ID );
	parentIndex	= _wtoi( m_recordset3.GetFieldString( PEOPLE_MOTHER_INDEX ) );
	parentIndexCalc	= _wtoi( m_recordset3.GetFieldString( PEOPLE_MOTHER_INDEX2 ) );
	comment			= m_recordset3.GetFieldString( PEOPLE_COMMENT );
	arm				= m_recordset3.GetFieldString( PEOPLE_ARM );


	m_name.Format( L"%s %s", last_name, first_name );

	if( m_bold ) 
		str = L"<b>";
	else
		str.Empty();
	
	if( !title.IsEmpty() ) { str += title, str += " "; }
	if( !titolo.IsEmpty() ) { str += titolo, str += L" "; }
	if( !last_name.IsEmpty() ) { str += last_name, str += L" "; };
	if( !first_name.IsEmpty() ) { str += first_name, str += L" "; };
	str.TrimRight();
	
	if( m_bold ) str += L"</b>";

	if( parentIndex ) 
	{
		str1.Format( L"/%d", parentIndex);
		str += str1;
	}
	
	if( !birth_place.IsEmpty() || !birth_date.IsEmpty() )
	{
		str += L" *";
		if( !birth_place.IsEmpty() ) str += birth_place;
		if( !birth_date.IsEmpty() )
		{
			if( !birth_place.IsEmpty() ) str += sp;
			str += birth_date;
		}
	}

	if( !death_place.IsEmpty() || !death_date.IsEmpty() )
	{
		str += L" +";
		if( !death_place.IsEmpty() ) str += death_place;
		if( !death_date.IsEmpty() )
		{
			if( !death_place.IsEmpty() ) str += sp;
			str += death_date;
		}
	}

	if( !comment.IsEmpty() ) { str += " "; str += comment; }

	if( !arm.IsEmpty() )
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::getPeopleString_vA( int i )
{
	CString rowid		= vA.at(i).rowid;
	CString spouse_id	= vA.at(i).spouse_id;

	CString people;

	CString sp(L" ");
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid );
	if( !query3( m_command ) ) return(L"");
	
	CString	str;
	CString str1;
	CString generation; 
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	int		parentIndex;
	int		parentIndexCalc;
	CString comment;
	CString arm;
	CString csalad;

	generation		= m_recordset3.GetFieldString( PEOPLE_GENERATION );
	title			= m_recordset3.GetFieldString( PEOPLE_TITLE );
	titolo			= m_recordset3.GetFieldString( PEOPLE_TITOLO );
	last_name		= m_recordset3.GetFieldString( PEOPLE_LAST_NAME );
	first_name		= m_recordset3.GetFieldString( PEOPLE_FIRST_NAME );
	birth_place		= m_recordset3.GetFieldString( PEOPLE_BIRTH_PLACE );
	birth_date		= m_recordset3.GetFieldString( PEOPLE_BIRTH_DATE );
	death_place		= m_recordset3.GetFieldString( PEOPLE_DEATH_PLACE );
	death_date		= m_recordset3.GetFieldString( PEOPLE_DEATH_DATE );
	father_id		= m_recordset3.GetFieldString( PEOPLE_FATHER_ID );
	mother_id		= m_recordset3.GetFieldString( PEOPLE_MOTHER_ID );
	parentIndex	= _wtoi( m_recordset3.GetFieldString( PEOPLE_MOTHER_INDEX ) );
	parentIndexCalc	= _wtoi( m_recordset3.GetFieldString( PEOPLE_MOTHER_INDEX2 ) );
	comment			= m_recordset3.GetFieldString( PEOPLE_COMMENT );
	arm				= m_recordset3.GetFieldString( PEOPLE_ARM );
	csalad			= m_recordset3.GetFieldString( PEOPLE_CSALAD );

	m_name.Format( L"%s %s", last_name, first_name );

	if( m_bold ) 
		str = L"<b>";
	else
		str.Empty();
	
	if( !title.IsEmpty() ) { str += title, str += " "; }
	if( !titolo.IsEmpty() ) { str += titolo, str += L" "; }
	if( !last_name.IsEmpty() ) { str += last_name, str += L" "; };
	if( !first_name.IsEmpty() ) { str += first_name, str += L" "; };
	str.TrimRight();
	
	if( m_bold ) str += L"</b>";

	if( parentIndex ) 
	{
		str1.Format( L"/%d", parentIndex);
		str += str1;
	}
	
	if( !birth_place.IsEmpty() || !birth_date.IsEmpty() )
	{
		str += L" *";
		if( !birth_place.IsEmpty() ) str += birth_place;
		if( !birth_date.IsEmpty() )
		{
			if( !birth_place.IsEmpty() ) str += sp;
			str += birth_date;
		}
	}

	if( !death_place.IsEmpty() || !death_date.IsEmpty() )
	{
		str += L" +";
		if( !death_place.IsEmpty() ) str += death_place;
		if( !death_date.IsEmpty() )
		{
			if( !death_place.IsEmpty() ) str += sp;
			str += death_date;
		}
	}

	if( !comment.IsEmpty() ) { str += " "; str += comment; }

	str += L" ";
	str += getMarriage( rowid, spouse_id );
	str.TrimRight();


	
	if( !arm.IsEmpty() )
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	if( !csalad.IsEmpty() )
	{
		str += L" [";
		str += csalad;
		str += L" család]";
	}
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::getPeople( CString rowid )
{
	if( rowid.IsEmpty() )
	{
		m_sex.Empty();
		m_last_name.Empty();
		m_first_name.Empty();
		m_father_id.Empty();
		m_mother_id.Empty();
		m_nameA.Empty();
	}
	else
	{
		m_command.Format( L"SELECT sex_id, last_name, first_name, father_id, mother_id FROM people WHERE rowid='%s'", rowid );
		if( !query3( m_command ) ) return(L"");

		m_sex			= m_recordset3.GetFieldString( 0 );
		m_last_name		= m_recordset3.GetFieldString( 1 );
		m_first_name	= m_recordset3.GetFieldString( 2 );
		m_father_id		= m_recordset3.GetFieldString( 3 );
		m_mother_id		= m_recordset3.GetFieldString( 4 );
		m_nameA.Format( L"%s %s", m_last_name, m_first_name );

		if( m_father_id == L"0" ) m_father_id.Empty();
		if( m_mother_id == L"0" ) m_mother_id.Empty();

	}
	return rowid;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::getMarriage( CString rowid, CString rowidS )
{
	if( rowidS.IsEmpty() ) return L"";

	CString marriage(L"");

	m_command.Format( L"SELECT place, date FROM marriages WHERE spouse1_id ='%s' AND spouse2_id= '%s'", rowid, rowidS );
	if( !query4( m_command ) ) return L"";

	for( UINT i = 0; i < m_recordset4.RecordsCount(); ++i, m_recordset4.MoveNext() )
	{
		if( m_recordset4.RecordsCount() == 1 )
			marriage = L"=";
		else
		{
			str.Format( L" %d=", i+1 );
			marriage += str; 
		}
		marriage += m_recordset4.GetFieldString( 0 );
		marriage.TrimRight();
		marriage += L" ";
		marriage += m_recordset4.GetFieldString( 1 );
		marriage.Trim();
	}
	int n = marriage.GetLength();
	if( n < 3 ) marriage.Empty();
	return marriage;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendants::openFile()
{
	CString title;
	CString fileSpec;

	title.Format( L"%s felmenõi", m_name );
	fileSpec = theApp.openHtmlFile( &fh1, title, L"w+" );


	CString program;
	program.Format( L"Dragon version %s", VERSION );

	CString head;
	head = L"<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>";
	if( m_code == UTF8 )
	{
		head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>";
		head = UnicodeToUtf8( head ); 
	}
	fwprintf( fh1, L"%s\n", head );

	/*
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	*/

	if( m_code == UTF8 ) title = UnicodeToUtf8( title ); 
	fwprintf( fh1, L"<center>%s</center>\n", title );
	
	
	
	fwprintf( fh1, L"<pre>" );
	
	str.Format( L"%s %s\n",		L"program                        :", program );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"%s %s\n",		L"adatbázis                      :", theApp.m_databaseSpec );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"%s %s<br>\n",	L"lista készült                  :", theApp.getPresentDateTime() );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"Apai generációk száma          :", m_numOfFG );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"Apai felmenõk száma            :", m_numOfFA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d<br>\n",	L"Egyenesági apai felmenõk száma :", m_numOfFDA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"Anyai generációk száma         :", m_numOfMG );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"Anyai felmenõk száma           :", m_numOfMA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"Egyenesági anyai felmenõk száma:", m_numOfMDA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
/*
	fwprintf( fh1, L"%s %s\n",		L"program                        :", program );
	fwprintf( fh1, L"%s %s\n",		L"adatbázis                      :", theApp.m_databaseSpec );
	fwprintf( fh1, L"%s %s<br>\n",	L"lista készült                  :", theApp.getPresentDateTime() );

	fwprintf( fh1, L"%s %d\n",		L"Apai generációk száma          :", m_numOfFG );
	fwprintf( fh1, L"%s %d\n",		L"Apai felmenõk száma            :", m_numOfFA );
	fwprintf( fh1, L"%s %d<br>\n",	L"Egyenesági apai felmenõk száma :", m_numOfFDA );
	fwprintf( fh1, L"%s %d\n",		L"Anyai generációk száma         :", m_numOfMG );
	fwprintf( fh1, L"%s %d\n",		L"Anyai felmenõk száma           :", m_numOfMA );
	fwprintf( fh1, L"%s %d\n",		L"Egyenesági anyai felmenõk száma:", m_numOfMDA );
*/
	fwprintf( fh1, L"</pre>\n" );
	return fileSpec;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::query( CString command )
{
	if( m_recordset.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::query3( CString command )
{
	if( m_recordset3.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendants::query4( CString command )
{
	if( m_recordset4.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printVA( CString title )
{
	fwprintf( fh1, L"<pre>" );


	fwprintf( fh1, L"apai generációik száma:     %5d\n", m_numOfFG );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"anyai generációk száma:     %5d\n", m_numOfMG );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"összes felmenõ száma:       %5d\n", m_numOfA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"apai ág felmenõinek száma:  %5d\n", m_numOfFA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"anyai ág felmenõinek száma: %5d\n", m_numOfMA );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	fwprintf( fh1, L"</pre>" );

	fwprintf( fh1, L"<br>\n<pre><br>" );

	str.Format( L"%s<br><br>", title );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );


	str.Format( L"%5s %4s %20s %s %9s %9s %9s %3s %s\n",\
	L"sorsz",\
	L"g",\
	L"k",\
	L"w",\
	L"rowid",\
	L"father_id",\
	L"mother_id",\
	L"sex",\
	L"név" );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );	

	for( UINT i = 0; i < vA.size(); ++i )
	{
		str.Format( L"%5d %4d %20I64d %d %9s %9s %9s %3s %s<br>",\
		i+1,\
		vA.at(i).g,\
		vA.at(i).k,\
		vA.at(i).w,\
		vA.at(i).rowid,\
		vA.at(i).father_id,\
		vA.at(i).mother_id,\
		vA.at(i).sex_id,\
		vA.at(i).name  ); 
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );

		fflush( fh1 );
	}
	fwprintf( fh1, L"</pre><br>" );
	fflush( fh1 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printCouples()  
{
	CProgressWnd wndProgress(NULL,L"Felmenõk rendezése...." ); 
	wndProgress.GoModal();
	std::sort( vA.begin(), vA.end(), sortVAByK );
	wndProgress.DestroyWindow();

	CString	people;
	CString	ss;
	int		gen;

	_int64	kekule;
	_int64	k = 0;
	int		genPrev = -1;

	_int64 kMax = (_int64)pow( 2 , m_numOfG ) -1;

//	printVfel( L"Felmenõk" );

	m_cnt_ul = 0;
	k = 1;
	for( UINT i = 0; i < vA.size(); ++i )
	{
		if( vA.at(i).g <= m_numOfG )
		{
			kekule	= vA.at(i).k;
			if( kekule != k )
			{
				if( m_NN )
				{
					people = L"N N";
					kekule = k;
					str.Format( L"%s%I64d &diams;%s<br>\n", ss, kekule, people );
					if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
					fwprintf( fh1, L"%s", str );


					--i;  // ennek az embernek a kiírását próbálja megismételni, ha sorra kerül
					++k;
					continue;
				}
			}
			gen = vA.at(i).g;
			ss = indent( gen, genPrev );
			genPrev = gen;

			people = getPeopleString_vA( i );
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, kekule, people );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
			++k;
		}
	}

	if( m_NN && k < kMax )
	{
		for( ; k <= kMax; ++k )
		{
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, k, L"N N" );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
		}
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( fh1, L"</ul>\n" );
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::print1()
{
	CString people;
	CString ss;
	CString rowid;
	int		gPrev = -1;
	BOOL	volt;
	int		g;
	_int64	k;


//	printVA( L"vA");
	fillVKekule();
//	printKekule( L"Apai-anyai ág" );
//	printVfel( L"vFel" );

	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vKekule.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	m_cnt_ul = 0;
	for( UINT i = 0; i < vKekule.size(); ++i )
	{
		volt = FALSE;
		g = vKekule.at(i).g;
		k = vKekule.at(i).k;

		for( UINT j = 0; j < vA.size(); ++j )
		{
			if( vA.at(j).k == k )
			{
				ss	= indent( g, gPrev );
				gPrev	= g;

				people = getPeopleString_vA( j );
				if( people.IsEmpty() ) people = L"N N";
				str.Format( L"%s%I64d &diams;%s <div style=display:none>%s</div><br>\n", ss, k, people, vA.at(j).rowid );
				if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
				fwprintf( fh1, L"%s", str );

				volt = TRUE;
				break;
			}
		}
		if( !volt && m_NN )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;
			people = L"N N";
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, k, people );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( fh1, L"</ul>\n" );
	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printX()  
{
	CString people;
	CString ss;
	int		gPrev = -1;
	int		g;
	_int64	k;

//	printVA( L"vA" );

	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vA.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	for( UINT i = 0; i < vA.size(); ++i )
	{
		g = vA.at(i).g;
		k = vA.at(i).k;
		if( !vA.at(i).rowid.IsEmpty() )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;

			people = getPeopleString_vA( i );
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, k, people );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
		}
		else if( m_NN )
		{
			ss	= indent( g, gPrev );
			gPrev	= g;
			people = L"N N";
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, k, people );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	ss	= indent( 0, gPrev );
	fwprintf( fh1, ss );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// maxG == true		m_maxG generációig olvassa be az adatokat a vA vektorba				a felmenõk nyomtatásához
// maxG == false	minden ismert felmenõt betesz a vA vektorba, amenyit csak talál		a femenõk statisztikájához
bool CGaAscendants::findAscendants( bool maxG )
{
	ASCENDANTS gk;

	_int64	i;
	_int64	K;
	int		G;
	BOOL	apaiAg;
	BOOL	vege = false;
	CString	rowid;

	m_numOfFG	= 0;
	m_numOfMG	= 0;

	m_numOfFA	= 0;		// apai ág felmenõinek száma
	m_numOfMA	= 0;		// anyai ág felmenõinek száma
	m_numOfA	= 0;		// összes felmenõk száma

	vA.clear();

	rowid = getPeople( m_rowid );

	gk.g			= 1;
	gk.k			= 1;
	gk.w			= 0;
	gk.name			= m_name;
	gk.rowid		= m_rowid;
	gk.sex_id		= m_sex;
	gk.father_id	= m_father_id;
	gk.mother_id	= m_mother_id;
	vA.push_back( gk );
	++m_numOfFG;


	G = 2;
	K = 2;

	if( m_lista == ANYAI )
	{
		if( m_mother_id.IsEmpty() )
		{
			str.Format( L"%s anyja nem ismer!!", m_name );
			AfxMessageBox( str );
			return false;
		}

		rowid = getPeople( m_mother_id );
		gk.g			= 2;
		gk.k			= 3;
		gk.w			= 0;
		gk.name			= m_nameA;
		gk.rowid		= rowid;
		gk.sex_id		= m_sex;
		gk.father_id	= m_father_id;
		gk.mother_id	= m_mother_id;
		vA.push_back( gk );
		G = 3;
		K = 6;
		apaiAg = false;
		++m_numOfMG;
		++m_numOfA;
		++m_numOfMA;


	}
	else
	{
		if( m_father_id.IsEmpty() )
		{
			str.Format( L"%s apja nem ismer!!", m_name );
			AfxMessageBox( str );
			return false;
		}
		rowid = getPeople( m_father_id );
		gk.g			= 2;
		gk.k			= 2;
		gk.w			= 0;
		gk.name			= m_nameA;
		gk.rowid		= rowid;
		gk.sex_id		= m_sex;
		gk.father_id	= m_father_id;
		gk.mother_id	= m_mother_id;
		vA.push_back( gk );
		G = 3;
		K = 4;
		apaiAg = true;
		++m_numOfFG;
		++m_numOfA;
		++m_numOfFA;
	}
//	_int64 KMAX = 9223372036854775807;
	do
	{
		if( isEven( K ) )
			rowid = getPeople( m_father_id );
		else
			rowid = getPeople( m_mother_id );
	
		if( maxG && G > m_numOfG )		// ha generációk maximálva vannak, akkor e fölött üres rekordokat csinál
		{
			rowid.Empty();
			m_nameA.Empty();
			m_sex.Empty();
			m_father_id.Empty();
			m_mother_id.Empty();
		}

		if( !rowid.IsEmpty() )
		{
			if( _wtoi(m_father_id) > NOFATHERID ) m_father_id.Empty();
			gk.g			= G;
			gk.k			= K;
			gk.w			= 0;
			gk.name			= m_nameA;
			gk.rowid		= rowid;
			gk.sex_id		= m_sex;
			gk.father_id	= m_father_id;
			gk.mother_id	= m_mother_id;

			vA.push_back( gk );

			if( maxG && isOdd( K ) && m_sex != WOMANS )
			{
				fwprintf( fh1, L"%10I64d %s %s<br>", K, m_sex, m_nameA ); 
			}

			++m_numOfA;
			if( apaiAg )
			{
				m_numOfFG = __max( G, m_numOfFG );
				++m_numOfFA;
			}
			else
			{
				m_numOfMG = __max( G, m_numOfMG );
				++m_numOfMA;
			}
		}

		if( isOdd( K ) )										// nõt viszgáltunk
		{
			for( int j = vA.size()-1; j >= 0; --j )
			{
				if( vA.at(j).k	== K - 1 )
				{
					vA.at(j).w = 1;								// a férjénél (ha van ) bejegyezzük, hogy már vizsgáltuk a feleségét
					vA.at(j).spouse_id = rowid;					// és annak rowid-ját is ( spouse_id );
					break;
				}
			}

			if( rowid.IsEmpty() )								// nem volt felmenõ
			{
																// megkeressük az elsõ férfit, akinek a feleségét még nem kereste
				for( i = vA.size() - 1; 0 <= i; --i )			// azt kell kiírnunk.
				{
					if( isEven( vA.at(i).k ) && vA.at(i).w == 0 )
					{
						G			= vA.at(i).g;
						K			= vA.at(i).k + 1;
						vA.at(i).w	= 1;
						m_mother_id	= vA.at(i-1).mother_id;
						break;
					}
				}
				if( i == -1 )
				{
					vege = true;
					break;							// minden feleség ki lett már írva!! Vége.
				}
				if( K == 3 )
				{
					if( m_lista == APAI ) 
						vege  = true;
					else
						apaiAg = false;
				}
			}
			else												// volt ismert feleség
			{
				G = G + 1;										// a következõ generációban
				K = K * 2;										// a nõ apját írjuk ki
			}
		}
		else													// férfit írtunk ki
		{
			if( rowid.IsEmpty() )								// nem találtunk ismmert felmenõt
			{
				K = K + 1;										// következik feleség ugyanabban a generációban ( ha van )
				m_mother_id = vA.at( vA.size()-1).mother_id;	// gyermeke anyja az õ felesége
			}
			else												// találtunk ismert apát
			{
				G = G + 1;										// a következõ generációban
				K = K * 2;										// keressük az apját
			}
		}
	}
	while( !vege );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Létrehgozza a vKekule vektort, ami tartralmazza az adott generációszámig az
// apai+anyai, apai vagy anyai ágakban elõforduéó összes kekule számot a kiírás sorrendjeben.
// Ez fogja meghatározni, hogy a vFel vektorban összegyûjtött emberek közül kit és milyen sorrendben ír ki a printAscendats.
//
//	gk.g	generációk száma
//	gk.k	kekule szám
//	gl.w	1: ennek az embernek a felesége is benne van már a vKekule vektorban
//			A legnagyobb generációból történõ visszalépésekhez szükséges
void CGaAscendants::fillVKekule()
{
	KEKULE gk;
	_int64 i;

	_int64	K;
	int		G;
//	int		ix;

	_int64	KMAX = (_int64)pow( (double)2, (double)m_numOfG ) - 1;

	vKekule.clear();

	gk.g	= 1;											//
	gk.k	= 1;
	gk.w	= 0;
	vKekule.push_back( gk );

	if( m_lista == APAIANYAI || m_lista == APAI )								// apai generációval folytatja
	{
		gk.g	= 2;
		gk.k	= 2;
		gk.w	= 0;
		vKekule.push_back( gk );

		G = 3;
		K = 4;
	}
	else													// anyai generációval kezdi
	{
		gk.g	= 2;
		gk.k	= 3;
		gk.w	= 0;
		vKekule.push_back( gk );

		G = 3;
		K = 6;
	}
	
	while( K <= KMAX )
	{
		gk.g = G;
		gk.k = K;
		gk.w = 0;
		vKekule.push_back( gk );
		
		if( K == KMAX ) break;

		if( isOdd( K ) )									// nõt írtunk ki
		{
			if( G == m_numOfG )						// az utolsó generációban, akkor 1-el vagy többel  vissza
			{
															// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva.
				for( i = vKekule.size() - 1; 0 <= i; --i )	// azt kell kiírnunk.
				{
					if( isEven( vKekule.at(i).k ) && vKekule.at(i).w == 0 )
					{
						G = vKekule.at(i).g;
						K = vKekule.at(i).k + 1;
						vKekule.at(i).w = 1;
						break;
					}
				}
				if( i == 0 ) break;							// minden feleség ki lett már írva!! Vége.
				if( m_lista == APAI && G == 2 ) break;				// apai ág vége
			}
			else											// még nem az utolsó generációban vagyunk
			{
				G = G + 1;									// a következõ generációban
				K = K * 2;									// a nõ apját írjuk ki
			}
		}
		else												// férfit írtunk ki
		{
			if( G < m_numOfG )						// még van további generáció
			{
				G = G + 1;
				K = K * 2;									// következik: következõ generációban az apa
			}
			else											// nincs több generáció
			{
				vKekule.at( vKekule.size()- 1 ).w	= 1;	// a feleségét kiírja!
				K = K + 1;									// következik feleség ugyanabban a generációban
			}
		}
	}
//	printKekule(L"");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::printKekule( CString title )
{
	fwprintf( fh1, L"<br>\n<pre>" );
	str.Format( L"%s\n\n", title );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	for( _int64 i = 0; i < vKekule.size(); ++i )
	{
		str.Format( L"%4d. %4d %12d %d\n", i+1, vKekule.at(i).g, vKekule.at(i).k, vKekule.at(i).w );
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );
	}
	fwprintf( fh1, L"</pre>" );
	fflush( fh1 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnClickedCheckNn()
{
	UpdateData(FROMSCREEN );
	if( m_NN )
	{
		if( m_numOfG > 6 )
		{
			if( AfxMessageBox( L"Tényleg helyettesíteni akarod a nem ismert felmenõ nevét?", MB_YESNO ) == IDNO )
			{
				m_NN = false;
				UpdateData( TOSCREEN );
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnAnyaie()
{
	GetDlgItem( IDC_EDIT )->SetWindowText( L"" );
	GetDlgItem( IDC_EDIT )->EnableWindow( FALSE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( FALSE );
	m_numOfG = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnRadioApaie()
{
	GetDlgItem( IDC_EDIT )->SetWindowText( L"" );
	GetDlgItem( IDC_EDIT )->EnableWindow( FALSE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( FALSE );
	m_numOfG = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnClickedRadioHazastarsak()
{
	GetDlgItem( IDC_EDIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( TRUE );
	str.Format( L"%d", __max( m_numOfFG, m_numOfMG ) );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnRadioApaianyai()
{
	GetDlgItem( IDC_EDIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( TRUE );
	str.Format( L"%d", __max( m_numOfFG, m_numOfMG ) );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnRadioApai()
{
	GetDlgItem( IDC_EDIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( TRUE );
	str.Format( L"%d", m_numOfFG );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendants::OnRadioAnyai()
{
	GetDlgItem( IDC_EDIT )->EnableWindow( TRUE );
	GetDlgItem( IDC_CHECK_NN )->EnableWindow( TRUE );
	str.Format( L"%d", m_numOfMG );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////