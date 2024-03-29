// descendantNew.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsLinearTable.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include <algorithm>
#include "build_defs.h"

IMPLEMENT_DYNAMIC(CDescendantsLinearTable, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsLinearTable::CDescendantsLinearTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTS_LINEARTABLE, pParent)
{
	m_recP = new CSqliteDBRecordSet;
	m_recM = new CSqliteDBRecordSet;
	m_log = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsLinearTable::~CDescendantsLinearTable()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_SearchCtrl);
	DDX_Control(pDX, IDC_RICHEDIT, m_RichEditCtrl);
	DDX_Control(pDX, IDC_RICHEDIT1, m_RichEditCtrl1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsLinearTable, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDescendantsLinearTable::OnSelchangeTab)
	ON_STN_CLICKED(IDC_STATIC_NEXT, &CDescendantsLinearTable::OnClickedStaticNext)

	ON_COMMAND(ID_HTMLLISTA_FIX, &CDescendantsLinearTable::OnHtmlListaFix)
	ON_COMMAND(ID_HTMLLISTA_PRINTABLE, &CDescendantsLinearTable::OnHtmlListaPrintable)
	ON_COMMAND(ID_HTMLLIST_FIX_NOREP, &CDescendantsLinearTable::OnHtmllistFixNorep)
	ON_COMMAND(ID_HTMLLIST_PR_NOREP, &CDescendantsLinearTable::OnHtmllistPrNorep)

	ON_MESSAGE(WM_SET_ROW_COLOR, OnSetRowColor)
	ON_MESSAGE(WM_RELOAD_D, OnReloadListCtrl)
	ON_COMMAND(ID_INDENTED_LIST, &CDescendantsLinearTable::OnIndentedList)

	
	ON_COMMAND(ID_DESCENDANT_LENGTH, &CDescendantsLinearTable::OnDescendantLength)
	ON_COMMAND(ID_TABLE, &CDescendantsLinearTable::OnGraph)
	ON_COMMAND(ID_ABCD, &CDescendantsLinearTable::OnAbcd)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsLinearTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_brRed.CreateSolidBrush(RED);
	m_brYellow.CreateSolidBrush(YELLOW);
	COLORREF clrTextBk = RGB(230, 230, 230);
	COLORREF clrText = RGB(255, 0, 0);
	bool tippingBk = false;
	bool tippingTx = false;

	int gPrev = 0;
	int childorderPrev = 0;

	CProgressWnd wndP(NULL, L"Gener�ci� v�lt�sok jel�l�se...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vDesc->size());
	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (vDesc->at(i).id == -1) goto cont;
		// gener�ci� v�lt�sok jel�l�se a clrTextBk-ban
		if (vDesc->at(i).g != gPrev)
			tippingBk = !tippingBk;
		gPrev = vDesc->at(i).g;
		vDesc->at(i).clrTextBk = tippingBk;

		// az azonos sz�l�kh�z tartoz� gyerekek jel�l�se a clrText-ben
		if (vDesc->at(i).childorder != childorderPrev + 1)
			tippingTx = !tippingTx;
		childorderPrev = vDesc->at(i).childorder;
		vDesc->at(i).clrText = tippingTx;

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
/*
	if (m_log)
	{
		fs_vGKR2 = theApp.openTextFile(&flvGKR2, L"vGKR2", L"w+");  // log f�jl
		fs_vSp = theApp.openTextFile(&flvSp, L"vSp", L"w+");  // log f�jl
		fs_vFC = theApp.openTextFile(&flvFC, L"vCF", L"w+");  // log f�jl
		fs_vSib = theApp.openTextFile(&flvSib, L"vSib", L"w+");  // log f�jl		
	}

	create_vGKR2();		// vGKR kib�v�t�se ixF �s ixF -el �s togle-al vSp l�trahoz�sa
*/

	m_RichEditCtrl.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	m_RichEditCtrl1.GetSelectionCharFormat(cf1);
	cf1.cbSize = sizeof(cf1);
	cf1.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf1.dwEffects = CFE_BOLD;
	cf1.crTextColor = RED;
	cf1.yHeight = 200;


	m_titleF.Format(L"%s lesz�rmazotti list�ja", m_name);
	m_titleU.Format(L"%s lesz�rmazotti list�ja ism�tl�sek kihagy�s�val", m_name);


	m_aF.p_descendantName = p_descendantName;
	m_aF.Create(IDD_DESCENDANTS_FULL, this);
	m_aU.Create(IDD_DESCENDANTS_UNIQUE, this);

	int i = 0;

	TCITEM tcItem;
	tcItem = { 0 };
	tcItem.mask = TCIF_TEXT | TCIF_PARAM | TCIF_STATE;
	tcItem.pszText = L"Ism�tl�sek n�lk�l";
	tcItem.lParam = 0;
	tcItem.dwStateMask = TCIS_HIGHLIGHTED;
	tcItem.dwState = TCIS_HIGHLIGHTED;

	m_TabCtrl.InsertItem(i, &tcItem);
	++i;
	m_TabCtrl.InsertItem(i, L"Ism�tl�sekkel");

	attachDialogs();

	SetWindowTextW(theApp.m_caption);
	ShowWindow(SW_MAXIMIZE);

 	m_RichEditCtrl.SetWindowTextW(m_titleF);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);
	m_RichEditCtrl.SetBackgroundColor(false, RGB(240, 240, 240));

	CString title1;
	CString title2;
	CString title;
	switch (p_childrenOrder)
	{
	case DE::ORDER_INPUT:
		title2 = L"Gyerekek beviteli sorrendben\n";
		break;
	case DE::ORDER_BIRTH:
		title2 = L"Gyerekek sz�let�si/beviteli sorrendben\n";
		break;
	case DE::ORDER_NUMOFDESC:
		title2 = L"Gyerekek lesz�rmazainak sz�ma sorrendj�ben\n";
		break;
	}
	if (p_womenDescendants)
	{
		title1 = L"N�k lesz�rmazottai is a list�ban";
	}
	else
	{
		title1 = L"N�k lesz�rmazottai nincsenek a list�ban";
	}
	title.Format(L"%s\n%s", title1, title2);
	m_RichEditCtrl1.SetWindowTextW(title);
	m_RichEditCtrl1.SetSel(0, -1);
	m_RichEditCtrl1.SetSelectionCharFormat(cf1);
	m_RichEditCtrl1.SetBackgroundColor(false, RGB(240, 240, 240));


	CMenu* menu;
	menu = GetMenu();
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);


