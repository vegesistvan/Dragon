#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"

// CCheckNumOfChildren dialog

class CCheckNumOfChildren : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckNumOfChildren)

public:
	CCheckNumOfChildren(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCheckNumOfChildren();

	CString str;
	CString m_command;
	int m_orderix;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECK_NUMOFSPOUSES };
#endif
	FILE* fl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CCheckNumOfChildren::createColumns();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnHtmlChildren();
	afx_msg void OnDbEdit();

	void CCheckNumOfChildren::keress(int start);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnClickedNext();
	CColorStatic colorNext;
};
