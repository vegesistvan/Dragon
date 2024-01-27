// descendantsNew.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "old_descendants.h"
#include "old_descendantsParam.h"
#include "Table_people_columns.h"
#include "Table_tables.h"
#include "Table_people.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByOrder(const DN::DESC& v1, const DN::DESC& v2)
{
	if (v1.order == v2.order)
		return(v1.original < v2.original);
	return(v1.order < v2.order );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByOrderDesc(const DN::LEN& v1, const DN::LEN& v2)
{
	return(v1.order > v2.order);
}
bool sortByOrderAsc(const DN::LEN& v1, const DN::LEN& v2)
{
	return(v1.order < v2.order);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDescendantsOld, CWnd)
CDescendantsOld::CDescendantsOld()
{
	m_descendantsPath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_descendantsPath, 0))
		_wmkdir(m_descendantsPath);

	m_log = false;			// nyomtassa-e a vektorokat?
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsOld::~CDescendantsOld()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsOld, CWnd)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDescendantsNew()
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		theApp.message(L"Kijel�lt �s�k list�ja", L"\nNincs kijel�lve senki!");
		return;
	}

	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;
	CString tableNumber;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);
		m_command.Format(L"SELECT rowid FROM people WHERE father_id ='%s' OR mother_id='%s'", rowid, rowid);
		if (!query(m_command)) return;
		if (!m_recordset->RecordsCount())
		{
			str.Format(L"%s rowid bejegyz�snek nincs lesz�rmoazottja", rowid);
			AfxMessageBox(str);
			return;
		}

		theApp.v_rowid.push_back(rowid);
		if (theApp.m_inputMode == GAHTML)
		{
			tableNumber = m_ListCtrl.GetItemText(nItem, G_TABLENUMBER);
			theApp.v_tableNumbers.push_back(tableNumber);
		}
		else
			theApp.v_tableNumbers.push_back( L"1" );
	}
	CDescendantsOld desc;
	desc.start();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsOld::start()
{
	if (!parameters()) return false;

	m_startTime = theApp.getStartTime();

	CString file;
	CString title;
	vDesc.clear();
	
	m_listedP = 0;
	m_listedD = 0;



	for (int i = 0; i < theApp.v_rowid.size(); ++i)
	{
		m_rowid = theApp.v_rowid.at(i);

		if (theApp.m_inputMode == GEDCOM)
			m_tablenumber = L"1";
		else
		{
			m_command.Format(L"SELECT tablenumber FROM people WHERE rowid = '%s'", m_rowid);
			if (!query(m_command)) return false;
			m_tablenumber = rs.GetFieldString(0);
		}

		// Az �s berak�sa a vDesc vektorba
		DN::DESC desc;
		queryR(m_rowid);
/*
		if (peoS.numOfChildren == 0)
		{
			str.Format(L"%s-nek nincsenek gyerekei!", peoS.name);
			//AfxMessageBox(str, MB_ICONEXCLAMATION);
			continue;
		}
*/
		if (!i || !p_oneOutputFile || p_print)
		{
			if (theApp.v_rowid.size() > 1 && p_oneOutputFile)
				file.Format(L"%d ember lesz�rmazotti list�ja", theApp.v_rowid.size());
			else
				file.Format(L"%s lesz�rmazotti list�ja", peoS.name);
			title = file;

			if (!openHtml(file, title, p_colorBgrnd)) return false;
		}

		m_tablenumber = peoS.tablenumber;

		desc.g = 0;
		desc.rowid = m_rowid;
		desc.rowidF.Empty();
		desc.name = peoS.name;
		desc.sex = peoS.sex;
		desc.numOfSpouses = peoS.numOfSpouses;
		desc.numOfChildren = peoS.numOfChildren;
		desc.motherIndex = 1;
		desc.procChildren = 0;
		desc.childorder = 1;
		desc.parentSex = peoS.sex;
		desc.parentIndex = 0;
		desc.status = 0;
		desc.hidden = false;
		desc.cntRep = 0;
		desc.order = 0;
		desc.original = 0;
		desc.fatherIndex = -1;  // nincs apja
		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		if (p_print )
		{
			descendantsOld();
			if (theApp.v_rowid.size() > 1)
			{
				for (int i = 0; i < m_indent; ++i)
					fwprintf(fl, L"%s", m_tag2);
				fwprintf(fl, L"<br><br><br>");
			}
		}
		else
		{
			descendants();
			numOfDesc();
			multipleRowid();
			printVector(i);
			linearTable(i, 1);
			linearTable(i, 2);
		}


		if (i == theApp.v_rowid.size() - 1 || !p_oneOutputFile)
		{
			closeHtml();
		}

	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsOld::tablesDescendants()  // list�zand� t�bl�k a theapp.v_tableNumbers vektorban
{
	CString file;
	CString title;

	CString familyName;
	CString tableHeader;
	CString tableRoman;
	CString father_id;
	CString rowid;
	m_listedP = 0;
	m_listedD = 0;


	if (!parameters()) return false;

	m_startTime = theApp.getStartTime();

	DN::DESC desc;

	vDesc.clear();

	if (p_oneOutputFile)
	{
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(0));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableHeader = rs.GetFieldString(1);
		tableRoman = rs.GetFieldString(2);
		title.Format(L"%s %s t�bla", familyName, tableRoman);
		title.Trim();

		if (theApp.v_tableNumbers.size() > 1)
			file.Format(L"%s �s tov�bbi %d t�bla", title, theApp.v_tableNumbers.size() - 1);
		else
			file.Format(L"%s_t�bla", title);
		openHtml(file, title, p_colorBgrnd);
	}

	CProgressWnd wndProgress(NULL, L"Lesz�rmazotti t�bl�k k�sz�t�se folyik....");
	wndProgress.GoModal();
	wndProgress.SetRange(0, theApp.v_tableNumbers.size());
	wndProgress.SetPos(0);

	for (UINT i = 0; i < theApp.v_tableNumbers.size(); ++i)
	{
		m_tablenumber = theApp.v_tableNumbers.at(i);

		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(i));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableHeader = rs.GetFieldString(1);
		tableRoman = rs.GetFieldString(2);

		if (!p_oneOutputFile )
		{
			title.Format(L"%s %s t�bla", familyName, tableRoman);
			title.Trim();
			title.Format(L"%s t�bla lesz�rmazotti list�ja", (CString)title);

			file.Format(L"%s_t�bla", title);
			openHtml(file, title, p_colorBgrnd);
		}
		else
		{
			if (!i && theApp.v_tableNumbers.size() > 1)
			{
				title.Format(L"%d t�bla lesz�rmazotti list�ja", theApp.v_tableNumbers.size());
				file = title;
				openHtml(file, title, p_colorBgrnd);
			}
		}

		m_command.Format(L"SELECT rowid, last_name, sex_id, father_id, peer FROM people WHERE tableNumber = '%s' AND tableAncestry == 1 ", m_tablenumber);
		if (!query(m_command)) return false;

		if (!rs.RecordsCount())
		{
			str.Format(L"%s t�bl�ban nincs �s!!", m_tablenumber);
			AfxMessageBox(str);
			continue;
//			return false;
		}
		rowid = rs.GetFieldString(0);
		father_id = rs.GetFieldString(3);

		if (father_id.IsEmpty() || !father_id.Compare(L"0"))  // ha nincs apa, akkor mag�t az �st teszi be a vDesc-be
		{
			queryR(rowid);
			desc.rowid = rowid;     //m_rowid1;
			desc.hidden = false;
		}
		else		// ha van apja az �snek, akkor az ap�t hidden-k�nt elteszi, hogy ha t�bb gyereke van, akkor mind list�zza 
		{
			queryR(father_id);
			desc.rowid = father_id;
			desc.hidden = true;
		}
		desc.g = 0;
		desc.rowidF = father_id;
		desc.sex = peoS.sex;
		desc.numOfChildren = peoS.numOfChildren;
		desc.childorder = 1;
		desc.procChildren = 0;
		desc.numOfSpouses = 0;
		desc.parentIndex = 0;
		desc.motherIndex = 1;
		desc.name = peoS.name;
		desc.cntRep = 0;
		desc.order = 0;
		desc.fatherIndex = -1;  // ez nem biztos!!!!!!!!

		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		if (p_oneOutputFile && theApp.v_tableNumbers.size() > 1)
		{
			str.Format(L"<b>%s</b>\n", tableHeader);
			print(str);
		}

		descendants();
		multipleRowid();
		printVector(i);
		linearTable(i, 1);
		linearTable(i, 2);
		if (!p_oneOutputFile)
		{
			closeHtml();
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	theApp.v_tableNumbers.clear();
	if (p_oneOutputFile)
		closeHtml();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::descendants()
{
	CString rowid;
	int i;
	int j;
	int numOfChildren;
	int procChildren;
	int z = 0;
	int cnt = 0;
	int maxGen = _wtoi(p_generationMax) - 2;
	int fatherIndex;
	m_actGen = 0;
	m_gMax = 0;
	m_givup = 0;
	m_canceled = false;
	m_cntRepeated = 0;
	DN::DESC desc;

	m_listedP = 0;
	m_listedD = 0;
	m_maxDesc = 2000000;
	if (maxGen == -2) maxGen = 0;

	str.Format(L"%s lesz�rmazottainak �sszegy�jt�se folyik...", m_os);
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

//	m_folyt_db = 0;
	while (true)
	{
		if (cnt == 10000)	cnt = 0;
		wndP.SetPos(cnt);
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			m_canceled = true;
			break;
		}
		++cnt;

		if (maxGen && m_actGen > maxGen)
		{
			m_actGen = -100;		// jelzi, hogy a list�ra �rja ki, hogy a gener�ci�k magas sz�ma miatt szakdt meg a list�z�s
			break;
		}

		// visszamegy az utols� emberre, akinek m�g van ki fel nem dolgozott gyereke
		for ( i = vDesc.size() - 1; i >= 0;  --i)
		{
			numOfChildren = vDesc.at(i).numOfChildren;
			if (numOfChildren)
			{
				procChildren = vDesc.at(i).procChildren;
				if (numOfChildren != procChildren)	// m�g van fel nem dolgozott gyerek
					break;	
			}
		}
		if (i < 0) 	break;	// v�ge van a programnak, nincs t�bb fel nem dolgozott gyerek!! Kil�p a ciklusb�l


		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		// sz�l� index: i;
		str = m_tablenumber;
 		rowid = getNextChildRowid( i );
		m_actGen = vDesc.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;
		queryR(rowid);		// lek�rdezi a gyereket �s be�ll�tja m_sex_id-t �s m_numOfSpouses-t
		
		m_tablenumber = peoS.tablenumber;

		if (rowid == L"308824")
			z = 1;
		desc.rowidF = vDesc.at(i).rowid;
		desc.rowid = rowid;
		desc.name = peoS.name;
		desc.sex = peoS.sex;
		desc.numOfSpouses = peoS.numOfSpouses;
		desc.numOfChildren = peoS.numOfChildren;
		desc.motherIndex = peoS.motherIndex;
		desc.procChildren = 0;
		desc.order = 0;
		desc.original = vDesc.size();
			
		desc.parentIndex = i;
		desc.parentSex = vDesc.at(i).sex;
		desc.g = vDesc.at(i).g + 1;		// apja gener�ci�j�n�l 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;
		
		vDesc.at(i).procChildren += 1;		// a most feldolgozott testv�reinek sz�ma n� egyet
		desc.childorder = vDesc.at(i).procChildren;
		
		desc.hidden = false;
		desc.cntRep = 0;
		if (desc.sex == WOMAN && !p_womenDescendants)		// ha n� a lesz�rmazott �s annak a gyerekeit nem akarjuk list�zni
			desc.numOfChildren = 0;

		if (p_repeated == 1)						// ism�tl�d� lesz�rmazottak kihagy�sa
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)		// szerepel-e a m�r meglal�ltak k�z�tt a rowid?
					break;
			}
			if (j == vDesc.size())					// csak akkor teszi el, ha nem tal�lta ism�tl�d�nek
				vDesc.push_back(desc);				// 
			else
				++m_cntRepeated;
		}
		else
			vDesc.push_back(desc);
	
		if (vDesc.size() == m_maxDesc )
		{
			str.Format(L"%d lesz�rmazottn�l feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vDesc.size();
			break;
		}
	}
	wndP.DestroyWindow();

	if( p_childrenOrder == ORDER_INCREASING || p_childrenOrder == ORDER_DECREASING ) //  lesz�rmazotti sz�l n�vekv� vagy cs�kken� sorrendj�t k�rj�k
		order_vDesc();
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::print_vDesc()
{
	CString fileSpec;
	fileSpec = theApp.openTextFile(&text, L"vDesc", L"w+");

	for (int i = 0; i < vDesc.size(); ++i)
	{
		fwprintf(text, L"%4d. %6s %6s %5d %-40s\n", i, vDesc.at(i).rowid, vDesc.at(i).rowidF, vDesc.at(i).nOfD, vDesc.at(i).name );
	}
	fclose(text);
	theApp.showFile(fileSpec);
}

/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::descendants()
{
	CString rowid;
	int i;
	int j;
	int numOfChildren;
	int procChildren;
	int z = 0;
	int cnt = 0;
	int maxGen = _wtoi(p_generationMax) - 2;
	m_actGen = 0;
	m_gMax = 0;
	m_givup = 0;
	m_canceled = false;
	m_cntRepeated = 0;
	DN::DESC desc;

	m_listedP = 0;
	m_listedD = 0;
	m_maxDesc = 2000000;
	if (maxGen == -2) maxGen = 0;
	//	CString fileSpec;
	//	fileSpec = theApp.openTextFile(&fl, L"vDesc", L"w+");


	str.Format(L"%s lesz�rmazottainak �sszegy�jt�se folyik...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

//	m_folyt_db = 0;
	while (true)
	{
		if (cnt == 10000)	cnt = 0;
		wndP.SetPos(cnt);
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			m_canceled = true;
			break;
		}
		++cnt;

		if (maxGen && m_actGen > maxGen)
		{
			m_actGen = -100;		// jelzi, hogy a list�ra �rja ki, hogy a gener�ci�k magas sz�ma miatt szakdt meg a list�z�s
			break;
		}


		// A vektor utols� bejegyz�s�t�l vagy a visszatekert sz�l�t�l indul
		//numOfChildren = vDesc.at(i).numOfChildren;
		//procChildren = vDesc.at(i).procChildren;
		//visszamegy az utols� emberre, akinek m�g van ki fel nem dolgozott gyereke
		for (i = vDesc.size() - 1; i >= 0; --i)
		{

			if (vDesc.at(i).rowid == L"26103")
				z = 1;

			//			if (vDesc.at(i).hidden) continue;   // mert csak gy�j

			if (vDesc.at(i).sex == WOMAN && !p_womenDescendants)		// ha n� a lesz�rmazott �s annak a gyerekeit nem akarjuk list�zni
			{
				vDesc.at(i).numOfChildren = 0;
				vDesc.at(i).procChildren = 0;
			}

			numOfChildren = vDesc.at(i).numOfChildren;
			procChildren = vDesc.at(i).procChildren;
			if (numOfChildren && numOfChildren != procChildren)
				break;

		}
		if (i < 0)
			break;	// v�ge van a programnak, nincs t�bb fel nem dolgozott gyerek!! Kil�p a ciklusb�l
		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		// sz�l� index: i;
		str = m_tablenumber;
		rowid = getNextChildRowid(i);
		if (rowid.IsEmpty()) continue;  // nincs gyereke
		m_actGen = vDesc.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;
		queryR(rowid);		// lek�rdezi a gyereket �s be�ll�tja m_sex_id-t �s m_numOfSpouses-t

		m_tablenumber = peoS.tablenumber;

		if (rowid == L"308824")
			z = 1;

		desc.rowid = rowid;
		desc.name = peoS.name;
		desc.sex = peoS.sex;
		desc.numOfSpouses = peoS.numOfSpouses;
		desc.numOfChildren = peoS.numOfChildren;
		desc.motherIndex = peoS.motherIndex;
		desc.procChildren = 0;
		desc.order = 0;
		desc.original = vDesc.size();

		desc.parentIndex = i;
		desc.parentSex = vDesc.at(i).sex;
		desc.g = vDesc.at(i).g + 1;		// apja gener�ci�j�n�l 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;

		vDesc.at(i).procChildren += 1;		// a most feldolgozott testv�reinek sz�ma n� egyet
		desc.childorder = vDesc.at(i).procChildren;

		desc.hidden = false;
		desc.cntRep = 0;

		if (p_repeated == 1)						// ism�tl�d� lesz�rmazottak kihagy�sa
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)		// szerepel-e a m�r meglal�ltak k�z�tt a rowid?
					break;
			}
			if (j == vDesc.size())					// csak akkor teszi el, ha nem tal�lta ism�tl�d�nek
				vDesc.push_back(desc);				// 
			else
				++m_cntRepeated;
		}
		else
			vDesc.push_back(desc);

		if (vDesc.size() == m_maxDesc)
		{
			str.Format(L"%d lesz�rmazottn�l feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vDesc.size();
			break;
		}

		i = vDesc.size() - 1;
		//		fwprintf(fl, L"%20s %8s %2d %1d\n", vDesc.at(i).name, vDesc.at(i).rowid, vDesc.at(i).numOfChildren, vDesc.at(i).sex);
		//		fflush(fl);
		if (rowid == L"349494")
			z = 1;
	}
	wndP.DestroyWindow();
