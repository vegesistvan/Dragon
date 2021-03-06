#pragma once

typedef struct
{
	TCHAR	*name;
	TCHAR	*type;
}COLUMN;

typedef struct
{
	TCHAR	*name;
	const COLUMN	*columns;
	int		size;
}DB;

typedef struct
{
	TCHAR	*table;
	TCHAR	*column;
	TCHAR	*name;
}INDEXES;

typedef struct
{
	CString modifier;
}DATEMODIFIERS;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// sqlite_master table //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	M_TYPE = 0,						// 'table' or 'index'
	M_NAME,
	M_TBL_NAME,
	M_ROOTPAGE,
	M_SQL							// CREATE .....
};
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
	FS_ROWID = 0,
	FSC_XID,
	FS_TYPE,
	FS_FILESPEC,
	FS_CREATED,
	FS_MODIFIED,
	FS_LOADED,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN marriages[] =
{
	{ L"xid",			L"INTEGER PRIMARY KEY ASC" },
	{ L"fileNumber",	L"INT"  },
	{ L"familyNumber",  L"INT"  },
	{ L"tableNumber",	L"INT"  },
	{ L"lineNumber",	L"INT"  },
	{ L"spouse1_id",	L"INT"	},		// a kapcsolat egyik tagja
	{ L"spouse2_id",	L"INT"	},		// a kapcsolat m�sik tagja
	{ L"type_id",		L"INT"  },		// a kapcsolat t�pusa 0: h�zass�g 
	{ L"orderHusband",	L"INT"  },		// 1. f�rj 2. f�rj, stb
	{ L"orderWife",		L"INT"  },		// 1. feles�g, 2. feles�g, stb
	{ L"place",			L"TEXT" },
	{ L"date",			L"TEXT" },
	{ L"end",			L"TEXT" },
	{ L"source",        L"INT"  }		// 1 lesz�rmazott h�zass�ga; 2 = h�zast�rs tov�bbi h�zass�ga; 3 = h�zast�rs sz�leinek h�zass�ga
};

