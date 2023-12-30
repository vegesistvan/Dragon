#pragma once
#include "inputGA_typedef.h"
#include "ColorStatic.h"
#define TEST false  // beolvasás során kiírja a ga.htm sorok sorszámát debugolás céljából


const PEERS peers[] =
{
	{L"br", L"báró"},
	{L"gr", L"gróf"},
	{L"õrgr", L"õrgróf"},
	{L"hg", L"herceg"}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CInputGA : public CDialogEx
{
	DECLARE_DYNAMIC(CInputGA)
public:
	CInputGA(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputGA();

	enum { IDD = IDD_INPUTGA };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// F E L B O N T Á S H O Z   H A S Z N Á L T   V Á L T O Z Ó K ,  S T R I U K T U R Á K ,  V E K T O R O K ////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<FOLYT>			vFolyt;
	std::vector<GENERATIONS>	v_generations;
	TABLEHEADER m_tableHeader;
	std::vector<TABLEHEADER>	v_tableHeader;


	FILE* textF;
	CString m_textFilePath;

	struct 
	{
		CString nameSubstr;
		CString birthSubstr;
		CString deathSubstr;
	}descS;

// sorok szétszedése
	CString	m_generationFirst; // egy tábla õsének generációja ( az elágazások összekapcsolásához kell )
	int		m_rowid;	// az utoljára insertált ember azonosítója. Azért számoljuk és nem visszakérdezzük a SELECT 'last_insert_rowid'-val
						// mert ez magszakítaná a BEGIN_COMMIT tranzakciót és nagyon lelassulna a beolvasás!!!
						// Az m_rowid-ra egyébként azért van szükség, hogy a házastársak rowid-párjait gyûjtsük és a marriages-táblában mrgõrizzük.
						// Valamint a szülõk rowid-ját is megõrizzük a people tábláan!!
	CString	m_rowidLastDescendant;


	// elsõ foku substringek
	CString	m_descendant;
	std::vector<MARRIAGES>		v_marriages;		// leszármazott házasságai
	std::vector<SPOUSESPOUSES>	v_spouseSpouses;	// házastársak további házastársai	
	
	// a szétszedett sor módosított substringjei
	CString m_weddingNameSubstr; // wedding+name+leírás
	CString m_nameSubstr;		// name+leírás
	CString m_birthSubstr;
	CString m_deathSubstr;
	CString m_name;			//nameSubstr-bõl levett név, ami már nem tartalmazza az esetleges leírást
	CString m_peer;
	CString m_posterior;
	CString m_comment;

	CString m_peerHeader;

	PEOPLE d;		// a leszármazott adatai
	PEOPLE s;		// a leszármazott házastársának adatai						vector lesz belõle (v_spouse)
	PEOPLE sf;		// a leszármazott házastársa apjának adatai					vector lesz belõle
	PEOPLE sm;		// a leszármazott házastársa anyjának adatai				vector lesz belõle
	PEOPLE ss;		// a leszármazott házastársa további házastársának adatai	vector lesz belõle, az s vector indexét megõrizni!



	bool	m_tableAncestry;			// a táblák elsõ emberét, az õst jelzi ha TRUE;

	int m_rollToTable;
	int m_rollToFamily;
	int m_rollToLine;
	int m_num;

	int		m_fileNumber;
	CString	m_familyName;
	CString m_titolo;
	CString m_tableName;

	
	int		m_familyNumber;
	int		m_tableNumber;
	int		m_lineNumber;

	bool CInputGA::isKivetel(CString word);
	int CInputGA::getSuffix(CStringArray* A, int k );
	bool CInputGA::isname(CString line, PEOPLE* p);
	int CInputGA::isTitle(CString word);
	int CInputGA::isTitolo(CStringArray* A, int n, int i, CString *titolo);
	int CInputGA::getLastName(CStringArray* A, int n, int i, CString *last_name );
	int CInputGA::getFirstName(CStringArray* A, int i, CString* firstname, CString* sex);

	int CInputGA::isWeddingDate(CStringArray* A, int i, CString* datum);  ///!!!!!!!!!!!!!!!
	bool	insertEntries();
	bool	inputFile();
	void	splitLine( CString cLine);
	void	splitTableHeader( CString cLine ); 
	int		rollFile( CStdioFile* file );

	void	CInputGA::insertTableHeader();
	void CInputGA::processSpouseSubstrings();
	CString CInputGA::processWedding( int ix);
	CString CInputGA::cutBirthDeath(CString spouseSubstr);
	void	processPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns );
	void	splitSubstrs( PEOPLE* any );
	void	processDescendantSubstring( CString cLine );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:

	CColorStatic colorComment;
	CColorStatic colorHtml;
	BOOL m_sameDBName;
	BOOL REFRESH;

	CString str;
	CString m_command;

	CString m_inputHtmlSpec;
	CString m_inputHtmlName;

	CString _contractions;
	CString m_fieldsP;		// "people" tábla oszlopai
	CString m_fieldsM;		// "marriages" tábla oszlopai
	CString m_fieldsT;		// "tables" tábla oszlopai 
	
	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;

	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );

	void	clearAll();
	void	clearPeople( PEOPLE *p);
	

	MARRIAGE_INSERT d_s;
	MARRIAGE_INSERT s_ss;
	MARRIAGE_INSERT sf_sm;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CString drive;
	CString dir;
	CString fname;
	CString ext;
	
	CString m_weddingDate;
	CString m_weddingPlace;

	bool CInputGA::isNameNew(CString line, CString* before, PEOPLE* p, CString* after );

	int m_genCnt;

	int		m_tableNumber1;
	int m_cnt; // az adatbázisban lévõ emberek száma a beolvasás elõtt;

	int	m_spouseOrder;
	int m_orderMother;


	CString m_cLine;
	TCHAR	m_generationPrev;
	CString m_fileSpec;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int CInputGA::setGenCnt(CString line);

	bool CInputGA::setFatherMother();
	
	bool CInputGA::isPeer(CString title);
	CString getBranch( CString cLine );

	CString getCsalad( CString root );
	void	connectFolyt();
	void CInputGA::peerHeritage();
	void	connectCsalad();


	CString	getDescendant( CString cLine );
	void	getMarriageSubstrings( CString cLine );
	CString CInputGA::cutParentsSposuses(int i);

	void CInputGA::splitToSubstr(CString line);
	void CInputGA::splitNameComment();

	CString	insertAny( PEOPLE* p );
	CString	insertDescendant();
	CString	insertDescendantSpouse( UINT i);
	CString	insertSpouseF( UINT i );
	CString	insertSpouseM( UINT i );
	CString	insertSpouseS( UINT i );
	int		insertMarriage( CString husband_id, CString wife_id, CString whicHusband, CString whichWife, CString place, CString date, int source );
	int		insertDescMarriage( UINT i );
	int		insertSpouseParentsMarriage( UINT i );
	int		insertSpouseSpousesMarriage( UINT i );

	void	processRelatives(int i);

	void	processSpousesSpouses( int i, std::vector<PEOPLE>* v_p);
	int		getSpouseOrder( std::vector<PEOPLE>* vp );
	void CInputGA::setParentIndex();
	CString	checkSex( CString sex_id );
	void CInputGA::checkSexCouple( CString* sex1, CString* sex2);
	CString setPeer( PEOPLE* p );
	CString getPeerFromString(CString str);
	CString CInputGA::cleanHtmlTags(int lineNumber, CString cLine);

	void CInputGA::processSpouse(PEOPLE* s);
	void CInputGA::processPlaceNameSubstr(CString placeName, PEOPLE* s);
	void CInputGA::processNameSubstr(CString nameSubstr, PEOPLE* s);
	CString CInputGA::splitStringAt(CString str, int at, CString* str2);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedStaticHtml();
	afx_msg void OnClickedButtonFile();
	afx_msg void OnClickedButtonFamily();
	afx_msg void OnClickedButtonTable();
	afx_msg void OnClickedInputGahtmlFromline();
	CStatic m_mappaCtrl;
	CStatic m_databaseCtrl;
	CStatic m_htmlCtrl;
};

