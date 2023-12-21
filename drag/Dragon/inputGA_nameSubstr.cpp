#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

bool isLowerWord(CString word);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// name [[,] [posterior][comment]] szétszedése elemeire
// A name[[,] [comment] elválasztó index keresése.
// Az elválasztó index biztos nagyobb mint 1, mert vezetéknévnek mindenképpen kell lenni
// A 'name' vége: 
// 'name,' vagy 
//	a balról-jobbra elsõ keresztnév, amit nem keresztnév követ
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
		A[i].Replace(',', ' ');						// a név után lehet vesszõ !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}

	if (m_nameSubstr.Find(L"Bedeghi és Berencsi gr Nyáry Eszter") != -1)
		z = 1;
///////////////////////////////////////////////////////////////////////////////////////////////////////	
	// leszedi a stringbõl a fõnemesi címet és a title
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
			break;					// elsõ nemkisbetûs szóra kiugrik
	}
	if (i)
		from = i;
	if (!title.IsEmpty() )
	{
		title.TrimRight();
		any->title = title;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////
	// leszedi a sor végérõl az elõ keresztnév után következõ nemkeresztnévtõl kezdve: ez a posterior
	// közben gyûjti a keresztnevek nemét
	// az elsõ szót, ami biztos nem keresztnév, átugorja, hogy az esetles név azonosság egy keresztnévvel ne okozzon bajt
	for (i = from + 1; i < n; ++i)						// elsõ keresztnév keresése
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  elsõ keresztnévig pörög
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utolsó keresznév nemét fogadja el
			break;
		}
	}
	for (i += 1 ; i < n; ++i)								// utolsó keresztnév keresése
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  = elsõ nem keresztnévig pörög
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utolsó keresznév nemét fogadja el
		}
		else
			break;
	}
	if (!vSex.size())
	{
		any->last_name = A[0];
		any->first_name = A[1];
		any->sex_id = -1;
		return;							// nincs keresztnév!!
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
// full nam mácsak [titolo] család [család2] kereszt[kereszt2][kereszt3]

	n = splitCString(fullName, ' ', false, &A);
	if (n < 2) return;

	///////////////////////////////////////////////////////////////////////////////////////////////////////	// csak 2 szóból áll, akkor az család- és keresztnevek
	if (n == 2)  // ha csak két tagból áll, akkor az "családnév keresztnév"
	{
		any->last_name = A[0];
		any->first_name = A[1];
		return;
	}

	for (i = 0; i < n; ++i)
	{
//		A[i].Remove('(');
//		A[i].Remove(')');
		A[i].Replace(',', ' ');						// a név után lehet vesszõ !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}
	i = 0;
	str = m_nameSubstr;
	// x és y ELÕNEVEK AZONOSÍTÁSA
	if (i + 1 < n && A[i + 1] == L"és")					// X és Y, ahol X nem i-re végzõdik
	{
		any->titolo = packWords(&A, i, 3);
		i += 3;
	}         
	else if (((i + 2) < n) && A[i + 2] == L"és")		// Giczi, Assa és Ablánckürti Ghyczy Anna Mária
	{
		any->titolo = packWords(&A, i, 4);
		i += 4;
	}
	// Xi elõnevek azonosítása	
	else if (isLastCharacter(A[i], 'i'))					// i-re végzõdõ egytagú elõnév azonosítása 
	{
		str = m_nameSubstr;
		if ((sex = theApp.isFirstName(A[i + 1])) != -1)		// az ezt követõ szó keresznév, ami lehet családnév is
		{
			if (i + 2 < n)	// azaz már csak 3 tag van hátra, akkor ezek: titolo, lastname, firstname
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
			else if (i + 1 < n)							// már csak 2 szó van hátra, akkor azok: lasstname, firstname
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
	// kettõs vezetéknevek kezelése
	// i-n a család- és keresztenevek sorozata
	// a családnév is lehet kettõs és a kerezstnév is lehet kettõs, vagy többszörös
	str = m_nameSubstr;
	numOfFirstNames = vSex.size();
	for (int i = 0; i < vSex.size(); ++i)
	{
		if (vSex.at(i) != any->sex_id)
		{
			--numOfFirstNames;
		}
	}
//	k = n - i;			// a szavak száma a családnév után nem jó!!
	first_name = packWords(&A, n - numOfFirstNames, numOfFirstNames);
	if (isLowerWord(A[i]) )
	{
		if (isPeer(A[i]))
			any->peer = A[i];
		else
			any->posterior = A[i];
		++i;
	}
	k = n - i;			// a szavak száma a családnév után nem jó!!

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
// name [[,] [posterior][comment]] szétszedése elemeire
// A name[[,] [comment] elválasztó index keresése.
// Az elválasztó index biztos nagyobb mint 1, mert vezetéknévnek mindenképpen kell lenni
// A 'name' vége: 
// 'name,' vagy 
//	a balról-jobbra elsõ keresztnév, amit nem keresztnév követ
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
	n = getPosterior(&A, 0, &posterior, &peer);   // már nincs posterior!!

	for (i = 0; i < n; ++i)
	{
		A[i].Replace(',', ' ');						// a név után lehet vesszõ !!
		A[i].Trim();
	}

	if (m_nameSubstr.Find(L"Bedeghi és Berencsi gr Nyáry Eszter") != -1)
		z = 1;
	///////////////////////////////////////////////////////////////////////////////////////////////////////	
		// leszedi az elejérõl a kisbetûs szavakat, ami lehet fõnemesi cím vagy title
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
			break;					// elsõ nemkisbetûs szóra kiugrik
	}
	if (i)
		from = i;
	if (!title.IsEmpty())
	{
		title.TrimRight();
		any->title = title;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
		// leszedi a sor végérõl az elõ keresztnév után következõ nemkeresztnévtõl kezdve: ez a posterior
		// közben gyûjti a keresztnevek nemét
		// az elsõ szót, ami biztos nem keresztnév, átugorja, hogy az esetles név azonosság egy keresztnévvel ne okozzon bajt
	for (i = from + 1; i < n; ++i)						// elsõ keresztnév keresése
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  elsõ keresztnévig pörög
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utolsó keresznév nemét fogadja el
			break;
		}
	}
	for (i += 1; i < n; ++i)								// utolsó keresztnév keresése
	{
		word = A[i];
		if ((sex = theApp.isFirstName(word)) != -1)		//  = elsõ nem keresztnévig pörög
		{
			vSex.push_back(sex);
			any->sex_id = sex;							// az utolsó keresznév nemét fogadja el
		}
		else
			break;
	}
	if (!vSex.size())
	{
		any->last_name = A[0];
		any->first_name = A[1];
		any->sex_id = -1;
		return;							// nincs keresztnév!!
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
	// full nam mácsak [titolo] család [család2] kereszt[kereszt2][kereszt3]

	n = splitCString(fullName, ' ', false, &A);
	if (n < 2) return;

	///////////////////////////////////////////////////////////////////////////////////////////////////////	// csak 2 szóból áll, akkor az család- és keresztnevek
	if (n == 2)  // ha csak két tagból áll, akkor az "családnév keresztnév"
	{
		any->last_name = A[0];
		any->first_name = A[1];
		return;
	}

	for (i = 0; i < n; ++i)
	{
		//		A[i].Remove('(');
		//		A[i].Remove(')');
		A[i].Replace(',', ' ');						// a név után lehet vesszõ !!
//		A[i].Replace('?', ' ');

		A[i].Trim();
	}
	i = 0;
	str = m_nameSubstr;
	// x és y ELÕNEVEK AZONOSÍTÁSA
	if (i + 1 < n && A[i + 1] == L"és")					// X és Y, ahol X nem i-re végzõdik
	{
		any->titolo = packWords(&A, i, 3);
		i += 3;
	}
	else if (((i + 2) < n) && A[i + 2] == L"és")		// Giczi, Assa és Ablánckürti Ghyczy Anna Mária
	{
		any->titolo = packWords(&A, i, 4);
		i += 4;
	}
	// Xi elõnevek azonosítása	
	else if (isLastCharacter(A[i], 'i'))					// i-re végzõdõ egytagú elõnév azonosítása 
	{
		str = m_nameSubstr;
		if ((sex = theApp.isFirstName(A[i + 1])) != -1)		// az ezt követõ szó keresznév, ami lehet családnév is
		{
			if (i + 2 < n)	// azaz már csak 3 tag van hátra, akkor ezek: titolo, lastname, firstname
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
			else if (i + 1 < n)							// már csak 2 szó van hátra, akkor azok: lasstname, firstname
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
	// kettõs vezetéknevek kezelése
	// i-n a család- és keresztenevek sorozata
	// a családnév is lehet kettõs és a kerezstnév is lehet kettõs, vagy többszörös
	str = m_nameSubstr;
	numOfFirstNames = vSex.size();
	for (int i = 0; i < vSex.size(); ++i)
	{
		if (vSex.at(i) != any->sex_id)
		{
			--numOfFirstNames;
		}
	}
	//	k = n - i;			// a szavak száma a családnév után nem jó!!
	first_name = packWords(&A, n - numOfFirstNames, numOfFirstNames);
	if (isLowerWord(A[i]))
	{
		if (isPeer(A[i]))
			any->peer = A[i];
		else
			any->posterior = A[i];
		++i;
	}
	k = n - i;			// a szavak száma a családnév után nem jó!!

	last_name = packWords(&A, i, k - numOfFirstNames);
	if (last_name.IsEmpty() && !first_name.IsEmpty())
	{
		last_name = getFirstWord(first_name);
		first_name = dropFirstWord(first_name);
	}

	any->first_name = first_name;
	any->last_name = last_name;


}