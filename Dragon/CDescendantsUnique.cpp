// descendantsUnique.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendantsLinearTable.h"
#include "CDescendantsUnique.h"
#include "Relatives.h"

// CDescendantsUnique dialog

IMPLEMENT_DYNAMIC(CDescendantsUnique, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsUnique::CDescendantsUnique(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTS_UNIQUE, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsUnique::~CDescendantsUnique()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrlU);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsUnique, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CDescendantsUnique::OnCustomdrawList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDescendantsUnique::OnDblclkList)
	ON_COMMAND(ID_ALL_ASCENDANTS, &CDescendantsUnique::OnAllAscendants)
	ON_COMMAND(ID_ALL_DESCENDANTS, &CDescendantsUnique::OnAllDescendants)
	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CDescendantsUnique::OnNotepad)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsUnique::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListCtrlU.m_ctlHeader.h11 = 1;
	m_ListCtrlU.m_ctlHeader.h12 = 10;

	m_ListCtrlU.SortByHeaderClick(FALSE);
	m_ListCtrlU.SetExtendedStyle(m_ListCtrlU.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlU.InsertColumn(L_CLRTEXTBK, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_CLRTEXT, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_NAME, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_ROWID, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_LINENUMBER, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_NUMOFD, L"desc", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(L_ISM, L"ism", LVCFMT_CENTER, 50, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(L_ID, L"id", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_IDC, L"idC", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_IDF, L"idP", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_MINDEX, L"A", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_DBC, L"dbC", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_GEN, L"gener", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(L_DESCENDANT, L"leszármazott", LVCFMT_LEFT, 1600, -1, COL_TEXT);


	return TRUE; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	CString gen;
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
		gen = m_ListCtrlU.GetItemText(nItem, L_GEN);

		if (_wtoi(m_ListCtrlU.GetItemText(nItem, L_CLRTEXTBK)))
			pLVCD->clrTextBk = WHITE;
		else
			pLVCD->clrTextBk = RGB(230, 230, 230);

		if (nCol == L_GEN)
		{
			if (_wtoi(m_ListCtrlU.GetItemText(nItem, L_CLRTEXT)))
				pLVCD->clrText = RGB(255, 0, 0);
			else
				pLVCD->clrText = RGB(0, 0, 255);
		}
		if( nCol == L_DESCENDANT && gen.IsEmpty())
			pLVCD->clrText = RGB(0, 0, 255);
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
LRESULT CDescendantsUnique::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CDescendantsUnique::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CRelatives dlgR;
	dlgR.m_rowid = m_ListCtrlU.GetItemText(nItem, L_ROWID);
	if (dlgR.DoModal() == IDCANCEL) return;
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnAllAscendants()
{
	int n = m_ListCtrlU.GetItemCount();
	int id;
	int idF;
	int idC;
	int dbC;
	CString gen;
	CString ascendant;
	int j;

	std::vector<int> vN;

	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString descendant = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
	CString file;
	file.Format(L"%s felmenõi", m_ListCtrlU.GetItemText(nItem, L_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Felmenõi lánc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s felmenõ\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");

	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, L_DESCENDANT);
		if (str == descendant)
		{
			vN.push_back(i);
		}
	}

	// vN-ben az ember mindenegyes elõfordulása, akiknke a felmenõit akarjuk listázni
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		id = nItem;
		idF = _wtoi(m_ListCtrlU.GetItemText(nItem, L_IDF));
		vid.clear();
		vid.push_back(nItem);
		for (j = nItem; j > -1; --j)
		{
			if (_wtoi(m_ListCtrlU.GetItemText(j, L_ID)) == idF)
			{
				idF = _wtoi(m_ListCtrlU.GetItemText(j, L_IDF));
				vid.push_back(j);
			}
		}

		// vid-ban a felmenõk
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = _wtoi(m_ListCtrlU.GetItemText(j, L_ID));
			idF = _wtoi(m_ListCtrlU.GetItemText(j, L_IDF));
			idC = _wtoi(m_ListCtrlU.GetItemText(j, L_IDC));
			dbC = _wtoi(m_ListCtrlU.GetItemText(j, L_DBC));
			gen = m_ListCtrlU.GetItemText(j, L_GEN);
			ascendant = m_ListCtrlU.GetItemText(j, L_DESCENDANT);
			fwprintf(flDesc, L"%5d. %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), ascendant.Trim());
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnAllDescendants()
{
	int id;
	int idF;
	int idC;
	int dbC;
	CString gen;
	CString ascendant;
	int j;

	std::vector<int> vN;
	std::vector<int>vC1;
	std::vector<int>vC2;

	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString descendant = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
	CString file;
	file.Format(L"%s leszármazottai", m_ListCtrlU.GetItemText(nItem, L_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log fájl

	fwprintf(flDesc, L"Leszármazotti lánc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s leszármazott\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");


	// vN-be a kijelölt leszármazott elõfordulásai
	for (int i = 0; i < m_ListCtrlU.GetItemCount(); ++i)
	{
		str = m_ListCtrlU.GetItemText(i, L_DESCENDANT);
		if (str == descendant)
		{
			vN.push_back(i);
		}
	}
	// vN-ben az ember mindenegyes elõfordulása, akiknek a leszármazottait akarjuk listázni
	// vC1-be gyõjtjük a vN.at(i)-t és leszármazottait
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		vid.clear();
		vC1.clear();
		vC1.push_back(nItem);
		vid.push_back(nItem);

		while (vC1.size())
		{
			// vC1-be tesszük a  gyerekeit
			// Az összes gyerek összes gyerekét vC2-be majd vC1-be tesszük
			vC2.clear();
			for (int j = 0; j < vC1.size(); ++j)
			{
				idC = _wtoi(m_ListCtrlU.GetItemText(vC1.at(j), L_IDC));
				dbC = _wtoi(m_ListCtrlU.GetItemText(vC1.at(j), L_DBC));
				for (int k = idC; k < idC + dbC; ++k)
				{
					vC2.push_back(k);
					vid.push_back(k);
				}
			}
			vC1 = vC2;
		}


		// vid-ben a leszármazottak
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = _wtoi(m_ListCtrlU.GetItemText(j, L_ID));
			idF = _wtoi(m_ListCtrlU.GetItemText(j, L_IDF));
			idC = _wtoi(m_ListCtrlU.GetItemText(j, L_IDC));
			dbC = _wtoi(m_ListCtrlU.GetItemText(j, L_DBC));
			gen = m_ListCtrlU.GetItemText(j, L_GEN);
			ascendant = m_ListCtrlU.GetItemText(j, L_DESCENDANT);
			fwprintf(flDesc, L"%5d. %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), ascendant.Trim());
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnNotepad()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlU.GetItemText(nItem, L_LINENUMBER);
	if (!lineNumber.IsEmpty())
	{
		if (theApp.m_inputMode == GAHTML)
			theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
		else if (theApp.m_inputMode == GEDCOM)
			theApp.editNotepad(theApp.m_gedPathName, lineNumber);
	}
}