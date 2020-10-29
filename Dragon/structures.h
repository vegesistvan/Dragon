#pragma once
#include "build_defs.h"

	const unsigned char build[] =
{
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    '-',
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    '-',
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    ' ',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    ':',
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    '\0'
};
/*
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
*/

typedef struct
{
	CString refI;
	CString link;
	CString rowidP;
	CString fileName;
	int		primaryPic;
}PHOTOS;

/*
typedef struct
{
	CString filename;
	CString fileSpec;
	CString ext;
	u_int64	size;
}BLOBSTAT;
*/
typedef struct FAMS
{
	int	indi;
	int	sex;
	int	fams;
	int order;
	int	husb;
	int wife;
}FAMS;

typedef struct 
{
	TCHAR	generation;
	int		mother_index;
}MOTHERINDEX;


typedef struct
{
	CString m_rowid_table;
	CString name;
	CString birth_date;
}PARENT;




typedef struct
{
	CString	columnname;
	int		justification;
	int		length;
}COLUMNS;

typedef struct
{
	int		index;
	UINT	group;
	TCHAR	same;
	CString name;
	CString	rowid;
	CString	lineNumber;
	CString	tableNumber;
	CString	generation;
	int		source;
	CString	united;
	CString	sex_id;
	CString	repleceBy;
	CString birth_date;
	CString death_date;
	CString identical;
	CString timeConflict;
	CString father;
	CString mother;
	CString lineNumberF;
	CString lineNumberM;
	CString birth_father;
	CString death_father;
	CString birth_mother;
	CString death_mother;
}SAME; 


typedef struct
{
	CString	sex_id;
	CString rowid;
	int		source;
	CString rowidBy;
}REPLACE;



typedef struct
{
	CString rowid;
	CString oder;
	CString name;
	CString name2;
	CString id;
	CString birth_date;
	CString death_date;
}SPOUSES;

typedef struct
{
	CString spouses;
	CString spouses2;
}SPOUSES2;


typedef struct
{
	CString rowid;
	CString mother_index;
	CString name;
	CString name2;
	CString id;
	CString birth_date;
	CString death_date;
}KIDS;





// CIdenticalSpouses
typedef struct
{
	CString rowidM;
	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString sex_id;
	CString generation;
	CString source;
	CString united;
	CString spouse;
	CString birthDate;
	CString deathDate;
	CString father;
	CString mother;
	CString sourceF;
	CString sourceM;
	CString birthDateF;
	CString birthDateM;
	CString deathDateF;
	CString deathDateM;
	CString lineNumberF;
	CString lineNumberM;
	int		diff;
}MORESPOUSES;


// VCouples az összes, vSame az azonos nevû házaspárokat tartalmazza
typedef struct
{
	UINT	cnt;
	UINT	group;				// az azonos nevû házastársak között több összevonható csoport lehet. Ez jelzi, hogy melyik csoportba tartozik
								// 0 - nem tartozik csoportba, egyedi adatai vannak
								// 1-n az 1-n csoportba tartozik
	int		status1;			// férj: 1 = megtartandó, -1 törlendõ 
	int		status2;			// feleség: 1 = megtartandó, -1 törlendõ 
	int		contracted;			// false: nincs a csoportban összevont ember, az egész vSame a hibás oldalon listázandó
								// true: van a VSame-ben összevont ember, az egész vSame az összevontak oldalán listázandó
	CString rowidM;				// házaság azonosítója
	CString dateM;				// házasság dátuma

	CString rowidS1;			// férj adatai
	CString sex_idS1;
	CString generationS1;
	CString sourceS1;
	CString unitedS1;
	CString lineS1;
	CString spouse1;
	CString birthS1;
	CString deathS1;
	CString mother_idS1;
	CString motherS1;
	CString father_idS1;
	CString fatherS1;
	
	CString rowidS2;			// feleség adatai
	CString sex_idS2;
	CString generationS2;
	CString sourceS2;
	CString unitedS2;
	CString lineS2;
	CString spouse2;
	CString birthS2;
	CString deathS2;
	CString mother_idS2;
	CString motherS2;
	CString father_idS2;
	CString fatherS2;

}COUPLES;

typedef struct
{
	UINT	cnt;
	UINT	group;				// 0 - nem tartozik csoportba, egyedi adatai vannak
								// 1-n az 1-n csoportba tartozik
	UINT	group2;				// más csoporttól vette el
	int		status;				// 1 = megtartandó, -1 törlendõ 
	UINT	match;
	CString rowid;
	CString line;
	CString generation;
	CString source;
	CString united;
	CString	sex_id;
	CString name;
	CString birth;
	CString death;
	CString father;				// father
	CString rowidF;
	CString birthF;
	CString deathF;
	CString mother;				// mother
	CString rowidM;			
	CString birthM;
	CString deathM;
	CString spouses;
	CString rowidS;				// spouse 
	CString lineF;
}SAMENAMES;

typedef struct
{
	CString rowid;
	CString rowidBy;
	CString sex_id;
}CONTRACT;