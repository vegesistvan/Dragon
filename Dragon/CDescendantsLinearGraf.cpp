#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include <algorithm>
/*
bool sort_G_idF_chO(const DE::DESC& d1, const DE::DESC& d2)
{
	if (d1.g == d2.g)
	{
		if (d1.idF == d2.idF)
			return(d1.id < d1.id);
		else
		{
			return(d1.idF < d2.idF);
		}
	}
	else
		return(d1.g < d2.g);
}
*/
bool sort_G(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.g < d2.g);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnGraph()
{
	CString rowidF;
	int cntRepI;
	int cntRepJ;
	int j;
	int length;
	int g;

	order_numOfD();
	std::sort(vDesc->begin(), vDesc->end(), sort_G);

	std::vector<DE::DESC> vD;
	std::vector<DE::DESC> vD1;
	std::vector<DE::DESC> vD2;

	DE::DESC desc;
	DE::DESC desci;
	DE::DESC descj;

	for (int i = 0; i < vDesc->size(); ++i)
		vDesc->at(i).printed = false;


	str.Format(L"Lesz�rmazottak rendez�se a t�bl�hoz...");
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc->size());
	wndP.SetPos(0);

	vD1.clear();
	vDesc->at(0).printed = true;
	vD1.push_back(vDesc->at(0));
	vD = vD1;

	// vD1-k m�r ki vannak �rva, a gyerekeiken kell v�gigmenni, akiket vD2-be gy�jt�nk
	while (true)
	{
		vD2.clear();
		for (int i = 0; i < vD1.size(); ++i)
		{
			// gyerekeiket keress�k
			for (int k = 0; k < vDesc->size(); ++k)
			{
				if (!vDesc->at(k).printed)
				{
					if (vDesc->at(k).rowidF == vD1.at(i).rowid)
					{
						vDesc->at(k).printed = true;
						vD2.push_back(vDesc->at(k));
						vD.push_back(vDesc->at(k));
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
		if (!vD2.size()) break;
		vD1 = vD2;
	}
	wndP.DestroyWindow();
	vDesc->clear();
	*vDesc = vD;

	// id, idF, idC be�ll�t�sa
	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).id = i;
	}
	// ap�k azonos�t�ja
	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).idF = 1234567890;
		rowidF = vDesc->at(i).rowidF;
		cntRepI = vDesc->at(i).cntRep;
		g = vDesc->at(i).g;
		for (j = 0; j < i; ++j)
		{
			cntRepJ = vDesc->at(j).cntRep;
			if (cntRepJ && cntRepI)
			{
				if ((vDesc->at(j).rowid == rowidF) && (cntRepJ == cntRepI))
				{
					vDesc->at(i).idF = vDesc->at(j).id;
					break;
				}
			}
			else
			{
				if (vDesc->at(j).rowid == rowidF)
				{
					vDesc->at(i).idF = vDesc->at(j).id;
					break;
				}
			}
		}
		if (j == i)
		{
			for (j = 0; j < vDesc->size(); ++j)
			{
				if (vDesc->at(j).rowidF == rowidF)
				{
					vDesc->at(i).idF = vDesc->at(j).id;
					break;
				}
			}
		}
	}
	// gyerekek azonos�t�ja
	for (int i = 0; i < vDesc->size(); ++i)
	{
		for (j = 0; j < vDesc->size(); ++j)
		{
			if (vDesc->at(j).rowidF == vDesc->at(i).rowid && vDesc->at(j).g == vDesc->at(i).g + 1)
			{
				vDesc->at(i).idC = vDesc->at(j).id;
				break;
			}
		}
		if (j == vDesc->size())
		{
			for (j = 0; j < vDesc->size(); ++j)
			{
				if (vDesc->at(j).rowidF == vDesc->at(i).rowid && vDesc->at(j).g == vDesc->at(i).g + 1)
				{
					vDesc->at(i).idC = vDesc->at(j).id;
					break;
				}
			}
		}
	}

	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).printed = false;		// ki�rtuk m�r?
		vDesc->at(i).shift = 0;				// a bejegyz�s els� karakter�nek poz�ci�ja a sorban
	}

	Y = vDesc->at(vDesc->size() - 1).g;		// legmagasabb gener�ci� sz�m

	// a gener�ci�n bel�li sorsz�mok. m�g nem tudom. hogy mire j�
	int order = 1;
	for (g = 0; g <= Y; ++g)
	{
		order = 1;
		for (int i = 0; i < vDesc->size(); ++i)
		{
			if (vDesc->at(i).g == g)
			{
				vDesc->at(i).order = order;
				++order;
			}
		}
	}

	graph();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::graph()
{
	DE::DESC desc;
	CString title;
	int g;
	int id;


	title.Format(L"%s lesz�rmazotti gr�fja", m_name);
	m_txtFile.Format(L"%s\\%s_%s.txt", m_descendantsPath, title, getTimeTag());
	if (!openFileSpec(&txtOut, m_txtFile, L"w+")) return;
	printX(title);
	printX(L"");


	// vL vektor felt�lt�se a gener�ci� jel�vel
	DE::L l;
	for (int i = 0; i <= Y; ++i)
	{
		str.Format(L"%c ", get_gABC(i));
		l.id = -1;
		l.l1 = str;
		l.l2 = L"  ";
		l.l3 = L"  ";
		l.pos = 0;
		vL.push_back(l);
	}

	// Els� lesz�rmazotti l�nc insert�l�sa vL-be
	desc = vDesc->at(0);
	id = desc.id;
	vL.at(0).id = 0;
	vL.at(0).l1 += L"|";
	vL.at(0).l1 += getDesc(0);
	vL.at(0).l2 += L"|";
	vL.at(0).l3 += L"|";
	vDesc->at(0).printed = true;
	g = 1;
	for (int i = 1; i < vDesc->size(); ++i)
	{
		if (vDesc->at(i).g == g)   // a gener�ci� els� bejegyz�se
		{
			desc = vDesc->at(i);
			if (desc.idF == id)		// az el�z� gener�ci�ban az els� bejegyz�s az apa
			{
				vDesc->at(i).shift = 2;
				vDesc->at(i).printed = true;
				vL.at(g).id = i;
				vL.at(g).pos = vL.at(g).l1.GetLength();
				vL.at(g).l1 += L"|";
				vL.at(g).l1 += getDesc(i);
				vL.at(g).l2 += L"|";
				vL.at(g).l2 += m_birthdeath;
				vL.at(g).l3 += L"|";
				id = desc.id;
				++g;
			}
		}
	}
	// h�tulr�l keresem a testv�reket

	int idF;
	int cnt = 1;

	while (cnt)
	{
		// Egy gener�ci�n a testv�rek �s azok lesz�rmazottait �rja ki, am�g van
		for (g = vL.size() - 1; g >= 0; --g)
		{
			cnt = 0;
			id = vL.at(g).id;				// a gener�ci� el�z� (utols�) bejegyz�se
			if (id < 1) continue;			// nincs a gener�ci�ban lesz�rmazott ( a v�g�n el�fordulhat )
			idF = vDesc->at(id).idF;
			// testv�rek keres�se
			for (int i = id + 1; i < vDesc->size(); ++i)
			{
				if (vDesc->at(i).idF == idF && !vDesc->at(i).printed)  // testv�rek
				{
					sibling(i);
					++cnt;
					idF = vDesc->at(i).id;
					break;
				}
			}
			if (cnt)
			{
				// testv�r lesz�rmazottai
				for (int k = 0; k < vDesc->size(); ++k)
				{
					if (vDesc->at(k).idF == idF && !vDesc->at(k).printed)   // lesz�rmazottak
					{
						child(k);
						++cnt;
						idF = vDesc->at(k).id;
					}
				}
			}
			if (cnt)	// a gener�ci�ban nem volt �j bejegyz�s
				break;	// menj�nk a k�vetkez� gener�ci�ra
		}
	}

	for (int i = 0; i < vL.size(); ++i)
	{
		printX(vL.at(i).l1);
		printX(vL.at(i).l2);
		printX(vL.at(i).l3);
	}
	printX(L"\n\n");

	// a gr�fban nem szerepl� lesz�rmazottak
	cnt = 0;
	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (!vDesc->at(i).printed)
			++cnt;
	}
	if (cnt)
		printX(L"Valami baj van, mert az al�bbi bejegyz�sek kimaradtak a lesz�rmazotti gr�fb�l.");

	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (!vDesc->at(i).printed)
		{
			printX(getDesc(i));
		}
	}

	fclose(txtOut);
	theApp.notepad(m_txtFile, L"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::child(int id )
{

	int g = vDesc->at(id).g;				// gyerek gener�ci�ja
	int idF = vDesc->at(id).idF;			// gyerek apja
	int ln1 = vDesc->at(idF).shift;			// apja kezd� karakter sorsz�ma

	int ln2 = vL.at(g - 1).l2.GetLength();	// apa gener�ci�ja jelsor�nak hossza
	int ln = ln1 - ln2;
	fillL2(g - 1, ln-1, L" ");				// maga f�l�tt apj�val �sszek�ti
//	vL.at(g-1).l2 += L"|";

	ln2 = vL.at(g - 1).l3.GetLength();		// apa gener�ci�ja jelsor�nak hossza
	ln = ln1 - ln2;
	fillL3(g - 1, ln-1, L" ");				// apj�val �sszek�ti
	vL.at(g - 1).l3 += L"|";


	ln2 = vL.at(g).l1.GetLength();			// gyerek gener�ci�j�nak hossza
	ln = ln1 - ln2;
	fillL1(g, ln-1, L" ");
	vL.at(g).l1 += L"|";					// saj�t nevbe el� teszi

	ln2 = vL.at(g).l2.GetLength();			// gyerek gener�ci�j�nak hossza
	ln = ln1 - ln2;
	fillL2(g, ln - 1, L" ");
	vL.at(g).l2 += L"|";					// saj�t nevbe el� teszi


	vDesc->at(id).shift = vL.at(g).l1.GetLength();  // gyerek kezd� karakter sorsz�ma
	vL.at(g).l1 += getDesc(id);			// most m�r � az utols� bejegyz�s a sorban
	vL.at(g).l2 += m_birthdeath;
	vDesc->at(id).printed = true;
	vL.at(g).id = id;					// vL-be beteszi


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::sibling( int id)
{
	int g = vDesc->at(id).g;					// testv�r gener�ci�ja
	int idF = vDesc->at(id).idF;
	int ln11 = vL.at(g).l1.GetLength();			// testv�r sor hossza
	int ln12 = getLongestUnderG(g);				// g alatti gener�ci�k leghosszabb sora
	int ln1;
	int ln2;

	if (ln11 > ln12)  ln1 = 3;
	else
		ln1 = ln12 - ln11 + 3;

	int ln21 = vL.at(g).l2.GetLength();			// testv�r l2 sor hossza
	if (ln21 > ln12)  ln2 = 3;
	else
		ln2 = ln12 - ln21 + 3;

	if (!vDesc->at(id).numOfChildren)			
	{
		fillL1(g, ln1, L"-");					// elh�z�s ln-nel
		fillL2(g, ln2, L" ");
	}
	else
	{
		fillL1(g, ln1 - 1, L"-");				// ha lesz gyereke, akkor | tesz a n�v el�
		vL.at(g).l1 += L"|";					// saj�t neve el� tesz |

		fillL2(g, ln2 - 1, L" ");
		vL.at(g).l2 += L"|";					// sz�let�s el� tesz |
	}
	vDesc->at(id).shift = vL.at(g).l1.GetLength();
	vL.at(g).l1 += getDesc(id);					// most m�r � az utols� bejegyz�s a sorban
	vL.at(g).l2 += m_birthdeath;
	vDesc->at(id).printed = true;
	vL.at(g).id = id;							// vL-be beteszi
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::printX(CString str)
{
	fwprintf(txtOut, L"%s\n", str);
	fflush(txtOut);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getDesc(int i)
{
	DE::DESC desc = vDesc->at(i);
	str.Format(L"(%d %d %d %d %d)%s", desc.g, desc.idF, desc.id, desc.idC, desc.shift, desc.firstname);

	str.Format(L"%s", desc.firstname);

/*/
	if (!desc.birth.IsEmpty())
		str.Format(L"%s *%s", (CString)str, desc.birth);
	if( !desc.death.IsEmpty())
		str.Format(L"%s +%s", (CString)str, desc.death);
*/

	m_lastname = desc.lastname;
	m_birthdeath.Empty();
	m_lastname.Empty();
	if (!desc.birth.IsEmpty())
		m_birthdeath.Format(L"*%s", desc.birth);
	if (!desc.death.IsEmpty())
		m_birthdeath.Format(L"%s +%s", (CString)m_birthdeath, desc.death);
	m_birthdeath.TrimLeft();

	return(str);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fillL1(int g, int ln, CString kar)
{
	for (int i = 0; i < ln; ++i)
		vL.at(g).l1 += kar;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fillL2(int g, int len, CString kar)
{
	for (int j = 0; j < len; ++j)
		vL.at(g).l2 += kar;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fillL3(int g, int ln, CString kar)
{
	for (int i = 0; i < ln; ++i)
		vL.at(g).l3 += kar;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fillL4(int g, int ln, CString kar)
{
	for (int i = 0; i < ln; ++i)
		vL.at(g).l4 += kar;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A 'g' gener�ci�t�l a leghosszabb sor hossz�nak meghat�roz�sa, amik m�g az � lesz�rmazottai el�tt vannak
// g - az � gener�ci�ja
int CDescendantsLinearTable::getLongestUnderG(int g)
{
	int length;
	int lengthMax = 0;
	for( int i = g; i < vL.size(); ++i )
	{ 
		length = vL.at(i).l1.GetLength();
		if (length > lengthMax) lengthMax = length;
		length = vL.at(i).l2.GetLength();
		if (length > lengthMax) lengthMax = length;
		length = vL.at(i).l3.GetLength();
		if (length > lengthMax) lengthMax = length;

	}
	return lengthMax;

}