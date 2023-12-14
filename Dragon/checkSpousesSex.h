#pragma once
#include "listctrlex.h"


// CCheckSpousesSex dialog

class CCheckSpousesSex : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSpousesSex)

public:
	CCheckSpousesSex(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckSpousesSex();

// Dialog Data
	enum { IDD = IDD_CHECK_SPOUSES_SEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;

	void createColumns();
	void fillColumns();

	void OnEditNotepad();
//	void OnHtmlEditLines();


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnGahtmlLine();
	afx_msg void OnList();

	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFirstnames();
};
