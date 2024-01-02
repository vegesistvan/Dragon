// contractEntries.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "unitedEntries.h"
#include "Relatives.h"
#include "compareAscendants.h"

IMPLEMENT_DYNAMIC(CUnitedEntries, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntries::CUnitedEntries(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITED_ENTRIES, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntries::~CUnitedEntries()
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	DDX_Control(pDX, IDC_STATIC_NEXT, colorNext);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_SearchCtrl);
	DDX_Control(pDX, IDC_RICHEDIT, m_RichEditCtrl);
	DDX_Control(pDX, IDC_COMBO, m_comboCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnitedEntries, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CUnitedEntries::OnSelchangeTab)
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC_NEXT, &CUnitedEntries::OnClickedStaticNext)
	ON_COMMAND(ID_FILTER_UNFILTERED, &CUnitedEntries::OnFilterUnfiltered)
	ON_COMMAND(ID_FILTER_ITERATION1, &CUnitedEntries::OnFilterIteration1)
	ON_COMMAND(ID_FILTER_ITERATION2, &CUnitedEntries::OnFilterIteration2)
	ON_COMMAND(ID_ATVETT, &CUnitedEntries::OnAtvett)
	ON_COMMAND(ID_INFO_UNION, &CUnitedEntries::OnInfoUnion)
	ON_COMMAND(ID_INFO_UNITED, &CUnitedEntries::OnInfoUnited)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntries::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (theApp.m_uniteVersion.IsEmpty())
	{
		AfxMessageBox(L"Az adatbázis nem egyesített!");
		return false;
	}
	
	m_comboCtrl.AddString( L"anya:" );
	m_comboCtrl.AddString( L"ember:" );
	m_comboCtrl.SetCurSel(1);

	m_RichEditCtrl.GetSelectionCharFormat(cf);
	m_RichEditCtrl.SetBackgroundColor(false, RGB(240, 240, 240));


	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	m_ceU.Create(IDD_UNITED_ENTRIES_UNITED, this);
	m_ceD.Create(IDD_UNITED_ENTRIES_DIFFERENT, this);
	m_ceE.Create(IDD_UNITED_ENTRIES_ERRORS, this);
	m_ceX.Create(IDD_UNITED_ENTRIES_X, this);

	int i = 0;

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT | TCIF_PARAM | TCIF_STATE;
	tcItem.pszText = L"Egyesített";
	tcItem.lParam = 0;
	tcItem.dwStateMask = TCIS_HIGHLIGHTED;
	tcItem.dwState = TCIS_HIGHLIGHTED;

	m_TabCtrl.InsertItem(i, &tcItem);
	++i;
	m_TabCtrl.InsertItem(i, L"Különbözõek");
	++i;
	m_TabCtrl.InsertItem(i, L"Hibák");
	++i;
	m_TabCtrl.InsertItem(i, L"Kérdéses");

	attachDialogs();

	colorNext.SetTextColor(LIGHTBLUE);

	SetWindowTextW( theApp.m_caption );
	m_title0 = L"Azonos nevû bejegyzések, amelyek között vannak egyesítettek";
	m_title1 = L"Azonos nevû bejegyzések, amelyek között nincsenek egyesítettek";
	m_title2 = L"Azonos nevû bejegyzések, amelyek között vannak azonos és ellentmondó adatpárok";
	m_title3 = L"Azonos nevû bejegyzések, amelyek között nincs ellentmondás, de nem is egyesítettek";
	
	m_RichEditCtrl.SetWindowTextW(m_title0);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

	ShowWindow(SW_MAXIMIZE);

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntries::attachDialogs()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetWindowRect(&tabRect);
	ScreenToClient(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX = tabRect.left + 1 + 5;
	nY = tabRect.top + itemRect.bottom + 1 + 5;
	nXc = tabRect.right - tabRect.left - 4 - 10;
	nYc = tabRect.bottom - nY - 2 - 5;

	m_ceU.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_ceD.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_ceE.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_ceX.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

	m_ListCtrl	= &(m_ceU.m_ListCtrlU);
	vPeople		= &(m_ceU.vPeople);
	vFiltered	= &(m_ceU.vFiltered);


	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMenu* menu;
	menu = GetMenu();
	DestroyMenu((HMENU)menu);

	CString title;
	int nItem = m_TabCtrl.GetCurSel();
	switch ( nItem)
	{
	case 0:
		m_ListCtrl = &(m_ceU.m_ListCtrlU);
		vPeople = &(m_ceU.vPeople);
		vFiltered = &(m_ceU.vFiltered);

		m_ceU.ShowWindow(SW_SHOW);
		m_ceD.ShowWindow(SW_HIDE);
		m_ceE.ShowWindow(SW_HIDE);
		m_ceX.ShowWindow(SW_HIDE);

		menu->LoadMenuW(IDR_UNITED_ENTRIES0);
		m_title = m_title0;
		break;
	case 1:
		m_ListCtrl = &(m_ceD.m_ListCtrlD);
		vPeople = &(m_ceD.vPeople);
		vFiltered = &(m_ceD.vFiltered);

		m_ceD.ShowWindow(SW_SHOW);
		m_ceU.ShowWindow(SW_HIDE);
		m_ceE.ShowWindow(SW_HIDE);
		m_ceX.ShowWindow(SW_HIDE);

		menu->LoadMenuW(IDR_UNITED_ENTRIES1);
		m_title = m_title1;
		break;
	case 2:
		m_ListCtrl = &(m_ceE.m_ListCtrlE);
		vPeople = &(m_ceE.vPeople);

		m_ceE.ShowWindow(SW_SHOW);
		m_ceU.ShowWindow(SW_HIDE);
		m_ceD.ShowWindow(SW_HIDE);
		m_ceX.ShowWindow(SW_HIDE);

		menu->LoadMenuW(IDR_UNITED_ENTRIES2);
		m_title = m_title2;
		break;
	case 3:
		m_ListCtrl = &(m_ceX.m_ListCtrlX);
		vPeople = &(m_ceX.vPeople);

		m_ceX.ShowWindow(SW_SHOW);
		m_ceE.ShowWindow(SW_HIDE);
		m_ceU.ShowWindow(SW_HIDE);
		m_ceD.ShowWindow(SW_HIDE);

		menu->LoadMenuW(IDR_UNITED_ENTRIES1);
		m_title = m_title3;
		break;
	}
	SetMenu(menu);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();

	m_RichEditCtrl.SetWindowTextW(m_title );
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	attachDialogs();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntries::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnClickedStaticNext()
{
	int nItem = m_ListCtrl->GetNextItem(-1, LVNI_SELECTED);
	keress(nItem + 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::keress(int start)
{
	CString	search;
	m_SearchCtrl.GetWindowTextW(search);
	if( m_comboCtrl.GetCurSel() == 0 )
		theApp.keress(search, m_ListCtrl, U_MOTHER, start, true);
	else
		theApp.keress(search, m_ListCtrl, U_NAME, start, true );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnFilterUnfiltered()
{
	CString title;
	int nItem = m_TabCtrl.GetCurSel();
	m_ListCtrl->DeleteAllItems();

	m_title0 = L"Azonos nevû bejegyzések, amelyek között vannak egyesítettek";
	m_title1 = L"Azonos nevû bejegyzések, amelyek között nincsenek összevonhatóak";

	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();
	switch (nItem)
	{
	case 0:
		
		m_RichEditCtrl.SetWindowTextW(m_title0);
		m_ListCtrl->SetItemCountEx( m_ceU.vPeople.size() + 1);
		m_ListCtrl->AttachDataset(&(m_ceU.vPeople));
		break;
	case 1:
		
		m_RichEditCtrl.SetWindowTextW(m_title1);
		m_ListCtrl->SetItemCountEx(m_ceD.vPeople.size() + 1);
		m_ListCtrl->AttachDataset(&(m_ceD.vPeople));
		break;
	}
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnFilterIteration1()
{
	filter(1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnFilterIteration2()
{
	filter(2);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::filter(int iter)
{
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();

	int nItem = m_TabCtrl.GetCurSel();
	switch (nItem)
	{
	case 0:
		m_title0.Format(L"Azonos nevû bejegyzések, amelyek között vannak egyesítettek - a %d. iterációban", iter );
		m_RichEditCtrl.SetWindowTextW(m_title0);
		break;
	case 1:
		m_title1.Format(L"Azonos nevû bejegyzések, amelyek között nincsenek összevonhatóak - a %d. iterációban", iter);
		m_RichEditCtrl.SetWindowTextW(m_title1);
		break;
	}
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

	bool gotit = false;
	int loop;
	vFiltered->clear();
	for (UINT i = 0; i < vPeople->size() - U_COLUMNSCOUNT + 1; i += U_COLUMNSCOUNT)
	{
		loop = _wtoi(vPeople->at(i + 1));
		if (loop > iter) break;

		if (loop == iter)
		{
			for (UINT j = 0; j < U_COLUMNSCOUNT; ++j) // ix-1 a cnt-re mutat
			{
				vFiltered->push_back(vPeople->at(i + j));
			}
			gotit = true;
		}
		if (loop == 0 && gotit)    // elválasztó üres sorok
		{
			for (UINT j = 0; j < U_COLUMNSCOUNT; ++j) // ix-1 a cnt-re mutat
			{
				vFiltered->push_back(vPeople->at(i + j));
			}
		}
	}
	m_ListCtrl->DeleteAllItems();
	m_ListCtrl->SetItemCountEx(vFiltered->size() + 1);
	m_ListCtrl->AttachDataset(vFiltered);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnAtvett()
{
	m_ceU.atvett();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::clearRichEdit()
{

	CRichEditCtrl* pEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT);
	pEdit->SetSel(0, -1);
	pEdit->Clear();
	pEdit->EmptyUndoBuffer();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
