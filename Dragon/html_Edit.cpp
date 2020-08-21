// GaLine.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_edit.h"
#include "afxdialogex.h"


// CHtmlEdit dialog
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHtmlEdit, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEdit::CHtmlEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlEdit::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEdit::~CHtmlEdit()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_EditCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHtmlEdit, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDOK, &CHtmlEdit::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT, &CHtmlEdit::OnChangeEdit)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDOK,		ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDCANCEL,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_INIT()

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_caption.Format( L"A %s fájl %d. sora", theApp.m_htmlFileSpec, m_lineNumber );
	SetWindowTextW( m_caption );

	GetDlgItem( IDOK )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_line );
//	m_EditCtrl.Clear();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEdit::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEdit::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,100,fwSide,pRect); 

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEdit::OnBnClickedOk()
{
	GetDlgItem( IDC_EDIT )->GetWindowTextW( m_line );
	if( AfxMessageBox( L"Felülírod a ga.html fájl sorát ezzel a módosított sorral?", MB_YESNO ) == IDNO ) return;

	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEdit::OnChangeEdit()
{
	GetDlgItem( IDOK )->EnableWindow( TRUE );

}
