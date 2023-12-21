#pragma once
#include <string>
#include "stdafx.h"
#include <afx.h>
	// beolvasásnál használja
#define MAXNUMOFSPOUSES  15



typedef struct
{
	CString value;
}VALUES;
 
const VALUES m_titles[] =
{
	L"dr",
	L"özv",
	L"ifj",
	L"lg",
	L"ns",
	L"jur",
	L"õrgr",
	L"vitéz",
	L"von",
	L"de",
	L"phil",
};
const int numOfTitles = sizeof(m_titles) / sizeof(VALUES);
const VALUES m_kivetelek[] =
{
	L"de_l",
	L"van_",
	L"von_l"
};
const int m_numOfKivetelek = sizeof(m_kivetelek) / sizeof(VALUES);



// Az egy sorban található emberek feldolgozása és insertálása után készíti el az a sor adataiból az alábbi blokkot, 
// amit a v_generations vektorba insertál. A v_generations vektor egy tábla kezdetekor újra kezdõdik. 
//
// Egy sor feldolgozása során felhasználja a korábban insertált blokkok közül az elõzõ testvérének és a szüleinek
// az adatblokkját, amit a gen ( generációs kód )-al azonosít. Ezekbõl meghatározza a parentIndex-et,
// a descendant_id-t,  ami az apa rowid-ja valamint a 
// spouse_id[partent_index-1]-et ami az anya rowid-ja.



// Insertálás elõtt a vektor végérõl eldobja a magasabb generácihoz tartozó bejegyzéseket, ha vannak ilyenek, mert ezekre már nem lesz szükség
// A vektor végérõl indulva megkeresi az elsõ azonos generációt, azaz a testvérét, ha volt ilyen. A testvérétõl örökli a parentIndex-et, ha magának nem volt.
// Aztán a magánál egyel kisebb generációt keresi visszafele, amiben a szülei adatai vannak. Inne veszi át az apja és anyja 
// azonosítóját

