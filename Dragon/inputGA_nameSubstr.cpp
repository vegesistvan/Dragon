#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

bool isLowerWord(CString word);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// name [[,] [posterior][comment]] sz�tszed�se elemeire
// A name[[,] [comment] elv�laszt� index keres�se.
// Az elv�laszt� index biztos nagyobb mint 1, mert vezet�kn�vnek mindenk�ppen kell lenni
// A 'name' v�ge: 
// 'name,' vagy 
//	a balr�l-jobbra els� keresztn�v, amit nem keresztn�v k�vet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processNameSubstr(PEOPLE* any)
{

	if (m_nameSubstr.IsEmpty()) return;

	bool ret;
	if (!(ret = person(m_nameSubstr, any)))
		ret = 1;
	if( TEST)
		fwprintf(textF, L"%10d %s\n", m_lineNumber, m_nameSubstr);
	return;

	CStringArray A;
	int pos;
	int i;
	int k; 
	CString word;
	CString word2;
	CString fullName;
	int sex;
	int from = 0;
	int to;
	int numOfFirstNames = 0;
	any->sex_id = 0;
	any->peer.Empty();
	any->title.Empty();
	any->titolo.Empty();
	any->last_name.Empty();
	any->first_name.Empty();
	any->posterior.Empty();


	std::vector<int> vSex;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;

	int z;

	int n = splitCString(m_nameSubstr, ' ', false, &A);
	
	
	for (i = 0; i < n; ++i)
	{
//		A[i].Remove('(');
//		A[i].Remove(')');
		A[i].Replace(',', ' ');						// a n�v ut�n lehet vessz� !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}

	if (m_nameSubstr.Find(L"Bedeghi �s Berencsi gr Ny�ry Eszter") != -1)
		z = 1;
///////////////////////////////////////////////////////////////////////////////////////////////////////	
	// leszedi a stringb�l a f�nemesi c�met �s a title
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		if (isLowerWord(word) )
		{
			if (isPeer(word))
			{
				any->peer = word;
			}
			else
			{
				title += word;
				title += L" ";
			}
		}
		else
			break;					// els� nemkisbet�s sz�ra kiugrik
	}
	if (i)
		from = i;
	if (!title.IsEmpty() )
	{
		title.TrimRight();
		any->title = title;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// leszedi a sor v�g�r�l az el� keresztn�v ut�n k�vetkez� nemkeresztn�vt�l kezdve: ez a posterior
	// k�zben gy�jti a keresztnevek nem�t
	// az els� sz�t, ami biztos nem keresztn�v, �tugorja, hogy az esetles n�v azonoss�g egy keresztn�vvel ne okozzon bajt
	for (i = from + 1; i < n; ++i)						// els� keresztn�v keres�se
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  els� keresztn�vig p�r�g
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utols� kereszn�v nem�t fogadja el
			break;
		}
	}
	for (i += 1 ; i < n; ++i)								// utols� keresztn�v keres�se
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  = els� nem keresztn�vig p�r�g
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utols� kereszn�v nem�t fogadja el
		}
		else
			break;
	}
	if (!vSex.size())
	{
		any->last_name = A[0];
		any->first_name = A[1];
		any->sex_id = -1;
		return;							// nincs keresztn�v!!
	}
	
//	for (int k = i; k < n; ++k)
	for ( k = i; k < n; ++k)
	{
		if (isPeer(A[k]))
		{
			any->peer = A[k];
			A[k].Empty();
			break;
		}
	}
	any->posterior = packWords(&A, i, n - i);
	fullName = packWords(&A, from, i - from);
