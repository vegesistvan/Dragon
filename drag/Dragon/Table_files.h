#pragma once
#include "listctrlex.h"


class CTableFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CTableFiles)

public:
	CTableFiles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableFiles();

	enum { IDD = IDD_TABLE_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString str;
	CString m_command;
	CListCtrlEx m_ListCtrl;

	DECLARE_MESSAGE_MAP()
public:
	CString m_selected;
	CString m_selected_id;

	virtual BOOL OnInitDialog();
	afx_msg void OnBrowser();
	afx_msg void OnTexteditor();
	afx_msg void OnSelectTable();
	afx_msg void OnUpdateSelectTable(CCmdUI *pCmdUI);
};
