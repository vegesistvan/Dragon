// UNreferencedItems.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "UNreferencedItems.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"


// CUnreferencedItems dialog

IMPLEMENT_DYNAMIC(CUnreferencedItems, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnreferencedItems::CUnreferencedItems(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnreferencedItems::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnreferencedItems::~CUnreferencedItems()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnreferencedItems::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnreferencedItems, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnreferencedItems::OnHtmlNotepad)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnreferencedItems::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_DUMMY,		L"dummy",		LVCFMT_RIGHT,	 65,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 65,-1,COL_NUM );
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"linennumber",	LVCFMT_RIGHT,	 65,-1,COL_NUM );
	else
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"linennumber",	LVCFMT_RIGHT,	 65,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_PEOPLE,		L"bejegyzés",	LVCFMT_LEFT,	 500,-1,COL_TEXT );

	CString filespec;
	CString filename = L"unreferenced";



// nekik nincs sem apjuk sem anyjuk
	m_command.Format( L"SELECT rowid FROM people WHERE \
( father_id='0' OR father_id= '' OR typeof(father_id)='null' ) AND \
( mother_id='0' OR mother_id= '' OR typeof(mother_id)='null' ) ");
	if(!theApp.query( m_command ) ) return false;
	int n = theApp.m_recordset->RecordsCount();

	int nItem = 0;
	CString rowid;
	CString people;
	CString	linenumber;
	int cnt = 0;
	CProgressWnd wndProgress(NULL, L"Nem kapcsolódó bejegyzések keresése..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

// Havatkoznak rá mint szülõ vagy házastárs?
	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid = theApp.m_recordset->GetFieldString( 0 );
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s' OR mother_id= '%s'", rowid, rowid );   
		if( !theApp.query1( m_command ) ) return false;

		m_command.Format( L"SELECT rowid FROM marriages WHERE spouse1_id = '%s' OR spouse2_id= '%s'", rowid, rowid );   
		if( !theApp.query2( m_command ) ) return false;

		if( !theApp.m_recordset1->RecordsCount() && !theApp.m_recordset2->RecordsCount() )
		{
			++cnt;
			people = theApp.selectPeople( rowid, &linenumber );
			m_ListCtrl.InsertItem( nItem, rowid );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, linenumber );
			m_ListCtrl.SetItemText( nItem, L_PEOPLE, people );
			++nItem;
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( !cnt )
	{
		AfxMessageBox( L"Nincs olyan bejegyzés, amire ne lenne hivatkozás!" );
		OnCancel();
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CUnreferencedItems:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	if( theApp.m_inputMode != GAHTML ) return true;

	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_NOTEBOOK ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnreferencedItems::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}