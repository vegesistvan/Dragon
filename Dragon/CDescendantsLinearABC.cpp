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

	CString title;
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
	str.Format(L"%d lesz�rmazott van a list�ban.", vDesc->size());
	printX(str);


	printX(L"");

	order_numOfD();

	// inicializ�l�s
	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).printed = false;
		vDesc->at(i).procChildren = 0;
	}

	Y = vDesc->at(vDesc->size() - 1).g;		// legmagasabb gener�ci� sz�m

	// felt�lti a vL vektort �res sorokkal
	vL.clear();
	DE::L l;
	for (int i = 0; i <= Y; ++i)
	{
		l.id = -1;
		l.pos = 0;
		l.l1.Empty();
		l.l2.Empty();
		vL.push_back(l);
	}

	int g;
	int id = 0;
	int idF;
	int idC;
	int dbC;
	m_cnt = 129;
	CString cntS;
	cntS.Format(L"%d", m_cnt);
//	CGetString dlg;
//	dlg.m_string = cntS;
//	if( !dlg.DoModal()) return;
//	m_cnt = _wtoi(dlg.m_string);
	
	long double x;
	int y;
	CString sex;
	while( true )
	{
		g = vDesc->at(id).g;
		putG(id, false);									// ki�rja id-t, aki az �s, vagy egy gyerek
		// a gyerek �sszes lesz�rmazottj�t ki�rja
		while (true)
		{
			id = vDesc->at(id).idC;
			if (id == 0) break;								// nincs t�bb lesz�rmazott
			putG(id, true);									// lesz�rmazottak ki�r�sa
		}
		for (g = Y; g > 0; --g)								// k�vetkez� kiirand� id �s g keres�se
		{
			id = vL.at(g).id;
			if (id > 0)										// lehetnek m�g �res gener�ci�k, ahol id == 0
			{
				idF = vDesc->at(id).idF;
				sex = vDesc->at(idF).sex;					// apa els� gyermek�nek indexe

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
		--m_cnt;
//		if (!m_cnt) break;
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
	if (cnt)
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
void CDescendantsLinearTable::fill_l1(int g, int lg, CString kar)
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

void CDescendantsLinearTable::putG(int id, bool descendant)
{
	int x = 0;
	if ( m_cnt == 2 )
	{
		x = 13;
		x = pow(x, 2);
	}
	int g = vDesc->at(id).g;
	int lg1 = longestLine(g);
	int lg = vL.at(g).l1.GetLength();
	int idL = vL.at(g).id;
	if (!descendant)					// horizont�lis terjeszked�s
	{
		if (g)
		{
			if (vDesc->at(id).idF == vDesc->at(idL).idF)		// testv�rek
			{
				if (!vDesc->at(id).numOfChildren)				// �j embernek nincs lesz�rmazottja
					goto cont;
				else											// �j embernek van lesz�rmazottja
				{

					if (lg1 >= lg)
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