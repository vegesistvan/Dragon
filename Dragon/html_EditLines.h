#pragma once
#include "listctrlex.h"
#include "EasySizeHD.h"

// CHtmlEditLines dialog

class CHtmlEditLines : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlEditLines)
	DECLARE_EASYSIZE

public:
	CHtmlEditLines(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHtmlEditLines();

// Dialog Data
	enum { IDD = IDD_HTML_EDITLINES };


	std::vector<CString>* vLines;
	int		m_linenumber;
	CString m_rowid;
	CString m_type;
	CString m_title;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;
	CString m_command;
	CString str;
	int		m_nItem;

	CString m_line;

	void displayLine( );
	void displayLines( );
	void fatherAndSiblings();
	void children();
	void parents();

	CString getHtmlLine( CString lineNumber );
	CString cleanHtmlLine( CString cLine );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnListLines();
	afx_msg void OnClickedModify();
	afx_msg void OnChangeEdit();
	afx_msg void OnInfo();
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLista();
};
