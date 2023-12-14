#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CSelectTable dialog

class CSelectTable : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectTable)

public:
	CSelectTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectTable();

// Dialog Data
	enum { IDD = IDD_SELECTTABLE };
	CString str;
	CString m_tableNameSelected;
	INT_PTR	m_tableNumberSelected;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CSelectTable::keress( int start );
	int m_orderix;
	CColorStatic colorKeress;

	std::vector<TCHAR*> vList;

	void CSelectTable::push(CString str);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedKeress();

};
