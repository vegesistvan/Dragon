// ascendantsUnique.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendantsUnique.h"
#include "ascendantsEnum.h"
#include "Relatives.h"

IMPLEMENT_DYNAMIC(CAscendantsUnique, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsUnique::CAscendantsUnique(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS_UNIQUE, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsUnique::~CAscendantsUnique()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNIQUE, m_ListCtrlU);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendantsUnique, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_UNIQUE, &CAscendantsUnique::OnCustomdrawList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CAscendantsUnique::OnFunctionsNotepad)
	ON_COMMAND(ID_ALL_ASCENDANTS, &CAscendantsUnique::OnAllAscendants)
	ON_COMMAND(ID_ALL_DESCENDANTS, &CAscendantsUnique::OnAllDescendants)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UNIQUE, &CAscendantsUnique::OnDblclkListUnique)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsUnique::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlU.m_ctlHeader.h11 = 1;
	m_ListCtrlU.m_ctlHeader.h12 = 10;

	m_ListCtrlU.SortByHeaderClick(FALSE);
	m_ListCtrlU.SetExtendedStyle(m_ListCtrlU.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlU.InsertColumn(U_TOGGLE, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_MOREC, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_ROWID, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_LINENUMBER, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_CNT, L"#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_ISM, L"ism.", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_SIBLINGS, L"testvér", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_SPOUSES, L"hzstárs", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDM, L"anya", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDF, L"apa", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDC, L"gyerek", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_ID, L"azonosító", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_ASCENDANT, L"felmenõ", LVCFMT_LEFT, 1600, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_NAME, L"", LVCFMT_LEFT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_DESCRIPTION, L"", LVCFMT_LEFT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_COLOR, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	UINT rgbColor;
	UINT rgbFont;
	int toggle;
	int nCol;
	int moreC;
	int family;

	*pResult = 0;

	nItem = pLVCD->nmcd.dwItemSpec;
	nCol = pLVCD->iSubItem;
	toggle = _wtoi(m_ListCtrlU.GetItemText(nItem, U_TOGGLE));
	moreC = _wtoi(m_ListCtrlU.GetItemText(nItem, U_MOREC) );
	family = _wtoi(m_ListCtrlU.GetItemText(nItem, U_COLOR));
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		
		if (toggle == 0)
		{
			rgbColor = WHITE;			// változatlan marad
			rgbFont = RED;
		}
		else
		{
			rgbColor = RGB(230, 230, 230);
			rgbFont = BLUE;
		}
		pLVCD->clrTextBk = rgbColor;

		if (nCol == U_ID)
			pLVCD->clrText = rgbFont;
		else if (nCol == U_IDC && moreC)
		{
			pLVCD->clrTextBk = RGB( 36, 239, 231 ); 
		}
		
		if( nCol == U_ASCENDANT && family == 4 )
			pLVCD->clrText = BLUE;

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
LRESULT CAscendantsUnique::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CAscendantsUnique::OnDblclkListUnique(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CRelatives dlgR;
	dlgR.m_rowid = m_ListCtrlU.GetItemText(nItem, U_ROWID);
	if (dlgR.DoModal() == IDCANCEL) return;
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnAllAscendants()
{
	int n = m_ListCtrlU.GetItemCount();
	CString id;
	CString idF;
	CString idC;
	int j;

	std::vector<int> vN;

	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString ascendant = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
	CString file;
	file.Format(L"%s felmenõi", m_ListCtrlU.GetItemText(nItem, U_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Felmenõi lánc\n\n");
	fwprintf(flDesc, L"%6s %15s %15s %15s felmenõ\n", L"", L"idF", L"idC", L"id");
	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, U_ASCENDANT);
		if (str == ascendant)
		{
			vN.push_back(i);
		}
	}
	// vN-ben az ember mindenegyes elõfordulása, akiknke a felmenõit akarjuk listázni
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		idF = m_ListCtrlU.GetItemText(nItem, U_IDF);
		vid.clear();
		vid.push_back(nItem);

		for (i = nItem + 1; i < m_ListCtrlU.GetItemCount(); ++i)
		{
			id = m_ListCtrlU.GetItemText(i, U_ID);
			if (id == idF)
			{
				idF = m_ListCtrlU.GetItemText(i, U_IDF);
				vid.push_back(i);
			}
		}

		// vid-ban a felmenõk
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = m_ListCtrlU.GetItemText(j, U_ID);
			idF = m_ListCtrlU.GetItemText(j, U_IDF);
			idC = m_ListCtrlU.GetItemText(j, U_IDC);
			ascendant = m_ListCtrlU.GetItemText(j, U_ASCENDANT);
			fwprintf(flDesc, L"%5d. %15s %15s %15s %s\n", i + 1, idF, idC, id, ascendant);
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnAllDescendants()
{
	CString id;
	CString idF;
	CString idC;
	CString ascendant;
	int j;

	std::vector<int> vN;
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString file;
	file.Format(L"%s leszármazottai", m_ListCtrlU.GetItemText(nItem, U_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Leszármazotti lánc\n\n");
	fwprintf(flDesc, L"%6s %7s %7s %7s leszármazott\n", L"", L"idF", L"idC", L"id");
	// vN-be a kijelölt leszármazott elõfordulásai

	CString descendant = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
	for (int i = 0; i < m_ListCtrlU.GetItemCount(); ++i)
	{
		str = m_ListCtrlU.GetItemText(i, U_ASCENDANT);
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
		idC = m_ListCtrlU.GetItemText(nItem, U_IDC);
		for (int i = nItem - 1; i > -1; --i)
		{
			id = m_ListCtrlU.GetItemText(i, U_ID);
			if (id == idC)
			{
				idC = m_ListCtrlU.GetItemText(i, U_IDC);
				vid.push_back(i);
			}
		}
		// vid-ben a leszármazottak
		for (int j = 0; j < vid.size(); ++j)
		{
			nItem = vid.at(j);
			id = m_ListCtrlU.GetItemText(nItem, U_ID);
			idF = m_ListCtrlU.GetItemText(nItem, U_IDF);
			idC = m_ListCtrlU.GetItemText(nItem, U_IDC);
			descendant = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
			fwprintf(flDesc, L"%5d. %7s %7s %7s %s\n", nItem + 1, idF, idC, id, descendant);
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnFunctionsNotepad()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlU.GetItemText(nItem, U_LINENUMBER);
	if (!lineNumber.IsEmpty())
	{
		if (theApp.m_inputMode == GAHTML)
			theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
		else if (theApp.m_inputMode == GEDCOM)
			theApp.editNotepad(theApp.m_gedPathName, lineNumber);
	}
}

