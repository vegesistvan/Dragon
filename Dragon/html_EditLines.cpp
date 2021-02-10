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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlEditLines::OnDblclkList)
	ON_COMMAND(ID_LIST_LINES, &CHtmlEditLines::OnListLines)
	ON_BN_CLICKED(IDC_MODIFY, &CHtmlEditLines::OnClickedModify)
	ON_EN_CHANGE(IDC_EDIT, &CHtmlEditLines::OnChangeEdit)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlEditLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	IDC_LIST,		ES_BORDER,		ES_BORDER,		0 );

	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()

/*	
	int		nItem = 0;
	CString parentsL;
	CString line;
	CString caption;
	CString cLine;
	CString htmlLine(L"");
	CString command;
	CString father_id;
	CString mother_id;
	CString grandpa_id;
	CString linenumberP;
	CString linenumberF;
	CString linenumberGP;
	CString lineGP;
	CString lineF;
	CString lineP;

	CString linenumber;
	std::vector<CString> vLinenumbers;
*/
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	

	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );

	if( m_type == L"F_SIBLINGS" )
	{
		fatherAndSiblings();
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

/*
	if( !m_rowid.IsEmpty() )
	{
		m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
		m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
		m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

		command.Format( L"SELECT linenumber, father_id, mother_id FROM people WHERE rowid ='%s'", m_rowid );
		if( !theApp.query( command ) ) return false;
		linenumberP = theApp.m_recordset->GetFieldString( 0 );
		father_id	= theApp.m_recordset->GetFieldString( 1 );
		mother_id	= theApp.m_recordset->GetFieldString( 2 );
		
		command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
		if( !theApp.query( command ) ) return false;
		linenumberF = theApp.m_recordset->GetFieldString( 0 );

		command.Format( L"SELECT father_id FROM people WHERE rowid ='%s'", mother_id );
		if( !theApp.query( command ) ) return false;
		grandpa_id = theApp.m_recordset->GetFieldString( 0 );
		command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", grandpa_id );
		if( !theApp.query( command ) ) return false;
		linenumberGP = theApp.m_recordset->GetFieldString( 0 );

		lineGP	= getHtmlLine( linenumberGP );
		lineF	= getHtmlLine( linenumberF );
		lineP	= getHtmlLine( linenumberP );

		nItem = m_ListCtrl.InsertItem( 0, L"anyai nagyapa" );
		m_ListCtrl.SetItemText( nItem, 1, linenumberGP );
		m_ListCtrl.SetItemText( nItem, 2, lineGP );

		nItem = m_ListCtrl.InsertItem( 1, L"apa" );
		m_ListCtrl.SetItemText( nItem, 1, linenumberF );
		m_ListCtrl.SetItemText( nItem, 2, lineF );

		nItem = m_ListCtrl.InsertItem( 2, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumberP );
		m_ListCtrl.SetItemText( nItem, 2, lineP );

		m_title = L"Egy ember, annak apja és anyai nagyapja";
	}
*/
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
	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	
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
void CHtmlEditLines::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
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
//	if( AfxMessageBox( L"Felülírod a ga.html fájl sorát ezzel a módosított sorral?", MB_YESNO ) == IDNO ) return;

	theApp.saveHtmlLine( m_linenumber, m_line );


	m_ListCtrl.SetItemText( m_nItem, 2, m_line );
	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( L"" );
//	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnChangeEdit()
{
	GetDlgItem( IDC_MODIFY )->EnableWindow( TRUE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CHtmlEditLines::motherAndSiblings()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( L"Anya és gyermekei" );

	int nItem;
	CString line;
	CString linenumber;
	CString mother_id;

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT mother_id FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	mother_id	= theApp.m_recordset->GetFieldString( 0 );

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", mother_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"anya" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );


	m_command.Format( L"SELECT linenumber FROM people WHERE mother_id ='%s' ORDER BY linenumber", mother_id );
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
*/
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::fatherMotherHe()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;
	CString mother_id;
	CString linenumberC;

	CProgressWnd wndP(NULL, L"Keresem a szülõket és testvéreket..." ); 
	wndP.GoModal();


	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT father_id, mother_id, linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	father_id	= theApp.m_recordset->GetFieldString( 0 );
	mother_id	= theApp.m_recordset->GetFieldString( 1 );
	linenumberC	= theApp.m_recordset->GetFieldString( 2 );

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"szülõk" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' AND mother_id ='%s' ORDER BY linenumber", father_id, mother_id );
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
		if (wndP.Cancelled()) break;
	}
}
*/
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CHtmlEditLines::parents()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;
	CString mother_id;
	CString linenumberC;

	CProgressWnd wndP(NULL, L"Keresem a szülõket..." ); 
	wndP.GoModal();

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT father_id, mother_id, linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	father_id	= theApp.m_recordset->GetFieldString( 0 );
	mother_id	= theApp.m_recordset->GetFieldString( 1 );
	linenumberC	= theApp.m_recordset->GetFieldString( 2 );



	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"szülõk" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	line		= getHtmlLine( linenumberC );
	nItem = m_ListCtrl.InsertItem( 1, L"gyerek" );
	m_ListCtrl.SetItemText( nItem, 1, linenumberC );
	m_ListCtrl.SetItemText( nItem, 2, line );

	wndP.DestroyWindow();
}
*/
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