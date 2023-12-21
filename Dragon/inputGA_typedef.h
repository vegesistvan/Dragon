#pragma once
#include <string>
#include "stdafx.h"
#include <afx.h>
	// beolvas�sn�l haszn�lja
#define MAXNUMOFSPOUSES  15



typedef struct
{
	CString value;
}VALUES;
 
const VALUES m_titles[] =
{
	L"dr",
	L"�zv",
	L"ifj",
	L"lg",
	L"ns",
	L"jur",
	L"�rgr",
	L"vit�z",
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



// Az egy sorban tal�lhat� emberek feldolgoz�sa �s insert�l�sa ut�n k�sz�ti el az a sor adataib�l az al�bbi blokkot, 
// amit a v_generations vektorba insert�l. A v_generations vektor egy t�bla kezdetekor �jra kezd�dik. 
//
// Egy sor feldolgoz�sa sor�n felhaszn�lja a kor�bban insert�lt blokkok k�z�l az el�z� testv�r�nek �s a sz�leinek
// az adatblokkj�t, amit a gen ( gener�ci�s k�d )-al azonos�t. Ezekb�l meghat�rozza a parentIndex-et,
// a descendant_id-t,  ami az apa rowid-ja valamint a 
// spouse_id[partent_index-1]-et ami az anya rowid-ja.



// Insert�l�s el�tt a vektor v�g�r�l eldobja a magasabb gener�cihoz tartoz� bejegyz�seket, ha vannak ilyenek, mert ezekre m�r nem lesz sz�ks�g
// A vektor v�g�r�l indulva megkeresi az els� azonos gener�ci�t, azaz a testv�r�t, ha volt ilyen. A testv�r�t�l �r�kli a parentIndex-et, ha mag�nak nem volt.
// Azt�n a mag�n�l egyel kisebb gener�ci�t keresi visszafele, amiben a sz�lei adatai vannak. Inne veszi �t az apja �s anyja 
// azonos�t�j�t

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
		CString spouse_id[MAXNUMOFSPOUSES];	// a v_marriages spouse_id �rt�ket teszi bele
		int		parentIndex;
		CString	descendant_sex_id;
		int		whichChildFather;	// apja hagyadik gyereke
		int		whichChildMother;	// anyja hanyadik gyereke	
		int		numOfSpouses;	// h�zast�rsainak sz�ma
		CString familyName;		
		CString title;			
		CString peer;			
	} GENERATIONS;


	typedef struct						// az ebb�l k�sz�lt vektor indexe a gener�ci� sorsz�ma ( 0-n)
	{									// az al�bbi adatok erre a gener�ci�ra vonatkoznak
		CString rowid;					// az utols�  
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
	// beolvas�sn�l d, s, sm, sf, ss strukt�r�ja
	typedef struct
	{
		CString	rowid;
		int fileNumber;
		int familyNumber;
		CString tableNumber;
		int	lineNumber;
		int	source;				// az ember t�pusa 1=lesz�rmazott, 2=h�zast�rsa, 3=h�zast�rs apja, 4=h�zast�rs anyja, 5=h�zast�rs tov�bbi h�zast�rsa
		int united;
		int spouse;
		int spouseparent;
		int spousespouse;
		TCHAR	generation;			// gener�ci�s karakter lesz�razott eset�n
		CString	sex_id;				// 1=f�rfi, 2= n�
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
		int		tableAncestry;			// TRUE: egy t�bla �se
		CString tableRoman;	// a t�bla fejl�c r�mai sz�ma
		CString arm;						// [] z�r�jelben l�v� string a csal�d �ga [ arm ] 
		CString sources;
		CString occupation;
		CString csalad;
		int		gap;
		CString folyt;						// "folyt" ut�ni r�mai sz�m, ha van.
		CString	father_id;
		CString	mother_id;
		int		parentIndex;
		int		spouseIndex;				// a lesz�rmazott h�zast�rs�n�l van megadva, hogy hanyadik h�zast�rsa a lesz�rmazottnak. Csak �sszevon�s el�tt van �rtelme!!
		int		whichHusband;			// az apja hanyadik f�rje az anyj�nak
		int		whichWife;			// az anyja hanyadik feles�ge az apj�nak  	( == parentIndex )
		int		whichChildFather;			// hanyadik gyerek az apj�nak
		int		whichChildMother;			// hanyadik gyereke az anyj�nak
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
	// Lesz�rmazott h�zast�rs�nak struktur�ja  v_marriages vektort alkot
	typedef struct
	{
		int		order;			// = jel el�tti sz�m ( vagy 0 )
		CString line;
		CString marriageSubstr;	// = jel ut�ni substr
		CString weddigSubstr;
		CString spouseSubstr;
		CString nameSubstr;
		CString birthSubstr;
		CString deathSubstr;
		CString relativesSubstr;
		CString parentsSubstr;
		CString brace;			// () z�r�jel tartalma
		CString parents;
		CString spouses;				// a z�r�jelben felsorolt tov�bbi h�zast�rsak
		CString arm;			// [ ] z�r�jel tartalma

		CString place;
		CString date;
		CString fullname;
		CString comment;

		CString spouse_id;
		CString	sex_id;					// h�zast�rs blokkja
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

		CString	sex_idF;				// h�zast�rs apj�nak blokkja
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
		CString	sex_idM;				// h�zast�rs anyj�nak blokkja
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
		CString father;					// a z�r�jelben megadott apa ut�neve
		CString mother;					// a z�r�jelben megadott anya teljes neve
	} MARRIAGES;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lesz�rmazott h�zast�rs�nak tov�bbi h�zast�rsai  v_spouseSpouses vektort alkot
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
		int		familyNumber;	// csal�d sorsz�ma ( 1- N )
		int		tableNumber;	// t�bla sorsz�am ( 1- N)
		int		lineNumber;		// a t�bla sor sorzs�am
		CString tableRoman;		// a t�bla r�mai sz�ma a csal�don bel�l
		CString tableHeader;	// a teljes t�bla sor
		CString percent;		// %%% vagy %%
		CString familyName;		// a tableHeaddr-b�l kivett csal�dn�v
		CString titolo;			// a tableHeaderb�l kivett el�n�v, ill. el�tt �ll� 
		CString alias;			// a tableHeaderb�l kivett alias
		CString comment;		// a tableHeaderb�l kivett comment
		CString torzs;
		CString arm;
		int		joint;
		CString folyt;			// a t�bl�ban el�fordul� folyt sorok hivatkoz�sai vessz�vel elv�lasztva ( csak inform�ci�)
		CString csalad;
		CString peer;
		CString ancestor;		// a t�ble els� ember�nek rowid-je
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