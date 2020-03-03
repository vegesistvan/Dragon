// ColumnsList.cpp : implementation file
//

#include "stdafx.h"
#include "fa.h"
#include "ColumnsList.h"
#include "afxdialogex.h"


// CColumnsList dialog

IMPLEMENT_DYNAMIC(CColumnsList, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CColumnsList::CColumnsList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColumnsList::IDD, pParent)
	, m_underline(FALSE)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CColumnsList::~CColumnsList()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCOLUMNS, m_ListCtrl);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_underline);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CColumnsList, CDialogEx)
	ON_BN_CLICKED(IDOK, &CColumnsList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, &CColumnsList::OnClickedCheckUnderline)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CColumnsList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	int i;
	m_numberOfColumns = p_ListCtrl->GetHeaderCtrl()->GetItemCount();
	HDITEM hditem = {0};
	TCHAR  lpBuffer[100];
	hditem.pszText		= lpBuffer;
	hditem.mask			= HDI_TEXT;
	hditem.cchTextMax	= 100;


	str.Format( L"List %d selected lines", m_selected );
	SetWindowText( str );

	m_ListCtrl.EnableHeaderChkbox( TRUE );
	m_ListCtrl.InsertColumn( 0, L"column name", LVCFMT_LEFT, 100, -1, COL_TEXT );
	for( i=0; i < m_numberOfColumns; i++ )
	{
		VERIFY( p_ListCtrl->GetHeaderCtrl()->GetItem(i, &hditem ));
		m_ListCtrl.InsertItem( i, lpBuffer );
		m_ListCtrl.SetCheck( i, 1 );
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::OnBnClickedOk()
{
	for( int i = 0; i < m_numberOfColumns; i++ )
	{
//		selected[i] = m_ListCtrl.GetCheck( i );
		selected->push_back( m_ListCtrl.GetCheck( i ) );
	}
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::OnClickedCheckUnderline()
{
	m_underline = !m_underline;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////