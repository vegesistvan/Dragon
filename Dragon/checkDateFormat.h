#pragma once
#include "listctrlex.h"


// CDateFormat dialog

class CDateFormat : public CDialogEx
{
	DECLARE_DYNAMIC(CDateFormat)

public:
	CDateFormat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDateFormat();

// Dialog Data
	enum { IDD = IDD_CHECK_DATEFORMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CDateFormat::createColumns();
	void CDateFormat::fillTable();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();
	CListCtrlEx m_ListCtrl;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
