#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"




// A neten talált példa. Nem1 hívható, be kell indítani, ha látni akarod.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::fixTableHeader()
{
	CString filename;
	CString title;

	title.Format(L"fix header");
	filename = title;
	m_fixHeaderFile.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
	if (!openFileSpec(&fhfixHeader, m_fixHeaderFile, L"w+")) return false;

	printF(L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	printF(L"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"><head>");
	printF(L"<title>Pure CSS Scrollable Table with Fixed Header</title>");
	printF(L"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />");
	printF(L"<meta http-equiv=\"language\" content=\"en-us\"/>");
	printF(L"<script type=\"text/javascript\">");

	printF(L"<!--");
	printF(L"/\* http://www.alistapart.com/articles/zebratables/ */");
	printF(L"function removeClassName (elem, className) {");
	printF(L"	elem.className = elem.className.replace(className, \"\").trim();");
	printF(L"}\n");

	printF(L"function addCSSClass (elem, className) {");
	printF(L"	removeClassName (elem, className);");
	printF(L"	elem.className = (elem.className + \" \" + className).trim();");
	printF(L"}\n");

	printF(L"String.prototype.trim = function() {");
	printF(L"	return this.replace( /^\\s+|\\s+$/, \"\" );");
	printF(L"}\n");

	printF(L"function stripedTable() {");
	printF(L"	if (document.getElementById && document.getElementsByTagName) {");
	printF(L"		var allTables = document.getElementsByTagName('table');");
	printF(L"		if (!allTables) { return; }\n");

	printF(L"		for (var i = 0; i < allTables.length; i++) {");
	printF(L"			if (allTables[i].className.match(/[\\w\\s ]*scrollTable[\\w\\s ]*/)) {");
	printF(L"				var trs = allTables[i].getElementsByTagName(\"tr\");");
	
	printF(L"				for (var j = 0; j < trs.length; j++) {");
	printF(L"					removeClassName(trs[j], 'alternateRow');");
	printF(L"					addCSSClass(trs[j], 'normalRow');");
	printF(L"				}");
	printF(L"				for (var k = 0; k < trs.length; k += 2) {");
	printF(L"					removeClassName(trs[k], 'normalRow');");
	printF(L"					addCSSClass(trs[k], 'alternateRow');");
	printF(L"				}");
	printF(L"			}");
	printF(L"		}");
	printF(L"	}");
	printF(L"}\n");

	printF(L"window.onload = function() { stripedTable(); }");
	printF(L"-->");
	printF(L"</script>");
	printF(L"<style type=\"text/css\">");
	printF(L"<!--");
	printF(L"/* Terence Ordona, portal[AT]imaputz[DOT]com         */");
	printF(L"/* http://creativecommons.org/licenses/by-sa/2.0/    */\n");

	printF(L"/* begin some basic styling here                     */");
	printF(L"body {");
	printF(L"	background: #FFF;");
	printF(L"	color: #000;");
	printF(L"	font: normal normal 12px Verdana, Geneva, Arial, Helvetica, sans-serif;");
	printF(L"	margin: 10px;");
	printF(L"	padding: 0");
	printF(L"}\n");

	printF(L"table, td, a {");
	printF(L"	color: #000;");
	printF(L"	font: normal normal 12px Verdana, Geneva, Arial, Helvetica, sans-serif");
	printF(L"}\n");

	printF(L"h1 {");
	printF(L"	font: normal normal 18px Verdana, Geneva, Arial, Helvetica, sans-serif;");
	printF(L"	margin: 0 0 5px 0");
	printF(L"}\n");

	printF(L"h2 {");
	printF(L"	font: normal normal 16px Verdana, Geneva, Arial, Helvetica, sans-serif;");
	printF(L"	margin: 0 0 5px 0");
	printF(L"}\n");

	printF(L"h3 {");
	printF(L"	font: normal normal 13px Verdana, Geneva, Arial, Helvetica, sans-serif;");
	printF(L"	color: #008000;");
	printF(L"	margin: 0 0 15px 0");
	printF(L"}");
	printF(L"/* end basic styling                                 */\n");

	printF(L"/* define height and width of scrollable area. Add 16px to width for scrollbar*/");
	printF(L"div.tableContainer {");
	printF(L"	clear: both;");
	printF(L"	border: 1px solid #963;");
	printF(L"	height: 285px;");
	printF(L"	overflow: auto;");
	printF(L"	width: 756px");
	printF(L"}\n");

	printF(L"/* Reset overflow value to hidden for all non-IE browsers. */");
	printF(L"html>body div.tableContainer {");
	printF(L"	overflow: hidden;");
	printF(L"	width: 756px");
	printF(L"}\n");

	printF(L"/* define width of table. IE browsers only                 */");
	printF(L"div.tableContainer table {");
	printF(L"	float: left;");
	printF(L"	width: 740px");
	printF(L"}\n");

	printF(L"/* define width of table. Add 16px to width for scrollbar.           */");
	printF(L"/* All other non-IE browsers.                                        */");
	printF(L"html>body div.tableContainer table {");
	printF(L"	width: 756px");
	printF(L"}\n");

	printF(L"/* set table header to a fixed position. WinIE 6.x only                                       */");
	printF(L"/* In WinIE 6.x, any element with a position property set to relative and is a child of       */");
	printF(L"/* an element that has an overflow property set, the relative value translates into fixed.    */");
	printF(L"/* Ex: parent element DIV with a class of tableContainer has an overflow property set to auto */");
	printF(L"thead.fixedHeader tr {");
	printF(L"	position: relative");
	printF(L"}\n");

	printF(L"/* set THEAD element to have block level attributes. All other non-IE browsers            */");
	printF(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */");
	printF(L"html>body thead.fixedHeader tr {");
	printF(L"	display: block");
	printF(L"}\n");

	printF(L"/* make the TH elements pretty */");
	printF(L"thead.fixedHeader th {");
	printF(L"	background: #C96;");
	printF(L"	border-left: 1px solid #EB8;");
	printF(L"	border-right: 1px solid #B74;");
	printF(L"	border-top: 1px solid #EB8;");
	printF(L"	font-weight: normal;");
	printF(L"	padding: 4px 3px;");
	printF(L"	text-align: left");
	printF(L"}\n");

	printF(L"/* make the A elements pretty. makes for nice clickable headers                */");
	printF(L"thead.fixedHeader a, thead.fixedHeader a:link, thead.fixedHeader a:visited {");
	printF(L"	color: #FFF;");
	printF(L"	display: block;");
	printF(L"	text-decoration: none;");
	printF(L"	width: 100%");
	printF(L"}\n");

	printF(L"/* make the A elements pretty. makes for nice clickable headers                */");
	printF(L"/* WARNING: swapping the background on hover may cause problems in WinIE 6.x   */");
	printF(L"thead.fixedHeader a:hover{");
	printF(L"	color: #FFF;");
	printF(L"	display: block;");
	printF(L"	text-decoration: underline;");
	printF(L"	width: 100%");
	printF(L"}\n");

	printF(L"/* define the table content to be scrollable                                              */");
	printF(L"/* set TBODY element to have block level attributes. All other non-IE browsers            */");
	printF(L"/* this enables overflow to work on TBODY element. All other non-IE, non-Mozilla browsers */");
	printF(L"/* induced side effect is that child TDs no longer accept width: auto                     */");
	printF(L"html>body tbody.scrollContent {");
	printF(L"	display: block;");
	printF(L"	height: 262px;");
	printF(L"	overflow: auto;");
	printF(L"	width: 100%");
	printF(L"}\n");

	printF(L"/* make TD elements pretty. Provide alternating classes for striping the table */");
	printF(L"/* http://www.alistapart.com/articles/zebratables/                             */");
	printF(L"tbody.scrollContent td, tbody.scrollContent tr.normalRow td {");
	printF(L"	background: #FFF;");
	printF(L"	border-bottom: none;");
	printF(L"	border-left: none;");
	printF(L"	border-right: 1px solid #CCC;");
	printF(L"	border-top: 1px solid #DDD;");
	printF(L"	padding: 2px 3px 3px 4px");
	printF(L"}\n");

	printF(L"tbody.scrollContent tr.alternateRow td {");
	printF(L"	background: #EEE;");
	printF(L"	border-bottom: none;");
	printF(L"	border-left: none;");
	printF(L"	border-right: 1px solid #CCC;");
	printF(L"	border-top: 1px solid #DDD;");
	printF(L"	padding: 2px 3px 3px 4px");
	printF(L"}\n");

	printF(L"/* define width of TH elements: 1st, 2nd, and 3rd respectively.          */");
	printF(L"/* Add 16px to last TH for scrollbar padding. All other non-IE browsers. */");
	printF(L"/* http://www.w3.org/TR/REC-CSS2/selector.html#adjacent-selectors        */");
	printF(L"html>body thead.fixedHeader th {");
	printF(L"	width: 200px");
	printF(L"}\n");

	printF(L"html>body thead.fixedHeader th + th {");
	printF(L"	width: 240px");
	printF(L"}\n");

	printF(L"html>body thead.fixedHeader th + th + th {");
	printF(L"	width: 316px");
	printF(L"}\n");

	printF(L"/* define width of TD elements: 1st, 2nd, and 3rd respectively.          */");
	printF(L"/* All other non-IE browsers.                                            */");
	printF(L"/* http://www.w3.org/TR/REC-CSS2/selector.html#adjacent-selectors        */");
	printF(L"html>body tbody.scrollContent td {");
	printF(L"	width: 200px");
	printF(L"}\n");

	printF(L"html>body tbody.scrollContent td + td {");
	printF(L"	width: 240px");
	printF(L"}\n");

	printF(L"html>body tbody.scrollContent td + td + td {");
	printF(L"	width: 300px");

	printF(L"}");
	printF(L"-->");
	printF(L"</style>");
	printF(L"</head><body>\n");

	printF(L"<h1>Pure CSS Scrollable Table with Fixed Header</h1>");
	printF(L"<h2>Using CSS to allow scrolling within a single HTML table</h2>\n");

	printF(L"<div><br/></div>\n");

	printF(L"<h2>The Big 4 Version</h2>");
	printF(L"<h3>Basic CSS Browser Filtering</h3>\n");

	printF(L"<div id=\"tableContainer\" class=\"tableContainer\">");
	printF(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">");
	printF(L"<thead class=\"fixedHeader\">");
	printF(L"	<tr>");
	printF(L"		<th><a href=\"#\">Header 1</a></th>");
	printF(L"		<th><a href=\"#\">Header 2</a></th>");
	printF(L"		<th><a href=\"#\">Header 3</a></th>");
	printF(L"	</tr>");
	printF(L"</thead>");
	printF(L"<tbody class=\"scrollContent\">");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L" 		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>And Repeat 1</td>");
	printF(L"		<td>And Repeat 2</td>");
	printF(L"		<td>And Repeat 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Cell Content 1</td>");
	printF(L"		<td>Cell Content 2</td>");
	printF(L"		<td>Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>More Cell Content 1</td>");
	printF(L"		<td>More Cell Content 2</td>");
	printF(L"		<td>More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>Even More Cell Content 1</td>");
	printF(L"		<td>Even More Cell Content 2</td>");
	printF(L"		<td>Even More Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"	<tr>");
	printF(L"		<td>End of Cell Content 1</td>");
	printF(L"		<td>End of Cell Content 2</td>");
	printF(L"		<td>End of Cell Content 3</td>");
	printF(L"	</tr>");
	printF(L"</tbody>");
	printF(L"</table>");
	printF(L"</div>\n");

	printF(L"<div><br/ ></div>\n");

	printF(L"<div>");
	printF(L"	<p>Also see the <a href=\"http://www.imaputz.com/cssStuff/bulletVersion.html\">The Bullet Resistant Version</a> :: Wider range of support and degrading of Browsers</p>\n");

	printF(L"	<h3>Browser Support (table is scrollable with fixed headers)</h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>Opera 7.x + (All Platforms) :: Tested with 7.2x and 7.5x</li>");
	printF(L"		<li>Mozilla 1.x + (All Platforms) :: Tested with 1.0x and 1.6x</li>");
	printF(L"		<li>IE 6.x + (Windows) :: Tested with 6.0x</li>");
	printF(L"		<li>Safari 1.x + (MacOS) :: Tested with 1.2x</li>");
	printF(L"		<li>Konqueror 3.x + (Linux / BSD) :: Tested with 3.2x</li>");
	printF(L"	</ul>\n");

	printF(L"	<h3>Almost works (table is scrollable) </h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>IE 5.x + (Windows) :: Tested with 5.0x and 5.5x</li>");
	printF(L"	</ul>\n");

	printF(L"	<h3>Doesn't work (table is not viewable)</h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>Opera 5.x and 6.x :: Tested with 5.1x and 6.x</li>");
	printF(L"		<li>IE 5.x + (MacOS) :: Tested with 5.2x</li>");
	printF(L"	</ul>\n");

	printF(L"	<h3>Degrades gracefully</h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>All other non-supporting browsers</1i>");
	printF(L"	</ul>\n");

	printF(L"	<h3>Notes:</h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>Opera v5 to v7 adds margins to the THEAD and TBODY and their children</li>");
	printF(L"		<li>On Konqueror 3.x the scrollbar may be slightly off.</li>");
	printF(L"	</ul>\n");

	printF(L"	<h3>Updates:</h3>\n");

	printF(L"	<ul>");
	printF(L"		<li>2004.10.15 11am: Fixed incorrect comments on Opera filtering</li>");
	printF(L"		<li>2004.10.15 11am: Added link to Bullet Resistant Version</li>");
	printF(L"		<li>2004.11.02 01pm: Fixed incorrect width on 2nd Cell. Was 250px, should be 240px.</li>");
	printF(L"		<li>2004.11.02 01pm: Updated Browser support list.</li>");
	printF(L"	</ul>");
	printF(L"</div>\n\n");


	printF(L"<div>");
	printF(L"<br /><br /><br /><br /><br /><br /><br /><br /><br / ><br /><br /><br />");
	printF(L"put a bunch of breaks to test scrolling within the HTML document itself.");
	printF(L"<br /><br /><br /><br /><br /><br /><br /><br /><br / ><br /><br /><br />");
	printF(L"put a bunch of breaks to test scrolling within the HTML document itself.");
	printF(L"<br /><br /><br /><br /><br /><br /><br /><br /><br / ><br /><br /><br />");
	printF(L"put a bunch of breaks to test scrolling within the HTML document itself.");
	printF(L"<br /><br /><br /><br /><br /><br /><br /><br /><br / ><br /><br /><br />");
	printF(L"done.");
	printF(L"</div>\n");

	printF(L"</body></html>\n");


	fclose(fhfixHeader);
	ShellExecute(NULL, L"open", m_fixHeaderFile, NULL, NULL, SW_SHOWNORMAL);

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printF(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fhfixHeader, L"%s\n", str);
	fflush(fhfixHeader);
}
