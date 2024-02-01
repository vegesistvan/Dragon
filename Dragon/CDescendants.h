#pragma once
#include "afxdialogex.h"
#include "colorstatic.h"
#include "colorbutton.h"

#define WM_RELOAD			WM_USER + 0x1FF9
namespace DE
{
	typedef struct
	{
		int original;
		int g;
		int order;			// 1,2,3,4,5... a leszármazotti sor hosszúság szerinti sorszámok. 1 a leghosszabb. 
		CString rowid;
		CString rowidF;		// apa rowid-ja
		CString rowidM;		// anya rowid-ja
		CString lastname;
		CString titolo;
		CString tablenumber;
		CString linenumber;
		CString name;
		CString sex;
		int numOfSpouses;
		int numOfChildren;
		int procChildren;
		CString parentSex;
		int parentIndex;
		int fatherIndex;
		int childorder;
		int motherIndex;
		bool hidden;
		int cntRep;
		int numOfRep;
		int status;			// 0: alaphelyzet, listázandó (fekete)  1: ismétlõdõ elsõ bejegyzés, listázandó 2 (kék): ismétlõdõ második bejegyzés, nem listázandó(piros)
		
		bool printed;		// bejegyzés kiírva
		int	id;			// a lineáris listázásnál a bejegyzés id-je; id-re történt rendezés után a vDesc indexe
		int idC;
		int idF;
		CString ixM;
		int nOfD;			// a bejegyzés leszármazottainak száma
		bool clrText;
		bool clrTextBk;

	}DESC;


	typedef struct
	{
		CString tablenumber;
		CString linenumber;
		CString	sex;
		CString posterior;
		CString firstname;
		CString titolo;
		CString name;
		int numOfSpouses;
		int numOfChildren;
		int motherIndex;
		int fatherIndex;
	}PEO;

	typedef struct
	{
		int g;					// vizsgált generáció
		int multiplyer;			// generációhoz rendelt szorzó
		int ixFirst;			// intervallum vGKR beli elsõ indexe
		int ixLast;				// intervallum vGKR beli utolsó indexe
		int order;				// intervallum hossza * multiplyer         
		int gBack;				// az intervallumban történt visszaugrott generáció. Ha nincs akkor 0 
	}LEN;

	typedef struct
	{
		CString text;
		CString code1;
		CString code2;
	}ATTRIB;

	typedef struct
	{
		int g;
		int lastMotherIndex;
	}LMX;

	typedef struct
	{
		int i;

	}SIB;

	const ATTRIB attrib[] =
	{
		L"normál ",		L"",						L"",
		L"félkövér",	L"<b>",						L"</b>",
		L"dõlt",		L"<i>",						L"</i>",
		L"kék",			L"<font color='blue'>",		L"</font>",
		L"piros",		L"<font color='red'>",		L"</font>",
		L"zöld",		L"<font color='green'>",	L"</font>",
	};


	enum
	{
		NORMAL = 0,
		FELKOVER,
		DOLT,
		KEK,
		PIROS,
		ZOLD,
	};
	enum
	{
		SZLUHA = 0,
		VIL,
		TUP,

	};
	enum
	{
		ORDER_INPUT = 0,
		ORDER_BIRTH,
		ORDER_INCREASING,
		ORDER_DECREASING
	};

