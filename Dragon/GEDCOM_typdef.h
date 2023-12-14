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
	CString refF;				// mely családban született
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
	CString FAMC;				// családindex, amelyben gyerekként elõfordul

	CString	father_id;			// apja azonosítója
	CString mother_id;			// anyja azonosítója
	int		parentIndex;		// apja hanyadik feleségének gyermeke, ami kiírásra kerül a html-ben - ez majd kiirtandó
	int		whichWife;			// apja hanyadik feleségének gyereke
	int		whichHusband;			// anyja hanyadik férjének gyereke
	int		whichChildFather;		// apja hagyadik gyermeke
	int		whichChildMother;		// anyja hanyadik gyermeke
	int		numOfSpouses;		// házastársai száma  -  INDI rekordban fel vannak sorolva, megszámoljuk beolvasáskor 
	int		numOfChildren;		// gyerekeinek száma - FAM rekordokból kell kiszámolni
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
	int		cnt;			// v_rekord sorszáam, csak azért kell, hogy az eredeti sorrendet visszaállíthassuk
	CString refF;			// család azonosítója
	CString refH;			// férj azonosítója
	CString refW;			// feleség azonosítója
	CString place;			// házasságkötés helye
	CString date;			// házasságkötés ideje
	int		marriageH;		// férj házasságának sorszáma
	int		marriageW;		// feleség házasságának sorszáma
	int		marriageHAll;	// férj összes házasságának száma
	int		marriageWAll;	// felaség összes házasságának száma
	int		numOfChildren;	// gyerekek száma
}FAM;

typedef struct
{
	int		cnt;
	CString refF;			// család azonosítója
	CString	refH;			// apa azonosítója
	CString refW;			// anya azonosítója
	CString refC;			// gyerek azonosítója
	int		numRefI;
	int		parentIndex;	// apa hanyadik felesége az anyja
	int		whichChildFather;	//
	int		whichChildMother;
}CHIL;

typedef struct
{
	CString refI;	// INDI rekord xref-je		// gyerek azonosítója
	CString refF;	// INDI rekord INDIFAMC-re	// család azonosítója
}INDIFAMC;

typedef struct
{
	CString	sex;
	CString refI;	// INDI rekord xref-je		// INDI azonosítója
	CString refF;	// INDI rekord FAMS-je		// család azonosítója, amelyben õ háézastárs volt
	int		order;	// FAMS sorszáma az INDI rekordban
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

