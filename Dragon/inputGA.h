#pragma once
#include "inputGA_typedef.h"
#include "ColorStatic.h"
#define TEST false  // beolvas�s sor�n ki�rja a ga.htm sorok sorsz�m�t debugol�s c�lj�b�l


const PEERS peers[] =
{
	{L"br", L"b�r�"},
	{L"gr", L"gr�f"},
	{L"�rgr", L"�rgr�f"},
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
///////////// F E L B O N T � S H O Z   H A S Z N � L T   V � L T O Z � K ,  S T R I U K T U R � K ,  V E K T O R O K ////////////////
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

// sorok sz�tszed�se
	CString	m_generationFirst; // egy t�bla �s�nek gener�ci�ja ( az el�gaz�sok �sszekapcsol�s�hoz kell )
	int		m_rowid;	// az utolj�ra insert�lt ember azonos�t�ja. Az�rt sz�moljuk �s nem visszak�rdezz�k a SELECT 'last_insert_rowid'-val
						// mert ez magszak�tan� a BEGIN_COMMIT tranzakci�t �s nagyon lelassulna a beolvas�s!!!
						// Az m_rowid-ra egy�bk�nt az�rt van sz�ks�g, hogy a h�zast�rsak rowid-p�rjait gy�jts�k �s a marriages-t�bl�ban mrg�rizz�k.
						// Valamint a sz�l�k rowid-j�t is meg�rizz�k a people t�bl�an!!
	CString	m_rowidLastDescendant;


	// els� foku substringek
	CString	m_descendant;
	std::vector<MARRIAGES>		v_marriages;		// lesz�rmazott h�zass�gai
	std::vector<SPOUSESPOUSES>	v_spouseSpouses;	// h�zast�rsak tov�bbi h�zast�rsai	
	
	// a sz�tszedett sor m�dos�tott substringjei
	CString m_weddingNameSubstr; // wedding+name+le�r�s
	CString m_nameSubstr;		// name+le�r�s
	CString m_birthSubstr;
	CString m_deathSubstr;
	CString m_name;			//nameSubstr-b�l levett n�v, ami m�r nem tartalmazza az esetleges le�r�st
	CString m_peer;
	CString m_posterior;
	CString m_comment;

	CString m_peerHeader;

	PEOPLE d;		// a lesz�rmazott adatai
	PEOPLE s;		// a lesz�rmazott h�zast�rs�nak adatai						vector lesz bel�le (v_spouse)
	PEOPLE sf;		// a lesz�rmazott h�zast�rsa apj�nak adatai					vector lesz bel�le
	PEOPLE sm;		// a lesz�rmazott h�zast�rsa anyj�nak adatai				vector lesz bel�le
	PEOPLE ss;		// a lesz�rmazott h�zast�rsa tov�bbi h�zast�rs�nak adatai	vector lesz bel�le, az s vector index�t meg�rizni!



	bool	m_tableAncestry;			// a t�bl�k els� ember�t, az �st jelzi ha TRUE;

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
	CString m_fieldsP;		// "people" t�bla oszlopai
	CString m_fieldsM;		// "marriages" t�bla oszlopai
	CString m_fieldsT;		// "tables" t�bla oszlopai 
	
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
	int m_cnt; // az adatb�zisban l�v� emberek sz�ma a beolvas�s el�tt;

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

