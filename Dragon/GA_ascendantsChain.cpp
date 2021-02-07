// AscendantsChain.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GA_ascendantsChain.h"
#include "afxdialogex.h"
#include "version.h"
#include "ProgressWnd.h"
#include "utilities.h"

// CGaAscendantsChain dialog
#define SPOUSES 0
#define FATHER_MOTHER_BRANCH 1

IMPLEMENT_DYNAMIC(CGaAscendantsChain, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaAscendantsChain::CGaAscendantsChain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaAscendantsChain::IDD, pParent)
	, m_numOfP(20)
	, m_list(FALSE)
	, m_NN(TRUE)
	, m_kekule(16)
	, m_bold(FALSE)
	, m_code(FALSE)
{
	GMAX = 5;
	GNEXT = 1;
	KNEXT = 1;


	KBIAS = 0;
	GBIAS = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaAscendantsChain::~CGaAscendantsChain()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_numOfP);
	DDV_MinMaxInt(pDX, m_numOfP, 1, 100);
	DDX_Radio(pDX, IDC_RADIO1, m_list);
	DDX_Check(pDX, IDC_CHECK_NN, m_NN);
	DDX_Text(pDX, IDC_KEKULE, m_kekule);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bold);
	DDX_Radio(pDX, IDC_ANSI, m_code);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGaAscendantsChain, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGaAscendantsChain::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_bold = FALSE;
	str.Format( L"%s 5 generációs felmenõi piramisainak lánca", m_name );
	SetWindowTextW( str );

	m_code = 0;

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::getPeople( CString rowid )
{
	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid = '%s'", rowid );
	if( !theApp.query( m_command ) ) return false;
	m_name.Format( L"%s %s %s", theApp.m_recordset->GetFieldString( PEOPLE_TITOLO ), theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME), theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
	m_name.Trim();
	m_sex_id	= theApp.m_recordset->GetFieldString( PEOPLE_SEX_ID );

	m_father_id	= theApp.m_recordset->GetFieldString( PEOPLE_FATHER_ID );
	if( _wtoi( m_father_id ) > FATHERID ) m_father_id.Empty();
	if( _wtoi( m_father_id ) == 0 ) m_father_id.Empty();

	m_mother_id	= theApp.m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
	if( _wtoi( m_mother_id ) == 0 ) m_mother_id.Empty();

	if( m_sex_id == MANS )
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id='%s'", rowid );
	else
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse2_id='%s'", rowid );
	if( !theApp.query( m_command ) ) return false;
	m_spouse_id = theApp.m_recordset->GetFieldString( 0 );

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
//	m_ComboKekule.GetLBText( m_ComboKekule.GetCurSel(), str );

	m_KStart = m_kekule;


	CString fileSpec;
	fileSpec = openFile();


	if( !getProbantus( m_rowid, m_kekule ) )
	{
		str.Format( L"%s %d kekule számú felmenõje nem ismert!", m_name, m_kekule );
		AfxMessageBox( str );
		return;
	}

//	str.Format( L"<br><br>%s felmenõi", m_name );
//	fwprintf( fh1, str );

//	fillVKekule();
	fillVKekuleNew( );
//	printVKekule( L"alma" );

	for( int i = 0; i < m_numOfP; ++i )
	{
		if( m_rowid.IsEmpty() || !m_rowid.Compare( L"0" ) ) break;

		getPeople( m_rowid );
		setVA();

		vA.at(0).rowid		= m_rowid;        // probantus adatai
		vA.at(0).sex_id		= m_sex_id;
		vA.at(0).name		= m_name;
		vA.at(0).father_id	= m_father_id;
		vA.at(0).mother_id	= m_mother_id;
		vA.at(0).spouse_id	= m_spouse_id;
		fillVA();

//		printVA( L"" );

		printAscendants(i);

		m_rowid = vA.at( 15 ).father_id;
	}


	fclose( fh1 );
	ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::setVA()
{
	ASC a;
	int K1;
	int	K2;
	int	G;
	int	numOfK;
	int	cnt = 1;

	vA.clear();


	for( int g = 1; g <= 5; ++g )
	{
		K1 = (int)pow( 2, g-1 );			// k kezdõérték
		K2 = (int)pow( 2, g ) - 1;
		for( int k = K1; k <= K2; ++k )
		{
			++cnt;
			a.cnt	= cnt;
			a.G		= g;
			a.K		= k;
			vA.push_back( a );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::fillVA()
{
	int		j;
	int		k;
	int		ki;
	int		kFel;
	CString parent_id;
	CString rowid;
	CString sex_id;
	CString name;
	CString father_id;
	CString mother_id;
	CString spouse_id;

	for( int i = 1; i < vA.size(); ++i )  // elsõ embert (0.) kihagyja
	{
		k	= vA.at(i).K;
		kFel = k/2;
		ki	= i/2;
		for( j = 0; j < vA.size(); ++j )
		{
			if( vA.at(j).K == kFel ) break;
		}

		if( isEven( k ) )  // apát kell meghatározni
		{
			parent_id = vA.at(j).father_id;
		}
		else
		{
			parent_id = vA.at(j).mother_id;
		}

		getPeople( parent_id );
		vA.at(i).rowid		= parent_id;
		vA.at(i).sex_id		= m_sex_id;
		vA.at(i).name		= m_name;
		vA.at(i).father_id	= m_father_id;
		vA.at(i).mother_id	= m_mother_id;
		vA.at(i).spouse_id	= m_spouse_id;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::printVA( CString title)
{
	int cnt;
	int G;
	int K;
	CString sex_id;
	CString rowid;
	CString name;
	CString father_id;
	CString mother_id;
	CString spouse_id;

	fwprintf( fh1, L"<br>\n<pre>" );
	
	str.Format( L"%s<br><br>", title );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%4s %3s %10s %-40s %10s %10s %10s %10s<br><br>", L"cnt", L"G", L"K", L"name", L"rowid", L"father_id", L"mother_id", L"spouse_id" ); 
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	for( int i = 0; i < vA.size(); ++i )
	{
		cnt			= vA.at(i).cnt;
		G			= vA.at(i).G + GBIAS;
		K			= vA.at(i).K + KBIAS;
		rowid		= vA.at(i).rowid;
		name		= vA.at(i).name;
		father_id	= vA.at(i).father_id;
		mother_id	= vA.at(i).mother_id;
		spouse_id	= vA.at(i).spouse_id;
		str.Format( L"%3d. %3d %10d %-40s %10s %10s %10s %10s<br>", cnt, G, K, name, rowid, father_id, mother_id, spouse_id );  
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );
	}
	fwprintf( fh1, L"</pre>");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::printVKekule( CString title)
{
	int cnt;
	int G;
	int K;
	int W;

	fwprintf( fh1, L"<br>\n<pre>" );
	str.Format( L"%s<br><br>", title );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%4s %4s %4s %s<br><br>", L"cnt", L"G", L"K", L"W" ); 
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	for( int i = 0; i < vKekule.size(); ++i )
	{
		G	= vKekule.at(i).g;
		K	= vKekule.at(i).k;
		W = vKekule.at(i).w;
		str.Format( L"%3d. %4d %4d %d<br>", i+1, G, K, W );  
		if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
		fwprintf( fh1, L"%s", str );
	}
	fwprintf( fh1, L"</pre>");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CGaAscendantsChain::printAscendants( int i )
{
	if( m_list == 0 )
		print0();
	else
		print0();
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::printAscendants( int cnt )  // házaspárok generációnként
{
	CString	people;
	CString	ss;
	int		gen;
	int		genPrev		= -1;
	int		kekule;
	int		kHusband;
	int		kek;
	int		kPrevious;
	int		kGen;

	m_cnt_ul = 0;
	m_kFirst = m_kekule + cnt*32 - 1;
	for( UINT i = 0; i < vA.size(); ++i )
	{
		gen = vA.at(i).G;
		ss = indent( gen, genPrev );
		
		if( i == 0 )
		{
			kekule = m_kekule * (int)pow( 2, cnt*5 );
			kPrevious = kekule;
			kGen = kekule;
		}
		else
		{
			if( isOdd( i ) )					// férj
			{
				if( gen == genPrev )
				{
					++kekule ;
				}
				else
				{
					kekule = kGen *2;			// generáció váltáskor az elõzõ generáció k*2
					kGen = kekule;
				}
				kPrevious = kekule;
			}
			else								// feleség
			{
				kekule = kPrevious + 1;
			}

		}
		genPrev = gen;
		people.Empty();
		if( vA.at(i).rowid.IsEmpty() )
		{
			if( m_NN )	people = L"N N";
		}
		else
			people = getPeopleString( i );
		if( !people.IsEmpty() )
		{
			str.Format( L"%s%I64d &diams;%s<br>\n", ss, kekule, people );
			if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
			fwprintf( fh1, L"%s", str );
		}
	}
	for( int i =0; i < m_cnt_ul; ++i )
		fwprintf( fh1, L"</ul>\n" );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::print1()		// Apai-anyai ág
{

	CString people;
	CString ss;
	int		gPrev = -1;
	BOOL	volt;
	int		g;
	_int64	k;

	CProgressWnd wndProgress(NULL,L"Felmenõk listázása...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vKekule.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	for( UINT i = 0; i < vKekule.size(); ++i )
	{
		volt = FALSE;
		g = vKekule.at(i).g;
		k = vKekule.at(i).k;

		for( UINT j = 0; j < vA.size(); ++j )
		{
			if( vA.at(j).K == k )
			{
				ss	= indent( g, gPrev );
				gPrev	= g;

				people = getPeopleString( j );
				str.Format( L"%s%I64d &diams;%s<br>\n", ss, k, people );
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
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::fillVKekule()
{
	KEKULE gk;

	_int64 i;

	_int64 KMAX;
	_int64	K;
	int		G = 0;
	int	generationsMax = 5;

	KMAX = (_int64)pow( (double)2, (double)generationsMax ) - 1;

	vKekule.clear();

	gk.g	= 1;
	gk.k	= 1;
	gk.w	= 0;
	vKekule.push_back( gk );

	gk.g	= 2;
	gk.k	= 2;
	gk.w	= 0;
	vKekule.push_back( gk );

	G = 3;
	K = 4;
	
	while( K <= KMAX )
	{
		gk.g = G;
		gk.k = K;
		gk.w = 0;
		vKekule.push_back( gk );
		if( K == KMAX ) break;

		if( isOdd( K ) )								// nõt írtunk ki
		{
			if( G == generationsMax )					// az utolsó generációban, akkor 1-el vagy többel  vissza
			{

				// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva. Azt kell kiírnunk
				for( i = vKekule.size() - 1; 0 <= i; --i )
				{
					if( isEven( vKekule.at(i).k ) && vKekule.at(i).w	== 0 )
					{
						G = vKekule.at(i).g;
						K = vKekule.at(i).k + 1;
						vKekule.at(i).w = 1;
						break;
					}
				}
				if( i == 0 ) break;   // minden feleség ki lett írva!!
			}
			else										// még nem az utolsó generációban vagyunk
			{
				G = G + 1;								// a következõ generációban
				K = K * 2;								// a nõ apját írjuk ki
			}
		}
		else											// férfit írtunk ki
		{
			if( G < generationsMax )					// még van további generáció
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
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaAscendantsChain::fillVKekuleNew()
{
	KEKULE gk;

	_int64  kx;
	_int64 i;

	_int64 KMAX;
	_int64	K;
	int		G;
	int		generationsMax = 5;

	int	gBase; 

	KMAX = (_int64)pow( (double)2, (double)generationsMax ) - 1;


	vKekule.clear();



	// apai generáció 

	gk.g	= 1;
	gk.k	= 1;
	gk.w	= 0;
	vKekule.push_back( gk );

	gk.g	= 2;
	gk.k	= 2;
	gk.w	= 0;
	vKekule.push_back( gk );

	G = 3;
	K = 4;
	
	while( K <= KMAX )
	{
		gk.g = G;
		gk.k = K;
		gk.w = 0;
		vKekule.push_back( gk );
		
		if( K == KMAX ) break;

		if( isOdd( K ) )								// nõt írtunk ki
		{
			if( G == generationsMax )					// az utolsó generációban, akkor 1-el vagy többel  vissza
			{

				// megkeressük az elsõ férfit, akinek a felesége még nem volt kiírva. Azt kell kiírnunk
				for( i = vKekule.size() - 1; 0 <= i; --i )
				{
					if( isEven( vKekule.at(i).k ) && vKekule.at(i).w	== 0 )
					{
						G = vKekule.at(i).g;
						K = vKekule.at(i).k + 1;
						vKekule.at(i).w = 1;
						break;
					}
				}
				if( i == 0 ) break;   // minden feleség ki lett írva!!
			}
			else										// még nem az utolsó generációban vagyunk
			{
				G = G + 1;								// a következõ generációban
				K = K * 2;								// a nõ apját írjuk ki
			}
		}
		else											// férfit írtunk ki
		{
			if( G < generationsMax )					// még van további generáció
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendantsChain::indent( int gen, int genPrev )
{
	CString ss;

//	if( gen == 0 ) return L"";

	if( gen == genPrev )			// azonos generáció, azonos behúzás
		ss = L"<li>";
	else if( gen > genPrev )
	{
		ss = L"<ul><li>";			// új generáció, beljebb nyomja, 
		++m_cnt_ul;
	}
	else if( gen < genPrev )        // régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for( int j = 0; j < ( genPrev - gen ); ++j )
		{
			fwprintf( fh1, L"</ul>" );
		}
		ss = L"<li>";
	}
	return ss;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendantsChain::getPeopleString( int i )
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

	str += L" ";
	str += getMarriage( rowid, spouse_id );
	str.TrimRight();


	
	if( !arm.IsEmpty() )
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendantsChain::getMarriage( CString rowid, CString rowidS )
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendantsChain::openFile()
{
	CString title;
	CString fileSpec;

	title.Format( L"%s felmenõi piramisok", m_name );
	fileSpec = theApp.openHtmlFile( &fh1, title, L"w+" );


	CString head;
	head = L"<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>";
	if( m_code == UTF8 )
	{
		head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>";
		head = UnicodeToUtf8( head ); 
	}
	fwprintf( fh1, L"%s\n", head );

	CString program;
	program.Format( L"Dragon version %s", VERSION );

/*
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
*/
	str.Format( L"<center>%s</center>\n", title );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	fwprintf( fh1, L"<pre>" );

	str.Format( L"%s %s\n",		L"program               :", program );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %s\n",		L"adatbázis             :", theApp.m_databaseSpec );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %d\n",		L"listázott generációk  :", GMAX );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );
	
	str.Format( L"%s %d\n",		L"piramis csúcsa:       :", m_KStart );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	str.Format( L"%s %s\n",		L"lista készült         :", theApp.getPresentDateTime() );
	if( m_code == UTF8 ) str = UnicodeToUtf8( str ); 
	fwprintf( fh1, L"%s", str );

	fwprintf( fh1, L"</pre>\n" );

	return fileSpec;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::query( CString command )
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
BOOL CGaAscendantsChain::query1( CString command )
{
	if( m_recordset1.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::query2( CString command )
{
	if( m_recordset2.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::query3( CString command )
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
BOOL CGaAscendantsChain::query4( CString command )
{
	if( m_recordset4.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaAscendantsChain::getProbantus( CString rowid, int kekule )
{
	FELMENOK	fel;
	int		gMax;
	_int64	kMin;
	_int64	kMax;
	_int64	k;
	_int64 kFather;			// egy generációban a legnagyobb férfiági Kekule szám
	int		peoplePerG;
	int		ix = 0;			// a keresett ember szõlõjét tartalmazó vFel index
	CString father_id;
	CString mother_id;
	CString parent_id;
	CString name;
	BOOL kell;
	int i;


	vFel.clear();

	fel.orig		= 1;
	fel.id			= L"";
	fel.rowid		= m_rowid;
	fel.generation	= 0;
	fel.name		= m_name;
	fel.sex_id		= _wtoi( m_sex_id );
	fel.kekule		= 1;
	vFel.push_back( fel );

	gMax = (int)Log2( kekule ) + 1;


	str.Format( L"%d kekulé számú probandus meghatározása....", kekule );
	CProgressWnd wndProgress(NULL, str ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, gMax );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
	

	for( int g = 1; g <= gMax; ++g )
	{
		str.Format( L"Probandus meghatározása..." );
#ifndef _DEBUG
		wndProgress.SetText( str );
#endif


		peoplePerG = 0;
//		m_maxG = g;
		kMin = (_int64) pow( 2, g );
		kMax = (_int64) pow( 2, g + 1 ) - 1;

		kFather = kMin + (kMax - kMin )/2;
		for( k = kMin; k <= kMax; k += 2 )
		{
			// megkeerssük a k kekule számú ember gyerekét a vFel vektorban
			for( i = 0; i < vFel.size(); ++i )		// a k Kekulé számú ember gyerekének azonosítása a vFel vektorban.
			{										// azért szükséges, hogy a father_id, mother_id-t meghatározzuk és
				if( vFel.at(i).kekule == k/2 )		// ezzel azonosítsuk az apát és anyát. Azért kell keresni, mert a Vfel index
				{									// ismertelen felmenõk miatt különbözhet k/2 tõl.
					ix = k/2 - 1;
					ix = i;
					break;
				}
			}
			if( k != 1 && i == vFel.size() )		// 0 generációval vagy nemlétezõ gerekkel nem foglalkozunk
				continue;

			rowid = vFel.at(ix).rowid;
			m_command.Format( L"SELECT father_id, mother_id FROM people WHERE rowid='%s'", rowid );
			if( !query( m_command ) ) return false;

			for( int i = 0; i < 2; ++i )
			{
				parent_id =m_recordset.GetFieldString( i );
				if( parent_id.IsEmpty() || parent_id == L"0" || _wtoi(parent_id ) > FATHERID ) 
					continue;		// nincs apa vagy anya
				// ha egy ember többször szerepelne különbözõ minõségben, akkor annak újabb elõfordulását kihagyja!!!
				for( int j = 0; j < vFel.size(); ++j )
				{
					if( vFel.at(j).rowid == parent_id )
						continue;
				}


				name = getName( parent_id );							// emberük szülõjét elteszi
				fel.orig		= vFel.size() + 1;
				fel.id			= rowid;
				fel.rowid		= parent_id;
				fel.spouse_id	= m_recordset.GetFieldString( 1 );
				fel.generation	= g;
				fel.name		= name;
				fel.sex_id		= i + 1;
				fel.kekule		= k + i;
				vFel.push_back( fel );

				++peoplePerG;
			}
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
		if( peoplePerG == 0 ) break;  // a generációban nincs egyetlen ember sem! Vége!
	}
	wndProgress.DestroyWindow();

	m_rowid.Empty();
	for( int i = 0; i < vFel.size(); ++i )
	{
		if( vFel.at(i).kekule == kekule )
		{
			m_rowid		= vFel.at(i).rowid;
			m_name		= vFel.at(i).name;
			m_sex_id.Format( L"%d", vFel.at(i).sex_id );
			m_father_id	= vFel.at(i).father_id;
			m_mother_id	= vFel.at(i).mother_id;
			m_spouse_id	= vFel.at(i).spouse_id;
			break;
		}
	}
	if( m_rowid.IsEmpty() ) return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaAscendantsChain::getName( CString rowid )
{
	CString name;
	m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", rowid );
	if( !query3( m_command ) ) return(L"");

	name.Format( L"%s %s", m_recordset3.GetFieldString( 0 ), m_recordset3.GetFieldString( 1 ) );
	return name;
}

