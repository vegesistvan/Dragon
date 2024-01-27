#pragma once
//#include "afxdialogex.h"

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


namespace DN
{
	typedef struct
	{
		int original;
		int g;
		int order;			// 1,2,3,4,5... a leszármazotti sor hosszúság szerinti sorszámok. 1 a leghosszabb. 
		CString rowid;
		CString rowidF;		// apa rowid-ja
		CString rowidM;		// anya rowid-ja
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
		int id2;			// rendezés utáni id
		bool printed;		// bejegyzés kiírva
		int	id;				// a lineáris listézásnál a bejegyzés id-je;
		int nOfD;			// a bejegyzés leszármazottainak száma
	}DESC;


	typedef struct
	{
		CString tablenumber;
		CString	sex;
		CString posterior;
		CString firstname;
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
}
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDescendantsOld : public CWnd
{
	DECLARE_DYNAMIC(CDescendantsOld)

public:
	CDescendantsOld();
	virtual ~CDescendantsOld();
	BOOL CDescendantsOld::start();
	bool CDescendantsOld::tablesDescendants();  // listázandó táblák a theapp.v_tableNumbers vektorban
	bool CDescendantsOld::linearTable2(int i, int which);
	CString m_rowid;
protected:

	CString str;
	CString m_command;
	CString m_htmlFile;
	CString m_os;
	_int64 m_startTime;

	CString m_txtFile;
// beállítható paraméterek //////////////////////
	
	int p_fontSize;
	
	CString p_rowWidth;
	CString m_tablenumber;
	CString p_generationMax;
	
	bool m_log;

	int	p_descendantName;
	int p_numberingSystem;
	int p_descendantAttrib;
	int p_otherNameAttrib;
	int p_commentAttrib;
	int p_specAttrib;
	bool p_capitalName;
	bool p_bold;
	bool p_folyt;
	int  p_repeated;
	bool p_repeatedColor;
	bool p_oneOutputFile;
	bool p_connect;
	bool p_womenDescendants;
	UINT p_colorBgrnd;
	bool p_print;		// azonnal nyomtassa ki a leszármazottat, a már szükségtelen rekordokat dobja el a vDesc-bõl. Ez hiba kereséshez jó.
	bool p_checkCRLF;
	int	 p_childrenOrder;
///////////////////////////////////////////////

	int m_cntRepeated;  // az ismétlõdõ leszármazottak száma

	CString m_descendantsPath;


	CString m_familyName;
	CString m_tableNumber;

	
	int		m_listedD;
	int		m_listedP;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// behúzások aktuális száma( <ol>/<il>-ek száma ) 
	
	std::vector<DN::DESC> vDesc;
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása
	std::vector<CString>vSpouseIndex;
	std::vector<CString>vFullname;		// már listázott házastársak neve
	std::vector<CString>vRowid;			// már listázott házastársak neve
	std::vector<LMX>vLMX;				// a generáció utolsó kiírt motherIndexe a leszármazotti listában
	std::vector<DN::LEN>* vLenOu;
	std::vector<int> vSib;

	DN::PEO peoS;
	
	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a listázott generációl száma
	bool gflag = true;
	bool m_printed;
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

	PPEOPLE p;
	PPEOPLE s;
	PPEOPLE sf;
	PPEOPLE sm;
	PPEOPLE ss;

	bool CDescendantsOld::parameters();
	void CDescendantsOld::descendants();
	void CDescendantsOld::descendantsOld();
	CString CDescendantsOld::getNextChildRowid(UINT i);
	BOOL CDescendantsOld::openHtml(CString file, CString title, UINT colorBgrnd);
	void CDescendantsOld::createStyle();
	void CDescendantsOld::closeHtml();
	void CDescendantsOld::queryPeople(CString rowid, PPEOPLE* p);
	void CDescendantsOld::print(CString str);
	void CDescendantsOld::printBegining( int i);
	void CDescendantsOld::printDescendant(int i);

	void CDescendantsOld::printSpouses(int i);
	void CDescendantsOld::printMarriage(CString place, CString date, int i, int numberOfSpouses);
	void CDescendantsOld::printSpouse();
	void CDescendantsOld::printSpRelatives();
	CString CDescendantsOld::getLastFirst(PPEOPLE* p);
	bool CDescendantsOld::voltmar(CString index);
	void CDescendantsOld::printVector(int i);
	int CDescendantsOld::getNumberOfChildren(CString rowid, CString sex_id);
	void CDescendantsOld::listDescendants();
	void CDescendantsOld::multipleRowid();
	void CDescendantsOld::multipleRowid0();
	void CDescendantsOld::signeD(int i, int j);
	CString CDescendantsOld::getTableHeader();
	CString CDescendantsOld::getPlaceDateBlock(CString place, CString date, CString jel);
	CString CDescendantsOld::getColoredString(CString str, int index);

	void CDescendantsOld::printVectorTxt(int i);
	bool CDescendantsOld::openTxt(CString file, CString title);
	void CDescendantsOld::closeTxt();
	void CDescendantsOld::printTxt(CString str);
	void CDescendantsOld::printBeginingTxt(int i);
	void CDescendantsOld::printDescendantTxt(int i);
	CString CDescendantsOld::createDescendant(int i, bool parentIndex );
	void CDescendantsOld::printSpousesTxt(int i);
	void CDescendantsOld::printMarriageTxt(CString place, CString date, int i, int numberOfSpouses);
	void CDescendantsOld::printSpouseTxt();
	void CDescendantsOld::printSpRelativesTxt();

	bool CDescendantsOld::queryR(CString rowid);

	BOOL CDescendantsOld::queryP(CString command);
	BOOL CDescendantsOld::queryM(CString command);

	BOOL CDescendantsOld::query(CString command);
	BOOL CDescendantsOld::query2(CString command);


	bool CDescendantsOld::linearTable(int i, int which);
	void CDescendantsOld::dataTable(int i);
	bool CDescendantsOld::printTopContainer(CString title, int which);
	void CDescendantsOld::printOnly(CString str, int which);
	CString CDescendantsOld::createSpouses(int i);
	CString CDescendantsOld::createMarriage(CString place, CString date, int i, int numberOfSpouses);
	CString CDescendantsOld::createSpouse();
	CString CDescendantsOld::createSpRelatives();
	CString CDescendantsOld::getComplexDescription(int i, bool parentIndex );
	TCHAR CDescendantsOld::get_gABC(int g);

	void CDescendantsOld::order_vDesc();
	void CDescendantsOld::printvLen();
	void CDescendantsOld::print_vDesc();
	void CDescendantsOld::printDesc( int i, int which );
	void CDescendantsOld::numOfDesc();
	int CDescendantsOld::sort_vDesc();


	// descendantsOld változói
	int m_genPeer;
	CString m_lastnamePrev;
	CString m_sexidPrev;


	void CDescendantsOld::printGAline();
	bool CDescendantsOld::rowidExist();

	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnInitDialog();
//	afx_msg void OnTableDescendants();
};
