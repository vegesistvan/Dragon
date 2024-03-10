#pragma once
#include "afxdialogex.h"
#include "GA_ascendants_str.h"

// CAscendants dialog

class CAscendants : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendants)

public:
	CAscendants(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendants();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASCENDANTS };
#endif
	CString str;
	CString str1;
	CString m_command;
	CString m_ulStyle;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CColorStatic colorGroupDataText;
	CColorStatic colorAnyaiText;
	CColorStatic colorApaiText;

	// a felmen�kr�l �ssze�ll�tott adatok
	ENTRY e;
	std::vector<ENTRY> vE;
	std::vector<GKR> vGKR;

	std::vector<FELMENOK> vFel;

	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;

	FILE* fl;
	FILE* fh;

	int		m_numOfG;		// list�zand� gener�ci�k sz�ma
	int		m_numOfA;		// �sszes felmen�k sz�ma;	

	int		m_numOfFG;		// apai �g gener�ci�inek sz�ma
	int		m_numOfFA;		// apai �g felmen�inek sz�ma
	int		m_numOfFDA;		// apai egyenes �gi felmen�k sz�ma

	int		m_numOfMG;		// anyai �g gener�ci�inak sz�ma
	int		m_numOfMA;		// anyai �g felmen�inek sz�ma
	int		m_numOfMDA;		// anyai egyenes �gi felmen�k sz�ma
	int		m_maxG;			// maxim�lis gener�ci� sz�m, ameddig a felmen�ket keress�k
	int		m_maxAct;		// a t�nylegsen tal�lt maxim�lis gener�ci�

	
	int		m_cnt_ul;
	int		m_cnt;

	int		m_GPrev;		// utolj�ra ki�rt gener�ci�
	CString m_filespec;

	BOOL CAscendants::query(CString command);
	BOOL CAscendants::query1(CString command);
	BOOL CAscendants::query2(CString command);
	BOOL CAscendants::query3(CString command);
	BOOL CAscendants::query4(CString command);

	BOOL CAscendants::createScreen();
	void CAscendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color);
	bool CAscendants::statistic();
	bool CAscendants::getParents(_int64 k, PARENTS* p);
	bool CAscendants::getEntry(CString rowid, ENTRY* e);
	int CAscendants::getFG();
	int CAscendants::getMG();
	

	void CAscendants::printKekule2();
	void CAscendants::printKekule1();
	void CAscendants::allAscendants();
	void CAscendants::paternalAscendants();
	void CAscendants::maternalAscendants();
	

	void CAscendants::ascendants(UINT i, bool fatherBranch );

	void CAscendants::listPeople( int i);
	void CAscendants::printNN(int g, _int64 k);
	void CAscendants::getPeople(UINT i);
	void CAscendants::print(CString);
	UINT CAscendants::apja(UINT i);
	UINT CAscendants::anyja(UINT i);
	UINT CAscendants::felesege(UINT i);

	

	CString CAscendants::indent(int gen, int genPrev);
	bool CAscendants::openHtml(CString filename, CString title);
	
	CString CAscendants::getPeopleString(int i);
	CString CAscendants::getMarriage(CString rowid, CString rowidS);



	DECLARE_MESSAGE_MAP()
public:
	CString m_rowid;
	CString m_name;


	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	

	afx_msg void OnClickedUjra();
	CColorStatic colorUjra;
	CColorStatic colorUjra2;
	CString ujra;
	CColorStatic colorMaxGText;
	CColorStatic colorGroupList;
	CColorStatic colorGroupType;
	BOOL m_NN;
	BOOL m_code;
	BOOL m_bold;
	CString m_GList;
	CButton m_RadioList;
	afx_msg void OnKillfocusEditKekule();
	CString K1S;
	afx_msg void OnClickedButtonPiramid();
	afx_msg void OnClickedButtonBranches();
	CColorStatic colorKekuleText;
};
