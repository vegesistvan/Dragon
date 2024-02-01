#pragma once
#include "afxdialogex.h"
#include "inputGEDCOM_typdef.h"
#include "utilities_dragon.h"
#include "ColorStatic.h"

typedef struct
{
	CString title;
}TITLE;

const TITLE t[] =
{
	{ L"vitéz" },
	{ L"dr." },
	{ L"ifj."},
	{ L"id."},
};


class CInputGEDCOM : public CDialogEx
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CInputGEDCOM)

public:
	CInputGEDCOM(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInputGEDCOM();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_GEDCOM };
#endif


//	bool CInputGEDCOM::printINDI();
//	bool CInputGEDCOM::printFAM();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	int cnt;

	CSqliteDBRecordSet* m_recordset;
	CColorStatic colorComment;
	CColorStatic colorGED;

	CStdioFile gedFile;
	CString m_blobFolder;

	CString m_note;
	int m_lineNumber;
	int m_indiLine;
	int m_numberOfTags;
	int m_count; // az adatbázisban lévõ emberek száa a beolvasás elõtt

	CString m_columnsPeople;
	CString m_columnsMarriages;

	GEDLINE lxtv;

	std::vector<GEDLINE> v_lxtv;

	std::vector<INDI> v_INDI;
	std::vector<FAM> v_FAM;
	std::vector<CHIL> v_CHIL;
	std::vector<INDIFAMS>v_indiFAMS;

	std::vector<PHOTOS> vPhotos;

	INDI I;
	FAM F;
	INDIFAMS IFAMS;
	CHIL CHIL;

	FILE* fl;

	void CInputGEDCOM::emptyDatabase();
	BOOL CInputGEDCOM::setDB();
	BOOL CInputGEDCOM::query(CString command);
	bool CInputGEDCOM::fillVectors();
	int CInputGEDCOM::gedCHAR(CString fileSpec);
	bool CInputGEDCOM::process0();

	CString CInputGEDCOM::getRowid(CString xref);
	bool CInputGEDCOM::insertINDI();
	void CInputGEDCOM::clearIndi();

	
	bool CInputGEDCOM::insertFAM();
	void CInputGEDCOM::clearFAM();
	void CInputGEDCOM::updateParents();

	CStatic m_databaseCtrl;
	CStatic m_mappaCtrl;
	CStatic m_gedCtrl;
		
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedStaticGed();
	
public:

	void abbr();
	void addr();
	void adr1();
	void adr2();
	void date();
	void birt();
	void deat();
	void indi();

	void occu();

	void surn();
	void givn();
	void cont();
	void educ();



	void name();
	void famc();
	void fams();
	void time();
	void titl();
	void trlr();
	void form();
	void obje();
	void nsfx();
	void npfx();
	void sex();
	void spfx();
	void getFile();
	void sour();
	void note();
	void plac();
	void chan();
	void text();
	void reli();
	void head();
	void subm();
	void vers();
	void charx();
	void gedc();

	void fam();
	void husb();
	void wife();
	void chil();
	void marr();
	void info();


	virtual BOOL OnInitDialog();

//	afx_msg void OnBnClickedCancel();
};
