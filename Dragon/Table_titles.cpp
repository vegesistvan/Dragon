// Titles.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_titles.h"
#include "afxdialogex.h"
#include "EditText.h"

// CTitles dialog

enum
{
	L_ROWID = 0,
	L_TITLE,
};

IMPLEMENT_DYNAMIC(CTitles, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTitles::CTitles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTitles::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTitles::~CTitles()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTitles, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CTitles::OnClickedButtonInsert)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_COMMAND(ID_EDIT_UPDATE, &CTitles::OnEditUpdate)
	ON_COMMAND(ID_EDIT_DELETE, &CTitles::OnEditDelete)
	ON_EN_CHANGE(IDC_SEARCH, &CTitles::OnChangeSearch)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTitles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_ROWID,	L"rowid",		LVCFMT_LEFT,	40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_TITLE,	L"rang",	LVCFMT_LEFT,	170,-1,COL_TEXT);


	fillTable();
	m_orderix = 1;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::fillTable()
{

	int nItem;

	m_command =  L"SELECT  rowid, * FROM titles";
	if( !theApp.querySystem( m_command ) ) return;


	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i )
	{
		nItem = m_ListCtrl.InsertItem( i, theApp.m_recordsetSystem->GetFieldString( L_ROWID ) );
		m_ListCtrl.SetItemText( nItem, L_TITLE, theApp.m_recordsetSystem->GetFieldString( L_TITLE ) );
		theApp.m_recordsetSystem->MoveNext();
	}
	m_ListCtrl.SetSortOrder( 1, 2 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTitles::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu( IDR_DROPDOWN_DELETE_UPDATE ))
    {
		pPopup = Menu.GetSubMenu(0);
		if( m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString rang = m_ListCtrl.GetItemText( nItem, L_TITLE );



	str.Format( L"Tényleg törölni akarod a %s rangot?", rang  );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
	
	m_command.Format( L"DELETE FROM titles WHERE title='%s'", rang );
	if( !theApp.executeSys( m_command ) ) return;

	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::OnEditUpdate()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve semmi!" );
		return;
	}
	CString title_old = m_ListCtrl.GetItemText( nItem, L_TITLE );

	CString title;
	GetDlgItem( IDC_EDIT )->GetWindowText( title );
	if( title.IsEmpty() )
	{
		AfxMessageBox( L"Nincs módosított cím megadva!" );
		return;
	}
	str.Format( L"Tényleg módosítani akarod a '%s' címet '%s' névre?", title_old, title );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	m_command.Format( L"UPDATE titles SET title ='%s' WHERE rowid='%s'", title, rowid );
	if( !theApp.executeSys( m_command ) ) return;

	m_ListCtrl.SetItemText( nItem, L_TITLE, title );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::OnClickedButtonInsert()
{
	CString title;
	GetDlgItem( IDC_EDIT )->GetWindowText( title );
	if( title.IsEmpty() )
	{


	}
	m_command.Format( L"INSERT INTO titles ( title ) VALUES ( '%s' )", title );
	if( !theApp.executeSys( m_command ) ) return;

	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTitles::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CTitles::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTitles::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_TAB:
		case VK_RETURN:
			OnClickedButtonInsert();
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTitles::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int nSubItem = pNMItemActivate->iSubItem;
	str.Format( L"%d %d", nItem, nSubItem );
	AfxMessageBox( str );


	*pResult = 0;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTitles::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_EDIT )->GetWindowText( search );
	theApp.search( search, m_orderix,  &m_ListCtrl );
}
