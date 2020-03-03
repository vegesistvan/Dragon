// SelectFamilyName.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "SelectFamilyName.h"
#include "afxdialogex.h"
#include "SelectFamily.h"

// CSelectFamilyName dialog

IMPLEMENT_DYNAMIC(CSelectFamilyName, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectFamilyName::CSelectFamilyName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectFamilyName::IDD, pParent)
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
//	DDX_Control(pDX, IDC_FAMILY_NAME, colorFamilyName);
	DDX_Control(pDX, IDC_FAMILYNAME, colorFamilyName);
	//  DDX_Control(pDX, IDC_FAMILYNUMBER, colorFamilyNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectFamilyName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectFamilyName::OnBnClickedOk)
	ON_STN_CLICKED(IDC_FAMILYNAME, &CSelectFamilyName::OnClickedFamilyname)
//	ON_STN_CLICKED(IDC_FAMILYNUMBER, &CSelectFamilyName::OnClickedFamilynumber)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectFamilyName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorFamilyName.SetTextColor( theApp.m_colorClick );
//	colorFamilyNumber.SetTextColor( theApp.m_colorClick );

	m_familyName	= theApp.GetProfileString( L"Settings", L"familyName", L"" );
	m_familyNumber	= theApp.GetProfileInt( L"Settings", L"familyNumber", 1  );
	

	str.Format( L"%d-%s", m_familyNumber, m_familyName );
	GetDlgItem( IDC_FAMILYNAME )->SetWindowTextW( str );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamilyName::OnBnClickedOk()
{
	int pos;
	CString family;
	GetDlgItem( IDC_FAMILYNAME )->GetWindowTextW( family );
	if( m_familyName.IsEmpty() )
	{
		AfxMessageBox( L"A családnevet ki kell választani!" );
		return;
	}

	if( ( pos = family.Find( '-' ) ) == -1 ) return;
	
	str				= family.Left( pos );
	m_familyNumber	= _wtoi( str );
	m_familyName	= family.Mid( pos+1 );


	theApp.WriteProfileStringW( L"Settings", L"familyName", m_familyName );
	theApp.WriteProfileInt( L"Settings", L"familyNumber", m_familyNumber );
	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectFamilyName::OnClickedFamilyname()
{
	CSelectFamily dlg;

	if( dlg.DoModal() == IDCANCEL ) return;

	m_familyName = dlg.m_familyName;
	m_familyNumber = dlg.m_familyNumber;

	str.Format( L"%d-%s", m_familyNumber, m_familyName );
	GetDlgItem( IDC_FAMILYNAME )->SetWindowTextW( str );
}
