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
		str.Format(L"%snek nincs leszármazottottja!", m_name);
		AfxMessageBox(str);
		return;
	}

	DE::DESC desc;
	CString title;

	int g;
	int id;


	title.Format(L"%s leszármazotti strukturája", m_name);
	m_txtFile.Format(L"%s\\%s_%s.txt", m_descendantsPath, title, getTimeTag());
	if (!openFileSpec(&txtOut, m_txtFile, L"w+")) return;
	printX(title);
	printX(L"");


	if (p_womenDescendants)
	{
		title = L"Nõk leszármazottai is a listában";
	}
	else
	{
		title = L"Nõk leszármazottai nincsenek a listában";
	}
	printX(title);
	printX(L"");


	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).printed = false;
		vDesc->at(i).procChildren = 0;
	}

	Y = vDesc->at(vDesc->size() - 1).g;		// legmagasabb generáció szám

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
	// Elsõ leszármazotti lánc insertálása vL-be
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
		putG(id, false );				// iírja id-t, aki az õs, vagy egy gyerek id-ja
		// a gyerek összes leszármazottját kiírja
		while( true)
		{
			id = vDesc->at(id).idC;
			if (id == 0) break;
			putG(id, true );
		}
		for ( g = Y; g > 0; --g)
		{
			id = vL.at(g).id;
			if (id > 0)										// lehetnek még üres generációk
			{
				idF = vDesc->at(id).idF;
				idC = vDesc->at(idF).idC;					// apa elsõ gyermekének indexe
				if (idC > 0)
				{
					dbC = vDesc->at(idF).numOfChildren;		// apa gyermekeinek száma
					for (id = idC; id < idC + dbC; ++id)
					{
						if (!vDesc->at(id).printed)
							break;							// talált valakit
					}
					if (id < idC + dbC)
						break;								// talált valakit
				}
			}
		}
		if (!g) break;				// nem talált senkit
	}

	// kiírás
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
		printX(L"\nFel nem használt bejegyzések\n");
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
// descendant	true: leszármazottat írunk ki akit függülegesen összeköt az elõzõ generációval
//				false: vízszintesen terjeszkedük, testvért vagy nem testvért írunk ki
// 
// Ha a generáció utolsó tagja a testvére, akkor vagy közvetlen mellé tesszük,
// vagy eltoljuk és "---"-al kötjök össze.
// 
// Ha  nem testvérek, akkor eltoljuk az alatta lévõk hosszától függõen.
// Ha testvérek, és egyiköjüknek nincsenel leszármozottai, akkor közvetlen egymás mellé írjukl.
// Ha mindkettõjüknek vannak leszármazottai, akkor az eltolálás az alatta lévõk legnagyobb 
// hossza + 3, amiket "-"-el kötünk össze.

void CDescendantsLinearTable::putG( int id, bool descendant )
{
	int g = vDesc->at(id).g;
	int lg1 = longestLine(g);
	int lg;
	int idL = vL.at(g).id;
	if (!descendant)					// horizontális terjeszkedés
	{
		if (g)
		{
			if (vDesc->at(id).idF == vDesc->at(idL).idF)		// testvérek
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
			else									// nem testvérek
			{
				fill_l1(g, lg1, L" ");
			}
		}
	}
	else										// leszármazott
	{
		if (g)
		{
			lg = vL.at(g - 1).l1.GetLength();		// az elõtte lévõ hossza
			fill_l2(g - 1, lg - 1, L" ");			// ugyanennyi legyen a segéd sor hossza
			vL.at(g - 1).l2 += L"|";				// összekötkük a fölötte lévõvel
			fill_l1(g, lg - 1, L" ");				// saját generációját is eltolja ugyanennyivel
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