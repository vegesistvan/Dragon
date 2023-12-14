#pragma once
#include "OpenDatabase.h"
typedef struct
{
	CString modifier;
}DATEMODIFIERS;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// M A I N   D A T A B A S E //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN filespec[] =
{
	{ L"xid",			L"INTEGER PRIMARY KEY ASC" },
	{ L"type",			L"NUM" },							// fájl típus a FILETYPES szerint
	{ L"filespec",		L"TEXT"	},
	{ L"created",		L"TEXT" },
	{ L"modified",		L"TEXT" },
	{ L"loaded",		L"TEXT" },
};
enum
{
	DBF_ROWID = 0,
	DBF_XID,
	DBF_TYPE,
	DBF_FILENAME,
	DBF_CREATED,
	DBF_MODIFIED,
	DBF_LOADED,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN marriages[] =
{
	{ L"xid",			L"INTEGER PRIMARY KEY ASC" },
	{ L"tableNumber",	L"INT"  },
	{ L"lineNumber",	L"INT"  },
	{ L"husband_id",	L"TXT"	},		// a férj azonosítója
	{ L"wife_id",		L"TXT"	},		// a feleség azonosítója
	{ L"type_id",		L"INT"  },		// a kapcsolat típusa 0: házasság 
	{ L"whichHusband",	L"INT"  },		// a feleség hanyadik férje
	{ L"whichWife",		L"INT"  },		// a férj hanyadik felesége
	{ L"place",			L"TEXT" },		// házasságkötés helye
	{ L"date",			L"TEXT" },		// házasságkötés ideje	
	{ L"end",			L"TEXT" },		// házasság vége
	{ L"source",        L"INT"  }		// 1 leszármazott házassága; 2 = házastárs további házassága; 3 = házastárs szüleinek házassága
};

enum
{
	DBM_ROWID = 0,
	DBM_XID,
	DBM_TABLENUMBER,
	DBM_LINENUMBER,
	DBM_HUSBAND_ID,
	DBM_WIFE_ID,
	DBM_TYPE_ID,
	DBM_WHICHHUSBAND,
	DBM_WHICHWIFE,
	DBM_PLACE,
	DBM_DATE,
	DBM_END,
	DBM_SOURCE,
	DBM_COUNT
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a 'people' tábla parentIndex-ét motherIndex-re kellen majd átírni
const COLUMN people[] =
{
	{ L"xid",						L"INTEGER PRIMARY KEY ASC" },
	{ L"fileNumber",				L"INT"  },		// file number (obsolate)
	{ L"familyNumber",				L"INT"  },		// %%%% család-tábla rowid-je 	
	{ L"tableNumber",				L"INT"  },		// %% tábla rowid-je
	{ L"lineNumber",				L"INT"  },		// sor sorszáma a ga.html fájlban
	{ L"source",					L"INT"  },		// hol volt eredetileg ez az ember a családfában? 1: leszármazott, 2: házastárs 3: házastárs apja, 4: házastárs anyja 5: házastárs további házastársa
	{ L"united",					L"INT"	},		// hány embert egyesít ez a rekord?
	{ L"spouse",					L"INT"  },   
	{ L"spouseparent",				L"INT"	},
	{ L"spousespouse",				L"INT"	},
	{ L"generation",				L"TEXT" },		// generációs jel a ga-html fájlban.
	{ L"sex_id",					L"TEXT"	},		// az ember neme: 1 - férfi, 2 - nõ
	{ L"peer",						L"TEXT" },		// fõnemesi cím
	{ L"title",						L"TEXT"	},		// titulus
	{ L"titolo",					L"TEXT" },		// elõnév
	{ L"first_name",				L"TEXT"	},		// kerseztnév
	{ L"last_name",					L"TEXT"	},		// családnév
	{ L"posterior",					L"TEXT" },		// név utáni valami
	{ L"other_names",				L"TEXT" },		// családnév_( családnév2, családnév 3..) -ból a zárójelben lévõ nevek
	{ L"birth_place",				L"TEXT"	},		// születés helye
	{ L"birth_date",				L"TEXT"	},		// születé dátuma
	{ L"death_place",				L"TEXT"	},		// halál helye
	{ L"death_date",				L"TEXT"	},		// halál dátuma
	{ L"comment",					L"TEXT" },		// megjegyzés
	{ L"tableAncestry",				L"INT"  },		// ga.html esetén egy tábla elsõ embere, õse talán felesleges lehet
	{ L"tableRoman",				L"TEXT" },		// a tábla római száma, amelyben elõfordult. Csak a tábla elsõ-generációs leszármazottainál van megadva!!
	{ L"arm",						L"TEXT" },		// a cxsalád valamelyik ágát jelzi	
	{ L"sources",					L"TEXT"	},		// adat források leírása
	{ L"occupation",				L"TEXT" },		// elfoglaltság, munkakör
	{ L"csalad",					L"TEXT" },		// családalapító [ xy család õse] 
	{ L"gap",						L"INT"  },		// 1-valamely család õse,   egyébként 0;
	{ L"folyt",						L"TEXT"	},		// %%% folyt VI
	{ L"father_id",					L"TEXT"	},		// apa azonosítója
	{ L"mother_id",					L"TEXT"	},		// anya azonosítója
	{ L"parentIndex",				L"TEXT"	},		// a második szülõ indexe
	{ L"whichHusband",				L"INT"	},		// anyja hanyadik férjének gyermeke. A marraiges táblából kerül bele az érték.
	{ L"whichWife",					L"INT"	},		// apja hanyadik feleségének gyermeke. A marraiges táblából kerül bele az érték.
	{ L"whichChildFather",			L"INT"	},		// apja hanyadik gyereke A html listánál meghatározza a listázási sorrendet!!
	{ L"whichChildMother",			L"INT"	},		// anyja hanyadik gyereke
	{ L"numOfSpouses",				L"INT"  },		// házasságok száma
	{ L"numOfChildren",				L"INT"  },		// gyerekek száma
	{ L"spouseIndex",				L"INT"	},		// hülyeség, meg kell szüntetni!!! leszármazott házastársának sorszámával egyenlõ, összevonásnál a megtartott bejegyzés örökli, leszármazotti listánál eszerint kell lekérdezni a házastársakat
	{ L"indi",						L"TEXT" },		// GEDCOM INDI azonosító

};
//																																	honnan van az értée?
// oszlop név			jelentése									felhasználása													anya alatti gyereknél	apa alatti gyereknél
// whichChildMother		anyja hanyadik gyereke						ez lesz a sorszáma az anya alatt felsorolt gyerekeknek			gyerek sorszáma			????
// whichChildFather		apja hanyadik gyereke						ez lesz a sorszáma az apa alatt felsorolt gyerekeknek			???						gyerek sorszáma
// whichHusband	apja	hanyadik férje az anyjának					ez lesz a parent index az anya alatt felsorolt gyerekeknek		gyerek parent indexe	????	
// whichWife			anyja hanyadik felesége az apjának			ez lesz a parent index az apa alatt felsorolt gyerekeknek		????					gyerek parent indexe

enum
{
	DBP_ROWID = 0,
	DBP_XID,
	DBP_FILENUMBER,
	DBP_FAMILYNUMBER,
	DBP_TABLENUMBER,
	DBP_LINENUMBER,
	DBP_SOURCE,
	DBP_UNITED,
	DBP_SPOUSE,
	DBP_SPOUSEPARENT,
	DBP_SPOUSESPOUSE,
	DBP_GENERATION,
	DBP_SEX_ID,
	DBP_PEER,
	DBP_TITLE,
	DBP_TITOLO,
	DBP_FIRST_NAME,
	DBP_LAST_NAME,
	DBP_POSTERIOR,
	DBP_OTHER_NAMES,
	DBP_BIRTH_PLACE,
	DBP_BIRTH_DATE,
	DBP_DEATH_PLACE,
	DBP_DEATH_DATE,
	DBP_COMMENT,
	DBP_TABLEANCESTRY,
	DBP_TABLEROMAN,
	DBP_ARM,
	DBP_SOURCES,
	DBP_OCCUPATION,
	DBP_CSALAD,
	DBP_GAP,
	DBP_FOLYT,

