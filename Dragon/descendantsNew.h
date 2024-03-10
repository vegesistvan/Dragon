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
		int status;			// 0: alaphelyzet, list�zand� (fekete)  1: ism�tl�d� els� bejegyz�s, list�zand� 2 (k�k): ism�tl�d� m�sodik bejegyz�s, nem list�zand�(piros)
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
} PPEOPLE;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDescendantsNew : public CWnd
{
	DECLARE_DYNAMIC(CDescendantsNew)

public:
	CDescendantsNew();
	virtual ~CDescendantsNew();
	BOOL CDescendantsNew::start();
	bool CDescendantsNew::tablesDescendants();  // list�zand� t�bl�k a theapp.v_tableNumbers vektorban
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
	

	bool p_html;			// true: html lista k�sz�l, false: text f�jl k�sz�l

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
	bool p_print;		// azonnal nyomtassa ki a lesz�rmazottat, a m�r sz�ks�gtelen rekordokat dobja el a vDesc-b�l
///////////////////////////////////////////////
	int m_folyt_db;
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

	DN::PEO peoS;
	
	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a list�zott gener�ci�l sz�ma
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


	

	// descendantsOld v�ltoz�i
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
