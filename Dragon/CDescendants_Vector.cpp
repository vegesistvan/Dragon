
#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include "Table_people_columns.h"
#include "Table_tables.h"
#include "Table_people.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortById3(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.id < d2.id);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByNumOfD(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.nOfD > d2.nOfD);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByOrder(const DE::DESC& v1, const DE::DESC& v2)
{
	if (v1.order == v2.order)
		return(v1.original < v2.original);
	return(v1.order < v2.order);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByOrderDesc(const DE::LEN& v1, const DE::LEN& v2)
{
	return(v1.order > v2.order);
}
bool sortByOrderAsc(const DE::LEN& v1, const DE::LEN& v2)
{
	return(v1.order < v2.order);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::create_vDesc()
{
	m_startTime = theApp.getStartTime();
	CString file;
	CString title;
	vDesc.clear();

	m_listedP = 0;
	m_listedD = 0;


	if (!theApp.v_rowid.size() && theApp.v_tableNumbers.size())
	{
		tablesDescendants();
		return false;
	}

	DE::DESC desc;
	for (int i = 0; i < theApp.v_rowid.size(); ++i)
	{
		// Az õs berakása a vDesc vektorba
		m_rowid = theApp.v_rowid.at(i);
		queryR(m_rowid);

//		m_tablenumber = peoS.tablenumber;
		desc = {};
		desc.g = 0;
//		desc.X = 0;
		desc.rowid = m_rowid;
		desc.tablenumber = peoS.tablenumber;
		desc.linenumber = peoS.linenumber;
		desc.titolo = peoS.titolo;
		desc.rowidF.Empty();
		desc.lastname = m_lastname;
		desc.firstname = m_firstname;
		desc.name = peoS.name;
		desc.birth = peoS.birth;
		desc.death = peoS.death;
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
		desc.numOfRep = 0;
		desc.original = 0;
		desc.fatherIndex = -1;  // nincs apja
		desc.shift = -1;
		vDesc.clear();
		vDesc.push_back(desc);
		m_os = desc.name;

		descendants();
		numOfDesc();
		multipleRowid();

		indentedHtml();
		create_id();
		order_numOfD();
	
		CDescendantsLinearTable dlg;
		dlg.p_descendantName = p_descendantName;
		dlg.p_connect = p_connect;
		dlg.p_womenDescendants = p_womenDescendants;
		dlg.p_generationMax = p_generationMax;
		dlg.p_childrenOrder = p_childrenOrder;
		dlg.p_titololower = p_titololower;
		dlg.p_descendantAttrib = p_descendantAttrib;
		dlg.p_otherNameAttrib = p_otherNameAttrib;
		dlg.p_specAttrib = p_specAttrib;
		dlg.p_commentAttrib = p_commentAttrib;
		dlg.p_numberingSystem = p_numberingSystem;		// leszármazottak számozása
		dlg.p_descendantAttrib = p_descendantAttrib;
		dlg.p_otherNameAttrib = p_otherNameAttrib;
		dlg.p_commentAttrib = p_commentAttrib;
		dlg.p_specAttrib = p_specAttrib;
		dlg.p_colorBgrnd = p_colorBgrnd;
		dlg.p_fontSize = p_fontSize;
		dlg.p_capitalName = p_capitalName;
		dlg.p_folyt = p_folyt;
		dlg.p_capital = p_capital;
		dlg.p_bold = p_bold;
		dlg.p_checkCRLF = p_checkCRLF;
		dlg.p_oneOutputFile = p_oneOutputFile;
		dlg.p_repeatedColor = p_repeatedColor;
		dlg.p_rowWidth = p_rowWidth;
		dlg.p_colorBgrnd = p_colorBgrnd;

		dlg.m_descendantsPath = m_descendantsPath;
		dlg.vDesc = &vDesc;
		dlg.m_name = vDesc.at(0).name;
		dlg.m_htmlPathName1 = m_htmlPathName1;

		dlg.DoModal();
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::tablesDescendants()  // listázandó táblák a theapp.v_tableNumbers vektorban
{
	CString title;
	CString file;
	CString descendantsTablePathName;
	CString familyName;
	CString tableHeader;
	CString tableRoman;
	CString father_id;
	CString rowid;
	m_listedP = 0;
	m_listedD = 0;


	m_startTime = theApp.getStartTime();

	DE::DESC desc;

	vDesc.clear();

	if (p_oneOutputFile)
	{
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(0));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableHeader = rs.GetFieldString(1);
		tableRoman = rs.GetFieldString(2);
		title.Format(L"%s %s tábla", familyName, tableRoman);
		title.Trim();

		descendantsTablePathName.Format(L"%s\\%s_%s.html", m_descendantsPath, title, getTimeTag());

		if (theApp.v_tableNumbers.size() > 1)
			file.Format(L"%d_tabulált tábla", theApp.v_tableNumbers.size() - 1);
		else
			file.Format(L"%s_tabulált_tábla", title);
		if (!openHtml(file, title, p_colorBgrnd)) return false;
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

		if (!p_oneOutputFile)
		{
			title.Format(L"%s %s tábla", familyName, tableRoman);
			title.Trim();
			title.Format(L"%s tábla leszármazotti listája", (CString)title);

			file.Format(L"%s_tábla", title);
			if (!openHtml(file, title, p_colorBgrnd)) return false;
		}
		else
		{
			if (!i && theApp.v_tableNumbers.size() > 1)
			{
				title.Format(L"%d tábla tabulált leszármazotti listája", theApp.v_tableNumbers.size());
				file = title;
				if( !openHtml(file, title, p_colorBgrnd)) return false;
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
//		desc.X = 0;
		desc.tablenumber = peoS.tablenumber;
		desc.linenumber = peoS.linenumber;
		desc.titolo = peoS.titolo;
		desc.rowidF = father_id;
		desc.sex = peoS.sex;
		desc.numOfChildren = peoS.numOfChildren;
		desc.childorder = 1;
		desc.procChildren = 0;
		desc.numOfSpouses = 0;
		desc.parentIndex = 0;
		desc.motherIndex = 1;
		desc.lastname = m_lastname;
		desc.firstname = m_firstname;
		desc.name = peoS.name;
		desc.birth = peoS.birth;
		desc.death = peoS.death;
		desc.cntRep = 0;
		desc.order = 0;
		desc.numOfRep = 0;
		desc.fatherIndex = -1;  // ez nem biztos!!!!!!!!
		desc.shift = -1;
		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		if (p_oneOutputFile && theApp.v_tableNumbers.size() > 1)
		{
			str.Format(L"<b>%s</b>\n", tableHeader);
			print(str);
		}

		descendants();
		numOfDesc();
		multipleRowid();

		indentedHtml();
		create_id();
		CDescendantsLinearTable dlg;

		dlg.p_descendantName = p_descendantName;
		dlg.p_connect = p_connect;
		dlg.p_womenDescendants = p_womenDescendants;
		dlg.p_generationMax = p_generationMax;
		dlg.p_childrenOrder = p_childrenOrder;
		dlg.p_titololower = p_titololower;
		dlg.p_descendantAttrib = p_descendantAttrib;
		dlg.p_otherNameAttrib = p_otherNameAttrib;
		dlg.p_specAttrib = p_specAttrib;
		dlg.p_commentAttrib = p_commentAttrib;
		dlg.p_numberingSystem = p_numberingSystem;		// leszármazottak számozása
		dlg.p_descendantAttrib = p_descendantAttrib;
		dlg.p_otherNameAttrib = p_otherNameAttrib;
		dlg.p_commentAttrib = p_commentAttrib;
		dlg.p_specAttrib = p_specAttrib;
		dlg.p_colorBgrnd = p_colorBgrnd;
		dlg.p_fontSize = p_fontSize;
		dlg.p_capitalName = p_capitalName;
		dlg.p_folyt = p_folyt;
		dlg.p_capital = p_capital;
		dlg.p_bold = p_bold;
		dlg.p_checkCRLF = p_checkCRLF;
		dlg.p_oneOutputFile = p_oneOutputFile;
		dlg.p_repeatedColor = p_repeatedColor;

		dlg.m_descendantsPath = m_descendantsPath;
		dlg.vDesc = &vDesc;
		dlg.m_name = vDesc.at(0).name;
		dlg.m_htmlPathName1 = m_htmlPathName1;
		dlg.DoModal();

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	theApp.v_tableNumbers.clear();
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
	int maxGen = _wtoi(p_generationMax) - 2;
	int fatherIndex;
	m_actGen = 0;
	m_gMax = 0;
	m_givup = 0;
	m_canceled = false;
	m_cntRepeated = 0;
	DE::DESC desc;

	m_listedP = 0;
	m_listedD = 0;
	m_maxDesc = 2000000;
	if (maxGen == -2) maxGen = 0;

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

		// visszamegy az utolsó emberre, akinek még van ki fel nem dolgozott gyereke
		for (i = vDesc.size() - 1; i >= 0; --i)
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
		rowid = getNextChildRowid(i);
		m_actGen = vDesc.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;
		queryR(rowid);		// lekérdezi a gyereket és beállítja m_sex_id-t és m_numOfSpouses-t

		m_tablenumber = peoS.tablenumber;

		if (rowid == L"308824")
			z = 1;
//		desc.X = 0;
		desc.tablenumber = peoS.tablenumber;
		desc.linenumber = peoS.linenumber;
		desc.titolo = peoS.titolo;
		desc.rowidF = vDesc.at(i).rowid;
		desc.rowid = rowid;
		desc.lastname = m_lastname;
		desc.firstname = m_firstname;
		desc.name = peoS.name;
		desc.birth = peoS.birth;
		desc.death = peoS.death;
		desc.sex = peoS.sex;
		desc.numOfSpouses = peoS.numOfSpouses;
		desc.numOfChildren = peoS.numOfChildren;
		desc.motherIndex = peoS.motherIndex;
		desc.procChildren = 0;
		desc.order = 0;
		desc.numOfRep = 0;
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
		desc.shift = -1;
		if (desc.sex == WOMAN && !p_womenDescendants)		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
			desc.numOfChildren = 0;
		vDesc.push_back(desc);

		if (vDesc.size() == m_maxDesc)
		{
			str.Format(L"%d leszármazottnál feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vDesc.size();
			break;
		}
	}
	wndP.DestroyWindow();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::print_vDesc()
{
	CString fileSpec;
	fileSpec = theApp.openTextFile(&text, L"vDesc", L"w+");

	for (int i = 0; i < vDesc.size(); ++i)
	{
		fwprintf(text, L"%4d. %6s %6s %5d %-40s\n", i, vDesc.at(i).rowid, vDesc.at(i).rowidF, vDesc.at(i).nOfD, vDesc.at(i).name);
	}
	fclose(text);
	theApp.showFile(fileSpec);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::descendantsOld()
{
	DE::DESC desc;
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


		if (vDesc.at(m_gen).sex == WOMAN && !p_womenDescendants)		// ha nõ a leszármazott és annak a gyerekeit nem akarjuk listázni
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

	m_command.Format(L"SELECT generation, sex_id, posterior, last_name, first_name, tableNumber, folyt, parentIndex, numOfChildren, titolo, linenumber, birth_date, death_date  FROM people WHERE rowid ='%s'", rowid);
	if (!queryP(m_command)) return false;

	CString folyt;
	folyt = rsP.GetFieldString(6);
	//	if ( !folyt.IsEmpty())
	//		++m_folyt_db;
	peoS.tablenumber = rsP.GetFieldString(5);
	m_tableNumber = peoS.tablenumber;
	peoS.linenumber = rsP.GetFieldString(10);
	peoS.titolo = rsP.GetFieldString(9);
	if (p_titololower)
		towlower(peoS.titolo[0]);
	peoS.sex = rsP.GetFieldString(1);
	if (peoS.sex == BISEX)
		peoS.sex = MAN;
	peoS.posterior = rsP.GetFieldString(2);

	m_lastname = rsP.GetFieldString(3);
	if (!m_lastname.IsEmpty() && !iswupper(m_lastname.GetAt(0)))
		m_lastname.Empty();
	m_lastname.Replace('|', ' ');
	m_lastname.Replace('/', ' ');
	m_lastname.Replace('?', ' ');

	m_firstname = rsP.GetFieldString(4);
	peoS.tablenumber = rsP.GetFieldString(5);
	m_firstname.Replace('/', ' ');
	m_firstname.Replace('(', ' ');
	m_firstname.Replace(')', ' ');
	m_firstname.Replace('*', ' ');
	m_firstname.Replace('|', ' ');
	m_firstname.Replace('"', ' ');
	m_firstname.Replace('?', ' ');
	m_firstname.Trim();
	peoS.firstname = m_firstname;
	peoS.name.Format(L"%s %s", m_lastname, m_firstname);
	peoS.name.Trim();
	peoS.numOfChildren = getNumberOfChildren(rowid, peoS.sex);
	//	peoS.numOfChildren = _wtoi(rsP.GetFieldString(8));
	peoS.birth = rsP.GetFieldString(11);
	peoS.death = rsP.GetFieldString(12);

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
CString CDescendants::getNextChildRowid(UINT i)
{
	CString parent_id = vDesc.at(i).rowid;
	UINT	ixChild = vDesc.at(i).procChildren;	// leszármazott ixChild-adik gyerekét keresi  
	CString	sex = vDesc.at(i).sex;
	int z;
	if (parent_id == L"308346")
		z = 0;
	if (sex == MAN)
	{
		if (p_connect)	// ha õsszekapcsolás van, akkor táblától függetlenül keres
		{
			if (p_childrenOrder == DE::ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY linenumber", parent_id);
		}
		else				// ha nincs összekapcsolás, akkor csak táblán belül keres
		{
			if (p_childrenOrder == DE::ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
		}
	}
	else
	{
		if (p_connect)
		{
			if (p_childrenOrder == DE::ORDER_BIRTH)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY linenumber", parent_id);
		}
		else
		{
			if (p_childrenOrder == DE::ORDER_BIRTH)
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::multipleRowid()
{
	CString rowid;
	int g;
	int cnt;
	// az ismétlõdõket megjelöli:  az elsõ elõfordulást 1-el, a következõket -1-el
	str.Format(L"Ismétlõdõ leszármazottak jelölése...");
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size()*2);
	wndP.SetPos(0);

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
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
/*
	if (p_repeatedColor )   // apa leszármazottait tartja meg
	{
		str.Format(L"Ismétlõdõ leszármazottak jelölése...");
		CProgressWnd wndP(NULL, str);
		wndP.GoModal();
		wndP.SetRange(0, vDesc.size()*2);
		wndP.SetPos(0);
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
			else if (vDesc.at(i).status == 2) // ismétlõdõ második elõfordulása, aki kihagyásra van kijelölve
			{
				if (vDesc.at(i).parentSex == MAN)	// ha férfi, akkor listázandó lesz
				{
					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i)
						vDesc.at(i).status = 1;
				}
				//				else							// h anõ, akkor elfogadja kihagyandónak
				//					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i);
			}
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;

		}
	}
*/
	wndP.DestroyWindow();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
BOOL CDescendants::query2(CString command)
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

	// a leszármazatti sor elõtt kiírja a megváltozott családnevet: % name
	if (m_lastnamePrev != p.last_name && !p_capitalName)
	{
		str.Format(L"%c %s\n", '%', p.last_name);
		print(str);
	}
	m_lastnamePrev = p.last_name;
	m_sexidPrev = p.sex_id;

	printBegining(vDesc.size() - 1);	// html kódok és generáció elkészítése; 
	desc = createDescendant(vDesc.size() - 1, true);
	spouses = createSpouses(vDesc.size() - 1);
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printvLen()
{
	if (!m_log) return;
	DE::LEN len;
	fwprintf(orderTxt, L"%2s %7s %7s %7s %7s %2s\n", L"g", L"multi", L"ixFirst", L"ixLast", L"order", L"gBack");
	for (int i = 0; i < vLenOu->size(); ++i)
	{
		len = vLenOu->at(i);
		fwprintf(orderTxt, L"%2d %7d %7d %7d %7d %2d\n", len.g, len.multiplyer, len.ixFirst, len.ixLast, len.order, len.gBack);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::numOfDesc()
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

	str.Format(L"Leszármazottak számának számolása...");
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size());
	wndP.SetPos(0);
	// leszármazottak számának (nOfD) számolása minden bejegyzéshez
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
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::create_id()
{

	// Az id kitöltése, hogy a tényleges listában meg tudja adni a gyerek id-jét
	for (int i = 0; i < vDesc.size(); ++i)
	{
		vDesc.at(i).printed = false;
		vDesc.at(i).idC = 0;
		vDesc.at(i).idF = 0;
		vDesc.at(i).id = -1; 
		vDesc.at(i).ixM.Empty();
	}
	
	std::vector<DE::DESC> vD1;
	std::vector<DE::DESC> vD2;
	int cnt;
	int cntRepI;
	int cntRepJ;
	int g = 1;
	DE::DESC desc;
	DE::DESC desci;
	DE::DESC descj;
	int numOfSpouses;

	str.Format(L"Leszármazottak rendezése a lineáris listához...");
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size());
	wndP.SetPos(0);


	cnt = 0;
	vD1.clear();
	vDesc.at(0).id = cnt;
	++cnt;
	vDesc.at(0).printed = true;
	vD1.push_back(vDesc.at(0));

	CString rowidF;
	CString ixM;
	
	// vD1-k már ki vannak írva, a gyerekeiken kell végigmenni, akiket vD2-be gyûjtünk
	while (true)
	{
		vD2.clear();
		for (int i = 0; i < vD1.size(); ++i)
		{
			desc = vD1.at(i);
			rowidF = desc.rowid;
			for (int j = 0; j < desc.numOfChildren; ++j)
			{
				// gyerekeiket keressük
				for (int k = 0; k < vDesc.size(); ++k)
				{
					if (!vDesc.at(k).printed) 
					{
						if (vDesc.at(k).rowidF == rowidF)
						{
							if (desc.numOfSpouses > 1)
								vDesc.at(k).ixM.Format(L"%d", vDesc.at(k).motherIndex);
							else
								vDesc.at(k).ixM.Empty();


							vDesc.at(k).printed = true;
							vDesc.at(k).id = cnt;		// saját id;
							++cnt;
							vD2.push_back(vDesc.at(k));
						}
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		vD1 = vD2;
		if (!vD1.size()) break;
	}
	std::sort(vDesc.begin(), vDesc.end(), sortById3);


	int j;
	int gPrev = -1;
	// apák azonosítója
	for (int i = 0; i < vDesc.size(); ++i)
	{
		vDesc.at(i).idF = 1234567890;
		rowidF = vDesc.at(i).rowidF;
		cntRepI = vDesc.at(i).cntRep;
		g = vDesc.at(i).g;
		for (j = 0; j < i; ++j)
		{
			cntRepJ = vDesc.at(j).cntRep;
			if( cntRepJ && cntRepI)
			{
				if ((vDesc.at(j).rowid == rowidF) && (cntRepJ == cntRepI))
				{
					vDesc.at(i).idF = vDesc.at(j).id;
					break;
				}
			}
			else
			{
				if (vDesc.at(j).rowid == rowidF )
				{
					vDesc.at(i).idF = vDesc.at(j).id;
					break;
				}
			}
		}
		if (j == i )
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowidF == rowidF )
				{
					vDesc.at(i).idF = vDesc.at(j).id;
					break;
				}
			}
		}
	}


	for (int i = 0; i < vDesc.size(); ++i)
	{
		// gyerekek azonosítója
		for ( j = 0; j < vDesc.size(); ++j)
		{
			if (vDesc.at(j).rowidF == vDesc.at(i).rowid && vDesc.at(j).g == vDesc.at(i).g + 1)
			{
				vDesc.at(i).idC = vDesc.at(j).id;
				break;
			}
		}
		if (j == vDesc.size())
		{
			for (j = 0; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowidF == vDesc.at(i).rowid && vDesc.at(j).g == vDesc.at(i).g + 1)
				{
					vDesc.at(i).idC = vDesc.at(j).id;
					break;
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
		
	}
//	std::sort(vDesc.begin(), vDesc.end(), sortById3);

	wndP.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::order_numOfD()
{
	int idC;
	int dbC;
	int id;
	int j;
	int k;
	int cnt;
	DE::DESC desc;
	std::vector<DE::DESC> vCH;

	for (int i = 0; i < vDesc.size(); ++i)
	{
		desc = vDesc.at(i);

		idC = desc.idC;
		dbC = desc.numOfChildren;

		if (dbC > 1)
		{
			j = idC;
			k = idC + dbC;
			std::sort(vDesc.begin() + idC, vDesc.begin() + k, sortByNumOfD);
			cnt = 1;
			for (j = idC; j < k; ++j)
			{
				vDesc.at(j).id = j;
				vDesc.at(j).childorder = cnt;
				vDesc.at(j).idF = i;
				++cnt;

			}
		}
	}
}