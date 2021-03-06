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

typedef struct
{
	CString refI;
	CString link;
	CString rowidP;
	CString fileName;
	int		primaryPic;
}PHOTOS;


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
	int		parentIndex;
	int		orderMother;
}PARENT2INDEX;


typedef struct
{
	CString rowid_table;
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
	CString wedding;
	CString birth;
	CString death;
}SPOUSES;

typedef struct
{
	CString spouses;
	CString spouses2;
}SPOUSES2;


typedef struct
{
	CString rowid;
	CString parentIndex;
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
	CString wedding;
	CString father;
	CString mother;
	CString sourceF;
	CString sourceM;
	CString father_id;
	CString mother_id;
	CString birthDateF;
	CString birthDateM;
	CString deathDateF;
	CString deathDateM;
	CString lineNumberF;
	CString lineNumberM;
	int		diff;
}MORESPOUSES;


// VCouples az �sszes, vSame az azonos nev� h�zasp�rokat tartalmazza
typedef struct
{
	UINT	cnt;
	UINT	group;
	UINT	subGroup;			// az azonos nev� h�zast�rsak k�z�tt t�bb �sszevonhat� csoport lehet. Ez jelzi, hogy melyik csoportba tartozik
								// 0 - nem tartozik csoportba, egyedi adatai vannak
								// 1-n az 1-n csoportba tartozik
	int		status1;			// f�rj: 1 = megtartand�, -1 t�rlend� 
	int		status2;			// feles�g: 1 = megtartand�, -1 t�rlend� 
	int		contracted;			// false: nincs a csoportban �sszevont ember, az eg�sz vSame a hib�s oldalon list�zand�
								// true: van a VSame-ben �sszevont ember, az eg�sz vSame az �sszevontak oldal�n list�zand�
	CString rowidM;				// h�zas�g azonos�t�ja
	CString dateM;				// h�zass�g d�tuma

	CString rowidS1;			// f�rj adatai
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
	
	CString rowidS2;			// feles�g adatai
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
	CString rowid;
	CString rowidBy;
	CString sex_id;
	int		sourceBy;
	int		sourceDel;
}CONTRACT;