// descendantsUnique.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendantsLinearTable.h"
#include "CDescendantsUnique.h"


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
	ON_COMMAND(ID_KERESS_BEGINNING, &CDescendantsUnique::OnKeressBeginning)
	ON_COMMAND(ID_KERESS_NEXT, &CDescendantsUnique::OnKeressNext)
	ON_COMMAND(ID_KERESS_PREVIOUS, &CDescendantsUnique::OnKeressPrevious)

	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CDescendantsUnique::OnFunctionsNotepad)

	ON_COMMAND(ID_DESCENDANTS_MARKED, &CDescendantsUnique::OnDescendantsMarked)
	ON_COMMAND(ID_DESCENDANTS_LISTED, &CDescendantsUnique::OnDescendantsListed)
	ON_COMMAND(ID_ASCENDANTS_MARKED, &CDescendantsUnique::OnAscendantsMarked)
	ON_COMMAND(ID_ASCENDANTS_LISTED, &CDescendantsUnique::OnAscendantsListed)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnKeressBeginning()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString ism = m_ListCtrlU.GetItemText(nItem, L_ISM);
	if (ism.IsEmpty())
	{
		AfxMessageBox(L"Többször nem fordul elõ a kijelölt leszármazott!");
	}
	CString name = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlU.GetItemCount();
	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, L_DESCENDANT);
		if (str == name)
		{
			if (i != nItem)
			{
				m_ListCtrlU.EnsureVisible(i, FALSE);
				m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnKeressNext()
{
	bool gotit = false;
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString name = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlU.GetItemCount();
	for (int i = nItem + 1; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, L_DESCENDANT);
		if (str == name)
		{
			m_ListCtrlU.EnsureVisible(i, FALSE);
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			gotit = true;
			break;
		}
	}
	if (!gotit)
		AfxMessageBox(L"Többször nem fordul elõ a kijelölt leszármazott!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnKeressPrevious()
{
	bool gotit = false;
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == 0) return;
	CString name = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlU.GetItemCount();

	for (int i = nItem - 1; i >= 0; --i)
	{
		str = m_ListCtrlU.GetItemText(i, L_DESCENDANT);
		if (str == name)
		{
			m_ListCtrlU.EnsureVisible(i, FALSE);
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			gotit = true;
			break;
		}
	}
	if (!gotit)
		AfxMessageBox(L"Többször nem fordul elõ a kijelölt leszármazott!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnDescendantsMarked()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	int dbC;
	int id;
	int idC;
	int idF;
	CString gen;
	int cnt;
	int j = nItem;
	CString descendant;

	std::vector<int> vid1;
	std::vector<int> vid2;
	idC = _wtoi(m_ListCtrlU.GetItemText(nItem, L_IDC));
	if (!idC)
	{
		descendant = m_ListCtrlU.GetItemText(nItem, L_DESCENDANT);
		str = getTwoWords(descendant);
		str.Format(L"%s-nek nincs leszármazottja!", (CString)str);
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return;
	}
	vid.clear();
	vid1.push_back(nItem);  // ezeknek a leszármazottait fogja keresni
	vid.push_back(nItem);	// összes leszármazott	
	theApp.unselectAll(&m_ListCtrlU);
	m_ListCtrlU.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	while (true)
	{
		vid2.clear();		// a következõ generációt gyûjti benne

		for (int i = 0; i < vid1.size(); ++i)
		{
			nItem = vid1.at(i);	// apa azonosítója
			idC = _wtoi(m_ListCtrlU.GetItemText(nItem, L_IDC));  // elsõ gyereke azonosítója
			if (idC)
			{
				cnt = 0;
				dbC = _wtoi(m_ListCtrlU.GetItemText(nItem, L_DBC));
				for (j = nItem + 1; j < m_ListCtrlU.GetItemCount(); ++j)
				{
					if (_wtoi(m_ListCtrlU.GetItemText(j, L_ID)) == idC)
					{
						m_ListCtrlU.SetItemState(j, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
						//						m_ListCtrlF.EnsureVisible(j, FALSE);
						vid2.push_back(j);  // a következõ generációt gyûjti
						vid.push_back(j);	// az összes leszármazottat gyûjti
						++cnt;
						++idC;
						if (cnt == dbC) break;
					}

				}
			}
		}
		if (!vid2.size()) break;
		vid1 = vid2;
	}
	m_ListCtrlU.SetItemState(j, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListCtrlU.EnsureVisible(j, FALSE);	// TODO: Add your command handler code here
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnDescendantsListed()
{
	OnDescendantsMarked();

	int dbC;
	int id;
	int idC;
	int idF;
	int j;
	CString gen;
	CString descendant;
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"descU", L"w+");  // log fájl

	fwprintf(flDesc, L"Leszármazotti lánc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s leszármazott\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");
	for (int i = 0; i < vid.size(); ++i)
	{
		j = vid.at(i);
		id = _wtoi(m_ListCtrlU.GetItemText(j, L_ID));
		idF = _wtoi(m_ListCtrlU.GetItemText(j, L_IDF));
		idC = _wtoi(m_ListCtrlU.GetItemText(j, L_IDC));
		dbC = _wtoi(m_ListCtrlU.GetItemText(j, L_DBC));
		gen = m_ListCtrlU.GetItemText(j, L_GEN);
		descendant = m_ListCtrlU.GetItemText(j, L_DESCENDANT);

		fwprintf(flDesc, L"%5d %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), descendant.Trim());
	}
	fclose(flDesc);
	theApp.showFile(filePathName);	// TODO: Add your command handler code here
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnAscendantsMarked()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	int n = m_ListCtrlU.GetItemCount();
	int id;
	int idF;
	int i = nItem;

	id = nItem;
	idF = _wtoi(m_ListCtrlU.GetItemText(nItem, L_IDF));

	vid.clear();
	vid.push_back(nItem);
	theApp.unselectAll(&m_ListCtrlU);
	m_ListCtrlU.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	for (i = nItem; i > -1; --i)
	{
		if (_wtoi(m_ListCtrlU.GetItemText(i, L_ID)) == idF)
		{
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			idF = _wtoi(m_ListCtrlU.GetItemText(i, L_IDF));
			vid.push_back(i);
		}
	}
	m_ListCtrlU.EnsureVisible(i, FALSE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnAscendantsListed()
{
	OnAscendantsMarked();

	int dbC;
	int id;
	int idC;
	int idF;
	int j;
	CString gen;
	CString ascendant;
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"asc", L"w+");  // log fájl


	fwprintf(flDesc, L"Felmenõi lánc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s felmenõ\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");

	for (int i = 0; i < vid.size(); ++i)
	{
		j = vid.at(i);
		id  = _wtoi(m_ListCtrlU.GetItemText(j, L_ID));
		idF = _wtoi(m_ListCtrlU.GetItemText(j, L_IDF));
		idC = _wtoi(m_ListCtrlU.GetItemText(j, L_IDC));
		dbC = _wtoi(m_ListCtrlU.GetItemText(j, L_DBC));
		gen = m_ListCtrlU.GetItemText(j, L_GEN);
		ascendant = m_ListCtrlU.GetItemText(j, L_DESCENDANT);
		if (dbC)
			fwprintf(flDesc, L"%5d. %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), ascendant.Trim());
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsUnique::OnFunctionsNotepad()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlU.GetItemText(nItem, L_LINENUMBER);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
