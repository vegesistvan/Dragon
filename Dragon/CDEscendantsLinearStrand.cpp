#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include <algorithm>
#include "build_defs.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByNumOfD2(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.nOfD > d2.nOfD);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnDescendantLength()
{
	m_startTime = theApp.getStartTime();

	order_numOfD();

	descendants();

	openHtml();
	multipleRowid();
	printVector(0);
	closeHtml();

	ShellExecute(NULL, L"open", m_htmlPathName, NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::order_numOfD()
{
	int idC;
	int dbC;
	int id;
	int j;
	int k;
	int cnt;
	DE::DESC desc;
	std::vector<DE::DESC> vCH;

	for (int i = 0; i < vDesc->size(); ++i)
	{
		desc = vDesc->at(i);

		idC = desc.idC;
		dbC = desc.numOfChildren;

		if (dbC > 1)
		{
			j = idC;
			k = idC + dbC;
			std::sort(vDesc->begin() + idC, vDesc->begin() + k, sortByNumOfD2);
			cnt = 1;
			for (j = idC; j < k; ++j)
			{
				vDesc->at(j).id = j;
				vDesc->at(j).childorder = cnt;
				vDesc->at(j).idF = i;
				++cnt;

			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::descendants()
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
	int ixC; // gyerek index;
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

	for (int i = 0; i < vDesc->size(); ++i)
		vDesc->at(i).procChildren = 0;
	// Az �s berak�sa a vDesc vektorba

	desc = vDesc->at(0);

	m_rowid = desc.rowid;
	m_tablenumber = desc.tablenumber;
	vD.clear();
	vD.push_back(desc);
	m_os = desc.name;


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

		// visszamegy az utols� emberre, akinek m�g van fel nem dolgozott gyereke
		for (i = vD.size() - 1; i >= 0; --i)
		{
			numOfChildren = vD.at(i).numOfChildren;
			if (numOfChildren)
			{
				procChildren = vD.at(i).procChildren;
				if (numOfChildren != procChildren)	// m�g van fel nem dolgozott gyerek
					break;
			}
		}
		if (i < 0) 	break;	// v�ge van a programnak, nincs t�bb fel nem dolgozott gyerek!! Kil�p a ciklusb�l

		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		// sz�l� index: i;
		// gyerek index 
		ixC = vD.at(i).idC + procChildren;
		m_actGen = vD.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;

		desc = vDesc->at(ixC);
		desc.procChildren = 0;
		desc.order = 0;
		desc.cntRep = 0;
		desc.numOfRep = 0;
		desc.original = vD.size();
		desc.parentIndex = i;
		desc.parentSex = vD.at(i).sex;
		desc.g = vD.at(i).g + 1;		// apja gener�ci�j�n�l 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;
		desc.hidden = false;

		vD.at(i).procChildren += 1;		// a most feldolgozott testv�reinek sz�ma n� egyet
		desc.childorder = vD.at(i).procChildren;

		vD.push_back(desc);
		if (vD.size() == m_maxDesc)
		{
			str.Format(L"%d lesz�rmazottn�l feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vD.size();
			break;
		}
	}
	wndP.DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::openHtml()
{
	CString descendantsPath;
	CString dragonjpgPathName;
	CString file;
	file = L"descendantsLength";

	descendantsPath.Format(L"%s\\%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(descendantsPath, 0))
		_wmkdir(descendantsPath);
	dragonjpgPathName.Format(L"%s\\dragon.jpg", descendantsPath);
	CopyFile(theApp.m_dragonjpg, dragonjpgPathName, false);

	m_htmlPathName.Format(L"%s\\%s_lesz�rmazotti t�bl�ja_%s.html", descendantsPath, m_os, getTimeTag());

	if (!openFileSpec(&fhDescTable, m_htmlPathName, L"w+")) return false;

	int l = -37;
	CString today;
	today = getPresentDateTime();
	CString yesno;
	CString nok;
	if (p_womenDescendants)
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
	connect = p_connect ? L"igen" : L"nem";


	print(L"<!DOCTYPE html>");
	print(L"<html lang=hu>");
	print(L"<head>");
	str.Format(L"<title>%s</title>", today);
	print(str);
	print(L"<meta charset=\"UTF-8\">");
	createStyle();
	print(L"</head>");

	CString body;
	body.Format(L"<body bgcolor=\"%06x\">", p_colorBgrnd);
	print(body);



	str.Format(L"<b><center>%s lesz�rmazotti t�bl�ja</center></b><br>", m_os);
	print(str);
	print(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>");

	print(L"<br>\n<pre>\n");
	str.Format(L"%*s Dragon v. %s", l, L"Program:", theApp.m_version);
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

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM)
	{
		str.Format(L"%*s %s %s", l, L"Bementi f�jl k�sz�lt:", created, inputFile);
		print(str);
	}
	str.Format(L"%*s %s %s", l, L"Adatb�zis k�sz�lt:", dateDB, theApp.m_dbFileName);
	print(str);
	if (!theApp.m_inputVersion.IsEmpty() && theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvas�s programverzi�ja:", theApp.m_inputVersion);
		print(str);
	}
	if (!theApp.m_uniteVersion.IsEmpty())
	{
		if (theApp.m_uniteVersion != theApp.m_version)
		{
			str.Format(L"%*s %s", l, L"�sszevon�s programverzi�ja:", theApp.m_uniteVersion);
			print(str);
		}
	}

	str.Format(L"%*s %s<br>", l, L"Lista k�sz�lt:", theApp.getPresentDateTime());
	print(str);
	str.Format(L"%*s %s", l, L"Gener�ci�k max sz�ma:", maxGen);
	print(str);
	str.Format(L"%*s %s", l, L"El�gaz�sok �sszek�t�se:", connect);
	print(str);
	str.Format(L"%*s %s", l, L"N�k lesz�rmazottai:", nok);
	print(str);

	CString sorrend;
	sorrend = L"cs�kken� hossz�s�g� lesz�rmazotti sz�lak";
	str.Format(L"%*s %s", l, L"Sorrend:", sorrend);
	print(str);

	if (p_repeatedColor)
	{
		if (p_repeated == 0)
			str.Format(L"%*s %s", l, L"Ism�tl�d�k sz�nez�se:", L"z�ld - els� el�fordul�s, piros - �jabb el�fordul�s");
		else
			str.Format(L"%*s %s", l, L"Ism�tl�d�k sz�nez�se:", L"z�ld");
		print(str);
	}
	print(L"</pre>\n\n");


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::createStyle()
{

	print(L"<style>");
	if (p_rowWidth != L"k�perny�" && p_rowWidth != L"0")
	{
		str.Format(L"html{width:%scm;}", p_rowWidth);
		print(str);
	}


	CString font;
	font = L"body {font-family: courier, serif;}";
	font = L"body {font-family: Arial, Helvetica, sans-serif;}";
	font = L"body {font-family: Verdana;}";
	font = L"<body class=\"w3-container\">";
	//	fwprintf( fl, L"%s\n", font );
	CString fontSize;
	fontSize.Format(L"body {font-size: %dpx;}", 10 + 2 * p_fontSize);
	str.Format(L"%s", fontSize);
	print(str);

	//	fwprintf( fl, L"body {font-size: 10px;}" );


	//	CString font;
	//	font = L"<font-family: Avenir Next, Calibri, Verdana, sans-serif>\n";
	//	print( font );

	print(L"ul {");
	print(L"display: block;");
	print(L"max-width: 200%;");
	//	print( L"list-style-type: square;\n" );
	print(L"list-style-type: none;");
	print(L"margin-top: 0 em;");
	print(L"margin-bottom: 0 em;");
	print(L"margin-left: 0;");
	print(L"margin-right: 0;");
	print(L"padding-left: 40px;");
	print(L"}");

	print(L"ol {");
	print(L"display: block;");
	print(L"list-style-type: decimal;");
	print(L"margin-top: 0 em;");
	print(L"margin-bottom: 0 em;");
	print(L"margin-left: 0;");
	print(L"margin-right: 0;");
	print(L"padding-left: 40px;");
	print(L"}");
	print(L"</style>");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::closeHtml()
{
	print(L"</body>");
	print(L"</html>");
	int l = 46;

	if (m_actGen == -100)
	{
		str.Format(L"A gener�ci�k sz�ma el�rte az el��rt maximumot, ez�rt a list�z�s abbamaradt.<br><br>");
		print(str);
	}
	if (m_givup)
	{
		str.Format(L"A lesz�rmazottak sz�ma el�rte a %d-t, a list�z�s abbamaradt.<br><br>", m_maxDesc);
		print(str);
	}
	if (m_canceled)
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
		if (p_repeated)
		{
			str.Format(L"%-*s %8s", l, L"Ism�tl�d�, nem list�zott lesz�rmazottak sz�ma:", thousand(m_cntRepeated));
			print(str);
		}
		str.Format(L"%-*s %8s", l, L"�sszes list�zott emberek sz�ma:", thousand(m_listedP));
		print(str);
		str.Format(L"%-*s %s", l, L"Fut�si id�:", theApp.getTimeElapsed(m_startTime));
		print(str);
		print(L"</pre>");
	}
	fclose(fhDescTable);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::printVector(int tbl)
{
	//	CString tableheader;
	//	CString tableNumber;
	CString people;
	CString line;
	DE::DESC desc;
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
	str.Format(L"%s lesz�rmazottainak tabul�lt list�z�sa...", vD.at(0).name);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vD.size());
	wndP.SetPos(0);

	for (int i = 0; i < vD.size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		desc = vD.at(i);

		if (vD.at(i).hidden) continue;   // apa bejegyz�se, aki m�s t�bl�ban szerepel
		if (p_repeated && vD.at(i).status == 2)
			continue;

		queryPeople(vD.at(i).rowid, &p);

		if (!p_womenDescendants && vD.at(i).parentSex == WOMAN) continue;

		printBegining(i);	// html k�dok �s gener�ci� elk�sz�t�se; 
		people = getComplexDescriptionL1(i, true);

		line.Format(L"%s%s", m_diamond, people);
		line.Replace('|', '\'');
		print(line);
		m_genPrev = vD.at(i).g;
	}

	for (int i = 0; i < m_indent; ++i)			// a kih�z�sok megsz�ntet�se, vissza az alapvonalra
	{
		fwprintf(fhDescTable, m_tag2);
		print(str);
	}
	wndP.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genr�ci� v�ltoz�st�l f�gg� behuz�s �s genr�ci�s k�d nyomtat�sa az m_sytax �rt�k�t�l f�gg�en
void CDescendantsLinearTable::printBegining(int i)
{
	CString family;
	CString shift;
	int z;
	int g = vD.at(i).g;

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

	gen = get_gABC(g);

	// Kiemelt csal�dn�v //////////////////////////////////////
	if (p_descendantName == DE::RAISED)
	{
		if (m_familyName != p.last_name)
		{
			family = getTableHeader();
			str = getColoredString(family, DE::KEK);
			if (p_repeatedColor)
			{
				switch (vDesc->at(i).status)
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
		str.Format(L"%s<li>%c%d&diams;", shift, gen, vDesc->at(i).childorder);
	else if (p_numberingSystem == DE::TUP)
	{
		z = vSerial.size() - 1;
		if (g < z)
			++vSerial.at(g);
		else
			vSerial.push_back(1);
		str.Format(L"%s<li>%c-%d&diams;", shift, gen, vSerial.at(g));
	}
	m_diamond = str;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getComplexDescriptionL1(int i, bool parentIndex)
{
	DE::DESC desc = vD.at(i);
	queryPeople(desc.rowid, &p);
	CString people = createDescendantL1(i, parentIndex);
	CString spouses = createSpousesL1(i);
	CString arm;
	CString csalad;
	CString folyt;


	people.Trim();
	people += L" ";
	people += spouses.Trim();
	people.Trim();

	if (!p.arm.IsEmpty())
	{
		people += p.arm;
	}
	if (!p.csalad.IsEmpty())
	{
		people += p.csalad;
	}
	return people;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createDescendantL1(int i, bool parentIndex)
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
		if (p_capital)
			lastname = convertNameToUpper(lastname);

		if (lastname == L"N;")
			name.Format(L"\n%s %s %s", lastname, p.first_name, p.peer);
		else
			name.Format(L"\n%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		name.Trim();
		if (!p.other_names.IsEmpty())
			name.Format(L"\n%s_(%s)", (CString)name, p.other_names);
	}
	name = getColoredString(name, p_descendantAttrib);

	if (i && vD.at(vD.at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apj�nak t�bb feles�ge volt
	{
		last = vLMX.size() - 1;
		if (last == -1 || vLMX.at(last).g < vD.at(i).g)		// magasabb gener�ci�ban van
		{
			lmx.g = vD.at(i).g;
			lmx.lastMotherIndex = p.parentIndex - 1;
			vLMX.push_back(lmx);
		}
		else													// alacsonyabb gener�ci�ban van
		{
			// megkeresi saj�t gener�ci�j�nak utolj�ra list�zott motherIndex-�t
			j = vLMX.size() - 1;
			for (j = vLMX.size() - 1; j > 0; --j)
			{
				if (vLMX.at(j).g > vD.at(i).g) // a nagyobb gener�ci�k eldob�sa
					vLMX.pop_back();
			}
		}
		last = vLMX.size() - 1;


		// kitegye az esetleg l�tez� parentindexet
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

	if (p.comment.GetAt(0) != ',')
		name += L" ";

	if (!p.posterior.IsEmpty())
	{
		name += p.posterior;
	}


	str = getPlaceDateBlock(p.birth_place, p.birth_date, L"*");
	line = name.Trim();
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

	comment = getColoredString(str, p_commentAttrib);
	comment.Trim();
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	// ha sz�nezni akarjuk, akkor a t�rlend� piros, a megtartand� k�k
	if (p_repeatedColor)
	{
		switch (vD.at(i).status)
		{
		case 1:
			line = getColoredString(line, DE::ZOLD);
			break;
		case 2:
			line = getColoredString(line, DE::PIROS);
			break;
		}
	}
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	++m_listedP;
	++m_listedD;
	return(line);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createSpousesL1(int i)
{
	CString rowid = vD.at(i).rowid;
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
			marriage = createMarriageL(place, date, i, numberOfSpouses);
			spouse = createSpouseL1();				// ki�r�s el�tt ellen�rzi, hogy szerepel-e ilyen n�vem m�r ki�rt h�zast�rs. Ha igen, akkor sz�nezi
			vFullname.push_back(s.fullname);	// vFullname-en gy�jti a m�r kilist�zott h�zast�rsak nev�t
			spRelatives = createSpRelativesL1();
			if (!marriage.IsEmpty())
				spouses += marriage;
			if (!spouse.IsEmpty())
				spouses += spouse;
			if (!spRelatives.IsEmpty())
				spouses += spRelatives;
		}
	}
	spouses.Trim();
	return spouses;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs list�zados blokkj�t elk�sz�ti
CString CDescendantsLinearTable::createSpouseL1()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if (p_capital)
		lastname = convertNameToUpper(lastname);
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
		//		if (p_checkCRLF)
		//			fullname += L"\n";
	}
	fullname.Trim();
	fullname = getColoredString(fullname, p_otherNameAttrib);
	spouse = fullname;

	//	m_birth = s.birth_date;
	str = getPlaceDateBlock(s.birth_place, s.birth_date, L"*");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	str = getPlaceDateBlock(s.death_place, s.death_date, L"+");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	//str = getColoredString(s.comment, p_commentAttrib);
	str = s.comment;
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
CString CDescendantsLinearTable::createSpRelativesL1()
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
		father = getColoredString(father, p_otherNameAttrib);
	}
	if (!s.mother_id.IsEmpty() && s.mother_id != L"0")
	{
		queryPeople(s.mother_id, &sm);
		mother = getLastFirst(&sm, false);
		mother.Trim();
		mother = getColoredString(mother, p_otherNameAttrib);
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
				spouseSpouse = getLastFirst(&ss, false);

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
	return(parents);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::multipleRowid()
{
	CString rowid;
	int g;
	int cnt;
	// az ism�tl�d�ket megjel�li:  az els� el�fordul�st 1-el, a k�vetkez�ket -1-el
	str.Format(L"Ism�tl�d� lesz�rmazottak jel�l�se...");
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vD.size());
	wndP.SetPos(0);

	for (int i = 0; i < vD.size(); ++i)
	{
		if (vD.at(i).status == 0)
		{
			rowid = vD.at(i).rowid;
			cnt = 1;
			for (int j = i + 1; j < vD.size(); ++j)
			{
				if (vD.at(j).rowid == rowid)
				{
					vD.at(i).status = 1;		// els� el�fordul�s
					vD.at(j).status = 2;		// tov�bbi el�fordul�s

					vD.at(i).cntRep = 1;
					++cnt;
					vD.at(j).cntRep = cnt;
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	if (p_repeated)   // apa lesz�rmazottait tartja meg
	{
		str.Format(L"Ism�tl�d� lesz�rmazottak jel�l�se...");
		CProgressWnd wndP(NULL, str);
		wndP.GoModal();
		wndP.SetRange(0, vD.size());
		wndP.SetPos(0);
		for (int i = 0; i < vD.size();)
		{
			if (vD.at(i).status == 0)  // ha nem ism�tl�d�, akkor elfogadja list�zand�nak
			{
				for (i; i < vD.size() && vD.at(i).status == 0; ++i);
			}
			else if (vD.at(i).status == 1)		// ism�tl�d� els� el�fordul�sa, aki list�zand�
			{
				if (vD.at(i).parentSex == WOMAN)	// ha n�, akkor nem list�zand� lesz
				{
					for (i; i < vD.size() && vD.at(i).status == 1; ++i)
						vD.at(i).status = 2;
				}
				else							// ha f�rfi, akkor elfogadja list�zand�nak
					for (i; i < vD.size() && vD.at(i).status == 1; ++i);
			}
			else if (vD.at(i).status == 2) // ism�tl�d� m�sodik el�fordul�sa, aki kihagy�sra van kijel�lve
			{
				if (vD.at(i).parentSex == MAN)	// ha f�rfi, akkor list�zand� lesz
				{
					for (i; i < vD.size() && vD.at(i).status == 2; ++i)
						vD.at(i).status = 1;
				}
			}
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;

		}
	}
}
