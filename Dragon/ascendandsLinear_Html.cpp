// ascendantsTable.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendantsLinear_Table.h"
#include "ascendants.h"
#include "ascendantsEnum.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include "ProgressWnd.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendantsLinearTable::createHtmlFile(int which)
{
	CString filename;
	CString title;
	int tableTop = 205;
	int height = theApp._h - tableTop;
	int width = theApp._w;

	CString fullPathName;
	CString uniquePathName;

	nItem = 0;

	if ( m_TabCtrl.GetCurSel() == FULL_TAB )
	{
		if( which == FULL )
			m_title.Format(L"%s felmenõinek teljes lineáris táblája", m_name);
		else
			m_title.Format(L"%s felmenõinek teljes nyomtatható lineáris táblája", m_name);
		fullPathName.Format(L"%s\\%s_%s.html", m_ascendantsPath, m_title, getTimeTag());
		if (!openFileSpec(&fh_Full, fullPathName, L"w+")) return false;
	}
	else
	{
		if( which == FULL)
			m_title.Format(L"%s felmenõinek nyomtatható lineáris táblája ismétlõdések kihagyásával", m_name);
		else
			m_title.Format(L"%s felmenõinek lineáris táblája ismétlõdések kihagyásával", m_name);
		uniquePathName.Format(L"%s\\%s_%s.html", m_ascendantsPath, m_title, getTimeTag());
		if (!openFileSpec(&fh_Unique, uniquePathName, L"w+")) return false;
	}

	printOnly(L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">", which);
	printOnly(L"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"><head>", which);
	str.Format(L"<title>%s</title>", m_title);
	printOnly(str, which);
	printOnly(L"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />", which);
	printOnly(L"<meta http-equiv=\"language\" content=\"en-us\"/>", which);
	printOnly(L"<script type=\"text/javascript\">", which);

//	if (which == FULL_P || which == UNIQUE_P)
//		printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", which);

	printOnly(L"<!--", which);
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
	//	printOnly(L"/* Terence Ordona, portal[AT]imaputz[DOT]com         */", which);
	//	printOnly(L"/* http://creativecommons.org/licenses/by-sa/2.0/    */\n", which);

	printOnly(L"/* begin some basic styling here                     */", which);
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

	printOnly(L"bY{ background: yellow; }", which);

	printOnly(L"/* end basic styling                                 */\n", which);

	printOnly(L"/* define height and width of scrollable area. Add 16px to width for scrollbar*/", which);
	printOnly(L"div.tableContainer {", which);
	printOnly(L"	clear: both;", which);
	printOnly(L"	border: 1px solid #963;", which);
	printOnly(L"	overflow: auto;", which);
	printOnly(L"	width: device-width", which);
//	if (which == FULL || which == UNIQUE)   // azaz nme nyomtatható verzió
//		str.Format(L"width: %dpx", width);  // ez számít!!
	printOnly(L"}\n", which);

	printOnly(L"p{ background - color: yellow;}", which);

	printOnly(L"/* Reset overflow value to hidden for all non-IE browsers. */", which);
	printOnly(L"html>body div.tableContainer {", which);
	printOnly(L"	overflow: hidden;", which);
	//	str.Format(L" height:  %dpx", height);
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
	
	if (which == FULL || which == UNIQUE)  // vagyis fix fejléc, nem nyomtatható
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

	if ( m_TabCtrl.GetCurSel() == FULL_TAB )
	{
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
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td{", which);
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td{", which);
		str.Format(L"width: %dpx", width - (3 * 55 + 2 * 35));
		printOnly(str, which);
		printOnly(L"}", which);

		//	printOnly(L"-->", which);

		printOnly(L"</style>", which);
		printOnly(L"</head><body>\n", which);

		printTopContainer(m_title, which);

		printOnly(L"<div id=\"tableContainer\" class=\"tableContainer\">", which);
		printOnly(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">", which);

		printOnly(L"<thead class=\"fixedHeader\">", which);
		printOnly(L"<tr>", which);

		printOnly(L"<th style=\"width: 35px\">#</th>", which);
		printOnly(L"<th style=\"width: 35px\">ism.</th>", which);
		printOnly(L"<th style=\"width: 55px\">apa</th>", which);
		printOnly(L"<th style=\"width: 55px\">gyerek</th>", which);
		printOnly(L"<th style=\"width: 55px\">id</th>", which);
		str.Format(L"<th style=\"width: %dpx\">felmenõ</th>", width - (3 * 55 + 2 * 35));
		printOnly(str, which);
	}
	else
	{
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
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td{", which);
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td{", which);
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td{", which);
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td{", which);
		printOnly(L"width: 55px", which);
		printOnly(L"}", which);

		printOnly(L"html>body tbody.scrollContent td + td + td + td + td + td + td + td + td{", which);
		str.Format(L"width: %dpx", width - (6 * 55 + 2 * 35 + 120));
		printOnly(str, which);
		printOnly(L"}", which);


		printOnly(L"</style>", which);
		printOnly(L"</head><body>\n", which);

		printTopContainer(m_title, which);

		printOnly(L"<div id=\"tableContainer\" class=\"tableContainer\">", which);
		printOnly(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">", which);

		printOnly(L"<thead class=\"fixedHeader\">", which);
		printOnly(L"<tr>", which);

		printOnly(L"<th style=\"width: 35px\">#</th>", which);
		printOnly(L"<th style=\"width: 35px\">ism.</th>", which);
		printOnly(L"<th style=\"width: 55px\">testvér</th>", which);
		printOnly(L"<th style=\"width: 55px\">hzstárs</th>", which);
		printOnly(L"<th style=\"width: 55px\">anya</th>", which);
		printOnly(L"<th style=\"width: 55px\">apa</th>", which);
		printOnly(L"<th style=\"width: 55px\">gyerek</th>", which);
		printOnly(L"<th style=\"width: 55px\">id</th>", which);
		str.Format(L"<th style=\"width: %dpx\">felmenõ</th>", width - (5 * 55 + 2 * 35));
		printOnly(str, which);

	}


	printOnly(L"</tr>", which);
	printOnly(L"</thead>", which);

	printOnly(L"<tbody class=\"scrollContent\">", which);

	//	dataTable(which);
	printData();

	printOnly(L"</tbody>", which);
	printOnly(L"</table>", which);
	printOnly(L"</div>\n", which);

	printOnly(L"<div><br/ ></div>\n", which);
	printOnly(L"</body></html>\n", which);


	if (which == FULL || which == FULL_P)
	{
		fclose(fh_Full);
		ShellExecute(NULL, L"open", fullPathName, NULL, NULL, SW_SHOWNORMAL);
	}
	if (which == UNIQUE || which == UNIQUE_P)
	{
		fclose(fh_Unique);
		ShellExecute(NULL, L"open", uniquePathName, NULL, NULL, SW_SHOWNORMAL);
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printTopContainer(CString title, int which)
{
	CString yesno;


	str.Format(L"<b><center>%s</center></b><br>", title);
	printOnly(str, which);

	//	printOnly(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>", which);

	str.Format(L"<b><center>Dragon v. %s</center></b>", theApp.m_version);
	printOnly(str, which);


	printOnly(L"<div class=\"top-container\">", which);
	printOnly(L"<pre>", which);


	int l = -37;
	CString dateHtml = theApp.getFileCreationDate(theApp.m_htmlPathName);
	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	str.Format(L"%*s %s", l, L"Alapkönyvtár:", theApp.m_dbFolderPath);
	printOnly(str, which);
	if (theApp.m_inputMode == GAHTML)
	{
		str.Format(L"%*s %s %s", l, L"Bementi fájl készült:", dateHtml, theApp.m_htmlFileName);
		printOnly(str, which);
	}
	str.Format(L"%*s %s %s", l, L"Adatbázis készült:", dateDB, theApp.m_dbFileName);
	printOnly(str, which);
	if (theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvasás programverziója:", theApp.m_inputVersion);
		printOnly(str, which);
	}
	if (theApp.m_uniteVersion.IsEmpty())
		str = "A bejegyzések nincsenek összevonva!!!\n";
	printOnly(str, which);
	if (theApp.m_uniteVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Összevonás programverziója:", theApp.m_uniteVersion);
		printOnly(str, which);
	}

	str.Format(L"%*s %sn", l, L"Lista készült:", theApp.getPresentDateTime());
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Apai generációk száma:", m_numOfGF);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Anyai generációk száma:", m_numOfGM);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Egyenesági apai genrációk száma:", m_numOfGDF);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Egyenesági anyai generációk száma:", m_numOfGDM);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Apai felmenõk száma:", m_numOfAF);
	printOnly(str, which);
	str.Format(L"%*s %d\n", l, L"Anyai felmenõk száma:", m_numOfAM);
	printOnly(str, which);
	printOnly(L"</pre>", which);
	printOnly(L"</div>", which);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printData()
{
	GKR gkr;
	int gPrev = 0;
	bool gflag = true;
	int moreC;
	int pos;

	CString ascendant;
	CString children;
	CString siblings;
	CString spouses;

	CProgressWnd wndP(NULL, L"html fájl készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR->size());


	if (m_TabCtrl.GetCurSel() == FULL )
	{


		for (int i = 0; i < m_ListCtrl->GetItemCount(); ++i)
		{
			gkr = vGKR->at(i);
			int pos;
			if (gkr.g != gPrev)
				gflag = !gflag;

			if (gflag)
				printF(L"<tr style=\"background-color: #fff;\">");
			else
				printF(L"<tr style=\"background-color: #eee;\">");

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, F_CNT));
			printF(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, F_ISM));
			printF(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, F_IDF));
			printF(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, F_IDC));
			printF(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, F_ID));
			printF(str);

			str = L"<td><b>";
			str += m_ListCtrl->GetItemText(i, F_NAME);
			str += L"</b>";
			str += m_ListCtrl->GetItemText(i, F_DESCRIPTION);
			str += L"</td>";
			printF(str);

			printF(L"</tr>");

			gPrev = gkr.g;

			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;
		}
	}
	else
	{
		for (int i = 0; i < m_ListCtrl->GetItemCount(); ++i)
		{

			gkr = vGKR->at(i);

			if (gkr.g != gPrev)
				gflag = !gflag;

			moreC = _wtoi(m_ListCtrl->GetItemText(i, U_MOREC));

			if (gflag)
				printU(L"<tr style=\"background-color: #fff;\">");
			else
				printU(L"<tr style=\"background-color: #eee;\">");


			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, U_CNT));
			printU(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, U_ISM));
			printU(str);


			siblings = m_ListCtrl->GetItemText(i, U_SIBLINGS);
			siblings.Replace(L",", L"<br>");
			str.Format(L"<td><center>%s</center></td>", siblings);
			printU(str);


			spouses = m_ListCtrl->GetItemText(i, U_SPOUSES);
			spouses.Replace(L",", L"<br>");
			str.Format(L"<td><center>%s</center></td>", spouses);
			printU(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, U_IDM));
			printU(str);

			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, U_IDF));
			printU(str);

			children = m_ListCtrl->GetItemText(i, U_IDC);
			children.Replace(L",", L"<br>");
			if (moreC)
				str.Format(L"<td style=\"background-color: black;\"><font color=\"white\"><center>%s</center></font></td>", children);
			else
				str.Format(L"<td><center>%s</center></td>", children);
			printU(str);


			str.Format(L"<td><center>%s</center></td>", m_ListCtrl->GetItemText(i, U_ID));
			printU(str);


			str = L"<td><b>";
			str += m_ListCtrl->GetItemText(i, U_NAME);
			str += L"</b>";
			str += m_ListCtrl->GetItemText(i, U_DESCRIPTION);
			str += L"</td>";
			printU(str);

			printU(L"</tr>");
			gPrev = gkr.g;

			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printOnly(CString str, int which)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	switch (which)
	{
	case FULL:
		fwprintf(fh_Full, L"%s\n", str);
//		fflush(fh_Full);
		break;
	case UNIQUE:
		fwprintf(fh_Unique, L"%s\n", str);
//		fflush(fh_Unique);
		break;
	case FULL_P:
		fwprintf(fh_Full, L"%s\n", str);
//		fflush(fh_Full);
		break;
	case UNIQUE_P:
		fwprintf(fh_Unique, L"%s\n", str);
//		fflush(fh_Unique);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printF(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fh_Full, L"%s\n", str);
//	fflush(fh_Full);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printU(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fh_Unique, L"%s\n", str);
//	fflush(fh_Unique);
}


