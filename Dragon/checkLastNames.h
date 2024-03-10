#pragma once
#include "listctrlex.h"

// CCheckLastNames dialog

class CCheckLastNames : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckLastNames)
	DECLARE_MESSAGE_MAP()

public:
	CCheckLastNames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckLastNames();

// Dialog Data
	enum { IDD = IDD_CHECK_LASTNAMES };

	bool m_message;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;

	CString str;
	CString m_command;
	CString cLine;

	CStdioFile gafile;

	int m_lineNumberPrev;

	CString getHtmlLine( CString lineNumber );
	CString cleanHtmlLine( CString cLine );
	CString getPeople();
	CString getName();

	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();
	
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();
};
