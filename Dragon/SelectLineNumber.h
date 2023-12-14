#pragma once
#include "afxdialogex.h"


// SelectLineNumber dialog

class SelectLineNumber : public CDialogEx
{
	DECLARE_DYNAMIC(SelectLineNumber)

public:
	SelectLineNumber(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SelectLineNumber();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECTLINENUMBER };
#endif

	CString m_caption;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_lineNumber;
	CString m_numberOfLines;
	afx_msg void OnBnClickedOk();
};
