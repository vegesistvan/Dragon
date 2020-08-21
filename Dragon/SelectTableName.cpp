// SelectTableName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectTableName.h"
#include "afxdialogex.h"
#include "SelectTableHtml.h"


// CSelectTableName dialog

IMPLEMENT_DYNAMIC(CSelectTableName, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectTableName::CSelectTableName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectTableName::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectTableName::~CSelectTableName()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTableName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_STATIC_TABLE, ColorTableName);
	DDX_Control(pDX, IDC_TABLE_NAME, ColorTableName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectTableName, CDialogEx)
//	ON_STN_CLICKED(IDC_STATIC_TABLE, &CSelectTableName::OnClickedStaticTable)
	ON_BN_CLICKED(IDOK, &CSelectTableName::OnBnClickedOk)
	ON_STN_CLICKED(IDC_TABLE_NAME, &CSelectTableName::OnClickedTableName)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectTableName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ColorTableName.SetTextColor( theApp.m_colorClick );
	m_tableNameSelected		= theApp.GetProfileString( L"Settings", L"lastTableName", L"" );
	m_tableNumberSelected	= theApp.GetProfileInt( L"Settings", L"lastTableNumber", 1 );

	CString str;
	str.Format( L"%d", m_tableNumberSelected );
	GetDlgItem( IDC_TABLE_NAME )->SetWindowTextW( m_tableNameSelected );
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTableName::OnClickedTableName()
{
	CSelectTable dlg;
	if( dlg.DoModal() == IDCANCEL ) return;
	m_tableNameSelected		= dlg.m_tableNameSelected;
	m_tableNumberSelected	= dlg.m_tableNumberSelected;

	theApp.WriteProfileStringW( L"Settings", L"lastTableName", m_tableNameSelected );
	theApp.WriteProfileInt( L"Settings", L"lastTableNumber", m_tableNumberSelected );

	GetDlgItem( IDC_TABLE_NAME )->SetWindowTextW( m_tableNameSelected );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTableName::OnBnClickedOk()
{
//	GetDlgItem( IDC_TABLE_NAME )->GetWindowTextW( m_tableNameSelected );
//	if( m_tableNameSelected.IsEmpty() )
//	{
//		AfxMessageBox( L"A táblát ki kell választani!" );
//		return;
//	}
//	theApp.WriteProfileStringW( L"Settings", L"tableNameSelected", m_tableNameSelected );
//	theApp.WriteProfileInt( L"Settings", L"tableNumberSelected", m_tableNumberSelected );

	CDialogEx::OnOK();
}
