#pragma once
#include "listctrlex.h"



// CCheckSpousesDiff dialog

class CCheckSpousesDiff : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSpousesDiff)
//	DECLARE_EASYSIZE

public:
	CCheckSpousesDiff(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckSpousesDiff();

// Dialog Data
	enum { IDD = IDD_CHECK_SPOUSES_DIFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_command;
	CString str;
	CString m_title;
	int		m_diff;
	CListCtrlEx m_ListCtrl;

	void spousesDiff();
	void createColumns();

	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnList();
};
