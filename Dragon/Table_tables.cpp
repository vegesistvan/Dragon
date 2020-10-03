// Table_tables.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "Table_tables.h"
#include "afxdialogex.h"
#include "EditTable.h"
#include "GA_descendants.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	T_DUMMY = 0,
	T_ROWID,
	T_FILENUMBER,
	T_FAMILYNUMBER,
	T_LINENUMBER,
	T_TABLEHEADER,
	T_PERCENT,
	T_FAMILYNAME,
	T_TABLENUMBERROMAN,
	T_FOLYT,
	T_TORZS,
	T_TITOLO,
	T_COMMENT,
	T_ARM,
	T_JOINT,
	T_NEWFAMILIES,
	T_KNOWN_AS,
	T_ALIAS,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableTables, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableTables::CTableTables(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableTables::IDD, pParent)
{
	m_filter.Empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableTables::~CTableTables()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableTables, CDialogEx)
	ON_COMMAND(ID_EDIT_INSERT, &CTableTables::OnEditInsert)
	ON_COMMAND(ID_EDIT_UPDATE, &CTableTables::OnEditUpdate)
	ON_COMMAND(ID_EDIT_DELETE, &CTableTables::OnEditDelete)

	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_SEARCH, &CTableTables::OnChangeSearch)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_EXPORT_ALL, &CTableTables::OnExportAll)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTableTables::OnExportSelected)
	ON_COMMAND(ID_UNFILTER, &CTableTables::OnUnfilter)
	ON_COMMAND(ID_FILTER_FAMILY, &CTableTables::OnFilterFamily)
	ON_BN_CLICKED(IDOK, &CTableTables::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableTables::OnDblclkList)
	ON_COMMAND(ID_GALINE, &CTableTables::OnGaline)
	ON_COMMAND(ID_GAHTML, &CTableTables::OnGahtml)

	ON_COMMAND(ID_DESCENDANTS_TABLE, &CTableTables::OnDescendantsTable)
	ON_COMMAND(ID_FILTER_NAME, &CTableTables::OnFilterName)
	ON_COMMAND(ID_FILTER_TABLES, &CTableTables::OnFilterTables)
