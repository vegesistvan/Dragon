#pragma once
#include "listctrlex.h"


// CCheckNames dialog

class CCheckNames : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckNames)
	DECLARE_EASYSIZE

public:
	CCheckNames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckNames();

// Dialog Data
	enum { IDD = IDD_CHECK_NAMES };

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

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnRokonsag();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
};
