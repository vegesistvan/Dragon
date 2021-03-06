// EditComment.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "EditComment.h"
#include "afxdialogex.h"


// CEditComment dialog

IMPLEMENT_DYNAMIC(CEditComment, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditComment::CEditComment(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditComment::IDD, pParent)
	, m_comment(_T(""))
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditComment::~CEditComment()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditComment::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_comment);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditComment, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditComment::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();
	SetWindowTextW( m_caption );
	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditComment::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditComment::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////