#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "dragon.h"

#include "Table_people.h"
#include "Table_firstnames.h"
#include "inputGA.h"
#include "Info_db.h"
#include "Languages.h"
#include "Table_marriages.h"
#include "Table_tables.h"
#include "Table_files.h"
#include "checkFamilyDates.h"
#include "uniteEntries.h"
#include "unitedEntries.h"
#include "html_File.h"
#include "html_Spouses.h"
#include "Brackets.h"
#include "inputGEDCOM.h"
#include "EditPeople.h"
#include "GEDCOM_file.h"
#include "checkSameNameAnd.h"
#include "checkSameSpouses.h"
#include "checkIntegrity.h"
#include "checkUnreferencedItems.h"
#include "checkNames.h"
#include "checkDateFormat.h"
#include "checkFamilyDates.h"
#include "checkLifespan.h"
#include "checkSpousesDiff.h"
#include "checkParentChild.h"
#include "checkMotherDeath.h"
#include "checkFatherDeath.h"
#include "checkSpouseAge.h"
#include "checkGenerations.h"
#include "checkSpousesSex.h"
#include "checkMotherIndex.h"
#include "checkBefore.h"
#include "ConnectCsaladTorzs.h"
#include "GetString.h"
#include "FirstnamesMerge.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// T A B L E S ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateListsAll(CCmdUI* pCmdUI)
{

}
void CDragApp::OnListsAll()
{
	CTablePeople dlg;
	dlg.m_list = ALL;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateListsRowid(CCmdUI* pCmdUI)
{

}
void CDragApp::OnListsRowid()
{
	CTablePeople dlg;
	dlg.m_list = ROWIDPEOPLE;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateFullname(CCmdUI* pCmdUI)
{

}
void CDragApp::OnFullname()
{
	CTablePeople dlg;
	dlg.m_list = FULLNAME;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriageAll(CCmdUI* pCmdUI)
{

}
void CDragApp::OnMarriageAll()
{
	CTableMarriages dlg;
	dlg.m_what = 1;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriageRowid(CCmdUI* pCmdUI)
{

}
void CDragApp::OnMarriageRowid()
{
	CTableMarriages dlg;
	dlg.m_what = 2;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriageName(CCmdUI* pCmdUI)
{
}
void CDragApp::OnMarriageName()
{
	CTableMarriages dlg;
	dlg.m_what = 3;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateFirstnames(CCmdUI* pCmdUI)
{

}
void CDragApp::OnFirstnames()
{
	CTableFirstnames dlg;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
void CDragApp::OnFirstnamesMerge()
{
	CFirstnamesMerge dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// D A T A B A S E  ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateInputSzluha(CCmdUI* pCmdUI)
{
	if (m_inputMode == MANUAL ) pCmdUI->Enable(FALSE);
}
void CDragApp::OnInputSzluha()
{
	CInputGA dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	createCaption();
	m_pMainWnd->SetWindowText(m_caption);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, false);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_UTILITIES, true);



	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK2, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_GAHTML, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_UTILITIES, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, true);


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateInputGedcom(CCmdUI* pCmdUI)
{
	if (m_inputMode == MANUAL ) pCmdUI->Enable(FALSE);
}
void CDragApp::OnInputGedcom()
{
	CInputGEDCOM dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	theApp.m_inputVersion = theApp.m_version;
	theApp.m_inputMode = GEDCOM;
	m_command.Format(L"INSERT INTO properties (inputVersion, inputMode) VALUES ('%s', '%s')", theApp.m_inputVersion, theApp.m_inputMode);
	theApp.execute(m_command);

	createCaption();
	m_pMainWnd->SetWindowText(m_caption);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, false);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_UTILITIES, true);


	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK2, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_GEDCOM, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_UTILITIES, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, true);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateInputManual(CCmdUI* pCmdUI)
{
	if (m_inputMode == GAHTML || m_inputMode == GEDCOM )pCmdUI->Enable(FALSE);
}
void CDragApp::OnInputManual()
{
	CEditPeople dlg;

	dlg.m_rowid.Empty();
	if (dlg.DoModal() == IDCANCEL) return;
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_TABLES, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_CHECK2, true);

	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GAHTML, false);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_GEDCOM, false);
	theApp.pM->m_wndRibbonBar.ShowCategory(CAT_UTILITIES, false);

	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK1, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_MERGE, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_CHECK2, true);
	theApp.pM->m_wndRibbonBar.SetActiveCategory(pCAT_TABLES, true);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnInfoDb()
{
	CInfoDb dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriages(CCmdUI* pCmdUI)
{

}
void CDragApp::OnMarriages()
{
	CTableMarriages dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateTables(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnTables()
{
	CTableTables dlg;
	dlg.m_select = false;
	dlg.m_caption = L"Az adatbázisban lévõ táblák";
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateInputfiles(CCmdUI* pCmdUI)
{
	if ( m_inputMode == MANUAL) pCmdUI->Enable(FALSE);
}
void CDragApp::OnInputfiles()
{
	CTableFiles dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateUnite(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML || !m_uniteVersion.IsEmpty()) pCmdUI->Enable(FALSE);
}
void CDragApp::OnUnite()
{
	CUniteEntries unite;
	if (!unite.parameteres())
		return;
	createCaption();
	m_pMainWnd->SetWindowText(m_caption);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnMergeName3()
{
	OnMergeName();
}
void CDragApp::OnMergeName2()
{
	OnMergeName();
}
void CDragApp::OnMergeName()
{
	CUniteEntries unite;
	unite.GetName();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateUnionShow(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML || m_uniteVersion.IsEmpty()) pCmdUI->Enable(FALSE);
}
void CDragApp::OnUnionShow()
{
	CUnitedEntries dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBrowse(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnBrowse()
{
	CHtmlFile w;
	w.GaOpen();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateNotepad(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnNotepad()
{
	CHtmlFile w;
	w.GaEditor();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   N E T  //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGithub(CCmdUI* pCmdUI)
{

}
void CDragApp::OnGithub()
{
	CString str1("open"), str2("https:://github.com/vegesistvan");
	ShellExecute(NULL, str1, str2, NULL, NULL, SW_SHOWNORMAL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateEmail(CCmdUI* pCmdUI)
{

}
void CDragApp::OnEmail()
{
	CString str1("open"), str2("mailto:veges.istvan@gmail.com ? subject=Hi & cc=");
	ShellExecute(NULL, str1, str2, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// U T I L I T I E S _ G A . H T M L  ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateHtmlInfo(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnHtmlInfo()
{
	str = L"Az itt található mûveletek a program html-fájl beolvasásának, a sorok felbontásának ellenõrzésére szolgálnak.\n\
Ugyanazokat a beolvasási algoritmusokat használja, mint a tényleges beolvasásnál, csak nem teszi be az adatbázisba a beolvasott adatokat.";
	AfxMessageBox(str, MB_ICONINFORMATION);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateDescendantsFile(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnDescendantsFile()
{
	CHtmlFile w;
	w.descendantFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateDescendantsTable(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnDescendantsTable()
{
	CHtmlFile w;
	w.descendantTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateDescendantsLine(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnDescendantsLine()
{
	CHtmlFile w;
	w.descendantLine();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriagesFile(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnMarriagesFile()
{
	CHtmlFile w;
	w.marriagesFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriagesLine(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnMarriagesLine()
{
	CHtmlFile w;
	w.marriagesLine();
	int lineNumber;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMarriagesTable(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnMarriagesTable()
{
	CHtmlFile w;
	w.marriagesTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateSpousesFile(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnSpousesFile()
{
	CHtmlFile w;
	w.ssFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateSpousesLine(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnSpousesLine()
{
	CHtmlFile w;
	w.ssLine();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateSpousesTable(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnSpousesTable()
{
	CHtmlFile w;
	w.ssTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBracketsRound(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnBracketsRound()
{
	CString htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		theApp.selectHtml();
		htmlPathName = theApp.m_htmlPathName;
		if (htmlPathName.IsEmpty()) return;
	}
	CBrackets dlg;
	dlg.m_bracketCode = ROUND;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBracketsSquare(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnBracketsSquare()
{
	CString htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		theApp.selectHtml();
		htmlPathName = theApp.m_htmlPathName;
		if (htmlPathName.IsEmpty()) return;
	}
	CBrackets dlg;
	dlg.m_bracketCode = SQUARE;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBracketsBraces(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML ) pCmdUI->Enable(FALSE);
}
void CDragApp::OnBracketsBraces()
{
	CString htmlPathName = theApp.m_htmlPathName;
	if (htmlPathName.IsEmpty())
	{
		theApp.selectHtml();
		htmlPathName = theApp.m_htmlPathName;
		if (htmlPathName.IsEmpty()) return;
	}

	CBrackets dlg;
	dlg.m_bracketCode = BRACES;
	dlg.m_htmlPathName = htmlPathName;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// U T I L I T I E S _ G E D C O M //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
void CDragApp::OnUpdateGedcomInfo(CCmdUI* pCmdUI)
{
	
}
void CDragApp::OnGedcomInfo()
{
	str = L"Az itt található mûveletek a GEDCOM formátumú fájlok vitsgálatára szolgálnak.\n\
A szabálytalan GEDCOM fájlokból kiszûrhetjük a szabálytalan sorokat beolvasás elõtt.";
	AfxMessageBox(str, MB_ICONINFORMATION);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomList(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomList()
{
	CGedcomFile w;
	w.listFile();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomListTab(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomListTab()
{
	CGedcomFile w;
	w.listFileTab();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomChain(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomChain()
{
	CGedcomFile w;
	w.listChain();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomFilter(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomFilter()
{
	CGedcomFile w;
	w.filterIrregular();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomKnownUnknown(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomKnownUnknown()
{
	CGedcomFile w;
	w.checkTags();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateGedcomLevel0(CCmdUI* pCmdUI)
{
//	if (m_inputMode != GEDCOM) pCmdUI->Enable(FALSE);
}
void CDragApp::OnGedcomLevel0()
{
	CGedcomFile w;
	w.listZero();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSamenameSamemothername(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckSamenameSamemothername()
{
	CcheckSameNameAnd dlg;
	dlg. and = L"mother";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSamenameSamefathername(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckSamenameSamefathername()
{
	CcheckSameNameAnd dlg;
	dlg. and = L"father";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSamenameSamebirth(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckSamenameSamebirth()
{
	CcheckSameNameAnd dlg;
	dlg. and = L"birth";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSamenameSamedeath(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckSamenameSamedeath()
{
	CcheckSameNameAnd dlg;
	dlg. and = L"death";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckMorespouses(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckMorespouses()
{
	CCheckSameSpouses dlg;
	dlg.m_wedding = false;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCouplesDifferentwdates(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCouplesDifferentwdates()
{
	CCheckSameSpouses dlg;
	dlg.m_wedding = true;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckReferences(CCmdUI* pCmdUI)
{
}
void CDragApp::OnCheckReferences()
{
	CCheckIntegrity cp;
	cp.integrity();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckUnreferenced(CCmdUI* pCmdUI)
{
}
void CDragApp::OnCheckUnreferenced()
{
	CUnreferencedItems dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckUnknownNames(CCmdUI* pCmdUI)
{
}
void CDragApp::OnCheckUnknownNames()
{
	CCheckNames dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateDatesFamily(CCmdUI* pCmdUI)
{
}
void CDragApp::OnDatesFamily()
{
	CCheckFamilyDates dlg;
	dlg.m_all = true;  // minden családtagot írjon ki
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateDatesValid(CCmdUI* pCmdUI)
{
}
void CDragApp::OnDatesValid()
{
	CDateFormat dlg;
	dlg.m_massage = true;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckLifetime(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckLifetime()
{
	CLifeSpan dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSpousedates(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckSpousedates()
{
	CCheckSpousesDiff dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateMotherandchild(CCmdUI* pCmdUI)
{

}
void CDragApp::OnMotherandchild()
{
	CCheckParentChild dlg;

	dlg.m_function = MCG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateFatherandchild(CCmdUI* pCmdUI)
{

}
void CDragApp::OnFatherandchild()
{
	CCheckParentChild dlg;

	dlg.m_function = FCG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBornAfterfatherdeath(CCmdUI* pCmdUI)
{
}
void CDragApp::OnBornAfterfatherdeath()
{
	CCheckFatherDeath9 dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateBornAftermotherdeath(CCmdUI* pCmdUI)
{
}
void CDragApp::OnBornAftermotherdeath()
{
	CCheckMotherDeath dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckHusbandage(CCmdUI* pCmdUI)
{

}
void CDragApp::OnCheckHusbandage()
{
	CCheckSpouseAge dlg;

	dlg.m_function = HG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckWifeage(CCmdUI* pCmdUI)
{
}
void CDragApp::OnCheckWifeage()
{
	CCheckSpouseAge dlg;

	dlg.m_function = WG;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckGenerationcode(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnCheckGenerationcode()
{
	CCheckGenerations dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckMotherindex(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnCheckMotherindex()
{
	CCheckMotherIndex dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckSpousesex(CCmdUI* pCmdUI)
{
}
void CDragApp::OnCheckSpousesex()
{
	CCheckSpousesSex dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckFolyt(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnCheckFolyt()
{
	CCheckBefore w;
	w.checkFolyt();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCsaladTorzs(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnCsaladTorzs()
{
	CConnectCsalad conn;
	conn.connectCsalad(FALSE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnUpdateCheckRepeatedFamilies(CCmdUI* pCmdUI)
{
	if (m_inputMode != GAHTML) pCmdUI->Enable(FALSE);
}
void CDragApp::OnCheckRepeatedFamilies()
{
	CCheckBefore w;
	w.repeatedFamilies();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::OnHelp()
{
	if (_waccess(m_helpFileSpec, 0))
	{
		AfxMessageBox(L"Nincs a DragonHelp.chm fájl a program könyvtárában!");
		return;
	}
	else
	{
		ShellExecute(NULL, L"open", m_helpFileSpec, NULL, NULL, SW_SHOWNORMAL);
	}
}
