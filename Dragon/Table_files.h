#pragma once
#include "listctrlex.h"


// CTableFiles dialog

class CTableFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CTableFiles)

public:
	CTableFiles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableFiles();

// Dialog Data
	enum { IDD = IDD_TABLE_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString str;
	CString m_command;

	DECLARE_MESSAGE_MAP()
public:
	CString m_selected;
	CString m_selected_id;

	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnEditDelete();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBrowser();
	afx_msg void OnTexteditor();
	afx_msg void OnSelectTable();
	afx_msg void OnToTable();
};
