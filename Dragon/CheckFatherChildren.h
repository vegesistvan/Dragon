#pragma once
#include "afxdialogex.h"
#include "utilities.h"


// CCheckFatherChildren dialog

class CCheckFatherChildren : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFatherChildren)

public:
	CCheckFatherChildren(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCheckFatherChildren();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECK_FATHERCHILDREN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;

	void CCheckFatherChildren::keress(int start);
	void CCheckFatherChildren::createListColumns();
	void CCheckFatherChildren::fatherSameChildren();
	void CCheckFatherChildren::setRow(CSqliteDBRecordSet* rec, int nItem, int data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);

	afx_msg void OnHtml();
	afx_msg void OnEditNotepadParents();
	afx_msg void OnFatherandsiblings();

	void CCheckFatherChildren::OnHtmlNotepadParents();
	void CCheckFatherChildren::OnHtmlChildren();
	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlFamily();
	void OnDbEdit();
	void OnHtmlFatherAndSiblings();
private:
	
public:
	CString m_search;
};
