#pragma once


#include "GA_input_structures.h"

// beolvas�sn�l haszn�lja
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

typedef struct						// az ebb�l k�sz�lt vektor indexe a gener�ci� sorsz�ma ( 0-n)
{									// az al�bbi adatok erre a gener�ci�ra vonatkoznak
	CString rowid;					// az utols�  
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
// beolvas�sn�l d, s, sm, sf, ss strukt�r�ja
typedef struct
{
	int		lineNumber;
	CString	rowid;
	int		tableAncestry;			// TRUE: egy t�bla �se
	CString tableNumber;
	CString tableRoman;	// a t�bla fejl�c r�mai sz�ma
	int		source;				// az ember t�pusa 1=lesz�rmazott, 2=h�zast�rsa, 3=h�zast�rs apja, 4=h�zast�rs anyja, 5=h�zast�rs tov�bbi h�zast�rsa
	TCHAR	generation;			// gener�ci�s karakter lesz�razott eset�n
	int		sex_id;				// 1=f�rfi, 2= n�
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
	CString folyt;						// "folyt" ut�ni r�mai sz�m, ha van.
	CString arm;						// [] z�r�jelben l�v� string a csal�d �ga [ arm ] 
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
// Lesz�rmazott h�zast�rs�nak struktur�ja  v_marriages vektort alkot
typedef struct
{
	int		order;			// = jel el�tti sz�m ( vagy 0 )
	CString line;
	CString marriageSubstr;	// = jel ut�ni substr
	CString weddigSubstr;
	CString nameSubstr;
	CString birthSubstr;
	CString deathSubstr;
	CString relativesSubstr;
	CString parentsSubstr ;
	CString spousesSubstr;

	CString brace;			// () z�r�jel tartalma
	CString parents;
	CString spouses;				// a z�r�jelben felsorolt tov�bbi h�zast�rsak
	CString arm;			// [ ] z�r�jel tartalma

	CString place;
	CString date;
	CString fullname;
	CString comment;

	CString spouse_id;

	int		sex_id;					// h�zast�rs blokkja
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

	int		sex_idF;				// h�zast�rs apj�nak blokkja
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


	int		sex_idM;				// h�zast�rs anyj�nak blokkja
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
	
	CString father;					// a z�r�jelben megadott apa ut�neve
	CString mother;					// a z�r�jelben megadott anya teljes neve
} MARRIAGES;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lesz�rmazott h�zast�rs�nak tov�bbi h�zast�rsai  v_spouseSpouses vektort alkot
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
///////////// F E L B O N T � S H O Z   H A S Z N � L T   V � L T O Z � K ,  S T R I U K T U R � K ,  V E K T O R O K ////////////////
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

// sorok sz�tszed�se
	CString m_known_as;
	CString	m_generationFirst; // egy t�bla �s�nek gener�ci�ja ( az el�gaz�sok �sszekapcsol�s�hoz kell )
	int		m_rowid;	// az utolj�ra insert�lt ember azonos�t�ja. Az�rt sz�moljuk �s nem visszak�rdezz�k a SELECT 'last_insert_rowid'-val
						// mert ez magszak�tan� a BEGIN_COMMIT tranzakci�t �s nagyon lelassulna a beolvas�s!!!
						// Az m_rowid-ra egy�bk�nt az�rt van sz�ks�g, hogy a h�zast�rsak rowid-p�rjait gy�jts�k �s a marriages-t�bl�ban mrg�rizz�k.
						// Valamint a sz�l�k rowid-j�t is meg�rizz�k a people t�bl�an!!
//	CString	m_rowid_table;
	CString	m_rowidLastDescendant;


	// els� foku substringek
	CString	m_descendant;
	std::vector<MARRIAGES>		v_marriages;		// lesz�rmazott h�zass�gai
	std::vector<SPOUSESPOUSES>	v_spouseSpouses;	// h�zast�rsak tov�bbi h�zast�rsai	


	// a sz�tszedett sor m�dos�tott substringjei
	CString m_nameSubstr;
	CString m_birthSubstr;
	CString m_deathSubstr;

	PEOPLE d;		// a lesz�rmazott adatai
	PEOPLE s;		// a lesz�rmazott h�zast�rs�nak adatai						vector lesz bel�le (v_spouse)
	PEOPLE sf;		// a lesz�rmazott h�zast�rsa apj�nak adatai					vector lesz bel�le
	PEOPLE sm;		// a lesz�rmazott h�zast�rsa anyj�nak adatai				vector lesz bel�le
	PEOPLE ss;		// a lesz�rmazott h�zast�rsa tov�bbi h�zast�rs�nak adatai	vector lesz bel�le, az s vector index�t meg�rizni!



	int		m_tableAncestry;			// a t�bl�k els� ember�t, az �st jelzi ha TRUE;

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

