// SelectString.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "SelectString.h"
#include "afxdialogex.h"


// CSelectString dialog

IMPLEMENT_DYNAMIC(CSelectString, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectString::CSelectString(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectString::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectString::~CSelectString()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectString::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectString, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectString::OnBnClickedOk)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectString::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetSortOrder(1, 2 );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,L"",LVCFMT_RIGHT, 40, -1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,L"gyakoriság",LVCFMT_RIGHT, 70, -1,COL_NUM );

	fillTable();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectString::fillTable()
{
	m_command = L"SELECT occurance FROM firstnames GROUP BY occurance";
	if( !theApp.querySystem( m_command ) ) return;
	int nItem;
		
	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i )
	{
		nItem = m_ListCtrl.InsertItem( i, L"" );
		m_ListCtrl.SetItemText( nItem, 1, theApp.m_recordsetSystem->GetFieldString( 0 ) );
		theApp.m_recordsetSystem->MoveNext();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectString::OnBnClickedOk()
{
	int	nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( nItem < 0 )
	{
		AfxMessageBox(L"Nincs kijelölés!"); 
		return;
	}
	m_selected	= m_ListCtrl.GetItemText( nItem, 1 );

	CDialogEx::OnOK();
}
