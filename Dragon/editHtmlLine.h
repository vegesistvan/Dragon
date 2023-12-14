#pragma once
#include "afxwin.h"


// CEditHtmlLine dialog

class CEditHtmlLine : public CDialogEx
{
	DECLARE_DYNAMIC(CEditHtmlLine)
public:
	CEditHtmlLine(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditHtmlLine();

// Dialog Data
	enum { IDD = IDD_HTML_EDITLINE };
	CString m_line;
	CString m_title;
	CString	m_linenumber;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit();
	afx_msg void OnClickedMent();
};
