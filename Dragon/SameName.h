#pragma once
#include "listctrlex.h"


class CSameName : public CDialogEx
{
	DECLARE_DYNAMIC(CSameName)
	DECLARE_MESSAGE_MAP()

public:
	CSameName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameName();

// Dialog Data
	enum { IDD = IDD_SAME_NAME };

	CString m_caption;
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
	CString m_people;
	CString str;
	CString m_command;


	CListCtrlEx m_ListCtrl;
	CSqliteDBRecordSet*	 m_recordset;

	void fillTable();
	BOOL query( CString command );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
