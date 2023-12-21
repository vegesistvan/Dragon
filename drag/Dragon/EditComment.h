#pragma once


// CEditComment dialog

class CEditComment : public CDialogEx
{
	DECLARE_DYNAMIC(CEditComment)
public:
	CEditComment(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditComment();

	enum { IDD = IDD_EDIT_COMMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_comment;
	CString m_caption;

};
