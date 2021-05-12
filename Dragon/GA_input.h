#pragma once


#include "GA_input_structures.h"

// beolvasásnál használja
typedef struct 
{
	TCHAR	gen;
	CString	descendant_id;
	CString spouse_id[10];
	int		parentIndex;
	int		descendant_sex_id;
	int		orderFather;
	int		orderMother;
	int		numOfSpouses;
	CString known_as;
	CString familyName;
} GENERATIONS;

typedef struct						// az ebbõl készült vektor indexe a generáció sorszáma ( 0-n)
{									// az alábbi adatok erre a generációra vonatkoznak
	CString rowid;					// az utolsó  
	CString sexid;
	CString numofSpouses;
	CString parentIndex;
	UINT	numofChildren;
	UINT	numofChildrenPrinted;
	UINT	printedInGeneration;
	UINT	orderFather;
	UINT	orderMother;
}GA;






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// beolvasásnál d, s, sm, sf, ss struktúrája
typedef struct
{
	int		lineNumber;
	CString	rowid;
	int		tableAncestry;			// TRUE: egy tábla õse
	CString tableNumber;
	CString tableRoman;	// a tábla fejléc római száma
	int		source;				// az ember típusa 1=leszármazott, 2=házastársa, 3=házastárs apja, 4=házastárs anyja, 5=házastárs további házastársa
	TCHAR	generation;			// generációs karakter leszárazott esetén
	int		sex_id;				// 1=férfi, 2= nõ
	CString title;				
	CString titolo;
	CString first_name;
	CString last_name;
	CString posterior;
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
	int		orderFather;
	int		orderMother;
	CString folyt;						// "folyt" utáni római szám, ha van.
	CString arm;						// [] zárójelben lévõ string a család ága [ arm ] 
	CString csalad;
	int		gap;
} PEOPLE;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	int order;
	CString marriage;
	CString place;
	CString date;
	CString spouse;
	CString parents;
	CString spouse_spouses;
}MARRIAGE;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszármazott házastársának strukturája  v_marriages vektort alkot
typedef struct
{
	int		order;			// = jel elõtti szám ( vagy 0 )
	CString line;
	CString marriageSubstr;	// = jel utáni substr
	CString weddigSubstr;
	CString nameSubstr;
	CString birthSubstr;
	CString deathSubstr;
	CString relativesSubstr;
	CString parentsSubstr ;
	CString spousesSubstr;

	CString brace;			// () zárójel tartalma
	CString parents;
	CString spouses;				// a zárójelben felsorolt további házastársak
	CString arm;			// [ ] zárójel tartalma

	CString place;
	CString date;
	CString fullname;
	CString comment;

	CString spouse_id;

	int		sex_id;					// házastárs blokkja
	CString title;
	CString titolo;
	CString first_name;
	CString last_name;
	CString posterior;

	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString	rowid;
	CString	father_id;
	CString	mother_id;
	int		orderSpouse;

	CString moreSpouses;

	int		sex_idF;				// házastárs apjának blokkja
	CString titleF;
	CString titoloF;
	CString firstNameF;
	CString lastNameF;
	CString posteriorF;
	CString birthPlaceF;
	CString birthDateF;
	CString deathPlaceF;
	CString deathDateF;
	CString commentF;
	CString	rowidF;


	int		sex_idM;				// házastárs anyjának blokkja
	CString titleM;
	CString titoloM;
	CString firstNameM;
	CString lastNameM;
	CString posteriorM;
	CString birthPlaceM;
	CString birthDateM;
	CString deathPlaceM;
	CString deathDateM;
	CString commentM;
	CString	rowidM;

	
	CString spouseFullname;
	
	CString father;					// a zárójelben megadott apa utóneve
	CString mother;					// a zárójelben megadott anya teljes neve
} MARRIAGES;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszármazott házastársának további házastársai  v_spouseSpouses vektort alkot
typedef struct
{
	int		spouseIndex;		// a v_marriages vektor indexe
	CString	rowid;
	int		order;
	int		sex_id;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString posterior;
	CString comment;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
} SPOUSESPOUSES;


