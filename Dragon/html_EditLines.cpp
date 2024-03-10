// HtmlEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_EditLines.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "EditItem.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Az MFC Dynamic Layout-al  Debug módban lerobban a program, ezért EasySize-al történik az átméretezés.
//  Egyszerûbb dialog ablakoknál mûködik DEbug módban is.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implicit vagy explicit megadott GA.html sorokat listázza.
// Bármelyik sorra duplakattintással a sort a szerkesztõ ablakba helyezi. Ott szerkeszthetjük, majd visszaírhatjuk a ga-html fájlba.
// Ezt megismételhetjük bármelyik sorral, akár hányszor.
// A "Kilép" gombra kattintva léphetünk ki.
//
// Public vátozók:
//
// CString m_type			implicit megadott sorok listázása
//							F_SIBLINGS		- az m_rowid-val megadott ember apját és testvéreit listázza
//							FATHERMOTHERHE	- az m_rowid-val megadott emeber szüleit és testvéreit listázza
//
// vector<int>*vLines		a listázandó sorok sorszámát tartalmazó vektor, ha nincs m_type megadva
//
// CString m_title			az ablak felirat

IMPLEMENT_DYNAMIC(CHtmlEditLines, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLines::CHtmlEditLines(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlEditLines::IDD, pParent)
{
	m_linenumber = 0;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLines::~CHtmlEditLines()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlEditLines, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlEditLines::OnDblclkList)
	ON_COMMAND(ID_LISTA, &CHtmlEditLines::OnListLines)
	ON_BN_CLICKED(IDC_MODIFY, &CHtmlEditLines::OnClickedModify)
	ON_EN_CHANGE(IDC_EDIT, &CHtmlEditLines::OnChangeEdit)
	ON_COMMAND(ID_INFO, &CHtmlEditLines::OnInfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CHtmlEditLines::OnClickList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlEditLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	IDC_LIST,		ES_BORDER,		ES_BORDER,		0 );
	EASYSIZE_ADD( IDC_STATIC_GYUJTO,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_STATIC_EDITBOX,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );

	EASYSIZE_ADD( IDC_MODIFY,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()


	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	

	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );

	if( m_type == L"F_SIBLINGS" )
	{
		fatherAndSiblings();
		return true;
	}
	if( m_type == L"F_CHILDREN" )
	{
		children();
		return true;
	}
	if( m_linenumber )
	{
		displayLine();
		return true;
	}

	if( vLines->size() )
	{
		displayLines();
		return true;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CHtmlEditLines::getHtmlLine( CString lineNumber )
{
	if( lineNumber.IsEmpty() ) return L"";
	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );
	CStdioFile file( theApp.m_htmlPathName, CFile::modeRead); 
	
	CString cLine;
	int ln = _wtoi( lineNumber );

	for( int i = 0; i < ln - 2; ++i ) file.ReadString( cLine );  // elõzõ sor elé megy
	file.ReadString( cLine );											// elõzõ sor

	file.ReadString( cLine );											// kérdéses sor
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	
	cLine = cleanHtmlLine( cLine );
	file.Close();
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CHtmlEditLines::cleanHtmlLine( CString cLine )
{
	CString htmlLine = cLine;
	int		pos;
	TCHAR	gen;

	if( ( pos = cLine.ReverseFind('>') ) != -1 )
	{
		cLine = cLine.Mid( pos+1 );
		gen = cLine[0];
		if( ( pos = cLine.ReverseFind(';') ) != -1 )
		{
			cLine = cLine.Mid( pos+1 );
			htmlLine.Format( L"%c %s", gen, cLine ); 
		}
	}
	return htmlLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nItem			= pNMItemActivate->iItem;
	CString line;

	m_linenumber = _wtoi( m_ListCtrl.GetItemText( m_nItem, 1 ) );
	line = m_ListCtrl.GetItemText( m_nItem, 2 );
	line = line.Mid( 2 );		// geerációs kód eldobása

	GetDlgItem( IDC_EDIT )->SetWindowTextW( line );

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnListLines()
{
	CString	logFile(L"problémás_html_Sorok"); 
	CString	title = logFile;
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnClickedModify()
{
	GetDlgItem( IDC_EDIT )->GetWindowTextW( m_line );

	theApp.saveHtmlLine( m_linenumber, m_line );

	m_ListCtrl.SetItemText( m_nItem, 2, m_line );
	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( L"" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnChangeEdit()
{
	GetDlgItem( IDC_MODIFY )->EnableWindow( TRUE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::children()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;

	if( !theApp.m_recordset->RecordsCount() )
	{
		str.Format( L"Nincs %s rowid-jû ember az adatbázisban!", m_rowid );
		AfxMessageBox( str );
		CDialog::OnCancel();
		return;
	}

	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"szülõk" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' OR mother_id='%s' ORDER BY linenumber", m_rowid, m_rowid  );
	if( !theApp.query( m_command ) ) return;

	for( INT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		linenumber	= theApp.m_recordset->GetFieldString( 0 );
		line		= getHtmlLine( linenumber );
		nItem = m_ListCtrl.InsertItem( i+1, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumber );
		m_ListCtrl.SetItemText( nItem, 2, line );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::fatherAndSiblings()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;
	CString linenumberC;

	m_command.Format( L"SELECT father_id, linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	father_id	= theApp.m_recordset->GetFieldString( 0 );
	if( father_id.IsEmpty() || father_id == L"0" )
	{
		AfxMessageBox( L"Nincs apja" );
		CDialog::OnCancel();
		return;
	}

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);


	CProgressWnd wndP(NULL, L"Keresem a szülõket és testvéreket..." ); 
	wndP.GoModal();

	linenumberC	= theApp.m_recordset->GetFieldString( 1 );

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	
	
	nItem = m_ListCtrl.InsertItem( 0, L"szülõk" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' ORDER BY linenumber", father_id );
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange( 0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( INT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		linenumber	= theApp.m_recordset->GetFieldString( 0 );
		line		= getHtmlLine( linenumber );
		nItem = m_ListCtrl.InsertItem( 2+i, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumber );
		m_ListCtrl.SetItemText( nItem, 2, line );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			CDialog::OnCancel();
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::displayLine( )
{
	CString linenumber;
	CString line;

	linenumber.Format( L"%d", m_linenumber ); 
	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);
	
	line = getHtmlLine( linenumber );

	m_ListCtrl.InsertItem( 0, L"" );
	m_ListCtrl.SetItemText( 0, 1, linenumber );
	m_ListCtrl.SetItemText( 0, 2, line );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::displayLines( )
{
	int nItem;
	CString line;

	CProgressWnd wndP(NULL, L"Keresem a kijelölt sorokat..." ); 
	wndP.GoModal();
	wndP.SetRange( 0, vLines->size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	for( UINT i = 0; i < vLines->size(); ++ i )
	{
		nItem = m_ListCtrl.InsertItem( i, L"" );
		m_ListCtrl.SetItemText( nItem, 1, vLines->at(i) );
		line = getHtmlLine( vLines->at(i) );
		m_ListCtrl.SetItemText( nItem, 2, line );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	SetWindowTextW( m_title );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnInfo()
{
	CString info = L"\
A GA.html fájl kijelölt sorait a gyûjtõ ablakba olvassa be. A bal egérgombbal 1x ráklikkelve egy sorra, \
a sort áthelyezhetjük a szerkesztõ ablakba.\r\n\r\n\
A szerkesztõ ablakban módosíthatjuk a sort. A gyûjtõ ablakban 2x kattintva a bal egérgombbal, a sor \
szerkeszthetõvé válik, de csak arra használjuk, hogy Ctrl-C, Ctrl-V-vel áthelyezhetünk szövegrészeket \
a szerkesztendõ sorba.\r\n\
A \"Módosít\" gomb csak a szerkesztõ ablakban lévõ sort írja vissza  a GA.htm fájlba!!!\r\n\r\n\
A \"Módosít\" gomb visszateszi a módosított sort a fájlba és a gyûjtõ ablakba is.\r\n\
Ha szükséges, a fenti módon folytathatjuk a szerkesztését egy másik sorral.\
";
	AfxMessageBox( info, MB_ICONINFORMATION );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnLista()
{




}
