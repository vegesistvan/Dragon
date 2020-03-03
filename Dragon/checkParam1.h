#pragma once
#include "afxwin.h"


// CCheckParam1 dialog

class CCheckParam1 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam1)

public:
	CCheckParam1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam1();

// Dialog Data
	enum { IDD = IDD_CHECK_PARAM1 };
	BOOL _html;
	CString _caption;
	CString _info;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_RadioHtml;
};
