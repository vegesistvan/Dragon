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
	int		familyNumber;	// csal�d sorsz�ma ( 1- N )
	int		tableNumber;	// t�bla sorsz�am ( 1- N)
	int		lineNumber;		// a t�bla sor sorzs�am
	int		tableNumber1;
	CString tableRoman;		// a t�bla r�mai sz�ma a csal�don bel�l
	CString tableHeader;	// a teljes t�bla sor
	CString percent;		// %%% vagy %%
	CString familyName;		// a tableHeaddr-b�l kivett csal�dn�v
	CString titolo;			// a tableHeaderb�l kivett el�n�v
	CString alias;			// a tableHeaderb�l kivett alias
	CString known_as;
	CString comment;		// a tableHeaderb�l kivett comment

	CString torzs;
	CString arm;
	int		joint;
	CString folyt;			// a t�bl�ban el�fordul� folyt sorok hivatkoz�sai vessz�vel elv�lasztva ( csak inform�ci�)
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


