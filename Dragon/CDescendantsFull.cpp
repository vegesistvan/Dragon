// CDescendantsFull.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include "CDescendantsFull.h"


IMPLEMENT_DYNAMIC(CDescendantsFull, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsFull::CDescendantsFull(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTS_FULL, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsFull::~CDescendantsFull()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrlF);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsFull, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CDescendantsFull::OnCustomdrawList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_KERESS_BEGINNING, &CDescendantsFull::OnKeressBeginning)
	ON_COMMAND(ID_KERESS_NEXT, &CDescendantsFull::OnKeressNext)
	ON_COMMAND(ID_KERESS_PREVIOUS, &CDescendantsFull::OnKeressPrevious)
	ON_COMMAND(ID_FUNCTIONS_DESCENDANDS, &CDescendantsFull::OnFunctionsDescendands)
	ON_COMMAND(ID_FUNCTIONS_ASCENDANTS, &CDescendantsFull::OnFunctionsAscendants)

END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsFull::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	m_ListCtrlF.m_ctlHeader.h11 = 1;
//	m_ListCtrlF.m_ctlHeader.h12 = 7;

	m_ListCtrlF.SortByHeaderClick(FALSE);
	m_ListCtrlF.SetExtendedStyle(m_ListCtrlF.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlF.InsertColumn(L_CLRTEXTBK, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_CLRTEXT, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_NUMOFD, L"desc", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlF.InsertColumn(L_ISM, L"ism", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_ID, L"id", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_IDC, L"idC", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_IDF, L"idP", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_MINDEX, L"A", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_DBC, L"dbC", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_GEN, L"gener", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_DESCENDANT, L"leszármazott", LVCFMT_LEFT, 1600, -1, COL_TEXT);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	int nItem;
	int nCol;
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

		if (_wtoi(m_ListCtrlF.GetItemText(nItem, L_CLRTEXTBK)))
			pLVCD->clrTextBk = WHITE;
		else
			pLVCD->clrTextBk = RGB(230, 230, 230);

		if (nCol == L_GEN)
		{
			if (_wtoi(m_ListCtrlF.GetItemText(nItem, L_CLRTEXT)))
				pLVCD->clrText = RGB(255,0,0);
			else
				pLVCD->clrText = RGB(0,0,255);
		}
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
LRESULT CDescendantsFull::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CDescendantsFull::OnKeressBeginning()
{
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_ALL);
	CString ism = m_ListCtrlF.GetItemText(nItem, L_ISM);
	if (ism.IsEmpty())
	{
		AfxMessageBox(L"Csak egyszer fordul elõ a kijelölt leszármazott!");
	}
	bool getit = false;
	CString name = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlF.GetItemCount();
	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlF.GetItemText(i, L_DESCENDANT);
		if (str == name)
		{
			if (i != nItem)
			{
				m_ListCtrlF.EnsureVisible(i, FALSE);
				m_ListCtrlF.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				getit = true;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnKeressNext()
{
	bool gotit = false;
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_ABOVE);
	CString name = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlF.GetItemCount();
	for (int i = nItem + 1; i < n; ++i)
	{
		str = m_ListCtrlF.GetItemText(i, L_DESCENDANT);
		if (str == name)
		{
			m_ListCtrlF.EnsureVisible(i, FALSE);
			m_ListCtrlF.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			gotit = true;
			break;
		}
	}
	if( !gotit)
		AfxMessageBox(L"Csak egyszer fordul elõ a kijelölt leszármazott!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnKeressPrevious()
{
	bool gotit = false;
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_BELOW);
	if (nItem == 0) return;
	CString name = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	int n = m_ListCtrlF.GetItemCount();

	for (int i = nItem - 1; i >= 0; --i)
	{
		str = m_ListCtrlF.GetItemText(i, L_DESCENDANT );
		if (str == name)
		{
			m_ListCtrlF.EnsureVisible(i, FALSE);
			m_ListCtrlF.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			gotit = true;
			break;
		}
	}
	if (!gotit)
		AfxMessageBox(L"Csak egyszer fordul elõ a kijelölt leszármazott!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnFunctionsDescendands()
{
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	int dbC;
	int id;
	int idC;
	CString descendant;

	std::vector<int> vid1;
	std::vector<int> vid2;
	std::vector<int> vid; // leszármazottak
	idC = _wtoi( m_ListCtrlF.GetItemText(nItem, L_IDC) );
	if (idC == 0 )
	{
		descendant = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
		str = getTwoWords(descendant);
		str.Format(L"%s-nek nincs leszármazottja!", (CString)str);
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return;
	}

	id = nItem;
	vid1.push_back(id);  // ezeknek a leszármazottait fogja keresni
	vid.push_back(id);	// összes leszármazott	
	theApp.unselectAll(&m_ListCtrlF);
	m_ListCtrlF.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	while( true)
	{
		vid2.clear();		// a következõ generációt gyûjti benne
		for (int i = 0; i < vid1.size(); ++i)
		{
			id = vid1.at(i);	// apa azonosítója
			idC = _wtoi(m_ListCtrlF.GetItemText(id, L_IDC));  // elsõ gyereke azonosítója
			if (idC)
			{
				dbC = _wtoi(m_ListCtrlF.GetItemText(id, L_DBC));
				for (int j = idC; j < idC+dbC; ++j)  // gyerekeket keressük
				{
					m_ListCtrlF.SetItemState(j, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					vid2.push_back(j);  // a következõ generációt gyûjti
					vid.push_back(j);	// az összes leszármazottat gyûjti
				}
			}
		}
		if (!vid2.size()) break;
		vid1 = vid2;
	}

/*
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"desc", L"w+");  // log fájl
	for (int i = 0; i < vid.size(); ++i)
	{
		descendant = m_ListCtrlF.GetItemText(i, L_DESCENDANT);
		fwprintf(flDesc, L"%-8d %s\n", i, descendant);
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnFunctionsAscendants()
{
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	int n = m_ListCtrlF.GetItemCount();
	int id;
	int idF;

	id = nItem;
	idF = _wtoi(m_ListCtrlF.GetItemText(nItem, L_IDF) );

	theApp.unselectAll(&m_ListCtrlF);
	m_ListCtrlF.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	while (true)
	{
		m_ListCtrlF.SetItemState(idF, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		if (!idF) break;
		idF = _wtoi(m_ListCtrlF.GetItemText(idF, L_IDF));
	}

/*
	CString ascendant;
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"asc", L"w+");  // log fájl
	ascendant = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	fwprintf(flDesc, L"%-8s %s\n", id, ascendant.TrimLeft());
	fclose(flDesc);
	theApp.showFile(filePathName);
*/
}
