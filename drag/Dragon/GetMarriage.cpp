// GetMarriage.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GetMarriage.h"
#include "afxdialogex.h"
#include "GetPeople.h"

// CGetMarriage dialog

IMPLEMENT_DYNAMIC(CGetMarriage, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetMarriage::CGetMarriage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetMarriage::IDD, pParent)
	, m_date(_T(""))
	, m_place(_T(""))
	, m_spouse(_T(""))
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGetMarriage::~CGetMarriage()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetMarriage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPOUSE, colorSpouse);
	DDX_Text(pDX, IDC_DATE, m_date);
	DDX_Text(pDX, IDC_PLACE, m_place);
	DDX_Text(pDX, IDC_WIFE, m_spouse);
	DDX_Control(pDX, IDC_COMBO_HORDER, m_ComboHOrder);
	DDX_Control(pDX, IDC_COMBO_WORDER, m_ComboWOrder);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGetMarriage, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_SPOUSE, &CGetMarriage::OnClickedStaticSpouse)
	ON_BN_CLICKED(IDOK, &CGetMarriage::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGetMarriage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for( int i = 1; i < 10; ++i )
	{
		str.Format( L"%d.", i );
		m_ComboHOrder.AddString( str );
		m_ComboWOrder.AddString( str );
	}

	if( m_rowidM.IsEmpty() )
		str = L"Új házasság megadása";
	else
		str = L"Házasság szerkesztése";
	SetWindowTextW( str );
	

	m_ComboWOrder.SetCurSel( m_orderH - 1 );
	m_ComboHOrder.SetCurSel( m_orderW -1 );

	colorSpouse.SetTextColor( theApp.m_colorClick );

	GetDlgItem( IDC_HUSBAND )->SetWindowTextW( m_husband );
	UpdateData( TOSCREEN );  // update-hez
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetMarriage::OnClickedStaticSpouse()
{

	CGetPeople dlg;

	if( m_sex_id == MAN )
	{
		dlg.m_sex_id = WOMAN;
	}
	else
	{
		dlg.m_sex_id = MAN;
	}
	dlg.m_caption = L"Válaszd ki a házastársat!";
	if( dlg.DoModal() == IDCANCEL ) return;

	m_spouse	= dlg.m_people;
	if( m_sex_id == MAN )
		m_rowidS2	= dlg.m_rowid;
	else
		m_rowidS1	= dlg.m_rowid;

	GetDlgItem( IDC_WIFE )->SetWindowTextW( m_spouse );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGetMarriage::OnBnClickedOk()
{
	if( m_spouse.IsEmpty() )
	{
		AfxMessageBox( L"A házastársat meg kell adni!" );
		return;
	}

	GetDlgItem( IDC_PLACE )->GetWindowTextW( m_place );
	GetDlgItem( IDC_DATE )->GetWindowTextW( m_date );
	GetDlgItem( IDC_WIFE )->GetWindowTextW( m_spouse );

	m_orderW = m_ComboWOrder.GetCurSel() + 1;
	m_orderH = m_ComboHOrder.GetCurSel() + 1;


	CString fields = L"place, date, husband_id, wife_id, whichWife, whichHusband";
	CString values;

	if( m_rowidM.IsEmpty() )
	{
		values.Format( L"'%s','%s','%s','%s','%d','%d'", m_place, m_date, m_rowidS1, m_rowidS2, m_orderW, m_orderH );
		m_command.Format( L"INSERT INTO marriages (%s) VALUES (%s)", fields, values );
	}
	else
	{
		m_command.Format( L"UPDATE marriages SET \
place			='%s',\
date			='%s',\
husband_id		='%s',\
wife_id		='%s',\
whichWife		='%d',\
whichHusband	='%d'\
WHERE rowid='%s' ",\
m_place,\
m_date,\
m_rowidS1,\
m_rowidS2,\
m_orderW,\
m_orderH,\
m_rowidM );
	}

	if(!theApp.execute( m_command ) ) return;

	CDialogEx::OnOK();
}
