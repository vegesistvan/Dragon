
#pragma once


enum 
{
	SZLUHA = 0,
	VIL,
	TUP,

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
	CString	generation;			// gener�ci�s karakter lesz�razott eset�n
	CString	sex_id;				// 1=f�rfi, 2= n�
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
	CString folyt;						// "folyt" ut�ni r�mai sz�m, ha van.
	CString arm;						// [] z�r�jelben l�v� string a csal�d �ga [ arm ] 
	CString csalad;
	CString	gap;
	CString occupation;
} PPEOPLE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A lesz�rmazotti lista egy ciklusban k�sz�l, amelyben a vector<DESCENDANTS> = vDesc vektor bejegyz�seit haszn�ljuk
// a list�zand� ember azonos�t�j�nak nyilv�ntart�s�ra valamint a lesz�rmazotti l�nc felg�nygy�l�t�s�re.
// A vDEsc vektor indexe a gener�ci� sorsz�ma.
//
// A 0 index� bejegyz�s a lesz�rmazotti lista �s�t tartlamazza, aki a nulladik gener�ci�.
// Ha egy bejegyz�sben szerepl� embernek van gyermeke, akkor egy �j bejegyz�st hoz l�tre, amelyben a gyermek ugyanolyan
// adatai vannak megadva.
//
// Ha nincs gyermeke, vagy m�r minden gyermeket list�ztunk, akkor visszmegy�nk a legutols� olyan genber�ci�hoz,
// amelyiknek m�g van ki nem nyomtatott gyermeke. Az enn�l magasabb gener�ci�k bejegyz�seit, amelyek m�r minden gyermek�t
// list�ztuk, eldobjuk. 
//
// Ha a lesz�rmazotti t�bl�t nem egy �s lesz�rmazottaib�l k�sz�tj�k, hanem egy GA.html t�bl�t k�sz�t�nk, akkor ha a
// 0. gener�ci�s bejegyz�snek van apja, akkor azt tartjuk nyilv�n 0. gener�ci�sk�nt, de hidden-k�nt, ami azt jelenti,
// hogy nem list�zzuk. Az�rt van � nyilv�ntartava, hoyg ha t�bb gyereke van, akkor azokat 
typedef struct
{
	CString rowid;					// a list�zand� ember azonos�t�ja
	int		sex_id;					// a neme, a le�ny�gi lesz�rmazottak list�z�s�hoz sz�ks�ges
	int		numOfSpouses;			// az � h�zast�rsainak sz�ma
	int		numOfChildren;			// h�ny gyereke van
	int		childrenPrinted;		// h�ny gyereket list�ztunk m�r
	int		parentIndexLast;		// mi volt az utolj�ra list�zott gyermek anya-indexe
	BOOL	hidden;					// ha van apja az �snek ( %%% folyt ), akkor az ap�t hidden-k�nt elteszi 
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
	L"f�lk�v�r",	L"<b>",						L"</b>",
	L"d�lt",		L"<i>",						L"</i>",
	L"k�k",			L"<font color='blue'>",		L"</font>",
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

	// bemeneti param�terek
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
	UINT	m_colorBgrnd;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>
	int		m_indent;	// beh�z�sok aktu�lis sz�ma( <ol>/<il>-ek sz�ma ) 

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

	std::vector<DESCENDANTS> vDesc;		// A vDesc vektoban egy lesz�rmazott addig marad benn, am�g minden gyermek�t list�ztuk
										// a vDesc.size() -1 a gener�ci� sorsz�ma
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny sz�moz�shoz a sorsz�mok nyilv�ntart�sa

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
