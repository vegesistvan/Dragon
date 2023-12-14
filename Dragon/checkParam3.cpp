// CheckParam3.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "CheckParam3.h"
#include "afxdialogex.h"


// CCheckParam3 dialog

IMPLEMENT_DYNAMIC(CCheckParam3, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam3::CCheckParam3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckParam3::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam3::~CCheckParam3()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_LIST, m_RadioList);
	DDX_Control(pDX, IDC_RADIO_ALL, m_RadioAll);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParam3, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckParam3::OnBnClickedOk)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParam3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_EDIT )->SetWindowText( _info );

	m_RadioList.SetCheck( TRUE );
	m_RadioAll.SetCheck( TRUE );

	SetWindowTextW( _caption );

	m_Combo.AddString( L"0" );
	m_Combo.AddString( L"1" );
	m_Combo.AddString( L"2" );
	m_Combo.AddString( L"3" );
	m_Combo.AddString( L"4" );
	m_Combo.AddString( L"5" );

	m_Combo.SetCurSel( 0 );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam3::OnBnClickedOk()
{
	_azonos = m_Combo.GetCurSel();
	_all =  m_RadioAll.GetCheck();
	_html = m_RadioList.GetCheck();
	CDialogEx::OnOK();
}
