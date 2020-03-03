#pragma once
#include "listctrlex.h"


// CTitles dialog

class CTitles : public CDialogEx
{
	DECLARE_DYNAMIC(CTitles)

public:
	CTitles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTitles();

	// Dialog Data
	enum { IDD = IDD_TITLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;
	int		m_orderix;
	void CTitles::fillTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedButtonInsert();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnChangeSearch();
};
