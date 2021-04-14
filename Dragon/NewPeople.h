#pragma once
#include "afxwin.h"
#include "listctrlex.h"
#include "colorstatic.h"

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
	CString m_last_name;	// mert apja gyermekének azonos a családneve
	bool	m_inserted;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	CString m_name;
	int		m_titleX;
	CString str;
	CString m_command;

	CComboBox comboSex;
	CComboBox comboBirth;
	CComboBox comboDeath;

	CColorStatic colorLeiras;
	CColorStatic colorPhotos;


// control változók ///////////////////
	
	CString m_first_name;
	CString m_titolo;
	CString m_title;

	CString m_birth_date;
	CString m_birth_place;

	CString m_death_date;
	CString m_death_place;
	CString m_comment;
//////////////////////////////////////

	CString m_fileSpec;

	CRect rectW;
	bool	m_paint;
	

	bool	peopleExists();
	bool	insertPeople();
	bool	updatePeople();

	bool	setScreen();
	void	fillBlobTable();
	void	displayPicture();
	bool	writeBlockToFile( void* block, int blob_size  );

	bool	checkDate( CString date );
	BOOL	PreTranslateMessage(MSG* pMsg);


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
		
	BOOL CNewPeople::query( CString command );
	BOOL CNewPeople::query1( CString command );
	BOOL CNewPeople::query2( CString command );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKillfocusBirthDate();
	afx_msg void OnKillfocusDeathDate();
	afx_msg void OnKillfocusFirstName();
	afx_msg void OnPaint();
	afx_msg void OnClickedPhotos();
	afx_msg void OnClickedLeiras();

	CListCtrlEx m_ListCtrlB;
};
