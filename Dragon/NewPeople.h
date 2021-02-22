#pragma once
#include "afxwin.h"

enum
{
	FATHER = 0,
	MOTHER,
	CHILD,
	SPOUSE,
	SIBLING,
	NOBODY
};

class CNewPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CNewPeople)

public:
	CNewPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewPeople();

// Dialog Data
	enum { IDD = IDD_NEW_PEOPLE };
// input

	CString m_caption;

	int		nItem;			// a kiválasztott ember sorszáma a listában
	CString m_rowid;		// az új ember rowidja
	int		m_sex_id;		// az új meber neme

	bool	m_inserted;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	CString m_name;
	int		m_titleX;
	CString str;
	CString m_command;

	CComboBox comboSex;
//	CComboBox comboTitle;
	CComboBox comboBirth;
	CComboBox comboDeath;

	bool	peopleExists();
	bool	insertPeople();

	bool	checkDate( CString date );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKillfocusBirthDate();
	afx_msg void OnKillfocusDeathDate();
	afx_msg void OnKillfocusFirstName();
	
	CString m_last_name;
	CString m_first_name;
	CString m_titolo;
//	CString m_title;
	CString m_comment;
	CString m_death_date;
	CString m_death_place;
	CString m_birth_date;
	CString m_birth_place;
	CString m_title;
};
