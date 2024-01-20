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
	m_ListCtrlU.InsertColumn(U_CNT, L"#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlU.InsertColumn(U_ISM, L"ism.", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_SIBLINGS, L"testvér", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_SPOUSES, L"hzstárs", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDM, L"anya", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDF, L"apa", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_IDC, L"gyerek", LVCFMT_RIGHT, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_ID, L"azonosító", LVCFMT_CENTER, 50, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_ASCENDANT, L"felmenõ", LVCFMT_LEFT, 1600, -1, COL_TEXT);
	m_ListCtrlU.InsertColumn(U_NAME, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlU.InsertColumn(U_DESCRIPTION, L"", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
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