#pragma once
#include <string>
#include "stdafx.h"


typedef struct
{
	int		level;
	CString xref;
	CString tag;
	CString value;
}GEDLINE;


typedef struct
{
	TCHAR* name;
	TCHAR* type;
	void   (*foo)(int level, CString xref, CString tag, CString value);
}GEDTAGS;

typedef struct
{
	CString tag;
	int cnt;
}TAGCNT;

typedef struct
{
	CString refI;
	CString refF;				// mely csal�dban sz�letett
	int		linenumber;
	CString tablenumber;
	CString rowid;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString posterior;
	CString sex;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString occupation;
	CString source;
	CString FAMC;				// csal�dindex, amelyben gyerekk�nt el�fordul

	CString	father_id;			// apja azonos�t�ja
	CString mother_id;			// anyja azonos�t�ja
	int		parentIndex;		// apja hanyadik feles�g�nek gyermeke, ami ki�r�sra ker�l a html-ben - ez majd kiirtand�
	int		whichWife;			// apja hanyadik feles�g�nek gyereke
	int		whichHusband;			// anyja hanyadik f�rj�nek gyereke
	int		whichChildFather;		// apja hagyadik gyermeke
	int		whichChildMother;		// anyja hanyadik gyermeke
	int		numOfSpouses;		// h�zast�rsai sz�ma  -  INDI rekordban fel vannak sorolva, megsz�moljuk beolvas�skor 
	int		numOfChildren;		// gyerekeinek sz�ma - FAM rekordokb�l kell kisz�molni
	CString https;
}INDI;

typedef struct
{
	CString titolo;
	CString name;
}SURN;

typedef struct
{
	int		linenumber;
	int		cnt;			// v_rekord sorsz�am, csak az�rt kell, hogy az eredeti sorrendet vissza�ll�thassuk
	CString refF;			// csal�d azonos�t�ja
	CString refH;			// f�rj azonos�t�ja
	CString refW;			// feles�g azonos�t�ja
	CString place;			// h�zass�gk�t�s helye
	CString date;			// h�zass�gk�t�s ideje
	int		marriageH;		// f�rj h�zass�g�nak sorsz�ma
	int		marriageW;		// feles�g h�zass�g�nak sorsz�ma
	int		marriageHAll;	// f�rj �sszes h�zass�g�nak sz�ma
	int		marriageWAll;	// felas�g �sszes h�zass�g�nak sz�ma
	int		numOfChildren;	// gyerekek sz�ma
}FAM;

typedef struct
{
	int		cnt;
	CString refF;			// csal�d azonos�t�ja
	CString	refH;			// apa azonos�t�ja
	CString refW;			// anya azonos�t�ja
	CString refC;			// gyerek azonos�t�ja
	int		numRefI;
	int		parentIndex;	// apa hanyadik feles�ge az anyja
	int		whichChildFather;	//
	int		whichChildMother;
}CHIL;

typedef struct
{
	CString refI;	// INDI rekord xref-je		// gyerek azonos�t�ja
	CString refF;	// INDI rekord INDIFAMC-re	// csal�d azonos�t�ja
}INDIFAMC;

typedef struct
{
	CString	sex;
	CString refI;	// INDI rekord xref-je		// INDI azonos�t�ja
	CString refF;	// INDI rekord FAMS-je		// csal�d azonos�t�ja, amelyben � h��zast�rs volt
	int		order;	// FAMS sorsz�ma az INDI rekordban
}INDIFAMS;


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
	CString refI;
	CString link;
	CString rowidP;
	CString fileName;
	int		primaryPic;
}PHOTOS;


typedef struct
{
	CString honap;
	int month;
}MONTHES;

typedef struct
{
	CString english;
	CString hungarian;
}MODIFIERS;

