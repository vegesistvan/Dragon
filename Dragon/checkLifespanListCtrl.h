#pragma once
#include "listctrlex.h"


// CLifespanListCtrl dialog

class CLifespanListCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CLifespanListCtrl)
	DECLARE_EASYSIZE

public:
	CLifespanListCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLifespanListCtrl();

	CString _fileSpec;
	CString _caption;

// Dialog Data
	enum { IDD = IDD_LIFESPANLISTCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void createColumns();
	void fillTable();
	CString str;
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();
	void OnRokonsag();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnList();
};
