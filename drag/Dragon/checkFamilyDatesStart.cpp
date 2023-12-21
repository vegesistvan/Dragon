// FamilyDatesStart.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckFamilyDatesStart.h"
#include "afxdialogex.h"


// CCheckFamilyDatesStart dialog

IMPLEMENT_DYNAMIC(CCheckFamilyDatesStart, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDatesStart::CCheckFamilyDatesStart(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFamilyDatesStart::IDD, pParent)
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
CCheckFamilyDatesStart::~CCheckFamilyDatesStart()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDatesStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAXAGEHATWEDD, m_maxAgeHAtWedd);
	DDX_Text(pDX, IDC_MINAGEHATWEDD, m_minAgeHAtWedd);
	DDX_Text(pDX, IDC_MAXAGEWATWEDD, m_maxAgeWAtWedd);
	DDX_Text(pDX, IDC_MINAGEWATWEDD, m_minAgeWAtWedd);
	DDX_Text(pDX, IDC_MINDIFFFC, m_minDiffFC);
	DDX_Text(pDX, IDC_MINDIFFMC, m_minDiffMC);
	DDX_Text(pDX, IDC_MAXDIFFMC, m_maxDiffMC);
	DDX_Text(pDX, IDC_MAXLIFESPAN, m_maxLifespan);
	DDX_Text(pDX, IDC_MAXDIFFFC, m_maxDiffFC);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFamilyDatesStart, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckFamilyDatesStart::OnBnClickedOk)
	ON_STN_CLICKED(IDC_INFO, &CCheckFamilyDatesStart::OnClickedInfo)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CCheckFamilyDatesStart::OnClickedButtonSelect)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDatesStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_extrem = L"Extrém hatérértékek belálítása";
	m_normal = L"Normál határértékek beállítása";

	GetDlgItem( IDC_BUTTON_SELECT )->SetWindowTextW( m_extrem );

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDatesStart::OnClickedInfo()
{
	theApp.m_pszAppName = _tcsdup( L"Családon belüli dátumok vizsgálata" );
	AfxMessageBox( m_info, MB_ICONINFORMATION );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDatesStart::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDatesStart::OnClickedButtonSelect()
{
	int pos;
	CString str;
	GetDlgItemTextW( IDC_BUTTON_SELECT, str );
	if( (pos = str.Find( L"Extrém" ) ) != -1 )
	{
		GetDlgItem( IDC_BUTTON_SELECT )->SetWindowTextW( m_normal );
		m_maxAgeHAtWedd	= 100;
		m_minAgeHAtWedd	= 10;
		m_maxAgeWAtWedd	= 100;
		m_minAgeWAtWedd	= 10;
		m_maxDiffFC		= 100;
		m_minDiffFC		= 10;
		m_minDiffMC		= 10;
		m_maxDiffMC		= 100;
		m_maxLifespan	= 150;	
	}
	else
	{
		GetDlgItem( IDC_BUTTON_SELECT )->SetWindowTextW( m_extrem );
		m_maxAgeHAtWedd	= 70;
		m_minAgeHAtWedd	= 18;
		m_maxAgeWAtWedd	= 60;
		m_minAgeWAtWedd	= 14;
		m_maxDiffFC		= 70;
		m_minDiffFC		= 18;
		m_minDiffMC		= 14;
		m_maxDiffMC		= 60;
		m_maxLifespan	= 110;
	}
	UpdateData( TOSCREEN );
}