//	ON_COMMAND(ID_PRIVAT_DESCENDANTS_TABLE, &CTableTables::OnPrivatDescendantsTable)
ON_COMMAND(ID_PRIVAT_DESCENDANTS_TABLE, &CTableTables::OnPrivatDescendantsTable)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDOK,		ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDCANCEL,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );

	EASYSIZE_INIT()

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetWindowText( m_caption );

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( T_DUMMY,		L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( T_ROWID,		L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_FILENUMBER,	L"fájl#",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_FAMILYNUMBER,L"család#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_TABLEHEADER,	L"header",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_PERCENT,		L"%%%",			LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_FAMILYNAME,	L"családnév",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TABLENUMBERROMAN,L"roman#",	LVCFMT_RIGHT,	 60,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_FOLYT,		L"folyt",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TITOLO,		L"elõnév",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_COMMENT,		L"megj.",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TORZS,		L"törzs",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_ARM,			L"ág",			LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_JOINT,		L"kapcs",		LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_NEWFAMILIES,	L"új családok",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_KNOWN_AS,	L"ismert",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_ALIAS,		L"alias",		LVCFMT_LEFT,	 50,-1,COL_TEXT);


	if( !m_select )
	{
		SetMenu(NULL);
		::DestroyMenu(GetMenu()->GetSafeHmenu());
		CMenu menu;
		menu.LoadMenuW(IDR_TABLE_TABLES );
		SetMenu(&menu);
		if( wcscmp( theApp.m_username, L"végesistvan" ) )
			menu.EnableMenuItem( 4, MF_BYPOSITION|MF_GRAYED);
	}


	m_orderix = 7;
	fillTables();



	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::fillTables()
{
	CString familyName;

	if( m_filter.IsEmpty() )
		m_command = L"SELECT rowid,* FROM tables";
	else
		m_command.Format( L"SELECT rowid, * FROM tables %s", m_filter ); 
	if( !theApp.query( m_command ) ) return;


	 GetDlgItem( IDC_CAPTION )->SetWindowTextW( m_conditionText );

	 m_ListCtrl.DeleteAllItems();
	for( UINT nItem =  0; nItem < theApp.m_recordset->RecordsCount(); ++nItem, theApp.m_recordset->MoveNext() )
	{
		m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( TABLES_ROWID ));
		setRow( nItem );
	}
	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::setRow( int nItem )
{
	m_ListCtrl.SetItemData( nItem, 4 );
	m_ListCtrl.SetItemText( nItem, T_ROWID,				theApp.m_recordset->GetFieldString( TABLES_ROWID ));
	m_ListCtrl.SetItemText( nItem, T_FILENUMBER,		theApp.m_recordset->GetFieldString( TABLES_FILENUMBER ));
	m_ListCtrl.SetItemText( nItem, T_FAMILYNUMBER,		theApp.m_recordset->GetFieldString( TABLES_FAMILYNUMBER ));
	m_ListCtrl.SetItemText( nItem, T_LINENUMBER,		theApp.m_recordset->GetFieldString( TABLES_LINENUMBER ));
	m_ListCtrl.SetItemText( nItem, T_TABLEHEADER,		theApp.m_recordset->GetFieldString( TABLES_TABLEHEADER ));
	m_ListCtrl.SetItemText( nItem, T_PERCENT,			theApp.m_recordset->GetFieldString( TABLES_PERCENT ));
	m_ListCtrl.SetItemText( nItem, T_FAMILYNAME,		theApp.m_recordset->GetFieldString( TABLES_FAMILY_NAME ));
	m_ListCtrl.SetItemText( nItem, T_TITOLO,			theApp.m_recordset->GetFieldString( TABLES_TITOLO ));
	m_ListCtrl.SetItemText( nItem, T_TABLENUMBERROMAN,	theApp.m_recordset->GetFieldString( TABLES_TABLENUMBERROMAN ) );
	m_ListCtrl.SetItemText( nItem, T_COMMENT,			theApp.m_recordset->GetFieldString( TABLES_COMMENT ) );
	m_ListCtrl.SetItemText( nItem, T_TORZS,				theApp.m_recordset->GetFieldString( TABLES_TORZS ) );
	m_ListCtrl.SetItemText( nItem, T_ARM,				theApp.m_recordset->GetFieldString( TABLES_ARM ) );
	int joint = _wtoi( theApp.m_recordset->GetFieldString( TABLES_JOINT ) );
	if( !joint )
		m_ListCtrl.SetItemText( nItem, T_JOINT,			L"nem" );
	m_ListCtrl.SetItemText( nItem, T_NEWFAMILIES,		theApp.m_recordset->GetFieldString( TABLES_NEWFAMILIES ) );
	m_ListCtrl.SetItemText( nItem, T_KNOWN_AS,			theApp.m_recordset->GetFieldString( TABLES_KNOWN_AS ) );
	m_ListCtrl.SetItemText( nItem, T_ALIAS,				theApp.m_recordset->GetFieldString( TABLES_ALIAS ) );
	m_ListCtrl.SetItemText( nItem, T_FOLYT,				theApp.m_recordset->GetFieldString( TABLES_FOLYT ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableTables::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnEditInsert()
{
	m_nItem = -1;
	editUpdate( L"" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnEditUpdate()
{
	m_nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( m_nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve egy tábla sem!" );
		return;
	}
	m_tableNumber = m_ListCtrl.GetItemText( m_nItem, T_ROWID );
	editUpdate( m_tableNumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}

	CString	rowid;
	CString tableHeader;
	CString familyName;
	rowid		= m_ListCtrl.GetItemText( nItem, T_ROWID );
	tableHeader	= m_ListCtrl.GetItemText( nItem, T_TABLEHEADER );
	familyName	= m_ListCtrl.GetItemText( nItem, T_FAMILYNAME );

	str.Format( L"Tényleg törölni akarod a %s. táblát\na 'people','marriages' és 'tables' táblákból?", rowid );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM people WHERE tableNumber='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_command.Format( L"DELETE FROM marriages WHERE tableNumber='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_command.Format( L"DELETE FROM tables WHERE rowid='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_ListCtrl.DeleteItem( nItem );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::editUpdate( CString tableNumber )
{
	BOOL uj = FALSE;
	CEditTable dlg;
	dlg.m_tableNumber = tableNumber;
	if( tableNumber.IsEmpty() )
	{
		uj = TRUE;
		dlg.m_caption = L"Add meg az új tábla adatait!";
	}
	else
		dlg.m_caption = L"Tábla adatainak módosítása";
	if( dlg.DoModal() == IDCANCEL ) return;

	m_command.Format( L"SELECT rowid,* FROM tables WHERE rowid = '%s'", dlg.m_tableNumber );
	if( !theApp.query( m_command ) ) return;


	if( uj )
	{
		m_nItem = m_ListCtrl.GetItemCount();
		m_ListCtrl.InsertItem( m_nItem, theApp.m_recordset->GetFieldString( TABLES_ROWID ));
	}
	setRow( m_nItem );
	m_ListCtrl.EnsureVisible( m_nItem, FALSE );
	m_ListCtrl.SetItemState( m_nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableTables::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableTables::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnExportAll()
{
	CString	logFile(L"tables"); 
	CString	title(L"Nyilvántartott táblák");;
	
//	title.Format( L"%s", m_filterText );
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
void CTableTables::OnExportSelected()
{
	CString	logFile(L"tables_selected"); 
	CString	title( L"Kijelölt táblák" );
	//title.Format( L"%s (selected from %d projects)", m_filterText, m_ListCtrl.GetItemCount() );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnUnfilter()
{
	m_filter.Empty();
	m_conditionText = L"Szûretlen adatok";
	fillTables();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnFilterFamily()
{
	m_filter = L"WHERE percent='%%%'";
	fillTables();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnFilterTables()
{
	m_filter = L"WHERE percent='%%'";
	fillTables();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnFilterName()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	CString familyName = m_ListCtrl.GetItemText( nItem, TABLES_FAMILY_NAME );
	m_filter.Format( L"WHERE familyName = '%s'", familyName );
	fillTables();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if( !m_select )
		OnEditUpdate();
	else
		OnBnClickedOk();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnDescendantsTable()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	theApp.v_rowids.clear();
	theApp.v_tableNumbers.clear();
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int	rowid;
	
	while(pos)		
	{
		nItem	= m_ListCtrl.GetNextSelectedItem( pos );
		rowid = _wtoi( m_ListCtrl.GetItemText( nItem, T_ROWID ) );
		theApp.v_tableNumbers.push_back( rowid );
	}
	CGaDescendants dlg;
	dlg.m_numbering = 0;
	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnPrivatDescendantsTable()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	theApp.v_rowids.clear();
	theApp.v_tableNumbers.clear();
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int	rowid;
	
	while(pos)		
	{
		nItem	= m_ListCtrl.GetNextSelectedItem( pos );
		rowid = _wtoi( m_ListCtrl.GetItemText( nItem, T_ROWID ) );
		theApp.v_tableNumbers.push_back( rowid );
	}
	CGaDescendants dlg;
	dlg.m_numbering = 1;
	if( dlg.DoModal() == IDCANCEL ) return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnBnClickedOk()
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}
	if( m_selected.IsEmpty() )
		m_selected	= m_ListCtrl.GetItemText(n,T_FAMILYNAME);
	else
		m_selected	= m_ListCtrl.GetItemText(n,T_TABLEHEADER );

	m_last_name		= m_ListCtrl.GetItemText(n,T_FAMILYNAME);
	m_tableHeader	= m_ListCtrl.GetItemText( n, T_TABLEHEADER );

	if( m_select_id == L"familyNumber" )
	{
		m_id		= m_ListCtrl.GetItemText(n,T_FAMILYNUMBER);
	}
	else
	{
		m_id		= m_ListCtrl.GetItemText(n,T_ROWID );
	}
	
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nItem;
	CString rowid;

	v_tableNumbers.clear();
	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		rowid = m_ListCtrl.GetItemText( nItem, T_ROWID ); 
		v_tableNumbers.push_back( rowid );
	}
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableTables::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
    return CWnd::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnGaline()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	T_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnGahtml()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, T_LINENUMBER );
	CString fileSpec;
	CString command;

	command.Format( L"%s -n%s -alwaysOnTop -nosession", theApp.m_htmlFileSpec, lineNumber );
	ShellExecute(NULL,L"open",theApp.m_editorName, command,theApp.m_editorFolder,SW_SHOW);

//	TCHAR command[100];
//	swprintf(command, 100, L"%d", 1 );
//	ShellExecute(NULL, L"open", L"webApp.exe", NULL, L"d:\\VS2019\\cpp\\webApp\\Release", SW_SHOW);
}
