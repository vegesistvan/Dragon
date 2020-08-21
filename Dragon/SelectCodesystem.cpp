// SelectCodesystem.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SelectCodesystem.h"
#include "afxdialogex.h"


// CSelectCodesystem dialog

IMPLEMENT_DYNAMIC(CSelectCodesystem, CDialogEx)

CSelectCodesystem::CSelectCodesystem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectCodesystem::IDD, pParent)
{

}

CSelectCodesystem::~CSelectCodesystem()
{
}

void CSelectCodesystem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectCodesystem, CDialogEx)
END_MESSAGE_MAP()


// CSelectCodesystem message handlers


BOOL CSelectCodesystem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
