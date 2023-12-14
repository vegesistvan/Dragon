// descendantsNew.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "descendants.h"
#include "descendantsParam.h"
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
IMPLEMENT_DYNAMIC(CDescendants, CWnd)
CDescendants::CDescendants()
{
	m_descendantsPath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_descendantsPath, 0))
		_wmkdir(m_descendantsPath);

	m_log = false;			// nyomtassa-e a vektorokat?
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendants::~CDescendants()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendants, CWnd)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDescendantsNew()
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		theApp.message(L"Kijelölt õsök listája", L"\nNincs kijelölve senki!");
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
		theApp.v_rowid.push_back(rowid);
		if (theApp.m_inputMode == GAHTML)
		{
			tableNumber = m_ListCtrl.GetItemText(nItem, G_TABLENUMBER);
			theApp.v_tableNumbers.push_back(tableNumber);
		}
		else
			theApp.v_tableNumbers.push_back( L"1" );
	}
	CDescendants desc;
	desc.start();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::start()
{
	if (!parameters()) return false;

	m_startTime = theApp.getStartTime();

	CString file;
	CString title;
	vDesc.clear();
	


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

		// Az õs berakása a vDesc vektorba
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
		if (!i || !p_oneFile || p_print)
		{
			if (theApp.v_rowid.size() > 1 && p_oneFile)
				file.Format(L"%d ember leszármazotti listája", theApp.v_rowid.size());
			else
				file.Format(L"%s leszármazotti listája", peoS.name);
			title = file;
			if (p_html)
				openHtml(file, title, p_colorBgrnd);
			else
				openTxt(file, title);
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
		desc.length = 0;
		desc.fatherIndex = -1;  // nincs apja
		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		if (p_print &&  p_html)
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
			multipleRowid();
			if (p_html)
			{
				printVector(i);
				linearTable(i);

			}
			else
				printVectorTxt(i);
		}


		if (i == theApp.v_rowid.size() - 1 || !p_oneFile)
		{
			if (p_html)
				closeHtml();
			else
				closeTxt();
		}

	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::tablesDescendants()  // listázandó táblák a theapp.v_tableNumbers vektorban
{
	CString file;
	CString title;

	CString familyName;
	CString tableHeader;
	CString tableRoman;
	CString father_id;
	CString rowid;

	if (!parameters()) return false;

	m_startTime = theApp.getStartTime();

	DN::DESC desc;

	vDesc.clear();

	if (p_oneFile)
	{
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(0));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableHeader = rs.GetFieldString(1);
		tableRoman = rs.GetFieldString(2);
		title.Format(L"%s %s tábla", familyName, tableRoman);
		title.Trim();

		if (theApp.v_tableNumbers.size() > 1)
			file.Format(L"%s és további %d tábla", title, theApp.v_tableNumbers.size() - 1);
		else
			file.Format(L"%s_tábla", title);
		openHtml(file, title, p_colorBgrnd);
	}

	CProgressWnd wndProgress(NULL, L"Leszármazotti táblák készítése folyik....");
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

		if (!p_oneFile )
		{
			title.Format(L"%s %s tábla", familyName, tableRoman);
			title.Trim();
			title.Format(L"%s tábla leszármazotti listája", (CString)title);

			file.Format(L"%s_tábla", title);
			openHtml(file, title, p_colorBgrnd);
		}
		else
		{
			if (!i && theApp.v_tableNumbers.size() > 1)
			{
				title.Format(L"%d tábla leszármazotti listája", theApp.v_tableNumbers.size());
				file = title;
				openHtml(file, title, p_colorBgrnd);
			}
		}

		m_command.Format(L"SELECT rowid, last_name, sex_id, father_id, peer FROM people WHERE tableNumber = '%s' AND tableAncestry == 1 ", m_tablenumber);
		if (!query(m_command)) return false;

		if (!rs.RecordsCount())
		{
			str.Format(L"%s táblában nincs õs!!", m_tablenumber);
			AfxMessageBox(str);
			continue;
//			return false;
		}
		rowid = rs.GetFieldString(0);
		father_id = rs.GetFieldString(3);

		if (father_id.IsEmpty() || !father_id.Compare(L"0"))  // ha nincs apa, akkor magát az õst teszi be a vDesc-be
		{
			queryR(rowid);
			desc.rowid = rowid;     //m_rowid1;
			desc.hidden = false;
		}
		else		// ha van apja az õsnek, akkor az apõt hidden-ként elteszi, hogy ha több gyereke van, akkor mind listázza 
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
		desc.length = 0;
		desc.fatherIndex = -1;  // ez nem biztos!!!!!!!!

		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		if (p_oneFile && theApp.v_tableNumbers.size() > 1)
		{
			str.Format(L"<b>%s</b>\n", tableHeader);
			print(str);
		}

		descendants();
		multipleRowid();
		printVector(i);
		linearTable(i);
		if (!p_oneFile)
		{
			closeHtml();
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	theApp.v_tableNumbers.clear();
	if (p_oneFile)
		closeHtml();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::descendants()
{
	CString rowid;
	int i;
	int j;
	int numOfChildren;
	int procChildren;
	int z = 0;
	int cnt = 0;
	int maxGen = _wtoi(m_editGenMax) - 2;
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

	str.Format(L"%s leszármazottainak összegyûjtése folyik...", m_os);
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
			m_actGen = -100;		// jelzi, hogy a listára írja ki, hogy a generációk magas száma miatt szakdt meg a listázás
			break;
		}

		// visszamegy az utolsó emberre, akinek még van ki fel nem dolgozott gyereke
		for ( i = vDesc.size() - 1; i >= 0;  --i)
		{
			numOfChildren = vDesc.at(i).numOfChildren;
			if (numOfChildren)
			{
				procChildren = vDesc.at(i).procChildren;
				if (numOfChildren != procChildren)	// még van fel nem dolgozott gyerek
					break;	
			}
		}
		if (i < 0) 	break;	// vége van a programnak, nincs több fel nem dolgozott gyerek!! Kilép a ciklusból


		// a kinyomtatott ember következõ, míg ki nem nyomtatott gyerekét keresi
		// szülõ index: i;
		str = m_tablenumber;
 		rowid = getNextChildRowid( i );
		m_actGen = vDesc.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;
		queryR(rowid);		// lekérdezi a gyereket és beállítja m_sex_id-t és m_numOfSpouses-t
		
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
		desc.g = vDesc.at(i).g + 1;		// apja generációjánál 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;
		desc.length = 0;
		
		vDesc.at(i).procChildren += 1;		// a most feldolgozott testvéreinek száma nõ egyet
		desc.childorder = vDesc.at(i).procChildren;
		
		desc.hidden = false;
		desc.cntRep = 0;
		if (desc.sex == WOMAN && !p_mother)		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
			desc.numOfChildren = 0;

		if (p_repeated == 1)						// ismétlõdõ leszármazottak kihagyása
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)		// szerepel-e a már meglaláltak között a rowid?
					break;
			}
			if (j == vDesc.size())					// csak akkor teszi el, ha nem találta ismétlõdõnek
				vDesc.push_back(desc);				// 
			else
				++m_cntRepeated;
		}
		else
			vDesc.push_back(desc);
	
		if (vDesc.size() == m_maxDesc )
		{
			str.Format(L"%d leszármazottnál feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vDesc.size();
			break;
		}
	}
	wndP.DestroyWindow();
	
	CString rowidF;
	for (int i = vDesc.size() - 1; i >= 0; --i)
	{
		rowidF = vDesc.at(i).rowidF;
		for (int j = 0; j < vDesc.size(); ++j)
		{
			if (vDesc.at(j).rowid == rowidF && vDesc.at(j).length == 0 )  // megvan az apa
			{
				vDesc.at(j).length = i - j + 1;
			}
		}
	}
