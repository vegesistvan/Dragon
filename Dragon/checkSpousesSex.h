#pragma once
#include "listctrlex.h"


// CCheckSpousesSex dialog

class CCheckSpousesSex : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSpousesSex)
//	DECLARE_EASYSIZE

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

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();
	void OnRokonsag();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
	afx_msg void OnList();

	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
};
