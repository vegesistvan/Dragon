#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"


//////////////////////////////////////////////////////////////////////////////////////////////
// nameSubStr-bõl elõállított 
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
// vissza: 'ret' szóból áll a név, megfosztva az esetleges peer-tõl és posterior-tól
/*
bool CInputGA::splitNameSubstr( CString nameSubStr )

	int i;
	int j;
	int n = A->GetCount();
	int ret = n;
	CString word;
	CString res;
	bool	comma = false;
	TCHAR ch;

	m_comment.Empty();
	m_posterior.Empty();
	m_peer.Empty();


	for (i = k; i < n; ++i)  // a keresztnév keresése a k. szótól
	{
		word = A->GetAt(i);
		res = theApp.isFirstName(word);  // elteker az elsõ keresztnévig

		if (!res.IsEmpty())
			break;
	}
	if (i + 1 < n)		// talát keresztnevet és utána még van valami
	{
		// elteker az ezt követõ elsõ nem keresztnévig ( kivétel a Márkus és Batizfalvi típusú név, amin átmegy  )
		for (++i; i < n; ++i)
		{
			if (A->GetAt(i) != L"és")			// Márkus és Batizfalvi ....
			{
				word = A->GetAt(i);
				res = theApp.isFirstName(word);  // elteker az ezt követõ elsõ nem keresztnévig
				if (res.IsEmpty())
					break;
			}
		}
		ret = i;	// az utolsó keresznév utáni szó indexe

		if ((ch = getLastCharacter(A->GetAt(i - 1))) == ',')
			comma = true;		// a keresztnév után , következik!!

		if (i < n) // az utolsó keresztnév után talált valamit, ami nem keresztnév
		{
			if (!m_birthSubstr.IsEmpty() && !m_deathSubstr.IsEmpty())
				m_posterior = packWords(A, i, n - i);		// akkor csak utótag lehet
			else
			{
				m_comment = packWords(A, i, n - i);		// ez akkor mind leírás, amiben lehet fõnemesi cím is!!!	
				if (comma)
					m_comment.Format(L", %s", (CString)m_comment);
			}

			word = A->GetAt(i);
			if (isPeer(word))								// ez vagy peer
			{
				m_peer = getPeerFromString(word);		// csak azért, hogy a gróf-gr stb legyen
				++i;
			}

		}
	}
	return ret;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Megállapítja, hogy a bemeneti line stringben mely szavak alkotják a nevet, és mi van elõtte és utána.
// Ha igen, akkor azt felbontja elemeire a PEOPLE strukturába, 
// a maradák stringet az m_comment stringbe teszi és true értékkel tér vissza.
// 
// Ha úgy találja, hogy nem névvel kezdõdik a string, akkor false értékkel tér vissza.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isNameNew(CString line, CString* before, PEOPLE* p, CString* after)
{
	int pos;
	int i;
	int k;
	int end;			// név végének indexe, utána a posterior következik
	int cnt;
	int db;
	int num;
	int z;
	CStringArray A;
	CString word;

	CString sex;
	CString peer;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;

	CString name;
	CString comment;
	int j = -1;
	int iName;

	int n = wordList(&A, line, ' ', false);
	// név keresése
	// kezdjük a peer-rel és a title-val
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		if (isPeer(word))
		{
			if (peer.IsEmpty())
				iName = i;
			peer = word;
			A[i].Empty();
			j = i;
		}
		else if (isTitle(word))		// title több szóból is állhat
		{
			if (title.IsEmpty())
				iName = i;
			title += word;
			title += L" ";
			A[i].Empty();
			j = i;
		}
	}
	if (j == -1)	// nem volt se peer se title a stringben
		j = 1;		// a második szótól kezdjök a nevet keresni, mert legalább családnév van;	
	else
		++j;		// a peer, title utáni 2. szótól vizsgáljuk, mert
	for (j; j < n; ++j)
	{





	}


	return true;
}