#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include <algorithm>
#include "GetString.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnAbcd()
{
	if (!vDesc->at(0).numOfChildren)
	{
		str.Format(L"%snek nincs lesz�rmazottottja!", m_name);
		AfxMessageBox(str);
		return;
	}

	DE::DESC desc;
	CString title;

	int g;
	int id;


	title.Format(L"%s lesz�rmazotti struktur�ja", m_name);
	m_txtFile.Format(L"%s\\%s_%s.txt", m_descendantsPath, title, getTimeTag());
	if (!openFileSpec(&txtOut, m_txtFile, L"w+")) return;
	printX(title);
	printX(L"");


	if (p_womenDescendants)
	{
		title = L"N�k lesz�rmazottai is a list�ban";
	}
	else
	{
		title = L"N�k lesz�rmazottai nincsenek a list�ban";
	}
	printX(title);
	printX(L"");


	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).printed = false;
		vDesc->at(i).procChildren = 0;
	}

	Y = vDesc->at(vDesc->size() - 1).g;		// legmagasabb gener�ci� sz�m

	DE::L l;
	for (int i = 0; i <= Y; ++i)
	{
		l.id = -1;
		l.pos = 0;
		l.l1.Empty();
		l.l2.Empty();
		vL.push_back(l);
	}
/*
	int cnt = 5;
	CString cntS;
	cntS.Format(L"%d", cnt);

	CGetString dlg;
	dlg.m_string = cntS;
	if (!dlg.DoModal()) return;
	cnt = _wtoi(dlg.m_string);
*/
	// Els� lesz�rmazotti l�nc insert�l�sa vL-be
	int ret;
	int ix;
	int idS;
	int idF;
	int idC;
	int dbC;
	id = 0;
	int lg;
	int G;
	int ID;

	id = 0;
	while( true )
	{
		g = vDesc->at(id).g; 
		putG(id, false );				// i�rja id-t, aki az �s, vagy egy gyerek id-ja
		// a gyerek �sszes lesz�rmazottj�t ki�rja
		while( true)
		{
			id = vDesc->at(id).idC;
			if (id == 0) break;
			putG(id, true );
		}
		for ( g = Y; g > 0; --g)
		{
			id = vL.at(g).id;
			if (id > 0)										// lehetnek m�g �res gener�ci�k
			{
				idF = vDesc->at(id).idF;
				idC = vDesc->at(idF).idC;					// apa els� gyermek�nek indexe
				if (idC > 0)
				{
					dbC = vDesc->at(idF).numOfChildren;		// apa gyermekeinek sz�ma
					for (id = idC; id < idC + dbC; ++id)
					{
						if (!vDesc->at(id).printed)
							break;							// tal�lt valakit
					}
					if (id < idC + dbC)
						break;								// tal�lt valakit
				}
			}
		}
		if (!g) break;				// nem tal�lt senkit
	}

	// ki�r�s
	for (int i = 0; i < vL.size(); ++i)
	{
		if (vL.at(i).id != -1)
		{
			printX(vL.at(i).l1);
			printX(vL.at(i).l2);
		}
	}

	int cnt = 0;
	
	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (!vDesc->at(i).printed)
			++cnt;
	}
	if( cnt )
		printX(L"\nFel nem haszn�lt bejegyz�sek\n");
	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (!vDesc->at(i).printed)
				printX(vDesc->at(i).name);
	}
	fclose(txtOut);
	theApp.notepad(m_txtFile, L"");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fill_l1( int g, int lg, CString kar)
{
	int length = vL.at(g).l1.GetLength();
	if (length < lg)
	{
		length = lg - length;
		for (int i = 0; i < length; ++i)
			vL.at(g).l1 += kar;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fill_l2(int g, int lg, CString kar)
{
	int length = vL.at(g).l2.GetLength();
	if (length < lg)
	{
		length = lg - length;
		for (int i = 0; i < length; ++i)
			vL.at(g).l2 += kar;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet: 
// descendant	true: lesz�rmazottat �runk ki akit f�gg�legesen �sszek�t az el�z� gener�ci�val
//				false: v�zszintesen terjeszked�k, testv�rt vagy nem testv�rt �runk ki
// 
// Ha a gener�ci� utols� tagja a testv�re, akkor vagy k�zvetlen mell� tessz�k,
// vagy eltoljuk �s "---"-al k�tj�k �ssze.
// 
// Ha  nem testv�rek, akkor eltoljuk az alatta l�v�k hossz�t�l f�gg�en.
// Ha testv�rek, �s egyik�j�knek nincsenel lesz�rmozottai, akkor k�zvetlen egym�s mell� �rjukl.
// Ha mindkett�j�knek vannak lesz�rmazottai, akkor az eltol�l�s az alatta l�v�k legnagyobb 
// hossza + 3, amiket "-"-el k�t�nk �ssze.

void CDescendantsLinearTable::putG( int id, bool descendant )
{
	int g = vDesc->at(id).g;
	int lg1 = longestLine(g);
	int lg;
	int idL = vL.at(g).id;
	if (!descendant)					// horizont�lis terjeszked�s
	{
		if (g)
		{
			if (vDesc->at(id).idF == vDesc->at(idL).idF)		// testv�rek
			{
				if (!vDesc->at(id).numOfChildren) 
					goto cont;
				else
				{
					if (lg1)
					{
						fill_l1(g, lg1, L"-");
						vL.at(g).l1 += L"---";
					}
				}
			}
			else									// nem testv�rek
			{
				fill_l1(g, lg1, L" ");
			}
		}
	}
	else										// lesz�rmazott
	{
		if (g)
		{
			lg = vL.at(g - 1).l1.GetLength();		// az el�tte l�v� hossza
			fill_l2(g - 1, lg - 1, L" ");			// ugyanennyi legyen a seg�d sor hossza
			vL.at(g - 1).l2 += L"|";				// �sszek�tk�k a f�l�tte l�v�vel
			fill_l1(g, lg - 1, L" ");				// saj�t gener�ci�j�t is eltolja ugyanennyivel
		}
	}
cont:vL.at(g).l1 += get_gABC(g); 
	vL.at(g).id = id;
	vDesc->at(id).printed = true;

}
///////////////////////////////////////////////////////////////////////////////////////////////////
int CDescendantsLinearTable::longestLine(int g)
{
	int ln;
	int lnMax = 0;
	for (int i = g + 1; i <= Y; ++i)
	{
		ln = vL.at(i).l1.GetLength();
		if (ln > lnMax) lnMax = ln;
	}
	return lnMax;
}