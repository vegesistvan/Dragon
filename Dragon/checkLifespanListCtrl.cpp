// LifespanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "checkLifespanListCtrl.h"
#include "afxdialogex.h"
#include "html_Lines.h"
#include "Relations.h"


// txt fájl oszlopok
enum
{
	_ROWID,
	_LINENUMBER,
	_TABLENUMBER,
	_GENERATION,
	_SOURCE,
	_UNITED,
	_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_LIFESPAN,
	_COLUMNS
};


// ListCtrl oszlopok
enum
{
	L_CNT,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_LIFESPAN,
};


IMPLEMENT_DYNAMIC(CLifespanListCtrl, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifespanListCtrl::CLifespanListCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLifespanListCtrl::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifespanListCtrl::~CLifespanListCtrl()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLifespanListCtrl, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CLifespanListCtrl::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CLifespanListCtrl::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CLifespanListCtrl::OnHtmlNotepad)
	ON_COMMAND(ID_ROKONSAG, &CLifespanListCtrl::OnRokonsag)
	ON_COMMAND(ID_GAHTML_LINE, &CLifespanListCtrl::OnGahtmlLine)

	ON_NOTIFY(NM_CLICK, IDC_LIST, &CLifespanListCtrl::OnClickList)
	ON_COMMAND(ID_LIST, &CLifespanListCtrl::OnList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifespanListCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );

	EASYSIZE_INIT();

	SetWindowTextW( _caption );

	createColumns();
	fillTable();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CLifespanListCtrl::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::createColumns()
{
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LIFESPAN,	L"élettartam",	LVCFMT_LEFT,	100,-1,COL_NUM);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::fillTable()
{
	CStdioFile file( _fileSpec, CFile::modeRead); 

	CString			cLine;
	CStringArray	A;
	int				n;
	int				nItem = 0;

	CString	rowid;
	CString	lineNumber;
	CString tableNumber;
	CString generation;
	CString source;
	CString united;
	CString name;
	CString birth;
	CString death;
	CString lifespan;

	while(file.ReadString(cLine)) 
	{
		if( cLine.IsEmpty() )	continue;

		A.RemoveAll();
		n = wordList( &A, cLine, '\t', TRUE );

		if( n != _COLUMNS )
		{
			str.Format( L"Oszlopok száma %d !=%d", n, _COLUMNS );
			AfxMessageBox( str );
			return;
		}


		str.Format( L"%d", nItem + 1 );
		nItem = m_ListCtrl.InsertItem( nItem, str );
		m_ListCtrl.SetItemText( nItem, L_ROWID, A[_ROWID] );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, A[_LINENUMBER] );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, A[_TABLENUMBER]);
		m_ListCtrl.SetItemText( nItem, L_UNITED, A[_UNITED] );
		m_ListCtrl.SetItemText( nItem, L_GENERATION, A[_GENERATION] );
		m_ListCtrl.SetItemText( nItem, L_SOURCE, A[_SOURCE] );
		m_ListCtrl.SetItemText( nItem, L_NAME, A[_NAME] );
		m_ListCtrl.SetItemText( nItem, L_BIRTH, A[_BIRTH_DATE] );
		m_ListCtrl.SetItemText( nItem, L_DEATH, A[_DEATH_DATE] );
		m_ListCtrl.SetItemText( nItem, L_LIFESPAN, A[_LIFESPAN] );
		++nItem;
	}
	file.Close();
	m_ListCtrl.SetSortOrder( 2, 11, 8 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CLifespanListCtrl:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_SHOWS, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, L_NAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, L_NAME );
			++cnt;
		}
	

	}

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnList()
{
	CString	logFile(L"lifespan"); 
	CString	title(L"Emberek élettartama");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnGahtmlLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if( nItem == - 1 )
	{
		theApp.message( L"GA.html szerkesztése", L"Nincs kijelölve ember!" );
		return;
	}
	

	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );

	theApp.listHtmlLine( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanListCtrl::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnGahtmlLine();
	*pResult = 0;
}


