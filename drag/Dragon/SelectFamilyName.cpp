// SelectFamilyName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectFamilyName.h"
#include "afxdialogex.h"
#include "SelectFamily.h"

// CSelectFamilyName dialog

IMPLEMENT_DYNAMIC(CSelectFamilyName, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFamilyName::CSelectFamilyName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectFamilyName::IDD, pParent)
	, m_num(L"1")
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFamilyName::~CSelectFamilyName()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamilyName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FAMILYNAME, colorFamilyName);
	DDX_Text(pDX, IDC_EDIT_NUM, m_num);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectFamilyName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectFamilyName::OnBnClickedOk)
	ON_STN_CLICKED(IDC_FAMILYNAME, &CSelectFamilyName::OnClickedFamilyname)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectFamilyName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorFamilyName.SetTextColor( theApp.m_colorClick );

	m_familyName	= theApp.GetProfileString( L"dragon", L"familyName", L"" );
	m_familyNumber	= theApp.GetProfileString( L"dragon", L"familyNumber", L"" );
	

	str.Format( L"%s-%s", m_familyNumber, m_familyName );
	GetDlgItem( IDC_FAMILYNAME )->SetWindowTextW( str );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamilyName::OnBnClickedOk()
{
	int pos;
	CString family;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowTextW(m_num);
	GetDlgItem( IDC_FAMILYNAME )->GetWindowTextW( family );
	if( m_familyName.IsEmpty() )
	{
		AfxMessageBox( L"A családnevet ki kell választani!" );
		return;
	}

	if( ( pos = family.Find( '-' ) ) == -1 ) return;
	
	m_familyNumber	= family.Left(pos); 
	m_familyName	= family.Mid( pos+1 );


	theApp.WriteProfileStringW( L"dragon", L"familyName", m_familyName );
	theApp.WriteProfileStringW( L"dragon", L"familyNumber", m_familyNumber );
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamilyName::OnClickedFamilyname()
{
	CSelectFamily dlg;

	if( dlg.DoModal() == IDCANCEL ) return;

	m_familyName	= dlg.m_familyName;
	m_familyNumber.Format( L"%d", dlg.m_familyNumber );

	str.Format( L"%s-%s", m_familyNumber, m_familyName );
	GetDlgItem( IDC_FAMILYNAME )->SetWindowTextW( str );
}