	enum
	{
		NOINLINE = 0,
		INLINE,
		RAISED
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// leszármazotti listánál p, s, sf, sm, ss strukturák
	typedef struct
	{
		CString	lineNumber;
		CString	rowid;
		CString	tableAncestry;			// TRUE: egy tábla õse
		CString tableNumber;
		CString tableRoman;	// a tábla fejléc római száma
		CString	source;				// az ember típusa 1=leszármazott, 2=házastársa, 3=házastárs apja, 4=házastárs anyja, 5=házastárs további házastársa
		CString generation;
		CString	sex_id;				// 1=férfi, 2= nõ
		CString peer;
		CString title;
		CString titolo;
		CString first_name;
		CString last_name;
		CString posterior;
		CString other_names;
		CString birth_place;
		CString birth_date;
		CString death_place;
		CString death_date;
		CString comment;
		CString	father_id;
		CString	mother_id;
		int		parentIndex;
		CString whichChildFather;
		CString	whichChildMother;
		CString folyt;						// "folyt" utáni római szám, ha van.
		CString arm;						// [] zárójelben lévõ string a család ága [ arm ] 
		CString csalad;
		CString	gap;
		CString occupation;
		CString fullname;
		CString whichHusband;
		CString whichWife;
		CString printed;
		CString printedC;
	} PPEOPLE;
	enum
	{
		SZLUHAP = 0,
		VILP,
		TUPP,

	};
}
#define WM_RELOAD_D			WM_USER + 0x1FFA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDescendants : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendants)

public:
	CDescendants(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendants();

//Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS };
#endif
	bool CDescendants::tablesDescendants();  // listázandó táblák a theapp.v_tableNumbers vektorban
	CString m_rowid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// paraméterek osztályai
	CButton	m_descendantNameRadio;
	CColorButton m_backgroundColor;
	CComboBox m_specAttribCombo;
	CComboBox m__commentAttribCombo;
	CComboBox m_otherNameAttribCombo;
	CComboBox m_descendantAttribCombo;
	CComboBox m_fontSizeCombo;

	
	// Lineáris és tabulált listához is
	
	int		p_descendantName;
	int		p_connect;
	int		p_childrenOrder;
	BOOL	p_womenDescendants;
	BOOL	p_titololower;
	CString	p_generationMax;
	
	// Csak a html listákhoz

	int	p_descendantAttrib;
	bool p_capital;
	bool p_bold;
	int	p_otherNameAttrib;
	int	p_specAttrib;
	int	p_commentAttrib;
	int p_familyNameAttrib;

	int	p_numberingSystem;		// leszármazottak számozása
	int	p_repeated;
	
	UINT p_colorBgrnd;
	
	int p_fontSize;
	bool p_capitalName;
	BOOL p_folyt;
	BOOL p_checkCRLF;
	BOOL p_oneOutputFile;
	BOOL p_repeatedColor;
	CString p_rowWidth;


////////////////////////////////////////////////////////////////////////////////////////
//	bool p_print;		// azonnal nyomtassa ki a leszármazottat, a már szükségtelen rekordokat dobja el a vDesc-bõl. Ez hiba kereséshez jó.
	
	CString str;
	int m_generationMax;

	
	BOOL m_ceckDeleteRepeated;
	
	
	

	CColorStatic colorFamilyInline;
	CColorStatic colorFamilynameNo;
	CColorStatic colorFamilynameUp;
	CColorStatic colorName;
	CColorStatic colorNum;
	CColorStatic colorAttrib;
	CColorStatic colorContent;
	CColorStatic colorOutput;
	CColorStatic colorPrint;
	CColorStatic colorOrder;
	CColorStatic colorRepeated;
	CColorStatic colorBgrnd;


	bool m_printed;
	CString m_tableNumber;
	CString m_lastname;

	CString m_command;
	CString m_htmlFile;
	CString m_os;
	_int64 m_startTime;

	CString m_txtFile;
	CString m_tablenumber;

	bool m_log;
	///////////////////////////////////////////////
	int m_cntRepeated;  // az ismétlõdõ leszármazottak száma

	CString m_descendantsPath;
	CString m_familyName;
	int		m_listedD;
	int		m_listedP;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// behúzások aktuális száma( <ol>/<il>-ek száma ) 
	CString m_diamond;


	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása
	std::vector<CString>vSpouseIndex;
	std::vector<CString>vFullname;		// már listázott házastársak neve
	std::vector<CString>vRowid;			// már listázott házastársak neve
	std::vector<DE::LMX>vLMX;				// a generáció utolsó kiírt motherIndexe a leszármazotti listában
	std::vector<DE::LEN>* vLenOu;
	std::vector<int> vSib;

	DE::PEO peoS;

	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a listázott generációl száma
	bool gflag = true;
	int	m_givup;
	int m_maxDesc;
	bool m_canceled;
	int m_cnt;
	int m_iPrev;;
	CString genPrev;
	FILE* orderTxt;
	CString m_orderFileTitle;
	CString m_orderPathName;

	CSqliteDBRecordSet rs;
	CSqliteDBRecordSet rs2;
	CSqliteDBRecordSet rsP;
	CSqliteDBRecordSet rsM;
	FILE* fl;
	FILE* text;
	FILE* txtOut;
	FILE* fhDescTable;
	FILE* fhPrintable;

	DE::PPEOPLE p;
	DE::PPEOPLE s;
	DE::PPEOPLE sf;
	DE::PPEOPLE sm;
	DE::PPEOPLE ss;

	CComboBox m_comboFamilyName;
	// descendantsOld változói
	int m_genPeer;
	CString m_lastnamePrev;
	CString m_sexidPrev;

	CColorStatic groupAllText;
	CColorStatic groupDontworkText;
	CColorStatic groupIndentedText;



	void CDescendants::setParameters();
	void CDescendants::OnClickedDefault();
	void CDescendants::updateParameters();
	void CDescendants::updateRadioDName();
	void CDescendants::updateRepeated();
	void CDescendants::updateRadioNumbering();
	void CDescendants::OnClickedCheckConnect();
	void CDescendants::OnClickedCheckWoman();
	void CDescendants::OnClickedCheckFolyt();
//	void CDescendants::OnClickedCheckCapital();
	void CDescendants::OnClickedSzluha();
	void CDescendants::OnVillers();
	void CDescendants::OnTupigny();
	void CDescendants::OnClickedRadioNofamilyname();
	void CDescendants::OnRadioFamilyname();
	void CDescendants::OnRadioFamilynameup();
	void CDescendants::OnClickedStaticBackground();
	void CDescendants::OnBnClickedOk();
	void CDescendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color);
	BOOL CDescendants::create_vDesc();
	void CDescendants::descendants();
	void CDescendants::descendantsOld();
	CString CDescendants::getNextChildRowid(UINT i);
	BOOL CDescendants::openHtml(CString file, CString title, UINT colorBgrnd);
	void CDescendants::createStyle();
	void CDescendants::closeHtml();
	void CDescendants::queryPeople(CString rowid, DE::PPEOPLE* p);
	void CDescendants::print(CString str);
	void CDescendants::printBegining(int i);

