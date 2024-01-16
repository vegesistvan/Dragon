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
	if (m_TabCtrl.GetCurSel() == FULL)
		createHtmlFile(0, FULL_P);
	else
		createHtmlFile(0, UNIQUE_P);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnHtmlListaFix()
{
	if (m_TabCtrl.GetCurSel() == FULL)
		createHtmlFile(0, FULL);
	else
		createHtmlFile(0, UNIQUE);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::createHtmlFile(int i, int which)
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
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(i));
		if (!query(m_command)) return false;
		familyName = rs.GetFieldString(0);
		tableRoman = rs.GetFieldString(2);
		title.Format(L"%s %s leszármazotti tábla", familyName, tableRoman);
	}
	else
		title.Format(L"%s leszármazotti táblája", m_os);

	filename = title;
	if (which == UNIQUE || which == FULL )
	{
		descendantsTablePathName.Format(L"%s\\%s_%s.html", m_descendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhDescTable, descendantsTablePathName, L"w+")) return false;
	}
	else if (which == UNIQUE_P || which == FULL_P)
	{
		printableTablePathName.Format(L"%s\\%s_printable%s.html", m_descendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhPrintable, printableTablePathName, L"w+")) return false;
	}

	printOnly(L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">", which);
	printOnly(L"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"><head>", which);
	str.Format(L"<title>%s</title>", title);
	printOnly(str, which);
	printOnly(L"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />", which);
	printOnly(L"<meta http-equiv=\"language\" content=\"en-us\"/>", which);
	printOnly(L"<script type=\"text/javascript\">", which);
	printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", which);
	//	printOnly(L"<!--", which);
	printOnly(L"/\* http://www.alistapart.com/articles/zebratables/ */", which);
	printOnly(L"function removeClassName (elem, className) {", which);
	printOnly(L"	elem.className = elem.className.replace(className, \"\").trim();", which);
	printOnly(L"}\n", which);

	printOnly(L"function addCSSClass (elem, className) {", which);
	printOnly(L"	removeClassName (elem, className);", which);
	printOnly(L"	elem.className = (elem.className + \" \" + className).trim();", which);
	printOnly(L"}\n", which);

	printOnly(L"String.prototype.trim = function() {", which);
	printOnly(L"	return this.replace( /^\\s+|\\s+$/, \"\" );", which);
	printOnly(L"}\n", which);

	printOnly(L"function stripedTable() {", which);
	printOnly(L"	if (document.getElementById && document.getElementsByTagName) {", which);
	printOnly(L"		var allTables = document.getElementsByTagName('table');", which);
	printOnly(L"		if (!allTables) { return; }\n", which);

	printOnly(L"		for (var i = 0; i < allTables.length; i++) {", which);
	printOnly(L"			if (allTables[i].className.match(/[\\w\\s ]*scrollTable[\\w\\s ]*/)) {", which);
	printOnly(L"				var trs = allTables[i].getElementsByTagName(\"tr\");", which);

	printOnly(L"				for (var j = 0; j < trs.length; j++) {", which);
	printOnly(L"					removeClassName(trs[j], 'alternateRow');", which);
	printOnly(L"					addCSSClass(trs[j], 'normalRow');", which);
	printOnly(L"				}", which);
	printOnly(L"				for (var k = 0; k < trs.length; k += 2) {", which);
	printOnly(L"					removeClassName(trs[k], 'normalRow');", which);
	printOnly(L"					addCSSClass(trs[k], 'alternateRow');", which);
	printOnly(L"				}", which);
	printOnly(L"			}", which);
	printOnly(L"		}", which);
	printOnly(L"	}", which);
	printOnly(L"}\n", which);

	printOnly(L"window.onload = function() { stripedTable(); }", which);
	printOnly(L"-->", which);
	printOnly(L"</script>", which);

	printOnly(L"<style type=\"text/css\">", which);
	//	printOnly(L"<!--", which);
	printOnly(L"/* Terence Ordona, portal[AT]imaputz[DOT]com         */", which);
	printOnly(L"/* http://creativecommons.org/licenses/by-sa/2.0/    */\n", which);

	printOnly(L"/* begin some basic styling here                     */", which);

	printOnly(L"fB{ font color= \"blue\"; }", which);
	printOnly(L"body {", which);
	printOnly(L"	background: #FFF;", which);
	printOnly(L"	color: #000;", which);
	printOnly(L"	font: normal normal 12px Verdana, Geneva, Arial, Helvetica, sans-serif;", which);
	printOnly(L"	margin: 10px;", which);
	printOnly(L"	padding: 0", which);
	printOnly(L"}\n", which);

	printOnly(L"table, th, a {", which);
	printOnly(L"	color: #000;", which);
	printOnly(L"	font: normal bold 12px Verdana, Geneva, Arial, Helvetica, sans-serif", which);
	printOnly(L"}\n", which);


	printOnly(L"table, td, a {", which);
	printOnly(L"	color: #000;", which);
	printOnly(L"	font: normal normal 12px Verdana, Geneva, Arial, Helvetica, sans-serif", which);
	printOnly(L"}\n", which);

	printOnly(L"h1 {", which);
	printOnly(L"	font: normal normal 18px Verdana, Geneva, Arial, Helvetica, sans-serif;", which);
	printOnly(L"	margin: 0 0 5px 0", which);
	printOnly(L"}\n", which);

	printOnly(L"h2 {", which);
	printOnly(L"	font: normal normal 16px Verdana, Geneva, Arial, Helvetica, sans-serif;", which);
	printOnly(L"	margin: 0 0 5px 0", which);
	printOnly(L"}\n", which);

	printOnly(L"h3 {", which);
	printOnly(L"	font: normal normal 13px Verdana, Geneva, Arial, Helvetica, sans-serif;", which);
	printOnly(L"	color: #008000;", which);
	printOnly(L"	margin: 0 0 15px 0", which);
	printOnly(L"}", which);
	printOnly(L"/* end basic styling                                 */\n", which);

	printOnly(L"/* define height and width of scrollable area. Add 16px to width for scrollbar*/", which);
	printOnly(L"div.tableContainer {", which);
	printOnly(L"	clear: both;", which);
	printOnly(L"	border: 1px solid #963;", which);
	printOnly(L"	overflow: auto;", which);
	printOnly(L"	width: device-width", which);
	printOnly(L"}\n", which);


	printOnly(L"/* Reset overflow value to hidden for all non-IE browsers. */", which);
	printOnly(L"html>body div.tableContainer {", which);
	printOnly(L"	overflow: hidden;", which);
	printOnly(L"}\n", which);

	printOnly(L"/* set table header to a fixed position. WinIE 6.x only                                       */", which);
	printOnly(L"/* In WinIE 6.x, any element with a position property set to relative and is a child of       */", which);
	printOnly(L"/* an element that has an overflow property set, the relative value translates into fixed.    */", which);
	printOnly(L"/* Ex: parent element DIV with a class of tableContainer has an overflow property set to auto */", which);
	printOnly(L"thead.fixedHeader tr {", which);
	printOnly(L"	position: relative", which);
	printOnly(L"}\n", which);

	printOnly(L"/* set THEAD element to have block level attributes. All other non-IE browsers            */", which);
	printOnly(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */", which);
	printOnly(L"html>body thead.fixedHeader tr {", which);
	printOnly(L"	display: block", which);
	printOnly(L"}\n", which);

	printOnly(L"/* make the TH elements pretty */", which);
	printOnly(L"thead.fixedHeader th {", which);
	printOnly(L"	color: black;", which);
	printOnly(L"	background: AntiqueWhite;", which);
	printOnly(L"	border-left: 1px solid #EB8;", which);
	printOnly(L"	border-right: 1px solid #B74;", which);
	printOnly(L"	border-top: 1px solid #EB8;", which);
	printOnly(L"	border-bottom: 1px solid #EB8;", which);
	printOnly(L"	font-weight: bold;", which);
	printOnly(L"	padding: 4px 3px;", which);
	printOnly(L"	text-align: center", which);
	printOnly(L"}\n", which);

	printOnly(L"/* make the A elements pretty. makes for nice clickable headers                */", which);
	printOnly(L"thead.fixedHeader a, thead.fixedHeader a:link, thead.fixedHeader a:visited {", which);
	printOnly(L"	display: block;", which);
	printOnly(L"	text-decoration: none;", which);
	printOnly(L"	width: 100%", which);
	printOnly(L"}\n", which);

	printOnly(L"/* make the A elements pretty. makes for nice clickable headers                */", which);
	printOnly(L"/* WARNING: swapping the background on hover may cause problems in WinIE 6.x   */", which);
	printOnly(L"thead.fixedHeader a:hover{", which);
	printOnly(L"	display: block;", which);
	printOnly(L"	text-decoration: underline;", which);
	printOnly(L"	width: 100%", which);
	printOnly(L"}\n", which);

	printOnly(L"/* define the table content to be scrollable                                              */", which);
	printOnly(L"/* set TBODY element to have block level attributes. All other non-IE browsers            */", which);
	printOnly(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */", which);
	printOnly(L"/* induced side effect is that child TDs no longer accept width: auto                     */", which);
	printOnly(L"html>body tbody.scrollContent {", which);
	printOnly(L"	display: block;", which);

	if (which == UNIQUE || which == FULL)  // vagyis fix fejléc, nem nyomtatható
	{
		str.Format(L"    height: %dpx;", height);
		printOnly(str, which);
	}

	printOnly(L"	overflow: auto;", which);
	printOnly(L"	width: 100%", which);
	printOnly(L"}\n", which);


	printOnly(L"/* make TD elements pretty. Provide alternating classes for striping the table */", which);
	printOnly(L"/* http://www.alistapart.com/articles/zebratables/                             */", which);

	printOnly(L"tbody.scrollContent td, tbody.scrollContent tr.normalRow td {", which);
	printOnly(L"	border-bottom: none;", which);
	printOnly(L"	border-left: none;", which);
	printOnly(L"	border-right: 1px solid #CCC;", which);
	printOnly(L"	border-top: 1px solid #DDD;", which);
	printOnly(L"	padding: 2px 3px 3px 4px", which);
	printOnly(L"}\n", which);

	printOnly(L"tbody.scrollContent tr.alternateRow td {", which);
	printOnly(L"	border-bottom: none;", which);
	printOnly(L"	border-left: none;", which);
	printOnly(L"	border-right: 1px solid #CCC;", which);
	printOnly(L"	border-top: 1px solid #DDD;", which);
	printOnly(L"	padding: 2px 3px 3px 4px", which);
	printOnly(L"}\n", which);


	printOnly(L"/* define width of TD elements: 1st, 2nd, and 3rd respectively.          */", which);
	printOnly(L"/* All other non-IE browsers.                                            */", which);
	printOnly(L"/* http://www.w3.org/TR/REC-CSS2/selector.html#adjacent-selectors        */", which);

	printOnly(L"html>body tbody.scrollContent td {", which);
	printOnly(L"	width: 45px", which);
	printOnly(L"}\n", which);


	printOnly(L"html>body tbody.scrollContent td + td {", which);
	printOnly(L"	width: 20px", which);
	printOnly(L"}\n", which);

	printOnly(L"html>body tbody.scrollContent td + td + td {", which);
	printOnly(L"	width: 35px", which);
	printOnly(L"}\n", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td {", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td{", which);
	printOnly(L"	width: 20px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td + td{", which);
	str.Format(L"width: %dpx", width - (45 + 4 * 55 + 35 + + 2*20 + 120));
	printOnly(str, which);
	printOnly(L"}", which);


	printOnly(L"</style>", which);
	printOnly(L"</head><body>\n", which);

	printTopContainer(title, which);
	printOnly(L"<br><br>", which);

	printOnly(L"<div id=\"tableContainer\" class=\"tableContainer\">", which);
	printOnly(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">", which);
	printOnly(L"<thead class=\"fixedHeader\">", which);
	printOnly(L"<tr>", which);

	printOnly(L"<th style=\"width: 45px\">desc</th>", which);
	printOnly(L"<th style=\"width: 20px\">I</th>", which);
	printOnly(L"<th style=\"width: 35px\">id</th>", which);
	printOnly(L"<th style=\"width: 55px\">gyerek<br>id</th>", which);
	printOnly(L"<th style=\"width: 55px\">szülõ<br>id</th>", which);
	printOnly(L"<th style=\"width: 20px\">A</th>", which);
	printOnly(L"<th style=\"width: 55px\">gyerek<br>db</th>", which);
	printOnly(L"<th style=\"width: 55px\">generáció<br>-sorszám</th>", which);
	str.Format(L"<th style=\"width: %dpx\">leszármazott</th>", width - (4 * 55 + 35 + 2*20 ));
	printOnly(str, which);

	printOnly(L"</tr>", which);
	printOnly(L"</thead>", which);
	printOnly(L"<tbody class=\"scrollContent\">", which);

	dataTable(which);

	printOnly(L"</tbody>", which);
	printOnly(L"</table>", which);
	printOnly(L"</div>\n", which);

	printOnly(L"<div><br/ ></div>\n", which);
	printOnly(L"</body></html>\n", which);



	if (which == UNIQUE || which == FULL)
	{
		fclose(fhDescTable);
		ShellExecute(NULL, L"open", descendantsTablePathName, NULL, NULL, SW_SHOWNORMAL);
	}
	else if (which == UNIQUE_P || which == FULL_P )
	{
		fclose(fhPrintable);
		ShellExecute(NULL, L"open", printableTablePathName, NULL, NULL, SW_SHOWNORMAL);
	}


	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::dataTable(int which)
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

		printDesc(i, which);
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
void CDescendantsLinearTable::printDesc( int i, int which  )
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
	if (!m_checkMother && desc.parentSex == WOMAN) return;

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
/*
	ixM.Empty();
	for (int j = 0; j < vDesc->size(); ++j)
	{
		if (vDesc->at(j).rowid == desc.rowidF)
		{
			numOfSpouses = vDesc->at(j).numOfSpouses;
			break;
		}
	}
	if( numOfSpouses > 1 )
	{
		if (desc.motherIndex)
			ixM.Format(L"%d", desc.motherIndex);
	}
*/
	if (m_lastname == 2 && m_familyName != m_radioDNameX)  // kiemelt családnév
	{
		str = getTableHeader();
		family = getColoredString(str, DE::KEK);

		str.Format(L"<td>%s</td>", L"");
		printOnly(L"<tr>", which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		printOnly(str, which);
		name.Format(L"<td>%s</td>", family);
		printOnly(name, which);
		printOnly(L"</tr>", which);
	}
	m_familyName = p.last_name;

//	printOnly(L"<tr>", which);
	if ( gen != genPrev)
		gflag = !gflag;

	if (gflag)
		printOnly(L"<tr style=\"background-color: #fff;\">", which);
	else
		printOnly(L"<tr style=\"background-color: #eee;\">", which);

	str.Format(L"<td><center>%d</center></td>", desc.nOfD);
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", rep);
	printOnly(str, which);
	str.Format(L"<td><center>%d</center></td>", desc.id);
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", idC);
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", idF);
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", desc.ixM );
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", numOfChildren);
	printOnly(str, which);
	str.Format(L"<td><center>%s</center></td>", idD);
	printOnly(str, which);

	str.Format(L"<td>%s</td>", people);
	printOnly(str, which);
	printOnly(L"</tr>", which);

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
	if (!p.folyt.IsEmpty() && m_checkFolyt )
	{
		folyt.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		people += folyt;
	}
	return people;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::printOnly(CString str, int which)
{
	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	switch (which)
	{
//	case 0:
//		fwprintf(fl, L"%s", str);  // a soreleji %-okat printelési karakterekenk értelmezné, ha közvetlenül nyomtatnánk!!! 
		//		fflush(fl);
//		break;
	case UNIQUE:
		fwprintf(fhDescTable, L"%s\n", str);
		//		fflush(fhDescTable);
		break;
	case FULL:
		fwprintf(fhDescTable, L"%s\n", str);
		//		fflush(fhDescTable);
		break;
	case UNIQUE_P:
		fwprintf(fhPrintable, L"%s\n", str);
		//		fflush(fhDescTable);
		break;
	case FULL_P:
		fwprintf(fhPrintable, L"%s\n", str);
		//		fflush(fhDescTable);
		break;
	}
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
bool CDescendantsLinearTable::printTopContainer(CString title, int which)
{
	int l = -37;
	CString today;
	today = getPresentDateTime();
	CString yesno;
	CString nok;
	if (m_checkMother)
		nok = L"igen";
	else
		nok = L"nem";

	CString kihagy;
	switch (m_repeated)
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

	CString maxGen;
	if (m_editGenMax.IsEmpty())
		maxGen = L"minden generáció";
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


	//str.Format(L"<b><center>%s</center>\n<pre></b>\n", title);
	str.Format(L"<b><center>%s</center></b><br>\n\n", title);
	printOnly(str, which);
	//	printOnly(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>", which );

	printOnly(L"<pre>", which);
	str.Format(L"%*s Dragon v. %s", l, L"Program:", theApp.m_version);
	printOnly(str, which);
	str.Format(L"%*s %s", l, L"Dragon.exe készült:", MultiToUnicode(LPCSTR(BUILD)));
	printOnly(str, which);

	str.Format(L"%*s %s", l, L"Alapkönyvtár:", theApp.m_dbFolderPath);
	printOnly(str, which);

	str = L"ÜRES";
	if (theApp.m_inputMode == MANUAL)
		str = L"kézi adatbevitel";
	else if (theApp.m_inputMode == GEDCOM)
		str = L"GEDCOM fájl";
	else if (theApp.m_inputMode = GAHTML)
		str = L"GA.htm fájl";

	str.Format(L"%*s %s", l, L"Adatbázis bemenete:", (CString)str);
	printOnly(str, which);

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM)
	{
		str.Format(L"%*s %s %s", l, L"Bementi fájl készült:", created, inputFile);
		printOnly(str, which);
	}
	str.Format(L"%*s %s %s", l, L"Adatbázis készült:", dateDB, theApp.m_dbFileName);
	printOnly(str, which);
	//	if (!theApp.m_inputVersion.IsEmpty() && theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvasás programverziója:", theApp.m_inputVersion);
		printOnly(str, which);
	}

	//	if (theApp.m_uniteVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Összevonás programverziója:", theApp.m_uniteVersion);
		printOnly(str, which);
	}

	str.Format(L"%*s %s<br>", l, L"Lista készült:", theApp.getPresentDateTime());
	printOnly(str, which);
	str.Format(L"%*s %s", l, L"Generációk max száma:", maxGen);
	printOnly(str, which);
	str.Format(L"%*s %s", l, L"Elágazások összekötése:", connect);
	printOnly(str, which);
	str.Format(L"%*s %s", l, L"Nõk leszármazottai:", nok);
	printOnly(str, which);

	//	str.Format(L"%*s %s", l, L"Ismétlõdõk kihagyása:", kihagy);
	//	printOnly(str, which );

	CString sorrend;
	switch (m_radioOrder)
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
	printOnly(str, which);
	str.Format(L"<br>%*s desc - a bejegyzés leszármazottainak száma", l, L"Oszlopok:");
	printOnly(str, which);
	str.Format(L"%*s I - ismétlõdõ leszármazottak sorszáma", l, L" ");
	printOnly(str, which);
	str.Format(L"%*s id - a bejegyzés azonosítója", l, L" ");
	printOnly(str, which);
	str.Format(L"%*s A - anya index", l, L" ");
	printOnly(str, which);

	printOnly(L"</pre>", which);
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
		block = getColoredString(jel, m_comboSpec);
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
CString CDescendantsLinearTable::getLastFirst(DE::PPEOPLE* p) // házatárs anyjának é stovábbi házastársainak neve képzõdik így
{

	CString name = p->last_name;
	if (m_checkCapital)
		name = convertNameToUpper(name);
	if (m_checkBold)
		name.Format(L"<b>%s</b>", name);
	if (!p->other_names.IsEmpty())
		name.Format(L"%s_(%s)", p->last_name, p->other_names);

	if (!p->first_name.IsEmpty() && !p->last_name.IsEmpty())
	{
		name += L" ";
		name += p->first_name;
		if (m_html)
			name = getColoredString(name, m_comboOtherName);
	}
	if (!p->peer.IsEmpty())
		name.Format(L"%s %s", p->peer, (CString)name);

	return name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getTableHeader()
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
		str.Format(L"%s (%s)", (CString)str, tableHeader);
	}
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
	if (m_radioDNameX == 1)
	{
		lastname = p.last_name;
		if (lastname == L"N") lastname.Empty();
		if (m_checkCapital)
			lastname = convertNameToUpper(lastname);
		if (m_checkBold)
			lastname.Format(L"<b>%s</b>", lastname);
		if (lastname == L"N;")
			name.Format(L"\n%s %s %s", lastname, p.first_name, p.peer);
		else
			name.Format(L"\n%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		name.Trim();
		if (!p.other_names.IsEmpty())
			name.Format(L"\n%s_(%s)", (CString)name, p.other_names);
	}
	name = getColoredString(name, m_comboDAttrib );



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
		if (m_checkCRLF)
			name += L"\n";
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

	comment = getColoredString(str, m_comboComment);
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	// ha színezni akarjuk, akkor a törlendõ piros, a megtartandó kék
	if (m_repeatedColor)
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
			//			print(marriage);
			spouse = createSpouse();				// kiírás elõtt ellenõrzi, hogy szerepel-e ilyen névem már kiírt házastárs. Ha igen, akkor színezi
			//			print(spouse);
			vFullname.push_back(s.fullname);	// vFullname-en gyûjti a már kilistázott házastársak nevét
			spRelatives = createSpRelatives();
			//			print(spRelatives);
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

	//	fwprintf(fl, L"\n");
	//	fflush(fl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createMarriage(CString place, CString date, int i, int numberOfSpouses)
{
	CString marriage;

	if (numberOfSpouses > 1)
		str.Format(L" %d=", i + 1);
	else
		str = L" =";

	marriage = getColoredString(str, m_comboSpec);

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

	if (m_checkCapital)
		lastname = convertNameToUpper(lastname);
	if (m_checkBold)
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
	fullname = getColoredString(fullname, m_comboOtherName );

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
		if (m_checkCRLF)
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

	str = getColoredString(s.comment, m_comboComment );
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

		father = getColoredString(str, m_comboOtherName );
		//		if (!sf.peer.IsEmpty())
		//			father.Format(L"%s %s", sf.peer, (CString)father);


	}
	if (!s.mother_id.IsEmpty() && s.mother_id != L"0")
	{
		queryPeople(s.mother_id, &sm);
		mother = getLastFirst(&sm);
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
				spouseSpouse = getLastFirst(&ss);

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
	fwprintf(fl, L"%s", str);  // a soreleji %-okat printelési karakterekenk értelmezné, ha közvetlenül nyomtatnánk!!! 
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