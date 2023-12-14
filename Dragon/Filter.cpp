// Filter.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Filter.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(CFilter, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////
CFilter::CFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilter::IDD, pParent)
	, m_relation(_T("OR"))
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
	DDX_Text(pDX, IDC_CONDITION1, m_filterText1);
	DDX_Text(pDX, IDC_CONDITION2, m_filterText2);
	DDX_Control(pDX, IDC_STATIC_ORAND, colorOrAnd);
	DDX_Text(pDX, IDC_STATIC_ORAND, m_relation);
	DDX_Control(pDX, IDC_CONDITION1, colorCondi1);
	DDX_Control(pDX, IDC_CONDITION2, colorCondi2);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFilter, CDialogEx)
	ON_BN_CLICKED(IDC_FILTER_ALL, &CFilter::OnClickedFilterAll)
	ON_BN_CLICKED(IDC_FILTER_LAST, &CFilter::OnClickedFilterLast)
	ON_STN_CLICKED(IDC_STATIC_ORAND, &CFilter::OnClickedStaticOrand)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorCondi1.SetTextColor(RED);
	colorOrAnd.SetTextColor(theApp.m_colorClick);
	colorCondi2.SetTextColor(RED);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedFilterAll()
{
	GetDlgItem(IDC_STATIC_ORAND)->GetWindowText( m_relation );

	m_filterNew.Format(L"(%s) %s %s", m_filter1, m_relation, m_filter2 );
	m_filterTextNew.Format(L"(%s) %s %s", m_filterText1, m_relation, m_filterText2 );					// a korábbi és az új szûrõfeltétel összeépítésa
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedFilterLast()
{
	m_filterNew = m_filter2;
	m_filterTextNew = m_filterText2;
	CDialogEx::OnOK();
	OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFilter::OnClickedStaticOrand()
{
	GetDlgItem(IDC_STATIC_ORAND)->GetWindowText(m_relation );
	if (m_relation == L"OR")
		m_relation = L"AND";
	else
		m_relation = L"OR";
	GetDlgItem(IDC_STATIC_ORAND)->SetWindowText(m_relation);
}
