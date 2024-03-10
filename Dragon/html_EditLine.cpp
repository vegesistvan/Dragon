// GaLine.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_EditLine.h"
#include "afxdialogex.h"


// CHtmlEditLine dialog
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CHtmlEditLine, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLine::CHtmlEditLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlEditLine::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLine::~CHtmlEditLine()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_EditCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHtmlEditLine, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_EDIT, &CHtmlEditLine::OnChangeEdit)
	ON_BN_CLICKED(IDOK, &CHtmlEditLine::OnClickedMent)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlEditLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDOK,		ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDCANCEL,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_INIT()


//	m_caption.Format( L"A %s fájl %s. sora", theApp.m_htmlPathName, m_linenumber );
	
	SetWindowTextW( m_title );

	GetDlgItem( IDOK )->EnableWindow( FALSE );

	m_line = theApp.getHtmlLine( m_linenumber );
	m_line = m_line.Mid( 2 );

	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_line );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLine::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLine::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,100,fwSide,pRect); 

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLine::OnClickedMent()
{
	GetDlgItem( IDC_EDIT )->GetWindowTextW( m_line );
//	if( AfxMessageBox( L"Felülírod a ga.html fájl sorát ezzel a módosított sorral?", MB_YESNO ) == IDNO ) return;

	theApp.saveHtmlLine( _wtoi( m_linenumber ), m_line );
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLine::OnChangeEdit()
{
	GetDlgItem( IDOK )->EnableWindow( TRUE );

}