//	i = m_folyt_db;
	if (p_childrenOrder == ORDER_INCREASING || p_childrenOrder == ORDER_DECREASING ) //  lesz�rmazotti sz�l n�vekv� vagy cs�kken� sorrendj�t k�rj�k
		order_vDesc();
	//	fclose(fl);
	//	theApp.showFile(fileSpec);
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::descendantsOld()
{
	DN::DESC desc;
	CString rowid;
	int generationsMax = 55;
	m_indent = 0;
	m_genPrev = 0;
	m_gen = 0;
	m_genPeer = 0;

	vRowid.clear();
	vSerial.clear();
	vSerial.push_back(1);
/*
	if (p_numbering == SZLUHA) // orderd list
	{
		m_tag1 = L"<ol>\n";
		m_tag2 = L"</ol>\n";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>\n";
	}
*/
	CProgressWnd wndP(NULL, L"Lesz�rmazotti t�bl�k k�sz�t�se folyik....");
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

	int cnt = 0;
	// az els� ember a treePeople, treeTables-ben ker�l bet�lt�sre 
	while (true)
	{
		if (cnt == 10000)	cnt = 0;
		wndP.SetPos(cnt);
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			m_canceled = true;
			break;
		}
		++cnt;

		if (!vDesc.at(m_gen).hidden)
		{
			printGAline();	// a vDesc tetej�n l�v� lesz�rmazottat kinyomtatjuk  m_genPrev-et be�ll�tja
		}


		if (vDesc.at(m_gen).sex == WOMAN && !p_womenDescendants)		// ha n� a lesz�rmazott �s annak a gyerekeit nem akarjuk list�zni
		{
			vDesc.at(m_gen).numOfChildren = 0;
			vDesc.at(m_gen).procChildren = 0;
		}

		if (vDesc.at(m_gen).numOfChildren == vDesc.at(m_gen).procChildren)	// m�r minden gyerek�t ki�rta
		{
			//visszamegy az el�z� gener�ci�ra, amelyiknak m�g van ki nem �rt gyereke
			while (m_gen >= 0 && vDesc.at(m_gen).numOfChildren == vDesc.at(m_gen).procChildren)
			{
				vDesc.pop_back();		// visszamegy az el�z� gener�ci�ra, azaz az apj�ra vagy anyj�ra
				--m_gen;
			}
		}
		if (m_gen < 0) break;			// nincs tov�bb!!!

		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		rowid = getNextChildRowid(vDesc.size() - 1);
		if (!rowid.IsEmpty())			// van m�g gyerek
		{
			queryR(rowid);		// lek�rdezi a gyereket �s be�ll�tja m_sex_id-t �s m_numOfSpouses-t
			desc.rowid = rowid;
			desc.sex = peoS.sex;
			desc.numOfSpouses = peoS.numOfSpouses;				// a gyerek apja feles?geinek sz?m?t meg?rzi 
			desc.numOfChildren = getNumberOfChildren(rowid, peoS.sex);
			desc.procChildren = 0;
			desc.motherIndex = peoS.motherIndex;
			desc.hidden = false;
			desc.parentIndex = vDesc.size() - 1;			// sz�l�je indexe
			desc.g = vDesc.at(vDesc.size() - 1).g + 1;		// gener�ci�ja az apja geer�ci�ja + 1
			desc.order = 0;

			vDesc.at(m_gen).procChildren += 1;							// a most kinyomtatand� testv�rek sz�ma n� egyet

			vDesc.push_back(desc);
			++m_gen;

			// Tupigny-h�z el��ll�tja a gener�ci�n bel�l egyes�vel n�vekv� sorsz�mokat
			if (m_gen < vSerial.size()) ++vSerial.at(m_gen);			// m�r l�tez� gener�ci�, 1-el n�veli a sorsz�mot	
			else	vSerial.push_back(1);								// �j gener�ci�: sorsz�m 1-el kezd�dik 
		}
	}
	wndP.DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::listDescendants()
{
	CString fileSpec;
	fileSpec = theApp.openTextFile(&fl, L"vDesc", L"w+");
	for (int i = 0; i < vDesc.size(); ++i)
	{
		fwprintf(fl, L"%20s %8s %2d %1d\n", vDesc.at(i).name, vDesc.at(i).rowid, vDesc.at(i).numOfChildren, vDesc.at(i).sex);
		fflush(fl);
	}
	fclose(fl);
	theApp.showFile(fileSpec);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsOld::queryR(CString rowid)
{
	int z;
	
	m_command.Format(L"SELECT generation, sex_id, posterior, last_name, first_name, tableNumber, folyt, parentIndex, numOfChildren  FROM people WHERE rowid ='%s'", rowid);
	if (!queryP(m_command)) return false;

	CString folyt;
	folyt = rsP.GetFieldString(6);
//	if ( !folyt.IsEmpty())
//		++m_folyt_db;

	peoS.tablenumber = rsP.GetFieldString(5);
	peoS.sex = rsP.GetFieldString(1);
	if (peoS.sex == BISEX)
		peoS.sex = MAN;
	peoS.posterior = rsP.GetFieldString(2);
	
	CString lastname = rsP.GetFieldString(3);
	if (!lastname.IsEmpty() && !iswupper(lastname.GetAt(0)))
		lastname.Empty();
	lastname.Replace('|', ' ');
	lastname.Replace('/', ' ');
	lastname.Replace('?', ' ');

	CString firstname = rsP.GetFieldString(4);
	peoS.tablenumber = rsP.GetFieldString(5);
	firstname.Replace('/', ' ');
	firstname.Replace('(', ' ');
	firstname.Replace(')', ' ');
	firstname.Replace('*', ' ');
	firstname.Replace('|', ' ');
	firstname.Replace('"', ' ');
	firstname.Replace('?', ' ');
	firstname.Trim();
	peoS.firstname = firstname;
	if (firstname == L"Em�lia")
		z = 1;
	peoS.name.Format(L"%s %s", lastname, firstname);
	peoS.name.Trim();
	peoS.numOfChildren = getNumberOfChildren(rowid, peoS.sex);

	peoS.motherIndex = _wtoi(rsP.GetFieldString(7));
	/*
	if (peoS.sex == MAN)
		m_command.Format(L"SELECT count() FROM people WHERE father_id = '%s'", rowid);
	else
		m_command.Format(L"SELECT count() FROM people WHERE mother_id = '%s'", rowid);
	if (!query(m_command)) return false;

	peoS.numOfChildren = _wtoi(rs.GetFieldString(0));
	*/
	if (peoS.sex == MAN)
		m_command.Format(L"SELECT count() FROM marriages WHERE husband_id = '%s'", rowid);
	else
		m_command.Format(L"SELECT count() FROM marriages WHERE wife_id = '%s'", rowid);
	if (!query(m_command)) return false;
	peoS.numOfSpouses = _wtoi(rs.GetFieldString(0));
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDescendantsOld::getNumberOfChildren(CString rowid, CString sex_id)
{
	if (sex_id == MAN)
	{
		if (p_connect)
			m_command.Format(L"SELECT count() FROM people WHERE father_id='%s'", rowid);
		else
			m_command.Format(L"SELECT count() FROM people WHERE father_id='%s' AND tableNumber='%s'", rowid, m_tablenumber);
	}
	else
	{
		if (p_connect)
			m_command.Format(L"SELECT count() FROM people WHERE mother_id='%s'", rowid);
		else
			m_command.Format(L"SELECT count() FROM people WHERE mother_id='%s' AND tableNumber='%s'", rowid, m_tablenumber);
	}
	if (!query(m_command)) return 0;

	return(_wtoi(rs.GetFieldString(0)));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsOld::getNextChildRowid( UINT i )
{
	CString parent_id = vDesc.at(i).rowid;
	UINT	ixChild = vDesc.at(i).procChildren;	// lesz�rmazott ixChild-adik gyerek�t keresi  
	CString	sex = vDesc.at(i).sex;
	int z;
	if (parent_id == L"308346")
		z = 0;
	if (sex == MAN )
	{
		if (p_connect)	// ha �sszekapcsol�s van, akkor t�bl�t�l f�ggetlen�l keres
		{
			if (p_childrenOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY linenumber", parent_id);
		}
		else				// ha nincs �sszekapcsol�s, akkor csak t�bl�n bel�l keres
		{
			if (p_childrenOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
		}
	}
	else
	{
		if (p_connect)
		{
			if( p_childrenOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY linenumber", parent_id);
		}
		else
		{
			if (p_childrenOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
		}
	}
	if (!query(m_command)) return L"";
	if (rs.RecordsCount() < ixChild)
	{
		str.Format(L"%s sz�l�nek csak %d gyereke van, te pedig a %d.-kat k�red!!", parent_id, rs.RecordsCount(), ixChild);
		AfxMessageBox(str);
		return L"";
	}

	rs.MoveTo(ixChild);
	return(rs.GetFieldString(0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsOld::parameters()
{
	CDescendantsParamOld dlg;
	if (dlg.DoModal() == IDCANCEL) return false;

	p_generationMax = dlg.p_generationMax;
	p_fontSize = dlg.p_fontSize;
	p_rowWidth = dlg.p_rowWidth;

	p_connect = dlg.p_connect;
	p_womenDescendants = dlg.p_womenDescendants;
	p_numberingSystem = dlg.p_numberingSystem;
	p_descendantName = dlg.p_descendantName;

	p_descendantAttrib = dlg.p_descendantAttrib;
	p_otherNameAttrib = dlg.p_otherNameAttrib;
	p_commentAttrib = dlg.p_commentAttrib;
	p_specAttrib = dlg.p_comboSpect;

	p_capitalName = dlg.p_capital;
	p_bold = dlg.p_bold;
	p_folyt = dlg.p_folyt;
	p_colorBgrnd = dlg.p_colorBgrnd;
	p_repeated = dlg.p_repeated;
	p_repeatedColor = dlg.p_repeatedColor;
	p_oneOutputFile = dlg.p_oneOutputFile;
	p_print = false;
	p_checkCRLF = dlg.p_checkCRLF;
	p_childrenOrder = dlg.p_childrenOrder;
	
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::multipleRowid()
{
	CString rowid;
	int g;
	int cnt;
	// az ism�tl�d�ket megjel�li:  az els� el�fordul�st 1-el, a k�vetkez�ket -1-el
	for (int i = 0; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).status == 0)
		{
			rowid = vDesc.at(i).rowid;
			cnt = 1;
			for (int j = i + 1; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)
				{
					vDesc.at(i).status = 1;		// els� el�fordul�s
					vDesc.at(j).status = 2;		// tov�bbi el�fordul�s

					vDesc.at(i).cntRep = 1;
					++cnt;
					vDesc.at(j).cntRep = cnt;
				}
			}
		}
	}

	if (p_repeated == 2)   // apa lesz�rmazottait tartja meg
	{
		for (int i = 0; i < vDesc.size();)
		{
			if (vDesc.at(i).status == 0)  // ha nem ism�tl�d�, akkor elfogadja list�zand�nak
			{
				for (i; i < vDesc.size() && vDesc.at(i).status == 0; ++i);
			}
			else if (vDesc.at(i).status == 1)		// ism�tl�d� els� el�fordul�sa, aki list�zand�
			{
				if (vDesc.at(i).parentSex == WOMAN)	// ha n�, akkor nem list�zand� lesz
				{
					for (i; i < vDesc.size() && vDesc.at(i).status == 1; ++i)
						vDesc.at(i).status = 2;
				}
				else							// ha f�rfi, akkor elfogadja list�zand�nak
					for (i; i < vDesc.size() && vDesc.at(i).status == 1; ++i);
			}
			else if (vDesc.at(i).status == 2 ) // ism�tl�d� m�sodik el�fordul�sa, aki kihagy�sra van kijel�lve
			{
				if (vDesc.at(i).parentSex == MAN)	// ha f�rfi, akkor list�zand� lesz
				{
					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i)
						vDesc.at(i).status = 1;
				}
//				else							// h an�, akkor elfogadja kihagyand�nak
//					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i);
			}
			
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i-t�l kezdve minden lesz�rmazottat ( saj�t �s magasabb gener�ci�kat) megtart
// j-t�l kezdve minden lesz�rmazottat ( saj�t �s magasabb gener�ci�kat) kihagy
void CDescendantsOld::signeD( int i, int j )
{
	int g = vDesc.at(i).g;
	for (int k = i; k < vDesc.size(); ++k)
	{
		if (vDesc.at(k).g < g)
			break;
		else
			vDesc.at(k).status = 1;              // list�zand�
	}
	// anya minden lesz�rmazottj�t kihagy�sra jelzi
	g = vDesc.at(j).g;
	for (int k = j; k < vDesc.size(); ++k)
	{
		if (vDesc.at(k).g < g)
			break;
		else
			vDesc.at(k).status = -1;			// nem kell list�zni
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsOld::query(CString command)
{
	if (rs.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, rs.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsOld::query2(CString command)
{
	if (rs2.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, rs2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsOld::queryP(CString command)
{
	if (rsP.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, rsP.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsOld::queryM(CString command)
{
	if (rsM.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, rsM.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirand� lesz�rmazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::printGAline()
{
	//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid;
	CString desc;
	CString spouses;
	rowid = vDesc.at(m_gen).rowid;
	queryPeople(rowid, &p);

	m_printed = rowidExist();					// ism�tl�d� lesz�rmazottak kisz�r�se
	if (m_printed && p_repeated)
		return;

	// a lesz�rmazatti sor el�tt ki�rja a megv�ltozott csal�dnevet: % name
	if (m_lastnamePrev != p.last_name && !p_capitalName )
	{
		str.Format(L"%c %s\n", '%', p.last_name);
		print(str);
	}
	m_lastnamePrev = p.last_name;
	m_sexidPrev = p.sex_id;

	printBegining( vDesc.size()-1);	// html k�dok �s gener�ci� elk�sz�t�se; 
//	printDescendant(vDesc.size() - 1 );
	desc = createDescendant(vDesc.size() - 1, true );
//	print(str);
	spouses = createSpouses(vDesc.size()-1);
//	print(str);
	str.Format(L"%s %s", desc, spouses);
	print(str);
	if (!p.arm.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s csal�d]</font>", p.arm);
		str.Format(L" <font color='blue'>[%s]</font>", p.arm);
		print(str);
	}
	if (!p.csalad.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s csal�d]</font>", p.csalad);
		str.Format(L" <font color='blue'>[%s]</font>", p.csalad);
		print(str);
	}

	if (!p.folyt.IsEmpty() && p_folyt)
	{
		str.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		print(str);
	}
	fwprintf(fl, L"\n");
	fflush(fl);
	m_genPrev = m_gen;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vRowid vektorba gy�jti a list�zott lesz�rmazottak rowid-jeit.
// El�tte megn�zi, hogy szerepel-e m�r benne. Ha igen true, ha nem false a visszat�r�s
bool CDescendantsOld::rowidExist()
{
	int i;
	for (i = 0; i < vRowid.size(); ++i)
	{
		if (vRowid.at(i) == p.rowid)
			return true;
	}
	vRowid.push_back(p.rowid);
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::printvLen()
{
	if (!m_log) return;
	DN::LEN len;
	fwprintf(orderTxt, L"%2s %7s %7s %7s %7s %2s\n", L"g", L"multi", L"ixFirst", L"ixLast", L"order", L"gBack");
	for (int i = 0; i < vLenOu->size(); ++i)
	{
		len = vLenOu->at(i);
		fwprintf(orderTxt, L"%2d %7d %7d %7d %7d %2d\n", len.g, len.multiplyer, len.ixFirst, len.ixLast, len.order, len.gBack);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A lesz�rmazottkat tartalmaz� vGKR vektort szakaszokra bontjuk, �s minden szakaszhoz egy sorrendet rendel�nk.
// A sorrendet visszavezetj�k a vGKR-be, k�v�ns�g szerint rendezz�k, majd ki�rjuk.
// A szakaszokat inter�ci�s m�dszerrel �ll�tjuk el�.
// Gener�ci�nk�t bontjuk szakaszokra a vGKR-t az al�bbi bejegyz�sekkel:
//
// int g;			// vizsg�lt gener�ci�
// int multiplyer;	// gener�ci�hoz rendelt szorz�
// int ixFirst;		// intervallum vGKR beli els� indexe
// int ixLast;		// intervallum vGKR beli utols� indexe
// int order;		// intervallum hossza * multiplyer         
// int gBack;		// az intervallumban t�rt�nt visszaugrott gener�ci�. Ha nincs akkor 0 
//
// A legkisebbt�l a legnagyobb gener�ci�ig ciklusban v�gezz�k az intervallumok felbont�s�t.
// Egy kor�bbi cikusban l�trehozott intervallumot elfogadunk, ha nincs benne gener�ci�s visszaugr�s ( gBack != 0 )
// Ha van, akkor azt a szakaszt tov�bb bontjuk a k�vetkez� gener�ci� szerint, hasonl� m�don, csak egy felbontott al-intervallum
//  "order"  �rt�ke az anya-intervallum "order" �rt�k�hez ad�dik hozz�

void CDescendantsOld::order_vDesc()
{
	int i;
	int j;
	int k;

	// vLen vektor elemei
	int g;			// vizsg�lt gener�ci�
	int multiplyer;	// gener�ci�hoz rendelt szorz�
	int ixFirst;	// intervallum vGKR beli els� indexe
	int ixLast;		// intervallum vGKR beli utols� indexe
	int order;		// intervallum hossza * multiplyer         
	int gBack;		// az intervallumban t�rt�nt visszaugrott gener�ci�. Ha nincs akkor 0 
	
	int maxLen;

	std::vector< DN::LEN > vLen1;		// a parent-intervallumokat tartalmaz� vektor (bemenet)
	std::vector< DN::LEN > vLen2;		// a gyerek-intervallumokat tartalmaz� vektor (kimenet)

	std::vector<DN::LEN>* vLenIn;
	std::vector<DN::LEN>* vLenTemp;		// vLenI  �s vLenOu v�lt�s�hoz �tmeneti c�m

	

	if (m_log)
	{
		m_orderFileTitle = L"order";
		m_orderPathName;
		m_orderPathName.Format(L"%s\\%s_%s.txt", m_descendantsPath, m_orderFileTitle, getTimeTag());

		if (!openFileSpec(&orderTxt, m_orderPathName, L"w+")) return;
	}

	// A legnagyobb gener�ci� be�ll�t�sa �s 
	// a gener�ci�k sz�m�t�l f�gg� szorz� be�ll�t�sa �gy, hogy 

	int gMax = 1;
	multiplyer = 1000;	// a vizsg�lt gener�cira vonatkoz� szorz�
	for (int i = 0; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).g > gMax)
		{
			gMax = vDesc.at(i).g;
//			multiplyer *= 10;
		}
	}



	// a ciklushoz felhaszn�lt v�ltoz�k:
	// g - vizsg�lt gener�ci�
	
	// A vLenOu vectorba kigy�jti a lesz�rmazotti szakaszokat 
	DN::LEN in;			// aktu�lis bemeneti intervallum
	DN::LEN ou;			// felbont�s ut�ni kimeneti intervallum
	vLenIn = &vLen1;
	vLenOu = &vLen2;

	bool found;

	int gP;			// parent intervallum gener�ci�ja
	int gAct;
	int gPrev = 0;
	vLenOu->clear();
	// B lesz�rmazotti l�nc rendezend� intervallumainak meghat�roz�sa
	j = 0;
	for (i = j+1; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).g == 1)
		{
			// intrevallum kezdete
			ou.g = 1;
			ou.multiplyer = multiplyer;
			ou.ixFirst = i;
			ou.gBack = 0;

			for ( j = i + 1; j < vDesc.size(); ++j)
			{
				gAct = vDesc.at(j).g;
				// az intervellumon bel�l el�fordul� gener�ci�s visszaugr�s
				if (gAct >1 && gAct < gPrev) // visszaugr�s
					ou.gBack = gAct;

				if ( gAct == 1)
				{
					ou.ixLast = j-1;
					ou.order = (j - i)*multiplyer;
					vLenOu->push_back(ou);
					break;
				}
				gPrev = gAct;
			}
		}
	}
	ou.ixLast = vDesc.size()-1;
	ou.order = ou.ixLast - ou.ixFirst + 1;
	vLenOu->push_back(ou);

	printvLen();
/*
	int gNext = ou.g;
	bool bontas = true;
	while (bontas)
	{
		// ciklusonk�nt felcser�li a k�t vLen vektort, hogy a bemenet a kor�bbi ciklus kimente legyen
		vLenTemp = vLenIn;
		vLenIn = vLenOu;
		vLenOu = vLenTemp;
		vLenOu->clear();
		bontas = false;
		multiplyer *= 2;
		++gNext;
		// minden intervallumon v�gigmegy
		for (i = 0; i < vLenIn->size(); ++i)
		{
			in = vLenIn->at(i);
			if (in.gBack == 0)  // azokat az intervallumokat, amelyekben nincs gener�ci�s visszaugr�s, �tteszi a kimenetbe
			{
				vLenOu->push_back( in );
			}
			else  // az intervallumban gener�ci�s visszaugr�s van, fel kell bontani child-intervallumokra  
			{
				// az intervallum els� tagj�t ki�rjuk, a marad�kot vizsg�ljuk
				ou = in;
				ou.ixLast = ou.ixFirst;
				ou.gBack = 0;
				ou.multiplyer = multiplyer;
				vLenOu->push_back(ou);
				
				for (int j = in.ixFirst +1; j <= in.ixLast; ++j)
				{
					gPrev = ou.g;
					// A k�vetkez� gener�ci�t vizsg�ljuk					
					
					if (vDesc.at(j).g == gNext )  // ou.g els� el�fordul�sa, 
					{
						//  a felbontott uj intervallum kezdete
						ou.g = gNext;
						ou.ixFirst = j;
						ou.gBack = 0;
						gPrev = gNext;
						// keress�k ou.g k�vekez� el�fordul�s�t, ami az intervallum v�ge lesz
						for ( k = j + 1; k <= in.ixLast; ++k)
						{
							gAct = vDesc.at(k).g;
							// van az intervellumon bel�l gener�ci�s visszaugr�s ?
							if (gAct < gPrev) // visszaugr�s
							{
								ou.gBack = gAct;
								bontas = true;
							}

							if (gAct == gNext )  // ou.g k�vetkez� el�fordul�sa
							{
								// az intervallum v�ge
								ou.ixLast = k - 1;
								vLenOu->push_back(ou);
								break;
							}
							gPrev = gAct;
						}
						if (k > in.ixLast)
						{
							ou.ixLast = k - 1;
							vLenOu->push_back(ou);
							break;
						}

					}

//					if (vDesc.at(j).g == in.gBack)
//					// ebben nincs ism�tl�d�s, et a szakaszt ki�rjuk, 
//					{
//						ou.ixFirst = in.ixFirst;
//						ou.ixLast = j-1;
//						ou.gBack = 0;
//						vLenOu->push_back(ou);
//					}
				}
			}
		}
		printvLen();
//		break;
	}
*/	
	

	// A vLen vektort k�v�ns�g szerint rendezi
	if (p_childrenOrder == ORDER_INCREASING)
	{
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderAsc);
	}
	else
	{
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderDesc);
	}
	printvLen();

	// A vDesc vektorba �tvezeti a sorrendet, majd ennek megfelel�en rendezi azt
	for (int i = 0; i < vLenOu->size(); ++i)
	{
		for (int j = vLenOu->at(i).ixFirst; j <= vLenOu->at(i).ixLast; ++j)
		{
			vDesc.at(j).order = i + 1;
		}
	}
	std::sort(vDesc.begin()+1, vDesc.end(), sortByOrder);

	if (m_log)
	{
		fclose(orderTxt);
		theApp.showFile(m_orderPathName);
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsOld::numOfDesc()
{
	CString rowidF;
	m_iPrev = 0;
	int g;
	int j = 0;
	int i1;
	int i2;
	int bottom = vDesc.size();


	// alaphelyzet
	for (int i = 0; i < vDesc.size(); ++i)
		vDesc.at(i).nOfD = 0;

	// lesz�rmazottak sz�m�nak (nOfD) sz�mol�sa minden bejegyz�shez
	vDesc.at(0).nOfD = vDesc.size() - 1;
	for (int i = 0; i < vDesc.size(); ++i)
	{
		rowidF = vDesc.at(i).rowid;
		g = vDesc.at(i).g;
		vSib.clear();
		for (j = i + 1; j < vDesc.size(); ++j)
		{
			if (vDesc.at(j).g <= g)
			{
				bottom = j;
				break;
			}
			// gyerekek indexe
			if (vDesc.at(j).rowidF == rowidF)
			{
				vSib.push_back(j);
			}
		}
		if (j == vDesc.size())
			bottom = vDesc.size();
		if (vSib.size())
		{
			for (j = 0; j < vSib.size() - 1; ++j)
			{
				vDesc.at(vSib.at(j)).nOfD = vSib.at(j + 1) - vSib.at(j) - 1;
			}
			vDesc.at(vSib.at(j)).nOfD = bottom - vSib.at(j) - 1;
		}

	}
}