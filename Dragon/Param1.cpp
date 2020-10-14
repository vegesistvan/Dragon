// Param1.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Param1.h"
#include "afxdialogex.h"


// CParam1 dialog

IMPLEMENT_DYNAMIC(CParam1, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParam1::CParam1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParam1::IDD, pParent)
	, m_text(_T(""))
	, _default( 0 )
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParam1::~CParam1()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParam1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_ComboCtrl);
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_Control(pDX, IDC_EDIT, m_edit);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CParam1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParam1::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CParam1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_caption );

	GetDlgItem( IDC_EDIT )->SetWindowText( m_info );

	m_ComboCtrl.AddString( L"0" );
	m_ComboCtrl.AddString( L"1" );
	m_ComboCtrl.AddString( L"2" );
	m_ComboCtrl.AddString( L"3" );
	m_ComboCtrl.AddString( L"4" );
	m_ComboCtrl.AddString( L"5" );
	m_ComboCtrl.AddString( L"6" );
	m_ComboCtrl.AddString( L"7" );
	m_ComboCtrl.AddString( L"8" );

	m_ComboCtrl.SetCurSel( _default );
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParam1::OnBnClickedOk()
{
	_azonos = m_ComboCtrl.GetCurSel();
	CDialogEx::OnOK();
}
