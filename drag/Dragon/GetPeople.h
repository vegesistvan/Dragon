#pragma once
#include "listctrlex.h"



class CGetPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CGetPeople)

public:
	CGetPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetPeople();

// Dialog Data
	enum { IDD = IDD_GETPEOPLE };

	CString m_caption;

	CString m_fullname;
	CString m_last_name;
	CString m_first_name;
	CString	m_sex_id;

	CString m_people;

	CString m_rowid;
	CString m_generation;
	CString m_birth_place;
	CString m_birth_date;
	CString m_death_place;
	CString m_death_date;
	CString m_comment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;
	CString str;
	CString m_command;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
