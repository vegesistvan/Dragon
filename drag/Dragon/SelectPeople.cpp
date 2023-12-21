// SelectPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectPeople.h"
#include "SelectParent.h"
#include "afxdialogex.h"


enum
{
	M_ROWID = 0,
	M_LAST_NAME,
	M_FIRST_NAME,
	M_BIRTH_DATE,
	M_DEATH_DATE,
	M_COMMENT
};
// CSelectPeople dialog

IMPLEMENT_DYNAMIC(CSelectPeople, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectPeople::CSelectPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectPeople::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectPeople::~CSelectPeople()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectPeople::OnBnClickedOk)
//	ON_EN_CHANGE(IDC_SEARCH, &CSelectPeople::OnChangeSearch)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)

	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSelectPeople::OnClickedButtonClear)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CSelectPeople::OnClickedKeress)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_recordset		= new CSqliteDBRecordSet;

	SetWindowTextW( m_caption );
	createColumns();
	fillTable();


	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::createColumns()
{
	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	


	m_ListCtrl.InsertColumn( M_ROWID,		L"rowid",			LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( M_LAST_NAME,	L"vezetéknév",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( M_FIRST_NAME,	L"keresztnév",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( M_BIRTH_DATE,	L"születés",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( M_DEATH_DATE,	L"halál",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( M_COMMENT,		L"leírás",		LVCFMT_LEFT,	100,-1,COL_TEXT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::fillTable()
{
	if( m_filter.IsEmpty() )
		m_command.Format( L"SELECT rowid,* FROM people" );
	else
		m_command.Format( L"SELECT rowid,* FROM people WHERE %s", m_filter );
	if( !query( m_command ) ) return;
	
	m_ListCtrl.DeleteAllItems();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		m_ListCtrl.InsertItem( i, m_recordset->GetFieldString( DBP_ROWID ) );
		m_ListCtrl.SetItemText( i, M_LAST_NAME, m_recordset->GetFieldString( DBP_LAST_NAME ) );
		m_ListCtrl.SetItemText( i, M_FIRST_NAME, m_recordset->GetFieldString( DBP_FIRST_NAME ) );
		m_ListCtrl.SetItemText( i, M_BIRTH_DATE, m_recordset->GetFieldString( DBP_BIRTH_DATE ) );
		m_ListCtrl.SetItemText( i, M_DEATH_DATE, m_recordset->GetFieldString( DBP_DEATH_DATE ) );
		m_ListCtrl.SetItemText( i, M_COMMENT, m_recordset->GetFieldString( DBP_COMMENT ) );
		m_recordset->MoveNext();
	}
	m_ListCtrl.SetSortOrder( 1, 2 );
	m_orderix = 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectPeople::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSelectPeople::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CSelectPeople::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::OnClickedButtonClear()
{
	m_rowid.Empty();
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::OnBnClickedOk()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	m_rowid	= m_ListCtrl.GetItemText( nItem, M_ROWID );
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectPeople::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectPeople::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////