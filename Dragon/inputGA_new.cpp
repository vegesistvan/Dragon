#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"


//////////////////////////////////////////////////////////////////////////////////////////////
// nameSubStr-b�l el��ll�tott 
// visszaadja a posterior kezdet�nek index�t �s a posterior-t a posteriorba teszi
// vissza: 'ret' sz�b�l �ll a n�v, megfosztva az esetleges peer-t�l �s posterior-t�l
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


	for (i = k; i < n; ++i)  // a keresztn�v keres�se a k. sz�t�l
	{
		word = A->GetAt(i);
		res = theApp.isFirstName(word);  // elteker az els� keresztn�vig

		if (!res.IsEmpty())
			break;
	}
	if (i + 1 < n)		// tal�t keresztnevet �s ut�na m�g van valami
	{
		// elteker az ezt k�vet� els� nem keresztn�vig ( kiv�tel a M�rkus �s Batizfalvi t�pus� n�v, amin �tmegy  )
		for (++i; i < n; ++i)
		{
			if (A->GetAt(i) != L"�s")			// M�rkus �s Batizfalvi ....
			{
				word = A->GetAt(i);
				res = theApp.isFirstName(word);  // elteker az ezt k�vet� els� nem keresztn�vig
				if (res.IsEmpty())
					break;
			}
		}
		ret = i;	// az utols� kereszn�v ut�ni sz� indexe

		if ((ch = getLastCharacter(A->GetAt(i - 1))) == ',')
			comma = true;		// a keresztn�v ut�n , k�vetkezik!!

		if (i < n) // az utols� keresztn�v ut�n tal�lt valamit, ami nem keresztn�v
		{
			if (!m_birthSubstr.IsEmpty() && !m_deathSubstr.IsEmpty())
				m_posterior = packWords(A, i, n - i);		// akkor csak ut�tag lehet
			else
			{
				m_comment = packWords(A, i, n - i);		// ez akkor mind le�r�s, amiben lehet f�nemesi c�m is!!!	
				if (comma)
					m_comment.Format(L", %s", (CString)m_comment);
			}

			word = A->GetAt(i);
			if (isPeer(word))								// ez vagy peer
			{
				m_peer = getPeerFromString(word);		// csak az�rt, hogy a gr�f-gr stb legyen
				++i;
			}

		}
	}
	return ret;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Meg�llap�tja, hogy a bemeneti line stringben mely szavak alkotj�k a nevet, �s mi van el�tte �s ut�na.
// Ha igen, akkor azt felbontja elemeire a PEOPLE struktur�ba, 
// a marad�k stringet az m_comment stringbe teszi �s true �rt�kkel t�r vissza.
// 
// Ha �gy tal�lja, hogy nem n�vvel kezd�dik a string, akkor false �rt�kkel t�r vissza.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::isNameNew(CString line, CString* before, PEOPLE* p, CString* after)
{
	int pos;
	int i;
	int k;
	int end;			// n�v v�g�nek indexe, ut�na a posterior k�vetkezik
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
	// n�v keres�se
	// kezdj�k a peer-rel �s a title-val
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
		else if (isTitle(word))		// title t�bb sz�b�l is �llhat
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
		j = 1;		// a m�sodik sz�t�l kezdj�k a nevet keresni, mert legal�bb csal�dn�v van;	
	else
		++j;		// a peer, title ut�ni 2. sz�t�l vizsg�ljuk, mert
	for (j; j < n; ++j)
	{





	}


	return true;
}