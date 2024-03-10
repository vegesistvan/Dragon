#pragma once
//#include "afxdialogex.h"

enum
{
	SZLUHA = 0,
	VIL,
	TUP,

};
namespace DN
{
	typedef struct
	{
		int g;
		CString rowid;
		CString name;
		CString sex;
		int numOfSpouses;
		int numOfChildren;
		int procChildren;
		CString parentSex;
		int parentIndex;
		int childorder;
		int motherIndex;
		bool hidden;
		int status;			// 0: alaphelyzet, listázandó (fekete)  1: ismétlõdõ elsõ bejegyzés, listázandó 2 (kék): ismétlõdõ második bejegyzés, nem listázandó(piros)
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
	}PEO;
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
} PPEOPLE;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDescendantsNew : public CWnd
{
	DECLARE_DYNAMIC(CDescendantsNew)

public:
	CDescendantsNew();
	virtual ~CDescendantsNew();
	BOOL CDescendantsNew::start();
	bool CDescendantsNew::tablesDescendants();  // listázandó táblák a theapp.v_tableNumbers vektorban
	CString m_rowid;
protected:

	CString str;
	CString m_command;
	CString m_htmlFile;
	CString m_os;
	_int64 m_startTime;

	CString m_txtFile;
// beállítható paraméterek //////////////////////
	
	int m_comboFontSize;
	
	CString m_editWidth;
	CString m_tablenumber;
	CString m_editGenMax;
	

	bool p_html;			// true: html lista készül, false: text fájl készül

	int p_numbering;
	int p_descStyle;
	int p_otherNameStyle;
	int p_commentStyle;
	int p_specStyle;
	bool p_capitalName;
	bool p_folyt;
	int	p_radioCode;
	int p_repeated;
	bool p_repeatedColor;
	bool p_oneFile;
	bool p_connect;
	bool p_mother;
	UINT	p_colorBgrnd;
	bool p_print;		// azonnal nyomtassa ki a leszármazottat, a már szükségtelen rekordokat dobja el a vDesc-bõl
///////////////////////////////////////////////
	int m_folyt_db;
	int m_cntRepeated;  // az ismétlõdõ leszármazottak száma

	CString m_descendantsPath;


	CString m_familyName;
	CString m_tableNumber;

	int		p_lastname;
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

	DN::PEO peoS;
	
	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a listázott generációl száma
	bool m_printed;
	int	m_givup;
	int m_maxDesc;
	bool m_canceled;

	CSqliteDBRecordSet rs;
	CSqliteDBRecordSet rsP;
	CSqliteDBRecordSet rsM;
	FILE* fl;
	FILE* text;
	FILE* txtOut;

	PPEOPLE p;
	PPEOPLE s;
	PPEOPLE sf;
	PPEOPLE sm;
	PPEOPLE ss;

	bool CDescendantsNew::parameters();
	void CDescendantsNew::descendants();
	void CDescendantsNew::descendantsOld();
	CString CDescendantsNew::getNextChildRowid(UINT i);
	BOOL CDescendantsNew::openHtml(CString file, CString title, UINT colorBgrnd);
	void CDescendantsNew::createStyle();
	void CDescendantsNew::closeHtml();
	void CDescendantsNew::queryPeople(CString rowid, PPEOPLE* p);
	void CDescendantsNew::print(CString str);
	void CDescendantsNew::printBegining( int i);
	void CDescendantsNew::printDescendant(int i);

	void CDescendantsNew::printSpouses(int i);
	void CDescendantsNew::printMarriage(CString place, CString date, int i, int numberOfSpouses);
	void CDescendantsNew::printSpouse();
	void CDescendantsNew::printSpRelatives();
	CString CDescendantsNew::getLastFirst(PPEOPLE* p);
	bool CDescendantsNew::voltmar(CString index);
	void CDescendantsNew::printVector(int i);
	int CDescendantsNew::getNumberOfChildren(CString rowid, CString sex_id);
	void CDescendantsNew::listDescendants();
	void CDescendantsNew::multipleRowid();
	void CDescendantsNew::multipleRowid0();
	void CDescendantsNew::signeD(int i, int j);
	CString CDescendantsNew::getTableHeader();
	CString CDescendantsNew::getPlaceDateBlock(CString place, CString date, CString jel);
	CString CDescendantsNew::getColoredString(CString str, int index);

	void CDescendantsNew::printVectorTxt(int i);
	bool CDescendantsNew::openTxt(CString file, CString title);
	void CDescendantsNew::closeTxt();
	void CDescendantsNew::printTxt(CString str);
	void CDescendantsNew::printBeginingTxt(int i);
	void CDescendantsNew::printDescendantTxt(int i);
	void CDescendantsNew::printSpousesTxt(int i);
	void CDescendantsNew::printMarriageTxt(CString place, CString date, int i, int numberOfSpouses);
	void CDescendantsNew::printSpouseTxt();
	void CDescendantsNew::printSpRelativesTxt();

	bool CDescendantsNew::queryR(CString rowid);

	BOOL CDescendantsNew::queryP(CString command);
	BOOL CDescendantsNew::queryM(CString command);

	BOOL CDescendantsNew::query(CString command);


	

	// descendantsOld változói
	int m_genPeer;
	CString m_lastnamePrev;
	CString m_sexidPrev;


	void CDescendantsNew::printGAline();
	bool CDescendantsNew::rowidExist();

	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnInitDialog();
//	afx_msg void OnTableDescendants();
};