typedef struct
{
	CString folyt;
	CString rowidD;
	CString	sex_id;
	CString rowidS[MAXNUMOFSPOUSES];
	CString peer;
}FOLYT;

	typedef struct
	{
		TCHAR	gen;
		CString	descendant_id;				// d.rowid-t teszi bele
		CString spouse_id[MAXNUMOFSPOUSES];	// a v_marriages spouse_id értéket teszi bele
		int		parentIndex;
		CString	descendant_sex_id;
		int		whichChildFather;	// apja hagyadik gyereke
		int		whichChildMother;	// anyja hanyadik gyereke	
		int		numOfSpouses;	// házastársainak száma
		CString familyName;		
		CString title;			
		CString peer;			
	} GENERATIONS;


	typedef struct						// az ebbõl készült vektor indexe a generáció sorszáma ( 0-n)
	{									// az alábbi adatok erre a generációra vonatkoznak
		CString rowid;					// az utolsó  
		CString sexid;
		CString numofSpouses;
		CString parentIndex;
		UINT	numofChildren;
		UINT	numofChildrenPrinted;
		UINT	printedInGeneration;
		UINT	whichChildFather;
		UINT	whichChildMother;
	}GA;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// beolvasásnál d, s, sm, sf, ss struktúrája
	typedef struct
	{
		CString	rowid;
		int fileNumber;
		int familyNumber;
		CString tableNumber;
		int	lineNumber;
		int	source;				// az ember típusa 1=leszármazott, 2=házastársa, 3=házastárs apja, 4=házastárs anyja, 5=házastárs további házastársa
		int united;
		int spouse;
		int spouseparent;
		int spousespouse;
		TCHAR	generation;			// generációs karakter leszárazott esetén
		CString	sex_id;				// 1=férfi, 2= nõ
		CString peer;
		CString title;
		CString titolo;
		CString first_name;
		CString last_name;
		CString posterior;
		CString other_names;
		CString birth_place;
		CString birth_date;
		CString death_place;
		CString death_date;
		CString comment;
		int		tableAncestry;			// TRUE: egy tábla õse
		CString tableRoman;	// a tábla fejléc római száma
		CString arm;						// [] zárójelben lévõ string a család ága [ arm ] 
		CString sources;
		CString occupation;
		CString csalad;
		int		gap;
		CString folyt;						// "folyt" utáni római szám, ha van.
		CString	father_id;
		CString	mother_id;
		int		parentIndex;
		int		spouseIndex;				// a leszármazott házastársánál van megadva, hogy hanyadik házastársa a leszármazottnak. Csak összevonás elõtt van értelme!!
		int		whichHusband;			// az apja hanyadik férje az anyjának
		int		whichWife;			// az anyja hanyadik felesége az apjának  	( == parentIndex )
		int		whichChildFather;			// hanyadik gyerek az apjának
		int		whichChildMother;			// hanyadik gyereke az anyjának
		int		numOfSpouses;
		int		numOfChildren;
	} PEOPLE;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct
	{
		int order;
		CString marriage;
		CString place;
		CString date;
		CString spouse;
		CString parents;
		CString spouse_spouses;
	}MARRIAGE;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Leszármazott házastársának strukturája  v_marriages vektort alkot
	typedef struct
	{
		int		order;			// = jel elõtti szám ( vagy 0 )
		CString line;
		CString marriageSubstr;	// = jel utáni substr
		CString weddigSubstr;
		CString spouseSubstr;
		CString nameSubstr;
		CString birthSubstr;
		CString deathSubstr;
		CString relativesSubstr;
		CString parentsSubstr;
		CString brace;			// () zárójel tartalma
		CString parents;
		CString spouses;				// a zárójelben felsorolt további házastársak
		CString arm;			// [ ] zárójel tartalma

		CString place;
		CString date;
		CString fullname;
		CString comment;

		CString spouse_id;
		CString	sex_id;					// házastárs blokkja
		CString peer;
		CString title;
		CString titolo;
		CString first_name;
		CString last_name;
		CString posterior;

		CString birth_place;
		CString birth_date;
		CString death_place;
		CString death_date;
		CString	rowidHusband;
		CString rowidWife;
		CString	father_id;
		CString	mother_id;
		CString	orderSpouse;

		CString whichWife;
		CString whichHusband;
		CString moreSpouses;

		CString	sex_idF;				// házastárs apjának blokkja
		CString peerF;
		CString titleF;
		CString titoloF;
		CString firstNameF;
		CString lastNameF;
		CString posteriorF;
		CString birthPlaceF;
		CString birthDateF;
		CString deathPlaceF;
		CString deathDateF;
		CString commentF;
		CString	rowidF;
		CString	sex_idM;				// házastárs anyjának blokkja
		CString peerM;
		CString titleM;
		CString titoloM;
		CString firstNameM;
		CString lastNameM;
		CString posteriorM;
		CString birthPlaceM;
		CString birthDateM;
		CString deathPlaceM;
		CString deathDateM;
		CString commentM;
		CString	rowidM;
		CString spouseFullname;
		CString father;					// a zárójelben megadott apa utóneve
		CString mother;					// a zárójelben megadott anya teljes neve
	} MARRIAGES;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Leszármazott házastársának további házastársai  v_spouseSpouses vektort alkot
	typedef struct
	{
		int		spouseIndex;		// a v_marriages vektor indexe
		CString	rowid;
		int		order;
		CString	sex_id;
		CString	husband_id;
		CString	wife_id;
		CString peer;
		CString title;
		CString titolo;
		CString last_name;
		CString first_name;
		CString posterior;
		CString comment;
		CString birth_place;
		CString birth_date;
		CString death_place;
		CString death_date;
	} SPOUSESPOUSES;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct
	{
		CString sex_id;
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
		CString place;
		CString date;
		CString comment;
	}PLACE_DATE_BLOCK;

	typedef struct
	{
		CString fullname;
		CString comment;
		CString	sex_id;
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
		CString tableRoman;		// a tábla római száma a családon belül
		CString tableHeader;	// a teljes tábla sor
		CString percent;		// %%% vagy %%
		CString familyName;		// a tableHeaddr-bõl kivett családnév
		CString titolo;			// a tableHeaderbõl kivett elõnév, ill. elõtt álló 
		CString alias;			// a tableHeaderbõl kivett alias
		CString comment;		// a tableHeaderbõl kivett comment
		CString torzs;
		CString arm;
		int		joint;
		CString folyt;			// a táblában elõforduló folyt sorok hivatkozásai vesszõvel elválasztva ( csak információ)
		CString csalad;
		CString peer;
		CString ancestor;		// a táble elsõ emberének rowid-je
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


	typedef struct
	{
		CString sh;
		CString lg;
	}PEERS;