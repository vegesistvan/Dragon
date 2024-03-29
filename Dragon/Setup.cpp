// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Setup.h"
#include "afxdialogex.h"


// CSetup dialog

IMPLEMENT_DYNAMIC(CSetup, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetup::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSetup::~CSetup()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSetup, CDialogEx)
	ON_EN_KILLFOCUS(IDC_FATHERAGEMAX, &CSetup::OnKillfocusFatheragemax)
	ON_EN_KILLFOCUS(IDC_FATHERAGEMIN, &CSetup::OnKillfocusFatheragemin)
	ON_EN_KILLFOCUS(IDC_MOTHERAGEMAX, &CSetup::OnKillfocusMotheragemax)
	ON_EN_KILLFOCUS(IDC_MOTHERAGEMIN, &CSetup::OnKillfocusMotheragemin)
	ON_BN_CLICKED(IDOK, &CSetup::OnBnClickedOk)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_fatherAgeMin.Format( L"%d", theApp.m_fatherAgeMin );
	m_fatherAgeMax.Format( L"%d", theApp.m_fatherAgeMax );
	GetDlgItem( IDC_FATHERAGEMIN )->SetWindowText( m_fatherAgeMin );
	GetDlgItem( IDC_FATHERAGEMAX )->SetWindowText( m_fatherAgeMax );

	m_motherAgeMin.Format( L"%d", theApp.m_motherAgeMin );
	m_motherAgeMax.Format( L"%d", theApp.m_motherAgeMax );
	GetDlgItem( IDC_MOTHERAGEMIN )->SetWindowText( m_motherAgeMin );
	GetDlgItem( IDC_MOTHERAGEMAX )->SetWindowText( m_motherAgeMax );


	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::OnKillfocusFatheragemax()
{
	GetDlgItem( IDC_FATHERAGEMAX )->GetWindowText( m_fatherAgeMax );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::OnKillfocusFatheragemin()
{
	GetDlgItem( IDC_FATHERAGEMIN )->GetWindowText( m_fatherAgeMin );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::OnKillfocusMotheragemax()
{
	GetDlgItem( IDC_MOTHERAGEMAX )->GetWindowText( m_motherAgeMax );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::OnKillfocusMotheragemin()
{
	GetDlgItem( IDC_MOTHERAGEMIN )->GetWindowText( m_motherAgeMin );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSetup::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CSetup::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
