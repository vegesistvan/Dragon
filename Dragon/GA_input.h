#pragma once


#include "GA_structures.h"


class CGaInput
{
public:
	CGaInput(void);
	~CGaInput(void);

	
	CString str;
	CString m_command;
	CString m_cLine;
	TCHAR	m_generationPrev;


	int		m_error_cnt1;
	int		m_error_cnt2;
	int		m_error_cnt3;
	int		m_error_cnt4;

	int		m_fileNumber;
	int		m_familyNumber;
	int		m_tableNumber;
	int		m_lineNumber;
	int		m_rowid;	// az utoljára insertált ember azonosítója. Azért számoljuk és nem visszakérdezzük a SELECT 'last_insert_rowid'-val
						// mert ez magszakítaná a BEGIN_COMMIT tranzakciót és nagyon lelassulna a beolvasás!!!
						// Az m_rowid-ra egyébként azért van szükség, hogy a házastársak rowid-párjait gyûjtsük és a marriages-táblában mrgõrizzük.
						// Valamint a szülõk rowid-ját is megõrizzük a people tábláan!!

	CString	m_rowid_table;

	

	CString _contractions;
	CString m_fieldsP;		// "people" tábla oszlopai
	CString m_fieldsM;		// "marriages" tábla oszlopai
	CString m_fieldsT;		// "tables" tábla oszlopai 
	
	FILE* fh3;
	FILE* fh4;
	FILE* fh1;
	FILE* fh2;

	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// F E L B O N T Á S H O Z   H A S Z N Á L T   V Á L T O Z Ó K ,  S T R I U K T U R Á K ,  V E K T O R O K ////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy sor felbontása
	TABLEHEADER m_tableHeader;
	
	CString	m_folyt;			// a sor végérõl leszedett folyt utáni római szám
	
	CString	m_familyName;
	CString m_titolo;
	CString m_tableName;
	CString m_known_as;
	CString	m_generationFirst; // egy tábla õsének generációja ( az elágazások összekapcsolásához kell )
	int		m_tableAncestry;			// a táblák elsõ emberét, az õst jelzi ha TRUE; 
	CString	m_rowidLastDescendant;

	std::vector<TABLEHEADER>	v_tableHeader;

// elsõ foku substringek
	CString	m_descendant;
	CString _descNameSS;
	CString	_descBirthSS;
	CString	_descDeathSS;
	CString _descCommentSS;
 
	std::vector<MARRIAGES>		v_marriages;		// leszármazott házasságai
	std::vector<SPOUSESPOUSES>	v_spouseSpouses;	// házastársak további házastársai

	std::vector<GENERATIONS> v_generations;
	std::vector<ORDERFATHER> v_orderFather;
	

	PEOPLE d;		// a leszármazott adatai
	PEOPLE s;		// a leszármazott házastársának adatai						vector lesz belõle (v_spouse)
	PEOPLE sf;		// a leszármazott házastársa apjának adatai					vector lesz belõle
	PEOPLE sm;		// a leszármazott házastársa anyjának adatai				vector lesz belõle
	PEOPLE ss;		// a leszármazott házastársa további házastársának adatai	vector lesz belõle, az s vector indexét megõrizni!

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
	
	int m_rollToTable;
	int m_rollToFamily;
	int m_rollToLine;
	int m_rollToLineFrom;

	CString m_knownAsTable;

	int	m_spouseOrder;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool	inputMethod1();

	CString getBranch( CString cLine );
	void	splitTableHeader( CString cLine ); 
	CString getCsalad( CString root );


	void	inputFile();
	void	splitLine( CString cLine);
	int		processTableHeader( CString cLine );

	int		rollFile( CStdioFile* file );
	
	void	insertTableHeader();
	void	insertEntries();
	void	connectBranches();
	void	setDummyFather();
	void	connectCsalad();

//	int		getTupigny( TCHAR gen );
//	int		putTupigny( TCHAR gen );

	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );

	void	clearAll();

	CString	splitLineToSubstrings( CString cLine );

	void	splitMarriageSubstrings();
	void	noDate( CString str, SNAMEBLOCK *snb );

	CString	getDescendant( CString cLine );
	void	getMarriageSubstrings( CString cLine );

	int		getMotherIndex( TCHAR generation, int n_mother_index );
	int		getSexId( CString first_name );
	BOOL	isTitle( CString cLine ) ;
	int		isFirstName( CString str );
	BOOL	isName( CString str, NAME* name );
	void	splitSpouseString( CString marriageString, SNAMEBLOCK *snb );
	void	splitSpouseStringNew( CString marriageString, SNAMEBLOCK *snb );
	void	splitSpouseStringNew2( CString marriageString, SNAMEBLOCK *snb );
//	void	splitSpouseString2( CString marriageString, SNAMEBLOCK *snb );

	int		getOrderSpouse( std::vector<PEOPLE>* vp );
//CString	isItRelatives( CString cLine );

	void	splitDescendantSubstring( CString cLine );
	void	splitPeopleString( int who, CString cLine, PEOPLE * p );
	void	splitDescNameString( CString nameSubstring );
	void	splitSpousesSpouses( CString sLine, std::vector<PEOPLE> *v_p);
	void	splitSpFatherName( CString cLine, NAME* name );
	void	splitSpouseNameString( CString nameComment, NAME* name ); 
	void	splitFullname( CStringArray* A, NAME* name );
	void	splitName( CString namestr, NAME* name );

	void	fillOrderFather( );
	void	fillFatherMother( );
	
	CString	insertAny( PEOPLE* p );
	CString	insertDescendant();
	CString	insertDescendantSpouse( UINT i);
	CString	insertSpouseF( UINT i );
	CString	insertSpouseM( UINT i );
	CString	insertSpouseS( UINT i );
	CString	insertSpouseFather( std::vector<MARRIAGES>* vM, UINT i);
	CString	insertSpouseMother( std::vector<MARRIAGES>* vM, UINT i);

	

	int		insertMarriage( CString spouse1_id, CString spouse2_id, int sex_id1, int sex_id2, CString order1, CString order2, CString place, CString date, int source );
	int		insertDescMarriage( UINT i );
	int		insertSpouseParentsMarriage( UINT i );
	int		insertSpouseSpousesMarriage( UINT i );
	void	updatePreviousDescendant( CString cLine );
//	void	updatePreviousTable( CString cLine );

	void	insertIntoFiles( CString fileSpec );	

	BOOL	InputFile();
	BOOL	InputFamily( CString familyName, int familyNumber);
	BOOL	InputTable( int tableNumber );
	BOOL	InputLine( int lineNumber );
	BOOL	InputFileFromLine( int lineNUmber);

};

