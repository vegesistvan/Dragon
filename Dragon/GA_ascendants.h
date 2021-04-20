#pragma once
#include "afxwin.h"

// CGaAscendants dialog

class CGaAscendants : public CDialogEx
{
	DECLARE_DYNAMIC(CGaAscendants)

public:
	CGaAscendants(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaAscendants();

// Dialog Data
	enum { IDD = IDD_GA_ASCENDANTS };

	BOOL m_bold;
	
	CString m_rowid;
	CString m_name;
	CString m_familyNumber;
	int		m_sex_id;
//	int		m_function;

	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;

	int m_lista;
	int m_chain;

	CString str;
	CString m_command;
	CString m_htmlFile;

	CString m_generation;
	int		m_generation_previous;
	CString m_known_as;
	CString m_rowidS;

	
	_int64	m_kekule;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	std::vector<FELMENOK> vFel;
	std::vector<ASCENDANTS> vA;
	std::vector<KEKULE> vKekule;
	std::vector<_int64> vK;

	int		m_listStyleIndex;
	CString m_ulStyle;
	
	CString m_title;
	CString m_sex;
	CString m_last_name;
	CString	m_first_name;
	CString m_father_id;
	CString m_mother_id;
	CString m_spouse_id;
	CString m_nameA;


	CString m_fileSpec;
	int		m_cnt_ul;
	FILE* fh1;
	CComboBox m_ListStyle;
	CString m_numOfG_S;

	// a felmenõkrõl összeállított adatok

	int		m_numOfG;		// listázandó generációk száma
	int		m_numOfA;		// összes felmenõk száma;	

	int		m_numOfFG;		// apai ág generációinek száma
	int		m_numOfFA;		// apai ág felmenõinek száma
	int		m_numOfFDA;		// apai egyenes ági felmenõk száma

	int		m_numOfMG;		// anyai ág generációinak száma
	int		m_numOfMA;		// anyai ág felmenõinek száma
	int		m_numOfMDA;		// anyai egyenes ági felmenõk száma
	

//	void printInfo();

	int CGaAscendants::getPDA( CString parent_id);
	void directFatherAscendants( );
	void directMotherAscendants( );
	BOOL readIn();
	CString getMarriage( CString rowidH, CString rowidS );
	CString getPeopleString_vFel( int i );
	CString getPeopleString_vA( int i );
	CString getPeople( CString rowid );
	void transSorsz();
	void printVA( CString title );
	void printX();
//	void print0();
	void print1();
	void printCouples();
	void printSame();
	void structure();
	bool findAscendants( bool maxG );
	void fillVKekule();
	void printKekule( CString title );


	CString indent( int gen, int genPrev );

	CString openFile();
	CString	printName( CString rowid, CString text  );
//	CString getName( CString rowid );



	BOOL ascLoop();
	BOOL ascLoop1();
	BOOL ascLoop2();
	BOOL m_NN;

	BOOL query( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckNn();

	afx_msg void OnRadioHazastarsak();
	afx_msg void OnRadioApaianyai();
	afx_msg void OnRadioApai();
	afx_msg void OnRadioAnyai();
	afx_msg void OnRadioApaiE();
	afx_msg void OnRadioAnyaiE();
	BOOL m_code;
	BOOL radio0;
};
