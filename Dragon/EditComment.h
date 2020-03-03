#pragma once


// CEditComment dialog

class CEditComment : public CDialogEx
{
	DECLARE_DYNAMIC(CEditComment)
	DECLARE_EASYSIZE
public:
	CEditComment(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditComment();

// Dialog Data
	enum { IDD = IDD_EDIT_COMMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_comment;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
