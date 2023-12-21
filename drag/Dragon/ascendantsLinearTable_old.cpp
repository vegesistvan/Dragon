#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"
#include "ascendantsLinearTable.h"
#include "ProgressWnd.h"

// which értékek
enum
{
	Piramid = 0,		// 0
	ListRep,			// 1
	ListNoRep,			// 2	
	TableRep,			// 3
	TableNoRep,			// 4
	Sticki,				// 5	
	FixHeadRep,			// 6
	FixHeadNoRep,		// 7	
	FixHeadRepPrint,	// 8
	FixHeadNoRepPrint,	// 9
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByRRcnt(const FC& v1, const FC& v2)
{
	if (v1.rowidF == v2.rowidF)
		if (v1.rowidC == v2.rowidC)
			return (v1.cnt < v2.cnt);
		else
			return(v1.rowidC < v2.rowidC);
	else
		return(v1.rowidF < v2.rowidF);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedLinear()
{
	UpdateData(FROMSCREEN);
	saveParameters();
//	CDialog::OnCancel();


	if (m_log)
	{
		fs_vGKR2 = theApp.openTextFile(&flvGKR2, L"vGKR2", L"w+");  // log fájl
		fs_vSp = theApp.openTextFile(&flvSp, L"vSp", L"w+");  // log fájl
		fs_vFC = theApp.openTextFile(&flvFC, L"vCF", L"w+");  // log fájl
		fs_vSib = theApp.openTextFile(&flvSib, L"vSib", L"w+");  // log fájl		
	}

	create_vGKR2();		// vGKR kibõvítése ixF és ixF -el és vSp látrahozása
//	correctReferences();

//	fixTableHeader();				// ez a neten talált példa a fix header-re
//	linearList(ListRep);			// lista fájlt hoz létre
//	linearList(ListNoRep);
//	linearTable(TableRep);			// táblázatot hoz létre, régi 
//	linearTable(TableNoRep);

	tableHeaderFix(FixHeadRep);	// Nem fix fejléces táblázatot hoz létre, ami nyomtatható
	correctReferences();
	tableHeaderFix(FixHeadNoRep);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vGKR
// {
// CString	r;					rowid
// CString	name;				név
// CString	birth;				születés
// CString	death;				halál
// int		sex_id				sex
// int		g;					generáció
// _int64	k;					Kekule szám
// int		gcnt				generáción belüli sorszám
// int		cnt;				a rowid ismétlõdésének sorszámaszáma  ( 0 ha nincs ismétlõdés  
// int		cntMax			ismétlõdések száma
// int		ixC					gyerek indexe a vGKR-ben	
// int		ixF					apa indexe a vGKR-ben
//	};
void CAscendants::create_vGKR2()
{
	if (m_log)
		fs_vGKR3 = theApp.openTextFile(&flvGKR3, L"vGKR3", L"w+");  // log fájl
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;

	_int64	k = 1;		// kekülé szám

	int x;
	CProgressWnd wndP3(NULL, L"Apa és elsõ gyerek indexeinek keresése...");
	wndP3.GoModal();
	wndP3.SetRange(0, vGKR.size());
	wndP3.SetStep(1);

	vGKR.at(0).ixF = 1;   /// 1 kellene!!!
	// minden felmenõbe beírjuk elsõ gyerekét és apját
	for (int i = 0; i < vGKR.size(); ++i)
	{
		gkr = vGKR.at(i);
		k = gkr.k / 2;					// gyerek Kekule száma
		for (int j = 0; j < i; ++j)		// megkeressük az elsõ gyerekét, és betesszük annak indexét 
		{
			if (vGKR.at(j).k == k)
			{
				vGKR.at(i).ixC = j;
				break;
			}
		}

		k = gkr.k * 2;								// apa Kekülé száma
		for (int j = i + 1; j < vGKR.size(); ++j)	// megkeressük az apját
		{
			if (vGKR.at(j).k == k)
			{
				vGKR.at(i).ixF = j;						// beírjuk az apa indexét
				vGKR.at(i).ixM = vGKR.at(j).ixSp;		// beírjuk az anya indexét	
			}
		}
		wndP3.StepIt();
		wndP3.PeekAndPump();
		if (wndP3.Cancelled()) break;
	}
	wndP3.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::correctReferences()
{
	int x;
	int z;


	// Az ismételt, tehát törlendõ felmenõkre való hivatkozást ( apa, házastársa, gyerek ) átirányítjuk a megmaradó bejegyzésre
	CProgressWnd wndP3(NULL, L"Ismételt felmenõkre hivatkozások korrigálása...");
	wndP3.SetRange(0, vGKR.size());
	wndP3.SetStep(1);
	
	bool volt;
	for (int i = 1; i < vGKR.size() - 1; ++i)
	{
		if (vGKR.at(i).cnt == 1)				// elsõ elõfordulás, ami megmarad
		{
			for (int j = i; j < vGKR.size(); ++j)
			{
				if (vGKR.at(j).rowid == vGKR.at(i).rowid)		// j a következõ elõfordulás, amit törölni fogunk, ezért j-t töröljük, ezért
				{												// j-re történõ hivatkozásokat keresi, hogy azokat i-re változtassa
					volt = false;
					for (int k = 0; k < vGKR.size(); ++k)		// törlendõ apákra, házastársra, gyerekre történõ hivatkozás korrigálása
					{
						if (vGKR.at(k).ixF == j)
						{
							vGKR.at(k).ixF = i;
							volt = true;
						}
						if (vGKR.at(k).ixC == j)
						{
							vGKR.at(k).ixC = i;
							volt = true;

						}
						if (vGKR.at(k).ixSp == j)
						{
							vGKR.at(k).ixSp = i;
							++vGKR.at(k).numOfSp;
							volt = true;
						}
//						if (volt)
//							break;
					}
				}
			}
		}
		wndP3.StepIt();
		wndP3.PeekAndPump();
		if (wndP3.Cancelled()) break;
	}
	wndP3.DestroyWindow();

	// több házastárs kigyûjtése
	// Ha egy ember többször házasodott, akkor házastársait a vSP vektorba gyûjti
	// SP.ix		// az ember indexe a vGKR vektorban
	// SP.ixSp		// házastársa indexe a VGKR vektorban	



	SP spouse;
	CString rowid1;
	CString rowid2;
	CString rowidSp1;
	CString rowidSp2;
	int ip1;
	int ip2;
	int ix;
	_int64 k;

	CProgressWnd wndP4(NULL, L"Több házastárs kigyûjtése...");
	wndP4.SetRange(0, vGKR.size());
	wndP4.SetStep(1);
	vSp.clear();
	for (int i = 1; i < vGKR.size()-1; ++i)
	{
		if (vGKR.at(i).cntMax < 2) continue;				//  Ha csak egyszer fordul elõ, akkor annak nem lehet több házastársa
		if (vGKR.at(i).cnt > 1) continue;					// csak az elsõ elõfordulást vizsgálja
//		if (vGKR.at(vGKR.at(i).ixSp).ixSp != i) continue;		// a házastársa nem õrá mutat, vagy nincs is házastárs tovább
		if (vGKR.at(i).ixSp == 0) continue;					// nincs házastársa


		if (i == 11)
			z = 1;
		rowid1 = vGKR.at(i).rowid;							// ember rowid-ja
		if (rowid1 == L"33")
			z = 2;

		rowidSp1 = vGKR.at(vGKR.at(i).ixSp).rowid;			// házastárs rowid-je
		spouse.ix = i;
		spouse.ixSp = vGKR.at(i).ixSp;
//		vSp.push_back(spouse);

		// megkeresi az ember (rowid1) további elõfordulásait
		for (int j = i + 1; j < vGKR.size() - 1; ++j)
		{
			rowid2 = vGKR.at(j).rowid;						
			if (rowid2 == L"82335")
				ix = 2;

			if (rowid2 == rowid1)						// ez ember újabb elõfordulása
			{
				k = vGKR.at(j).k;
				if (k % 2 == 0)							// páros, tehát férj
				{
					ip2 = j + 1;						// házastársa utána van, indexe a vGKR-ben
					if (vGKR.at(ip2).k != k + 1)		// nem az õ felesége
						continue;
				}
				else
				{
					ip2 = j - 1;						// páratlan, tehát feleség, házastársa elõtte van
					if (vGKR.at(ip2).k != k - 1)		// nem a férje
						continue;
				}

				rowidSp2 = vGKR.at(ip2).rowid;
				if (rowidSp1 != rowidSp2)				// más házastárs!
				{
					spouse.ix = i;
					spouse.ixSp = ip2;
					vSp.push_back(spouse);
				}
			}
		}
		wndP4.StepIt();
		wndP4.PeekAndPump();
		if (wndP4.Cancelled()) break;
	}

	int n;
	
	GKR gkri;
	GKR gkrj;
	SIBLINGS sib;
	bool first;

	vSib.clear();

	ip1 = vSp.size();
	wndP4.DestroyWindow();

	CProgressWnd wndP5(NULL, L"Testvérek kigyûjtése...");
	wndP5.SetRange(0, vGKR.size());
	wndP5.SetPos(0);
	wndP5.SetStep(1);
	for (int i = 1; i < vGKR.size() - 1; ++i)
	{
		gkri = vGKR.at(i);
		if (i == 1091)
			z = 1;
		for (int j = i + 1; j < vGKR.size(); ++j)
		{
			gkrj = vGKR.at(j);
			if (gkrj.cnt < 2 && gkrj.ixF != 0)		// nem ugyanaz, nem ismételt és van apja
			{
				if ((vGKR.at(gkri.ixF).rowid == vGKR.at(gkrj.ixF).rowid) && gkri.rowid != gkrj.rowid)	// testvérek, ha az apák azonosak, és nem ismétlõdõ 
				{
					sib.ix = i;					// testvér vica-versa
					sib.ixSib = j;
					sib.ixF = gkri.ixF;
					sib.listed = false;
					vSib.push_back(sib);

					sib.ix = j;
					sib.ixSib = i;
					sib.ixF = gkrj.ixF;
					sib.listed = false;
					vSib.push_back(sib);
				}
			}
		}
		wndP5.StepIt();
		wndP5.PeekAndPump();
		if (wndP5.Cancelled()) break;
	}
	wndP5.DestroyWindow();

	if (m_log)
	{
		printvGKR2();
		fclose(flvGKR2);
		theApp.notepad(fs_vGKR2, L"");

		printvSib();
		fclose(flvSib);
		theApp.notepad(fs_vSib, L"");

		printvSp();
		fclose(flvSp);
		theApp.notepad(fs_vSp, L"");

		printvFC();
		fclose(flvFC);
		theApp.notepad(fs_vFC, L"");
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::tableHeaderFix(int which)
{
	CString filename;
	CString title;
	int tableTop = 205;
	int height	= theApp._h - tableTop;
	int width	= theApp._w;

	
	if (which == FixHeadRep || which == FixHeadRepPrint)
	{
		m_title.Format(L"%s felmenõinek lineáris táblája", m_rootName);
		filename = m_title;
		m_fixHeaderFileRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhfixHeaderRep, m_fixHeaderFileRep, L"w+")) return false;
	}
	else if (which == FixHeadNoRep || which == FixHeadNoRepPrint)
	{
		m_title.Format(L"%s felmenõinek lineáris táblája ismétlõdések kihagyásával", m_rootName);
		filename = m_title;
		m_fixHeaderFileNoRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhfixHeaderNoRep, m_fixHeaderFileNoRep, L"w+")) return false;
	}

	printOnly(L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">", which);
	printOnly(L"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"><head>", which);
	str.Format(L"<title>%s</title>", m_title);
	printOnly(str, which);
	printOnly(L"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />", which);
	printOnly(L"<meta http-equiv=\"language\" content=\"en-us\"/>", which);
	printOnly(L"<script type=\"text/javascript\">", which);

	if (which == FixHeadRep || which == FixHeadNoRep )
		printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", which);

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
	if (which < FixHeadRep)  // 8
		str.Format(L"width: %dpx", width);  // ez számít!!
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
	if (which < 8)
	if( which == FixHeadRep || which == FixHeadNoRep )
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

	if (which == FixHeadRep || which == FixHeadRepPrint)
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
		str.Format(L"width: %dpx", width - (2 * 55 + 2 * 35 ));
		printOnly(str, which);
		printOnly(L"}", which);

		//	printOnly(L"-->", which);

		printOnly(L"</style>", which);
		printOnly(L"</head><body>\n", which);

		printTopContainer(title, which);

		printOnly(L"<div id=\"tableContainer\" class=\"tableContainer\">", which);
		printOnly(L"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" class=\"scrollTable\">", which);

		printOnly(L"<thead class=\"fixedHeader\">", which);
		printOnly(L"<tr>", which);

		printOnly(L"<th style=\"width: 35px\">#</th>", which);
		printOnly(L"<th style=\"width: 35px\">ism.</th>", which);
		printOnly(L"<th style=\"width: 55px\">apa</th>", which);
		printOnly(L"<th style=\"width: 55px\">felmenõ</th>", which);
		str.Format(L"<th style=\"width: %dpx\">felmenõ</th>", width - (2 * 55 + 2 * 35));
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

		//	printOnly(L"-->", which);

		printOnly(L"</style>", which);
		printOnly(L"</head><body>\n", which);

		printTopContainer(title, which);

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
		printOnly(L"<th style=\"width: 55px\">felmenõ</th>", which);
		str.Format(L"<th style=\"width: %dpx\">felmenõ</th>", width - (5 * 55 + 2 * 35));
		printOnly(str, which);

	}


	printOnly(L"</tr>", which);
	printOnly(L"</thead>", which);
	
	printOnly(L"<tbody class=\"scrollContent\">", which);

	dataTable(which);

	printOnly(L"</tbody>", which);
	printOnly(L"</table>", which);
	printOnly(L"</div>\n", which);

	printOnly(L"<div><br/ ></div>\n", which);
	printOnly(L"</body></html>\n", which);


	if (which == FixHeadRep || which == FixHeadRepPrint)
	{
		fclose(fhfixHeaderRep);
		ShellExecute(NULL, L"open", m_fixHeaderFileRep, NULL, NULL, SW_SHOWNORMAL);
	}
	if (which == FixHeadNoRep || which == FixHeadNoRepPrint )
	{
		fclose(fhfixHeaderNoRep);
		ShellExecute(NULL, L"open", m_fixHeaderFileNoRep, NULL, NULL, SW_SHOWNORMAL);
	}
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::dataTable(int which)
{
	int gPrev = 0;
	int ix;
	int ixC;
	int ixSib;
	int ixSp;
	int pos;
	GKR gkr;
	int limit;
	int cnt = 0;
	bool gflag = true;		// generáció váltás figyelésére
	bool moreC;
	CString siblings;
	CString children;
	CString spouses;
	CString gcC;


	m_gPrev = vGKR.at(0).g;
	m_gFlag = false;
	CProgressWnd wndP(NULL, L"Táblázatos teljes lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR.size());

	for (int i = 0; i < vGKR.size(); ++i)
	{
		gkr = vGKR.at(i);
		if (which == 3 || which == 6 || which == 8)
			limit = INT_MAX;
		else
			limit = 1;
		if (gkr.cnt > limit) goto cont;  // ismételteket átengedi vagy nem
		spouses.Empty();
		siblings.Empty();
		moreC = false;

		if (i != 0)
		{
			// elsõ gyerek
			ixC = gkr.ixC;
			children.Format(L"%3c-%-4d", vGKR.at(ixC).gABC, vGKR.at(ixC).gcnt);


			// testvérek és gyerekek listája
			for (int j = 0; j < vSib.size(); ++j)
			{
				// testvérek
				ixSib = vSib.at(j).ixSib;
				if (vSib.at(j).ix == i && vGKR.at(ixSib).cnt < 2)  // õ maga a leszármazott gyereke, testvére nem ismétlõdõ
				{
					str.Format(L"%3c-%-4d", vGKR.at(ixSib).gABC, vGKR.at(ixSib).gcnt);
					siblings += str;
					siblings += L"<br>";
				}

				// gyerekek
				if (vSib.at(j).ixF == i && vGKR.at(vSib.at(j).ix).cnt < 2)		// testvér apja, aki nem ismétlõdõ
				{
					ix = vSib.at(j).ix;
					gcC.Format(L"%3c-%-4d", vGKR.at(ix).gABC, vGKR.at(ix).gcnt);  // van gyerek
					if ((pos = children.Find(gcC)) == -1)
					{
						children += L"<br>";		// ha még nincs ez a gyerek, akkor hozzáadja
						children += gcC;
						moreC = true;

					}
				}

			}
		}


		// további házastársak listája
		if (gkr.numOfSp > 1 && (which == FixHeadNoRep || which == FixHeadNoRepPrint ) )
		{
			for (int j = 0; j < vSp.size(); ++j)
			{
				if (vSp.at(j).ix == i && vGKR.at(vSp.at(j).ixSp).cnt < 2)		// a hozzá tartozó házaspárok keresése)	
				{
					ixSp = vSp.at(j).ixSp;
					str.Format(L"%3c-%-4d", vGKR.at(ixSp).gABC, vGKR.at(ixSp).gcnt);
					if ((pos = spouses.Find(str)) == -1)
					{
						spouses += str;
						spouses += L"<br>";
					}
				}

				if (vSp.at(j).ixSp == i && vGKR.at(i).cnt < 2)		// a hozzá tartozó házaspárok keresése
				{
					ixSp = vSp.at(j).ix;
					str.Format(L"%3c-%-4d", vGKR.at(ixSp).gABC, vGKR.at(ixSp).gcnt);
					if ((pos = spouses.Find(str)) == -1)
					{
						spouses += str;
						spouses += L"<br>";
					}
				}

			}
		}
		else
		{
			if (i && gkr.numOfSp == 0 )
				spouses = L"n/a";
		}

		createAscendant(i, i, which );
		int pos;
		if (gkr.g != gPrev)
			gflag = !gflag;

		if (gflag)
			printOnly(L"<tr style=\"background-color: #fff;\">", which);
		else
			printOnly(L"<tr style=\"background-color: #eee;\">", which);




		str.Format(L"<td><center>%d</center></td>", cnt);
		printOnly(str, which);

		str.Format(L"<td><center>%s</center></td>", col.rep);
		printOnly(str, which);
				
		if (which == FixHeadRep || which == FixHeadRepPrint)
		{
			str.Format(L"<td><center>%s</center></td>", col.idF);
			printOnly(str, which);
		}
		else
		{
			str.Format(L"<td><center>%s</center></td>", siblings);
			printOnly(str, which);


			str.Format(L"<td><center>%s</center></td>", spouses);
			printOnly(str, which);

			str.Format(L"<td><center>%s</center></td>", col.idM);
			printOnly(str, which);

			str.Format(L"<td><center>%s</center></td>", col.idF);
			printOnly(str, which);

			if (moreC)
				str.Format(L"<td style=\"background-color: black;\"><font color=\"white\"><center>%s</center></font></td>", children);
			else
				str.Format(L"<td><center>%s</center></td>", children);
			printOnly(str, which);
		}
		str.Format(L"<td><center>%s</center></td>", col.idA);
		printOnly(str, which);

		str.Format(L"<td>%s</td>", col.ascString);
		printOnly(str, which);

		printOnly(L"</tr>", which);

		++cnt;
		gPrev = gkr.g;

	cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR CAscendants::get_gABC(int g)
{
	TCHAR gABC;
	int ix = g / 26;		  // ix = 0, ha g kisebb mint 26, = 1 Ha nagyobb
	gABC = TCHAR('A') + ix * 6 + g;   // 6 : a 'Z' és 'a' közötti karakterek száma,
	return gABC;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::createAscendant(int i, int cnt, int which)
{
	CString color;
	bool lineY = false;
	int kPrev = 0;
	GKR gkr = vGKR.at(i);
	int g = gkr.g;
	_int64 k = gkr.k;
	_int64 kFirstG;
	bool gFlag;
	int z;


	/*
	if (m_checkColor == true )	// ismétlõdõk kékkel
	{
		ix = vGKR.at(i).cnt - 1;
		if (ix >= 0)
		{
			j = ix % m_numOfColors;
			color = CK::colors[j].color;
			people.Format(L"<font color=\"blue\">%s</font>", (CString)people);
		}
	}
*/
	col.cnt = 0;
	col.rep.Empty();
	col.idA.Empty();
	col.idC.Empty();
	col.idF.Empty();
	col.idM.Empty();
	col.idSp.Empty();
	col.idSib.Empty();
	col.ascString = getPeopleString(i);

	// sorszám
	col.cnt = cnt;

	// a felmenõ elõfordulásának száma
	if (gkr.cnt)
		col.rep.Format(L"%3d/%-3d", gkr.cnt, gkr.cntMax);

	// 1 db testvére ( a fõsorba kerül, a többi alá

	for (int j = 0; j < vSib.size(); ++j)
	{
		if (vSib.at(j).ix == i)
		{
			col.idSib.Format(L"%3d-%-4d", vGKR.at(vSib.at(j).ixSib).gABC, vGKR.at(vSib.at(j).ixSib).gcnt );
			vSib.at(j).listed = true;
			break;		// csak az elsõ testvérét teszi bele
		}
	}

	// apja
	if (!i || vGKR.at(gkr.ixF).g)
	{
		col.idF.Format(L"%3c-%-4I64d", vGKR.at(gkr.ixF).gABC, vGKR.at(gkr.ixF).gcnt); // apa g-cnt elkészítése
	}

	// anyja
	col.idM = L"";
	if (which == FixHeadNoRep || which == FixHeadNoRepPrint)
	{
		if (!i || vGKR.at(gkr.ixM).g)
		{
			if (vGKR.at(gkr.ixF).gcnt + 1 != vGKR.at(gkr.ixM).gcnt)   ///nme a stadard anya
			{
				col.idM.Format(L"%3c-%-4I64d", vGKR.at(gkr.ixM).gABC, vGKR.at(gkr.ixM).gcnt); // apa g-cnt elkészítése
			}
		}
	}

	// felmenõ azonosítója
	col.idA.Format(L"<b>%3c-%-4I64d</b>", gkr.gABC, gkr.gcnt);

	if (gkr.g != m_gPrev)
		m_gFlag = !m_gFlag;

	if ( gkr.g && m_gFlag)
		col.idA.Format(L"<font color=\"blue\">%s</font>", CString(col.idA) );
	else
		col.idA.Format(L"<font color=\"red\">%s</font>", CString(col.idA));



	// házastársa. Csak akkor, ha nem a k, k+1 házastár

	if (gkr.g == 9 && gkr.cnt == 94)
		z = 1;
	if (i == 117)
		z = 1;
	if (gkr.numOfSp)
	{
		if (i && i < vGKR.size() - 1)
		{
			if ((gkr.sex_id == 1) && ((gkr.k + 1) != vGKR.at(gkr.ixSp).k) || (gkr.sex_id == 2) && (gkr.k != vGKR.at(gkr.ixSp).k + 1))
			{
				col.idSp.Format(L"<font color=\"blue\">%3c-%-4d</font>", vGKR.at(gkr.ixSp).gABC, vGKR.at(gkr.ixSp).gcnt);  // házastárs g-cnt elkészítése
			}
		}
	}
	else
		col.idSp = L"n/a   ";

	// gyereke
	if (gkr.ixC > -1)
	{
		col.idC.Format(L"%3c-%-4I64d", vGKR.at(gkr.ixC).gABC, vGKR.at(gkr.ixC).gcnt); // gyerek g-cnt elkészítése
	}

	if (which == ListRep)
		str.Format(L"%4d|%8s|%8s|%8s|%8s|%8s|%8s|%s", col.cnt, col.rep, col.idSib, col.idSp, col.idF, col.idC, col.idA, col.ascString);
	else
		str.Format(L"%4d|%4d|%8s|%8s|%8s|%8s|%8s|%8s|%s", i, col.cnt, col.rep, col.idSib, col.idSp, col.idF, col.idC, col.idA, col.ascString);

	m_gPrev = gkr.g;
	return(str);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Régi táblázatos lista. 
bool CAscendants::linearTable(int which)
{
	CString filename;
	CString title;

	if (which == 3)
	{
		title.Format(L"%s felmenõinek lineáris tábája", m_rootName);
		filename = L"Lineáris táblázat";
		m_linearTableFileRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhLinTabRep, m_linearTableFileRep, L"w+")) return false;
	}
	else if (which == 4)
	{
		title.Format(L"%s felmenõinek lineáris táblája ismétlõdések kihagyásával", m_rootName);
		filename = L"Lineáris lista kihagyva";
		m_linearTableFileNoRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhLinTabNoRep, m_linearTableFileNoRep, L"w+")) return false;

	}

	printOnly(L"<!DOCTYPE html>", which);
	printOnly(L"<html>", which);
	printOnly(L"<head>", which);
	printOnly(L"<meta charset=\"UTF-8\">", which);
	//	printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", which);

	printOnly(L"<style>", which);

	printOnly(L"bY{ background: yellow; }", which);
	printOnly(L".wrapper {", which);
	printOnly(L"border: 1px solid #ccc;", which);
	//	printOnly(L"background: #eee;", which);
	//	printOnly(L"width: 320px;", which);
		//		print4(L"width: 1000px;", which);
	//	printOnly(L"height: 200px;", which);
	printOnly(L"overflow: auto;", which);
	printOnly(L"position: relative;", which);
	printOnly(L"}", which);

	printOnly(L"table {", which);
	printOnly(L"border-spacing: 0;", which);
	printOnly(L"white-space: nowrap;", which);
	printOnly(L"table-layout: fixed;", which);
	printOnly(L"}", which);

	printOnly(L"thead,", which);
	printOnly(L"tr>th {", which);
	printOnly(L"position: sticky;", which);
	printOnly(L"background: AntiqueWhite;", which);
	printOnly(L"}", which);

	/*

		printOnly(L"thead {", which);
		printOnly(L"top: 0;", which);
		printOnly(L"z-index: 2;", which);
		printOnly(L"}", which);

		printOnly(L"tr>th {", which);
		printOnly(L"left: 0;", which);
		printOnly(L"z-index: 1;", which);
		printOnly(L"}", which);

		printOnly(L"thead tr>th:first-child {", which);
		printOnly(L"z-index: 3;", which);
		printOnly(L"}", which);
	*/
	printOnly(L"th,", which);
	printOnly(L"td{", which);
	//	printOnly(L"height: 50px;", which);
	printOnly(L"border: 1px solid #ccc;", which);
	printOnly(L"border-width: 0 0 1px 1px;", which);
	//	printOnly(L"text-align: left;", which);
	printOnly(L"padding: 10px;", which);
	printOnly(L"font-family: sans-serif;", which);
	printOnly(L"}", which);

	/*
		printOnly(L"td {", which);
		printOnly(L"background: #fff;", which);
		printOnly(L"}", which);

		printOnly(L"th:first-child {", which);
		printOnly(L"border-right-width: 1px;", which);
		printOnly(L"border-left: 0;", which);
		printOnly(L"}", which);

		printOnly(L"th+td, th:first-child+th {", which);
		printOnly(L"border-left: 0;", which);
		printOnly(L"}", which);

		printOnly(L"tbody tr:last-child>* {", which);
		printOnly(L"border-bottom: 0;", which);
		printOnly(L"}", which);

		printOnly(L"tr>*:last-child {", which);
		printOnly(L"border-right: 0;", which);
		printOnly(L"}", which);
	*/
	printOnly(L"</style>}", which);
	printOnly(L"</head>}", which);
	printOnly(L"<body>}", which);

	printTopContainer(title, which);

	printOnly(L"<div class =\"wrapper\">", which);
	printOnly(L"<table>", which);
	printOnly(L"<thead>", which);
	printOnly(L"<tr>", which);				// start of table row
	printOnly(L"<th>#</th>", which);
	printOnly(L"<th>ism.</th>", which);
	printOnly(L"<th style=\"width: 50px\">testvér</th>", which);
	printOnly(L"<th style=\"width: 50px\">hzstárs</th>", which);
	printOnly(L"<th style=\"width: 50px\">apa</th>", which);
	printOnly(L"<th style=\"width: 50px\">gyerek</th>", which);
	printOnly(L"<th style=\"width: 50px\">felmenõ</th>", which);
	printOnly(L"<th>felmenõk</th>", which);
	printOnly(L"</tr>", which);			// end of table row	
	printOnly(L"</thead>", which);
	printOnly(L"</tbody>", which);

	dataTable(which);

	printOnly(L"</tbody>", which);
	printOnly(L"</table>", which);
	printOnly(L"</div>", which);
	printOnly(L"</body>", which);
	printOnly(L"</html>", which);

	if (which == TableRep)
	{
		fclose(fhLinTabRep);
		ShellExecute(NULL, L"open", m_linearTableFileRep, NULL, NULL, SW_SHOWNORMAL);
	}
	else if( which == TableNoRep )
	{
		fclose(fhLinTabNoRep);
		ShellExecute(NULL, L"open", m_linearTableFileNoRep, NULL, NULL, SW_SHOWNORMAL);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CAscendants::linearTableNoRep()
{
	print4(L"<tbody>");

	print4(L"<tr>");
	print4(L"<th><row 1</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll<br>scroll 2 rows</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 2</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll wide wide wide</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 3</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 4</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 5</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 6</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");
/*
	print4(L"<tr>");
	print4(L"<th><row 7</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 8</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 9</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 10</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 11</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 12</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 13</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");

	print4(L"<tr>");
	print4(L"<th><row 13</th>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"<td>scroll</td>");
	print4(L"</tr>");
*/
/*
	print4(L"</tbody>");
	print4(L"</table>");
	print4(L"</div>");

	print4(L"</body>");
	print4(L"</html>");
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Régi listázós kimenet
bool CAscendants::linearList(int which)
{
	CString filename;
	CString title;
	if (which == 1)
	{
		title.Format(L"%s felmenõinek teljes listája", m_rootName);
		filename = L"Teljes lineáris lista";
		m_linearListFileRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhLinListRep, m_linearListFileRep, L"w+")) return false;
	}
	else if (which == 2)
	{
		title.Format(L"%s felmenõinek listája ismétlõdések kihagyásával", m_rootName);
		filename = L"Lineáris lista ismétlések kihagyásával";
		m_linearListFileNoRep.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
		if (!openFileSpec(&fhLinListNoRep, m_linearListFileNoRep, L"w+")) return false;
	}

	printOnly(L"<!DOCTYPE html>", which);
	printOnly(L"<html>", which);
	printOnly(L"<head>", which);
	printOnly(L"<meta charset=\"UTF-8\">", which);
	printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", which);

	printOnly(L"<style>", which);
	printOnly(L"body {", which);
	printOnly(L"margin: 0;", which);
	printOnly(L"}", which);

	printOnly(L".top-container {", which);
	printOnly(L"padding: 16px;", which);
	printOnly(L"}", which);

	printOnly(L".header {", which);
	printOnly(L"padding: 16px 0px;", which);
	printOnly(L"background: LightGray;", which);
	printOnly(L"}", which);

	printOnly(L".content {", which);
	printOnly(L"padding: 16px 0px;", which);
	printOnly(L"}", which);

	printOnly(L".sticky {", which);
	printOnly(L"position: fixed;", which);
	printOnly(L"top : 0;", which);
	printOnly(L"width: 100%;", which);
	printOnly(L"}", which);

	//	printOnly(L".sticky + .content {", which);
		//	printOnly(L"padding-top: 102px;", which);
	//	printOnly(L"}", which);

	printOnly(L"</style>", which);
	printOnly(L"</head>", which);
	printOnly(L"<body>", which);

	printTopContainer(title, which);

	printOnly(L"<div class = \"header\" id = \"myHeader\">", which);
	printOnly(L"<pre>", which);
	str.Format(L"<b>%4s|%4s|%-8s|%-35s|</b>", L" ", L" ", L"ismétlés", L"        azonosító: generáció-sorszám        ", which);
	printOnly(str, which);
	str.Format(L"<b>%4s|%4s|%-8s|%-8s|%-8s|%-8s|%-8s|%-8s|</b>", L"ix", L"#", L"sorsz.", L"testvér", L"házastrs", L"apa", L"gyerek", L"felmenõ", which);
	printOnly(str, which);
	printOnly(m_sep, which);
	printOnly(L"</pre>", which);
	printOnly(L"</div>", which);

	printOnly(L"<div class=\"content\">", which);
	dataList(which);
	printOnly(L"</div>", which);

	printOnly(L"<script>", which);
	printOnly(L"window.onscroll = function() { myFunction() };", which);

	printOnly(L"var header = document.getElementById(\"myHeader\");", which);
	printOnly(L"var sticky = header.offsetTop;", which);

	printOnly(L"function myFunction() {", which);
	printOnly(L"if (window.pageYOffset > sticky) {", which);
	printOnly(L"header.classList.add(\"sticky\");", which);
	printOnly(L"}", which);
	printOnly(L"else {", which);
	printOnly(L"header.classList.remove(\"sticky\");", which);
	printOnly(L"}", which);
	printOnly(L"}", which);
	printOnly(L"</script>", which);

	printOnly(L"</body>", which);
	printOnly(L"</html>", which);

	if (which == ListRep)
	{
		fclose(fhLinListRep);
		ShellExecute(NULL, L"open", m_linearListFileRep, NULL, NULL, SW_SHOWNORMAL);
	}
	else if (which == ListNoRep)
	{
		fclose(fhLinListNoRep);
		ShellExecute(NULL, L"open", m_linearListFileNoRep, NULL, NULL, SW_SHOWNORMAL);
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::dataList(int which)
{
	GKR gkr;
	CString gcC;
	CString gcP;
	int ixSp;
	int ixC;
	int limit;
	int z;

	if (which == 1)
		limit = INT_MAX;
	else
		limit = 2;


	int gPrev = 0;
	int cnt = 0;
	CProgressWnd wndP(NULL, L"Lineáris lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR.size());

	printOnly(L"<pre>", which);
	for (int i = 0; i < vGKR.size(); ++i)
	{
		gkr = vGKR.at(i);
		if (gkr.cnt < limit)		// Csak az elsõ elõfordulást írja ki
		{
			str = createAscendant(i, cnt, 2);
			++cnt;
			if (gkr.g != gPrev)
				printOnly(m_sep, which);
			printOnly(str, which);

			// további házastársak
			if (gkr.numOfSp)			// több házastársa van
			{
				for (int j = 0; j < vSp.size(); ++j)
				{
					if (vSp.at(j).ix == i && vGKR.at(vSp.at(j).ixSp).cnt < 2)		// a hozzá tartozó házaspárok keresése
					{
						ixSp = vSp.at(j).ixSp;
						gcP.Format(L"%3d-%-4d", vGKR.at(ixSp).g, vGKR.at(ixSp).gcnt);
						str.Format(L"%4s|%4s|%8s|%8s|%8s|%8s|%8s|%8s|", L" ", L" ", L" ", L" ", gcP, L" ", L" ", L" ");
						printOnly(str, which);
					}
					if (vSp.at(j).ixSp == i && vGKR.at(i).cnt < 2)		// a hozzá tartozó házaspárok keresése
					{
						ixSp = vSp.at(j).ix;
						gcP.Format(L"%3d-%-4d", vGKR.at(ixSp).g, vGKR.at(ixSp).gcnt);
						str.Format(L"%4s|%4s|%8s|%8s|%8s|%8s|%8s|%8s|", L" ", L" ", L" ", L" ", gcP, L" ", L" ", L" ");
						printOnly(str, which);
					}
				}
			}
			if (i == 132)
				z = 1;

			bool found;
			CStringArray A;
			if (vGKR.at(i).sex_id == 1)		// csak az apákhoz írja fel a további gyerekeket és házastársakat ????
			{
				A.RemoveAll();
				found = false;
				for (int j = 0; j < vSib.size(); ++j)
				{
					if (vSib.at(j).ixF == i && vGKR.at(vSib.at(j).ixF).ixC != vSib.at(j).ix)
					{
						gcC.Format(L"<b><font color=\"blue\">%3d-%-4d</font></b>", vGKR.at(vSib.at(j).ix).g, vGKR.at(vSib.at(j).ix).gcnt);  // van gyerek
						str.Format(L"%4s|%4s|%8s|%8s|%8s|%8s|%8s|%8s|", L" ", L" ", L" ", L" ", L" ", L" ", gcC, L" ");
						for (int jj = 0; jj < A.GetCount(); ++jj)
						{
							if (A.GetAt(jj) == gcC)
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							printOnly(str, which);
							A.Add(gcC);
						}

					}
				}
			}
			CString gcSib;
			for (int j = 0; j < vSib.size(); ++j)
			{
				if (vSib.at(j).ix == i && vSib.at(j).listed == false && vGKR.at(vSib.at(j).ixSib).cnt < 2)
				{
					gcSib.Format(L"%3d-%-4d", vGKR.at(vSib.at(j).ixSib).g, vGKR.at(vSib.at(j).ixSib).gcnt);
					str.Format(L"%4s|%4s|%8s|%8s|%8s|%8s|%8s|%8s|", L" ", L" ", L" ", gcSib, L" ", L" ", L" ", L" ");
					printOnly(str, which);
				}
			}
		}
		gPrev = gkr.g;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	printOnly(L"</pre>\br", which);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kisérleti minta
bool CAscendants::sticky()
{
	CString title;
	title.Format(L"Sticky lista");
	CString filename;
	filename = title;

	m_stickyFile.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
	if (!openFileSpec(&fhSticky, m_stickyFile, L"w+")) return false;


	printS(L"<!DOCTYPE html>");
	printS(L"<html>");
	printS(L"<head>");
	printS(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">");
	printS(L"<style>");
	printS(L"body {");
	printS(L"margin: 0;");
	printS(L"font - family: Arial, Helvetica, sans - serif;");
	printS(L"}");

	printS(L".top - container {");
	printS(L"background-color: #f1f1f1;");
	printS(L"padding: 30px;");
	printS(L"text-align: center;");
	printS(L"}");

	printS(L".header {");
	printS(L"padding: 10px 16px;");
	printS(L"background: #555;");
	printS(L"color: #f1f1f1;");
	printS(L"}");

	printS(L".content {");
	printS(L"padding: 16px;");
	printS(L"}");

	printS(L".sticky {");
	printS(L"position: fixed;");
	printS(L"top : 0;");
	printS(L"width: 100%;");
	printS(L"}");

	printS(L".sticky + .content {");
	printS(L"padding-top: 102px;");
	printS(L"}");
	printS(L"</style>");
	printS(L"</head>");
	printS(L"<body>");

	printS(L"<div class=\"top-container\">");
	printS(L"<h1>Scroll Down</h1>");
	printS(L"<p>Scroll down to see the sticky effect.</p>");
	printS(L"</div>");

	printS(L"<div class = \"header\" id = \"myHeader\">");
	printS(L"<h2>My Header</h2>");
	printS(L"</div>");
	printS(L"<div class=\"content\">");
	printS(L"<h3>On Scroll Sticky Header</h3>");
	printS(L"<p>The header will stick to the top when you reach its scroll position.</p>");
	printS(L"<p>Scroll back up to remove the sticky effect.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"<p>Some text to enable scrolling..Lorem ipsum dolor sit amet, illum definitiones no quo, maluisset concludaturque et eum, altera fabulas ut quo.Atqui causae gloriatur ius te, id agam omnis evertitur eum.Affert laboramus repudiandae nec et.Inciderint efficiantur his ad.Eum no molestiae voluptatibus.</p>");
	printS(L"</div>");


	printS(L"<script>");
	printS(L"window.onscroll = function() { myFunction() };");

	printS(L"var header = document.getElementById(\"myHeader\");");
	printS(L"var sticky = header.offsetTop;");

	printS(L"function myFunction() {");
	printS(L"if (window.pageYOffset > sticky) {");
	printS(L"header.classList.add(\"sticky\");");
	printS(L"}");
	printS(L"else {");
	printS(L"header.classList.remove(\"sticky\");");
	printS(L"}");
	printS(L"}");
	printS(L"</script>");

	printS(L"</body>");
	printS(L"</html>");

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printvSib()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvSib, L"%5s %5s %5s\n", L"ix", L"ixSib", L"ixF");
	for (int i = 0; i < vSib.size(); ++i)
	{
		fwprintf(flvSib, L"%5d.%5d %5d %5d\n", i, vSib.at(i).ix, vSib.at(i).ixSib, vSib.at(i).ixF);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printvSp()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvSp, L"%6s %5s %5s\n", L" ", L"ix", L"ixSp");
	for (int i = 0; i < vSp.size(); ++i)
	{
		fwprintf(flvSp, L"%5d.%5d %5d\n", i, vSp.at(i).ix, vSp.at(i).ixSp);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printvGKR2()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvGKR2, L"%10s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s %10s %10s|\
				   %7s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s %10s\n", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"maxc", L"cnt", L"ixC", L"ixF", L"ixSp", L"numOfSp", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"maxc", L"cnt", L"ixC", L"ixF", L"ixSp");
	fwprintf(flvGKR2, L"-------------------------------------------------------------------------------------------\n");

	gPrev = vGKR.at(0).g;
	gkr = vGKR.at(0);
	str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %4d %10d %4d %10d %10d\n", \
		0, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.cntMax, gkr.gcnt, gkr.k, gkr.cnt, gkr.ixC, gkr.numOfSp);
	fwprintf(flvGKR2, str);

	for (int i = 1; i < vGKR.size(); ++i)
	{
		gkr = vGKR.at(i);

		j = gkr.ixC;			// gyereke
		if (j < 0 || j > vGKR.size())
		{
			x = 1;
			return;
		}
		gkrC = vGKR.at(j);

		j = gkr.ixF;
		if (j < 0 || j > vGKR.size())
		{
			x = 1;
			return;
		}
		gkrF = vGKR.at(j);

		if (gkr.g != gPrev)
			fwprintf(flvGKR2, L"-------------------------------------------------------------------------------------------\n");
		str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d %10d %10d\
					 %5d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d %10d\n", \
			i, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.gcnt, gkr.k, gkr.cntMax, gkr.cnt, gkr.ixC, gkr.ixF, gkr.ixSp, gkr.numOfSp, \
			j, gkrC.name, gkr.sex_id, gkrC.rowid, gkrC.g, gkrC.gcnt, gkrC.k, gkr.cntMax, gkrC.cnt, gkrC.ixC, gkr.ixF, gkr.ixSp);
		fwprintf(flvGKR2, str);
		gPrev = gkr.g;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printvFC()
{
	fwprintf(flvFC, L"%5s %5s %5s %3s %6s %6s %-30s %s\n", L"i", L"ixF", L"ixC", L"cnt", L"rowidF", L"rowidC", L"apa", L"gyerek");
	for (int i = 0; i < vFC.size(); ++i)
	{
		if (vFC.at(i).ixF == -1)
			fwprintf(flvFC, L"%4d. %5d %5d %3d %6s %6s\n", i, vFC.at(i).ixF, vFC.at(i).ixC, vFC.at(i).cnt, vFC.at(i).rowidF, vFC.at(i).rowidC);
		else
			fwprintf(flvFC, L"%4d. %5d %5d %3d %6s %6s %-30s %s\n", i, vFC.at(i).ixF, vFC.at(i).ixC, vFC.at(i).cnt, vFC.at(i).rowidF, vFC.at(i).rowidC, vGKR.at(vFC.at(i).ixF).name, vGKR.at(vFC.at(i).ixC).name);
	}
}
