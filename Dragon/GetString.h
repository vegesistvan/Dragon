#pragma once


// CGetString dialog

class CGetString : public CDialogEx
{
	DECLARE_DYNAMIC(CGetString)

public:
	CGetString(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetString();

// Dialog Data
	enum { IDD = IDD_GET_STRING };

	CString m_caption;
	CString m_string;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnKillfocusEdit();
	afx_msg void OnChangeEdit();
};