	DBP_FATHER_ID,
	DBP_MOTHER_ID,
	DBP_PARENTINDEX,
	DBP_WHICHHUSBAND,
	DBP_WHICHWIFE,
	DBP_ORDERFATHER,
	DBP_ORDERMOTHER,
	DBP_NUMOFSPOUSES,
	DBP_NUMOFCHILDREN,

	DBP_INDI,
	DBP_COUNT

};	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN properties[] =
{
	{ L"xid",				L"INTEGER PRIMARY KEY ASC" },
	{ L"inputVersion",		L"TEXT" },
	{ L"uniteVersion",		L"TEXT" },
	{ L"inputMode",			L"TEXT" },
	{ L"snameEnough",       L"INT"  }
};
enum
{
	PROP_ROWID = 0,
	PROP_XID,
	PROP_INPUTVERSION,
	PROP_UNITEVERSION,
	PROP_INPUTMODE,
	PROP_SNAMEENOUGH,
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN tables[] =
{
	{ L"xid",				L"INTEGER PRIMARY KEY ASC" },
	{ L"fileNumber",		L"INT" },
	{ L"familyNumber",		L"INT" },
	{ L"lineNumber",		L"NUM" },
	{ L"tableHeader",		L"TEXT"	},
	{ L"percent",			L"TEXT"},
	{ L"familyName",		L"TEXT" },
	{ L"alias",				L"TEXT" },
	{ L"titolo",			L"TEXT" },
	{ L"tableRoman",		L"TEXT" },
	{ L"comment",			L"TEXT" },
	{ L"torzs",				L"TEXT" },
	{ L"arm",				L"TEXT" },
	{ L"joint",				L"INT"	},
	{ L"folyt",				L"TEXT" },
	{ L"csalad",			L"TEXT" },
	{ L"peer",				L"TEXT" },
	{ L"ancestor",			L"TEXT" },
};
enum
{
	DBT_ROWID = 0,
	DBT_XID,
	DBT_FILENUMBER,
	DBT_FAMILYNUMBER,
	DBT_LINENUMBER,
	DBT_TABLEHEADER,
	DBT_PERCENT,
	DBT_FAMILY_NAME,
	DBT_ALIAS,
	DBT_TITOLO,
	DBT_TABLENUMBERROMAN,
	DBT_COMMENT,
	DBT_TORZS,			// [törzs: xy]
	DBT_ARM,
	DBT_JOINT,	
	DBT_FOLYT,
	DBT_CSALAD,			// [xy csalad]
	DBT_PEER,
	DBT_ANCESTOR,
	DBT_COUNT
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN united[] =
{
	{ L"xid",		L"INTEGER PRIMARY KEY ASC" },
	{ L"date",		L"TEXT" }, 
	{ L"mode",		L"TEXT" },
	{ L"input",		L"NUM"},
	{ L"united",	L"NUM" },
	{ L"deleted",	L"NUM" },
	{ L"marriages",	L"NUM" },
};
enum
{
	UNITED_ROWID = 0,
	UNITED_XID,
	UNITED_DATE,
	UNITED_MODE,
	UNITED_INPUT,
	UNITED_UNITED,
	UNITED_DELETED,
	UNITED_MARRIAGES,
	UNITED_COUNT

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// S Y S T E M //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN firstnames[] =
{
	{ L"xid",			L"INTEGER PRIMARY KEY ASC" },
	{ L"first_name",	L"TEXT" },
	{ L"comment",		L"TEXT" },
	{ L"sex_id",		L"TEXT" },
	{ L"occurance",		L"NUM" },
	{ L"origin",		L"TEXT" },
	{ L"meaning",		L"TEXT" },
	{ L"language_id",	L"NUM" },
};
enum
{
	SEX_ROWID = 0,
	SEX_XID,
	SEX_FIRST_NAME,
	SEX_COMMENT,
	SEX_SEX_ID,
	SEX_OCCURANCE,
	SEX_ORIGIN,
	SEX_MEANING,
	SEX_LANGUAGE_ID,
	SEX_COUNT
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN languages[] =
{
	{ L"xid",		L"INTEGER PRIMARY KEY ASC" },
	{ L"language",	L"TEXT" }, 
};
enum
{
	LANG_ROWID = 0,
	LANG_XID,
	LANG_LANGUAGE,
	LANG_COUNT
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// B L O B /////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const COLUMN pictures[] =
{
	{ L"xid",		L"INTEGER PRIMARY KEY ASC" },
	{ L"id",		L"TEXT"	}, 
	{ L"table_id",	L"INT"	},
	{ L"primaryPic",L"INT"	},
	{ L"ext",		L"TEXT"	},
	{ L"date",		L"TEXT"	},
	{ L"filename",	L"TEXT"	},
	{ L"title",		L"TEXT"	},
	{ L"size",		L"INT"	},
	{ L"comment",	L"TEXT"	},
	{ L"picture",	L"BLOB"	},
	
};
enum
{
	PIC_ROWID = 0,
	PIC_XID,
	PIC_ID,
	PIC_TABLE_ID,
	PIC_PRIMARY,
	PIC_EXT,
	PIC_DATE,
	PIC_FILENAME,
	PIC_TITLE,
	PIC_SIZE,
	PIC_COMMENT,
	PIC_PICTURE,
	PIC_COUNT
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ABC sorrendben!!!
const DB databaseTables[] =
{
	{ L"filespec", (COLUMN*)&filespec, sizeof( filespec )/sizeof(COLUMN) },
	{ L"marriages", (COLUMN*)&marriages, sizeof( marriages )/sizeof(COLUMN) },
	{ L"people", (COLUMN*)&people, sizeof( people )/sizeof(COLUMN) },
	{ L"properties", (COLUMN*)&properties, sizeof(properties) / sizeof(COLUMN) },
	{ L"tables", (COLUMN*)&tables, sizeof( tables )/sizeof(COLUMN) },
	{ L"united", (COLUMN*)&united, sizeof( united )/sizeof(COLUMN) },
};
const int numberOfTables = sizeof( databaseTables )/ sizeof( DB );

const INDEXES databaseIndexes[] =
{
	{ L"people", L"father_id", L"fatherIx" },
	{ L"people", L"mother_id", L"motherIx" },
	{ L"people", L"birth_place", L"birthPlaceX" },
	{ L"people", L"death_place", L"deathPlaceX" },
	{ L"marriages", L"husband_id", L"spouse1Ix" },
	{ L"marriages", L"wife_id", L"spouse2Ix" },
	{ L"marriages", L"place", L"placeIx" },
};
const int numberOfIndexes = sizeof( databaseIndexes )/ sizeof( INDEXES );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ABC sorrendben!!!
const DB systemTables[] =
{
	{ L"firstnames", (COLUMN*)&firstnames, sizeof( firstnames )/sizeof(COLUMN) },
	{ L"languages", (COLUMN*)&languages, sizeof( languages )/sizeof(COLUMN) },
};
const int numberOfSystemTables = sizeof( systemTables )/ sizeof( DB );

const INDEXES systemIndexes[] =
{
	{ L"firstnames", L"first_name", L"firstnamesX" },
};
const int numberOfSystemIndexes = sizeof( systemIndexes )/ sizeof( INDEXES );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ABC sorrendben!!!
const DB blobStructure[] =
{
	{ L"pictures", (COLUMN*)&pictures, sizeof( pictures )/sizeof(COLUMN) },
};
const int blobNumberOfTables = sizeof( blobStructure )/ sizeof( DB );

const INDEXES blobIndexes[] =
{
	{ L"pictures", L"id", L"idX" },
};
const int numberOfBlobIndexes = sizeof( blobIndexes ) / sizeof( INDEXES );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const DATEMODIFIERS dateModifiers[] =
{
	{ L"" },
	{ L"kb" },
	{ L"elõtt"},
	{ L"után" },
	{ L"?" }
};
const UINT dateModifiersSize = sizeof( dateModifiers ) / sizeof( DATEMODIFIERS );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