//	if( p_womenDescendants )
//		menu->EnableMenuItem(5, MF_BYPOSITION | MF_GRAYED);


	fullTable();
	uniqueTable();

	m_TabCtrl.SetCurSel(REPEATED);
	m_ListCtrl = &(m_aF.m_ListCtrlF);
	m_aF.ShowWindow(SW_SHOW);
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsLinearTable::attachDialogs()
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

	m_aF.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_aU.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	//	CMenu* menu;
	//	menu = GetMenu();
	//	DestroyMenu((HMENU)menu);
	/*
		TCITEM curItem = { 0 };
		m_TabCtrl.DeselectAll(FALSE); //reset all tab items
		curItem.mask = TCIF_STATE;
	*/
	CString title;
	int nItem = m_TabCtrl.GetCurSel();
	switch (nItem)
	{
	case REPEATED:
		m_aF.ShowWindow(SW_SHOW);
		m_aU.ShowWindow(SW_HIDE);
		m_ListCtrl = &(m_aF.m_ListCtrlF);
		m_title = m_titleF;
		break;
	case NOTREPEATED:
		m_aU.ShowWindow(SW_SHOW);
		m_aF.ShowWindow(SW_HIDE);
		m_ListCtrl = &(m_aU.m_ListCtrlU);
		m_title = m_titleU;
		break;
	}

	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();

	m_RichEditCtrl.SetWindowTextW(m_title);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	attachDialogs();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsLinearTable::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			keress(0);
			return true;			// mert az als� return-re debug m�dban hib�t jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnClickedStaticNext()
{
	int nItem = m_ListCtrl->GetNextItem(-1, LVNI_SELECTED);
	keress(nItem + 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::keress(int start)
{
	CString	search;
	m_SearchCtrl.GetWindowTextW(search);

	theApp.keress(search, m_ListCtrl, L_DESCENDANT, start, false );
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LPDRAWITEMSTRUCT lpdis = lpDrawItemStruct;


	TCHAR       szTabText[100];
	RECT        rect;
	//	UINT        bkColor;
	int			txtColor;
	//	CBrush* cbr;
	CString cim;

	const TCHAR* pC;
	pC = szTabText;

	TC_ITEM     tci;

	//	cbr = &m_brRed;

	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB);

	memset(szTabText, '\0', sizeof(szTabText));

	if (pTabCtrl->m_hWnd == lpdis->hwndItem)
	{
		// which tab?
		switch (lpdis->itemID)
		{
		case REPEATED:
			//			cbr = &m_brRed;
			txtColor = RGB(255, 0, 0, );
			pC = L" Teljes t�bl�zat";
			break;

		case NOTREPEATED:
			//			cbr = &m_brYellow;
			txtColor = RGB(0, 0, 255);
			pC = L" Ism�tl�sek n�lk�l";
			break;
		}

		tci.mask = TCIF_TEXT;
		tci.pszText = szTabText;
		tci.cchTextMax = sizeof(szTabText) - 1;
		pTabCtrl->GetItem(lpdis->itemID, &tci);

		CDC* dc = CDC::FromHandle(lpdis->hDC);

		//		dc->FillRect(&lpdis->rcItem, cbr);
		//		dc->SetBkColor(bkColor);
		dc->SetTextColor(txtColor);
		//		SetTextAlign(lpdis->hDC, TA_CENTER);
		TextOut(lpdis->hDC,
			lpdis->rcItem.left,
			lpdis->rcItem.top,
			tci.pszText,
			lstrlen(tci.pszText));
	}
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CDescendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam)//wparam: F vagy Q t�bla, lparam: az CLIstCtrl*
{
	if (wParam == REPEATED)
		fullTable();
	else
		uniqueTable();
	return TRUE;
}
/////////////////////////////t//////////////////////////////////////////////////////////////////////////////////
LRESULT CDescendantsLinearTable::OnSetRowColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor = (CELLCOLOR*)lParam;
	cColor->bg = YELLOW;
	cColor->tx = BLACK;

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnIndentedList()
{
	ShellExecute(NULL, L"open", m_htmlPathName1, NULL, NULL, SW_SHOWNORMAL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::fullTable()
{
	m_TabCtrl.SetCurSel(REPEATED);
	int nItem = 0;
	CString gen;
	CString people;
	CString idC;
	CString idF;
	CString familyname;
	DE::DESC desc;

	CProgressWnd wndP(NULL, L"Line�ris teljes lista k�sz�t�se...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vDesc->size());


	m_ListCtrl = &(m_aF.m_ListCtrlF);
	for (int i = 0; i < vDesc->size(); ++i)
	{
		desc = vDesc->at(i);
		if ( p_descendantName == DE::RAISED && desc.lastname != m_lastnamePrev)
		{
			m_lastnamePrev = desc.lastname;

			str.Format(L"%d", desc.clrTextBk);
			nItem = m_ListCtrl->InsertItem(nItem, str);
			m_ListCtrl->SetItemText(nItem, L_CLRTEXT, L"");
			m_ListCtrl->SetItemText(nItem, L_NAME, L"");
			m_ListCtrl->SetItemText(nItem, L_ROWID, L"");
			m_ListCtrl->SetItemText(nItem, L_LINENUMBER, L"");
			m_ListCtrl->SetItemText(nItem, L_NUMOFD, L"");
			m_ListCtrl->SetItemText(nItem, L_ISM, L"");
			m_ListCtrl->SetItemText(nItem, L_ID, L"");
			m_ListCtrl->SetItemText(nItem, L_IDC, L"");
			m_ListCtrl->SetItemText(nItem, L_IDF, L"");
			m_ListCtrl->SetItemText(nItem, L_MINDEX, L"");
			m_ListCtrl->SetItemText(nItem, L_DBC, L"");
			m_ListCtrl->SetItemText(nItem, L_GEN, L"");

			familyname = getSeededName(i);
			m_ListCtrl->SetItemText(nItem, L_DESCENDANT, familyname);
			++nItem;
		}
		

//		if (!p_womenDescendants && desc.parentSex == WOMAN) goto cont;

		str.Format(L"%d", desc.clrTextBk);
		nItem = m_ListCtrl->InsertItem(nItem, str);

		str.Format(L"%d", desc.clrText);
		m_ListCtrl->SetItemText(nItem, L_CLRTEXT, str);

		m_ListCtrl->SetItemText(nItem, L_NAME, desc.name);
		m_ListCtrl->SetItemText(nItem, L_ROWID, desc.rowid);
		m_ListCtrl->SetItemText(nItem, L_LINENUMBER, desc.linenumber );

		str.Empty();
		if (desc.nOfD)
			str.Format(L"%d", desc.nOfD);
		m_ListCtrl->SetItemText(nItem, L_NUMOFD, str);

		str.Empty();
		if (desc.cntRep)
			str.Format(L"%d", desc.cntRep);
		m_ListCtrl->SetItemText(nItem, L_ISM, str);

		str.Format(L"%d", vDesc->at(i).id);
		m_ListCtrl->SetItemText(nItem, L_ID, str);

		idC.Empty();
		if (desc.idC > 0)
			idC.Format(L"%d", desc.idC);
		m_ListCtrl->SetItemText(nItem, L_IDC, idC);

		idF.Empty();
		if (desc.idF >= 0 && i)
			idF.Format(L"%d", desc.idF);
		m_ListCtrl->SetItemText(nItem, L_IDF, idF);

		m_ListCtrl->SetItemText(nItem, L_MINDEX, desc.ixM);

		str.Empty();
		if (desc.numOfChildren)
			str.Format(L"%d", desc.numOfChildren);
		m_ListCtrl->SetItemText(nItem, L_DBC, str);

		gen = get_gABC(desc.g);
		str.Format(L"%3s-%-4d", gen, desc.childorder);
		m_ListCtrl->SetItemText(nItem, L_GEN, str);

		people = getComplexDescriptionL(i, true);
		m_ListCtrl->SetItemText(nItem, L_DESCENDANT, people);
		++nItem;

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::uniqueTable()
{
	m_TabCtrl.SetCurSel(NOTREPEATED);
	int nItem = 0;
	CString gen;
	CString people;
	CString idC;
	CString idF;
	CString familyname;
	DE::DESC desc;

	CProgressWnd wndP(NULL, L"Ism�tl�d�sek kihagy�s�val line�ris lista k�sz�t�se...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vDesc->size());

	m_lastnamePrev.Empty();
	m_ListCtrl = &(m_aU.m_ListCtrlU);
	for (int i = 0; i < vDesc->size(); ++i)
	{
		desc = vDesc->at(i);
		if (desc.id == -1) goto cont;
		if (desc.cntRep > 1 ) goto cont;
		if (p_descendantName == DE::RAISED && desc.lastname != m_lastnamePrev)
		{
			m_lastnamePrev = desc.lastname;

			str.Format(L"%d", desc.clrTextBk);
			nItem = m_ListCtrl->InsertItem(nItem, str);
			m_ListCtrl->SetItemText(nItem, L_CLRTEXT, L"");
			m_ListCtrl->SetItemText(nItem, L_NAME, L"");
			m_ListCtrl->SetItemText(nItem, L_ROWID, L"");
			m_ListCtrl->SetItemText(nItem, L_LINENUMBER, L"");
			m_ListCtrl->SetItemText(nItem, L_NUMOFD, L"");
			m_ListCtrl->SetItemText(nItem, L_ID, L"");
			m_ListCtrl->SetItemText(nItem, L_IDC, L"");
			m_ListCtrl->SetItemText(nItem, L_IDF, L"");
			m_ListCtrl->SetItemText(nItem, L_MINDEX, L"");
			m_ListCtrl->SetItemText(nItem, L_DBC, L"");
			m_ListCtrl->SetItemText(nItem, L_GEN, L"");

			familyname = getSeededName(i);
			m_ListCtrl->SetItemText(nItem, L_DESCENDANT, familyname);
			++nItem;
		}
		if (!p_womenDescendants && desc.parentSex == WOMAN) return;

		str.Format(L"%d", desc.clrTextBk);
		nItem = m_ListCtrl->InsertItem(nItem, str );

		str.Format(L"%d", desc.clrText);
		m_ListCtrl->SetItemText(nItem, L_CLRTEXT, str );

		m_ListCtrl->SetItemText(nItem, L_NAME, desc.name);
		m_ListCtrl->SetItemText(nItem, L_ROWID, desc.rowid);
		m_ListCtrl->SetItemText(nItem, L_LINENUMBER, desc.linenumber );

		str.Empty();
		if (desc.nOfD)
			str.Format(L"%d", desc.nOfD);
		m_ListCtrl->SetItemText(nItem, L_NUMOFD, str);


		str.Empty();
		if (desc.cntRep)
			str.Format(L"%d", desc.cntRep);
		m_ListCtrl->SetItemText(nItem, L_ISM, str);

		str.Format(L"%d", vDesc->at(i).id);
		m_ListCtrl->SetItemText(nItem, L_ID, str);

		idC.Empty();
		if (desc.idC > 0)
			idC.Format(L"%d", desc.idC);
		m_ListCtrl->SetItemText(nItem, L_IDC, idC );

		idF.Empty();
		if (desc.idF >= 0 && i)
			idF.Format(L"%d", desc.idF);
		m_ListCtrl->SetItemText(nItem, L_IDF, idF);

		m_ListCtrl->SetItemText(nItem, L_MINDEX, desc.ixM );

		str.Empty();
		if (desc.numOfChildren)
			str.Format(L"%d", desc.numOfChildren);
		m_ListCtrl->SetItemText(nItem, L_DBC, str);

		gen = get_gABC(desc.g);
		str.Format(L"%3s-%-4d", gen, desc.childorder);
		m_ListCtrl->SetItemText(nItem, L_GEN, str);

		people = getComplexDescriptionL(i, true);
		m_ListCtrl->SetItemText(nItem, L_DESCENDANT, people);
		++nItem;
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getComplexDescriptionL(int i, bool parentIndex)
{
	DE::DESC desc = vDesc->at(i);
	queryPeople(desc.rowid, &p);
	CString people = createDescendantL(i, parentIndex);
	CString spouses = createSpousesL(i);
	CString arm;
	CString csalad;
	CString folyt;


	people.Trim();
	people += L" ";
	people += spouses.Trim();
	people.Trim();

	if (!p.arm.IsEmpty())
	{
		people += p.arm;
	}
	if (!p.csalad.IsEmpty())
	{
		people += p.csalad;
	}
	return people;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createDescendantL(int i, bool parentIndex)
{
	CString name;
	CString line;
	CString titolo;
	CString rang;
	CString lastname;
	CString comment;
	int motherIndex;
	int z;
	int j;
	int last;
	int	lastMotherIndex;
	DE::LMX lmx;
	TCHAR ch;

	name = p.first_name;
	if (p_descendantName == DE::INLINE)
	{
		lastname = p.last_name;
		if (lastname == L"N") lastname.Empty();
		if (p_capital)
			lastname = convertNameToUpper(lastname);
		
		if (lastname == L"N;")
			name.Format(L"\n%s %s %s", lastname, p.first_name, p.peer);
		else
			name.Format(L"\n%s %s %s %s", p.titolo, lastname, p.first_name, p.peer);
		name.Trim();
		if (!p.other_names.IsEmpty())
			name.Format(L"\n%s_(%s)", (CString)name, p.other_names);
	}

	if (i && vDesc->at(vDesc->at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apj�nak t�bb feles�ge volt
	{
		last = vLMX.size() - 1;
		if (last == -1 || vLMX.at(last).g < vDesc->at(i).g)		// magasabb gener�ci�ban van
		{
			lmx.g = vDesc->at(i).g;
			lmx.lastMotherIndex = p.parentIndex - 1;
			vLMX.push_back(lmx);
		}
		else													// alacsonyabb gener�ci�ban van
		{
			// megkeresi saj�t gener�ci�j�nak utolj�ra list�zott motherIndex-�t
			j = vLMX.size() - 1;
			for (j = vLMX.size() - 1; j > 0; --j)
			{
				if (vLMX.at(j).g > vDesc->at(i).g) // a nagyobb gener�ci�k eldob�sa
					vLMX.pop_back();
			}
		}
		last = vLMX.size() - 1;



		if (parentIndex)
		{
			if (vLMX.at(last).lastMotherIndex != p.parentIndex)   // ha az utolj�ra ki�rt motherIndex m�s, akkor ezt ki�rja
			{
				if (p.parentIndex)
				{
					str.Format(L"/%d", p.parentIndex);
					name += str;
					vLMX.at(last).lastMotherIndex = p.parentIndex;
				}
			}
		}

	}

	if (p.comment.GetAt(0) != ',')
		name += L" ";

	if (!p.posterior.IsEmpty())
	{
		name += p.posterior;
	}


	str = getPlaceDateBlockL1(p.birth_place, p.birth_date, L"*");
	line = name.Trim();
	if (!str.IsEmpty())
		line.Format(L"%s %s", name, str);

	str = getPlaceDateBlockL1(p.death_place, p.death_date, L"+");
	if (!str.IsEmpty())
		line.Format(L"%s %s", (CString)line, str);

	str.Empty();
	if (!p.comment.IsEmpty())
	{
		if (!p.occupation.IsEmpty())
			str.Format(L"%s, %s", p.comment, p.occupation);
		else
			str = p.comment;
	}
	else
	{
		if (!p.occupation.IsEmpty())
			str = p.occupation;
	}

	comment = str;
	if (str.GetAt(0) == ',')
		line.Format(L"%s%s", (CString)line, comment);
	else
		line.Format(L"%s %s", (CString)line, comment);


	++m_listedP;
	++m_listedD;
	return(line);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createSpousesL(int i)
{
	CString rowid = vDesc->at(i).rowid;
	CString rowidS;
	CString place;
	CString date;
	CString spouse_id;
	CString marriage;
	CString spouse;
	CString spRelatives;
	CString spouses;

	if (p.sex_id == MAN)
		m_command.Format(L"SELECT place, date, wife_id FROM marriages WHERE husband_id='%s' ORDER BY whichWife", rowid);
	else
		m_command.Format(L"SELECT place, date, husband_id FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", rowid);
	if (!queryM(m_command)) return L"";

	int numberOfSpouses = rsM.RecordsCount();

	vFullname.clear();
	for (int i = 0; i < numberOfSpouses; ++i, rsM.MoveNext())
	{
		place = rsM.GetFieldString(0);
		date = rsM.GetFieldString(1);
		spouse_id = rsM.GetFieldString(2);
		if (!spouse_id.IsEmpty() && spouse_id.Compare(L"0"))
		{
			queryPeople(spouse_id, &s);   // h�zast�rs adatainak beolvas�sa
			marriage = createMarriageL(place, date, i, numberOfSpouses);
			spouse = createSpouseL();				// ki�r�s el�tt ellen�rzi, hogy szerepel-e ilyen n�vem m�r ki�rt h�zast�rs. Ha igen, akkor sz�nezi
			vFullname.push_back(s.fullname);	// vFullname-en gy�jti a m�r kilist�zott h�zast�rsak nev�t
			spRelatives = createSpRelativesL();
			//			print(spRelatives);
			if (!marriage.IsEmpty())
				spouses += marriage;
			if (!spouse.IsEmpty())
				spouses += spouse;
			if (!spRelatives.IsEmpty())
				spouses += spRelatives;
		}
	}
	spouses.Trim();
	return spouses;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::createMarriageL(CString place, CString date, int i, int numberOfSpouses)
{
	CString marriage;

	if (numberOfSpouses > 1)
		marriage.Format(L" %d=", i + 1);
	else
		marriage = L" =";

	if (!place.IsEmpty())
	{
		marriage += place;
		if (date.IsEmpty())
			marriage += L",";
		marriage += L" ";
	}
	if (!date.IsEmpty())
	{
		marriage += date;
		marriage += L" ";
	}
	return marriage;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs list�zados blokkj�t elk�sz�ti
CString CDescendantsLinearTable::createSpouseL()
{
	CString spouse;
	CString fullname;
	CString lastname = s.last_name;

	if (p_capital)
		lastname = convertNameToUpper(lastname);
	if (lastname != L"N;" && !s.titolo.IsEmpty() && s.peer.IsEmpty())
	{
		fullname += s.titolo;
		fullname += L" ";
	}
	if (!s.last_name.IsEmpty())
	{
		fullname += lastname;
		fullname += L" ";
	}
	if (!s.other_names.IsEmpty())
	{
		fullname.TrimRight();
		fullname += L"_(";
		fullname += s.other_names;
		fullname += L") ";
	}
	if (!s.first_name.IsEmpty())
	{
		fullname += s.first_name;
	}
	fullname.Trim();


	if (!s.title.IsEmpty())
	{
		fullname.Format(L"%s %s", s.title, (CString)fullname);
	}
	if (!s.peer.IsEmpty())
	{
		fullname.Format(L"%s %s", s.peer, (CString)fullname);
	}

	if (!s.posterior.IsEmpty())
	{
		fullname += L" ";
		fullname += s.posterior;
//		if (p_checkCRLF)
//			fullname += L"\n";
	}
	fullname.Trim();
	spouse = fullname;

	//	m_birth = s.birth_date;
	str = getPlaceDateBlock(s.birth_place, s.birth_date, L"*");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	str = getPlaceDateBlock(s.death_place, s.death_date, L"+");
	if (!str.IsEmpty())
		spouse.Format(L"%s %s", (CString)spouse, str);

	//str = getColoredString(s.comment, p_commentAttrib);
	str = s.comment;
	if (!str.IsEmpty())
	{
		if (s.comment.GetAt(0) == ',')
			spouse.Format(L"%s%s", (CString)spouse, str);
		else
			spouse.Format(L"%s %s", (CString)spouse, str);
	}

	++m_listedP;
	return spouse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a h�zast�rs szem�lyle�r� blokkj�t elk�sz�ti, a neveket bold-dal kiemnelve
CString CDescendantsLinearTable::createSpRelativesL()
{
	CString father(L"");
	CString mother(L"");
	CString parents;
	int		numOfSpouses;
	CString spouse_id;
	CString order;
	CString spouseSpouse;
	int pos;

	if (!s.father_id.IsEmpty() && s.father_id != L"0")
	{
		queryPeople(s.father_id, &sf);
		sf.first_name.Replace('-', ' ');
		father = getFirstWord(sf.first_name);
	}
	if (!s.mother_id.IsEmpty() && s.mother_id != L"0")
	{
		queryPeople(s.mother_id, &sm);
		mother = getLastFirst(&sm, false);
		mother.Trim();
	}

	if (!father.IsEmpty() && !mother.IsEmpty())		// ( apa �s anya is meg van adva )
	{
		parents.Format(L" (%s-%s", father, mother);
		++m_listedP;
		++m_listedP;
	}

	if (!father.IsEmpty() && mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", father);
		else
			parents.Format(L" (%s l�nya", father);
		++m_listedP;
	}

	if (father.IsEmpty() && !mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", mother);
		else
			parents.Format(L" (%s l�nya", mother);
		++m_listedP;
	}


	if (s.sex_id == MAN)
		m_command.Format(L"SELECT wife_id, whichWife  FROM marriages WHERE husband_id='%s' ORDER BY whichWife", s.rowid); // a h�zast�rs h�zast�rsai
	else
		m_command.Format(L"SELECT husband_id, whichHusband FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", s.rowid); // a h�zast�rs h�zast�rsai
	if (!query(m_command)) return L"";
	numOfSpouses = rs.RecordsCount();
	if (numOfSpouses > 1)
	{
		vSpouseIndex.clear();
		for (int i = 0; i < numOfSpouses; ++i, rs.MoveNext())
		{
			spouse_id = rs.GetFieldString(0);
			order = rs.GetFieldString(1);
			order.Format(L"%d", i + 1);
			queryPeople(spouse_id, &ss);
			if (ss.rowid != p.rowid)		// a GA sorban szerepl� h�zast�rsat kihagyja
			{
				spouseSpouse = getLastFirst(&ss, false);

				if (parents.GetLength())
					parents += L", ";
				else
					parents = L" (";

				str.Format(L"%sf. %s", order, spouseSpouse);
				++m_listedP;
				parents += str;
				vSpouseIndex.push_back(order);
			}
		}
	}
	if (!parents.IsEmpty())
	{
		parents.TrimRight();
		parents += L")";
	}
	return(parents);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendantsLinearTable::getSeededName(int i)
{
	CString familyname;
	DE::DESC desc = vDesc->at(i);
	familyname = desc.lastname.MakeUpper();
	if (theApp.m_inputMode == GAHTML)
	{
		m_command.Format(L"SELECT tableHeader FROM tables WHERE rowid='%s'", desc.tablenumber);
		if (!query(m_command)) return L"";

		familyname.Format(L"%s, %s %s", (CString)familyname, desc.titolo, rs.GetFieldString(0));
	}
	return familyname;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////4