class CGaInput
{
public:
	CGaInput(void);
	~CGaInput(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// F E L B O N T Á S H O Z   H A S Z N Á L T   V Á L T O Z Ó K ,  S T R I U K T U R Á K ,  V E K T O R O K ////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<GENERATIONS>	v_generations;
	std::vector<ORDERFATHER>	v_orderFather;
	std::vector<PARENT2INDEX>	vParent2Index;

	TABLEHEADER m_tableHeader;
	std::vector<TABLEHEADER>	v_tableHeader;

	struct 
	{
		CString nameSubstr;
		CString birthSubstr;
		CString deathSubstr;
	}desc;

// sorok szétszedése
	CString m_known_as;
	CString	m_generationFirst; // egy tábla õsének generációja ( az elágazások összekapcsolásához kell )
	int		m_rowid;	// az utoljára insertált ember azonosítója. Azért számoljuk és nem visszakérdezzük a SELECT 'last_insert_rowid'-val
						// mert ez magszakítaná a BEGIN_COMMIT tranzakciót és nagyon lelassulna a beolvasás!!!
						// Az m_rowid-ra egyébként azért van szükség, hogy a házastársak rowid-párjait gyûjtsük és a marriages-táblában mrgõrizzük.
						// Valamint a szülõk rowid-ját is megõrizzük a people tábláan!!
//	CString	m_rowid_table;
	CString	m_rowidLastDescendant;


	// elsõ foku substringek
	CString	m_descendant;
	std::vector<MARRIAGES>		v_marriages;		// leszármazott házasságai
	std::vector<SPOUSESPOUSES>	v_spouseSpouses;	// házastársak további házastársai	


	// a szétszedett sor módosított substringjei
	CString m_nameSubstr;
	CString m_birthSubstr;
	CString m_deathSubstr;

	PEOPLE d;		// a leszármazott adatai
	PEOPLE s;		// a leszármazott házastársának adatai						vector lesz belõle (v_spouse)
	PEOPLE sf;		// a leszármazott házastársa apjának adatai					vector lesz belõle
	PEOPLE sm;		// a leszármazott házastársa anyjának adatai				vector lesz belõle
	PEOPLE ss;		// a leszármazott házastársa további házastársának adatai	vector lesz belõle, az s vector indexét megõrizni!



	int		m_tableAncestry;			// a táblák elsõ emberét, az õst jelzi ha TRUE;

	int m_rollToTable;
	int m_rollToFamily;
	int m_rollToLine;
	int m_rollToLineFrom;

	int		m_fileNumber;
	CString	m_familyName;
	CString m_tableName;
	CString m_titolo;

	int		m_familyNumber;
	int		m_tableNumber;
	int		m_lineNumber;


	

	void	insertEntries();
	bool	inputFile();
	void	splitLine( CString cLine);
	void	splitTableHeader( CString cLine ); 
	int		rollFile( CStdioFile* file );

	int		isFirstName( CString str );
	void	insertTableHeader();
	void	processMarriageSubstrings();
	void	processPlaceDateComment( CString placeDateComment, PLACE_DATE_BLOCK* ns );
	void	processPeopleStr( CString cLine,  PEOPLE* any );
	void	processDescendantSubstring( CString cLine );
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:

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
	void	clearTableHeader();

	

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
	

	CString m_knownAsTable;
	int		m_tableNumber1;

	int	m_spouseOrder;
	int m_orderMother;

	CString str;
	CString m_command;
	CString m_cLine;
	TCHAR	m_generationPrev;
	CString m_fileSpec;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString getBranch( CString cLine );
	CString getCsalad( CString root );
	void	connectBranches();
	void	setDummyFather();
	void	connectCsalad();
	CString	splitLineToSubstrings( CString cLine );
	void	noDate( CString str, SNAMEBLOCK *snb );
	CString	getDescendant( CString cLine );
	void	getMarriageSubstrings( CString cLine );
	int		getMotherIndex( TCHAR generation, int n_mother_index );
	int		getOrderMother( TCHAR generation, int orderMother );
	BOOL	isName( CString str, NAME* name );
	int		getOrderSpouse( std::vector<PEOPLE>* vp );
	void	splitSpouseNameString( CString nameComment, NAME* name ); 
	void	splitSpouseNameString( int ix );
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


	void	processPeopleString( int who,  CString cLine, PEOPLE * p );
//	void	processNameSubstr( CString nameSubstr, CString birthSubstr, CString deathSubstr,  PEOPLE* any );
	void	processNameSubstr( PEOPLE* any );
	CString processWedding( CString cLine, PLACE_DATE_BLOCK* w );

	void	processSpousesSpouses( CString spouses,  std::vector<PEOPLE> *v_p );
	void	processSpFatherName( CString nameStr, NAME* name ) ;
	void	processSpouseNameString( CString nameComment, NAME* name );
	int		getSpouseOrder( std::vector<PEOPLE>* vp );
	int		getParent2Index( TCHAR generation, int n_mother_index );
	void	splitFullnameA( CStringArray* A, NAME* name );
	int		checkSex( int sex_id );

};

