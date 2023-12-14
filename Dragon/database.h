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
	{ L"type",			L"NUM" },							// f�jl t�pus a FILETYPES szerint
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
	{ L"husband_id",	L"TXT"	},		// a f�rj azonos�t�ja
	{ L"wife_id",		L"TXT"	},		// a feles�g azonos�t�ja
	{ L"type_id",		L"INT"  },		// a kapcsolat t�pusa 0: h�zass�g 
	{ L"whichHusband",	L"INT"  },		// a feles�g hanyadik f�rje
	{ L"whichWife",		L"INT"  },		// a f�rj hanyadik feles�ge
	{ L"place",			L"TEXT" },		// h�zass�gk�t�s helye
	{ L"date",			L"TEXT" },		// h�zass�gk�t�s ideje	
	{ L"end",			L"TEXT" },		// h�zass�g v�ge
	{ L"source",        L"INT"  }		// 1 lesz�rmazott h�zass�ga; 2 = h�zast�rs tov�bbi h�zass�ga; 3 = h�zast�rs sz�leinek h�zass�ga
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
// a 'people' t�bla parentIndex-�t motherIndex-re kellen majd �t�rni
const COLUMN people[] =
{
	{ L"xid",						L"INTEGER PRIMARY KEY ASC" },
	{ L"fileNumber",				L"INT"  },		// file number (obsolate)
	{ L"familyNumber",				L"INT"  },		// %%%% csal�d-t�bla rowid-je 	
	{ L"tableNumber",				L"INT"  },		// %% t�bla rowid-je
	{ L"lineNumber",				L"INT"  },		// sor sorsz�ma a ga.html f�jlban
	{ L"source",					L"INT"  },		// hol volt eredetileg ez az ember a csal�df�ban? 1: lesz�rmazott, 2: h�zast�rs 3: h�zast�rs apja, 4: h�zast�rs anyja 5: h�zast�rs tov�bbi h�zast�rsa
	{ L"united",					L"INT"	},		// h�ny embert egyes�t ez a rekord?
	{ L"spouse",					L"INT"  },   
	{ L"spouseparent",				L"INT"	},
	{ L"spousespouse",				L"INT"	},
	{ L"generation",				L"TEXT" },		// gener�ci�s jel a ga-html f�jlban.
	{ L"sex_id",					L"TEXT"	},		// az ember neme: 1 - f�rfi, 2 - n�
	{ L"peer",						L"TEXT" },		// f�nemesi c�m
	{ L"title",						L"TEXT"	},		// titulus
	{ L"titolo",					L"TEXT" },		// el�n�v
	{ L"first_name",				L"TEXT"	},		// kerseztn�v
	{ L"last_name",					L"TEXT"	},		// csal�dn�v
	{ L"posterior",					L"TEXT" },		// n�v ut�ni valami
	{ L"other_names",				L"TEXT" },		// csal�dn�v_( csal�dn�v2, csal�dn�v 3..) -b�l a z�r�jelben l�v� nevek
	{ L"birth_place",				L"TEXT"	},		// sz�let�s helye
	{ L"birth_date",				L"TEXT"	},		// sz�let� d�tuma
	{ L"death_place",				L"TEXT"	},		// hal�l helye
	{ L"death_date",				L"TEXT"	},		// hal�l d�tuma
	{ L"comment",					L"TEXT" },		// megjegyz�s
	{ L"tableAncestry",				L"INT"  },		// ga.html eset�n egy t�bla els� embere, �se tal�n felesleges lehet
	{ L"tableRoman",				L"TEXT" },		// a t�bla r�mai sz�ma, amelyben el�fordult. Csak a t�bla els�-gener�ci�s lesz�rmazottain�l van megadva!!
	{ L"arm",						L"TEXT" },		// a cxsal�d valamelyik �g�t jelzi	
	{ L"sources",					L"TEXT"	},		// adat forr�sok le�r�sa
	{ L"occupation",				L"TEXT" },		// elfoglalts�g, munkak�r
	{ L"csalad",					L"TEXT" },		// csal�dalap�t� [ xy csal�d �se] 
	{ L"gap",						L"INT"  },		// 1-valamely csal�d �se,   egy�bk�nt 0;
	{ L"folyt",						L"TEXT"	},		// %%% folyt VI
	{ L"father_id",					L"TEXT"	},		// apa azonos�t�ja
	{ L"mother_id",					L"TEXT"	},		// anya azonos�t�ja
	{ L"parentIndex",				L"TEXT"	},		// a m�sodik sz�l� indexe
	{ L"whichHusband",				L"INT"	},		// anyja hanyadik f�rj�nek gyermeke. A marraiges t�bl�b�l ker�l bele az �rt�k.
	{ L"whichWife",					L"INT"	},		// apja hanyadik feles�g�nek gyermeke. A marraiges t�bl�b�l ker�l bele az �rt�k.
	{ L"whichChildFather",			L"INT"	},		// apja hanyadik gyereke A html list�n�l meghat�rozza a list�z�si sorrendet!!
	{ L"whichChildMother",			L"INT"	},		// anyja hanyadik gyereke
	{ L"numOfSpouses",				L"INT"  },		// h�zass�gok sz�ma
	{ L"numOfChildren",				L"INT"  },		// gyerekek sz�ma
	{ L"spouseIndex",				L"INT"	},		// h�lyes�g, meg kell sz�ntetni!!! lesz�rmazott h�zast�rs�nak sorsz�m�val egyenl�, �sszevon�sn�l a megtartott bejegyz�s �r�kli, lesz�rmazotti list�n�l eszerint kell lek�rdezni a h�zast�rsakat
	{ L"indi",						L"TEXT" },		// GEDCOM INDI azonos�t�

};
//																																	honnan van az �rt�e?
// oszlop n�v			jelent�se									felhaszn�l�sa													anya alatti gyerekn�l	apa alatti gyerekn�l
// whichChildMother		anyja hanyadik gyereke						ez lesz a sorsz�ma az anya alatt felsorolt gyerekeknek			gyerek sorsz�ma			????
// whichChildFather		apja hanyadik gyereke						ez lesz a sorsz�ma az apa alatt felsorolt gyerekeknek			???						gyerek sorsz�ma
// whichHusband	apja	hanyadik f�rje az anyj�nak					ez lesz a parent index az anya alatt felsorolt gyerekeknek		gyerek parent indexe	????	
// whichWife			anyja hanyadik feles�ge az apj�nak			ez lesz a parent index az apa alatt felsorolt gyerekeknek		????					gyerek parent indexe

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
	DBT_TORZS,			// [t�rzs: xy]
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
	{ L"el�tt"},
	{ L"ut�n" },
	{ L"?" }
};
const UINT dateModifiersSize = sizeof( dateModifiers ) / sizeof( DATEMODIFIERS );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
