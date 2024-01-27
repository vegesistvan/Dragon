#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include "Table_people_columns.h"
#include "Table_tables.h"
#include "build_defs.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBynOfD1(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.nOfD < d2.nOfD);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortById1(const DE::DESC& d1, const DE::DESC& d2 )
{
	return(d1.id < d2.id );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnHtmlListaPrintable()
{
	createHtmlFile( PRINTABLE, REPEATED);
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnHtmlListaFix()
{
	createHtmlFile( NOTPRINTABLE, REPEATED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnHtmllistFixNorep()
{
	createHtmlFile( NOTPRINTABLE, NOTREPEATED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnHtmllistPrNorep()
{
	createHtmlFile( PRINTABLE, NOTREPEATED);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::createHtmlFile(bool printable, bool repeated)
{
	CString filename;
	CString title;
	CString familyName;
	CString tableRoman;
	CString descendantsTablePathName;
	CString printableTablePathName;
	int tableTop = 240;
	int height = theApp._h - tableTop;			// 612   teljes: 1080  378  levonjuk a preTable-t
	int width = theApp._w;

	m_familyName.Empty();
	m_cnt = 0;
	genPrev.Empty();

	if (theApp.v_tableNumbers.size())
	{

		// ennek utána kell nézni, milyen tablenumber-re kérdezzen le
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(0));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableRoman = rs.GetFieldString(2);
		if (printable)
		{
			if (repeated)
				title.Format(L"%s %s nyomtatható lineáris leszármazotti tábla ismétlésekkel", familyName, tableRoman);
			else
				title.Format(L"%s %s nyomtatható lineáris leszármazotti tábla ismétlések nélkül", familyName, tableRoman);
		}
		else
		{
			if (repeated)
				title.Format(L"%s %s lineáris leszármazotti tábla ismétlésekkel", familyName, tableRoman);
			else
				title.Format(L"%s %s lineáris leszármazotti tábla ismétlések nélkül", familyName, tableRoman);
		}
	}
	else
	{
		if (printable)
		{
			if (repeated)
				title.Format(L"%s nyomtatható lineáris leszármazotti tábla ismétlésekkel", m_name);
			else
				title.Format(L"%s nyomtatható lineáris leszármazotti tábla ismétlések nélkül", m_name);
		}
		else
		{
			if (repeated)
				title.Format(L"%s lineáris leszármazotti táblája ismétlésekkel", m_name);
			else
				title.Format(L"%s lineáris leszármazotti táblája ismétlések nélkül", m_name);
		}
	}
	descendantsTablePathName.Format(L"%s\\%s_%s.html", m_descendantsPath, title, getTimeTag());
	if (!openFileSpec(&fhDescTable, descendantsTablePathName, L"w+")) return false;

	print(L"<!DOCTYPE html>");
	print(L"<html xml:lang=\"en\"><head>");
	str.Format(L"<title>%s</title>", title);
	print(str);
	print(L"<meta charset=UTF-8>");
	print(L"<meta http-equiv=\"language\" content=\"en-us\"/>");

	print(L"<script type=\"text/javascript\">");
	print(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">");

	print(L"function stripedTable() {");
	print(L"	if (document.getElementById && document.getElementsByTagName) {");
	print(L"		var allTables = document.getElementsByTagName('table');");
	print(L"		if (!allTables) { return; }\n");

	print(L"		for (var i = 0; i < allTables.length; i++) {");
	print(L"			if (allTables[i].className.match(/[\\w\\s ]*scrollTable[\\w\\s ])) {");
	print(L"				var trs = allTables[i].getElementsByTagName(\"tr\");");

	print(L"				for (var j = 0; j < trs.length; j++) {");
	print(L"					removeClassName(trs[j], 'alternateRow');");
	print(L"					addCSSClass(trs[j], 'normalRow');");
	print(L"				}");
	print(L"				for (var k = 0; k < trs.length; k += 2) {");
	print(L"					removeClassName(trs[k], 'normalRow');");
	print(L"					addCSSClass(trs[k], 'alternateRow');");
	print(L"				}");
	print(L"			}");
	print(L"		}");
	print(L"	}");
	print(L"}\n");

	print(L"window.onload = function() { stripedTable(); }");
	print(L"</script>");

	print(L"<style type=\"text/css\">");

	print(L"/* define height and width of scrollable area. Add 16px to width for scrollbar*/");
	print(L"div.tableContainer {");
	print(L"	clear: both;");
	print(L"	border: 1px solid #963;");
	print(L"	overflow: auto;");
	print(L"	width: device-width");
	print(L"}\n");


	print(L"/* Reset overflow value to hidden for all non-IE browsers. */");
	print(L"html>body div.tableContainer {");
	print(L"	overflow: hidden;");
	print(L"}\n");

	print(L"/* set table header to a fixed position. WinIE 6.x only                                       */");
	print(L"/* In WinIE 6.x, any element with a position property set to relative and is a child of       */");
	print(L"/* an element that has an overflow property set, the relative value translates into fixed.    */");
	print(L"/* Ex: parent element DIV with a class of tableContainer has an overflow property set to auto */");

	print(L"thead.fixedHeader tr {");
	print(L"	position: relative");
	print(L"}\n");

	print(L"/* set THEAD element to have block level attributes. All other non-IE browsers            */");
	print(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */");

	print(L"html>body thead.fixedHeader tr {");
	print(L"	display: block");
	print(L"}\n");

	print(L"/* make the TH elements pretty */");
	/* A táblafejléce barna lesz */
	print(L"thead.fixedHeader th {");
	print(L"	color: black;");
	print(L"	background: AntiqueWhite;");
	print(L"	border-left: 1px solid #EB8;");
	print(L"	border-right: 1px solid #B74;");
	print(L"	border-top: 1px solid #EB8;");
	print(L"	border-bottom: 1px solid #EB8;");
	print(L"	font-weight: bold;");
	print(L"	padding: 4px 3px;");
	print(L"	text-align: center");
	print(L"}\n");
	/* a tábla kerete barna lesz */
	print(L"/* make the A elements pretty. makes for nice clickable headers                */");
	print(L"thead.fixedHeader a, thead.fixedHeader a:link, thead.fixedHeader a:visited {");
	print(L"	display: block;");
	print(L"	text-decoration: none;");
	print(L"	width: 100%");
	print(L"}\n");


	print(L"thead.fixedHeader a:hover{");
	print(L"	display: block;");
	print(L"	text-decoration: underline;");
	print(L"	width: 100%");
	print(L"}\n");

	print(L"/* define the table content to be scrollable                                              */");
	print(L"/* set TBODY element to have block level attributes. All other non-IE browsers            */");
	print(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */");
	print(L"/* induced side effect is that child TDs no longer accept width: auto                     */");

	print(L"html>body tbody.scrollContent {");
	print(L"	display: block;");

	if (!printable)  // vagyis fix fejléc, nem nyomtatható
	{
		str.Format(L"    height: %dpx;", height);
		print(str);
	}

	print(L"	overflow: auto;");
	print(L"	width: 100%");
	print(L"}\n");


	print(L"/* make TD elements pretty. Provide alternating classes for striping the table */");
	print(L"/* http://www.alistapart.com/articles/zebratables/                             */");

	/* A tálázat belsõ vonalai */
	print(L"tbody.scrollContent td, tbody.scrollContent tr.normalRow td {");
	print(L"	border-bottom: none;");
	print(L"	border-left: none;");
	print(L"	border-right: 1px solid #CCC;");
	print(L"	border-top: 1px solid #DDD;");
	print(L"	padding: 2px 3px 3px 4px");
	print(L"}\n");

	print(L"tbody.scrollContent tr.alternateRow td {");
	print(L"	border-bottom: none;");
	print(L"	border-left: none;");
	print(L"	border-right: 1px solid #CCC;");
	print(L"	border-top: 1px solid #DDD;");
	print(L"	padding: 2px 3px 3px 4px");
	print(L"}\n");

	print(L"/* define width of TD elements: 1st, 2nd, and 3rd respectively.          */");
	print(L"/* All other non-IE browsers.                                            */");
	print(L"/* http://www.w3.org/TR/REC-CSS2/selector.html#adjacent-selectors        */");

	if (repeated)
	{
		print(L"html>body tbody.scrollContent td {");
		print(L"	width: 45px");
		print(L"}\n");

		print(L"html>body tbody.scrollContent td + td {");
		print(L"	width: 20px");
		print(L"}\n");

		print(L"html>body tbody.scrollContent td + td + td {");
		print(L"	width: 35px");
		print(L"}\n");

		print(L"html>body tbody.scrollContent td + td + td + td {");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td{");
		print(L"	width: 20px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td + td{");
		str.Format(L"width: %dpx", width - (45 + 4 * 55 + 35 + +2 * 20 + 120));
		print(str);
		print(L"}");
	}
	else
	{
		print(L"html>body tbody.scrollContent td {");
		print(L"	width: 45px");
		print(L"}\n");

		print(L"html>body tbody.scrollContent td + td {");
		print(L"	width: 35px");
		print(L"}\n");

		print(L"html>body tbody.scrollContent td + td + td {");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td{");
		print(L"	width: 20px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td + td{");
		print(L"	width: 55px");
		print(L"}");

		print(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td{");
		str.Format(L"width: %dpx", width - (45 + 4 * 55 + 35 + + 20 + 120));
		print(str);
		print(L"}");
	}

	print(L"</style>");

	print(L"</head><body>\n");

	printTopContainer(title);
	print(L"<br><br>");

	print(L"<div id=\"tableContainer\" class=\"tableContainer\">");
	print(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">");
	print(L"<thead class=\"fixedHeader\">");
	print(L"<tr>");

	print(L"<th style=\"width: 45px\">desc</th>");
	if( repeated )
		print(L"<th style=\"width: 20px\">I</th>");
	print(L"<th style=\"width: 35px\">id</th>");
	print(L"<th style=\"width: 55px\">gyerek<br>id</th>");
	print(L"<th style=\"width: 55px\">szülõ<br>id</th>");
	print(L"<th style=\"width: 20px\">A</th>");
	print(L"<th style=\"width: 55px\">gyerek<br>db</th>");
	print(L"<th style=\"width: 55px\">generáció<br>-sorszám</th>");
	str.Format(L"<th style=\"width: %dpx\">leszármazott</th>", width - (4 * 55 + 35 + 2*20 ));
	print(str);

	print(L"</tr>");
	print(L"</thead>");
	print(L"<tbody class=\"scrollContent\">");

	dataTable(repeated);

	print(L"</tbody>");
	print(L"</table>");
	print(L"</div>\n");

	print(L"<div><br/ ></div>\n");
	print(L"</body></html>\n");



	fclose(fhDescTable);
	ShellExecute(NULL, L"open", descendantsTablePathName, NULL, NULL, SW_SHOWNORMAL);
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::dataTable( bool repeated)
{
	DE::DESC desc;
	CString rowidF;
	CString rowid;
	CString people;
	CString tableheader;
	CString tableNumber;
	CString namePrev;
	CString name;
	CString family;
	CString	numOfChildren;
	CString numOfSib;
	CString gen;
	CString rep;
	int temp;

	int gcnt = 0;
	
	m_listedP = 0;
	m_listedD = 0;
	m_indent = 0;
/*
	vLMX.clear();		// a generáció utolsó kiírt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);
	str.Format(L"%s leszármazottainak listázása...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc->size());
	wndP.SetPos(0);

	// Az id kitöltése, hogy a tényleges listában meh tudja adni a gyerek id-jét
	for (int i = 0; i < vDesc->size(); ++i)
	{
		vDesc->at(i).printed = false;
		vDesc->at(i).id = -1;
	}
	std::vector<DE::DESC> vD1;
	std::vector<DE::DESC> vD2;
	int cnt;
	int g = 1;

	for (int i = 0; i < vDesc->size(); ++i)
	{
		desc = vDesc->at(i);
		if (desc.hidden || (p_repeated != 0 && desc.status == 2))
		{
			vDesc->at(i).printed = true;
			vDesc->at(i).id = -1;
		}
	}


	cnt = 0;
	vD1.clear();
	vDesc->at(0).id = cnt;
	++cnt;
	vDesc->at(0).printed = true;
	vD1.push_back(vDesc->at(0));

	// vD1-k már ki vannak írva, a gyerekeiken kell végigmenni, akiket vD2-be gyûjtünk
	while( true )
	{
		vD2.clear();
		for (int i = 0; i < vD1.size(); ++i)
		{
			desc = vD1.at(i);
			rowidF = vD1.at(i).rowid;		
			for (int j = 0; j < desc.numOfChildren; ++j)
			{
				// gyerekeiket keressük
				for (int k = i+1; k < vDesc->size(); ++k)
				{
					if (vDesc->at(k).printed) continue;
					if (vDesc->at(k).rowidF == rowidF)
					{
						vDesc->at(k).printed = true;
						vDesc->at(k).id = cnt;		// saját id;
						++cnt;
						vD2.push_back(vDesc->at(k));
					}
				}
			}
		}
		vD1 = vD2;
		if (!vD1.size()) break;
	}
*/
	str.Format(L"%s leszármazottainak listázása...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc->size());
	wndP.SetPos(0);

	std::sort(vDesc->begin(), vDesc->end(), sortById1 );
	for (int i = 0; i < vDesc->size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		desc = vDesc->at(i);
		if (desc.id < 0) continue;

		printDesc(i, repeated);
	}

/*
	int gPrev = -1;
	int ordPrev = 0;
	int ix1 = 0;		// rendezendõ bejegyzések elsõ elemének indexe
	int ix2;			// rendezendõ bejegyzések utolsó elemének indexe
	for (int i = 0; i > vDesc->size(); ++i)
	{
		if (vDesc->at(i).g != gPrev || vDesc->at(i).childorder != ordPrev + 1)
		{
			ix2 = i;
			if (ix2 > ix1)
			{
				std::sort(vDesc->begin() + ix1, vDesc->begin() + ix2, sortBynOfD1);
			}
			ix1 = i + 1;
			gPrev = vDesc->at(i).g;
			ordPrev = vDesc->at(i).childorder;
		}
	}
*/
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::printDesc( int i, bool repeated  )
{
	DE::DESC desc = vDesc->at(i);

	CString people;
	CString name;
	CString family;

	CString idD;
	CString idC;
	CString idF;
	CString ixM;
	CString	numOfChildren;
	CString gen;
	CString rep;
	
	int numOfSpouses = 1;
	int pos;

	int gcnt = 0;
	
	vLMX.clear();		// a generáció utolsó kiírt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);

	queryPeople(desc.rowid, &p);
	if (!p_womenDescendants && desc.parentSex == WOMAN) return;

	people = getComplexDescription(i, false );

	idC.Empty();
	if (desc.idC > 0)
		idC.Format(L"%d", desc.idC);

	idF.Empty();
	if (desc.idF > 0)
		idF.Format(L"%d", desc.idF);


	gen = get_gABC(desc.g);
	idD.Format(L"%3s-%-4d", gen, desc.childorder);
	if (desc.clrText)
		idD = getColoredString(idD, DE::KEK);
	else
		idD = getColoredString(idD, DE::PIROS);


	numOfChildren.Empty();
	if (desc.numOfChildren > 0)
		numOfChildren.Format(L"%d", desc.numOfChildren);

	rep.Empty();
	if (desc.cntRep)
		rep.Format(L"%d", desc.cntRep);

	if (p_descendantName == DE::RAISED && m_familyName != desc.lastname )  // kiemelt családnév
	{
		str = getSeededName(i);
		family = getColoredString(str, DE::KEK);

		str.Format(L"<td>%s</td>", L"");
		print(L"<tr>");
		print(str);
		if( repeated )
			print(str);
		print(str);
		print(str);
		print(str);
		print(str);
		print(str);
		print(str);
		name.Format(L"<td>%s</td>", family);
		print(name);
		print(L"</tr>");
	}
	m_familyName = desc.lastname;


	if ( gen != genPrev)
		gflag = !gflag;

	if (gflag)
		print(L"<tr style=\"background-color: #fff;\">");
	else
		print(L"<tr style=\"background-color: #eee;\">");

	str.Format(L"<td><center>%d</center></td>", desc.nOfD);
	print(str);
	if (repeated)
	{
		str.Format(L"<td><center>%s</center></td>", rep);
		print(str);
	}
	str.Format(L"<td><center>%d</center></td>", desc.id);
	print(str);
	str.Format(L"<td><center>%s</center></td>", idC);
	print(str);
	str.Format(L"<td><center>%s</center></td>", idF);
	print(str);
	str.Format(L"<td><center>%s</center></td>", desc.ixM );
	print(str);
	str.Format(L"<td><center>%s</center></td>", numOfChildren);
	print(str);
	str.Format(L"<td><center>%s</center></td>", idD);
	print(str);

	str.Format(L"<td>%s</td>", people);
	print(str);
	print(L"</tr>");

//	++m_cnt;
	vDesc->at(i).printed = true;
	genPrev = gen;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getComplexDescription(int i, bool parentIndex )
{
	CString people = createDescendant(i, parentIndex );
	CString spouses = createSpouses(i);
	CString arm;
	CString csalad;
	CString folyt;

	people.Trim();
	people += L" ";
	people += spouses;
	people.Trim();

	if (!p.arm.IsEmpty())
	{
		arm.Format(L" <font color='blue'>[%s]</font>", p.arm);
		people += arm;
	}
	if (!p.csalad.IsEmpty())
	{
		csalad.Format(L" <font color='blue'>[%s]</font>", p.csalad);
		people += csalad;
	}
/*
	if (!p.folyt.IsEmpty() && p_folyt )
	{
		folyt.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		people += folyt;
	}
*/
	return people;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR CDescendantsLinearTable::get_gABC(int g)
{
	TCHAR gABC;
	int ix = g / 26;		  // ix = 0, ha g kisebb mint 26, = 1 Ha nagyobb
	gABC = TCHAR('A') + ix * 6 + g;   // 6 : a 'Z' és 'a' közötti karakterek száma,
	return gABC;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::printTopContainer(CString title)
{
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
/*
	switch (p_repeated)
	{
	case 0:
		kihagy = L"Nem hagyja aki az ismétlõdõ leszármazottakat.";
		break;
	case 1:
		kihagy = L"Az elsõ leszármazottat kiírja, a többit elhagyja.";
		break;
	case 2:
		kihagy = L"Ha az apja leszármazott, akkor kiírja, ha az anyja, akkor nem";
	}
*/
	CString maxGen;
//	if (p_generationMax.IsEmpty())
//		maxGen = L"minden generáció";
//	else
//		maxGen = p_generationMax;

	CString inputFile;
	CString created;
	m_command = L"SELECT filespec, created FROM filespec";
	if (!theApp.query(m_command)) return false;
	inputFile = theApp.m_recordset->GetFieldString(0);
	created = theApp.m_recordset->GetFieldString(1);

	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	CString connect;
	connect = p_connect ? L"igen" : L"nem";


	//str.Format(L"<b><center>%s</center>\n<pre></b>\n", title);
	str.Format(L"<b><center>%s</center></b><br>\n\n", title);
	print(str);
	
	print(L"<pre>");
	str.Format(L"%*s Dragon v. %s", l, L"Program:", theApp.m_version);
	print(str);
	str.Format(L"%*s %s", l, L"Dragon.exe készült:", MultiToUnicode(LPCSTR(BUILD)));
	print(str);

	str.Format(L"%*s %s", l, L"Alapkönyvtár:", theApp.m_dbFolderPath);
	print(str);

	str = L"ÜRES";
	if (theApp.m_inputMode == MANUAL)
		str = L"kézi adatbevitel";
	else if (theApp.m_inputMode == GEDCOM)
		str = L"GEDCOM fájl";
	else if (theApp.m_inputMode = GAHTML)
		str = L"GA.htm fájl";

	str.Format(L"%*s %s", l, L"Adatbázis bemenete:", (CString)str);
	print(str);

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM)
	{
		str.Format(L"%*s %s %s", l, L"Bementi fájl készült:", created, inputFile);
		print(str);
	}
	str.Format(L"%*s %s %s", l, L"Adatbázis készült:", dateDB, theApp.m_dbFileName);
	print(str);
	//	if (!theApp.m_inputVersion.IsEmpty() && theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvasás programverziója:", theApp.m_inputVersion);
		print(str);
	}

	//	if (theApp.m_uniteVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Összevonás programverziója:", theApp.m_uniteVersion);
		print(str);
	}

	str.Format(L"%*s %s<br>", l, L"Lista készült:", theApp.getPresentDateTime());
	print(str);
	str.Format(L"%*s %s", l, L"Generációk max száma:", maxGen);
	print(str);
	str.Format(L"%*s %s", l, L"Elágazások összekötése:", connect);
	print(str);
	str.Format(L"%*s %s", l, L"Nõk leszármazottai:", nok);
	print(str);

	//	str.Format(L"%*s %s", l, L"Ismétlõdõk kihagyása:", kihagy);
	//	print(str);

	CString sorrend;
	switch (p_childrenOrder)
	{
	case DE::ORDER_INPUT:
		sorrend = L"bementi sorrend";
		break;
	case DE::ORDER_BIRTH:
		sorrend = L"születési idõ";
		break;
	case DE::ORDER_INCREASING:
		sorrend = L"növekvõ hosszúságú leszármazotti szálak";
		break;
	case DE::ORDER_DECREASING:
		sorrend = L"csökkenõ hosszúságú leszármazotti szálak";
		break;
	}
	str.Format(L"%*s %s", l, L"Sorrend:", sorrend);
	print(str);
	str.Format(L"<br>%*s desc - a bejegyzés leszármazottainak száma", l, L"Oszlopok:");
	print(str);
	str.Format(L"%*s I - ismétlõdõ leszármazottak sorszáma", l, L" ");
	print(str);
	str.Format(L"%*s id - a bejegyzés azonosítója", l, L" ");
	print(str);
	str.Format(L"%*s A - anya index", l, L" ");
	print(str);

	print(L"</pre>");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDescendantsLinearTable::sort_vDesc()
{
	int gPrev = -1;
	int ordPrev = 0;
	int ix1 = 0;		// rendezendõ bejegyzések elsõ elemének indexe
	int ix2;			// rendezendõ bejegyzések utolsó elemének indexe
	for (int i = 0; i > vDesc->size(); ++i)
	{
		if (vDesc->at(i).g != gPrev || vDesc->at(i).childorder != ordPrev + 1)
		{
			ix2 = i;
			if (ix2 > ix1)
			{
				std::sort(vDesc->begin() + ix1, vDesc->begin()+ ix2, sortBynOfD1);
			}
			ix1 = i + 1;
			gPrev = vDesc->at(i).g;
			ordPrev = vDesc->at(i).childorder;
		}
	}
	return ix1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsLinearTable::query(CString command)
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
BOOL CDescendantsLinearTable::queryP(CString command)
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
BOOL CDescendantsLinearTable::queryM(CString command)
{
	if (rsM.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, rsM.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::queryPeople(CString rowid, DE::PPEOPLE* p)
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
	p->generation = rsP.GetFieldString(DBP_GENERATION);
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
	p->whichHusband = rsP.GetFieldString(DBP_WHICHHUSBAND);
	p->whichWife = rsP.GetFieldString(DBP_WHICHWIFE);

	p->fullname.Format(L"%s %s", p->last_name, p->first_name);

	if ((p->comment.Find(L"http")) != -1) p->comment.Empty();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getPlaceDateBlock(CString place, CString date, CString jel)
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
		block = getColoredString(jel, p_specAttrib);
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
CString CDescendantsLinearTable::getLastFirst(DE::PPEOPLE* p, bool style ) // házatárs anyjának é stovábbi házastársainak neve képzõdik így
{

	CString name = p->last_name;
	if (p_capital)
		name = convertNameToUpper(name);
	if (p_bold)
		name.Format(L"<b>%s</b>", name);
	if (!p->other_names.IsEmpty())
		name.Format(L"%s_(%s)", p->last_name, p->other_names);

	if (!p->first_name.IsEmpty() && !p->last_name.IsEmpty())
	{
		name += L" ";
		name += p->first_name;
		if( style )
			name = getColoredString(name, p_otherNameAttrib);
	}
	if (!p->peer.IsEmpty())
		name.Format(L"%s %s", p->peer, (CString)name);

	return name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getTableHeader()
{
	CString last_name;
	CString tableHeader;


	m_command.Format(L"SELECT percent, familyName, tableRoman, arm, tableHeader FROM tables WHERE rowid='%s'", p.tableNumber);
	if (!query(m_command)) return str;

	tableHeader = rs.GetFieldString(4);
	last_name = p.last_name;
	last_name.MakeUpper();
	str = last_name;

	if (!p.titolo.IsEmpty())
		str.Format(L"%s, %s", (CString)str, p.titolo);
	if (theApp.m_inputMode == GAHTML)
		str.Format(L"%s (%s)", (CString)str, tableHeader);
	return(str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createDescendant(int i, bool parentIndex)
{
	CString name;
	CString line;
	CString titolo;
	CString rang;
	CString lastname;
	CString comment;
	//	int parentIndex;
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
	name = getColoredString(name, p_descendantAttrib );



	if (i && vDesc->at(vDesc->at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apjának több felesége volt
	{
		last = vLMX.size() - 1;
		if (last == -1 || vLMX.at(last).g < vDesc->at(i).g)		// magasabb generációban van
		{
			lmx.g = vDesc->at(i).g;
			lmx.lastMotherIndex = p.parentIndex - 1;
			vLMX.push_back(lmx);
		}
		else													// alacsonyabb generációban van
		{
			// megkeresi saját generációjának utoljára listázott motherIndex-ét
			j = vLMX.size() - 1;
			for (j = vLMX.size() - 1; j > 0; --j)
			{
				if (vLMX.at(j).g > vDesc->at(i).g) // a nagyobb generációk eldobása
					vLMX.pop_back();
			}
		}
		last = vLMX.size() - 1;



		if (parentIndex)
		{
			if (vLMX.at(last).lastMotherIndex != p.parentIndex)   // ha az utoljára kiírt motherIndex más, akkor ezt kiírja
			{
				//				if (p.parentIndex == 1)
				//					str.Format(L" ;/%d", p.parentIndex);
				//				else
				str.Format(L"/%d", p.parentIndex);
				name += str;
				vLMX.at(last).lastMotherIndex = p.parentIndex;
			}
		}

	}

	if (p.comment.GetAt(0) != ',')
		name += L" ";

	if (!p.posterior.IsEmpty())
	{
		name += p.posterior;
//		if (p_checkCRLF)
//			name += L"\n";
	}


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

	comment = getColoredString(str, p_commentAttrib);
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	// ha színezni akarjuk, akkor a törlendõ piros, a megtartandó kék
	if (p_repeatedColor)
	{
		switch (vDesc->at(i).status)
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
CString CDescendantsLinearTable::createSpouses(int i)
{
	CString rowid = vDesc->at(i).rowid;
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
			queryPeople(spouse_id, &s);   // házastárs adatainak beolvasása
			marriage = createMarriage(place, date, i, numberOfSpouses);
			spouse = createSpouse();				// kiírás elõtt ellenõrzi, hogy szerepel-e ilyen névem már kiírt házastárs. Ha igen, akkor színezi
			vFullname.push_back(s.fullname);	// vFullname-en gyûjti a már kilistázott házastársak nevét
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
CString CDescendantsLinearTable::createMarriage(CString place, CString date, int i, int numberOfSpouses)
{
	CString marriage;

	if (numberOfSpouses > 1)
		str.Format(L" %d=", i + 1);
	else
		str = L" =";
	marriage = str;
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
// a házastárs listázados blokkját elkészíti
CString CDescendantsLinearTable::createSpouse()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if (p_capital)
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
	fullname = getColoredString(fullname, p_otherNameAttrib );

	// Ellenõrti, hogy ilyen néven már listázott-e házastársat. Ha igen, akkoe színezi
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
//		if (p_checkCRLF)
//			fullname += L"\n";
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

/*
	str = getColoredString(s.comment, p_commentAttrib );
	if (!str.IsEmpty())
	{
		if (s.comment.GetAt(0) == ',')
			spouse.Format(L"%s%s", (CString)spouse, str);
		else
			spouse.Format(L"%s %s", (CString)spouse, str);
	}
*/
	++m_listedP;
	return spouse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a házastárs személyleíró blokkját elkészíti, a neveket bold-dal kiemnelve
CString CDescendantsLinearTable::createSpRelatives()
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


		father = getColoredString(str, p_otherNameAttrib );
		//		if (!sf.peer.IsEmpty())
		//			father.Format(L"%s %s", sf.peer, (CString)father);


	}
	if (!s.mother_id.IsEmpty() && s.mother_id != L"0")
	{
		queryPeople(s.mother_id, &sm);
		mother = getLastFirst(&sm, true );
		mother.Trim();
	}

	if (!father.IsEmpty() && !mother.IsEmpty())		// ( apa és anya is meg van adva )
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
			parents.Format(L" (%s lánya", father);
		++m_listedP;
	}

	if (father.IsEmpty() && !mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", mother);
		else
			parents.Format(L" (%s lánya", mother);
		++m_listedP;
	}


	if (s.sex_id == MAN)
		m_command.Format(L"SELECT wife_id, whichWife  FROM marriages WHERE husband_id='%s' ORDER BY whichWife", s.rowid); // a házastárs házastársai
	else
		m_command.Format(L"SELECT husband_id, whichHusband FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", s.rowid); // a házastárs házastársai
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
			if (ss.rowid != p.rowid)		// a GA sorban szereplõ házastársat kihagyja
			{
				spouseSpouse = getLastFirst(&ss, true);

				//spouseSpouse.Format(L"%s %s", ss.peer, (CString)spouseSpouse);
				// spouseSpouse.Trim();
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
bool CDescendantsLinearTable::voltmar(CString index)
{
	for (int i = 0; i < vSpouseIndex.size(); ++i)
	{
		if (vSpouseIndex.at(i) == index)
			return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::print(CString str)
{
	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(fhDescTable, L"%s\n", str);  
	fflush(fl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getColoredString(CString str, int index)
{
	//	str.Trim();
	CString colored(str);
	if (!str.IsEmpty() && index)
		colored.Format(L"%s%s%s", DE::attrib[index].code1, str, DE::attrib[index].code2);
	return colored;
}