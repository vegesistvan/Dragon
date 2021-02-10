// FamilyDatesStart.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "FamilyDatesStart.h"
#include "afxdialogex.h"


// CFamilyDatesStart dialog

IMPLEMENT_DYNAMIC(CFamilyDatesStart, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFamilyDatesStart::CFamilyDatesStart(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFamilyDatesStart::IDD, pParent)
	, m_type(2)

	, m_maxDiffBetweenHW(50)
	, m_maxAgeHAtWedd(70)
	, m_minAgeHAtWedd(18)
	, m_maxAgeWAtWedd(60)
	, m_minAgeWAtWedd(14)
	, m_maxDiffFC(70)
	, m_minDiffFC(18)
	, m_minDiffMC(14)
	, m_maxDiffMC(60)
	, m_maxLifespan(110)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFamilyDatesStart::~CFamilyDatesStart()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFamilyDatesStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NOCHILDREN, m_type);

	DDX_Text(pDX, IDC_MAXDIFFBETWEENHW, m_maxDiffBetweenHW);
	DDX_Text(pDX, IDC_MAXAGEHATWEDD, m_maxAgeHAtWedd);
	DDX_Text(pDX, IDC_MINAGEHATWEDD, m_minAgeHAtWedd);
	DDX_Text(pDX, IDC_MAXAGEWATWEDD, m_maxAgeWAtWedd);
	DDX_Text(pDX, IDC_MINAGEWATWEDD, m_minAgeWAtWedd);
	DDX_Text(pDX, IDC_MINDIFFFC, m_minDiffFC);
	DDX_Text(pDX, IDC_MINDIFFMC, m_minDiffMC);
	DDX_Text(pDX, IDC_MAXDIFFMC, m_maxDiffMC);
	DDX_Control(pDX, IDC_TITLE, colorTitle);
	DDX_Text(pDX, IDC_MAXLIFESPAN, m_maxLifespan);
	DDX_Text(pDX, IDC_MAXDIFFFC, m_maxDiffFC);
	DDV_MinMaxInt(pDX, m_maxDiffFC, 14, 90);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFamilyDatesStart, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFamilyDatesStart::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFamilyDatesStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MOREWIFES );
	pButton->SetCheck(true);
	m_type = 2;

	colorTitle.SetTextColor( RED );

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFamilyDatesStart::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	CDialogEx::OnOK();
}
