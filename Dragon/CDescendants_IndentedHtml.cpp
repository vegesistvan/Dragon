#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "Table_people_columns.h"
#include "utilities.h"
#include "build_defs.h"
#include "CDescendants.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ix a kiirand� lesz�rmazott indexe a vDesc vektorban
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::indentedHtml()
{
	printVector( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printVector( int tbl)
{
//	CString tableheader;
//	CString tableNumber;
	CString people;
	CString line;
//	tableNumber = theApp.v_tableNumbers.at(tbl);
//	m_command.Format(L"SELECT tableHeader FROM tables WHERE rowid = '%s'", tableNumber);
//	if (!theApp.query(m_command)) return;
//	tableheader = theApp.m_recordset->GetFieldString(0);
//	str.Format(L"<b>%s</b>\n\n", tableheader);
//	print(str);

	int ix;

	if (p_numberingSystem == DE::SZLUHA) // orderd list
	{
		m_tag1 = L"<ol>";
		m_tag2 = L"</ol>\n";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>\n";
	}

	m_indent = 0;
	m_genPrev = 0;

	m_familyName.Empty();
	vLMX.clear();		// a gener�ci� utols� ki�rt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);
	str.Format(L"%s lesz�rmazottainak tabul�lt list�z�sa...", m_os);
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size() );
	wndP.SetPos(0);

	for (int i = 0; i < vDesc.size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		if (vDesc.at(i).hidden) continue;   // apa bejegyz�se, aki m�s t�bl�ban szerepel
		if (vDesc.at(i).cntRep > 1 && p_repeatedDelete) continue;
		if (!p_womenDescendants && vDesc.at(i).parentSex == WOMAN) continue;

		queryPeople(vDesc.at(i).rowid, &p);
		printBegining(i);	// html k�dok �s gener�ci� elk�sz�t�se; 
		people = getComplexDescription(i, true );

		
		line.Format(L"%s%s", m_diamond, people);
		line.Replace('|', '\'');
		print(line);

		m_genPrev = vDesc.at(i).g;
	}

	for (int i = 0; i < m_indent; ++i)			// a kih�z�sok megsz�ntet�se, vissza az alapvonalra
		fwprintf(fl, L"%s", m_tag2);
	wndP.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genr�ci� v�ltoz�st�l f�gg� behuz�s �s genr�ci�s k�d nyomtat�sa az m_sytax �rt�k�t�l f�gg�en
void CDescendants::printBegining(int i)
{
	CString family;
	CString shift;
	int z;
	int g = vDesc.at(i).g;

	if (g > m_genPrev || m_genPrev == 0)	// jobbra tol�s, �j sor gener�ci�ja nagyobb, (csak 1-es�vel n�het a gener�ci�)
	{
		shift = m_tag1;
		++m_indent;
	}
	else if (g < m_genPrev)					// balra tol�s vissza, r�gi gener�ci�, kijebb hozza a gener�ci�-k�l�nbs�g-szeresen
	{
		for (UINT i = 0; i < (m_genPrev - g); ++i)
		{
			shift += m_tag2;
			--m_indent;
		}
	}

	TCHAR gen;
	int abc = 26;

	int cycle = (m_indent - 1) / abc;
	int bias = (m_indent - 1) % abc;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;

	// Kiemelt csal�dn�v //////////////////////////////////////
	if (p_descendantName == DE::RAISED ) 
	{
		if (m_familyName != p.last_name)
		{
			family = getTableHeader();
			family.Replace('|', '\'');
			str = getColoredString(family, DE::KEK);
			if (p_repeatedColor)
			{
				switch (vDesc.at(i).status)
				{
				case 0:
					str = getColoredString(family, DE::KEK);
					break;
				case 1:
					str = getColoredString(family, DE::ZOLD);
					break;
				case 2:
					str = getColoredString(family, DE::PIROS);
					break;
				}
			}
			if (g == m_genPrev) // ugyanabban  a gener�ci�ban n�vv�ltoz�s!!
				str.Format(L"<br>%s%s", shift, (CString)str);	// hogy ugyabban a betol�sban legy�nk, de �j sorban
			else
				str.Format(L"%s%s", shift, (CString)str);			// </ol> benne van a shiftben, ha kell
			shift.Empty();		// az ezt k�vet� lesz�rmazottat m�r nem kell tolni
			print(str);
			m_familyName = p.last_name;
		}
	}
	/////lesz�rmazott sor�nak kezedete /////////////////////////////////////////////
	if (p_numberingSystem == DE::SZLUHA)
		str.Format(L"%s<li>%c&diams;", shift, gen);
	else if (p_numberingSystem == DE::VIL)
		str.Format(L"%s<li>%c%d&diams;", shift, gen, vDesc.at(i).childorder);
	else if (p_numberingSystem == DE::TUP)
	{
		z = vSerial.size() - 1;
		if (g < z)
			++vSerial.at(g);
		else
			vSerial.push_back(1);
		str.Format(L"%s<li>%c-%d&diams;", shift, gen, vSerial.at(g));
	}
	if (p_repeatedColor)
	{
		switch (vDesc.at(i).status)
		{
		case 1:
			str = getColoredString(str, DE::ZOLD);
			break;
		case 2:
			str = getColoredString(str, DE::PIROS);
			break;
		}
	}
	m_diamond = str;
//	print(str);		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getTableHeader()
{
	CString last_name;
	CString percent;
	CString family;
	CString roman;
	CString arm;
	CString tableHeader;


	m_command.Format(L"SELECT percent, familyName, tableRoman, arm, tableHeader FROM tables WHERE rowid='%s'", p.tableNumber);
	if (!query(m_command)) return str;

	percent = rs.GetFieldString(0);
	family = rs.GetFieldString(1);
	roman = rs.GetFieldString(2);
	arm = rs.GetFieldString(3);
	tableHeader = rs.GetFieldString(4);
	last_name = p.last_name;
	last_name.MakeUpper();
	str = last_name;
	
	if (!p.titolo.IsEmpty())
		str.Format(L"%s, %s", (CString)str, p.titolo);
	if (theApp.m_inputMode == GAHTML)
	{
		//str.Format(L"%s (%s %s %s)", (CString)str, percent, family, roman );
		str.Format(L"%s (%s)", (CString)str, tableHeader );
	}
	return(str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::createDescendant(int i, bool parentIndex )
{
	CString name;
	CString line;
	CString titolo;
	CString rang;
	CString lastname;
	CString comment;
	int motherIndex;
	int z;
	int j;
	int last;
	int	lastMotherIndex;
	DE::LMX lmx;
	TCHAR ch;

	name = p.first_name;
	if (p_descendantName == DE::INLINE)
	{
		lastname = p.last_name;
		if (lastname == L"N") lastname.Empty();
		if (p_capitalName)
			lastname = convertNameToUpper(lastname);
		if (p_bold)
			lastname.Format(L"<b>%s</b>", lastname);
		if (lastname == L"N;")
			name.Format(L"\n%s %s %s", lastname, p.first_name, p.peer);
		else
			name.Format(L"\n%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		name.Trim();
		if (!p.other_names.IsEmpty())
			name.Format(L"\n%s_(%s)", (CString)name, p.other_names);
	}
	name = getColoredString(name, p_descendantAttrib);
	


	if (i && vDesc.at(vDesc.at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apj�nak t�bb feles�ge volt
	{
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
		

		
		if (parentIndex)
		{
			if (vLMX.at(last).lastMotherIndex != p.parentIndex)   // ha az utolj�ra ki�rt motherIndex m�s, akkor ezt ki�rja
			{
				if (p.parentIndex)
				{
					str.Format(L"/%d", p.parentIndex);
					name += str;
					vLMX.at(last).lastMotherIndex = p.parentIndex;
				}
			}
		}

	}
	
	if( p.comment.GetAt(0) != ',' )
		name += L" ";

	if (!p.posterior.IsEmpty() )
	{
		name += p.posterior;
		if (p_checkCRLF)
			name += L"\n";
	}


	str = getPlaceDateBlock(p.birth_place, p.birth_date, L"*");
	line = name.Trim();
	if (!str.IsEmpty())
		line.Format(L"%s %s", name.Trim(), str);

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

	comment = getColoredString(str, p_commentAttrib);
	comment.Trim();
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	// ha sz�nezni akarjuk, akkor a t�rlend� piros, a megtartand� k�k
	if (p_repeatedColor)
	{
		switch (vDesc.at(i).status)			
		{
		case 1:
			line = getColoredString(line, DE::ZOLD);
			break;
		case 2:
			line = getColoredString(line, DE::PIROS);
			break;
		}
	}
	++m_listedP;
	++m_listedD;
	return(line);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::createSpouses(int i)
{
	CString rowid = vDesc.at(i).rowid;
	CString rowidS;
	CString place;
	CString date;
	CString spouse_id;
	CString marriage;
	CString spouse;
	CString spRelatives;
	CString spouses;
	
	if (p.sex_id == MAN)
		m_command.Format(L"SELECT place, date, wife_id FROM marriages WHERE husband_id='%s' ORDER BY whichWife", rowid);
	else
		m_command.Format(L"SELECT place, date, husband_id FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", rowid);
	if (!queryM(m_command)) return L"";

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
			marriage = createMarriage(place, date, i, numberOfSpouses);
			spouse = createSpouse();				// ki�r�s el�tt ellen�rzi, hogy szerepel-e ilyen n�vem m�r ki�rt h�zast�rs. Ha igen, akkor sz�nezi
			vFullname.push_back(s.fullname);	// vFullname-en gy�jti a m�r kilist�zott h�zast�rsak nev�t
			spRelatives = createSpRelatives();
			if (!marriage.IsEmpty())
				spouses += marriage;
			if (!spouse.IsEmpty())
				spouses += spouse;
			if (!spRelatives.IsEmpty())
				spouses += spRelatives;
			spouses += L" ";
		}
	}
	spouses.Trim();
	return spouses;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::createMarriage(CString place, CString date, int i, int numberOfSpouses)
{
	CString marriage;

	if (numberOfSpouses > 1)
		str.Format(L" %d=", i + 1);
	else
		str = L" =";

	marriage = getColoredString(str, p_specAttrib);

	if (!place.IsEmpty())
	{
		marriage += place;
		if (date.IsEmpty())
			marriage += L",";
		marriage += L" ";
	}
	if (!date.IsEmpty())
	{
		marriage += date;
		marriage += L" ";
	}
	return marriage;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs list�zados blokkj�t elk�sz�ti
CString CDescendants::createSpouse()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if (p_capitalName)
		lastname = convertNameToUpper(lastname);
	if (p_bold)
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
		fullname.TrimRight();
		fullname += L"_(";
		fullname += s.other_names;
		fullname += L") ";
	}
	if (!s.first_name.IsEmpty())
	{
		fullname += s.first_name;
	}
	fullname.Trim();
	fullname = getColoredString(fullname, p_otherNameAttrib);

	// Ellen�rti, hogy ilyen n�ven m�r list�zott-e h�zast�rsat. Ha igen, akkoe sz�nezi
	for (int i = 0; i < vFullname.size(); ++i)
	{
		if (s.fullname == vFullname.at(i))
		{
			fullname = getColoredString(fullname, DE::PIROS);   //!!!!!!!!!!!!!!!!!!
			break;
		}
	}

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
		if (p_checkCRLF)
			fullname += L"\n";
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

	str = getColoredString(s.comment, p_commentAttrib);
	if (!str.IsEmpty())
	{
		if (s.comment.GetAt(0) == ',')
			spouse.Format(L"%s%s", (CString)spouse, str);
		else
			spouse.Format(L"%s %s", (CString)spouse, str);
	}

	++m_listedP;
	return spouse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs szem�lyle�r� blokkj�t elk�sz�ti, a neveket bold-dal kiemnelve
CString CDescendants::createSpRelatives()
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
		str = getFirstWord(sf.first_name);

		father = getColoredString(str, p_otherNameAttrib);
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
	if (!query(m_command)) return L"";
	numOfSpouses = rs.RecordsCount();
	if (numOfSpouses > 1)
	{
		vSpouseIndex.clear();
		for (int i = 0; i < numOfSpouses; ++i, rs.MoveNext())
		{
			spouse_id = rs.GetFieldString(0);
			order = rs.GetFieldString(1);
			order.Format(L"%d", i + 1);
			queryPeople(spouse_id, &ss);
			if (ss.rowid != p.rowid)		// a GA sorban szerepl� h�zast�rsat kihagyja
			{
				spouseSpouse = getLastFirst(&ss);
				if (parents.GetLength())
					parents += L", ";
				else
					parents = L" (";

				str.Format(L"%sf. %s", order, spouseSpouse);
				if (voltmar(order))
				{
					str = getColoredString(str, DE::PIROS);
				}
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
	return(parents);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::voltmar(CString index)
{
	for (int i = 0; i < vSpouseIndex.size(); ++i)
	{
		if (vSpouseIndex.at(i) == index)
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::print(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fl, L"%s\n", str);  // a soreleji %-okat printel�si karakterekenk �rtelmezn�, ha k�zvetlen�l nyomtatn�nk!!! 
	fflush(fl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::queryPeople(CString rowid, DE::PPEOPLE* p)
{
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowid);
	if (!queryP(m_command)) return;

	p->arm = rsP.GetFieldString(DBP_ARM);
	p->birth_date = rsP.GetFieldString(DBP_BIRTH_DATE);
	p->birth_place = rsP.GetFieldString(DBP_BIRTH_PLACE);
	p->comment = rsP.GetFieldString(DBP_COMMENT);
	p->csalad = rsP.GetFieldString(DBP_CSALAD);
	p->death_date = rsP.GetFieldString(DBP_DEATH_DATE);
	p->death_place = rsP.GetFieldString(DBP_DEATH_PLACE);
	p->father_id = rsP.GetFieldString(DBP_FATHER_ID);
	p->first_name = rsP.GetFieldString(DBP_FIRST_NAME);
	p->peer = rsP.GetFieldString(DBP_PEER);
	p->posterior = rsP.GetFieldString(DBP_POSTERIOR);
	p->folyt = rsP.GetFieldString(DBP_FOLYT);
	p->gap = rsP.GetFieldString(DBP_GAP);
	p->generation	= rsP.GetFieldString( DBP_GENERATION );
	p->last_name = rsP.GetFieldString(DBP_LAST_NAME);
	p->lineNumber = rsP.GetFieldString(DBP_LINENUMBER);
	p->mother_id = rsP.GetFieldString(DBP_MOTHER_ID);
	p->parentIndex = _wtoi(rsP.GetFieldString(DBP_PARENTINDEX));
	p->tableAncestry = rsP.GetFieldString(DBP_TABLEANCESTRY);
	p->occupation = rsP.GetFieldString(DBP_OCCUPATION);
	p->whichChildFather = rsP.GetFieldString(DBP_ORDERFATHER);
	p->whichChildMother = rsP.GetFieldString(DBP_ORDERMOTHER);
	p->other_names = rsP.GetFieldString(DBP_OTHER_NAMES);
	p->rowid = rsP.GetFieldString(DBP_ROWID);
	p->sex_id = rsP.GetFieldString(DBP_SEX_ID);
	p->source = rsP.GetFieldString(DBP_SOURCE);
	p->tableNumber = rsP.GetFieldString(DBP_TABLENUMBER);
	p->tableRoman = rsP.GetFieldString(DBP_TABLEROMAN);
	p->title = rsP.GetFieldString(DBP_TITLE);
	p->titolo = rsP.GetFieldString(DBP_TITOLO);
 	if (p_titololower && !p->titolo.IsEmpty() )
		p->titolo = konvTitolo(p->titolo);
	p->whichHusband = rsP.GetFieldString(DBP_WHICHHUSBAND);
	p->whichWife = rsP.GetFieldString(DBP_WHICHWIFE);

	p->fullname.Format(L"%s %s", p->last_name, p->first_name);

	if ((p->comment.Find(L"http")) != -1) p->comment.Empty();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::konvTitolo(CString titolo)
{
	TCHAR kar;

	for (int i = 0; i < titolo.GetLength(); ++i)
	{
		if (iswupper(titolo[i]))
		{
			kar = towlower(titolo[i]);
			titolo.SetAt(i, kar);
		}
	}
	return titolo;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getPlaceDateBlock(CString place, CString date, CString jel)
{

	CString block(L"");

	if (jel == L"+" && date == L"meghalt")
	{
		if (date.Left(4) < L"10")
			date.Empty();
	}

	if (!place.IsEmpty() || !date.IsEmpty())
	{
		block = jel;
		block = getColoredString(jel, p_specAttrib );
		if (!place.IsEmpty())
		{
			block += place;
			block += L" ";
		}
		if (!date.IsEmpty())
		{
			block += date;
		}
	}
	block.Trim();
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getLastFirst(DE::PPEOPLE* p) // h�zat�rs anyj�nak � stov�bbi h�zast�rsainak neve k�pz�dik �gy
{

	CString name = p->last_name;
	if (p_capitalName )
		name = convertNameToUpper(name);
	if (p_bold)
		name.Format( L"<b>%s</b>", name);
	if (!p->other_names.IsEmpty())
		name.Format(L"%s_(%s)", p->last_name, p->other_names);

	if (!p->first_name.IsEmpty() && !p->last_name.IsEmpty())
	{
		name += L" ";
		name += p->first_name;
			name = getColoredString(name, p_otherNameAttrib);
	}
	if (!p->peer.IsEmpty())
		name.Format(L"%s %s", p->peer, (CString)name);

	return name;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getColoredString(CString str, int index)
{
//	str.Trim();
	CString colored(str);
	if (!str.IsEmpty() && index)
		colored.Format(L"%s%s%s", DE::attrib[index].code1, str, DE::attrib[index].code2);
	return colored;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::openHtml(CString file, CString title, UINT colorBgrnd)
{
	CString dragonjpgPathName;
	dragonjpgPathName.Format(L"%s\\dragon.jpg", m_descendantsPath);
	CopyFile(theApp.m_dragonjpg, dragonjpgPathName, false);
	
//	if( theApp.v_rowid.size() > 1)
//		m_htmlPathName1.Format(L"%s\\%d_ember lesz�rmazotti t�bl�ja_%s.html", m_descendantsPath, theApp.v_rowid.size(), getTimeTag());
//	else

	m_htmlPathName1.Format(L"%s\\%s_%s.html", m_descendantsPath, file, getTimeTag());

	if (!openFileSpec(&fl, m_htmlPathName1, L"w+")) return false;


	int l = -37;
	CString today;
	today = getPresentDateTime();
	CString yesno;
	CString nok;
	if (p_womenDescendants)
		nok = L"igen";
	else
		nok = L"nem";

	CString maxGen;
	if (p_generationMax.IsEmpty())
		maxGen = L"minden gener�ci�";
	else
		maxGen = p_generationMax;

	CString inputFile;
	CString created;
	m_command = L"SELECT filespec, created FROM filespec";
	if (!theApp.query(m_command)) return false;
	inputFile = theApp.m_recordset->GetFieldString(0);
	created = theApp.m_recordset->GetFieldString(1);

	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	CString connect;
	connect = p_connect ? L"igen": L"nem";


	print(L"<!DOCTYPE html>");
	print(L"<html lang=hu>");
	print(L"<head>");
	str.Format(L"<title>%s</title>", today);
	print(str);
	print(L"<meta charset=\"UTF-8\">");
	createStyle();
	print(L"</head>");
	
//	CString head;
//	head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>\n";
//	print(head);

	CString body;
	body.Format(L"<body bgcolor=\"%06x\">", colorBgrnd);
//	body.Format(L"<body>\n" );
	print(body);



	//str.Format(L"<b><center>%s</center>\n<pre></b>\n", title);
	str.Format(L"<b><center>%s</center></b><br>", title);
	print(str);
	print(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>");

/*
	CString program;
	program.Format(L"Dragon v. %s", theApp.m_version);
	str.Format(L"<b><center>%s</center><pre></b>", program);
	str.Format(L"<b><center>%s</center></b>", program);
	print(str);
*/

	

	print(L"<br>\n<pre>\n");
	str.Format(L"%*s Dragon v. %s", l, L"Program:", theApp.m_version );
	print(str);
	str.Format(L"%*s %s", l, L"Dragon.exe k�sz�lt:", MultiToUnicode(LPCSTR(BUILD)));
	print(str);

	str.Format(L"%*s %s", l, L"Alapk�nyvt�r:", theApp.m_dbFolderPath);
	print(str);

	str = L"�RES";
	if (theApp.m_inputMode == MANUAL)
		str = L"k�zi adatbevitel";
	else if (theApp.m_inputMode == GEDCOM)
		str = L"GEDCOM f�jl";
	else if (theApp.m_inputMode = GAHTML)
		str = L"GA.htm f�jl";

	str.Format(L"%*s %s", l, L"Adatb�zis bemenete:", (CString)str);
	print(str);

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM )
	{
		str.Format(L"%*s %s %s", l, L"Bementi f�jl k�sz�lt:", created, inputFile );
		print(str);
	}
	str.Format(L"%*s %s %s", l, L"Adatb�zis k�sz�lt:", dateDB, theApp.m_dbFileName );
	print(str);
	if (!theApp.m_inputVersion.IsEmpty() && theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvas�s programverzi�ja:", theApp.m_inputVersion);
		print(str);
	}
	if (!theApp.m_uniteVersion.IsEmpty() )
	{
		if (theApp.m_uniteVersion != theApp.m_version)
		{
			str.Format(L"%*s %s", l, L"�sszevon�s programverzi�ja:", theApp.m_uniteVersion);
			print(str);
		}
	}
	CString rep;
	if (p_repeatedDelete)
		rep = L"t�r�lve";
	else
		rep = L"szerepelnek";


	str.Format(L"%*s %s<br>", l, L"Lista k�sz�lt:", theApp.getPresentDateTime());
	print(str);
	str.Format(L"%*s %s", l, L"Gener�ci�k max sz�ma:", maxGen);
	print(str);
	str.Format(L"%*s %s", l, L"El�gaz�sok �sszek�t�se:", connect);
	print(str);
	str.Format(L"%*s %s", l, L"N�k lesz�rmazottai:", nok);
	print(str);
	str.Format(L"%*s %s", l, L"Ism�tl�d� lesz�rmazottak:", rep);
	print(str);

//	str.Format(L"%*s %s", l, L"Ism�tl�d�k kihagy�sa:", kihagy);
//	print(str);

	CString sorrend;
	switch (p_childrenOrder)
	{
	case DE::ORDER_INPUT:
		sorrend = L"bementi sorrend";
		break;
	case DE::ORDER_BIRTH:
		sorrend = L"sz�let�si id�";
		break;
	}

	str.Format(L"%*s %s", l, L"Sorrend:", sorrend);
	print(str);

	if (p_repeatedColor)
	{
		str.Format(L"%*s %s", l, L"Ism�tl�d�k sz�nez�se:", L"z�ld - els� el�fordul�s, piros - �jabb el�fordul�s");
		print(str);
	}
	print(L"</pre>\n\n");


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::createStyle()
{

	fwprintf(fl, L"<style>\n");
	if (p_rowWidth != L"k�perny�" && p_rowWidth != L"0")
		fwprintf(fl, L"html{width:%scm;}\n", p_rowWidth);


	CString font;
	font = L"body {font-family: courier, serif;}";
	font = L"body {font-family: Arial, Helvetica, sans-serif;}";
	font = L"body {font-family: Verdana;}";
	font = L"<body class=\"w3-container\">";
	//	fwprintf( fl, L"%s\n", font );
	CString fontSize;
	fontSize.Format(L"body {font-size: %dpx;}", 10 + 2 * p_fontSize);
	fwprintf(fl, L"%s\n", fontSize);

	//	fwprintf( fl, L"body {font-size: 10px;}" );


	//	CString font;
	//	font = L"<font-family: Avenir Next, Calibri, Verdana, sans-serif>\n";
	//	fwprintf( fl, font );

	fwprintf(fl, L"ul {\n");
	fwprintf(fl, L"display: block;\n");
	fwprintf(fl, L"max-width: 200%;\n");
	//	fwprintf( fl, L"list-style-type: square;\n" );
	fwprintf(fl, L"list-style-type: none;\n");
	fwprintf(fl, L"margin-top: 0 em;\n");
	fwprintf(fl, L"margin-bottom: 0 em;\n");
	fwprintf(fl, L"margin-left: 0;\n");
	fwprintf(fl, L"margin-right: 0;\n");
	fwprintf(fl, L"padding-left: 40px;\n");
	fwprintf(fl, L"}\n");

	fwprintf(fl, L"ol {\n");
	fwprintf(fl, L"display: block;\n");
	fwprintf(fl, L"list-style-type: decimal;\n");
	fwprintf(fl, L"margin-top: 0 em;\n");
	fwprintf(fl, L"margin-bottom: 0 em;\n");
	fwprintf(fl, L"margin-left: 0;\n");
	fwprintf(fl, L"margin-right: 0;\n");
	fwprintf(fl, L"padding-left: 40px;\n");
	fwprintf(fl, L"}\n");

	fwprintf(fl, L"</style>\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::closeHtml()
{
//	for (int i = 0; i < m_indent; ++i)			// a kih�z�sok megsz�ntet�se, vissza az alapvonalra
//		fwprintf(fl, L"%s", m_tag2);

	fwprintf(fl, L"</body>\n");
	fwprintf(fl, L"</html>");
	int l = 46;

	if (m_actGen == -100)
	{
		str.Format(L"A gener�ci�k sz�ma el�rte az el��rt maximumot, ez�rt a list�z�s abbamaradt.<br><br>");
		print(str);
	}
	if (m_givup)
	{
		str.Format(L"A lesz�rmazottak sz�ma el�rte a %d-t, a list�z�s abbamaradt.<br><br>", m_maxDesc );
		print(str);
	}
	if( m_canceled )
	{
		str = L"A lesz�rmazottak list�z�sa megszak�tva.<br><br>";
		print(str);
	}
	int abc = 26;
	int g = m_gMax + 1;
	int cycle = g / abc;
	int bias = g % abc;
	TCHAR gen;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;


//	if ( theApp.v_tableNumbers.size() == 1   || !theApp.v_rowid.size() == 1 )
	{

		print(L"<pre>");
		str.Format(L"%-*s %8s(%c)", l, L"List�zott gener�ci�k sz�ma:", thousand(bias), gen);
		print(str);
		str.Format(L"%-*s %8s", l, L"List�zott lesz�rmazottak sz�ma:", thousand(m_listedD));
		print(str);
		str.Format(L"%-*s %8s", l, L"�sszes list�zott emberek sz�ma:", thousand(m_listedP));
		print(str);
		str.Format(L"%-*s %s", l, L"Fut�si id�:", theApp.getTimeElapsed(m_startTime));
		print(str);
		print(L"</pre>");
	}
	fclose(fl);
}
