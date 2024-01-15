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
		
		bool printed;		// bejegyz�s ki�rva
		int	id;			// a line�ris list�z�sn�l a bejegyz�s id-je;
		int id2;			// rendez�s ut�ni id
		int idC;
		int idF;
		CString ixM;
		int nOfD;			// a bejegyz�s lesz�rmazottainak sz�ma
		bool clrText;
		bool clrTextBk;

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
	bool CDescendants::tablesDescendants();  // list�zand� t�bl�k a theapp.v_tableNumbers vektorban
	CString m_rowid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;

	int m_radioDNameX;
	int m_comboDAttrib;
	int	m_comboOtherName;
	int	m_comboSpec;
	int	m_comboComment;
	int	m_comboKiemeltAttrib;
	int m_comboFontSize;
	int m_comboHtmlText;
	int m_html;

	BOOL m_checkFolyt;
	BOOL m_checkCapital;
	BOOL m_checkBold;
	BOOL m_checkConnect;
	BOOL m_checkMother;
	BOOL m_ceckDeleteRepeated;
	BOOL m_checkCRLF;
	bool m_folyt;

	CComboBox m_combo_spec;
	CComboBox m_combo_comment;
	CComboBox m_combo_FontSize;
	CComboBox m_combo_OtherName;
	CComboBox m_combo_DAttrib;

	CButton m_RadioFamily;
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

	int		m_radioOrder;
	int		m_radioNumbering;
	int		m_repeated;
	BOOL	m_oneFile;

	UINT	m_colorBgrnd;
	CColorStatic	colorBgrnd;
	CColorButton	colorBgn;
	CString m_editWidth;
	CString m_editGenMax;

	BOOL m_radioOne;
	BOOL m_repeatedColor;
	CColorStatic colorRepeated;
	CComboBox m_comboHtmlTxt;
	bool m_printed;
	CString m_tableNumber;

	int m_generationMax;

	void CDescendants::setParameters();
	void CDescendants::OnClickedDefault();
	void CDescendants::updateParameters();
	void CDescendants::updateRadioDName();
	void CDescendants::updateRepeated();
	void CDescendants::updateRadioNumbering();
	void CDescendants::OnClickedCheckConnect();
	void CDescendants::OnClickedCheckWoman();
	void CDescendants::OnClickedCheckFolyt();
	void CDescendants::OnClickedCheckCapital();
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

	CString m_command;
	CString m_htmlFile;
	CString m_os;
	_int64 m_startTime;

	CString m_txtFile;
	// be�ll�that� param�terek //////////////////////



	CString m_tablenumber;

	bool m_log;
	bool p_html;			// true: html lista k�sz�l, false: text f�jl k�sz�l
	int  p_numbering;
	int  p_descStyle;
	int  p_otherNameStyle;
	int  p_commentStyle;
	int  p_specStyle;
	bool p_capitalName;
	bool p_checkBold;
	bool p_folyt;
	int	 p_radioCode;
	int  p_repeated;
	bool p_repeatedColor;
	bool p_oneFile;
	bool p_connect;
	bool p_mother;
	UINT p_colorBgrnd;
	bool p_print;		// azonnal nyomtassa ki a lesz�rmazottat, a m�r sz�ks�gtelen rekordokat dobja el a vDesc-b�l. Ez hiba keres�shez j�.
	bool p_checkCRLF;
	int	 p_radioOrder;
	///////////////////////////////////////////////
	//	int m_folyt_db;
	int m_cntRepeated;  // az ism�tl�d� lesz�rmazottak sz�ma

	CString m_descendantsPath;


	CString m_familyName;

	int		p_lastname;
	int		m_listedD;
	int		m_listedP;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// beh�z�sok aktu�lis sz�ma( <ol>/<il>-ek sz�ma ) 


	std::vector<UINT>vSerial;			// A Meurgey de Tupigny sz�moz�shoz a sorsz�mok nyilv�ntart�sa
	std::vector<CString>vSpouseIndex;
	std::vector<CString>vFullname;		// m�r list�zott h�zast�rsak neve
	std::vector<CString>vRowid;			// m�r list�zott h�zast�rsak neve
	std::vector<DE::LMX>vLMX;				// a gener�ci� utols� ki�rt motherIndexe a lesz�rmazotti list�ban
	std::vector<DE::LEN>* vLenOu;
	std::vector<int> vSib;

	DE::PEO peoS;

	int m_genPrev;
	int m_actGen;
	int m_gen;
	int m_gMax;			// a list�zott gener�ci�l sz�ma
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

	void CDescendants::descendants();
	void CDescendants::descendantsOld();
	CString CDescendants::getNextChildRowid(UINT i);
	BOOL CDescendants::openHtml(CString file, CString title, UINT colorBgrnd);
	void CDescendants::createStyle();
	void CDescendants::closeHtml();
	void CDescendants::queryPeople(CString rowid, DE::PPEOPLE* p);
	void CDescendants::print(CString str);
	void CDescendants::printBegining(int i);
	void CDescendants::printDescendant(int i);

	void CDescendants::printSpouses(int i);
	void CDescendants::printMarriage(CString place, CString date, int i, int numberOfSpouses);
	void CDescendants::printSpouse();
	void CDescendants::printSpRelatives();
	CString CDescendants::getLastFirst(DE::PPEOPLE* p);
	bool CDescendants::voltmar(CString index);
	int CDescendants::getNumberOfChildren(CString rowid, CString sex_id);
	void CDescendants::listDescendants();
	void CDescendants::multipleRowid();
	void CDescendants::multipleRowid0();
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


	bool CDescendants::linearTable(int i, int which);
	void CDescendants::dataTable(int i);
	bool CDescendants::printTopContainer(CString title, int which);
	void CDescendants::printOnly(CString str, int which);
	CString CDescendants::createSpouses(int i);
	CString CDescendants::createMarriage(CString place, CString date, int i, int numberOfSpouses);
	CString CDescendants::createSpouse();
	CString CDescendants::createSpRelatives();
	CString CDescendants::getComplexDescription(int i, bool parentIndex);
	TCHAR CDescendants::get_gABC(int g);

	void CDescendants::order_vDesc();
	void CDescendants::printvLen();
	void CDescendants::print_vDesc();
	void CDescendants::printDesc(int i, int which);
	void CDescendants::numOfDesc();
	int CDescendants::sort_vDesc();
	void CDescendants::printVector(int tbl);
	void CDescendants::create_id();
	// descendantsOld v�ltoz�i
	int m_genPeer;
	CString m_lastnamePrev;
	CString m_sexidPrev;


	void CDescendants::printGAline();
	bool CDescendants::rowidExist();



	DECLARE_MESSAGE_MAP()
public:

	std::vector<DE::DESC> vDesc;

	virtual BOOL OnInitDialog();

	void CDescendants::indentedHtml();

	afx_msg void OnClickedRepeatedAll();
	afx_msg void OnRepeatedFather();
	afx_msg void OnRepeatedFirst();
	afx_msg void OnPaint();
	afx_msg void OnClickedCheckBold();
	afx_msg void OnClickedOrderInput();
	afx_msg void OnOrderLength();
	afx_msg void OnOrderDecreasing();
	afx_msg void OnOrderBirth();
};
