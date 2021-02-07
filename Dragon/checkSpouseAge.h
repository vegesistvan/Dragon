#pragma once
#include "listctrlex.h"


enum
{
	HG = 0,
	HL,
	WG,
	WL
};


class CCheckSpouseAge : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckSpouseAge)
	DECLARE_EASYSIZE

public:
	CCheckSpouseAge(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckSpouseAge();

	int m_function;
	

// Dialog Data
	enum { IDD = IDD_CHECK_SPOUSE_AGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_title;
	CString m_command;
	CString str;
	CListCtrlEx m_ListCtrl;	

	int m_diff;

	void createColumns();
	void fillColumns();

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlEditLines();
	void OnDbEdit();


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
	afx_msg void OnList();
};
