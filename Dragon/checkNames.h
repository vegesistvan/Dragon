#pragma once
#include "listctrlex.h"
#include "ColorStatic.h"

class CCheckNames : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckNames)
	DECLARE_MESSAGE_MAP()

public:
	CCheckNames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckNames();

	enum { IDD = IDD_CHECK_NAMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;
	CString str;
	CString m_command;
	CString cLine;
	CStdioFile gafile;
	CColorStatic colorAttention;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditKersztnev();
	afx_msg void OnEditNotepad();
	afx_msg void OnEditSplit();
};
