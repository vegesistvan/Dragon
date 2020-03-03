#pragma once
#include "afxwin.h"
#include "colorstatic.h"


// CFilter dialog
enum
{
	AND = 0,
	OR,
	ONLY1,
	ONLY2
};

class CFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CFilter)

public:
	CFilter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilter();

// Dialog Data
	enum { IDD = IDD_FILTER };
	CString m_orand;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int	m_selected_id;
	CString m_condition1;
	CString m_condition2;
	afx_msg void OnClickedOnly2();
	afx_msg void OnClickedOnly1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedOrand();
	afx_msg void OnClickedOr();
	CColorStatic m_colorOr;
};
