// ContractPeopleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractPeopleDlg.h"
#include "afxdialogex.h"


// CContractPeopleDlg dialog

IMPLEMENT_DYNAMIC(CContractPeopleDlg, CDialogEx)

CContractPeopleDlg::CContractPeopleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractPeopleDlg::IDD, pParent)
	, m_checkSpouse(FALSE)
{

}

CContractPeopleDlg::~CContractPeopleDlg()
{
}

void CContractPeopleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SPOUSE, m_checkSpouse);
}


BEGIN_MESSAGE_MAP(CContractPeopleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CContractPeopleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CContractPeopleDlg message handlers


BOOL CContractPeopleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CContractPeopleDlg::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );
	CDialogEx::OnOK();
}
