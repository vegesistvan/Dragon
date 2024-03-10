
#pragma once


enum 
{
	SZLUHA = 0,
	VIL,
	TUP,

};
//CString	generation;			// generációs karakter leszárazott esetén
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
	CString birth_place ;
	CString birth_date;
	CString death_place ;
	CString death_date;
	CString comment;
	CString	father_id;
	CString	mother_id;
	int		parentIndex;
	CString whichHusband;
	CString whichWife;
	CString whichChildFather;
	CString	whichChildMother;
	CString folyt;						// "folyt" utáni római szám, ha van.
	CString arm;						// [] zárójelben lévõ string a család ága [ arm ] 
	CString csalad;
	CString	gap;
	CString occupation;
	CString fullname;
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
	CString	sex_id;					// a neme, a leányági leszármazottak listázásához szükséges
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
	L"normál ",		L"",						L"",
	L"félkövér",	L"<b>",						L"</b>",
	L"dõlt",		L"<i>",						L"</i>",
	L"kék",			L"<font color='blue'>",		L"</font>",
	L"piros",		L"<font color='red'>",		L"</font>",
};

enum
{
	NORMAL = 0,
	FELKOVER,
	DOLT,
	KEK,
	PIROS
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "afxwin.h"
#include "colorstatic.h"
#include "colorbutton.h"
class CDescendants : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendants)
	DECLARE_MESSAGE_MAP()

public:
	CDescendants(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDescendants();

	enum { IDD = IDD_DESCENDANTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// feldolgozási paraméterek

	BOOL m_orderBirth;

	int m_radioDNameX;
	int m_comboDAttrib;
	int	m_comboOtherName;
	int	m_comboSpec;
	int	m_comboComment;
	int	m_comboKiemeltAttrib;
	int m_comboFontSize;

	BOOL m_checkFolyt;
//	BOOL m_check_titolo;
	BOOL m_checkCapital;
	BOOL m_checkConnect;
	BOOL m_checkMother;
	BOOL m_checkDeleteRepeated;

	CComboBox m_combo_spec;
	CComboBox m_combo_comment;
//	CComboBox m_combo_KiemeltAttrib;
	CComboBox m_combo_FontSize;
	CComboBox m_combo_OtherName;
	CComboBox m_combo_DAttrib;
	
	CButton m_RadioFamily;
	CColorStatic colorFamilyInline;
	CColorStatic colorFamilynameNo;
	CColorStatic colorFamilynameUp;
	CColorStatic colorOrderText;
//	CColorStatic colorPrint;

	int		m_radioNumbering;
	BOOL	m_radioCode;
	BOOL	m_oneFile;

	UINT	m_colorBgrnd;

	CString m_editWidth;
	CString m_editGenMax;
	CString m_descendantsPath;

	int		m_generationMax;

	_int64 m_startTime;

	int		m_db;
	CColorStatic colorName;
	CColorStatic colorNum;
	CColorStatic colorCode;
	CColorStatic colorAttrib;
	CColorStatic colorContent;

	
	CColorStatic	colorBgrnd;
	CColorButton	colorBgn;

	CString m_birth;		// az utoljára kinyomtatott ember születési ideje

	BOOL m_join;
	BOOL m_limited;
	bool m_printed;


	void parameters();
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString str;
	CString m_command;
	CString m_htmlFile;
	CString cLine;

	int		m_gen;
	int		m_genPrev;
	int		m_genPeer;


	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// behúzások aktuális száma( <ol>/<il>-ek száma ) 

	CString	m_sex_id;
	CString m_mother_index;
	CString m_familyName;
	CString m_last_name_prev;
	CString m_sex_id_prev;
	CString m_posterior;
	CString m_tableNumber;
	CString m_name;

	int		m_parentindexLast;

	HBRUSH* m_brush;

	BOOL m_numOfSpouses;
	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;
	CSqliteDBRecordSet recP;

	CButton m_checkOneFile;
	BOOL m_radioOne;

	PPEOPLE p;
	PPEOPLE s;
	PPEOPLE sf;
	PPEOPLE sm;
	PPEOPLE ss;

	DESCENDANTS desc;

	FILE* fl;

	std::vector<DESCENDANTS> vDesc;		// A vDesc vektoban egy leszármazott addig marad benn, amíg minden gyermekét listáztuk
										// a vDesc.size() -1 a generáció sorszáma
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása

	std::vector<CString>vRowid;			// listázott leszármazottak rowid-ja
	std::vector<CString>vSpouseIndex;
	std::vector<CString>vFullname;


	

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	

	void CDescendants::treeTables();
	void CDescendants::treePeople();

	BOOL openHtml( CString file, CString title, UINT colorBgrnd );
	void closeHtml();
	void queryPeople( CString rowid, PPEOPLE* p );
	void queryP( CString rowid );
	int	 getNumberOfChildren( CString rowid, CString ix );
	CString getNextChildRowid();
	void descendants();
	bool CDescendants::voltmar(CString index);

	void print( CString str );
	void printGAline();
	void printBegining();
	void printDescendant();
	void printMarriages();
	void printMarriage( CString place, CString date, int i, int numberOfSpouses );
	void printSpouse();
	void printSpRelatives();
	CString CDescendants::getTableHeader();
	bool CDescendants::rowidExist();
	void CDescendants::setParameters();
	void CDescendants::updateParameters();
	void CDescendants::updateRadioDName();
	void CDescendants::updateRadioNumbering();
	void createStyle();

	void CDescendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color);
	CString getColoredString( CString str, int index );

	CString getLastFirst( PPEOPLE* p );
	CString getPlaceDateBlock( CString place, CString date, CString jel );
	
//	CString getTableHeader();

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckWoman();
	afx_msg void OnClickedCheckConnect();
	afx_msg void OnClickedSzluha();
	afx_msg void OnTupigny();
	afx_msg void OnVillers();
	afx_msg void OnClickedStaticBackground();
//	afx_msg void OnClickedCheckTitolo();
	afx_msg void OnClickedCheckCapital();
	afx_msg void OnClickedRadioNofamilyname();
	afx_msg void OnRadioFamilyname();
	afx_msg void OnRadioFamilynameup();
	afx_msg void OnClickedCheckFolyt();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedDefault();
	virtual BOOL OnInitDialog();

	int generation;
public:
	CColorStatic colorOutput;
	CColorStatic colorPrint;
};
