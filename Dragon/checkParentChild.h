#pragma once
#include "listctrlex.h"
#include "colorstatic.h"

enum
{
	FCG = 0,
	FCL,
	MCG,
	MCL
};
// CCheckParentChild dialog

class CCheckParentChild : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParentChild)
	DECLARE_EASYSIZE

public:
	CCheckParentChild(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParentChild();
	int m_function;

// Dialog Data
	enum { IDD = IDD_CHECK_PARENTCHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;
	
	CString m_title;
	CString m_command;
//	CString m_parent_id_name;
	CString str;
	int m_diff;

	CColorStatic colorNext;
	CColorStatic colorKeres;

	void createColumns();
	void fillColumns();


	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();

	void keress( int start );
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnList();
	afx_msg void OnClickedKeres();
	afx_msg void OnClickedNext();
};
