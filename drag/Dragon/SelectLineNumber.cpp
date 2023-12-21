// SelectLineNumber.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "SelectLineNumber.h"


// SelectLineNumber dialog

IMPLEMENT_DYNAMIC(SelectLineNumber, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SelectLineNumber::SelectLineNumber(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECTLINENUMBER, pParent)
	, m_lineNumber(_T(""))
	, m_numberOfLines(_T(""))
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SelectLineNumber::~SelectLineNumber()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectLineNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LINENUMBER, m_lineNumber);
	DDX_Text(pDX, IDC_EDIT_NUMBEROFLINES, m_numberOfLines);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(SelectLineNumber, CDialogEx)
	ON_BN_CLICKED(IDOK, &SelectLineNumber::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SelectLineNumber::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_lineNumber = theApp.GetProfileString(L"dragon", L"lineNumber", L"");
	m_numberOfLines = theApp.GetProfileString(L"dragon", L"numberOfLines", L"");
	UpdateData(TOSCREEN);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectLineNumber::OnBnClickedOk()
{
	UpdateData(FROMSCREEN);
	if (m_lineNumber.IsEmpty())
	{
		AfxMessageBox(L"A kezdõ sor sorszámát meg kell adni!");
		return;

	}
	if (m_numberOfLines.IsEmpty())
	{
		AfxMessageBox(L"A beolvasandó sorok számát meg kell adni!");
		return;
	}
	theApp.WriteProfileStringW(L"dragon", L"lineNumber", m_lineNumber);
	theApp.WriteProfileStringW(L"dragon", L"numberOfLines", m_numberOfLines);
	CDialogEx::OnOK();
}
