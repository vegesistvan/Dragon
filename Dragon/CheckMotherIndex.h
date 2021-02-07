#pragma once
#include "listctrlex.h"


// CCheckMotherIndex dialog

class CCheckMotherIndex : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckMotherIndex)
	DECLARE_EASYSIZE

public:
	CCheckMotherIndex(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckMotherIndex();

// Dialog Data
	enum { IDD = IDD_CHECK_MOTHERINDEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;
	CStdioFile gafile;

	std::vector<UINT> vPos;
	CString m_command;
	CString str;
	CString cLine;

	CString getHtmlLine( CString lineNumber );
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnDbEdit();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
};
