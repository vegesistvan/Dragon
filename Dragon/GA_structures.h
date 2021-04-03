#pragma once

/*
typedef struct
{
	CString name;
	CString birth;
	CString death;
	CString description;
}SUBSTRING;
*/
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
	L"kék'",		L"<font color='blue'>",		L"</font>",
	L"piros'",		L"<font color='red'>",		L"</font>",
};

typedef struct
{
	int		orig;				// eredeti sorszám, a kijelölés sorrendje
	int		generation;			// generáció
	CString id;					// az emberünk rowid-je
	CString rowid;				// az apa rowid-je
	CString spouse_id;			// feleség rowid-ja
	CString name;				// neve
	int		sex_id;				// neme
	_int64	kekule;
	CString father_id;
	CString mother_id;

}FELMENOK;


typedef struct
{
	int		g;
	_int64	k;
	int		w;		// feleség ki volt-e már írva?
}KEKULE;


typedef struct
{
	int		g;
	_int64	k;
	int		w;		// feleség ki volt-e már írva?
	CString name;
	CString sex_id;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;		// a házasság helyének és idejének marriage táblából való lekérdezéséhez szükséges
}ASCENDANTS;


typedef struct
{
	int cnt;
	int	G;
	int K;
	CString name;
	CString sex_id;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;
}ASC; 



// A vDesc vektoban egy ember addig marad benn, amíg minden gyermekét listáztuk

typedef struct
{
	CString rowid;
	CString	parentIndex;
	BOOL	numOfSpouses;			// az õ házastársainak száma
	BOOL	numOfMothers;			// az apának több felesége van
	int		sex_id;
	UINT	gen;					// generáció sorszáma
	UINT	numOfChildren;			// hány gyereke van
	UINT	childrenPrinted;		// hány gyereket listáztunk már
	BOOL	hidden;					// ha van apja az õsnek ( %%% folyt ), akkor az apát hidden-ként elteszi 
	UINT	children;
}DESCENDANTS;


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
} GENERATIONS;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// leszármazott struktúrája
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// leszármazott struktúrája
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	int sex_id;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString posterior;
	CString comment;
	CString fullname;
	int		parentIndex;
}NAME;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy people rekordból kivett adatok
typedef struct
{
	CString rowid;
	CString xid;
	CString fileNumber;
	CString familyNumber;
	CString tableNumber;
	CString lineNumber;
	CString source;
	CString generation;
	CString sex_id;
	CString title;
	CString titolo;
	CString first_name;
	CString last_name;
	CString posterior;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString father_id;;
	CString mother_id;
	CString parentIndex;
	CString parentIndexCalc;
	CString folyt;
	CString tableAncestry;
	CString tableRoman;
}P;

typedef struct 
{
	TCHAR gen;
	int	orderFather;
	int orderMother;
}ORDERFATHER;



typedef struct
{
	CString place;
	CString date;
	CString comment;
}PLACE_DATE_BLOCK;

typedef struct
{
	CString fullname;
	CString comment;
	int		sex_id;
	CString title;
	CString first_name;
	CString posterior;
	int		parentIndex;
}NAMEBLOCK;

typedef struct
{
	CString place;
	CString date;
	CString name;
	CString comment;
}SNAMEBLOCK;

typedef struct
{
	int		familyNumber;
	int		tableNumber;
	int		lineNumber;
	CString tableHeader;
	CString percent;
	CString familyName;
	CString titolo;
	CString alias;
	CString tableRoman;
	CString comment;
	CString torzs;
	CString known_as;
	CString arm;
	int		joint;
	CString folyt;
}TABLEHEADER;


typedef struct
{
	CString	rowid1;
	CString	rowid2;
	int order1;
	int order2;
	int	source;
	CString place;
	CString date;
}MARRIAGE_INSERT;


typedef struct
{
	CString name;
	UINT	rgb;
}SZIN;
