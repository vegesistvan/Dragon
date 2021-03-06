// GaDescendants2.cpp : implementation file
//
// Ha n�k lesz�rmazottait is list�zzuk, akkor n�mely esetben olyan hatalmas lesz�rmazotti lista keletkezne, ami 
// olyan hat�st kelt, hogy legagyna a program. Lehet, hogy le is fagy, vagyis a html f�jl �sszet�rt sorokat tartalmaz,
// kezelhetetlen az eg�sz


#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_descendants.h"
#include <winuser.h>
#include "ProgressWnd.h"
#include "ColorButton.h"
#include <wingdi.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGaDescendants, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaDescendants::CGaDescendants(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaDescendants::IDD, pParent)
	,m_connect(true)		// t�bl�kat �sszek�sse-e
	,m_woman(false)			// n�k lesz�rmazottait list�zza-e
	,m_setCombo(1)			// 0= nincsenek kiemel�sek, 1= defeault kiemel�sek 

	,m_ixName( 1 )			// n�v bold
	,m_ixSpec( 0 )			// speci�lis karakterek (*+=) bold
	,m_ixComment( 2 )		// comment bold
	,m_ixFamily( 3 )
	,m_ixFontSize( 3 )

	,m_rowid1(L"")			// a lesz�rmazott rowid-ja
	,m_name(L"")			// lesz�rmazott �s neve
	,m_tableNumber(L"")		// tablenumber, ha a t�bl�zat lesz�rmazotti list�j�t k�rj�k
	,m_CheckLastName(true)	// csal�dn�v ki�r�sa
	,m_code(true)			// ANSI vagy UTF8 k�drendszer
	,m_numbering(TUP)		// milyen sz�moz�si rendszer legyen (0,1,2) SZLUHA/VIL/TUP
	,m_checkFamily(false)	// %%% csal�dn�v,el�n�v kiemel�se
	, m_wrap(true)
{
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaDescendants::~CGaDescendants()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_CONNECT, m_connect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, m_woman);

	DDX_Control(pDX, IDC_COMBO_COMMENT, m_ComboComm);
	DDX_Control(pDX, IDC_COMBO_NAME, m_ComboName);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_ComboSpec);
	DDX_Control(pDX, IDC_COMBO_FAMILY, m_ComboFamily);

	DDX_Radio(pDX, IDC_RADIO_CLEAR, m_setCombo);
	DDX_Check(pDX, IDC_CHECK_LASTNAME, m_CheckLastName);
	DDX_Radio(pDX, IDC_ANSI, m_code);

	DDX_Check(pDX, IDC_CHECK_FAMILY, m_checkFamily);
	DDX_Control(pDX, IDC_SZLUHA, m_szluhaCtrl);
	DDX_Check(pDX, IDC_CHECK_WRAP, m_wrap);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_ComboFontSize);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, colorBgn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGaDescendants, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_COMMAND(IDC_RADIO_DEFAULT, &CGaDescendants::OnRadioDefault)
	ON_COMMAND(IDC_TUPIGNY, &CGaDescendants::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CGaDescendants::OnVillers)
	
	ON_BN_CLICKED(IDC_RADIO_CLEAR, &CGaDescendants::OnClickedRadioClear)
	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CGaDescendants::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CGaDescendants::OnClickedCheckConnect)
	ON_BN_CLICKED(IDC_SZLUHA, &CGaDescendants::OnClickedSzluha)

	ON_BN_CLICKED(IDOK, &CGaDescendants::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_FAMILY, &CGaDescendants::OnClickedCheckFamily)
	ON_BN_CLICKED(IDC_CHECK_LASTNAME, &CGaDescendants::OnClickedCheckLastname)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CGaDescendants::OnClickedButtonDefault)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND, &CGaDescendants::OnClickedStaticBackground)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet:
