// Places.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_places.h"
#include "afxdialogex.h"
#include "GetString.h"
#include "ProgressWnd.h"

// CTablePlaces dialog

enum
{
	PLC_PLACE = 0,
	PLC_OCCURANCE,
	PLC_ROWID
};
IMPLEMENT_DYNAMIC(CTablePlaces, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTablePlaces::CTablePlaces(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTablePlaces::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTablePlaces::~CTablePlaces()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTablePlaces, CDialogEx)
	ON_COMMAND(ID_FILLTABLE, &CTablePlaces::OnFilltable)
	ON_EN_CHANGE(IDC_SEARCH, &CTablePlaces::OnChangeSearch)
	ON_COMMAND(ID_OCCURANCE, &CTablePlaces::OnOccurance)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_BN_CLICKED(ID_EDIT_UPDATE, &CTablePlaces::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CTablePlaces::OnEditDelete )
	ON_BN_CLICKED(ID_EDIT_INSERT, &CTablePlaces::OnEditInsert )
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTablePlaces::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	m_ListCtrl.InsertColumn( PLC_PLACE,		L"helység",		LVCFMT_LEFT, 200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( PLC_OCCURANCE,	L"elõfordulás",	LVCFMT_LEFT,  60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( PLC_ROWID,		L"rowid",		LVCFMT_LEFT,  40,-1,COL_HIDDEN );
	
	filltable();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::filltable()
{
	m_command.Format( L"SELECT rowid, * FROM places" );
	if( !theApp.query( m_command ) ) return;

	CString rowid;
	CString place;
	CString occurance;
	int		nItem = 0;
	CProgressWnd wndProgress(NULL,L"Helységek listázása..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		place		= theApp.m_recordset->GetFieldString( PLACES_PLACE );
		nItem		= m_ListCtrl.InsertItem( 0, place );

		fillrow( nItem );
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	str.Format( L"Helységek (%d db)", m_ListCtrl.GetItemCount() );
	SetWindowTextW( str );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::fillrow( int nItem )
{
	CString rowid		= theApp.m_recordset->GetFieldString( PLACES_ROWID );
	CString occurance	= theApp.m_recordset->GetFieldString( PLACES_OCCURANCE );

	m_ListCtrl.SetItemText( nItem, PLC_OCCURANCE, occurance );
	m_ListCtrl.SetItemText( nItem, PLC_ROWID, rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::OnFilltable()
{
	m_command = L"DELETE FROM places";
	if( !theApp.execute( m_command ) ) return;

	m_ListCtrl.DeleteAllItems();

	m_command.Format( L"SELECT birth_place, death_place FROM people" );
	if( !theApp.query( m_command ) ) return;

	CProgressWnd wndProgress(NULL,L"Születési és elhalálozási helyek ..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	CString place;
	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		place = theApp.m_recordset->GetFieldString( 0 );
		insertPlace( place );
		place = theApp.m_recordset->GetFieldString( 1 );
		insertPlace( place );

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	m_command.Format( L"SELECT place FROM marriages" );
	if( !theApp.query( m_command ) ) return;

	CProgressWnd wndP(NULL,L"Házasságkötési helyek ..." ); 
	wndP.GoModal();
	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		place = theApp.m_recordset->GetFieldString( 0 );
		insertPlace( place );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	wndP.DestroyWindow();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CTablePlaces::insertPlace( CString place )
{
	if( !place.IsEmpty() )
	{
		m_command.Format( L"SELECT rowid FROM places WHERE place='%s'", place );
		if( !theApp.query1( m_command ) ) return 0;

		if( !theApp.m_recordset1->RecordsCount() )
		{
			m_command.Format( L"INSERT INTO places (place) VALUES('%s')", place );
			if( !theApp.execute( m_command ) ) return -1;
			return(m_ListCtrl.InsertItem( 0, place ) );
		}
	}
	return - 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::OnChangeSearch()
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.search( search, 0,  &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::OnOccurance()
{
	CProgressWnd wndProgress(NULL,L"Elõfordulások számának meghatározása..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_ListCtrl.GetItemCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	int		occurance;
	CString place;
	CString rowid;

	for( int nItem = 0; nItem < m_ListCtrl.GetItemCount(); ++nItem )
	{
		m_ListCtrl.SetItemText( nItem, PLC_OCCURANCE, L"" );
	}
	if( !theApp.execute( L"BEGIN" ) ) return;
	for( int nItem = 0; nItem < m_ListCtrl.GetItemCount(); ++nItem )
	{
		occurance = 0;
		rowid = m_ListCtrl.GetItemText( nItem, PLC_ROWID );
		place = m_ListCtrl.GetItemText( nItem, PLC_PLACE );

		m_command.Format( L"SELECT count() FROM people WHERE birth_place='%s'", place );
		if( !theApp.query( m_command ) ) return;
		occurance += _wtoi( theApp.m_recordset->GetFieldString( 0 ) );

		m_command.Format( L"SELECT count() FROM people WHERE death_place='%s'", place );
		if( !theApp.query( m_command ) ) return;
		occurance += _wtoi( theApp.m_recordset->GetFieldString( 0 ) );

		m_command.Format( L"SELECT count() FROM marriages WHERE place='%s'", place );
		if( !theApp.query( m_command ) ) return;
		occurance += _wtoi( theApp.m_recordset->GetFieldString( 0 ) );

		str.Format( L"%d", occurance );
		m_ListCtrl.SetItemText( nItem, 1, str );

		m_command.Format( L"UPDATE places SET occurance='%d' WHERE rowid = '%s'", occurance, rowid  );
		if( !theApp.execute( m_command ) ) return;

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	wndProgress.DestroyWindow();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTablePlaces::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu(IDR_DROPDOWN_EDIT))
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
void CTablePlaces::OnEditUpdate()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid = m_ListCtrl.GetItemText( nItem, PLC_ROWID );

	CGetString dlg;
	dlg.m_caption = L"Helység nevének módosítása";
	dlg.m_string = m_ListCtrl.GetItemText( nItem, PLC_PLACE );
	if( dlg.DoModal() == IDCANCEL ) return;

	m_command.Format( L"UPDATE places SET place='%s' WHERE rowid = '%s'", dlg.m_string, rowid );
	if( !theApp.execute( m_command )) return;

	m_ListCtrl.SetItemText( nItem,	PLC_PLACE, dlg.m_string );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::OnEditDelete()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString place;

	place = m_ListCtrl.GetItemText( nItem, PLC_PLACE );

	str.Format( L"Tényleg törölni akarod a '%s' helységet?", place );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_command.Format( L"DELETE FROM places WHERE place='%s'", place );
	if( !theApp.execute( m_command ) ) return;
	m_ListCtrl.DeleteItem( nItem );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePlaces::OnEditInsert()
{
	CGetString dlg;
	dlg.m_caption = L"Új helység név megadása";
	dlg.m_string.Empty();
	if( dlg.DoModal() == IDCANCEL ) return;

	int nItem = insertPlace( dlg.m_string );

	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );
}
