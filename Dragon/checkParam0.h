#pragma once
#include "afxwin.h"


// CCheckParam0 dialog

class CCheckParam0 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam0)

public:
	CCheckParam0(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam0();

// Dialog Data
	enum { IDD = IDD_CHECK_PARAM0 };

	CString _caption;
	CString _info;
	BOOL _all;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton RadioAll;
	afx_msg void OnBnClickedOk();
};
