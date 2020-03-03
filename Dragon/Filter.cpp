// Filter.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "Filter.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(CFilter, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////
CFilter::CFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilter::IDD, pParent)
	, m_condition1(_T(""))
	, m_condition2(_T(""))
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
CFilter::~CFilter()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONDITION1, m_condition1);
	DDX_Text(pDX, IDC_CONDITION2, m_condition2);
	DDX_Control(pDX, IDC_ORAND, m_colorOr);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFilter, CDialogEx)
	ON_BN_CLICKED(IDC_ONLY2, &CFilter::OnClickedOnly2)
	ON_BN_CLICKED(IDC_ONLY1, &CFilter::OnClickedOnly1)
	ON_BN_CLICKED(IDOK, &CFilter::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ORAND, &CFilter::OnClickedOrand)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_colorOr.SetTextColor( theApp.m_colorClick );


	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedOnly2()
{
	CDialog::EndDialog( ONLY2 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedOnly1()
{
	CDialog::EndDialog( ONLY1);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnBnClickedOk()
{
	GetDlgItem( IDC_ORAND )->GetWindowText( m_orand );
	if( m_orand == L"OR" )
		m_selected_id = OR;
	else
		m_selected_id = AND;

	CDialog::EndDialog( m_selected_id );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedOrand()
{
	GetDlgItem( IDC_ORAND )->GetWindowText( m_orand );
	if( m_orand == L"OR" )
		m_orand = L"AND";
	else
		m_orand = L"OR";
	GetDlgItem( IDC_ORAND )->SetWindowText( m_orand );
}
