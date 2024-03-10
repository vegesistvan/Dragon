#pragma once
#include "afxwin.h"
#include "EasySizeHD.h"

// CHtmlEditLine dialog

class CHtmlEditLine : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlEditLine)
	DECLARE_EASYSIZE
public:
	CHtmlEditLine(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlEditLine();

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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit();
	afx_msg void OnClickedMent();
};
