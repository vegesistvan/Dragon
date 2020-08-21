// checkParam1.h.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkParam1.h"
#include "afxdialogex.h"


// CCheckParam1 dialog

IMPLEMENT_DYNAMIC(CCheckParam1, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam1::CCheckParam1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckParam1::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam1::~CCheckParam1()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_HTML, m_RadioHtml);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParam1, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckParam1::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParam1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString info;
	info = L"\
\r\n\r\n\
Választani lehet, hogy a kimenet html fájl vagy MFC lista legyen.\
\r\n\r\n\
Ha MFC listát kérünk, akkor a jobb egérgombbal történõ kattintással megjelenõ legördülõ menübõl választható \
funkciókkal lehetõség van a kijelölt emberek eredeti html fájl-sorainak megjelenítésére és azok szerkesztésére.\
\r\n\
";

	str.Format( L"%s %s", _info, info );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );

	m_RadioHtml.SetCheck( TRUE );

	SetWindowTextW( _caption );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam1::OnBnClickedOk()
{
	_html = m_RadioHtml.GetCheck();
	CDialogEx::OnOK();
}

