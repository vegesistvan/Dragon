// GaDescendants2.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GA_descendants.h"
#include "GA_structures.h"
#include <winuser.h>


SZIN szin[] =
{ 
	{ L"white",			0XFFFFFF }, 
	{ L"light yellow",	0XFFFFE0 },
	{ L"aquamarine",	0X7FFFD4 },
	{ L"cian",			0X00FFFF },
	{ L"deepskyblue",	0X00BFFF },
	{ L"lavenderBlush",	0XFFF0F5 },
	{ L"mistyRose",		0XFFE4E1 },
	{ L"bisque",		0XFFE4C4 },
	{ L"thistle",		0XD8BFD8 }, 
	{ L"aqua",			0X00FFFF },
	{ L"moccasin",		0XFFE4B5 }, 
};
const UINT szinSize = sizeof( szin ) / sizeof( SZIN );
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGaDescendants, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGaDescendants::CGaDescendants(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGaDescendants::IDD, pParent)
	, m_connect(FALSE)
	, m_woman(FALSE)
	,m_ixName( 1 )
	,m_ixSpec( 0 )
	,m_ixComment( 2 )
	,m_setCombo(1)
	,m_rowid1(L"")
	,m_name(L"")
	,m_tableNumber(L"")
	, m_CheckLastName(FALSE)
	, m_code(FALSE)
	, m_generateGenCode(FALSE)
	, m_numbering(0)
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
	DDX_Control(pDX, IDC_COMBO_SPEC, m_ComboSpec);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_connect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, m_woman);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_ComboComm);
	DDX_Control(pDX, IDC_COMBO_NAME, m_ComboName);
	DDX_Radio(pDX, IDC_RADIO_CLEAR, m_setCombo);
	DDX_Check(pDX, IDC_CHECK_LASTNAME, m_CheckLastName);
	DDX_Radio(pDX, IDC_ANSI, m_code);
	DDX_Control(pDX, IDC_COMBO_BGRD, m_ComboBgrd);
	DDX_Radio(pDX, IDC_RADIO_GENERATION, m_generateGenCode);
	DDX_Radio(pDX, IDC_OLD, m_numbering);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGaDescendants, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGaDescendants::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_CLEAR, &CGaDescendants::OnClickedRadioClear)
	ON_COMMAND(IDC_RADIO_DEFAULT, &CGaDescendants::OnRadioDefault)
	ON_WM_CTLCOLOR()
	ON_MESSAGE( WM_CTLCOLORBTN, OnCtlColorBtn )
	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CGaDescendants::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CGaDescendants::OnClickedCheckConnect)
	ON_BN_CLICKED(IDC_RADIO_GENERATION, &CGaDescendants::OnClickedRadioGeneration)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGaDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for( int i = 0; i < sizeof( attrib ) / sizeof( ATTRIB ); ++i )
	{
		m_ComboName.AddString( attrib[i].text );
		m_ComboSpec.AddString( attrib[i].text );
		m_ComboComm.AddString( attrib[i].text );
	}
	m_ComboName.SetCurSel( m_ixName );
	m_ComboComm.SetCurSel( m_ixComment );
	m_ComboSpec.SetCurSel( m_ixSpec );

	for( int i = 0; i < szinSize; ++i )
	{
		m_ComboBgrd.AddString( szin[i].name );
	}
	m_ComboBgrd.SetCurSel( 0 );

	if( theApp.m_inputMode == MANUAL )
	{
		m_connect	= true;
		m_woman		= true;
	}

	m_generateGenCode = false;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::treePeople()
{
	CString file;
	CString title;
	CString table;
	TCHAR* kicsoda[] = { L"leszármazott", L"leszármazott házastársa", L"Leszármazott házastársának apja", L"leszármazott házastársának anyja", L"leszármazott házastársának továnbbi házastársa" };

	
//	if( m_unordered == 0 )  // orderd list
	if( m_numbering == OLD ) // orderd list
	{
		m_tag1 = L"<ol>";
		m_tag2 = L"</ol>";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>";
	}

	file.Format( L"%s leszármazottai", m_name);
	title.Format( L"%s<br>\n", file );

	if( theApp.m_inputMode == GAHTML )
	{
		m_command.Format( L"SELECT tableHeader from tables WHERE rowid = %s", m_tableNumber );
		if( !query4( m_command ) ) return;
		table = m_recordset4.GetFieldString( 0 );

		title.Format( L"<b>%s ( %s a %s táblából)</b ><br>\n", file, kicsoda[m_source-1], table );
		title.Format( L"%s<br>\n", file );
	}
	
	openHtml( file, title, m_colorBgrnd );

	str.Format( L"%s\n", table );
	print( str );

	queryP( m_rowid1 );
	desc.gen				= 0;
	desc.rowid				= m_rowid1;
	desc.sex_id				= m_sex_id;
	desc.numOfSpouses		= m_numOfSpouses;
	desc.numOfChildren		= getNumberOfChildren( m_rowid1, m_sex_id );
	desc.childrenPrinted	= 0;
	desc.children			= 1;
	desc.hidden				= false;

	vDesc.clear();
	vDesc.push_back( desc );

//	vDesc.at(0).numOfChildren = getNumberOfChildren( m_rowid1, m_sex_id );

	m_genPrev	= 0;
	cnt_ol		= 0;

	descendants();
	closeHtml();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::treeTables()
{
	CString file;
	CString title(L"");
	CString last_name;
	CString family;
	CString fileNumber(L"");
	CString tableHeader;
	CString familyName;
	CString father_id;

	if( m_numbering == OLD || m_numbering == VIL )
	{
		m_tag1 = L"<ol list-style-type: circle>";
		m_tag2 = L"</ol>";
	}
	else
{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>";
	}

	if( theApp.v_tableNumbers.size() == 1 )
	{
		m_command.Format( L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%d'", theApp.v_tableNumbers.at(0) );
		if( !query( m_command ) ) return;
		familyName = m_recordset.GetFieldString( 0 );
		tableHeader = m_recordset.GetFieldString( 1 );
		title.Format( L"%s tábla leszármazottai<br>", tableHeader );
		file.Format( L"%s %s tábla leszármazottai", familyName, m_recordset.GetFieldString( 2 ) );
		title.Empty();
	}
	else
	{
		file.Format( L"%d tábla leszármazottai", theApp.v_tableNumbers.size() ); 
		title.Format( L"%s<br><br>\n", file );
	}
	openHtml( file, title, m_colorBgrnd );


	CProgressWnd wndProgress(NULL,L"Leszármazotti táblák készítése folyik...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange( 0, theApp.v_tableNumbers.size() );
	wndProgress.SetPos( 0 );

	for( UINT i = 0; i < theApp.v_tableNumbers.size(); ++i )
	{
		m_tableNumber.Format( L"%d", theApp.v_tableNumbers.at(i) );

		m_command.Format( L"SELECT tableHeader FROM tables WHERE rowid ='%d'", theApp.v_tableNumbers.at(i) );
		if( !query( m_command ) ) return;
		tableHeader = m_recordset.GetFieldString( 0 );
		str.Format( L"%s tábla leszármazottai<br><br>", tableHeader );
		print( str );

		m_command.Format( L"SELECT rowid, last_name, sex_id, father_id FROM people WHERE tableNumber = '%d' AND tableAncestry == 1 ", theApp.v_tableNumbers.at(i) );
		if( ! query( m_command ) ) return;

		if( !m_recordset.RecordsCount() )
		{
			str.Format( L"%d táblában nincs õs!!", theApp.v_tableNumbers.at(i) );
			AfxMessageBox( str );
			break;
		}

		father_id	= m_recordset.GetFieldString( 3 );
		if( father_id.IsEmpty() || !father_id.Compare( L"0" ) )  // ha nincs apa, akkor magát az õst teszi be a vDesc-be
		{
			p.generation = m_recordset1.GetFieldString( 3 );

			vDesc.clear();
			cnt_ol = 0;
			m_genPrev = 0;
			
			m_rowid1 = m_recordset.GetFieldString( 0 );
			m_sex_id = _wtoi( m_recordset.GetFieldString( 2 ) );

			desc.gen				= 0;
			desc.rowid				= m_rowid1;
			desc.sex_id				= m_sex_id;
			desc.childrenPrinted	= 0;
			desc.children			= 0;
			desc.numOfSpouses			= 0;
			desc.hidden				= false;
			vDesc.push_back( desc );

			vDesc.at(0).numOfChildren = getNumberOfChildren( m_rowid1, m_sex_id );
		}
		else		// ha van apja az õsnek, akkor az apát hidden-ként elteszi, hogy ha több gyereke van, akkor mind listázza 
		{
			m_command.Format( L"SELECT rowid, last_name, sex_id, generation FROM people WHERE father_id='%s'", father_id );
			if( ! query1( m_command ) ) return;

			p.generation = m_recordset1.GetFieldString( 3 );
			vDesc.clear();
			cnt_ol = 0;
			m_genPrev = 0;
			
			m_sex_id = 1;
			desc.gen				= 0;
			desc.rowid				= father_id;
			desc.sex_id				= m_sex_id;
			desc.numOfSpouses			= 0;
			desc.childrenPrinted	= 0;
			desc.children			= 1;
			desc.hidden				= true;
			vDesc.push_back( desc );

			vDesc.at(0).numOfChildren = getNumberOfChildren( father_id, m_sex_id );
		}
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
// A kinyomtatandó leszármazot azonosítóit a vDesc vektorba töltjük fel, és annak felhasználásával nyomtatjuk ki õket.
// Mindaddig abban maradnak, amíg az összes gyerekük kinyomtatásra kerül.Akkor töröljük öt a vektorból

void CGaDescendants::descendants()
{
	CString rowid;
	int ix = 0;
	
	// az elsõ emeber a treePeople, treeTables-ben kerül betöltésre 

	while( vDesc.size() )
	{
		if( !vDesc.at(ix).hidden	)	printGAline( ix );	// a leszármazottat kinyomttajuk
		
		if( vDesc.at(ix).sex_id == WOMAN && !m_woman )		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
		{
			vDesc.at(ix).numOfChildren		= 0;
			vDesc.at(ix).childrenPrinted	= 0;
		}

		if( vDesc.at( ix ).numOfChildren > vDesc.at( ix ).childrenPrinted )	// ha még nem nyomtatott ki minden gyereket
		{																	// akkor a következõ gyereket is beteszi  
			// a kinyomtatott ember következõ, még ki nem nyomtatott gyerekét keresi
			rowid = getNextChildRowid( ix );
			if( !rowid.IsEmpty() )
			{
				queryP( rowid );		// lekérdezi a gyereket és elkészíti vDesc-ét
				desc.gen				= vDesc.size();
				desc.rowid				= rowid;
				desc.sex_id				= m_sex_id;
				desc.numOfSpouses		= m_numOfSpouses;
				desc.numOfMothers		= vDesc.at(ix).numOfSpouses;
				desc.childrenPrinted	= 0;
				desc.children			= vDesc.at(ix).childrenPrinted + 1;
				desc.hidden				= false;

				vDesc.at(ix).childrenPrinted += 1;

				vDesc.push_back( desc );
				ix += 1;
				vDesc.at( vDesc.size() - 1 ).numOfChildren = getNumberOfChildren( rowid, m_sex_id );
			}
			else			// ha a gyerekek száma kisebb, mint amit kérünk. Csak hiba lehet
			{
				vDesc.clear();
				AfxMessageBox( L"Hiba!" );
			}
		}
		else   // nincs több gyereke, de a korábbiaknak lehet még!!
		{
			//visszamegy az elõzõ generációra, amelyiknak még van ki nem írt gyereke
			while( ix >= 0 && vDesc.at( ix ).numOfChildren == vDesc.at( ix ).childrenPrinted )
			{
				vDesc.pop_back();		// visszamegy az elõzõ generációra
				ix -= 1;
			}

			if( ix >= 0 && vDesc.at( ix ).numOfChildren > vDesc.at( ix ).childrenPrinted )  // van még kiirandó gyerek?
			{
				rowid = getNextChildRowid( ix );  // következõ gyerek
				queryP( rowid );

				desc.gen				= vDesc.size();
				desc.rowid				= rowid;
				desc.sex_id				= m_sex_id;
				desc.numOfSpouses		= m_numOfSpouses;
				desc.numOfMothers		= vDesc.at(ix).numOfSpouses;
				desc.childrenPrinted	= 0;
				desc.hidden				= false;
				desc.children			= vDesc.at(ix).childrenPrinted + 1;
				vDesc.at(ix).childrenPrinted += 1;

				vDesc.push_back( desc );
				ix += 1;
				vDesc.at( vDesc.size() - 1 ).numOfChildren = getNumberOfChildren( rowid, m_sex_id );
			}
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
CString CGaDescendants::getNextChildRowid( UINT ix )
{
	CString parent_id	= vDesc.at(ix).rowid;
	UINT	ixChild		= vDesc.at( ix ).childrenPrinted + 1;	// leszármazott ixChild-adik gyerekét keresi  
	int		sex_id		= vDesc.at(ix).sex_id;

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
/*
HBRUSH CGaDescendants::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;

	HBRUSH m_brushBack; 

	HBRUSH hBrush = NULL;
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		// just return a not NULL brush handle
//		hBrush = (HBRUSH)m_brushBack;
		break;
	case CTLCOLOR_STATIC:
	{
		switch (pWnd->GetDlgCtrlID())  
		{
			case IDC_YELLOW:
			  pDC->SetBkColor( YELLOW );
			  break;
		}
//		pDC->SetBkColor( YELLOW );
//		pDC->SetBkMode(TRANSPARENT);
	//	hBrush = (HBRUSH)m_brushBack;
	}
	break;
	default:
		// do the default processing
		hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	}
	return hBrush;


	return hbr;

}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CGaDescendants::OnCtlColorBtn( WPARAM wparam, LPARAM lparam )
{
	HDC pDC = (HDC)wparam;

	HWND hand = (HWND)lparam;



	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedRadioClear()  // fekete szín beállítása
{
	m_setCombo = 0;
	m_ComboName.SetCurSel( 0 );
	m_ComboSpec.SetCurSel( 0 );
	m_ComboComm.SetCurSel( 0 );
	m_ComboBgrd.SetCurSel( 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnRadioDefault()		// default szín beállítása
{
	m_setCombo = 1;
	m_ComboName.SetCurSel( 1 );
	m_ComboSpec.SetCurSel( 0 );
	m_ComboComm.SetCurSel( 2 );
	m_ComboBgrd.SetCurSel( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckWoman()
{
	m_woman				= true;
	m_generateGenCode	= true;
	m_connect			= true;
	UpdateData(TOSCREEN);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedCheckConnect()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnClickedRadioGeneration()
{


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGaDescendants::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	m_ixName	= m_ComboName.GetCurSel();
	m_ixSpec	= m_ComboSpec.GetCurSel();
	m_ixComment = m_ComboComm.GetCurSel();
	m_ixBgrd	= m_ComboBgrd.GetCurSel();
	m_colorBgrnd = szin[m_ixBgrd].rgb;

	if( m_woman ) m_connect = true;

	if( m_rowid1.IsEmpty() )
		treeTables();
	else
		treePeople();

	CDialogEx::OnOK();
}
