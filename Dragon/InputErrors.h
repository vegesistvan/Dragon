#pragma once
#include "listctrlex.h"


// CInputErrors dialog

class CInputErrors : public CDialogEx
{
	DECLARE_DYNAMIC(CInputErrors)
	DECLARE_EASYSIZE

public:
	CInputErrors(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputErrors();

// Dialog Data
	enum { IDD = IDD_INPUTERRORS };

	CString fileSpec1;
	CString fileSpec2;
	CString fileSpec3;
	CString fileSpec4;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrlEx m_ListCtrl;

	void OnHtmlEdit();
	void OnHtmlNotepad();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnErrorfile1();
	afx_msg void OnErrorfile2();
	afx_msg void OnErrorfile3();
	afx_msg void OnErrorfile4();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
};
