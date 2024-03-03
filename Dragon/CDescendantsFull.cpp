// CDescendantsFull.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include "CDescendantsFull.h"
#include "Relatives.h"

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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDescendantsFull::OnDblclkList)
	ON_COMMAND(ID_ALL_ASCENDANTS, &CDescendantsFull::OnAllAscendants)
	ON_COMMAND(ID_ALL_DESCENDANTS, &CDescendantsFull::OnAllDescendants)
	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CDescendantsFull::OnNotepad)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsFull::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlF.SortByHeaderClick(FALSE);
	m_ListCtrlF.SetExtendedStyle(m_ListCtrlF.GetExtendedStyle() | LVS_EX_GRIDLINES );

	m_ListCtrlF.InsertColumn(L_CLRTEXTBK, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_CLRTEXT, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_NAME, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_ROWID, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_LINENUMBER, L"", LVCFMT_RIGHT, 10, -1, COL_HIDDEN);
	m_ListCtrlF.InsertColumn(L_NUMOFD, L"desc", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlF.InsertColumn(L_ISM, L"ism", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_ID, L"id", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_IDC, L"idC", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_IDF, L"idP", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_MINDEX, L"A", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_DBC, L"dbC", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_GEN, L"gener", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlF.InsertColumn(L_DESCENDANT, L"lesz�rmazott", LVCFMT_LEFT, 2000, -1, COL_TEXT);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
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
		gen = m_ListCtrlF.GetItemText(nItem, L_GEN);
		if (nCol == L_DESCENDANT && gen.IsEmpty())
			pLVCD->clrText = RGB(0, 0, 255);

		if (_wtoi(m_ListCtrlF.GetItemText(nItem, L_CLRTEXTBK)))
			pLVCD->clrTextBk = WHITE;
		else
			pLVCD->clrTextBk = RGB(230, 230, 230);

		if (nCol == L_GEN)
		{
			if (_wtoi(m_ListCtrlF.GetItemText(nItem, L_CLRTEXT)))
				pLVCD->clrText = RGB(255, 0, 0);
			else
				pLVCD->clrText = RGB(0, 0, 255);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I � K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnDblclkList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CRelatives dlgR;
	dlgR.m_rowid = m_ListCtrlF.GetItemText( nItem, L_ROWID );
	if (dlgR.DoModal() == IDCANCEL) return;
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnAllAscendants()
{
	int n = m_ListCtrlF.GetItemCount();
	int id;
	int idF;
	int idC;
	int dbC;
	CString gen;
	CString ascendant;
	int j;

	std::vector<int> vN;
	
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == 0) return;
	CString descendant = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	CString file;
	file.Format(L"%s felmen�i", m_ListCtrlF.GetItemText( nItem, L_NAME ) );
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log f�jl

	fwprintf(flDesc, L"Felmen�i l�nc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s felmen�\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");

	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlF.GetItemText(i, L_DESCENDANT);
		if (str == descendant)
		{
			vN.push_back(i);
		}
	}

	// vN-ben az ember mindenegyes el�fordul�sa, akiknke a felmen�it akarjuk list�zni
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		id = nItem;
		idF = _wtoi(m_ListCtrlF.GetItemText( nItem, L_IDF));
		vid.clear();
		vid.push_back(nItem);
		for (j = nItem; j > -1; --j)
		{
			if (_wtoi(m_ListCtrlF.GetItemText(j, L_ID)) == idF)
			{
				idF = _wtoi(m_ListCtrlF.GetItemText(j, L_IDF));
				vid.push_back(j);
			}
		}

		// vid-ban a felmen�k
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = _wtoi(m_ListCtrlF.GetItemText(j, L_ID));
			idF = _wtoi(m_ListCtrlF.GetItemText(j, L_IDF));
			idC = _wtoi(m_ListCtrlF.GetItemText(j, L_IDC));
			dbC = _wtoi(m_ListCtrlF.GetItemText(j, L_DBC));
			gen = m_ListCtrlF.GetItemText(j, L_GEN);
			ascendant = m_ListCtrlF.GetItemText(j, L_DESCENDANT);
			fwprintf(flDesc, L"%5d. %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), ascendant.Trim());
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnAllDescendants()
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

	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	if (nItem == 0) return;
	CString descendant = m_ListCtrlF.GetItemText(nItem, L_DESCENDANT);
	CString file;
	file.Format(L"%s lesz�rmazottai", m_ListCtrlF.GetItemText(nItem, L_NAME));
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, file, L"w+");  // log f�jl

	fwprintf(flDesc, L"Lesz�rmazotti l�nc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s %5s %4s lesz�rmazott\n", L"", L"id", L"idF", L"idC", L"dbC", L"gen");

	
	// vN-be a kijel�lt lesz�rmazott el�fordul�sai
	for (int i = 0; i < m_ListCtrlF.GetItemCount(); ++i)
	{
		str = m_ListCtrlF.GetItemText(i, L_DESCENDANT);
		if (str == descendant)
		{
			vN.push_back(i);
		}
	}
	// vN-ben az ember mindenegyes el�fordul�sa, akiknek a lesz�rmazottait akarjuk list�zni
	// vC1-be gy�jtj�k a vN.at(i)-t �s lesz�rmazottait
	for (int i = 0; i < vN.size(); ++i)
	{
		nItem = vN.at(i);
		vid.clear();
		vC1.clear();
		vC1.push_back(nItem);
		vid.push_back(nItem);

		while( vC1.size())
		{
			// vC1-be tessz�k a  gyerekeit
			// Az �sszes gyerek �sszes gyerek�t vC2-be majd vC1-be tessz�k
			vC2.clear();
			for (int j = 0; j < vC1.size(); ++j)
			{
				idC = _wtoi(m_ListCtrlF.GetItemText(vC1.at(j), L_IDC));
				dbC = _wtoi(m_ListCtrlF.GetItemText(vC1.at(j), L_DBC));
				for (int k = idC; k < idC + dbC; ++k)
				{
					vC2.push_back(k);
					vid.push_back(k);
				}
			}
			vC1 = vC2;
		}

	
		// vid-ben a lesz�rmazottak
		for (int i = 0; i < vid.size(); ++i)
		{
			j = vid.at(i);
			id = _wtoi(m_ListCtrlF.GetItemText(j, L_ID));
			idF = _wtoi(m_ListCtrlF.GetItemText(j, L_IDF));
			idC = _wtoi(m_ListCtrlF.GetItemText(j, L_IDC));
			dbC = _wtoi(m_ListCtrlF.GetItemText(j, L_DBC));
			gen = m_ListCtrlF.GetItemText(j, L_GEN);
			ascendant = m_ListCtrlF.GetItemText(j, L_DESCENDANT);
			fwprintf(flDesc, L"%5d. %5d %5d %5d %5d %4s %s\n", i + 1, id, idF, idC, dbC, gen.Trim(), ascendant.Trim());
		}
		fwprintf(flDesc, L"\n");
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsFull::OnNotepad()
{
	int nItem = m_ListCtrlF.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlF.GetItemText(nItem, L_LINENUMBER);
	if (!lineNumber.IsEmpty())
	{
		if (theApp.m_inputMode == GAHTML)
			theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
		else if (theApp.m_inputMode == GEDCOM)
			theApp.editNotepad(theApp.m_gedPathName, lineNumber);
	}
}