
#include "utilities_dragon.h"
#include "utilities.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egys�gesen 1944.12.17 [modifier   alakra alak�tja a d�tumot, ami lehet:
// 
// [modifier 
//
//

CString date2date(CString date)
{
	const MONTHES monthes[] =
	{
		{ L"Jan", 1 },
		{ L"Feb", 2 },
		{ L"Mar", 3 },
		{ L"Apr", 4 },
		{ L"May", 5 },
		{ L"Jun", 6 },
		{ L"Jul", 7 },
		{ L"Aug", 8 },
		{ L"Sep", 9 },
		{ L"Oct", 10 },
		{ L"Nov", 11 },
		{ L"Dec", 12 },
		{ L"jan.", 1 },
		{ L"febr.", 2 },
		{ L"m�rc.", 3 },
		{ L"�pr.", 4 },
		{ L"m�j.", 5 },
		{ L"j�n.", 6 },
		{ L"j�l.", 7 },
		{ L"aug.", 8 },
		{ L"szept.", 9 },
		{ L"okt.", 10 },
		{ L"nov.", 11 },
		{ L"dec.", 12 }
	};
	int numOfMonthes = sizeof(monthes) / sizeof(MONTHES);

	MODIFIERS modifiers[] =
	{
		{ L"BEF", L"el�tt"},
		{ L"AFT", L"ut�n" },
		{ L"FROM", L"-t�l"},
		{ L"ABOUT", L"k�r�l"},
		{ L"APPR",  L"kb" },
	};
	int numOfModifiers = sizeof modifiers / sizeof (MODIFIERS );
	

	CString modifier;
	int year;
	int month;
	int day;

	CStringArray A;
	CString honap;
	CString str;

	int pos;
	int n;
	int	i;

	/*
	CString modi[] = { L"kb", L"el�tt", L"ut�n", L"k�r�l" };
	int pos1 = -1;
	int pos2 = -1;
	


	if ((pos1 = date.Find(L"FROM")) != -1)
		n = 1;

	if ((pos1 = date.Find(L"BEF")) != -1 || (pos2 = date.Find(L"AFT")) != -1)
	{
		if (pos1 != -1) modifier = L" el�tt";
		else if (pos2 != -1) modifier = L" ut�n";
		date = date.Mid(4);
	}

	for (i = 0; i < sizeof(TCHAR) / sizeof(modi); ++i)
	{
		if ((pos1 = date.Find(modi[i])) != -1)
		{
			modifier = modi[i];
			date = date.Left(pos1);
			break;
		}
	}
*/

	for (i = 0; i < numOfModifiers; ++i)
	{
		if ((pos = date.Find(modifiers[i].english)) != -1)
		{
			modifier = modifiers[i].hungarian;
			date = date.Mid(modifiers[i].english.GetLength());
		}
	}


	n = wordList(&A, date, ' ', FALSE);

	if (n != 3)
	{
		date.Remove('(');
		date.Remove(')');
		date.Trim();

		int len = date.GetLength();
		if (len == 11)
		{
			if (date.GetAt(len - 1) == '.')
			{
				str = date.Left(len - 1);
				date = str;
			}
		}
		return(date);
	}
	else
	{										
		day = _wtoi(A.GetAt(0));
		honap = A.GetAt(1);
		year = _wtoi(A.GetAt(2));

		// 17 DEC 1944
		for (int i = 0; i < numOfMonthes; i++)
		{
			honap = A.GetAt(1);
			honap.Trim();
			if (!honap.CompareNoCase( monthes[i].honap )	)
			{
				month = monthes[i].month;
				break;
			}
		}
	}
	str.Format(L"%4d.%02d.%02d", year, month, day);
	str += L" ";
	str += modifier;
	str.Trim();
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// El�fordulhatnak csupa nagybet�s csal�dnevek a GA.html �s a gedcon f�jlokban is.
// Ezeket konvert�lni kell kisbet�sre
// Figyelem!!: pl. d'URSE, McGREGOR, SZENT-IV�NY
CString convertNameToLower(CString name)
{
	int len = name.GetLength();
	int	i;
	CString out;
	TCHAR kar;

	out = name[0];									// az els� karaktert semmik�ppen nem konvert�ljuk
	for (i = 1; i < len; ++i)
	{
		if ( iswupper(name[i - 1]) && iswupper(name[i]) )
		{										// akkor konvert�ljuk kisbet�re, ha a karakter maga nagybet� �s az azt megel�z�
			kar = towlower(name[i]);			// karakter is nagybet�
			out += kar;
		}
		else
			out += name[i];
	
	}
	return out;
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// K�rni kehet csupa nagybet�s csal�dneveket a lesz�rmazotti list�n.
// Ezelket kell konvert�ni nagybet�sre.
// Figyelem! d.Urse, McGregor, Szent-Iv�ny
CString convertNameToUpper(CString name)
{
	int len = name.GetLength();
	int	i;
	CString out;
	TCHAR kar;

	out = name[0];								// az els� karaktert semmik�ppen nem konvert�ljuk
	for (i = 1; i < len-1; ++i)
	{
		if( iswlower(name[i+1] ) || !iswalpha( name[i+1] ) )
		{										// akkor konvert�ljuk nagybet�re, ha a k�vetkez� karakter
			kar = towupper(name[i]);			// kisbet� vagy nem bet�
			out += kar;
		}
		else
			out += name[i];

	}
	kar = towupper(name[i]);
	out += kar;
	return out;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// egy GEDCOM sor felbont�sa 'level-xref-tag-value' strukt�r�ba 
//
// return: lxtv structure
bool extract(CString cLine, GEDLINE* lxtv)
{
	CString line = cLine;
	CString word1;
	CString word2;
	CString word3;
	CString left3;

	CString msg;
	CString str;

	int pos;
	int pos2;
	int pos3;
	int pos4;
	int z;

	lxtv->level = -1;
	lxtv->xref.Empty();
	lxtv->value.Empty();

	if ((pos = cLine.Find(L"ADR1")) != -1)
		z = 1;


	// az els� 3 sz� megbat�roz�sa
	line.Trim();
	if ((pos2 = line.Find(' ')) == -1)				// 1. sz� meghat�roz�sa
	{
		return false;								// csak 1 sz� van, ami nem lehet
	}
	word1 = line.Left(pos2);						// 1. sz�

	line = line.Mid(pos2 + 1);
	if ((pos3 = line.Find(' ')) == -1)				// csak 2 sz� van
		word2 = line;								// 2. sz�
	else
	{
		word2 = line.Left( pos3 );					// 3 vagy t�bb sz� is van
		left3 = line.Mid(pos3+1);

		if ((pos4 = left3.Find(' ')) == -1)			// 4. sz� nincs
			word3 = left3;							// csak 3 sz� van, 
		else
			word3 = left3.Left(pos4);				// lehet 3-n�l t�bb sz� is: 3. sz�
	}

	
	// a szavak ellen�rz�se �s lxtv �rt�kad�sa
	if (!isNumeric(word1))
	{
		return false;
	}
	lxtv->level = _wtoi(word1);						// 1. sz� mindenk�popen level

	if ( word2.GetAt(0) == '@' )					// 2. sz� @ref@ vagy TAG
	{
		if( word3.IsEmpty() )						// 2. sz� @ref@, akkor kell lenni 3. sz�nak is!
		{
			return false;
		}
		lxtv->xref = word2;							// 2. sz� @ref@
		lxtv->tag = word3;
	}
	else
	{
		lxtv->tag = word2;							// 2. sz� TAG, 3. vagy van, vagy nincs	
		lxtv->value = left3;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortCBySource(const UNITE_ENTRIES& v1, const UNITE_ENTRIES& v2)
{
	return(v1.source < v2.source);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void splitPathName(CString pathName, CString* fileTitle, CString* fileName, CString* folderPath)
{
	int pos;
	CString fp;
	CString fn;
	CString ft;

	if ((pos = pathName.ReverseFind('\\')) != -1)
	{
		*folderPath = pathName.Left(pos);
		fn = pathName.Mid(pos + 1);
		if ((pos = fn.ReverseFind('.')) != -1)
		{
			*fileTitle = fn.Left(pos);
		}
	}
	*fileName = fn;
}








