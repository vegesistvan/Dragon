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
	CString m_rowidMain;	// ember azonosítója, akinek egy rokonát meg akarjuk adni
	CString m_rowidClick;	// a click-kelt ember azonosítója ( ha nincs, akkor empty ) ebebn adja vissza a megadott ember azonosítóját!
	int		m_newPeople;	// milyen rokonát adjuk meg?
	int		nItem;			// a kiválasztott ember sorszáma a listában

	int		m_orderFather;
	int		m_orderMother;
	int		m_mother_index;

	bool	EDIT;

// return : m_rowid az új ember azonosítója

	



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	int		m_sexIdMain;
	CString m_lastNameMain;
	CString m_firstNameMain;
	CString	m_fatherIdMain;
	CString	m_motherIdMain;
	CString m_nameMain;


	CString m_title;
	CString m_titolo;
	CString m_last_name;
	CString m_first_name;
	CString m_birth_date;
	CString m_birth_place;
	CString m_death_date;
	CString m_death_place;
	CString	m_comment;
	CString m_father_id;
	CString m_mother_id;
	CString m_name;
	int		m_sex_id;
	int		m_titleX;
	
	CString m_titleNew;
	CString m_titoloNew;
	CString m_lastNameNew;
	CString m_firstNameNew;
	CString m_birthPlaceNew;
	CString m_birthDateNew;
	CString m_deathPlaceNew;
	CString m_deathDateNew;
	CString m_commentNew;
	CString m_fatherIdNew;
	CString m_motherIdNew;
	CString m_nameNew;
	int		m_sexIdNew;
	
	CString m_rowidNew;		// az adatbázisba ett új ember;
	CString m_rowidExists;	// az adatbázisban nmár létezõ embert választottunk ki

	CString str;
	CString m_command;

	CComboBox comboTitle;
	CComboBox comboBirth;
	CComboBox comboDeath;

	void	newPeople();
	void	newChild();
	void	newSibling();
	void	newFather();
	void	newMother();
	void	newSpouse();

	bool	insertPeople();
	bool	insertChild();
	bool	insertFather();
	bool	insertMother();
	bool	insertSibling();
	bool	insertSpouse();

	bool	updatePeople();

	bool	existingChild();
	bool	existingFather();
	bool	existingMother();
	bool	existingSibling();
	bool	existingSpouse();

	bool	checkDate( CString date );
	bool	peopleExists();

	CString	getFatherId();
	CString getMotherId();


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	afx_msg void OnKillfocusBirthDate();
	afx_msg void OnKillfocusDeathDate();
	afx_msg void OnKillfocusFirstName();
	CComboBox comboSex;
};
