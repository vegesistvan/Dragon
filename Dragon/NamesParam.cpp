// NamesParam.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "NamesParam.h"
#include "afxdialogex.h"
#include "Languages.h"

// CNamesParam dialog

IMPLEMENT_DYNAMIC(CNamesParam, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNamesParam::CNamesParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNamesParam::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNamesParam::~CNamesParam()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNamesParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE, colorFile);
	DDX_Control(pDX, IDC_FILESPEC, colorFileSpec);
	DDX_Control(pDX, IDC_LANGUAGE, colorLanguage);
	DDX_Control(pDX, IDC_STATIC_LANG, colorStaticLang);
	DDX_Control(pDX, IDC_COMBO, m_ComboCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CNamesParam, CDialogEx)
	ON_STN_CLICKED(IDC_FILE, &CNamesParam::OnClickedFile)
	ON_STN_CLICKED(IDC_STATIC_LANG, &CNamesParam::OnClickedStaticLang)
	ON_BN_CLICKED(IDOK, &CNamesParam::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CNamesParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorFile.SetTextColor( theApp.m_colorClick ) ;
	colorFileSpec.SetTextColor( theApp.m_colorClick ) ;
	colorStaticLang.SetTextColor( theApp.m_colorClick ) ;
	colorLanguage.SetTextColor( theApp.m_colorClick ) ;

	GetDlgItem( IDC_LANGUAGE )->SetWindowTextW( L"magyar" );
	m_language_id = 1;

	m_sex_id = 1;
	m_ComboCtrl.AddString( L"férfi" );
	m_ComboCtrl.AddString( L"nõi" );
	m_ComboCtrl.AddString( L"bisex" );
	m_ComboCtrl.SetCurSel( m_sex_id - 1 );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNamesParam::OnClickedFile()
{
	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"Text files (*.txt)|*.txt|All Files (*.*)|*.*||" );
	
	dlg.m_ofn.lpstrTitle = L"Válaszd ki a nevket tartalmazó text fájlt!";
	if( dlg.DoModal( ) == IDCANCEL ) return;

	POSITION pos = dlg.GetStartPosition( );
	
	m_fileSpec = dlg.GetNextPathName( pos );
	GetDlgItem( IDC_FILESPEC )->SetWindowTextW( m_fileSpec );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNamesParam::OnClickedStaticLang()
{
	CLanguages dlg;

	if( dlg.DoModal() == IDCANCEL ) return;
	m_language_id = dlg.m_language_id;

	GetDlgItem( IDC_LANGUAGE )->SetWindowTextW( dlg.m_language );
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNamesParam::OnBnClickedOk()
{
	GetDlgItem( IDC_FILESPEC )->GetWindowTextW( m_fileSpec );
	if( m_fileSpec.IsEmpty() )
	{
		AfxMessageBox( L"A fájlt ki kell választani!" );
		return;
	}
	m_sex_id = m_ComboCtrl.GetCurSel() + 1;
	
	CDialogEx::OnOK();
}
