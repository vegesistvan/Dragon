// GaDescendants2.cpp : implementation file
//
// Ha nõk leszármazottait is listázzuk, akkor némely esetben olyan hatalmas leszármazotti lista keletkezne, ami 
// olyan hatást kelt, hogy legagyna a program. Lehet, hogy le is fagy, vagyis a html fájl összetört sorokat tartalmaz,
// kezelhetetlen az egész


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
	,m_connect(true)		// táblákat összekösse-e
	,m_woman(false)			// nõk leszármazottait listázza-e
	,m_setCombo(1)			// 0= nincsenek kiemelések, 1= defeault kiemelések 

	,m_ixName( 1 )			// név bold
	,m_ixSpec( 0 )			// speciális karakterek (*+=) bold
	,m_ixComment( 2 )		// comment bold
	,m_ixFamily( 3 )
	,m_ixFontSize( 3 )

	,m_rowid1(L"")			// a leszármazott rowid-ja
	,m_name(L"")			// leszármazott õs neve
	,m_tableNumber(L"")		// tablenumber, ha a táblázat leszármazotti listáját kérjük
	,m_CheckLastName(true)	// családnév kiírása
	,m_code(true)			// ANSI vagy UTF8 kódrendszer
	,m_numbering(TUP)		// milyen számozási rendszer legyen (0,1,2) SZLUHA/VIL/TUP
	,m_checkFamily(false)	// %%% családnév,elõnév kiemelése
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
	
	file.Format( L"%s leszármazottai", m_name);
	title.Format( L"%s leszármazottai", m_name );
	
	openHtml( file, title, m_colorBgrnd );

	if( theApp.m_inputMode == GAHTML && !m_checkFamily )
	{
		m_command.Format( L"SELECT tableHeader from tables WHERE rowid = %s", m_tableNumber );
		if( !query4( m_command ) ) return;
		tableHeader = m_recordset4.GetFieldString( 0 );
		str.Format( L"<b>%s</b>\n", tableHeader );
		print( str );
	}


	queryP( m_rowid1 );								// m_sex_id-t és m_numOfSpouses-t határozza meg
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

	m_genPrev	= 0;		// elõzõ kiírt generáció
	cnt_ol		= 0;		// a behúzások pillanatny száma

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
			file.Format( L"%s és további %d tábla", familyName, theApp.v_tableNumbers.size() - 1 );
		else
			file.Format( L"%s %s és további %d tábla", familyName, tableRoman, theApp.v_tableNumbers.size() - 1 );

	}
	else
	{
		if( tableRoman.IsEmpty() )
			file.Format( L"%s tábla", familyName );
		else
			file.Format( L"%s %s tábla", familyName, tableRoman );
	}
	title = file;
	openHtml( file, title, m_colorBgrnd );



	CProgressWnd wndProgress(NULL,L"Leszármazotti táblák készítése folyik...." ); 
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
			str.Format( L"%s táblában nincs õs!!", m_tableNumber );
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
		cnt_ol		= 0;
		m_genPrev	= 0;

		if( father_id.IsEmpty() || !father_id.Compare( L"0" ) )  // ha nincs apa, akkor magát az õst teszi be a vDesc-be
		{
			p.generation = m_recordset1.GetFieldString( 3 );
			m_rowid1 = m_recordset.GetFieldString( 0 );
			m_sex_id = _wtoi( m_recordset.GetFieldString( 2 ) );

			desc.sex_id				= m_sex_id;
			desc.rowid				= m_rowid1;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( m_rowid1, m_sex_id );

		}
		else		// ha van apja az õsnek, akkor az apát hidden-ként elteszi, hogy ha több gyereke van, akkor mind listázza 
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
			for( int i =0; i < cnt_ol; ++i )
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
// A kinyomtatandó leszármazot azonosítóit a <DESCENDANTS> vDesc vektorba töltjük fel, 
// és annak felhasználásával nyomtatjuk ki õket.
// Mindaddig abban maradnak, amíg az összes gyerekük kinyomtatásra kerül.Akkor töröljük öt a vektorból
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

	// az elsõ ember a treePeople, treeTables-ben kerül betöltésre 
	while( true )
	{

		if( !vDesc.at( m_gen ).hidden )
		{
			if( m_gen > generationsMax )
			{
				for( int i =0; i < cnt_ol; ++i )
					fwprintf( fl, L"%s\n", m_tag2 );
				str.Format( L"<br><br><font color='red'>%d-nál több generációt írt ki, már kezelhetetlen, ezért abbahagyom!!!</font>", generationsMax );
				print( str );
				break;
			}
			printGAline();	// a vDesc tetején lévõ leszármazottat kinyomttajuk  m_genPrev-et beállítja
		}

		
		if( vDesc.at( m_gen ).sex_id == WOMAN && !m_woman )		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
		{
			vDesc.at( m_gen ).numOfChildren		= 0;
			vDesc.at( m_gen ).childrenPrinted	= 0;
		}

		if( vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )	// már minden gyerekét kiírta
		{
			//visszamegy az elõzõ generációra, amelyiknak még van ki nem írt gyereke
			while( m_gen >= 0 && vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )
			{
				vDesc.pop_back();		// visszamegy az elõzõ generációra
				--m_gen;
			}
		}
		if( m_gen < 0 ) break;

		// a kinyomtatott ember következõ, még ki nem nyomtatott gyerekét keresi
		rowid = getNextChildRowid();
		if( !rowid.IsEmpty() )			// van még gyerek
		{
			queryP( rowid );		// lekérdezi a gyereket és beállítja m_sex_id-t és m_numOfSpouses-t
			desc.rowid				= rowid;
			desc.sex_id				= m_sex_id;
			desc.numOfSpouses		= m_numOfSpouses;				// a gyerek apja feleségeinek számát megõrzi 
			desc.childrenPrinted	= 0;
			desc.parentIndexLast	= 0;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( rowid, m_sex_id );

			vDesc.at( m_gen ).childrenPrinted += 1;							// a most kinyomtatandó testvérek száma nõ egyet

			vDesc.push_back( desc );
			++m_gen;

			// Tupigny-hÖz elõállítja a generáción belüõl egyesével növekvõ sorszámokat
			if( m_gen < vSerial.size() ) ++vSerial.at( m_gen );			// már létezõ generáció, 1-el növeli a sorszámot	
			else	vSerial.push_back( 1 );								// új generáció: sorszám1-el kezdõdik 
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
	UINT	ixChild		= vDesc.at( m_gen ).childrenPrinted + 1;	// leszármazott ixChild-adik gyerekét keresi  
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
		str.Format( L"%s apának csak %d gyereke van, te pedig a %d.-kat kéred!!", parent_id, m_recordset.RecordsCount(), ixChild );
		AfxMessageBox( str );
		return L"";
	}

	m_recordset.MoveTo( ixChild - 1 );
	return( m_recordset.GetFieldString( 0 ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// B E Á L L Í T Á S O K //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedRadioClear()  // fekete szín beállítása
{
	m_setCombo = 0;
	m_ComboName.SetCurSel( 0 );
	m_ComboSpec.SetCurSel( 0 );
	m_ComboComm.SetCurSel( 0 );
	m_ComboFamily.SetCurSel(0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnRadioDefault()		// default szín beállítása
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
	m_woman	= !m_woman;				// ha a nõk cgyerekeit is listázni akarjuk, akkor a táblákat is össze kell kötni!!
	if( m_woman )
		m_connect = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckConnect()
{
	if( m_woman )					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
		m_connect = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckFamily()
{
	m_checkFamily = ! m_checkFamily;	// azért kel, hogy OnClickedCheckWoman UpdateData-ja a helyes értéket állítsa be
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
