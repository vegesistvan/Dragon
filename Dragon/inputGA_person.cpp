#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Megállapítja, hogy a line string elején szeméylnév van-e. 
// Ha igen, akkor azt felbontja elemeire a PEOPLE strukturába, és true értékkel tér vissza.
// Ha úgy találja, hogy nem személynévvel kezdõdik a string, akkor false értékkel tér vissza.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::person(CString line, PEOPLE* p)
{
	int pos;
	int i;
	int k;
	int m;				// az összes szó száma
	int n;				// csak a posteriorig terjedõ szavak száma
	int end;			// név végének indexe, utána a posterior következik
	int cnt;
	int db;
	int num;		
	int z;
	CStringArray A;
	CString word;

	int sex;
	CString peer;
	CString title;
	CString titolo;
	CString last_name;
	CString cogname;
	CString first_name;
	CString posterior;
	CString name;
	CString comment;


	m = wordList(&A, line, ' ', false);
	if (m < 2) return false;		// egy név legalább 2 tagból áll

	n = getPosterior(&A, 1, &posterior, &peer );
//	p->posterior = posterior;
//	p->peer = peer;

	// az A-ban n-ig csak a név van, posterior levágva!
// innentõl új!	
	for (i = 0; i < n; ++i)
	{
		if (!iswalpha(A[i][0])) return false;		// a név minden elemének betûvel kell kezdõdni

		if (isPeer(A[i]))
		{
//			p->peer = A[i];
			peer = A[i];
			A[i].Empty();
		}
		else if (isTitle(A[i]))
		{
			title += A[i];
			title += L" ";
			A[i].Empty();
		}
	}
	title.Trim();

	name = packWords(&A, 0, n);
	n = wordList(&A, name, ' ', false);


	// = Kovács Adél, 
 	for (i = 0; i < n; ++i)
	{
		word = A.GetAt(i);
		if (word.IsEmpty())
			z = 2;
		if (word.GetAt(word.GetLength() - 1) == ',' )
		{
			name = packWords(&A, 0, i + 1);
			name.Remove(',');
			p->comment = packWords(&A, i+1, n-i-1);
			break;
		}
	}

	
	

//	if ((pos = name.Find(',')) != -1 && pos < name.GetLength() - 1)
//	{
//		return false;
//	}

	n = wordList(&A, name, ' ', false);
 	if (n < 2) return false;		// egy név legalább 2 tagból áll

	if (n > 2)
		i = isTitolo(&A, n, 0, &titolo);  // a titolo csak akkor lehet, ha utána még legalább 2 szó van
	else
		i = 0;
//	p->titolo = titolo;

	num = getLastName(&A, n, i, &last_name);

//	p->last_name = last_name;
	i += num;
	num = getFirstName(&A, i, &first_name, &sex);
	if (!iswupper(first_name[0])) return false;

//	p->first_name = first_name;
//	p->sex_id = sex;

	if (first_name.IsEmpty() || last_name.IsEmpty())
		return false;

	p->peer = peer;
	p->titolo = titolo;
	p->title = title;
	p->last_name = last_name;
	p->first_name = first_name;
	p->sex_id = sex;
	p->posterior = posterior;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInputGA::isTitle( CString word )
{
	for (int i = 0; i < numOfTitles; ++i)
	{
		if (word == m_titles[i].value)
			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az A[i]-tõl kezdve megnézi, hogy titolo következik-e.
// Ha igen, akkor beteszi a titolo-ba, és visszadja a titolo szavainak számát
// Ha nem, akkor o-t ad vissza
// titolo egy szó-sor, ha
// X és Y
// X,Y és Z
// Xi     ( i-re végzõdõ szó. Ez lehet családnév is!!!!!!!
int CInputGA::isTitolo(CStringArray* A, int n, int i, CString* titolo)
{
	CString word1;
	CString word2;
	CString word3;
	CString word4;
	CString word5;
	CString word6;
	TCHAR kar;
	int cnt = 0; // a titolo szavainak száma
	// X és Y típusú elõnevek azonosítása

	word1 = A->GetAt(i);
	if (!iswalpha(word1[0])) return 0;
	if (iswlower(word1[0])) return 0;

	if (i + 1 < n)
		word2 = A->GetAt(i + 1);
	if (i + 2 < n)
		word3 = A->GetAt(i + 2);
	if (i + 3 < n)
		word4 = A->GetAt(i + 3);
	if (i + 4 < n)
		word5 = A->GetAt(i + 4);
	if (i + 5 < n)
		word6 = A->GetAt(i + 5);

	if (word2 == L"és")  // Márkus és Batizfalvi Király Zoltán
	{
		if (iswupper(word3[0]) && iswupper(word4[0]) &&  iswupper(word5[0]))
			cnt = 3;
	}
	else if (word3 == L"és")	// Giczi, Assa és Ablánckürti Ghyczy Anna Mária 
	{
		if (iswupper(word2[0]) && iswupper(word4[0]) && iswupper(word5[0] && iswupper(word6[0])))
			cnt = 4;
	}
	else if ((kar = getLastCharacter(word1)) == 'i')
	{
		if (iswupper(word2[0]) && iswupper(word3[0]))
			cnt = 1;
	}
	*titolo = packWords(A, i, cnt);
	return cnt;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// Az A tönbb i-edik elemétõl keresi a családnevek
// Csaaládnévnek tekinti az elsõ szót és a további nem keresztneveket
int CInputGA::getLastName(CStringArray* A, int n, int i, CString *lastname)
{
	CString word;
	int cnt = 1;
	*lastname = A->GetAt(i);
	*lastname += L" ";
	for (++i; i < n; ++i)
	{
		word = A->GetAt(i);
		if (theApp.isFirstName(word) == -1)
		{
			*lastname += word;
			*lastname += L" ";
			++cnt;
		}
		else
		{
			break;
		}
	}
	*lastname->TrimRight();
	return cnt;
}
///////////////////////////////////////////////////////////////////////////////////////////////
int CInputGA::getFirstName(CStringArray* A, int i, CString* firstname, int *sex )
{
	CString word;
	int cnt = 0;
	int ret;
	int n = A->GetCount();

	for (i; i < n; ++i) // összeszedi a keresztneveket
	{
		word = A->GetAt(i);
//		word.Remove(',');
		if ((ret = theApp.isFirstName(word)) != -1) 
		{
			*firstname += word;
			*firstname += L" ";
			*sex = ret;
			++cnt;
		}
		else
			break;
	}
	*firstname->TrimRight();
	return cnt;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
int CInputGA::getPosterior(CStringArray* A, int k, CString* posterior, CString *peer )
{
	
	int i;
	int n = A->GetCount();
	int ret = n;
	CString word;


	for (i = k; i < n; ++i)  // a keresztnév kerseése a k. szótól
	{
		word = A->GetAt(i);
		if (theApp.isFirstName(A->GetAt(i)) != -1)  // elteker az elsõ keresztnévig
			break;
	}
	if (i < n)		// talát keresztnevet
	{
		for (++i; i < n; ++i)
		{
			if (A->GetAt(i) != L"és")			// Márkus és Batizfalvi ....
			{
				word = A->GetAt(i);
				if (theApp.isFirstName(word) == -1)  // elteker az ezt követõ elsõ nem keresztnévig
					break;
			}
		}
		ret = i;
		if (i < n)
		{
			word = A->GetAt(i);
			if (isPeer(word))
			{
				*peer = word;
				++i;
				

			}
			*posterior = packWords(A, i, n - i);
		}
	}
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
int CInputGA::findFirstDate( CStringArray *A )
{
	CString word;
	CString date;
	int n = A->GetCount();
	int i;
	int db;
	for (i = 0; i < n; ++i)
	{
		word = A->GetAt(i);
		if ((db = isDate( A, i, &date )) )
			break;
	}
	return i;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
int CInputGA::findFirstFirstName(CStringArray* A)
{
	CString word;
	int n = A->GetCount();
	int i;
	int sex;
	for (i = 0; i < n; ++i)
	{
		word = A->GetAt(i);
		if ((sex = theApp.isFirstName(word)) != -1)
			break;
	}
	return i;
}