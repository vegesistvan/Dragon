#pragma once


// CEditText dialog

class CEditText : public CDialogEx
{
	DECLARE_DYNAMIC(CEditText)

public:
	CEditText(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditText();

// Dialog Data
	enum { IDD = IDD_EDIT_TEXT };

	CString m_text;
	CString m_caption;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEdittext();
};
