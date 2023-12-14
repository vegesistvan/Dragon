#pragma once
#include "listctrlex.h"


// CTableBlobs dialog

class CTableBlobs : public CDialogEx
{
	DECLARE_DYNAMIC(CTableBlobs)

public:
	CTableBlobs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableBlobs();

	enum { IDD = IDD_TABLE_BLOBS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	int		m_orderix;
	CListCtrlEx m_ListCtrl;

	u_int64	m_sizeTotal;

	CString m_filterText;
	CString m_filterTextNew;
	CString	m_filterNew;
	CString	m_filter;
	bool m_AndOr;

	void CTableBlobs::fillBlobTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilterPeople();
	afx_msg void OnFilterTables();
	afx_msg void OnFilterMarriages();
	afx_msg void OnUnfilter();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditDelete();
	afx_msg void OnFilterOnepeople();
	afx_msg void OnMentAll();
	afx_msg void OnMentSelected();
	afx_msg void OnAttesz();
};
