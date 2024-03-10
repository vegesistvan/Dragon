
#pragma once


enum 
{
	SZLUHA = 0,
	VIL,
	TUP,

};
//CString	generation;			// gener�ci�s karakter lesz�razott eset�n
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
	CString folyt;						// "folyt" ut�ni r�mai sz�m, ha van.
	CString arm;						// [] z�r�jelben l�v� string a csal�d �ga [ arm ] 
	CString csalad;
	CString	gap;
	CString occupation;
	CString fullname;
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
	CString	sex_id;					// a neme, a le�ny�gi lesz�rmazottak list�z�s�hoz sz�ks�ges
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
	L"norm�l ",		L"",						L"",
	L"f�lk�v�r",	L"<b>",						L"</b>",
	L"d�lt",		L"<i>",						L"</i>",
	L"k�k",			L"<font color='blue'>",		L"</font>",
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
// feldolgoz�si param�terek

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

	CString m_birth;		// az utolj�ra kinyomtatott ember sz�let�si ideje

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
	int		m_indent;	// beh�z�sok aktu�lis sz�ma( <ol>/<il>-ek sz�ma ) 

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

	std::vector<DESCENDANTS> vDesc;		// A vDesc vektoban egy lesz�rmazott addig marad benn, am�g minden gyermek�t list�ztuk
										// a vDesc.size() -1 a gener�ci� sorsz�ma
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny sz�moz�shoz a sorsz�mok nyilv�ntart�sa

	std::vector<CString>vRowid;			// list�zott lesz�rmazottak rowid-ja
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
