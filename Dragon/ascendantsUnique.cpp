// ascendantsUnique.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendantsUnique.h"
#include "ascendantsEnum.h"

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
	ON_COMMAND(ID_KERESS_BEGINNING, &CAscendantsUnique::OnKeressBeginning)
	ON_COMMAND(ID_KERESS_NEXT, &CAscendantsUnique::OnKeressNext)
	ON_COMMAND(ID_KERESS_PREVIOUS, &CAscendantsUnique::OnKeressPrevious)

	ON_COMMAND(ID_DESCENDANTS_MARKED, &CAscendantsUnique::OnDescendantsMarked)
	ON_COMMAND(ID_DESCENDANTS_LISTED, &CAscendantsUnique::OnDescendantsListed)
	ON_COMMAND(ID_ASCENDANTS_MARKED, &CAscendantsUnique::OnAscendantsMarked)
	ON_COMMAND(ID_ASCENDANTS_LISTED, &CAscendantsUnique::OnAscendantsListed)
	ON_COMMAND(ID_FUNCTIONS_NOTEPAD, &CAscendantsUnique::OnFunctionsNotepad)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnKeressBeginning()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_ALL);
	CString ism = m_ListCtrlU.GetItemText(nItem, U_ISM);
	if (ism.IsEmpty())
	{
		AfxMessageBox(L"Csak egyszer fordul elõ a kijelölt felmenõ!");
	}
	CString name = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
	int n = m_ListCtrlU.GetItemCount();
	for (int i = 0; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, U_ASCENDANT);
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
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnKeressNext()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_ABOVE);
	CString name = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
	int n = m_ListCtrlU.GetItemCount();
	for (int i = nItem + 1; i < n; ++i)
	{
		str = m_ListCtrlU.GetItemText(i, U_ASCENDANT);
		if (str == name)
		{
			m_ListCtrlU.EnsureVisible(i, FALSE);
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnKeressPrevious()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_BELOW);
	if (nItem == 0) return;
	CString name = m_ListCtrlU.GetItemText(nItem, U_ASCENDANT);
	int n = m_ListCtrlU.GetItemCount();

	for (int i = nItem - 1; i >= 0; --i)
	{
		str = m_ListCtrlU.GetItemText(i, U_ASCENDANT);
		if (str == name)
		{
			m_ListCtrlU.EnsureVisible(i, FALSE);
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnDescendantsMarked()
{

	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	int n = m_ListCtrlU.GetItemCount();
	int i;
	CString id;
	CString idC;

	id = m_ListCtrlU.GetItemText(nItem, U_ID);
	idC = m_ListCtrlU.GetItemText(nItem, U_IDC);
	vid.clear();
	vid.push_back(nItem);

	theApp.unselectAll(&m_ListCtrlU);
	m_ListCtrlU.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	for (i = nItem - 1; i > -1; --i)
	{
		id = m_ListCtrlU.GetItemText(i, U_ID);
		if (id == idC)
		{
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			idC = m_ListCtrlU.GetItemText(i, U_IDC);
			vid.push_back(i);
		}
	}
	m_ListCtrlU.EnsureVisible(i, FALSE);
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnDescendantsListed()
{
	OnDescendantsMarked();

	CString id;
	CString idC;
	CString idF;
	int j;
	CString descendant;
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"desc", L"w+");  // log fájl

	fwprintf(flDesc, L"Leszármazotti lánc\n\n");
	fwprintf(flDesc, L"%6s %5s %5s %5s leszármazott\n", L"", L"idF", L"idC", L"id");
	for (int i = 0; i < vid.size(); ++i)
	{
		j = vid.at(i);
		id = m_ListCtrlU.GetItemText(j, U_ID);
		idF = m_ListCtrlU.GetItemText(j, U_IDF);
		idC = m_ListCtrlU.GetItemText(j, U_IDC);
		descendant = m_ListCtrlU.GetItemText(j, U_ASCENDANT);
		fwprintf(flDesc, L"%5d. %5s %5s %5s %s\n", i + 1, idF, idC, id, descendant);
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnAscendantsMarked()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	int n = m_ListCtrlU.GetItemCount();

	int i;
	CString id;
	CString idF;

	id = m_ListCtrlU.GetItemText(nItem, U_ID);
	idF = m_ListCtrlU.GetItemText(nItem, U_IDF);
	vid.clear();
	vid.push_back(nItem);

	theApp.unselectAll(&m_ListCtrlU);
	m_ListCtrlU.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	for (i = nItem + 1; i < m_ListCtrlU.GetItemCount(); ++i)
	{
		id = m_ListCtrlU.GetItemText(i, U_ID);
		if (id == idF)
		{
			m_ListCtrlU.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			idF = m_ListCtrlU.GetItemText(i, U_IDF);
			vid.push_back(i);
		}
	}
	m_ListCtrlU.EnsureVisible(i, FALSE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnAscendantsListed()
{
	OnAscendantsMarked();

	CString id;
	CString idC;
	CString idF;
	int j;
	CString ascendants;
	CString filePathName;
	filePathName = theApp.openTextFile(&flDesc, L"desc", L"w+");  // log fájl

	fwprintf(flDesc, L"Felmenõi lánc\n\n");
	fwprintf(flDesc, L"%6s %15s %15s %15s felmenõ\n", L"", L"idF", L"idC", L"id");
	for (int i = 0; i < vid.size(); ++i)
	{
		j = vid.at(i);
		idF = m_ListCtrlU.GetItemText(j, U_IDF);
		idC = m_ListCtrlU.GetItemText(j, U_IDC);
		id = m_ListCtrlU.GetItemText(j, U_ID);
		ascendants = m_ListCtrlU.GetItemText(j, U_ASCENDANT );
		fwprintf(flDesc, L"%5d. %15s %15s %15s %s\n", i + 1, idF, idC, id, ascendants);
	}
	fclose(flDesc);
	theApp.showFile(filePathName);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsUnique::OnFunctionsNotepad()
{
	int nItem = m_ListCtrlU.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlU.GetItemText(nItem, U_LINENUMBER);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
