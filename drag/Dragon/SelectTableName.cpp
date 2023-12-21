// SelectTableName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectTableName.h"
#include "afxdialogex.h"
#include "SelectTable.h"


// CSelectTableName dialog

IMPLEMENT_DYNAMIC(CSelectTableName, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectTableName::CSelectTableName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectTableName::IDD, pParent)
	, m_num(L"1")
	, m_number(_T(""))
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
	DDX_Control(pDX, IDC_TABLE_NAME, ColorTableName);
	DDX_Text(pDX, IDC_EDIT_NUM, m_num);
	DDX_Text(pDX, IDC_STATIC_NUMBER, m_number);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectTableName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectTableName::OnBnClickedOk)
	ON_STN_CLICKED(IDC_TABLE_NAME, &CSelectTableName::OnClickedTableName)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectTableName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ColorTableName.SetTextColor( theApp.m_colorClick );
	m_tableNameSelected		= theApp.GetProfileString( L"dragon", L"lastTableName", L"" );
	m_tableNumberSelected	= theApp.GetProfileInt( L"dragon", L"lastTableNumber", 1 );
	m_number.Format(L"%d. tábla", m_tableNumberSelected);

	
	GetDlgItem( IDC_TABLE_NAME )->SetWindowTextW( m_tableNameSelected );
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowTextW(m_number );
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTableName::OnClickedTableName()
{
	CSelectTable dlg;
	if( dlg.DoModal() == IDCANCEL ) return;

	CString rowid;

	m_tableNameSelected		= dlg.m_tableNameSelected;
	m_tableNumberSelected	= dlg.m_tableNumberSelected;

	theApp.WriteProfileStringW( L"dragon", L"lastTableName", m_tableNameSelected );
	theApp.WriteProfileInt( L"dragon", L"lastTableNumber", m_tableNumberSelected );

	str.Format(L"%d. tábla", m_tableNumberSelected);
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowTextW(str );
	GetDlgItem( IDC_TABLE_NAME )->SetWindowTextW( m_tableNameSelected );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectTableName::OnBnClickedOk()
{
	GetDlgItem(IDC_EDIT_NUM)->GetWindowTextW(m_num );
	GetDlgItem( IDC_TABLE_NAME )->GetWindowTextW( m_tableNameSelected );
	if( m_tableNameSelected.IsEmpty() )
	{
		AfxMessageBox( L"A táblát ki kell választani!" );
		return;
	}
	theApp.WriteProfileStringW( L"dragon", L"tableNameSelected", m_tableNameSelected );
	theApp.WriteProfileInt( L"dragon", L"tableNumberSelected", m_tableNumberSelected );


	CDialogEx::OnOK();
}