enum
{
	MARRIAGES_ROWID = 0,
	MARRIAGES_XID,
	MARRIAGES_FILENUMBER,
	MARRIAGES_FAMILYNUMBER,
	MARRIAGES_TABLENUMBER,
	MARRIAGES_LINENUMBER,
	MARRIAGES_SPOUSE1_ID,
	MARRIAGES_SPOUSE2_ID,
	MARRIAGES_TYPE_ID,
	MARRIAGES_ORDERHUSBAND,
	MARRIAGES_ORDERWIFE,
	MARRIAGES_PLACE,
	MARRIAGES_DATE,
	MARRIAGES_END,
	MARRIAGES_SOURCE
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const COLUMN people[] =
{
	{ L"xid",						L"INTEGER PRIMARY KEY ASC" },
	{ L"fileNumber",				L"INT"  },
	{ L"familyNumber",				L"INT"  },
	{ L"tableNumber",				L"INT"  },
	{ L"lineNumber",				L"INT"  },
	{ L"source",					L"INT"  },  //hol volt eredetileg ez az ember a csal�df�ban? 1: lesz�rmazott, 2: h�zast�rs 3: h�zast�rs apja, 4: h�zast�rs anyja 5: h�zast�rs tov�bbi h�zast�rsa
	{ L"united",					L"INT"	},  // h�ny embert egyes�t ez a rekord?
	{ L"spouse",					L"INT"  },	// h�ny eredetileg h�zast�rs van ebben a rekordban egyes�tve?
	{ L"spouseparent",				L"INT"  },	// h�ny eredetileg h�zast�rsa sz�l�je van ebben a rekordban egyes�tve?
	{ L"spousespouse",				L"INT"  },	// h�ny eredetileh h�zast�rs h�zast�rsa van ebben a rekordban egys�tve?
	{ L"generation",				L"TEXT" },
	{ L"sex_id",					L"INT"	},
	{ L"title",						L"TEXT"	},			// titulus
	{ L"titolo",					L"TEXT" },			// el�n�v
	{ L"first_name",				L"TEXT"	},
	{ L"last_name",					L"TEXT"	},
	{ L"posterior",					L"TEXT" },
	{ L"other_names",				L"TEXT" },  // csal�dn�v_( csal�dn�v2, csal�dn�v 3..) -b�l a z�r�jelben l�v� nevek
	{ L"known_as",					L"TEXT"	},
	{ L"birth_place",				L"TEXT"	},
	{ L"birth_date",				L"TEXT"	},
	{ L"death_place",				L"TEXT"	},
	{ L"death_date",				L"TEXT"	},
	{ L"comment",					L"TEXT" },
	{ L"father_id",					L"INT"	},
	{ L"mother_id",					L"INT"	},
	{ L"parentIndex",				L"TEXT"	},		// ami a html-ben van ( a m�sodik sz�l� indexe
	{ L"parentIndexCalc",			L"TEXT"	},		// mindig van benne �rv�nyes index, amit a getMotherIndex sz�m�t ki
	{ L"orderFather",				L"INT"	},		// apja hanyadik gyereke A html list�n�l meghat�rozza a list�z�si sorrendet!!
	{ L"orderMother",				L"INT"	},		// anyja hanyadik gyereke
	{ L"folyt",						L"TEXT"	},		// %%% folyt VI
	{ L"tableAncestry",				L"INT"  },		// ga.html eset�n egy t�bla els� embere, �se tak�n felesleges lehet
	{ L"tableRoman",				L"TEXT" },
	{ L"arm",						L"TEXT" },		// 	
	{ L"sources",					L"TEXT"	},		// adat forr�sok le�r�sa
	{ L"occupation",				L"TEXT" },		// elfoglalts�g, munkak�r
	{ L"csalad",					L"TEXT" },		// csal�dalap�t� [ xy csal�d �se] ?
	{ L"gap",						L"INT" },
	{ L"numOfMarriages",			L"INT" },		// h�zass�gok sz�ma
};

enum
{
	PEOPLE_ROWID = 0,
	PEOPLE_XID,
	PEOPLE_FILENUMBER,
	PEOPLE_FAMILYNUMBER,
	PEOPLE_TABLENUMBER,
	PEOPLE_LINENUMBER,
	PEOPLE_SOURCE,
	PEOPLE_UNITED,
	PEOPLE_SPOUSE,
	PEOPLE_SPOUSEPARENT,
	PEOPLE_SPOUSESPOUSE,
	PEOPLE_GENERATION,
	PEOPLE_SEX_ID,
	PEOPLE_TITLE,
	PEOPLE_TITOLO,
	PEOPLE_FIRST_NAME,
	PEOPLE_LAST_NAME,
	PEOPLE_POSTERIOR,
	PEOPLE_OTHER_NAMES,
	PEOPLE_KNOWN_AS,
	PEOPLE_BIRTH_PLACE,
	PEOPLE_BIRTH_DATE,
	PEOPLE_DEATH_PLACE,
	PEOPLE_DEATH_DATE,
	PEOPLE_COMMENT,
	PEOPLE_FATHER_ID,
	PEOPLE_MOTHER_ID,
	PEOPLE_MOTHER_INDEX,
	PEOPLE_MOTHER_INDEX2,
	PEOPLE_ORDERFATHER,
	PEOPLE_ORDERMOTHER,
	PEOPLE_FOLYT,
	PEOPLE_TABLEANCESTRY,
	PEOPLE_TABLEROMAN,
	PEOPLE_ARM,
	PEOPLE_SOURCES,
	PEOPLE_OCCUPATION,
	PEOPLE_CSALAD,
	PEOPLE_GAP,
	PEOPLE_NUMOFMARRIAGES,
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
	{ L"known_as",			L"TEXT" },
	{ L"arm",				L"TEXT" },
	{ L"joint",				L"INT"	},	 
	{ L"folyt",				L"TEXT" },
	{ L"csalad",			L"TEXT" },
};
enum
{
	TABLES_ROWID = 0,
	TABLES_XID,
	TABLES_FILENUMBER,
	TABLES_FAMILYNUMBER,
	TABLES_LINENUMBER, 
	TABLES_TABLEHEADER,
	TABLES_PERCENT,
	TABLES_FAMILY_NAME,
	TABLES_ALIAS,
	TABLES_TITOLO,
	TABLES_TABLENUMBERROMAN,
	TABLES_COMMENT,
	TABLES_TORZS,			// [t�rzs: xy]
	TABLES_KNOWN_AS,
	TABLES_ARM,
	TABLES_JOINT,	
	TABLES_FOLYT,
	TABLES_CSALAD,			// [xy csalad]
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
	UNITED_MARRIAGES
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
	{ L"sex_id",		L"NUM" },
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
	LANG_LANGUAGE
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
	PIC_PICTURE
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ABC sorrendben!!!
const DB databaseTables[] =
{
	{ L"filespec", (COLUMN*)&filespec, sizeof( filespec )/sizeof(COLUMN) },
	{ L"marriages", (COLUMN*)&marriages, sizeof( marriages )/sizeof(COLUMN) },
	{ L"people", (COLUMN*)&people, sizeof( people )/sizeof(COLUMN) },
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
	{ L"marriages", L"spouse1_id", L"spouse1Ix" },
	{ L"marriages", L"spouse2_id", L"spouse2Ix" },
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
