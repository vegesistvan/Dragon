#pragma once
#include "listctrlex.h"


// CTablePlaces dialog

class CTablePlaces : public CDialogEx
{
	DECLARE_DYNAMIC(CTablePlaces)

public:
	CTablePlaces(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTablePlaces();

// Dialog Data
	enum { IDD = IDD_TABLE_PLACES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	CListCtrlEx m_ListCtrl;
	int		m_orderix;

	void CTablePlaces::filltable();
	void CTablePlaces::fillrow( int nItem );
	int CTablePlaces::insertPlace( CString place );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnFilltable();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();

	afx_msg void OnChangeSearch();
	afx_msg void OnOccurance();
};