// m_rowid1
// m_name	
// m_source
// m_tableNumebr
// 
// m_numbering
// m_code			ANSI/UTF8
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for( int i = 0; i < sizeof( attrib ) / sizeof( ATTRIB ); ++i )
	{
		m_ComboName.AddString( attrib[i].text );
		m_ComboSpec.AddString( attrib[i].text );
		m_ComboComm.AddString( attrib[i].text );
		m_ComboFamily.AddString( attrib[i].text );
	}
	m_ComboName.SetCurSel( m_ixName );
	m_ComboComm.SetCurSel( m_ixComment );
	m_ComboSpec.SetCurSel( m_ixSpec );
	m_ComboFamily.SetCurSel( m_ixFamily );

	for( INT i = 0; i < 6; ++i )
	{
		str.Format( L"%d", 10+i*2 );
		m_ComboFontSize.AddString( str );
	}
	m_ComboFontSize.SetCurSel( m_ixFontSize );

	if( theApp.m_inputMode == MANUAL )
	{
		m_connect	= true;
		m_woman		= true;
	}

	colorBgrnd.SetTextColor( theApp.m_colorClick );

	m_colorBgrnd = WHITE;
	colorBgn.SetColor( BLACK, m_colorBgrnd );

	m_numbering = 2;
	((CButton *)GetDlgItem(IDC_SZLUHA))->SetCheck(FALSE); 
	((CButton *)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_TUPIGNY))->SetCheck(TRUE);

	UpdateData( TOSCREEN );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	m_ixName		= m_ComboName.GetCurSel();
	m_ixSpec		= m_ComboSpec.GetCurSel();
	m_ixComment		= m_ComboComm.GetCurSel();
	m_ixFamily		= m_ComboFamily.GetCurSel();
	m_ixFontSize	= m_ComboFontSize.GetCurSel();

	if( m_woman ) m_connect = true;

	CDialogEx::OnOK();

	if( m_rowid1.IsEmpty() )
		treeTables();
	else
		treePeople();

	CDialogEx::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::treePeople()
{
	CString file;
	CString title;
	CString tableHeader;
	
	file.Format( L"%s lesz�rmazottai", m_name);
	title.Format( L"%s lesz�rmazottai", m_name );
	
	openHtml( file, title, m_colorBgrnd );

	if( theApp.m_inputMode == GAHTML && !m_checkFamily )
	{
		m_command.Format( L"SELECT tableHeader from tables WHERE rowid = %s", m_tableNumber );
		if( !query4( m_command ) ) return;
		tableHeader = m_recordset4.GetFieldString( 0 );
		str.Format( L"<b>%s</b>\n", tableHeader );
		print( str );
	}


	queryP( m_rowid1 );								// m_sex_id-t �s m_numOfSpouses-t hat�rozza meg
	desc.rowid				= m_rowid1;
	desc.sex_id				= m_sex_id;
	desc.numOfSpouses		= m_numOfSpouses;
	desc.numOfChildren		= getNumberOfChildren( m_rowid1, m_sex_id );
	desc.childrenPrinted	= 0;
	desc.parentIndexLast	= 0;
	desc.hidden				= false;

	vSerial.clear();
	vSerial.push_back(1);

	vDesc.clear();
	vDesc.push_back( desc );

	m_genPrev	= 0;		// el�z� ki�rt gener�ci�
	m_indent		= 0;		// a beh�z�sok pillanatny sz�ma

	descendants();
	closeHtml();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::treeTables()
{
	CString file;
	CString title;

	CString familyName;
	CString tableHeader;
	CString tableRoman;
	CString father_id;


	m_command.Format( L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%d'", theApp.v_tableNumbers.at(0) );
	if( !query( m_command ) ) return;
	familyName	= m_recordset.GetFieldString( 0 );
	tableRoman	= m_recordset.GetFieldString( 2 );
	
	if( theApp.v_tableNumbers.size() > 1 )
	{
		if( tableRoman.IsEmpty() )
			file.Format( L"%s �s tov�bbi %d t�bla", familyName, theApp.v_tableNumbers.size() - 1 );
		else
			file.Format( L"%s %s �s tov�bbi %d t�bla", familyName, tableRoman, theApp.v_tableNumbers.size() - 1 );

	}
	else
	{
		if( tableRoman.IsEmpty() )
			file.Format( L"%s t�bla", familyName );
		else
			file.Format( L"%s %s t�bla", familyName, tableRoman );
	}
	title = file;
	openHtml( file, title, m_colorBgrnd );



	CProgressWnd wndProgress(NULL,L"Lesz�rmazotti t�bl�k k�sz�t�se folyik...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange( 0, theApp.v_tableNumbers.size() );
	wndProgress.SetPos( 0 );

	for( UINT i = 0; i < theApp.v_tableNumbers.size(); ++i )
	{
		m_familyName.Empty();
		m_tableNumber.Format( L"%d", theApp.v_tableNumbers.at(i) );

		m_command.Format( L"SELECT tableHeader FROM tables WHERE rowid ='%s'", m_tableNumber );
		if( !query( m_command ) ) return;
		tableHeader = m_recordset.GetFieldString( 0 );
		str.Format( L"<b>%s</b>\n", tableHeader );
		print( str );

		m_command.Format( L"SELECT rowid, last_name, sex_id, father_id FROM people WHERE tableNumber = '%s' AND tableAncestry == 1 ", m_tableNumber );
		if( ! query( m_command ) ) return;

		if( !m_recordset.RecordsCount() )
		{
			str.Format( L"%s t�bl�ban nincs �s!!", m_tableNumber );
			AfxMessageBox( str );
			OnCancel();
			return;
		}

		father_id	= m_recordset.GetFieldString( 3 );

		desc.childrenPrinted	= 0;
		desc.numOfSpouses		= 0;
		desc.parentIndexLast	= 0;

		vDesc.clear();
		vSerial.clear();
		m_indent	= 0;
		m_genPrev	= 0;

		if( father_id.IsEmpty() || !father_id.Compare( L"0" ) )  // ha nincs apa, akkor mag�t az �st teszi be a vDesc-be
		{
			p.generation = m_recordset1.GetFieldString( 3 );
			m_rowid1 = m_recordset.GetFieldString( 0 );
			m_sex_id = _wtoi( m_recordset.GetFieldString( 2 ) );

			desc.sex_id				= m_sex_id;
			desc.rowid				= m_rowid1;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( m_rowid1, m_sex_id );

		}
		else		// ha van apja az �snek, akkor az ap�t hidden-k�nt elteszi, hogy ha t�bb gyereke van, akkor mind list�zza 
		{
			m_command.Format( L"SELECT rowid, last_name, sex_id, generation FROM people WHERE father_id='%s'", father_id );
			if( ! query1( m_command ) ) return;

			p.generation = m_recordset1.GetFieldString( 3 );
			m_sex_id = 1;

			desc.sex_id				= 1;
			desc.rowid				= father_id;
			desc.hidden				= true;
			desc.numOfChildren		= getNumberOfChildren( father_id, m_sex_id );
		}
		vDesc.push_back( desc );
		vSerial.push_back(1);
		
		descendants();

		if( theApp.v_tableNumbers.size() > 1 )
		{
			for( int i =0; i < m_indent; ++i )
				fwprintf( fl, L"%s\n", m_tag2 );
			fwprintf( fl, L"<br><br><br>" );
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	theApp.v_tableNumbers.clear();
	closeHtml();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A kinyomtatand� lesz�rmazot azonos�t�it a <DESCENDANTS> vDesc vektorba t�ltj�k fel, 
// �s annak felhaszn�l�s�val nyomtatjuk ki �ket.
// Mindaddig abban maradnak, am�g az �sszes gyerek�k kinyomtat�sra ker�l.Akkor t�r�lj�k �t a vektorb�l
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::descendants()
{
	CString rowid;
	int generationsMax = 25;
	m_gen = 0;
	

	if( m_numbering == SZLUHA ) // orderd list
	{
		m_tag1 = L"<ol>";
		m_tag2 = L"</ol>";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>";
	}

	// az els� ember a treePeople, treeTables-ben ker�l bet�lt�sre 
	while( true )
	{

		if( !vDesc.at( m_gen ).hidden )
		{
			if( m_gen > generationsMax )
			{
				for( int i =0; i < m_indent; ++i )
					fwprintf( fl, L"%s\n", m_tag2 );
				str.Format( L"<br><br><font color='red'>%d-n�l t�bb gener�ci�t �rt ki, m�r kezelhetetlen, ez�rt abbahagyom!!!</font>", generationsMax );
				print( str );
				break;
			}
			printGAline();	// a vDesc tetej�n l�v� lesz�rmazottat kinyomttajuk  m_genPrev-et be�ll�tja
		}

		
		if( vDesc.at( m_gen ).sex_id == WOMAN && !m_woman )		// ha n� a lesz�rmazott �s annak a gyerekeit nem akarjuk list�zni
		{
			vDesc.at( m_gen ).numOfChildren		= 0;
			vDesc.at( m_gen ).childrenPrinted	= 0;
		}

		if( vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )	// m�r minden gyerek�t ki�rta
		{
			//visszamegy az el�z� gener�ci�ra, amelyiknak m�g van ki nem �rt gyereke
			while( m_gen >= 0 && vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )
			{
				vDesc.pop_back();		// visszamegy az el�z� gener�ci�ra
				--m_gen;
			}
		}
		if( m_gen < 0 ) break;			// nincs tov�bb!!!

		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		rowid = getNextChildRowid();
		if( !rowid.IsEmpty() )			// van m�g gyerek
		{
			queryP( rowid );		// lek�rdezi a gyereket �s be�ll�tja m_sex_id-t �s m_numOfSpouses-t
			desc.rowid				= rowid;
			desc.sex_id				= m_sex_id;
			desc.numOfSpouses		= m_numOfSpouses;				// a gyerek apja feles�geinek sz�m�t meg�rzi 
			desc.childrenPrinted	= 0;
			desc.parentIndexLast	= 0;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( rowid, m_sex_id );

			vDesc.at( m_gen ).childrenPrinted += 1;							// a most kinyomtatand� testv�rek sz�ma n� egyet

			vDesc.push_back( desc );
			++m_gen;

			// Tupigny-h�z el��ll�tja a gener�ci�n bel��l egyes�vel n�vekv� sorsz�mokat
			if( m_gen < vSerial.size() ) ++vSerial.at( m_gen );			// m�r l�tez� gener�ci�, 1-el n�veli a sorsz�mot	
			else	vSerial.push_back( 1 );								// �j gener�ci�: sorsz�m1-el kezd�dik 
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::queryP( CString rowid )
{
	m_command.Format( L"SELECT generation, sex_id FROM people WHERE rowid ='%s'", rowid );
	if( !query( m_command ) ) return;

	m_sex_id = _wtoi( m_recordset.GetFieldString( 1 ) );

	if( m_sex_id == MAN )
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query4( m_command ) ) return;
	m_numOfSpouses = _wtoi( m_recordset4.GetFieldString( 0 ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CGaDescendants::getNumberOfChildren( CString rowid, int sex_id  )
{
	if( sex_id == MAN )
	{
		if( m_connect )
			m_command.Format( L"SELECT count() FROM people WHERE father_id='%s'", rowid );
		else
			m_command.Format( L"SELECT count() FROM people WHERE father_id='%s' AND tableNumber='%s'", rowid, m_tableNumber );
	}
	else
	{
		if( m_connect )
			m_command.Format( L"SELECT count() FROM people WHERE mother_id='%s'", rowid );
		else
			m_command.Format( L"SELECT count() FROM people WHERE mother_id='%s' AND tableNumber='%s'", rowid, m_tableNumber );
	}


	if( !query( m_command ) ) return 0;
	return( _wtoi( m_recordset.GetFieldString(0) ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CGaDescendants::getNextChildRowid()
{
	CString parent_id	= vDesc.at( m_gen ).rowid;
	UINT	ixChild		= vDesc.at( m_gen ).childrenPrinted + 1;	// lesz�rmazott ixChild-adik gyerek�t keresi  
	int		sex_id		= vDesc.at( m_gen ).sex_id;

	if( sex_id == MAN )
	{
		if( m_connect )
			m_command.Format( L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY orderFather", parent_id );
		else
			m_command.Format( L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY orderFather", parent_id, m_tableNumber );
	}
	else
	{
		if( m_connect )
			m_command.Format( L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY orderMother", parent_id );
		else
			m_command.Format( L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY orderMother", parent_id, m_tableNumber );
	}

	if( !query( m_command ) ) return L"";
	if( m_recordset.RecordsCount() < ixChild )
	{
		str.Format( L"%s ap�nak csak %d gyereke van, te pedig a %d.-kat k�red!!", parent_id, m_recordset.RecordsCount(), ixChild );
		AfxMessageBox( str );
		return L"";
	}

	m_recordset.MoveTo( ixChild - 1 );
	return( m_recordset.GetFieldString( 0 ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// B E � L L � T � S O K //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedRadioClear()  // fekete sz�n be�ll�t�sa
{
	m_setCombo = 0;
	m_ComboName.SetCurSel( 0 );
	m_ComboSpec.SetCurSel( 0 );
	m_ComboComm.SetCurSel( 0 );
	m_ComboFamily.SetCurSel(0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnRadioDefault()		// default sz�n be�ll�t�sa
{
	m_setCombo = 1;
	m_ComboName.SetCurSel( 1 );
	m_ComboSpec.SetCurSel( 0 );
	m_ComboComm.SetCurSel( 2 );
	m_ComboFamily.SetCurSel(3);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
void CGaDescendants::OnClickedButtonDefault()
{
	OnRadioDefault();

	m_woman			= false;
	m_connect		= true;
	m_checkFamily	= true;
	m_CheckLastName = false;
	m_numbering		= 2;
	m_wrap			= false;
	UpdateData( TOSCREEN );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckWoman()
{
	m_woman	= !m_woman;				// ha a n�k cgyerekeit is list�zni akarjuk, akkor a t�bl�kat is �ssze kell k�tni!!
	if( m_woman )
		m_connect = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckConnect()
{
	if( m_woman )					// ha n�k lszr�mazoittait is list�zzuk, akkor mindenk�ppen �sszek�t�s kell
		m_connect = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckFamily()
{
	m_checkFamily = ! m_checkFamily;	// az�rt kel, hogy OnClickedCheckWoman UpdateData-ja a helyes �rt�ket �ll�tsa be
	if( m_checkFamily )
	{
		GetDlgItem( IDC_STATIC_FAMILY )->EnableWindow( true );
		GetDlgItem( IDC_COMBO_FAMILY )->EnableWindow( true );
	}
	else
	{
		GetDlgItem( IDC_STATIC_FAMILY )->EnableWindow( false );
		GetDlgItem( IDC_COMBO_FAMILY )->EnableWindow( false );
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckLastname()
{
	m_CheckLastName = !m_CheckLastName;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedSzluha()
{
	m_numbering = SZLUHA;
}
void CGaDescendants::OnVillers()
{
	m_numbering = VIL;
}
void CGaDescendants::OnTupigny()
{
	m_numbering = TUP;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedStaticBackground()
{
	CMFCColorDialog dlgColors;
	dlgColors.SetCurrentColor( WHITE );
	if( dlgColors.DoModal() == IDCANCEL ) return;

	COLORREF bgn = dlgColors.GetColor();
	colorBgn.SetColor( BLACK, bgn );

	m_colorBgrnd = GetRValue( bgn ) << 16 | GetGValue( bgn ) << 8 | GetBValue( bgn );
	return;
}
