// Table_tables.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_tables.h"
#include "afxdialogex.h"
#include "EditTable.h"
#include "CDescendants.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDisplayTables()
//{
//	CTableTables dlg;
//	dlg.m_select = false;
//	dlg.m_caption = L"Az adatbázisban lévõ táblák";
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	T_PEER,
	T_TITOLO,
	T_FAMILYNAME,
	T_ALIAS,
	T_ARM,
	T_DUMMY1,
	T_TABLENUMBERROMAN,
	T_JOINT,
	T_FOLYT,
	T_DUMMY2,
	T_CSALAD,
	T_TORZS,
	T_DUMMY3,
	T_COMMENT,
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
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableTables, CDialogEx)
// Dropdown functions
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CTableTables::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CTableTables::OnHtmlNotepad)
	ON_COMMAND(ID_EDIT_INSERT, &CTableTables::OnEditInsert)
	ON_COMMAND(ID_EDIT_UPDATE, &CTableTables::OnEditUpdate)
	ON_COMMAND(ID_EDIT_DELETE, &CTableTables::OnEditDelete)


	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_COMMAND(ID_EXPORT_ALL, &CTableTables::OnExportAll)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTableTables::OnExportSelected)
	ON_COMMAND(ID_UNFILTER, &CTableTables::OnUnfilter)
	ON_COMMAND(ID_FILTER_FAMILY, &CTableTables::OnFilterFamily)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableTables::OnDblclkList)
	ON_COMMAND(ID_GAHTML, &CTableTables::OnGahtml)

	ON_COMMAND(ID_DESCENDANTS_TABLE, &CTableTables::OnDescendantsTable)
	ON_COMMAND(ID_FILTER_NAME, &CTableTables::OnFilterName)
	ON_COMMAND(ID_FILTER_TABLES, &CTableTables::OnFilterTables)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CTableTables::OnClickedKeress)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CTableTables::OnCustomdrawList)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(theApp.m_caption);
	GetDlgItem( IDC_CAPTION)->SetWindowText(L"Az adatbázisban lévõ táblák" );

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( T_DUMMY,		L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( T_ROWID,		L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_FILENUMBER,	L"fájl#",		LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_FAMILYNUMBER,L"család#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( T_TABLEHEADER,	L"header",		LVCFMT_LEFT,	200,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( T_PERCENT,		L"%%%",			LVCFMT_LEFT,	 50,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( T_PEER,		L"cím",			LVCFMT_LEFT,     40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TITOLO,		L"elõnév",		LVCFMT_LEFT,	140,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_FAMILYNAME,	L"családnév",	LVCFMT_LEFT,	140,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_ALIAS,		L"alias",		LVCFMT_LEFT,	 50,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_ARM,			L"ág",			LVCFMT_LEFT,	100,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( T_DUMMY1,		L"",			LVCFMT_RIGHT,	 5,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TABLENUMBERROMAN,L"roman#",	LVCFMT_RIGHT,	 60,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_JOINT,		L"kapcs",		LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_FOLYT,		L"folyt",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_DUMMY2,		L"",			LVCFMT_RIGHT,	 5,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_CSALAD,		L"család->",	LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_TORZS,		L"->törzs",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( T_DUMMY3,		L"",			LVCFMT_RIGHT,	 5,-1,COL_TEXT);
	
	m_ListCtrl.InsertColumn( T_COMMENT,		L"megj.",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	colorKeres.SetTextColor( theApp.m_colorClick );

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


	m_colored = true;
	m_orderix = 9;
	fillTables();



	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::fillTables()
{
	vColor.clear();
	CString familyName;
	if( m_filter.IsEmpty() )
		m_command = L"SELECT rowid,* FROM tables ORDER BY familyName, titolo";
	else
		m_command.Format( L"SELECT rowid, * FROM tables %s ORDER BY familyName, titolo", m_filter ); 
	if( !theApp.query( m_command ) ) return;


	 GetDlgItem( IDC_CAPTION )->SetWindowTextW( m_conditionText );

	 m_ListCtrl.DeleteAllItems();
	for( UINT nItem =  0; nItem < theApp.m_recordset->RecordsCount(); ++nItem, theApp.m_recordset->MoveNext() )
	{
		m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( DBT_ROWID ));
		setRow( nItem );
	}
	m_ListCtrl.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::setRow( int nItem )
{
	CString titoloName;
	CString titoloName1;

//	m_ListCtrl.SetItemData( nItem, 0 );

	vColor.push_back(0);
	m_ListCtrl.SetItemText( nItem, T_ROWID,				theApp.m_recordset->GetFieldString( DBT_ROWID ));
	m_ListCtrl.SetItemText( nItem, T_FILENUMBER,		theApp.m_recordset->GetFieldString( DBT_FILENUMBER ));
	m_ListCtrl.SetItemText( nItem, T_FAMILYNUMBER,		theApp.m_recordset->GetFieldString( DBT_FAMILYNUMBER ));
	m_ListCtrl.SetItemText( nItem, T_LINENUMBER,		theApp.m_recordset->GetFieldString( DBT_LINENUMBER ));
	m_ListCtrl.SetItemText( nItem, T_TABLEHEADER,		theApp.m_recordset->GetFieldString( DBT_TABLEHEADER ));
	
	m_ListCtrl.SetItemText( nItem, T_PERCENT,			theApp.m_recordset->GetFieldString( DBT_PERCENT ));
	m_ListCtrl.SetItemText( nItem, T_TITOLO,			theApp.m_recordset->GetFieldString( DBT_TITOLO ));

	m_ListCtrl.SetItemText(nItem, T_PEER,				theApp.m_recordset->GetFieldString(DBT_PEER));
	m_ListCtrl.SetItemText( nItem, T_FAMILYNAME,		theApp.m_recordset->GetFieldString( DBT_FAMILY_NAME ));
	m_ListCtrl.SetItemText( nItem, T_ALIAS,				theApp.m_recordset->GetFieldString( DBT_ALIAS ) );
	m_ListCtrl.SetItemText( nItem, T_ARM,				theApp.m_recordset->GetFieldString( DBT_ARM ) );

	m_ListCtrl.SetItemText( nItem, T_TABLENUMBERROMAN,	theApp.m_recordset->GetFieldString( DBT_TABLENUMBERROMAN ) );
		int joint = _wtoi( theApp.m_recordset->GetFieldString( DBT_JOINT ) );
	if( !joint )
		m_ListCtrl.SetItemText( nItem, T_JOINT,			L"nem" );
	m_ListCtrl.SetItemText( nItem, T_FOLYT,				theApp.m_recordset->GetFieldString( DBT_FOLYT ) );
	m_ListCtrl.SetItemText( nItem, T_CSALAD,			theApp.m_recordset->GetFieldString( DBT_CSALAD ) );
	m_ListCtrl.SetItemText( nItem, T_TORZS,				theApp.m_recordset->GetFieldString( DBT_TORZS ) );

	m_ListCtrl.SetItemText( nItem, T_COMMENT,			theApp.m_recordset->GetFieldString( DBT_COMMENT ) );
	
	if( !nItem )									// a 0. sor mindenképpen színes
	{
//		m_ListCtrl.SetItemData( nItem, 1 );
		vColor.at( vColor.size() - 1) = 1;
		m_colored = true;
	}
	else
	{
		theApp.m_recordset->MovePrevious();
		titoloName1.Format( L"%s%s", theApp.m_recordset->GetFieldString( DBT_TITOLO ), theApp.m_recordset->GetFieldString( DBT_FAMILY_NAME ) );
		theApp.m_recordset->MoveNext();
		titoloName.Format( L"%s%s", theApp.m_recordset->GetFieldString( DBT_TITOLO ), theApp.m_recordset->GetFieldString( DBT_FAMILY_NAME ) );
		if( m_colored )								// ha az elõzõ szines volt
		{
			if( titoloName == titoloName1 )			// és azonos nevû, akkor ez is színes
			//	m_ListCtrl.SetItemData( nItem, 1 );
				vColor.at( vColor.size() - 1) = 1;
			else									// na nem azonios nevõ, akkor nem színes
				m_colored = false;
		}
		else										// ha az elõzõ nem volt színes
		{
			if( titoloName != titoloName1 )			// és a nevek sem egyemnlõek
			{
				vColor.at( vColor.size() - 1) = 1;
			//	m_ListCtrl.SetItemData( nItem, 1 );	// akkor ez színes lesz
				m_colored = true;
			}
		}
	}
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
		m_ListCtrl.InsertItem( m_nItem, theApp.m_recordset->GetFieldString( DBT_ROWID ));
	}
	setRow( m_nItem );
	m_ListCtrl.EnsureVisible( m_nItem, FALSE );
	m_ListCtrl.SetItemState( m_nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
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
	CString familyName = m_ListCtrl.GetItemText( nItem, DBT_FAMILY_NAME );
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
		returnSelected();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::returnSelected()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);

	m_tableHeader	= m_ListCtrl.GetItemText( nItem, T_TABLEHEADER );
	m_id			= m_ListCtrl.GetItemText( nItem, T_ROWID );
	m_last_name		= m_ListCtrl.GetItemText( nItem, T_FAMILYNAME );
	m_familyNumber	= m_ListCtrl.GetItemText(nItem, T_FAMILYNUMBER);
	OnOK();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnGahtml()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, T_LINENUMBER );
	CString fileSpec;
	CString command;
	
	command.Format( L"%s -n%s -alwaysOnTop -nosession", theApp.m_htmlPathName, lineNumber );
	ShellExecute(NULL,L"open",theApp.m_editorFileName, command, theApp.m_editorFolderPath, SW_SHOW);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableTables::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelezne
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableTables::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	if( theApp.m_inputMode == GEDCOM )
		MENU_IDR = IDR_DROPDOWN_EDIT;
	else if( theApp.m_inputMode == GAHTML )
		MENU_IDR = IDR_DROPDOWN_HTML;
	else if( theApp.m_inputMode == MANUAL )
		MENU_IDR = IDR_DROPDOWN_EDIT;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);

		if( theApp.m_inputMode == GAHTML )
		{
			pPopup->EnableMenuItem(ID_EDIT_INSERT,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
		}


		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_DB_EDIT,MF_BYCOMMAND | MF_GRAYED);

		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
//			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	T_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, T_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	int nItem;
	int nCol;
	int itemData;
	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		if( m_orderix == 8 )
		{
			nItem = pLVCD->nmcd.dwItemSpec;
			//itemData = m_ListCtrl.GetItemData( nItem );
			if( vColor.at(nItem) )
				pLVCD->clrTextBk = YELLOW;
		}
		nCol = pLVCD->iSubItem;
		if( nCol == T_DUMMY1 || nCol == T_DUMMY2 || nCol == T_DUMMY3 )
		{
			pLVCD->clrTextBk = RGB( 0,255,205);
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnDescendantsTable()  // Kijelölt táblák listája
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		AfxMessageBox(L"Nincs kijelölve senki!");
		return;
	}

	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, T_ROWID);
		theApp.v_tableNumbers.push_back(rowid);
	}
	CDescendants dlg;
	dlg.DoModal();
}
