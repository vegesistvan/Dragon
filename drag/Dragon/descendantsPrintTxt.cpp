#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "descendants.h"
#include "Table_people_columns.h"
#include "utilities.h"
#include "build_defs.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirand� lesz�rmazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printVectorTxt(int i)
{
	CString tableheader;
	CString tableNumber;
	tableNumber = theApp.v_tableNumbers.at(i);
	m_command.Format(L"SELECT tableHeader FROM tables WHERE rowid = '%s'", tableNumber);
	if (!theApp.query(m_command)) return;
	tableheader = theApp.m_recordset->GetFieldString(0);
	str.Format(L"<b>%s</b><br>\n\n", tableheader);
//	printTxt(str);

	int ix;

	m_listedP = 0;
	m_listedD = 0;
	m_indent = 0;
	m_genPrev = 0;

	m_familyName.Empty();
	vLMX.clear();		// a gener�ci� utols� ki�rt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);
	str.Format(L"%s lesz�rmazottainak list�z�sa...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size());
	wndP.SetPos(0);

	for (int i = 0; i < vDesc.size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		if (vDesc.at(i).hidden) continue;   // apa bejegyz�se, aki m�s t�bl�ban szerepel

		if (p_repeated != 0 && vDesc.at(i).status == 2) continue;	// ism�tl�d� bejegyz�s, amit nem ki akarunk ki�rni

		queryPeople(vDesc.at(i).rowid, &p);

		if (!p_mother && vDesc.at(i).parentSex == WOMAN) continue;

		printBeginingTxt(i);	// sor elej�nek elk�sz�t�se; 
		printDescendantTxt(i);
		printSpousesTxt(i);
		m_genPrev = vDesc.at(i).g;

		if (!p.arm.IsEmpty())
		{
			str.Format(L" [%s]", p.arm);
			printTxt(str);
		}
		if (!p.csalad.IsEmpty())
		{
			str.Format(L" [%s]", p.csalad);
			printTxt(str);
		}


		if (!p.folyt.IsEmpty() && p_folyt)
		{
			str.Format(L" %c%c%c folyt %s", '%', '%', '%', p.folyt);
			printTxt(str);
		}
		printTxt(L"\n");
		fflush(txtOut);
	}
	printTxt( L"\n");
	wndP.DestroyWindow();


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genr�ci� v�ltoz�st�l f�gg� behuz�s �s genr�ci�s k�d nyomtat�sa az m_sytax �rt�k�t�l f�gg�en
void CDescendants::printBeginingTxt(int i)
{
	CString family;
	int z;
	int g = vDesc.at(i).g;
	CString indent;

//	setlocale(LC_CTYPE, "");
	wchar_t diamond = 0x25C6;

	for (int i = 0; i < g; ++i)
		indent += L"\t";


	TCHAR gen;
	int abc = 26;

	int cycle = (g) / abc;
	int bias = (g) % abc;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;

	// Kiemelt csal�dn�v //////////////////////////////////////
	if (p_lastname == 2)
	{
		if (m_familyName != p.last_name)
		{
			family = getTableHeader();
			str.Format(L"%s   %s\n", indent, family);
			printTxt(str);
			m_familyName = p.last_name;

		}
	}
	/////lesz�rmazott sor�nak kezedete /////////////////////////////////////////////
	if (p_numbering == SZLUHA)
		str.Format(L"%s%d. %c%lc", indent, vDesc.at(i).childorder, gen, diamond);
	else if (p_numbering == VIL)
		str.Format(L"%s%c%d%lc", indent, gen, vDesc.at(i).childorder, diamond);
	else if (p_numbering == TUP)
	{
		z = vSerial.size() - 1;
		if (g < z)
			++vSerial.at(g);
		else
			vSerial.push_back(1);
		str.Format(L"%s%c-%d%lc", indent, gen, vSerial.at(g), diamond);
	}
	printTxt(str);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printDescendantTxt(int i)
{
	CString name;
	CString line;
	CString titolo;
	CString rang;
	CString lastname;
	int parentIndex;
	int motherIndex;
	int z;
	int j;
	int last;
	int	lastMotherIndex;
	LMX lmx;

	name = p.first_name;
	if (p_lastname == 1)
	{
		lastname = p.last_name;
		if (lastname == L"N") lastname.Empty();
		if (p_capitalName)
			lastname = convertNameToUpper(lastname);
		if (p_checkBold)
			lastname.Format(L"<b>%s</b>", lastname);
		if (lastname == L"N;")
			name.Format(L"\n%s %s %s", lastname, p.first_name, p.peer);
		else
			name.Format(L"\n%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		name.Trim();
		if (!p.other_names.IsEmpty())
			name.Format(L"\n%s_(%s)", (CString)name, p.other_names);
	}

	if (i && vDesc.at(vDesc.at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apj�nak t�bb feles�ge volt
	{
		if (vDesc.at(vDesc.at(i).parentIndex).sex == WOMAN)
			p.parentIndex = _wtoi(p.whichHusband);
		else
			p.parentIndex = _wtoi(p.whichWife);

		last = vLMX.size() - 1;
		if (last == -1 || vLMX.at(last).g < vDesc.at(i).g)		// magasabb gener�ci�ban van
		{
			lmx.g = vDesc.at(i).g;
			lmx.lastMotherIndex = p.parentIndex - 1;
			vLMX.push_back(lmx);
		}
		else													// alacsonyabb gener�ci�ban van
		{
			// megkeresi saj�t gener�ci�j�nak utolj�ra list�zott motherIndex-�t
			j = vLMX.size() - 1;
			for (j = vLMX.size() - 1; j > 0; --j)
			{
				if (vLMX.at(j).g > vDesc.at(i).g) // a nagyobb gener�ci�k eldob�sa
					vLMX.pop_back();
			}
		}
		last = vLMX.size() - 1;
		if (vLMX.at(last).lastMotherIndex != p.parentIndex)   // ha az utolj�ra ki�rt motherIndex m�s, akkor ezt ki�rja
		{
			str.Format(L"/%d", p.parentIndex);
			name += str;
			vLMX.at(last).lastMotherIndex = p.parentIndex;
		}
	}

	name += L" ";
	name += p.posterior;
	name.TrimRight();

	str = getPlaceDateBlock(p.birth_place, p.birth_date, L"*");
	line = name;
	if (!str.IsEmpty())
		line.Format(L"%s %s", name, str);

	str = getPlaceDateBlock(p.death_place, p.death_date, L"+");
	if (!str.IsEmpty())
		line.Format(L"%s %s", (CString)line, str);

	str.Empty();
	if (!p.comment.IsEmpty())
	{
		if (!p.occupation.IsEmpty())
			str.Format(L"%s, %s", p.comment, p.occupation);
		else
			str = p.comment;
	}
	else
	{
		if (!p.occupation.IsEmpty())
			str = p.occupation;
	}
	if (!str.IsEmpty())
	{
		line.Format(L"%s %s", (CString)line, str);
	}

	++m_listedP;
	++m_listedD;
	printTxt(line);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printSpousesTxt(int i)
{
	CString rowid = vDesc.at(i).rowid;
	CString rowidS;
	CString place;
	CString date;
	CString spouse_id;

	if (p.sex_id == MAN)
		m_command.Format(L"SELECT place, date, wife_id FROM marriages WHERE husband_id='%s' ORDER BY whichWife", rowid);
	else
		m_command.Format(L"SELECT place, date, husband_id FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", rowid);
	if (!queryM(m_command)) return;

	int numberOfSpouses = rsM.RecordsCount();

	vFullname.clear();
	for (int i = 0; i < numberOfSpouses; ++i, rsM.MoveNext())
	{
		place = rsM.GetFieldString(0);
		date = rsM.GetFieldString(1);
		spouse_id = rsM.GetFieldString(2);
		if (!spouse_id.IsEmpty() && spouse_id.Compare(L"0"))
		{
			queryPeople(spouse_id, &s);   // h�zast�rs adatainak beolvas�sa
			printMarriageTxt(place, date, i, numberOfSpouses);
			printSpouseTxt();				// ki�r�s el�tt ellen�rzi, hogy szerepel-e ilyen n�vem m�r ki�rt h�zast�rs. Ha igen, akkor sz�nezi
			vFullname.push_back(s.fullname);	// vFullname-en gy�jti a m�r kilist�zott h�zast�rsak nev�t
			printSpRelativesTxt();
		}
	}
	fflush(txtOut);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printMarriageTxt(CString place, CString date, int i, int numberOfSpouses)
{
	CString marriage;

	if (numberOfSpouses > 1)
		marriage.Format(L" %d=", i + 1);
	else
		marriage = L" =";

	if (!place.IsEmpty())
	{
		marriage += place;
		marriage += L" ";
	}
	if (!date.IsEmpty())
	{
		marriage += date;
		marriage += L" ";
	}
	printTxt(marriage);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs list�zados blokkj�t elk�sz�ti
void CDescendants::printSpouseTxt()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if (p_capitalName)
		lastname = convertNameToUpper(lastname);
	if (p_checkBold)
		lastname.Format(L"<b>%s</b>", lastname);
	if (lastname != L"N;" && !s.titolo.IsEmpty() && s.peer.IsEmpty())
	{
		fullname += s.titolo;
		fullname += L" ";
	}
	if (!s.last_name.IsEmpty())
	{
		fullname += lastname;
		fullname += L" ";
	}
	if (!s.other_names.IsEmpty())
	{
		fullname += L"_(";
		fullname += s.other_names;
		fullname += L") ";
	}
	if (!s.first_name.IsEmpty())
	{
		fullname += s.first_name;
	}
	fullname.Trim();


	if (!s.title.IsEmpty())
	{
		fullname.Format(L"%s %s", s.title, (CString)fullname);
	}
	if (!s.peer.IsEmpty())
	{
		fullname.Format(L"%s %s", s.peer, (CString)fullname);
	}

	if (!s.posterior.IsEmpty())
	{
		fullname += L" ";
		fullname += s.posterior;
	}
	fullname.Trim();
	spouse = fullname;

	//	m_birth = s.birth_date;
	str = getPlaceDateBlock(s.birth_place, s.birth_date, L"*");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	str = getPlaceDateBlock(s.death_place, s.death_date, L"+");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	if (!s.comment.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, s.comment);

	++m_listedP;
	printTxt(spouse);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs szem�lyle�r� blokkj�t elk�sz�ti, a neveket bold-dal kiemnelve
void CDescendants::printSpRelativesTxt()
{
	CString father(L"");
	CString mother(L"");
	CString parents;
	int		numOfSpouses;
	CString spouse_id;
	CString order;
	CString spouseSpouse;
	int pos;

	if (!s.father_id.IsEmpty() && s.father_id != L"0")
	{
		queryPeople(s.father_id, &sf);
		sf.first_name.Replace('-', ' ');
		father = getFirstWord(sf.first_name);
	}
	if (!s.mother_id.IsEmpty() && s.mother_id != L"0")
	{
		queryPeople(s.mother_id, &sm);
		mother = getLastFirst(&sm);
		mother.Trim();
	}

	if (!father.IsEmpty() && !mother.IsEmpty())		// ( apa �s anya is meg van adva )
	{
		parents.Format(L" (%s-%s", father, mother);
		++m_listedP;
		++m_listedP;
	}

	if (!father.IsEmpty() && mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", father);
		else
			parents.Format(L" (%s l�nya", father);
		++m_listedP;
	}

	if (father.IsEmpty() && !mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", mother);
		else
			parents.Format(L" (%s l�nya", mother);
		++m_listedP;
	}


	if (s.sex_id == MAN)
		m_command.Format(L"SELECT wife_id, whichWife  FROM marriages WHERE husband_id='%s' ORDER BY whichWife", s.rowid); // a h�zast�rs h�zast�rsai
	else
		m_command.Format(L"SELECT husband_id, whichHusband FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", s.rowid); // a h�zast�rs h�zast�rsai
	if (!query(m_command)) return;
	numOfSpouses = rs.RecordsCount();
	if (numOfSpouses > 1)
	{
		vSpouseIndex.clear();
		for (int i = 0; i < numOfSpouses; ++i, rs.MoveNext())
		{
			spouse_id = rs.GetFieldString(0);
			order = rs.GetFieldString(1);
			queryPeople(spouse_id, &ss);
			if (ss.rowid != p.rowid)		// a GA sorban szerepl� h�zast�rsat kihagyja
			{
				spouseSpouse = getLastFirst(&ss);
				if (parents.GetLength())
					parents += L", ";
				else
					parents = L" (";

				str.Format(L"%sf. %s", order, spouseSpouse);
				++m_listedP;
				parents += str;
				vSpouseIndex.push_back(order);
			}
		}
	}
	if (!parents.IsEmpty())
	{
		parents.TrimRight();
		parents += L")";
	}
	printTxt(parents);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::openTxt( CString file, CString title )
{
	CString descendantsPath;
	descendantsPath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(descendantsPath, 0))
		_wmkdir(descendantsPath);
	m_txtFile.Format(L"%s\\%s_%s.txt", descendantsPath, file, getTimeTag());

	if (!openFileSpec( &txtOut, m_txtFile, L"w+")) return false;


	int l = -37;
	CString nok;
	if (p_mother)
		nok = L"igen";
	else
		nok = L"nem";

	CString kihagy;
	switch (p_repeated)
	{
	case 0:
		kihagy = L"Nem hagyja aki az ism�tl�d� lesz�rmazottakat.";
		break;
	case 1:
		kihagy = L"Az els� lesz�rmazottat ki�rja, a t�bbit elhagyja.";
		break;
	case 2:
		kihagy = L"Ha az apja lesz�rmazott, akkor ki�rja, ha az anyja, akkor nem";
	}

	CString maxGen;
	if (m_editGenMax.IsEmpty())
		maxGen = L"minden gener�ci�";
	else
		maxGen = m_editGenMax;

	CString inputFile;
	CString created;
	m_command = L"SELECT filespec, created FROM filespec";
	if (!theApp.query(m_command)) return false;
	inputFile = theApp.m_recordset->GetFieldString(0);
	created = theApp.m_recordset->GetFieldString(1);

	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	CString connect;
	connect = p_connect ? L"igen" : L"nem";


	printTxt(title);
	printTxt(L"\n\n");
	str.Format(L"%*s Dragon v. %s\n", l, L"Program:", theApp.m_version);
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"Dragon.exe k�sz�lt:", MultiToUnicode(LPCSTR(BUILD)));
	printTxt(str);

	str.Format(L"%*s %s\n", l, L"Alapk�nyvt�r:", theApp.m_dbFolderPath);
	printTxt(str);

	str = L"�RES";
	if (theApp.m_inputMode == MANUAL)
		str = L"k�zi adatbevitel";
	else if (theApp.m_inputMode == GEDCOM)
		str = L"GEDCOM f�jl";
	else if (theApp.m_inputMode = GAHTML)
		str = L"GA.htm f�jl";

	str.Format(L"%*s %s\n", l, L"Adatb�zis bemenete:", (CString)str);
	printTxt(str);

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM)
	{
		str.Format(L"%*s %s %s\n", l, L"Bementi f�jl k�sz�lt:", created, inputFile);
		printTxt(str);
	}
	str.Format(L"%*s %s %s\n", l, L"Adatb�zis k�sz�lt:", dateDB, theApp.m_dbFileName);
	printTxt(str);
	if (!theApp.m_uniteVersion.IsEmpty() )
		str.Format(L"%*s van\n", l, L"�sszevon�s:");
	else
		str.Format(L"%*s nincs\n", l, L"�sszevon�s:");
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"Lista k�sz�lt:", theApp.getPresentDateTime());
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"Gener�ci�k max sz�ma:", maxGen);
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"El�gaz�sok �sszek�t�se:", connect);
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"N�k lesz�rmazottai:", nok);
	printTxt(str);
	str.Format(L"%*s %s\n", l, L"Ism�tl�d�k kihagy�sa:", kihagy);
	printTxt(str);

	if (p_repeatedColor)
	{
		if (p_repeated == 0)
			str.Format(L"%*s %s\n", l, L"Ism�tl�d�k sz�nez�se:", L"z�ld - els� el�fordul�s, piros - �jabb el�fordul�s");
		else
			str.Format(L"%*s %s\n", l, L"Ism�tl�d�k sz�nez�se:", L"z�ld");
		printTxt(str);
	}
	printTxt(L"\n\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::closeTxt()
{

	printTxt(L"\n\n");

	if (m_actGen == -100)
	{
		str.Format(L"A gener�ci�k sz�ma el�rte az el��rt maximumot, ez�rt a list�z�s abbamaradt.\n\n");
		printTxt(str);
	}
	if (m_givup)
	{
		str.Format(L"A lesz�rmazottak sz�ma el�rte a %d-t, a list�z�s abbamaradt.\n\n", m_maxDesc);
		printTxt(str);
	}
	if (m_canceled)
	{
		str = L"A lesz�rmazottak list�z�sa megszak�tva.\n\n";
		printTxt(str);
	}
	int l = 46;
	int abc = 26;
	int g = m_gMax + 1;
	int cycle = g / abc;
	int bias = g % abc;
	TCHAR gen;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;


	if (theApp.v_tableNumbers.size() == 1 || !theApp.v_rowid.size() == 1)
	{

		str.Format(L"%-*s %8s(%c)\n", l, L"List�zott gener�ci�k sz�ma:", thousand(bias), gen);
		printTxt(str);
		str.Format(L"%-*s %8s\n", l, L"List�zott lesz�rmazottak sz�ma:", thousand(m_listedD));
		printTxt(str);
		if (p_repeated)
		{
			str.Format(L"%-*s %8s\n", l, L"Ism�tl�d�, nem list�zott lesz�rmazottak sz�ma:", thousand(m_cntRepeated));
			printTxt(str);
		}
		str.Format(L"%-*s %8s\n", l, L"�sszes list�zott emberek sz�ma:", thousand(m_listedP));
		printTxt(str);
		str.Format(L"%-*s %s\n", l, L"Fut�si id�:", theApp.getTimeElapsed(m_startTime));
		printTxt(str);
	}
	fclose( txtOut );
	theApp.showFile(m_txtFile);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printTxt( CString str )
{
	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(txtOut, L"%s", str);  // a soreleji %-okat printel�si karakterekenk �rtelmezn�, ha k�zvetlen�l nyomtatn�nk!!! 
	fflush(fl);
}