// ascendantsFull.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"
#include "ascendantsFull.h"
#include "ascendantsEnum.h"
#include "ProgressWnd.h"
#include "Relatives.h"

IMPLEMENT_DYNAMIC(CAscendantsFull, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsFull::CAscendantsFull(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS_FULL, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsFull::~CAscendantsFull()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FULL, m_ListCtrlF);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendantsFull, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FULL, &CAscendantsFull::OnCustomdrawList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CAscendantsFull::OnFunctionsNotepad)
	ON_COMMAND(ID_ALL_ASCENDANTS, &CAscendantsFull::OnAllAscendants)
	ON_COMMAND(ID_ALL_DESCENDANTS, &CAscendantsFull::OnAllDescendants)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FULL, &CAscendantsFull::OnDblclkListFull)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsFull::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlF.m_ctlHeader.h11 = 1;
	m_ListCtrlF.m_ctlHeader.h12 = 7;

	m_ListCtrlF.SortByHeaderClick(FALSE);
	m_ListCtrlF.SetExtendedStyle(m_ListCtrlF.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlF.InsertColumn(F_TOGGLE, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_ROWID, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_LINENUMBER, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_SEX, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_CNT, L"#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlF.InsertColumn(F_ISM, L"ism.", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(F_IDF, L"apa", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(F_IDC, L"gyerek", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(F_ID, L"azonosító", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(F_ASCENDANT, L"felmenõ", LVCFMT_LEFT, 1560, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(F_NAME, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_DESCRIPTION, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(F_COLOR, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	return TRUE; 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::listTable()
{
	int nItem = 0;
	CString name;

	CProgressWnd wndP(NULL, L"Lineáris teljes lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
//	wndP.SetRange(0, vGKR->size());


//	for (int i = 0; i < vGKR->size(); ++i)
	{
//		getAscendant(i, FULL);

//		str.Format(L"%d", vGKR->at(i).toggle);		// hidden: generációk háttér színezésére
		m_ListCtrlF.InsertItem(nItem, str);
/*
		str.Format(L"%d", vGKR->at(i).sex_id);
		m_ListCtrlF.SetItemText(nItem, F_SEX, str);	// hidden

		str.Format(L"%d", i);
		m_ListCtrlF.SetItemText(nItem, F_CNT, str);

		m_ListCtrlF.SetItemText(nItem, F_ISM, col.rep);
		m_ListCtrlF.SetItemText(nItem, F_IDF, col.idF);
		m_ListCtrlF.SetItemText(nItem, F_IDC, col.idC);
		m_ListCtrlF.SetItemText(nItem, F_ID, col.idAL);

		name = col.name;
		name += col.description;
		m_ListCtrlF.SetItemText(nItem, F_ASCENDANT, name);

		m_ListCtrlF.SetItemText(nItem, F_NAME, col.name);
		m_ListCtrlF.SetItemText(nItem, F_DESCRIPTION, col.description);
		m_ListCtrlF.SetItemText(nItem, F_COLOR, L"");
		++nItem;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
*/
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	UINT rgbColor;
	UINT rgbFont;
	int toggle;
	int nCol;
	int color;

	*pResult = 0;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		nItem = pLVCD->nmcd.dwItemSpec;
		nCol = pLVCD->iSubItem;
		toggle = _wtoi(m_ListCtrlF.GetItemText(nItem, F_TOGGLE));
		if (toggle == false)
		{
			rgbColor = WHITE;			// változatlan marad
			rgbFont = RGB(255, 0, 0);
		}
		else
		{
			rgbColor = RGB( 230, 230, 230 );
			rgbFont = RGB( 0, 0, 255 );
		}
		if (nCol == F_ID)
		{
			pLVCD->clrText = rgbFont;
		}

		pLVCD->clrTextBk = rgbColor;

		color = _wtoi(m_ListCtrlF.GetItemText(nItem, F_COLOR ));
		if (color == 1)
			pLVCD->clrTextBk = RGB(0, 255, 255);

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CAscendantsFull::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_ASCENDANTS))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::OnDblclkListFull(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CRelatives dlgR;
	dlgR.m_rowid = m_ListCtrlF.GetItemText(nItem, F_ROWID);
	if (dlgR.DoModal() == IDCANCEL) return;
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::OnAllAscendants()
{
	int n = m_ListCtrlF.GetItemCount();
	CString id;
	CString idF;
	CString idC;
	int j;

	std::vector<int> vN;

	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	CString ascendant = m_ListCtrlF.GetItemText(nItem, F_ASCENDANT);
	CString file;
	file.Format(L"%s felmenõi", m_ListCtrlF.GetItemText(nItem, F_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Felmenõi lánc\n\n");
	fwprintf(flDesc, L"%6s %15s %15s %15s felmenõ\n", L"", L"idF", L"idC", L"id");
	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlF.GetItemText(i, F_ASCENDANT);
		if (str == ascendant)
		{
			vN.push_back(i);
		}
	}
	// vN-ben az ember mindenegyes elõfordulása, akiknke a felmenõit akarjuk listázni
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		idF = m_ListCtrlF.GetItemText(nItem, F_IDF);
		vid.clear();
		vid.push_back(nItem);

		for (i = nItem + 1; i < m_ListCtrlF.GetItemCount(); ++i)
		{
			id = m_ListCtrlF.GetItemText(i, F_ID);
			if (id == idF)
			{
				idF = m_ListCtrlF.GetItemText(i, F_IDF);
				vid.push_back(i);
			}
		}

		// vid-ban a felmenõk
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = m_ListCtrlF.GetItemText(j, F_ID);
			idF = m_ListCtrlF.GetItemText(j, F_IDF);
			idC = m_ListCtrlF.GetItemText(j, F_IDC);
			ascendant = m_ListCtrlF.GetItemText(j, F_ASCENDANT);
			fwprintf(flDesc, L"%5d. %15s %15s %15s %s\n", i + 1, idF, idC, id, ascendant);
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::OnAllDescendants()
{
	CString id;
	CString idF;
	CString idC;
	CString ascendant;
	int j;

	std::vector<int> vN;
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	CString file;
	file.Format(L"%s leszármazottai", m_ListCtrlF.GetItemText(nItem, F_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Leszármazotti lánc\n\n");
	fwprintf(flDesc, L"%6s %7s %7s %7s leszármazott\n", L"", L"idF", L"idC", L"id");
	// vN-be a kijelölt leszármazott elõfordulásai

	CString descendant = m_ListCtrlF.GetItemText(nItem, F_ASCENDANT);
	for (int i = 0; i < m_ListCtrlF.GetItemCount(); ++i)
	{
		str = m_ListCtrlF.GetItemText(i, F_ASCENDANT);
		if (str == descendant)
		{
			vN.push_back(i);
		}
	}
	// vN-ben az ember mindenegyes elõfordulása, akiknek a leszármazottait akarjuk listázni
	for (int i = 0; i < vN.size(); ++i)
	{
		vid.clear();
		nItem = vN.at(i);
		vid.push_back(nItem);
		idC = m_ListCtrlF.GetItemText(nItem, F_IDC);
		for (int i = nItem - 1; i > -1; --i)
		{
			id = m_ListCtrlF.GetItemText(i, F_ID);
			if (id == idC)
			{
				idC = m_ListCtrlF.GetItemText(i, F_IDC);
				vid.push_back(i);
			}
		}
		// vid-ben a leszármazottak
		for (int j = 0; j < vid.size(); ++j)
		{
			nItem = vid.at(j);
			id = m_ListCtrlF.GetItemText(nItem, F_ID);
			idF = m_ListCtrlF.GetItemText(nItem, F_IDF);
			idC = m_ListCtrlF.GetItemText(nItem, F_IDC);
			descendant = m_ListCtrlF.GetItemText(nItem, F_ASCENDANT);
			fwprintf(flDesc, L"%5d. %7s %7s %7s %s\n", nItem + 1, idF, idC, id, descendant);
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsFull::OnFunctionsNotepad()
{
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlF.GetItemText(nItem, F_LINENUMBER);
	if (!lineNumber.IsEmpty())
	{
		if (theApp.m_inputMode == GAHTML)
			theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
		else if (theApp.m_inputMode == GEDCOM)
			theApp.editNotepad(theApp.m_gedPathName, lineNumber);
	}
}

