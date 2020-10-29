// HtmlEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Lines.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"

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
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	IDC_LIST,		ES_BORDER,		ES_BORDER,		0 );

	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()

	CProgressWnd wndP(NULL, L"html sorok keresése..." ); 
	wndP.GoModal();
	wndP.SetRange( 0, int(vLines->size()) );
	wndP.SetPos( 0 );
	wndP.SetStep( 1 );

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	int		nItem = 0;
	CString parentsL;
	CString childL;
	CString caption;
	CString cLine;
	CString htmlLine(L"");

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,	L"line#",	LVCFMT_RIGHT,	 80,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,	 1500,-1,COL_TEXT);




	for( UINT i = 0; i < vLines->size(); ++ i )
	{
		nItem = m_ListCtrl.InsertItem( i, vLines->at(i) );
		m_ListCtrl.SetItemText( nItem, 1, vLines->at(i) );
		childL = getHtmlLine( vLines->at(i) );
		m_ListCtrl.SetItemText( nItem, 2, getHtmlLine( vLines->at(i) ) );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

	if( _what == 1 )
	{
		if( child.IsEmpty() )
			caption = L"Kijelölt sorok a htm fájlban";
		else
			caption.Format( L"%s kijelölt sora a html fájlban", child ); 
	}
	else
		caption.Format( L"%s és szülei a ga.html fájlban", child );

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

	line = m_ListCtrl.GetItemText( nItem, 2 );
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
