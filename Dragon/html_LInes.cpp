// HtmlEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Lines.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "EditItem.h"
// CHtmlLines dialog

IMPLEMENT_DYNAMIC(CHtmlLines, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlLines::CHtmlLines(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlLines::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlLines::~CHtmlLines()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlLines, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlLines::OnDblclkList)
	ON_COMMAND(ID_LIST_LINES, &CHtmlLines::OnListLines)
	ON_BN_CLICKED(IDC_MODIFY, &CHtmlLines::OnClickedModify)
	ON_EN_CHANGE(IDC_EDIT, &CHtmlLines::OnChangeEdit)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	IDC_LIST,		ES_BORDER,		ES_BORDER,		0 );

	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()

	
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
	std::vector<CString> vLinenumbers;

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	

	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );


	if( m_type == L"SIBLINGS" )
	{
		motherAndSiblings();
		return true;
	}
	if( m_type == L"F_SIBLINGS" )
	{
		fatherAndSiblings();
		return true;
	}

	if( m_type == L"FATHERMOTHERHE" )
	{
		fatherMotherHe();
		return true;
	}

	CStringArray A;
	A.Add( L"anyai nagyapa" );
	A.Add( L"apa" );
	A.Add( L"ember" );	
	
	

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

		nItem = m_ListCtrl.InsertItem( 0, A[0] );
		m_ListCtrl.SetItemText( nItem, 1, linenumberGP );
		m_ListCtrl.SetItemText( nItem, 2, lineGP );

		nItem = m_ListCtrl.InsertItem( 1, A[1] );
		m_ListCtrl.SetItemText( nItem, 1, linenumberF );
		m_ListCtrl.SetItemText( nItem, 2, lineF );

		nItem = m_ListCtrl.InsertItem( 2, A[2] );
		m_ListCtrl.SetItemText( nItem, 1, linenumberP );
		m_ListCtrl.SetItemText( nItem, 2, lineP );

		caption = L"Egy ember, annak apja és anyai nagyapja";
	}
	else
	{
		m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_HIDDEN );
		m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
		m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);
	
		for( UINT i = 0; i < vLines->size(); ++ i )
		{
			nItem = m_ListCtrl.InsertItem( i, L"" );
			m_ListCtrl.SetItemText( nItem, 1, vLines->at(i) );
			line = getHtmlLine( vLines->at(i) );
			m_ListCtrl.SetItemText( nItem, 2, line );
		}
	
		if( _what == 1 )
		{
			if( child.IsEmpty() )
				caption = L"Kijelölt sorok a htm fájlban";
			else
				caption.Format( L"%s kijelölt sora a html fájlban", child ); 
		}
		else
			caption.Format( L"%s és szülei a ga.html fájlban", child );
	}
	SetWindowTextW( caption );
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CHtmlLines::getHtmlLine( CString lineNumber )
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
CString CHtmlLines::cleanHtmlLine( CString cLine )
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
void CHtmlLines::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString line;

	m_linenumber = _wtoi( m_ListCtrl.GetItemText( nItem, 1 ) );
	line = m_ListCtrl.GetItemText( nItem, 2 );
	line = line.Mid( 2 );		// geerációs kód eldobása
		

	GetDlgItem( IDC_EDIT )->SetWindowTextW( line );
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::OnListLines()
{
	CString	logFile(L"problémás_html_Sorok"); 
	CString	title = logFile;
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::OnClickedModify()
{
	GetDlgItem( IDC_EDIT )->GetWindowTextW( m_line );
//	if( AfxMessageBox( L"Felülírod a ga.html fájl sorát ezzel a módosított sorral?", MB_YESNO ) == IDNO ) return;

	theApp.saveHtmlLine( m_linenumber, m_line );
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::OnChangeEdit()
{
	GetDlgItem( IDC_MODIFY )->EnableWindow( TRUE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::motherAndSiblings()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlLines::fatherAndSiblings()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT father_id FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	father_id	= theApp.m_recordset->GetFieldString( 0 );

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"apa" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );


	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' ORDER BY linenumber", father_id );
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
void CHtmlLines::fatherMotherHe()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;
	CString mother_id;
	CString linenumberC;

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

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' ORDER BY linenumber", father_id );
	if( !theApp.query( m_command ) ) return;
	for( INT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		linenumber	= theApp.m_recordset->GetFieldString( 0 );
		line		= getHtmlLine( linenumber );
		nItem = m_ListCtrl.InsertItem( 2+i, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumber );
		m_ListCtrl.SetItemText( nItem, 2, line );
	}
}