//	print_vDesc();


//	i = m_folyt_db;
	if( p_radioOrder == ORDER_INCREASING || p_radioOrder == ORDER_DECREASING ) //  leszármazotti szál növekvõ vagy csökkenõ sorrendjét kérjük
		order_vDesc();
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::print_vDesc()
{
	CString fileSpec;
	fileSpec = theApp.openTextFile(&text, L"vDesc", L"w+");

	for (int i = 0; i < vDesc.size(); ++i)
	{
		fwprintf(text, L"%4d. %6s %6s %5d %-40s\n", i, vDesc.at(i).rowid, vDesc.at(i).rowidF, vDesc.at(i).length, vDesc.at(i).name );
	}
	fclose(text);
	theApp.showFile(fileSpec);
}

/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::descendants()
{
	CString rowid;
	int i;
	int j;
	int numOfChildren;
	int procChildren;
	int z = 0;
	int cnt = 0;
	int maxGen = _wtoi(m_editGenMax) - 2;
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


	str.Format(L"%s leszármazottainak összegyûjtése folyik...", m_os);
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
			m_actGen = -100;		// jelzi, hogy a listára írja ki, hogy a generációk magas száma miatt szakdt meg a listázás
			break;
		}


		// A vektor utolsó bejegyzésétõl vagy a visszatekert szülõtöl indul
		//numOfChildren = vDesc.at(i).numOfChildren;
		//procChildren = vDesc.at(i).procChildren;
		//visszamegy az utolsó emberre, akinek még van ki fel nem dolgozott gyereke
		for (i = vDesc.size() - 1; i >= 0; --i)
		{

			if (vDesc.at(i).rowid == L"26103")
				z = 1;

			//			if (vDesc.at(i).hidden) continue;   // mert csak gyûj

			if (vDesc.at(i).sex == WOMAN && !p_mother)		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
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
			break;	// vége van a programnak, nincs több fel nem dolgozott gyerek!! Kilép a ciklusból
		// a kinyomtatott ember következõ, míg ki nem nyomtatott gyerekét keresi
		// szülõ index: i;
		str = m_tablenumber;
		rowid = getNextChildRowid(i);
		if (rowid.IsEmpty()) continue;  // nincs gyereke
		m_actGen = vDesc.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;
		queryR(rowid);		// lekérdezi a gyereket és beállítja m_sex_id-t és m_numOfSpouses-t

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
		desc.g = vDesc.at(i).g + 1;		// apja generációjánál 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;

		vDesc.at(i).procChildren += 1;		// a most feldolgozott testvéreinek száma nõ egyet
		desc.childorder = vDesc.at(i).procChildren;

		desc.hidden = false;
		desc.cntRep = 0;

		if (p_repeated == 1)						// ismétlõdõ leszármazottak kihagyása
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)		// szerepel-e a már meglaláltak között a rowid?
					break;
			}
			if (j == vDesc.size())					// csak akkor teszi el, ha nem találta ismétlõdõnek
				vDesc.push_back(desc);				// 
			else
				++m_cntRepeated;
		}
		else
			vDesc.push_back(desc);

		if (vDesc.size() == m_maxDesc)
		{
			str.Format(L"%d leszármazottnál feladom!", m_maxDesc);
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
	if (p_radioOrder == ORDER_INCREASING || p_radioOrder == ORDER_DECREASING ) //  leszármazotti szál növekvõ vagy csökkenõ sorrendjét kérjük
		order_vDesc();
	//	fclose(fl);
	//	theApp.showFile(fileSpec);
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::descendantsOld()
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
	CProgressWnd wndP(NULL, L"Leszármazotti táblák készítése folyik....");
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

	int cnt = 0;
	// az elsö ember a treePeople, treeTables-ben kerül betöltésre 
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
			printGAline();	// a vDesc tetején lévõ leszármazottat kinyomtatjuk  m_genPrev-et beállítja
		}


		if (vDesc.at(m_gen).sex == WOMAN && !p_mother)		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
		{
			vDesc.at(m_gen).numOfChildren = 0;
			vDesc.at(m_gen).procChildren = 0;
		}

		if (vDesc.at(m_gen).numOfChildren == vDesc.at(m_gen).procChildren)	// már minden gyerekét kiírta
		{
			//visszamegy az elõzõ generációra, amelyiknak még van ki nem írt gyereke
			while (m_gen >= 0 && vDesc.at(m_gen).numOfChildren == vDesc.at(m_gen).procChildren)
			{
				vDesc.pop_back();		// visszamegy az elõzõ generációra, azaz az apjára vagy anyjára
				--m_gen;
			}
		}
		if (m_gen < 0) break;			// nincs tovább!!!

		// a kinyomtatott ember következõ, míg ki nem nyomtatott gyerekét keresi
		rowid = getNextChildRowid(vDesc.size() - 1);
		if (!rowid.IsEmpty())			// van még gyerek
		{
			queryR(rowid);		// lekérdezi a gyereket és beállítja m_sex_id-t és m_numOfSpouses-t
			desc.rowid = rowid;
			desc.sex = peoS.sex;
			desc.numOfSpouses = peoS.numOfSpouses;				// a gyerek apja feles?geinek sz?m?t meg?rzi 
			desc.numOfChildren = getNumberOfChildren(rowid, peoS.sex);
			desc.procChildren = 0;
			desc.motherIndex = peoS.motherIndex;
			desc.hidden = false;
			desc.parentIndex = vDesc.size() - 1;			// szülõje indexe
			desc.g = vDesc.at(vDesc.size() - 1).g + 1;		// generéciója az apja geerációja + 1
			desc.order = 0;

			vDesc.at(m_gen).procChildren += 1;							// a most kinyomtatandó testvérek száma nõ egyet

			vDesc.push_back(desc);
			++m_gen;

			// Tupigny-höz elõállítja a generáción belül egyesével növekvõ sorszámokat
			if (m_gen < vSerial.size()) ++vSerial.at(m_gen);			// már létezõ generáció, 1-el növeli a sorszámot	
			else	vSerial.push_back(1);								// új generáció: sorszám 1-el kezdõdik 
		}
	}
	wndP.DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::listDescendants()
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
bool CDescendants::queryR(CString rowid)
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
	if (firstname == L"Emília")
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
int CDescendants::getNumberOfChildren(CString rowid, CString sex_id)
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
CString CDescendants::getNextChildRowid( UINT i )
{
	CString parent_id = vDesc.at(i).rowid;
	UINT	ixChild = vDesc.at(i).procChildren;	// leszármazott ixChild-adik gyerekét keresi  
	CString	sex = vDesc.at(i).sex;
	int z;
	if (parent_id == L"308346")
		z = 0;
	if (sex == MAN )
	{
		if (p_connect)	// ha õsszekapcsolás van, akkor táblától függetlenül keres
		{
			if (p_radioOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY linenumber", parent_id);
		}
		else				// ha nincs összekapcsolás, akkor csak táblán belül keres
		{
			if (p_radioOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
		}
	}
	else
	{
		if (p_connect)
		{
			if( p_radioOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY perentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY linenumber", parent_id);
		}
		else
		{
			if (p_radioOrder == ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
		}
	}
	if (!query(m_command)) return L"";
	if (rs.RecordsCount() < ixChild)
	{
		str.Format(L"%s szülõnek csak %d gyereke van, te pedig a %d.-kat kéred!!", parent_id, rs.RecordsCount(), ixChild);
		AfxMessageBox(str);
		return L"";
	}

	rs.MoveTo(ixChild);
	return(rs.GetFieldString(0));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::parameters()
{
	CDescendantsParam dlg;
	if (dlg.DoModal() == IDCANCEL) return false;

	m_editGenMax = dlg.m_editGenMax;
	m_comboFontSize = dlg.m_comboFontSize;
	m_editWidth = dlg.m_editWidth;

	p_connect = dlg.m_checkConnect;
	p_mother = dlg.m_checkMother;
	p_numbering = dlg.m_radioNumbering;
	p_lastname = dlg.m_radioDNameX;

	p_descStyle = dlg.m_comboDAttrib;
	p_otherNameStyle = dlg.m_comboOtherName;
	p_commentStyle = dlg.m_comboComment;
	p_specStyle = dlg.m_comboSpec;
	p_capitalName = dlg.m_checkCapital;
	p_checkBold = dlg.m_checkBold;
	p_folyt = dlg.m_checkFolyt;
	p_colorBgrnd = dlg.m_colorBgrnd;
	p_repeated = dlg.m_repeated;
	p_repeatedColor = dlg.m_repeatedColor;
	p_oneFile = dlg.m_oneFile;
	p_print = false;
	p_checkCRLF = dlg.m_checkCRLF;
	p_radioOrder = dlg.m_radioOrder;

	p_html = !dlg.m_html;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::multipleRowid()
{
	CString rowid;
	int g;
	int cnt;
	// az ismétlõdõket megjelöli:  az elsõ elõfordulást 1-el, a következõket -1-el
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
					vDesc.at(i).status = 1;		// elsõ elõfordulás
					vDesc.at(j).status = 2;		// további elõfordulás

					vDesc.at(i).cntRep = 1;
					++cnt;
					vDesc.at(j).cntRep = cnt;
				}
			}
		}
	}

	if (p_repeated == 2)   // apa leszármazottait tartja meg
	{
		for (int i = 0; i < vDesc.size();)
		{
			if (vDesc.at(i).status == 0)  // ha nem ismétlõdõ, akkor elfogadja listázandónak
			{
				for (i; i < vDesc.size() && vDesc.at(i).status == 0; ++i);
			}
			else if (vDesc.at(i).status == 1)		// ismétlõdõ elsõ elõfordulása, aki listázandó
			{
				if (vDesc.at(i).parentSex == WOMAN)	// ha nõ, akkor nem listázandó lesz
				{
					for (i; i < vDesc.size() && vDesc.at(i).status == 1; ++i)
						vDesc.at(i).status = 2;
				}
				else							// ha férfi, akkor elfogadja listázandónak
					for (i; i < vDesc.size() && vDesc.at(i).status == 1; ++i);
			}
			else if (vDesc.at(i).status == 2 ) // ismétlõdõ második elõfordulása, aki kihagyásra van kijelölve
			{
				if (vDesc.at(i).parentSex == MAN)	// ha férfi, akkor listázandó lesz
				{
					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i)
						vDesc.at(i).status = 1;
				}
//				else							// h anõ, akkor elfogadja kihagyandónak
//					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i);
			}
			
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i-tõl kezdve minden leszármazottat ( saját és magasabb generációkat) megtart
// j-tõl kezdve minden leszármazottat ( saját és magasabb generációkat) kihagy
void CDescendants::signeD( int i, int j )
{
	int g = vDesc.at(i).g;
	for (int k = i; k < vDesc.size(); ++k)
	{
		if (vDesc.at(k).g < g)
			break;
		else
			vDesc.at(k).status = 1;              // listázandó
	}
	// anya minden leszármazottját kihagyásra jelzi
	g = vDesc.at(j).g;
	for (int k = j; k < vDesc.size(); ++k)
	{
		if (vDesc.at(k).g < g)
			break;
		else
			vDesc.at(k).status = -1;			// nem kell listázni
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::query(CString command)
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
BOOL CDescendants::queryP(CString command)
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
BOOL CDescendants::queryM(CString command)
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
// ix a kiirandó leszármazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printGAline()
{
	//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid;
	CString desc;
	CString spouses;
	rowid = vDesc.at(m_gen).rowid;
	queryPeople(rowid, &p);

	m_printed = rowidExist();					// ismétlõdõ leszármazottak kiszûrése
	if (m_printed && p_repeated)
		return;

	// a leszármazatti sor elõtt kiírja a megváltozott családnevet: % name
	if (m_lastnamePrev != p.last_name && !p_capitalName )
	{
		str.Format(L"%c %s\n", '%', p.last_name);
		print(str);
	}
	m_lastnamePrev = p.last_name;
	m_sexidPrev = p.sex_id;

	printBegining( vDesc.size()-1);	// html kódok és generáció elkészítése; 
//	printDescendant(vDesc.size() - 1 );
	desc = createDescendant(vDesc.size() - 1);
//	print(str);
	spouses = createSpouses(vDesc.size()-1);
//	print(str);
	str.Format(L"%s %s", desc, spouses);
	print(str);
	if (!p.arm.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s család]</font>", p.arm);
		str.Format(L" <font color='blue'>[%s]</font>", p.arm);
		print(str);
	}
	if (!p.csalad.IsEmpty())
	{
		str.Format(L" <font color='blue'>[%s család]</font>", p.csalad);
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
// vRowid vektorba gyûjti a listázott leszármazottak rowid-jeit.
// Elõtte megnézi, hogy szerepel-e már benne. Ha igen true, ha nem false a visszatérés
bool CDescendants::rowidExist()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/*
void CDescendants::order_vDesc()
{
	int i;
	int gMax = 1;
	int ix = 0;	// leszármazott indexe	
	int order = 0;

	while (gMax != 0)		// amíg talál nem sorszámozott sort 
	{
		gMax = 0;
		++order;
		// megkeresi a legnagyobb mégnem sorszámozott generációt. Ennek indexe az ix.
		for ( i = 1; i < vDesc.size(); ++i)
		{
			if (vDesc.at(i).order == 0 && vDesc.at(i).g >= gMax)		// == is kell, hogy az utolsó testvérig elmenjen
			{
				gMax = vDesc.at(i).g;
				ix = i;
			}
		}
		// Ha tálát még nem sorszámozott generációt, akkor azt és annak felmenõit besorszámozza
		if (gMax != 0)
		{
			// testvéreket besorszámozza
			for ( int j = ix; j > 0; --j)  // visszafele megy
			{
				if ( vDesc.at(ix).fatherIndex == vDesc.at(j).fatherIndex )
					vDesc.at(j).order = order;
			}
			// felmenõket besorszámozza
			while (ix > 0)
			{
				vDesc.at(ix).order = order;
				ix = vDesc.at(ix).fatherIndex;
			}
		}
	}
	std::sort(vDesc.begin(), vDesc.end(), sortByOrder);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A vDesc vector rendezése a leszármazotti szálak hossza szerint
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A vLen vectorba kigyûjti a leszármazotti szakaszok hosszát.
// A vLen vektort kívánság szerint rendezi.
// A vDesc vektorba átvezeti a sorrendet, majd ennek megfelelõen rendezi azt.
// 
// Csak adott ember leszármozattaira mûködik, tábla leszármazottaira nem!!!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CDescendants::order_vDesc()
{
	int i;
	int g = 1;
	int maxLen;
	int order;
	std::vector< DN::LEN > vLen;
	DN::LEN len;

	// a ciklushoz felhasznált változók:
	// g - vizsgált generáció


		// A vLen vectorba kigyûjti a leszármazotti szakaszok hosszát
	len.ixFirst = 1;
	g = vDesc.at(0).g + 1;  // Az elsõ generációnál 1-el nagyobbat keres 
	for (i = 2; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).g <= g)
		{
			len.ixLast = i - 1;
			len.length = len.ixLast - len.ixFirst + 1;
			vLen.push_back(len);
			len.ixFirst = i;
		}
	}

	len.ixLast = i - 1;
	len.length = len.ixLast - len.ixFirst + 1;
	vLen.push_back(len);


	// A vLen vektort kívánság szerint rendezi
	if (p_radioOrder == ORDER_INCREASING)
		std::sort(vLen.begin(), vLen.end(), sortByLengthAsc);
	else
		std::sort(vLen.begin(), vLen.end(), sortByLengthDesc);


	// A vDesc vektorba átvezeti a sorendet, majd ennek megfelelõen rendezi azt
	for (int i = 0; i < vLen.size(); ++i)
	{
		for (int j = vLen.at(i).ixFirst; j <= vLen.at(i).ixLast; ++j)
		{
			vDesc.at(j).order = i + 1;
		}
	}
	std::sort(vDesc.begin(), vDesc.end(), sortByOrder);
 }
*/
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::order_vDesc()
{
	int i;
	int g = 1;

	int ixFirst;		// a vDesc index-tartománya, amelyben rendezünk
	int ixLast;
	int maxLen;
	int order;
	int b;
	int e;
	
	std::vector< DN::LEN > vLen1;
	std::vector< DN::LEN > vLen2;

	std::vector<DN::LEN>* vLenIn;
	std::vector<DN::LEN>* vLenTemp;		// vLenI  és vLenOu váltásához átmeneti cím

	DN::LEN len;
	m_orderFileTitle = L"order";
	m_orderPathName;
	m_orderPathName.Format(L"%s\\%s_%s.txt", m_descendantsPath, m_orderFileTitle, getTimeTag());

	if (!openFileSpec(&orderTxt, m_orderPathName, L"w+")) return;


	// A legnagyobb generáció beállítása és 
	// a generációk számától függõ szorzó beállítása úgy, hogy 

	int gMax		= 1;
	int multiplyer	= 1;	// a vizsgált generácira vonatkozó szorzó
	for (int i = 0; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).g > gMax)
		{
			gMax = vDesc.at(i).g;
			multiplyer *= 10;
		}
	}



	// a ciklushoz felhasznált változók:
	// g - vizsgált generáció

	// A vLen vectorba kigyûjti a leszármazotti szakaszok hosszát
//		g = vDesc.at(0).g + 1;  // Az elõírt generáció alatti generációkat vizsgálja
	
	vLenIn = &vLen1;
	vLenOu = &vLen2;

	bool found;
	len.ixFirst = 0;
	len.ixLast = vDesc.size() - 1;
	len.order = 1;
	vLenOu->push_back(len);
	printvLen(0);

	for (g = 0; g < 2; ++g)
	{
		vLenTemp = vLenIn;
		vLenIn = vLenOu;
		vLenOu = vLenTemp;
		vLenOu->clear();

		// minden intervallumon végigmegy
		for (int i = 0; i < vLenIn->size(); ++i)
		{
			ixFirst = vLenIn->at(i).ixFirst;
			ixLast = vLenIn->at(i).ixLast;
			found = false;
			if( ixFirst != ixLast )
			{
				// Egy intervallumon belül végigmegy a vDesc-en és 'g' generációt keres
				for (int j = ixFirst; j <= ixLast; ++j)
				{
					// az elsõ "g" generációt kertesi
					if (vDesc.at(j).g == g)
					{
						ixFirst = j;
						for (int k = j + 1; k <= ixLast; ++k)
						{
							// A következõ "g" genráció-t keresi és ha talált elteszi az intervellum-ot és a  hosszát
							if (vDesc.at(k).g == g)			
							{
								len.ixFirst = ixFirst;
								len.ixLast = k - 1;
								len.order = (len.ixLast - len.ixFirst + 1) * multiplyer;
								if (len.order != 0)
									vLenOu->push_back(len);
								ixFirst = k + 1;
								found = true;
								break;
							}
						}
						if ( !found )			// csak 1 db "g" generációt talált, elteszi 
						{
							len.ixFirst = ixFirst;
							len.ixLast = ixFirst;
							len.order = multiplyer;
							vLenOu->push_back(len);
							++ixFirst;
						}
					}
				}
			}
//			if (!found)
			{
				len.ixFirst = ixFirst;
				len.ixLast = ixLast;
				len.order = (len.ixLast - len.ixFirst + 1) * multiplyer;
				vLenOu->push_back(len);
			}
		}
		printvLen(g);
	}
	

	b = vLenOu->at(0).ixFirst - 1;
	e = vLenOu->at(vLenOu->size() - 1).ixLast + 1;
	b = 0;
	e = vDesc.size() -1;

	// A vLen vektort kívánság szerint rendezi
	if (p_radioOrder == ORDER_INCREASING)
	{
		vLenOu->at(0).order = INT_MIN;  // az õs mindenképpen az elejére kerüljön
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderAsc);
	}
	else
	{
		vLenOu->at(0).order = INT_MAX;
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderDesc);
	}


	// A vDesc vektorba átvezeti a sorrendet, majd ennek megfelelõen rendezi azt
	for (int i = 0; i < vLenOu->size(); ++i)
	{
		for (int j = vLenOu->at(i).ixFirst; j <= vLenOu->at(i).ixLast; ++j)
		{
			vDesc.at(j).order = i + 1;
		}
	}
	std::sort(vDesc.begin(), vDesc.end(), sortByOrder);

	printvLen();

	fclose(orderTxt);
	theApp.showFile(m_orderPathName);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printvLen()
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
// A leszármazottkat tartalmazó vGKR vektort szakaszokra bontjuk, és minden szakaszhoz egy sorrendet rendelünk.
// A sorrendet visszavezetjük a vGKR-be, kívánság szerint rendezzük, majd kiírjuk.
// A szakaszokat interációs módszerrel állítjuk elõ.
// Generációnkét bontjuk szakaszokra a vGKR-t az alábbi bejegyzésekkel:
//
// int g;			// vizsgált generáció
// int multiplyer;	// generációhoz rendelt szorzó
// int ixFirst;		// intervallum vGKR beli elsõ indexe
// int ixLast;		// intervallum vGKR beli utolsó indexe
// int order;		// intervallum hossza * multiplyer         
// int gBack;		// az intervallumban történt visszaugrott generáció. Ha nincs akkor 0 
//
// A legkisebbtõl a legnagyobb generációig ciklusban végezzük az intervallumok felbontását.
// Egy korábbi cikusban létrehozott intervallumot elfogadunk, ha nincs benne generációs visszaugrás ( gBack != 0 )
// Ha van, akkor azt a szakaszt tovább bontjuk a következõ generáció szerint, hasonló módon, csak egy felbontott al-intervallum
//  "order"  értéke az anya-intervallum "order" értékéhez adódik hozzá

void CDescendants::order_vDesc()
{
	int i;
	int j;
	int k;

	// vLen vektor elemei
	int g;			// vizsgált generáció
	int multiplyer;	// generációhoz rendelt szorzó
	int ixFirst;	// intervallum vGKR beli elsõ indexe
	int ixLast;		// intervallum vGKR beli utolsó indexe
	int order;		// intervallum hossza * multiplyer         
	int gBack;		// az intervallumban történt visszaugrott generáció. Ha nincs akkor 0 
	
	int maxLen;

	std::vector< DN::LEN > vLen1;		// a parent-intervallumokat tartalmazó vektor (bemenet)
	std::vector< DN::LEN > vLen2;		// a gyerek-intervallumokat tartalmazó vektor (kimenet)

	std::vector<DN::LEN>* vLenIn;
	std::vector<DN::LEN>* vLenTemp;		// vLenI  és vLenOu váltásához átmeneti cím

	

	if (m_log)
	{
		m_orderFileTitle = L"order";
		m_orderPathName;
		m_orderPathName.Format(L"%s\\%s_%s.txt", m_descendantsPath, m_orderFileTitle, getTimeTag());

		if (!openFileSpec(&orderTxt, m_orderPathName, L"w+")) return;
	}

	// A legnagyobb generáció beállítása és 
	// a generációk számától függõ szorzó beállítása úgy, hogy 

	int gMax = 1;
	multiplyer = 1000;	// a vizsgált generácira vonatkozó szorzó
	for (int i = 0; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).g > gMax)
		{
			gMax = vDesc.at(i).g;
//			multiplyer *= 10;
		}
	}



	// a ciklushoz felhasznált változók:
	// g - vizsgált generáció
	
	// A vLenOu vectorba kigyûjti a leszármazotti szakaszokat 
	DN::LEN in;			// aktuális bemeneti intervallum
	DN::LEN ou;			// felbontás utáni kimeneti intervallum
	vLenIn = &vLen1;
	vLenOu = &vLen2;

	bool found;

	int gP;			// parent intervallum generációja
	int gAct;
	int gPrev = 0;
	vLenOu->clear();
	// B leszármazotti lánc rendezendõ intervallumainak meghatározása
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
				// az intervellumon belül elõforduló generációs visszaugrás
				if (gAct >1 && gAct < gPrev) // visszaugrás
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
		// ciklusonként felcseréli a két vLen vektort, hogy a bemenet a korábbi ciklus kimente legyen
		vLenTemp = vLenIn;
		vLenIn = vLenOu;
		vLenOu = vLenTemp;
		vLenOu->clear();
		bontas = false;
		multiplyer *= 2;
		++gNext;
		// minden intervallumon végigmegy
		for (i = 0; i < vLenIn->size(); ++i)
		{
			in = vLenIn->at(i);
			if (in.gBack == 0)  // azokat az intervallumokat, amelyekben nincs generációs visszaugrás, átteszi a kimenetbe
			{
				vLenOu->push_back( in );
			}
			else  // az intervallumban generációs visszaugrás van, fel kell bontani child-intervallumokra  
			{
				// az intervallum elsõ tagját kiírjuk, a maradékot vizsgáljuk
				ou = in;
				ou.ixLast = ou.ixFirst;
				ou.gBack = 0;
				ou.multiplyer = multiplyer;
				vLenOu->push_back(ou);
				
				for (int j = in.ixFirst +1; j <= in.ixLast; ++j)
				{
					gPrev = ou.g;
					// A következõ generációt vizsgáljuk					
					
					if (vDesc.at(j).g == gNext )  // ou.g elsõ elõfordulása, 
					{
						//  a felbontott uj intervallum kezdete
						ou.g = gNext;
						ou.ixFirst = j;
						ou.gBack = 0;
						gPrev = gNext;
						// keressük ou.g kövekezõ elõfordulását, ami az intervallum vége lesz
						for ( k = j + 1; k <= in.ixLast; ++k)
						{
							gAct = vDesc.at(k).g;
							// van az intervellumon belül generációs visszaugrás ?
							if (gAct < gPrev) // visszaugrás
							{
								ou.gBack = gAct;
								bontas = true;
							}

							if (gAct == gNext )  // ou.g következõ elõfordulása
							{
								// az intervallum vége
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
//					// ebben nincs ismétlõdés, et a szakaszt kiírjuk, 
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
	

	// A vLen vektort kívánság szerint rendezi
	if (p_radioOrder == ORDER_INCREASING)
	{
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderAsc);
	}
	else
	{
		std::sort(vLenOu->begin(), vLenOu->end(), sortByOrderDesc);
	}
	printvLen();

	// A vDesc vektorba átvezeti a sorrendet, majd ennek megfelelõen rendezi azt
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
