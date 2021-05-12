
#pragma once


enum 
{
	SZLUHA = 0,
	VIL,
	TUP,

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
	CString	generation;			// generációs karakter leszárazott esetén
	CString	sex_id;				// 1=férfi, 2= nõ
	CString title;				
	CString titolo;
	CString first_name;
	CString last_name;
	CString posterior;
	CString other_names;
	CString known_as;
	CString birth_place ;
	CString birth_date;
	CString death_place ;
	CString death_date;
	CString comment;
	CString	father_id;
	CString	mother_id;
	int		parentIndex;
	int		parentIndexCalc;
	CString orderFather;
	CString	orderMother;
	CString folyt;						// "folyt" utáni római szám, ha van.
	CString arm;						// [] zárójelben lévõ string a család ága [ arm ] 
	CString csalad;
	CString	gap;
	CString occupation;
} PPEOPLE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A leszármazotti lista egy ciklusban készül, amelyben a vector<DESCENDANTS> = vDesc vektor bejegyzéseit használjuk
// a listázandó ember azonosítójának nyilvántartására valamint a leszármazotti lánc felgönygyölítésére.
// A vDEsc vektor indexe a generáció sorszáma.
//
// A 0 indexû bejegyzés a leszármazotti lista õsét tartlamazza, aki a nulladik generáció.
// Ha egy bejegyzésben szereplõ embernek van gyermeke, akkor egy új bejegyzést hoz létre, amelyben a gyermek ugyanolyan
// adatai vannak megadva.
//
// Ha nincs gyermeke, vagy már minden gyermeket listáztunk, akkor visszmegyünk a legutolsó olyan genberációhoz,
// amelyiknek még van ki nem nyomtatott gyermeke. Az ennél magasabb generációk bejegyzéseit, amelyek már minden gyermekét
// listáztuk, eldobjuk. 
//
// Ha a leszármazotti táblát nem egy õs leszármazottaiból készítjük, hanem egy GA.html táblát készítünk, akkor ha a
// 0. generációs bejegyzésnek van apja, akkor azt tartjuk nyilván 0. generációsként, de hidden-ként, ami azt jelenti,
// hogy nem listázzuk. Azért van õ nyilvántartava, hoyg ha több gyereke van, akkor azokat 
typedef struct
{
	CString rowid;					// a listázandó ember azonosítója
	int		sex_id;					// a neme, a leányági leszármazottak listázásához szükséges
	int		numOfSpouses;			// az õ házastársainak száma
	int		numOfChildren;			// hány gyereke van
	int		childrenPrinted;		// hány gyereket listáztunk már
	int		parentIndexLast;		// mi volt az utoljára listázott gyermek anya-indexe
	BOOL	hidden;					// ha van apja az õsnek ( %%% folyt ), akkor az apát hidden-ként elteszi 
}DESCENDANTS;

typedef struct
{
	CString text;
	CString code1;
	CString code2;
}ATTRIB;

const ATTRIB attrib[] =
{
	L" ",			L"",						L"",
	L"félkövér",	L"<b>",						L"</b>",
	L"dõlt",		L"<i>",						L"</i>",
	L"kék",			L"<font color='blue'>",		L"</font>",
	L"piros",		L"<font color='red'>",		L"</font>",
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "afxwin.h"
#include "colorstatic.h"
#include "colorbutton.h"
class CGaDescendants : public CDialogEx
{
	DECLARE_DYNAMIC(CGaDescendants)

public:
	CGaDescendants(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaDescendants();

	enum { IDD = IDD_GA_DESCENDANTS };

	// bemeneti paraméterek
	BOOL	m_code;
	CString m_rowid1;
	CString m_name;
	CString m_tableNumber;
	int		m_source;
	int		m_numbering;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	CString m_htmlFile;
	CString cLine;

	int		m_gen;
	int		m_genPrev;
	int		cnt_ol;
	UINT	m_colorBgrnd;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>

	int		m_sex_id;
	CString m_mother_index;
	CString m_familyName;


	int		m_parentindexLast;
	BOOL	m_checkFamily;

	BOOL m_numOfSpouses;
	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;
	CSqliteDBRecordSet recP;

	PPEOPLE p;
	PPEOPLE s;
	PPEOPLE sf;
	PPEOPLE sm;
	PPEOPLE ss;

	FILE* fl;

	std::vector<DESCENDANTS> vDesc;		// A vDesc vektoban egy leszármazott addig marad benn, amíg minden gyermekét listáztuk
										// a vDesc.size() -1 a generáció sorszáma
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása

	CComboBox m_ComboSpec;
	CComboBox m_ComboComm;
	CComboBox m_ComboName;
	CComboBox m_ComboFamily;
	CComboBox m_ComboFontSize;

	CButton m_szluhaCtrl;

	DESCENDANTS desc;

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	
	void CGaDescendants::treePeople();
	void CGaDescendants::treeTables();

	BOOL openHtml( CString file, CString title, UINT colorBgrnd );
	void closeHtml();
	void queryPeople( CString rowid, PPEOPLE* p );
	void queryP( CString rowid );
	int	 getNumberOfChildren( CString rowid, int ix );
	CString getNextChildRowid();
	void descendants();

	void print( CString str );
	void printGAline();
	void printBegining();
	void printDescendant();
	void printMarriages();
	void printMarriage( CString place, CString date, int i, int numberOfSpouses );
	void printSpouse();
	void printSpRelatives();

	void createStyle();

	CString getColoredString( CString str, int index );

	CString getLastFirst( PPEOPLE* p );
	CString getFullname( PPEOPLE* p );
	CString getPlaceDateBlock( CString place, CString date, CString jel );
	
	CString getFamilyName();

	int	m_ixName;
	int	m_ixSpec;
	int	m_ixComment;
	int	m_ixFamily;
	int m_ixFontSize;

	BOOL m_join;
	BOOL m_limited;

	BOOL m_CheckLastName;

	int m_setCombo;
	
	BOOL m_connect;
	BOOL m_woman;
	BOOL m_wrap;	

	CColorStatic colorBgrnd;
	CColorButton colorBgn;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedRadioClear();
	afx_msg void OnRadioDefault();
	afx_msg void OnClickedCheckWoman();
	afx_msg void OnClickedCheckConnect();
	afx_msg void OnClickedSzluha();
	afx_msg void OnTupigny();
	afx_msg void OnVillers();
	afx_msg void OnClickedCheckFamily();
	afx_msg void OnClickedCheckLastname();
	afx_msg void OnClickedButtonDefault();
	afx_msg void OnClickedStaticBackground();

	virtual BOOL OnInitDialog();
//public:
//	virtual BOOL OnInitDialog();



};
