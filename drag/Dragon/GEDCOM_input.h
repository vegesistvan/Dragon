#pragma once
#include <afxdialogex.h>
#include "GEDCOM_typdef.h"
#include "utilities_dragon.h"
#include "ColorStatic.h"


CString date2date(CString date);
bool sortByCnt(const FAM &v1, const FAM &v2);
bool multiSort_FAM_refH(const FAM &v1, const FAM &v2);
bool multiSort_FAM_refW(const FAM &v1, const FAM &v2);
bool multiSort_INDI_name(const INDI& v1, const INDI& v2);
bool multiSort_refW(const CHIL &v1, const CHIL &v2);
bool sortChil_cnt(const CHIL &v1, const CHIL &v2);
bool sortRefF(const INDIFAMS &v1, const INDIFAMS &v2);
bool sortSexRefI(const INDIFAMS &v1, const INDIFAMS &v2) ;
bool sort_refF(const INDIFAMC &v1, const INDIFAMC &v2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CGEDCOMInput : public CDialogEx
{
	DECLARE_DYNAMIC(CGEDCOMInput)
public:
	CGEDCOMInput(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CGEDCOMInput();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_GEDCOM };
#endif

	enum { IDD = IDD_INPUT_GEDCOM };
	
	void CGEDCOMInput::listPeople();
	void CGEDCOMInput::listFamiliesByName();

	bool CGEDCOMInput::gedcomInput();
	void CGEDCOMInput::indiFams();
	void CGEDCOMInput::listVindi();
	
	void CGEDCOMInput::printVectors();
	void CGEDCOMInput::printFAM();

	CString m_caption;
	CString m_inputMode;
	CStdioFile file_ged;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()


	CSqliteDBRecordSet* m_recordset;
	CColorStatic colorComment;
	CColorStatic colorGED;
	CStdioFile gedFile;
	BOOL m_sameDBName;
//	BOOL REFRESH;

	/*
	CString m_gedFileTitle;
	CString m_gedFileName;
	CString m_gedPathName;
	CString m_gedFolderPath;
	*/
	

	bool m_nomore;
	CString str;
	CString m_command;
//	BOOL	setDB();
	BOOL	query(CString command);

	int m_count;	// az adatbázisban lévõ emberek száma a beolvasás elõtt
	
	ULONGLONG fpos;
	FILE* fl;

	std::vector<INDI> v_indi;
	std::vector<FAM> v_fam;
	std::vector<CHIL> v_chil;
	std::vector<INDIFAMC> v_famc;
	std::vector<INDIFAMS>v_fams;

	std::vector<PHOTOS> vPhotos;
	
	std::vector<GEDLINE> v_lxtv;

	
	CString m_blobFolder;
	CString _refI;
	CString _refF;
	CString _refH;
	CString _refW;
	CString _refC;
	int		_ix;
	int	m_linenumber;
	int m_numberOfTags;

	void CGEDCOMInput::emptyDatabase();
	int CGEDCOMInput::gedCHAR( CString fileSpec );
	bool CGEDCOMInput::fillVectors();
	bool CGEDCOMInput::fill_v_indi();
	bool CGEDCOMInput::fill_v_fam();
	void CGEDCOMInput::sync_fam_indi();

	bool CGEDCOMInput::recordINDI( GEDLINE* gl );
	bool CGEDCOMInput::recordFAM( GEDLINE* gl, int cnt );

	UINT CGEDCOMInput::last1Bellow( int level );
	void CGEDCOMInput::listMultiMarriages();

	void CGEDCOMInput::processNAME(CString value, INDI* indi);
	void CGEDCOMInput::getFirst(CString value, INDI* indi);
	void CGEDCOMInput::getName( CString value, INDI* indi );
	void CGEDCOMInput::splitNAME( CString value, INDI* indi );
	
	CString CGEDCOMInput::getIndi( CString ref );
	int CGEDCOMInput::getIndexIndi( CString ref );
	int CGEDCOMInput::getIndexFam( CString ref );
	bool CGEDCOMInput::insertHttps( CString rowid, CString fileSpec, CString url, int cnt );
	void CGEDCOMInput::insertFile( CString rowid, CString fileSpec, int primary );
	void splitSURN( CString surn, INDI* indi );

	void CGEDCOMInput::insertPeople();
	void CGEDCOMInput::updateParents();
	void CGEDCOMInput::insertMarriages();
	CString CGEDCOMInput::getRowid( CString xref );
	CString CGEDCOMInput::findHref( CString refFam );
	

	

	CString CGEDCOMInput::getRefSpouse( CString sex, CString refF );
	int CGEDCOMInput::getMarriageAll( CString refI );
	CString CGEDCOMInput::getTablenumber( CString lastname );

	void (*foo)(CString );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedStaticGed();
	CStatic m_databaseCtrl;
	CStatic m_mappaCtrl;
	CStatic m_gedCtrl;
};



