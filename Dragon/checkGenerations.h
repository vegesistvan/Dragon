#pragma once
#include "listctrlex.h"


// CCheckGenerations dialog

class CCheckGenerations : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckGenerations)

public:
	CCheckGenerations(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckGenerations();

// Dialog Data
	enum { IDD = IDD_CHECK_GENERATIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	int m_cnt;

	void createColumns();
	void fillTable();

//	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlEditLines();
	void OnDbEdit();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
};