	CString CDescendants::getLastFirst(DE::PPEOPLE* p);
	bool CDescendants::voltmar(CString index);
	int CDescendants::getNumberOfChildren(CString rowid, CString sex_id);
	void CDescendants::listDescendants();
	void CDescendants::multipleRowid();
	void CDescendants::signeD(int i, int j);
	CString CDescendants::getTableHeader();
	CString CDescendants::getPlaceDateBlock(CString place, CString date, CString jel);
	CString CDescendants::getColoredString(CString str, int index);

	void CDescendants::printVectorTxt(int i);
	bool CDescendants::openTxt(CString file, CString title);
	void CDescendants::closeTxt();
	void CDescendants::printTxt(CString str);
	void CDescendants::printBeginingTxt(int i);
	void CDescendants::printDescendantTxt(int i);
	CString CDescendants::createDescendant(int i, bool parentIndex);
	void CDescendants::printSpousesTxt(int i);
	void CDescendants::printMarriageTxt(CString place, CString date, int i, int numberOfSpouses);
	void CDescendants::printSpouseTxt();
	void CDescendants::printSpRelativesTxt();

	bool CDescendants::queryR(CString rowid);

	BOOL CDescendants::queryP(CString command);
	BOOL CDescendants::queryM(CString command);

	BOOL CDescendants::query(CString command);
	BOOL CDescendants::query2(CString command);

	CString CDescendants::createSpouses(int i);
	CString CDescendants::createMarriage(CString place, CString date, int i, int numberOfSpouses);
	CString CDescendants::createSpouse();
	CString CDescendants::createSpRelatives();
	CString CDescendants::getComplexDescription(int i, bool parentIndex);

	void CDescendants::order_vDesc();
	void CDescendants::printvLen();
	void CDescendants::print_vDesc();
	void CDescendants::numOfDesc();
	void CDescendants::printVector(int tbl);
	void CDescendants::create_id();
	void CDescendants::printGAline();
	bool CDescendants::rowidExist();
	CString CDescendants::konvTitolo(CString titolo);

	DECLARE_MESSAGE_MAP()
public:

	std::vector<DE::DESC> vDesc;

	virtual BOOL OnInitDialog();

	void CDescendants::indentedHtml();

	afx_msg void OnClickedRepeatedAll();
	afx_msg void OnRepeatedFather();
	afx_msg void OnRepeatedFirst();
	afx_msg void OnPaint();
	afx_msg void OnClickedOrderInput();
	afx_msg void OnOrderLength();
	afx_msg void OnOrderDecreasing();
	afx_msg void OnOrderBirth();

	afx_msg void OnClickedTitololower();
};
