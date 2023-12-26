#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "descendants.h"
#include "descendantsParam.h"
#include "Table_people_columns.h"
#include "Table_tables.h"
#include "build_defs.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::linearTable(int i, int which)
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
	if (which == 1)
	{
		descendantsTablePathName.Format(L"%s\\%s_%s.html", m_descendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhDescTable, descendantsTablePathName, L"w+")) return false;
	}
	else if( which == 2)
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
	
	if (which == 1 )  // vagyis fix fejléc, nem nyomtatható
		str.Format(L"    height: %dpx;", height);
	printOnly(str, which);
	
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
	printOnly(L"	width: 35px", which);
	printOnly(L"}\n", which);

	printOnly(L"html>body tbody.scrollContent td + td {", which);
	printOnly(L"	width: 35px", which);
	printOnly(L"}\n", which);

	printOnly(L"html>body tbody.scrollContent td + td + td {", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td{", which);
	printOnly(L"	width: 55px", which);
	printOnly(L"}", which);

	printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td{", which);
	str.Format(L"width: %dpx", width - (4 * 55 + 2 * 35 + 120));
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

	printOnly(L"<th style=\"width: 35px\">#</th>", which);
	printOnly(L"<th style=\"width: 35px\">ism.</th>", which);
	printOnly(L"<th style=\"width: 55px\">testvér<br>db</th>", which);
	printOnly(L"<th style=\"width: 55px\">szülõ<br>#</th>", which);
	printOnly(L"<th style=\"width: 55px\">gyerek<br>db</th>", which);
	printOnly(L"<th style=\"width: 55px\">generáció<br>-sorszám</th>", which);
	str.Format(L"<th style=\"width: %dpx\">leszármazott</th>", width - (4 * 55 + 2 * 35));
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



	if (which == 1 )
	{
		fclose(fhDescTable);
		ShellExecute(NULL, L"open", descendantsTablePathName, NULL, NULL, SW_SHOWNORMAL);
	}
	else if (which == 2)
	{
		fclose(fhPrintable);
		ShellExecute(NULL, L"open", printableTablePathName, NULL, NULL, SW_SHOWNORMAL);
	}


	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::dataTable(int i)
{
	CString people;
	CString tableheader;
	CString tableNumber;
	CString namePrev;
	CString name;
	CString family;
	CString idF;
	CString idD;
	CString	numOfChildren;
	CString numOfSib;
	CString gen;
	CString rep;
	int temp;

	int gcnt = 0;
	int gPrev = 0;

	m_listedP = 0;
	m_listedD = 0;
	m_indent = 0;

	m_familyName.Empty();
	vLMX.clear();		// a generáció utolsó kiírt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);
	str.Format(L"%s leszármazottainak listázása...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vDesc.size());
	wndP.SetPos(0);

	DN::DESC desc;
	int which = 1;
	

	for (int i = 0; i < vDesc.size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		desc = vDesc.at(i);

		if (desc.hidden) continue;   // apa bejegyzése, aki más táblában szerepel

		if (p_repeated != 0 && desc.status == 2) continue;	// ismétlõdõ bejegyzés, amit nem ki akarunk kiírni

		queryPeople(desc.rowid, &p);

		if (!p_mother && desc.parentSex == WOMAN) continue;

		people = getComplexDescription(i);

		gen = get_gABC( desc.g );
		idD.Format(L"%3s-%-4d", gen, desc.childorder );


		if( desc.fatherIndex != -1 )
			idF.Format(L"%d", desc.fatherIndex);
		else
			idF.Empty();

		if (desc.numOfChildren > 0)
			numOfChildren.Format(L"%d", desc.numOfChildren );
		else
			numOfChildren.Empty();

		if (desc.fatherIndex > 0)
		{
			temp = vDesc.at(desc.fatherIndex).numOfChildren - 1;
			if (temp > 0)
				numOfSib.Format(L"%d", vDesc.at(desc.fatherIndex).numOfChildren - 1);
			else
				numOfSib.Empty();
		}
		else
			numOfSib.Empty();


		if (desc.cntRep)
			rep.Format(L"%d", desc.cntRep);
		else
			rep.Empty();



		if (p_lastname == 2 && m_familyName != p.last_name )  // kiemelt családnév
		{
			str = getTableHeader();
			family = getColoredString( str, KEK);
	
			str.Format(L"<td>%s</td>", L"");
			printOnly(L"<tr>", which);
			printOnly(str, which);
			printOnly(str, which);
			printOnly(str, which);
			printOnly(str, which);
			printOnly(str, which);
			printOnly(str, which);
			name.Format(L"<td>%s</td>", family );
			printOnly(name, which);
			printOnly(L"</tr>", which);
		}
		m_familyName = p.last_name;

		printOnly(L"<tr>", which);
		str.Format(L"<td><center>%d</center></td>", i);
		printOnly(str, which);
		str.Format(L"<td><center>%s</center></td>", rep );
		printOnly(str, which);
		str.Format(L"<td><center>%s</center></td>", numOfSib );
		printOnly(str, which);
		str.Format(L"<td><center>%s</center></td>", idF);
		printOnly(str, which);
		str.Format(L"<td><center>%s</center></td>", numOfChildren );
		printOnly(str, which);
		str.Format(L"<td><center>%s</center></td>", idD );
		printOnly(str, which);

		str.Format(L"<td>%s</td>", people );
		printOnly(str, which);
		printOnly(L"</tr>", which);

		gPrev = desc.g;
	}

	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getComplexDescription(int i)
//CString CDescendants::getPeople(int i)
{
	CString people		= createDescendant(i);
	CString spouses		= createSpouses(i);
	CString arm;
	CString csalad;
	CString folyt;

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
	if (!p.folyt.IsEmpty() && p_folyt)
	{
		folyt.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		people += folyt;
	}
	return people;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::printOnly(CString str, int which)
{
	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	switch (which)
	{
	case 0:
		fwprintf(fl, L"%s", str);  // a soreleji %-okat printelési karakterekenk értelmezné, ha közvetlenül nyomtatnánk!!! 
//		fflush(fl);
		break;
	case 1:
		fwprintf(fhDescTable, L"%s\n", str);
//		fflush(fhDescTable);
		break;
	case 2:
		fwprintf(fhPrintable, L"%s\n", str);
//		fflush(fhDescTable);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR CDescendants::get_gABC(int g)
{
	TCHAR gABC;
	int ix = g / 26;		  // ix = 0, ha g kisebb mint 26, = 1 Ha nagyobb
	gABC = TCHAR('A') + ix * 6 + g;   // 6 : a 'Z' és 'a' közötti karakterek száma,
	return gABC;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendants::printTopContainer(CString title, int which)
{
	int l = -37;
	CString today;
	today = getPresentDateTime();
	CString yesno;
	CString nok;
	if (p_mother)
		nok = L"igen";
	else
		nok = L"nem";

	CString kihagy;
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
	switch (p_radioOrder)
	{
	case ORDER_INPUT:
		sorrend = L"bementi sorrend";
		break;
	case ORDER_BIRTH:
		sorrend = L"születési idõ";
		break;
	case ORDER_INCREASING:
		sorrend = L"növekvõ hosszúságú leszármazotti szálak";
		break;
	case ORDER_DECREASING:
		sorrend = L"csökkenõ hosszúságú leszármazotti szálak";
		break;
	}

	str.Format(L"%*s %s", l, L"Sorrend:", sorrend);
	printOnly(str, which);

	printOnly(L"</pre>", which);
}
