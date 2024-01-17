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
namespace DN
{
	typedef struct
	{
		int original;
		int g;
		int order;			// 1,2,3,4,5... a lesz�rmazotti sor hossz�s�g szerinti sorsz�mok. 1 a leghosszabb. 
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
		int status;			// 0: alaphelyzet, list�zand� (fekete)  1: ism�tl�d� els� bejegyz�s, list�zand� 2 (k�k): ism�tl�d� m�sodik bejegyz�s, nem list�zand�(piros)
		int id2;			// rendez�s ut�ni id
		bool printed;		// bejegyz�s ki�rva
		int	id;				// a line�ris list�z�sn�l a bejegyz�s id-je;
		int nOfD;			// a bejegyz�s lesz�rmazottainak sz�ma
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
		int g;					// vizsg�lt gener�ci�
		int multiplyer;			// gener�ci�hoz rendelt szorz�
		int ixFirst;			// intervallum vGKR beli els� indexe
		int ixLast;				// intervallum vGKR beli utols� indexe
		int order;				// intervallum hossza * multiplyer         
		int gBack;				// az intervallumban t�rt�nt visszaugrott gener�ci�. Ha nincs akkor 0 
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
	L"norm�l ",		L"",						L"",
	L"f�lk�v�r",	L"<b>",						L"</b>",
	L"d�lt",		L"<i>",						L"</i>",
	L"k�k",			L"<font color='blue'>",		L"</font>",
	L"piros",		L"<font color='red'>",		L"</font>",
	L"z�ld",		L"<font color='green'>",	L"</font>",
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
// lesz�rmazotti list�n�l p, s, sf, sm, ss struktur�k
typedef struct
{
	CString	lineNumber;
	CString	rowid;
	CString	tableAncestry;			// TRUE: egy t�bla �se
	CString tableNumber;
	CString tableRoman;	// a t�bla fejl�c r�mai sz�ma
	CString	source;				// az ember t�pusa 1=lesz�rmazott, 2=h�zast�rsa, 3=h�zast�rs apja, 4=h�zast�rs anyja, 5=h�zast�rs tov�bbi h�zast�rsa
	CString generation;
	CString	sex_id;				// 1=f�rfi, 2= n�
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
	CString folyt;						// "folyt" ut�ni r�mai sz�m, ha van.
	CString arm;						// [] z�r�jelben l�v� string a csal�d �ga [ arm ] 
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
	bool CDescendantsOld::tablesDescendants();  // list�zand� t�bl�k a theapp.v_tableNumbers vektorban
	bool CDescendantsOld::linearTable2(int i, int which);
	CString m_rowid;
protected:

	CString str;
	CString m_command;
	CString m_htmlFile;
	CString m_os;
	_int64 m_startTime;

	CString m_txtFile;
// be�ll�that� param�terek //////////////////////
	
	int m_comboFontSize;
	
	CString m_editWidth;
	CString m_tablenumber;
	CString m_editGenMax;
	
	bool m_log;
	bool p_html;			// true: html lista k�sz�l, false: text f�jl k�sz�l
	int  p_numbering;
	int  p_descStyle;
	int  p_otherNameStyle;
	int  p_commentStyle;
	int  p_specStyle;
	bool p_capitalName;
	bool p_bold;
	bool p_folyt;
	int	 p_radioCode;
	int  p_repeated;
	bool p_repeatedColor;
	bool p_oneFile;
	bool p_connect;
	bool p_womenDescendants;
	UINT p_colorBgrnd;
	bool p_print;		// azonnal nyomtassa ki a lesz�rmazottat, a m�r sz�ks�gtelen rekordokat dobja el a vDesc-b�l. Ez hiba keres�shez j�.
	bool p_checkCRLF;
	int	 p_radioOrder;
///////////////////////////////////////////////
//	int m_folyt_db;
	int m_cntRepeated;  // az ism�tl�d� lesz�rmazottak sz�ma

	CString m_descendantsPath;


	CString m_familyName;
	CString m_tableNumber;

	int		p_lastname;
	int		m_listedD;
	int		m_listedP;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// beh�z�sok aktu�lis sz�ma( <ol>/<il>-ek sz�ma ) 
	
	std::vector<DN::DESC> vDesc;
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny sz�moz�shoz a sorsz�mok nyilv�ntart�sa
	std::vector<CString>vSpouseIndex;
	std::vector<CString>vFullname;		// m�r list�zott h�zast�rsak neve
	std::vector<CString>vRowid;			// m�r list�zott h�zast�rsak neve
	std::vector<LMX>vLMX;				// a gener�ci� utols� ki�rt motherIndexe a lesz�rmazotti list�ban
	std::vector<DN::LEN>* vLenOu;
	std::vector<int> vSib;

	DN::PEO peoS;
	
	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a list�zott gener�ci�l sz�ma
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


	// descendantsOld v�ltoz�i
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
