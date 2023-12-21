#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"

// CCheckNumOfSpouses dialog

class CCheckNumOfSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckNumOfSpouses)

public:
	CCheckNumOfSpouses(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCheckNumOfSpouses();

	CString str;
	CString m_command;
	int m_orderix;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECK_NUMOFSPOUSES };
#endif
	FILE* fl;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CCheckNumOfSpouses::createColumns();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnHtmlChildren();
	afx_msg void OnDbEdit();
	CString CCheckNumOfSpouses::getSpouses();

	void CCheckNumOfSpouses::keress(int start);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnClickedNext();
	CColorStatic colorNext;
};
