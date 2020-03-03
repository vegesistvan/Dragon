// SameNameAndParam.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "checkParam2.h"
#include "afxdialogex.h"


// CCheckParam2 dialog

IMPLEMENT_DYNAMIC(CCheckParam2, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam2::CCheckParam2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckParam2::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParam2::~CCheckParam2()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_LIST, m_RadioList);
	DDX_Control(pDX, IDC_RADIO_ALL, m_RadioAll);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParam2, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckParam2::OnBnClickedOk)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParam2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RadioList.SetCheck( TRUE );
	m_RadioAll.SetCheck( TRUE );

	CString info;
	info = L"\
\r\n\r\n\
A program mûködését az alábbi beállításokkal határozhatjuk meg:\
\r\n\r\n\
1. Az összes embert vizsgáljuk, vagy csak azokat, akiknek a családneve a megadandó karakter sorozattal kezdõdõdik.\
\r\n\
2. A kimenet html fájl vagy MFC lista legyen.\
\r\n\r\n\
Ha MFC listát kérünk, akkor a jobb egérgombbal történõ kattintással megjelenõ legördülõ menübõl választható \
funkciókkal lehetõség van a kijelölt emberek eredeti html fájl-sorainak megjelenítésére és azok szerkesztésére.\
\r\n\
";

	str.Format( L"%s %s", _info, info );
	str.Format( L"%s", _info );
	GetDlgItem( IDC_EDIT )->SetWindowText( str );


	SetWindowTextW( _caption );
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParam2::OnBnClickedOk()
{
	_all =  m_RadioAll.GetCheck();
	_html = m_RadioList.GetCheck();

	CDialogEx::OnOK();
}
