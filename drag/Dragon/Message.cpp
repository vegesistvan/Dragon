// Message.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "Message.h"
#include "afxdialogex.h"


// CMessage dialog

IMPLEMENT_DYNAMIC(CMessage, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMessage::CMessage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessage::IDD, pParent)
{
	caption.Empty();
	message.Empty();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMessage::~CMessage()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMessage, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMessage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( caption );
	GetDlgItem( IDC_BUTTON )->SetWindowTextW( message ); 
	return TRUE;
}