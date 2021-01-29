// EditTwoLines.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_edit2Lines.h"
#include "afxdialogex.h"


// CEditTwoLines dialog

IMPLEMENT_DYNAMIC(CEditTwoLines, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditTwoLines::CEditTwoLines(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditTwoLines::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditTwoLines::~CEditTwoLines()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditTwoLines, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_EDIT1, &CEditTwoLines::OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CEditTwoLines::OnChangeEdit2)
	ON_BN_CLICKED(IDOK, &CEditTwoLines::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditTwoLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT1,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT2,	ES_BORDER,	IDC_EDIT1,		ES_BORDER,		ES_BORDER,		0 );

	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()

	m_line1 = theApp.getHtmlLine( vLines->at(0) );
	m_line2 = theApp.getHtmlLine( vLines->at(1) );

	GetDlgItem( IDC_EDIT1)->SetWindowTextW( m_line1.Mid(2) );
	GetDlgItem( IDC_EDIT2)->SetWindowTextW( m_line2.Mid(2) );

	CString caption;

	caption.Format( L"%s fájl szerkesztése", theApp.m_htmlFileSpec );
	SetWindowTextW( caption );

	GetDlgItem( IDOK )->EnableWindow( FALSE );
	m_change1 = false;
	m_change2 = false;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::OnChangeEdit1()
{
	m_change1 = true;
	GetDlgItem( IDOK )->EnableWindow( TRUE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::OnChangeEdit2()
{
	m_change2 = true;
	GetDlgItem( IDOK )->EnableWindow( TRUE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTwoLines::OnBnClickedOk()
{
	CString line1;
	if( m_change1 && m_change2 )
	{
//		if( (AfxMessageBox( L"Mindkét sort cserélni akarod?", MB_YESNO ) ) == IDNO )
//			return;
		GetDlgItem( IDC_EDIT1)->GetWindowTextW( m_line1 );
		GetDlgItem( IDC_EDIT2)->GetWindowTextW( m_line2 );
		theApp.change( vLines->at(0), m_line1 ); 
		theApp.change( vLines->at(1), m_line2 ); 
	}
	else if( m_change1 )
	{
//		if( (AfxMessageBox( L"Cserélni akarod?", MB_YESNO ) ) == IDNO )
//			return;
		GetDlgItem( IDC_EDIT1)->GetWindowTextW( m_line1 );
		theApp.change( vLines->at(0), m_line1 ); 
	}
	else if( m_change2 )
	{
//		if( (AfxMessageBox( L"Cserélni akarod?", MB_YESNO ) ) == IDNO )
//			return;
		GetDlgItem( IDC_EDIT2)->GetWindowTextW( m_line2 );
		theApp.change( vLines->at(1), m_line2 ); 
	}
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////