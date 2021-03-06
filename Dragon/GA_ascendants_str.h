#pragma once

// felmen�k list�z�s�n�l haszn�lja
typedef struct
{
	int		orig;				// eredeti sorsz�m, a kijel�l�s sorrendje
	int		generation;			// gener�ci�
	CString id;					// az ember�nk rowid-je
	CString rowid;				// az apa rowid-je
	CString spouse_id;			// feles�g rowid-ja
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
	int		w;		// feles�g ki volt-e m�r �rva?
}KEKULE;

typedef struct
{
	int		g;
	_int64	k;
	int		w;		// feles�g ki volt-e m�r �rva?
	CString name;
	CString sex_id;
	CString rowid;
	CString father_id;
	CString mother_id;
	CString spouse_id;		// a h�zass�g hely�nek �s idej�nek marriage t�bl�b�l val� lek�rdez�s�hez sz�ks�ges
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

