#pragma once
#include "listctrlex.h"


// CSelectFromPeople dialog


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CSelectFromPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectFromPeople)

public:
	CSelectFromPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectFromPeople();

// Dialog Data
	enum { IDD = IDD_SELECTFROMPEOPLE };


	CString m_caption;
	CString m_rowid;
	CString m_rowidSel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	enum
{
	ROWID = 0,
	TABLENUMBER,
	SEXID,
	LASTNAME,
	FIRSTNAME,
	BIRTHPLACE,
	BIRTHDATE,
	DEATHPLACE,
	DEATHDATE,
	PROFESSION,
	COMMENT,
	FATHERID,
	MOTHERID,
	COLUMNSCOUNT
};
	CString m_command;
	CString m_columns;
	CString str;

	CString m_lastname;
	CString m_firstName;
	CString m_name;


	CString getPeople( CString rowid );
	CString pack( CString tag, CString place, CString date );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	CString m_info;
	afx_msg void OnBnClickedOk();
};
