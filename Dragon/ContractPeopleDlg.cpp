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
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractPeopleDlg::~CContractPeopleDlg()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeopleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, comboCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractPeopleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CContractPeopleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CContractPeopleDlg::OnBnClickedCancel)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractPeopleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString info;
	info = L"Azoknak a bejegyzéseknek az összevonását lehet elvégezni, amelyekben azon túl, hogy a nevek azonosak, \
egyik személyes adatukban sem különböznek, sõt az alább felsorolt adatok közül legalább az elõírt számú adat \
mindkét bejegyzésben ismert és egyforma, ami valószínûsíti, hogy a bejegyzések azonos emberre vonatkoznak.\
\r\n\r\n\
Sajnos ezek a kritériumok sem garantálják 100%-osan, hogy valóban azonos személyhez tartoznak a bejegyzések.\
\r\n\r\n\
Ha mindkét bejegyzés rangja 1, akkor azokat nem vonjuk össze, hiszen ugyanaz az ember nem fordulhat elõ kétszer \
leszármazottként.\
\r\n\r\n\
Az adatazonosságok megkövetelt default száma 2, ez ajánlott. Csak kisérleti célból van meg annak lehetõsége, \
hogy ezen változtassuk.\
\r\n\r\n\
Az adat-azonosság számának növelése szûkíti az összevont bejegyzések körét, de csökkenti a megalapozatlan összevonásokat.\
";

	GetDlgItem( IDC_INFO )->SetWindowTextW( info );

	comboCtrl.AddString( L"1" );
	comboCtrl.AddString( L"2" );
	comboCtrl.AddString( L"3" );
	comboCtrl.AddString( L"4" );
	comboCtrl.SetCurSel( 1 );


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeopleDlg::OnBnClickedOk()
{
	m_azonos = comboCtrl.GetCurSel() + 1;
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeopleDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////