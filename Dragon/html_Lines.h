#pragma once
#include "listctrlex.h"


// CHtmlLines dialog

class CHtmlLines : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlLines)
	DECLARE_EASYSIZE

public:
	CHtmlLines(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlLines();

// Dialog Data
	enum { IDD = IDD_HTML_LINES };

	int _what;
	CString parents;
	CString child;
	CString m_line;
	int	 m_linenumber;
	std::vector<CString>* vLines;
	CString m_rowid;
	CString m_type;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;
	CString m_command;

	void motherAndSiblings();

	CString getHtmlLine( CString lineNumber );
	CString cleanHtmlLine( CString cLine );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListLines();
//	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnRclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedModify();
	afx_msg void OnChangeEdit();
};
