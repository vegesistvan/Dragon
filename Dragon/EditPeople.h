#pragma once
#include "colorstatic.h"
#include "afxwin.h"
#include "listctrlex.h"
//#include "picturectrl.h"


// CEditPeople dialog

class CEditPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CEditPeople)

public:
	CEditPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPeople();

// Dialog Data
	enum { IDD = IDD_EDIT_PEOPLE };

	int		nItem;
	CString	m_sex_id;
	CString	m_tableNumber;
	CString m_rowid;
	CString m_last_name;
	CString m_caption;
	CString m_title;
	CString m_posterior;
	CString m_birth_place;
	CString m_name;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CColorStatic colorTable;

	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!
	CString m_table;

	bool	m_paint;
	bool	m_modified;

	CString	m_filename;
	CString	m_fileSpec;
	CString str;
	CString m_command;

	CComboBox m_ComboSex;
	CComboBox m_ComboTitle;

	CString m_titolo;
	
	CString m_first_name;
	CString m_comment;
	CString m_occupation;
	CString m_death_date;
	CString m_death_place;
	CString m_birth_date;
	

	CComboBox comboBirth;
	CComboBox comboDeath;
	CColorStatic colorPhotos;	


	CString	getTableName( CString tableNumber );
	CString	getParent( CString rowid );
	void	displayParent( int IDC, CString rowid );
	CString	getPeopleString( CString rowid );
	CString	getPlaceDateBlock( TCHAR jel, CString place, CString date );
	CString	getCommentBlock( CString comment, CString occupation  );
	void	updateM( int nItem );
	bool	writeBlockToFile( void* block, int blob_size  );

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnClickedStaticTable();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKillfocusEditFirstname();
	afx_msg void OnKillfocusEditBirthd();
	afx_msg void OnKillfocusEditDeathd();
	afx_msg void OnKillfocusComboSex();
	afx_msg void OnKillfocusLastName();
	afx_msg void OnPaint();
	afx_msg void OnClickedPhotos();
	


	
	afx_msg void OnKillfocusComment();
	afx_msg void OnKillfocusEditTitle();
	afx_msg void OnKillfocusTitolo();
	afx_msg void OnKillfocusEditPosterior();
//	afx_msg void OnKillfocusNamechanged();
	afx_msg void OnKillfocusBirthCombo();
	afx_msg void OnKillfocusDeathCombo();
	afx_msg void OnKillfocusBirthPlace();
	afx_msg void OnKillfocusDeathPlace();
};
