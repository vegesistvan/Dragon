// SaveFirstName.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SaveFirstName.h"
#include "afxdialogex.h"


// CSaveFirstName dialog

IMPLEMENT_DYNAMIC(CSaveFirstName, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSaveFirstName::CSaveFirstName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveFirstName::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSaveFirstName::~CSaveFirstName()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSaveFirstName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSaveFirstName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MEN, &CSaveFirstName::OnClickedButtonMen)
	ON_BN_CLICKED(IDC_BUTTON_WOMEN, &CSaveFirstName::OnClickedButtonWomen)
	ON_BN_CLICKED(IDC_BUTTON_BISEX, &CSaveFirstName::OnClickedButtonBisex)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSaveFirstName::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString caption;
	caption.Format( L"%s nincs a nyilvántartásban. Betegyem?", m_first_name );
	SetWindowTextW( caption );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSaveFirstName::OnClickedButtonMen()
{
	saveFirstName( 1 );
}
void CSaveFirstName::OnClickedButtonWomen()
{
	saveFirstName( 2 );
}
void CSaveFirstName::OnClickedButtonBisex()
{
	saveFirstName( 0 );
}
void CSaveFirstName::saveFirstName( int sex_id )
{
	m_sex_id = sex_id;
	CString command;
	command.Format( L"INSERT INTO firstnames (first_name,sex_id) VALUES('%s','%d')", m_first_name, m_sex_id );
	if( !theApp.executeSys( command ) ) return;
	CDialogEx::OnOK();
}