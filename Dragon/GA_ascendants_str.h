#pragma once

// felmenõk listázásánál használja
typedef struct
{
	int		orig;				// eredeti sorszám, a kijelölés sorrendje
	int		generation;			// generáció
	CString id;					// az emberünk rowid-je
	CString rowid;				// az apa rowid-je
	CString spouse_id;			// feleség rowid-ja
	CString name;				// neve
	int		sex_id;				// neme
	_int64	kekule;
	CString father_id;
	CString mother_id;

}FELMENOK;


typedef struct
{
	int		g;
	_int64	k;
	int		w;		// feleség ki volt-e már írva?
}KEKULE;

typedef struct
{
	int		g;
	_int64	k;
	int		w;		// feleség ki volt-e már írva?
	CString name;
	CString sex_id;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;		// a házasság helyének és idejének marriage táblából való lekérdezéséhez szükséges
}ASCENDANTS;


typedef struct
{
	int cnt;
	int	G;
	int K;
	CString name;
	CString sex_id;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;
}ASC; 

