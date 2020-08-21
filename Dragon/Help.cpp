// Help.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Help.h"
#include "afxdialogex.h"


// CHelp dialog

IMPLEMENT_DYNAMIC(CHelp, CDialogEx)

CHelp::CHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelp::IDD, pParent)
{

}

CHelp::~CHelp()
{
}

void CHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_EditCtrl);
}


BEGIN_MESSAGE_MAP(CHelp, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CHelp message handlers

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	SetWindowTextW( m_caption );
	m_EditCtrl.SetWindowTextW( m_help );

//	m_EditCtrl.SetSel( 0, 0 );
	m_EditCtrl.EnableWindow( FALSE );
//	m_EditCtrl.SetSel( -1, -1 );
//	mouse_event( MOUSEEVENTF_LEFTDOWN, 100, 100, 0, 0 );
//	mouse_event( MOUSEEVENTF_LEFTUP, 100, 100, 0, 0 );

	return FALSE;
}


void CHelp::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}


void CHelp::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}

