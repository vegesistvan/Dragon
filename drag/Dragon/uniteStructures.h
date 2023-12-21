#pragma once

typedef struct
{
	CString rowid1;
	CString rowid2;
	CString sex;
	CString source1;
	CString source2;
}OTHERS;


typedef struct
{
	int nItem;
	CString rowid;
	CString	sex_id;
	int		source;
}UNITE_ENTRIES;

typedef struct
{
	CString lastname;
	CString firstname;
}LASTFIRST;


typedef struct
{
	CString rowid;
	CString name;
}ENTRIES;

typedef struct
{
	CString line;
	CString rowid;
	CString source;
	CString name;
	CString birth;
	CString death;
	CString father_id;
	CString mother_id;
	CString sex_id;
}GETNAME3;

typedef struct
{
	UINT	rgbcolor;		// color kód, 
	UINT	group;			// csoport száma ( 0: nem tartozik csoportba, azaz nincs még egy olyan ember, mint õ
	UINT	status;			// 1 = megtartandó, -1 törlendõ 
	UINT	match;			// 
	UINT	united;
	CString index;
	CString source;

	CString line;
	CString rowid;			// emberünk
	CString	sex_id;
	CString name;
	CString birth;
	CString death;
	CString titolo;
	CString peer;

	CString lineF;
	CString rowidF;
	CString father;				// father
	CString birthF;
	CString deathF;

	CString lineM;
	CString rowidM;
	CString mother;				// mother
	CString birthM;
	CString deathM;

	CString spouses;

	CString lineGF;
	CString rowidGF;
	CString grandfather;				// grandfather
	CString birthGF;
	CString deathGF;

	CString lineGM;
	CString rowidGM;
	CString grandmother;				// grandfather
	CString birthGM;
	CString deathGM;

}USAME;
typedef struct
{
	UINT	rgbcolor;		// color kód, 
	UINT	group;			// csoport száma ( 0: nem tartozik csoportba, azaz nincs még egy olyan ember, mint õ
	UINT	status;			// 1 = megtartandó, -1 törlendõ 
	UINT	match;			// 
	UINT	united;
	CString index;
	CString source;
	int		spouseIndex;	// összevonáshoz megadjuk a házasság sorszámáta DescendanSpouses-hoz	

	CString line;
	CString rowid;			// emberünk
	CString	sex_id;
	CString name;
	CString birth;
	CString death;
	CString titolo;
	CString peer;
	CString birthPlace;
	CString deathPlace;
	CString comment;

	CString lineF;
	CString sourceF;
	CString rowidF;
	CString father;				// father
	CString birthF;
	CString deathF;

	CString lineM;
	CString sourceM;
	CString rowidM;
	CString mother;				// mother
	CString birthM;
	CString deathM;

	CString spouses;

}USAME3;

typedef struct
{
	CString	rowid;			// férj/feleség
	CString wedding;

	CString rowidS;			// a házastárs
	CString nameS;
	CString birthS;
	CString deathS;
	CString sourceS;
	CString sex;

	CString rowidF;			// házastárs apja
	CString father;
	CString birthF;
	CString deathF;
	CString sourceF;

	CString rowidM;			// házastárs anyjy
	CString mother;
	CString birthM;
	CString deathM;
	CString sourceM;

	CString spouses;		// az összse házastárs adataival együtt
	CString lineF;
	CString nameBD;
}USPOUSES3;