///////////////////////////////////////////////////////////////////////////////////////////////////
// full nam m�csak [titolo] csal�d [csal�d2] kereszt[kereszt2][kereszt3]

	n = splitCString(fullName, ' ', false, &A);
	if (n < 2) return;

	///////////////////////////////////////////////////////////////////////////////////////////////////////	// csak 2 sz�b�l �ll, akkor az csal�d- �s keresztnevek
	if (n == 2)  // ha csak k�t tagb�l �ll, akkor az "csal�dn�v keresztn�v"
	{
		any->last_name = A[0];
		any->first_name = A[1];
		return;
	}

	for (i = 0; i < n; ++i)
	{
//		A[i].Remove('(');
//		A[i].Remove(')');
		A[i].Replace(',', ' ');						// a n�v ut�n lehet vessz� !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}
	i = 0;
	str = m_nameSubstr;
	// x �s y EL�NEVEK AZONOS�T�SA
	if (i + 1 < n && A[i + 1] == L"�s")					// X �s Y, ahol X nem i-re v�gz�dik
	{
		any->titolo = packWords(&A, i, 3);
		i += 3;
	}         
	else if (((i + 2) < n) && A[i + 2] == L"�s")		// Giczi, Assa �s Abl�nck�rti Ghyczy Anna M�ria
	{
		any->titolo = packWords(&A, i, 4);
		i += 4;
	}
	// Xi el�nevek azonos�t�sa	
	else if (isLastCharacter(A[i], 'i'))					// i-re v�gz�d� egytag� el�n�v azonos�t�sa 
	{
		str = m_nameSubstr;
		if ((sex = theApp.isFirstName(A[i + 1])) != -1)		// az ezt k�vet� sz� kereszn�v, ami lehet csal�dn�v is
		{
			if (i + 2 < n)	// azaz m�r csak 3 tag van h�tra, akkor ezek: titolo, lastname, firstname
			{
				if (A[i + 2][0] != '(')
				{
					any->titolo = A[i];
					any->last_name = A[i + 1];
					any->first_name = A[i + 2];
				}
				else
				{
					any->last_name = A[i];
					any->first_name = A[i + 1];
					any->posterior = A[i + 2];
				}
				return;
			}
			else if (i + 1 < n)							// m�r csak 2 sz� van h�tra, akkor azok: lasstname, firstname
			{
				any->last_name = A[i];
				any->first_name = A[i + 1];
			}
			else
			{
				any->last_name = A[i];
				any->first_name = packWords(&A, i + 1, n - i - 1);
				if (i > 0)
				{
					any->titolo = A[0];
				}
			}
//			str = m_nameSubstr;
//			return;
		}
		else
		{
			any->titolo = A[i];
			++i;
		}
	}
	// kett�s vezet�knevek kezel�se
	// i-n a csal�d- �s keresztenevek sorozata
	// a csal�dn�v is lehet kett�s �s a kerezstn�v is lehet kett�s, vagy t�bbsz�r�s
	str = m_nameSubstr;
	numOfFirstNames = vSex.size();
	for (int i = 0; i < vSex.size(); ++i)
	{
		if (vSex.at(i) != any->sex_id)
		{
			--numOfFirstNames;
		}
	}
//	k = n - i;			// a szavak sz�ma a csal�dn�v ut�n nem j�!!
	first_name = packWords(&A, n - numOfFirstNames, numOfFirstNames);
	if (isLowerWord(A[i]) )
	{
		if (isPeer(A[i]))
			any->peer = A[i];
		else
			any->posterior = A[i];
		++i;
	}
	k = n - i;			// a szavak sz�ma a csal�dn�v ut�n nem j�!!

	last_name = packWords(&A, i, k - numOfFirstNames);
	if (last_name.IsEmpty() && !first_name.IsEmpty())
	{
		last_name = getFirstWord(first_name);
		first_name = dropFirstWord(first_name);
	}

	any->first_name = first_name;
	any->last_name = last_name;


}
///////////////////////////////////////////////////////////////////////////////////////////////
bool isLowerWord(CString word)
{
	
	for (int i = 0; i < word.GetLength(); ++i)
	{
		if (!iswlower(word.GetAt(i)))
			return false;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// name [[,] [posterior][comment]] sz�tszed�se elemeire
// A name[[,] [comment] elv�laszt� index keres�se.
// Az elv�laszt� index biztos nagyobb mint 1, mert vezet�kn�vnek mindenk�ppen kell lenni
// A 'name' v�ge: 
// 'name,' vagy 
//	a balr�l-jobbra els� keresztn�v, amit nem keresztn�v k�vet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processNameSubstr_uj(PEOPLE* any)
{
	/*
	if (m_nameSubstr.IsEmpty()) return;

	bool ret;
	if (!(ret = person(m_nameSubstr, any )))
		fwprintf(textF, L"%10d %s\n", m_lineNumber, m_nameSubstr);
	return;
	*/

	CStringArray A;
	int pos;
	int i;
	int k;
	CString word;
	CString word2;
	CString fullName;
	int sex;
	int from = 0;
	int to;
	int numOfFirstNames = 0;
	any->sex_id = 0;
	any->peer.Empty();
	any->title.Empty();
	any->titolo.Empty();
	any->last_name.Empty();
	any->first_name.Empty();
	any->posterior.Empty();


	std::vector<int> vSex;
	CString title;
	CString titolo;
	CString last_name;
	CString first_name;
	CString posterior;
	CString peer;

	int z;

	int n = splitCString(m_nameSubstr, ' ', false, &A);
	n = getPosterior(&A, 0, &posterior, &peer);   // m�r nincs posterior!!

	for (i = 0; i < n; ++i)
	{
		A[i].Replace(',', ' ');						// a n�v ut�n lehet vessz� !!
		A[i].Trim();
	}

	if (m_nameSubstr.Find(L"Bedeghi �s Berencsi gr Ny�ry Eszter") != -1)
		z = 1;
	///////////////////////////////////////////////////////////////////////////////////////////////////////	
		// leszedi az elej�r�l a kisbet�s szavakat, ami lehet f�nemesi c�m vagy title
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		if (isLowerWord(word))
		{
			
			if (isPeer(word))
			{
				any->peer = word;
			}
			else
			{
				title += word;
				title += L" ";
			}
		}
		else
			break;					// els� nemkisbet�s sz�ra kiugrik
	}
	if (i)
		from = i;
	if (!title.IsEmpty())
	{
		title.TrimRight();
		any->title = title;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
		// leszedi a sor v�g�r�l az el� keresztn�v ut�n k�vetkez� nemkeresztn�vt�l kezdve: ez a posterior
		// k�zben gy�jti a keresztnevek nem�t
		// az els� sz�t, ami biztos nem keresztn�v, �tugorja, hogy az esetles n�v azonoss�g egy keresztn�vvel ne okozzon bajt
	for (i = from + 1; i < n; ++i)						// els� keresztn�v keres�se
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  els� keresztn�vig p�r�g
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utols� kereszn�v nem�t fogadja el
			break;
		}
	}
	for (i += 1; i < n; ++i)								// utols� keresztn�v keres�se
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  = els� nem keresztn�vig p�r�g
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utols� kereszn�v nem�t fogadja el
		}
		else
			break;
	}
	if (!vSex.size())
	{
		any->last_name = A[0];
		any->first_name = A[1];
		any->sex_id = -1;
		return;							// nincs keresztn�v!!
	}

	//	for (int k = i; k < n; ++k)
	for (k = i; k < n; ++k)
	{
		if (isPeer(A[k]))
		{
			any->peer = A[k];
			A[k].Empty();
			break;
		}
	}
	any->posterior = packWords(&A, i, n - i);
	fullName = packWords(&A, from, i - from);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// full nam m�csak [titolo] csal�d [csal�d2] kereszt[kereszt2][kereszt3]

	n = splitCString(fullName, ' ', false, &A);
	if (n < 2) return;

	///////////////////////////////////////////////////////////////////////////////////////////////////////	// csak 2 sz�b�l �ll, akkor az csal�d- �s keresztnevek
	if (n == 2)  // ha csak k�t tagb�l �ll, akkor az "csal�dn�v keresztn�v"
	{
		any->last_name = A[0];
		any->first_name = A[1];
		return;
	}

	for (i = 0; i < n; ++i)
	{
		//		A[i].Remove('(');
		//		A[i].Remove(')');
		A[i].Replace(',', ' ');						// a n�v ut�n lehet vessz� !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}
	i = 0;
	str = m_nameSubstr;
	// x �s y EL�NEVEK AZONOS�T�SA
	if (i + 1 < n && A[i + 1] == L"�s")					// X �s Y, ahol X nem i-re v�gz�dik
	{
		any->titolo = packWords(&A, i, 3);
		i += 3;
	}
	else if (((i + 2) < n) && A[i + 2] == L"�s")		// Giczi, Assa �s Abl�nck�rti Ghyczy Anna M�ria
	{
		any->titolo = packWords(&A, i, 4);
		i += 4;
	}
	// Xi el�nevek azonos�t�sa	
	else if (isLastCharacter(A[i], 'i'))					// i-re v�gz�d� egytag� el�n�v azonos�t�sa 
	{
		str = m_nameSubstr;
		if ((sex = theApp.isFirstName(A[i + 1])) != -1)		// az ezt k�vet� sz� kereszn�v, ami lehet csal�dn�v is
		{
			if (i + 2 < n)	// azaz m�r csak 3 tag van h�tra, akkor ezek: titolo, lastname, firstname
			{
				if (A[i + 2][0] != '(')
				{
					any->titolo = A[i];
					any->last_name = A[i + 1];
					any->first_name = A[i + 2];
				}
				else
				{
					any->last_name = A[i];
					any->first_name = A[i + 1];
					any->posterior = A[i + 2];
				}
				return;
			}
			else if (i + 1 < n)							// m�r csak 2 sz� van h�tra, akkor azok: lasstname, firstname
			{
				any->last_name = A[i];
				any->first_name = A[i + 1];
			}
			else
			{
				any->last_name = A[i];
				any->first_name = packWords(&A, i + 1, n - i - 1);
				if (i > 0)
				{
					any->titolo = A[0];
				}
			}
			//			str = m_nameSubstr;
			//			return;
		}
		else
		{
			any->titolo = A[i];
			++i;
		}
	}
	// kett�s vezet�knevek kezel�se
	// i-n a csal�d- �s keresztenevek sorozata
	// a csal�dn�v is lehet kett�s �s a kerezstn�v is lehet kett�s, vagy t�bbsz�r�s
	str = m_nameSubstr;
	numOfFirstNames = vSex.size();
	for (int i = 0; i < vSex.size(); ++i)
	{
		if (vSex.at(i) != any->sex_id)
		{
			--numOfFirstNames;
		}
	}
	//	k = n - i;			// a szavak sz�ma a csal�dn�v ut�n nem j�!!
	first_name = packWords(&A, n - numOfFirstNames, numOfFirstNames);
	if (isLowerWord(A[i]))
	{
		if (isPeer(A[i]))
			any->peer = A[i];
		else
			any->posterior = A[i];
		++i;
	}
	k = n - i;			// a szavak sz�ma a csal�dn�v ut�n nem j�!!

	last_name = packWords(&A, i, k - numOfFirstNames);
	if (last_name.IsEmpty() && !first_name.IsEmpty())
	{
		last_name = getFirstWord(first_name);
		first_name = dropFirstWord(first_name);
	}

	any->first_name = first_name;
	any->last_name = last_name;


}