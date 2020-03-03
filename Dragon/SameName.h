#pragma once
#include "listctrlex.h"


// CSameName dialog

class CSameName : public CDialogEx
{
	DECLARE_DYNAMIC(CSameName)

public:
	CSameName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameName();

// Dialog Data
	enum { IDD = IDD_SAME_NAME };

	CString m_caption;
	CString m_people;
	CString str;
	CString m_rowid;

	CString m_last_name;
	CString m_first_name;
	CString m_birth_place;
	CString m_birth_date;
	CString m_death_place;
	CString m_death_date;
	CString m_comment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	void fillTable();
	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClickedOld();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
};
