// descendantsNew.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "descendantsNew.h"
#include "descendantsNewParam.h"
#include "Table_people_columns.h"
#include "Table_tables.h"

IMPLEMENT_DYNAMIC(CDescendantsNew, CWnd)
CDescendantsNew::CDescendantsNew()
{
	m_descendantsPath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_descendantsPath, 0))
		_wmkdir(m_descendantsPath);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsNew::~CDescendantsNew()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsNew, CWnd)
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
	CDescendantsNew desc;
	desc.start();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnTableDescendants()
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		AfxMessageBox(L"Nincs kijelölve senki!");
		return;
	}

	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, 1);
		theApp.v_tableNumbers.push_back(rowid);
	}
	CDescendantsNew desc;
	desc.tablesDescendants();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsNew::start()
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
				printVector(i);
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
bool CDescendantsNew::tablesDescendants()  // listázandó táblák a theapp.v_tableNumbers vektorban
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

		if (!p_oneFile)
		{
			title.Format(L"%s %s tábla", familyName, tableRoman);
			title.Trim();
			title.Format(L"%s tábla leszármazotti listája", (CString)title);

			file.Format(L"%s_tábla", title);
			openHtml(file, title, p_colorBgrnd);
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
		desc.sex = peoS.sex;
		desc.numOfChildren = peoS.numOfChildren;
		desc.procChildren = 0;
		desc.numOfSpouses = 0;
		desc.parentIndex = 0;
		desc.motherIndex = 1;
		desc.name = peoS.name;

		vDesc.clear();
		vDesc.push_back(desc);

		m_os = desc.name;
		descendants();
		multipleRowid();
		printVector(i);

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
void CDescendantsNew::descendants()
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
	m_maxDesc = 200000;
	if (maxGen == -2) maxGen = 0;
//	CString fileSpec;
//	fileSpec = theApp.openTextFile(&fl, L"vDesc", L"w+");


	str.Format(L"%s leszármazottainak összegyûjtése folyik...", m_os);
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

	m_folyt_db = 0;
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
		for ( i = vDesc.size() - 1; i >= 0;  --i)
		{
			
			if( vDesc.at(i).rowid == L"26103" )
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
 		rowid = getNextChildRowid( i );
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
			
		desc.parentIndex = i;
		desc.parentSex = vDesc.at(i).sex;
		desc.g = vDesc.at(i).g + 1;		// apja generációjánál 1-el nagyobb
		desc.status = 0;

		vDesc.at(i).procChildren += 1;		// a most feldolgozott testvéreinek száma nõ egyet
		desc.childorder = vDesc.at(i).procChildren;
		
		desc.hidden = false;

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

		i = vDesc.size() - 1;
//		fwprintf(fl, L"%20s %8s %2d %1d\n", vDesc.at(i).name, vDesc.at(i).rowid, vDesc.at(i).numOfChildren, vDesc.at(i).sex);
//		fflush(fl);
		if (rowid == L"349494")
			z = 1;
	}
	wndP.DestroyWindow();
	i = m_folyt_db;
//	fclose(fl);
//	theApp.showFile(fileSpec);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsNew::descendantsOld()
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
void CDescendantsNew::listDescendants()
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
bool CDescendantsNew::queryR(CString rowid)
{
	int z;
	
	m_command.Format(L"SELECT generation, sex_id, posterior, last_name, first_name, tableNumber, folyt, parentIndex  FROM people WHERE rowid ='%s'", rowid);
	if (!queryP(m_command)) return false;

	CString folyt;
	folyt = rsP.GetFieldString(6);
	if ( !folyt.IsEmpty())
		++m_folyt_db;

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
int CDescendantsNew::getNumberOfChildren(CString rowid, CString sex_id)
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
CString CDescendantsNew::getNextChildRowid( UINT i )
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
//			m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY parentIndex, birth_date", parent_id);
			m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY linenumber", parent_id);
		else				// ha nincs összekapcsolás, akkor csak táblán belül keres
	//		m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY parentIndex, birth_date", parent_id, m_tablenumber);
			m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
	}
	else
	{
		if (p_connect)
			m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY linenumber", parent_id);
		else
			m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY linenumber", parent_id, m_tablenumber);
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
bool CDescendantsNew::parameters()
{
	CDescendantsP dlg;
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
	p_folyt = dlg.m_checkFolyt;
	p_colorBgrnd = dlg.m_colorBgrnd;
	p_repeated = dlg.m_repeated;
	p_repeatedColor = dlg.m_repeatedColor;
	p_oneFile = dlg.m_oneFile;
	p_print = false;

	p_html = !dlg.m_html;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsNew::multipleRowid()
{
	CString rowid;
	int g;
	// az ismétlõdõket megjelöli:  az elsõ elõfordulást 1-el, a következõket -1-el
	for (int i = 0; i < vDesc.size(); ++i)
	{
		if (vDesc.at(i).status == 0)
		{
			rowid = vDesc.at(i).rowid;
			for (int j = i + 1; j < vDesc.size(); ++j)
			{
				if (vDesc.at(j).rowid == rowid)
				{
					vDesc.at(i).status = 1;		// elsõ elõfordulás
					vDesc.at(j).status = 2;		// további elõfordulás
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
				else							// h anõ, akkor elfogadja kihagyandónak
					for (i; i < vDesc.size() && vDesc.at(i).status == 2; ++i);
			}
			
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsNew::multipleRowid0()
{
	CString rowid;
	if (p_repeated == 2)   // apa leszármazottait tartja meg
	{
		for (int i = 0; i < vDesc.size(); ++i)
		{
			if (vDesc.at(i).status == 0)
			{
				rowid = vDesc.at(i).rowid;
				for (int j = i + 1; j < vDesc.size(); ++j)
				{
					if (vDesc.at(j).rowid == rowid && vDesc.at(i).status == 0)
					{
						if (vDesc.at(j).parentSex == MAN)	// apja a leszármazott
						{
							signeD(i, j);
						}
						else							// anyja a leszármazott
						{
							signeD(j, i);
						}
					}
				}
			}
		}
	}
	else					// vagy mindent kiír vagy az elsõt tartja meg
	{
		for (int i = 0; i < vDesc.size(); ++i)
		{
			if (vDesc.at(i).status == 0)
			{
				rowid = vDesc.at(i).rowid;
				for (int j = i + 1; j < vDesc.size(); ++j)
				{
					if (vDesc.at(j).rowid == rowid)
					{
						vDesc.at(i).status = 1;
						vDesc.at(j).status = -1;
					}
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i-tõl kezdve minden leszármazottat ( saját és magasabb generációkat) megtart
// j-tõl kezdve minden leszármazottat ( saját és magasabb generációkat) kihagy
void CDescendantsNew::signeD( int i, int j )
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
BOOL CDescendantsNew::query(CString command)
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
BOOL CDescendantsNew::queryP(CString command)
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
BOOL CDescendantsNew::queryM(CString command)
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
void CDescendantsNew::printGAline()
{
	//	fwprintf( fl, L"%d %d %d %d\n", vDesc.at(ix).childrenPrinted, vDesc.at(ix).gen, vDesc.at(ix).numOfChildren, vDesc.size() );

	CString rowid;
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
	printDescendant(vDesc.size() - 1 );
	printSpouses(vDesc.size()-1);

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
bool CDescendantsNew::rowidExist()
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