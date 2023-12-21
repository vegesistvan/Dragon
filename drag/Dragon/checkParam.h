#pragma once


// CCheckParam dialog

class CCheckParam : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam)

public:
	CCheckParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam();

// Dialog Data
	enum { IDD = IDD_CHECK_PARAM };

	CString m_title;
	CString m_function1;
	CString m_function2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_value;
	afx_msg void OnBnClickedOk();
};
