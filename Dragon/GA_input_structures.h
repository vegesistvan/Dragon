#pragma once




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



typedef struct 
{
	TCHAR gen;
	int	orderFather;
	int orderMother;
	CString familyname;
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
	int		familyNumber;	// család sorszáma ( 1- N )
	int		tableNumber;	// tábla sorszáam ( 1- N)
	int		lineNumber;		// a tábla sor sorzsáam
	int		tableNumber1;
	CString tableRoman;		// a tábla római száma a családon belül
	CString tableHeader;	// a teljes tábla sor
	CString percent;		// %%% vagy %%
	CString familyName;		// a tableHeaddr-bõl kivett családnév
	CString titolo;			// a tableHeaderbõl kivett elõnév
	CString alias;			// a tableHeaderbõl kivett alias
	CString known_as;
	CString comment;		// a tableHeaderbõl kivett comment

	CString torzs;
	CString arm;
	int		joint;
	CString folyt;			// a táblában elõforduló folyt sorok hivatkozásai vesszõvel elválasztva ( csak információ)
	CString csalad;
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


