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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool sortByNumOfD(const CH& d1, const CH& d2)
{
	return(d1.numOfD > d2.numOfD);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool sortById2(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.id < d2.id);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool sortByStatus(const DE::DESC& d1, const DE::DESC& d2)
{
	return(d1.status < d2.status );
}
*/
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

	CProgressWnd wndP(NULL, L"Generáció váltások jelölése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vDesc->size());
	for (int i = 0; i < vDesc->size(); ++i)
	{
		if (vDesc->at(i).id == -1) goto cont;
		// generáció váltások jelölése a clrTextBk-ban
		if (vDesc->at(i).g != gPrev)
			tippingBk = !tippingBk;
		gPrev = vDesc->at(i).g;
		vDesc->at(i).clrTextBk = tippingBk;

		// az azonos szülõkhöz tartozó gyerekek jelölése a clrText-ben
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
		fs_vGKR2 = theApp.openTextFile(&flvGKR2, L"vGKR2", L"w+");  // log fájl
		fs_vSp = theApp.openTextFile(&flvSp, L"vSp", L"w+");  // log fájl
		fs_vFC = theApp.openTextFile(&flvFC, L"vCF", L"w+");  // log fájl
		fs_vSib = theApp.openTextFile(&flvSib, L"vSib", L"w+");  // log fájl		
	}

	create_vGKR2();		// vGKR kibõvítése ixF és ixF -el és togle-al vSp látrahozása
*/

	m_RichEditCtrl.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	m_titleF.Format(L"%s teljes leszármazotti listája", m_name);
	m_titleU.Format(L"%s leszármazotti listája ismétlések kihagyásával", m_name);



	m_aF.Create(IDD_DESCENDANTS_FULL, this);
	m_aU.Create(IDD_DESCENDANTS_UNIQUE, this);

	int i = 0;

	TCITEM tcItem;
	tcItem = { 0 };
	tcItem.mask = TCIF_TEXT | TCIF_PARAM | TCIF_STATE;
	tcItem.pszText = L"Ismétlések nélkül";
	tcItem.lParam = 0;
	tcItem.dwStateMask = TCIS_HIGHLIGHTED;
	tcItem.dwState = TCIS_HIGHLIGHTED;

	m_TabCtrl.InsertItem(i, &tcItem);
	++i;
	m_TabCtrl.InsertItem(i, L"Ismétlésekkel");

	attachDialogs();

	SetWindowTextW(theApp.m_caption);
	ShowWindow(SW_MAXIMIZE);

 	m_RichEditCtrl.SetWindowTextW(m_titleF);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);
	m_RichEditCtrl.SetBackgroundColor(false, RGB(240, 240, 240));


	CMenu* menu;
	menu = GetMenu();
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);


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
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsLinearTable::PreTranslateMessage(MSG* pMsg)
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
			pC = L" Teljes táblázat";
			break;

		case NOTREPEATED:
			//			cbr = &m_brYellow;
			txtColor = RGB(0, 0, 255);
			pC = L" Ismétlések nélkül";
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
LRESULT CDescendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam)//wparam: F vagy Q tábla, lparam: az CLIstCtrl*
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
	ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);
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

	CProgressWnd wndP(NULL, L"Lineáris teljes lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vDesc->size());


	m_ListCtrl = &(m_aF.m_ListCtrlF);
	for (int i = 0; i < vDesc->size(); ++i)
	{
		desc = vDesc->at(i);
//		if (desc.id == -1) 
//			goto cont;

		if ( p_descendantName == DE::RAISED && desc.lastname != m_lastnamePrev)
		{
			m_lastnamePrev = desc.lastname;

			str.Format(L"%d", desc.clrTextBk);
			nItem = m_ListCtrl->InsertItem(nItem, str);
			m_ListCtrl->SetItemText(nItem, L_CLRTEXT, L"");
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
		

		if (!p_womenDescendants && desc.parentSex == WOMAN) goto cont;

		str.Format(L"%d", desc.clrTextBk);
		nItem = m_ListCtrl->InsertItem(nItem, str);

		str.Format(L"%d", desc.clrText);
		m_ListCtrl->SetItemText(nItem, L_CLRTEXT, str);

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

	CProgressWnd wndP(NULL, L"Ismétlõdések kihagyásával lineáris lista készítése...");
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

	if (i && vDesc->at(vDesc->at(i).parentIndex).numOfSpouses > 1)  // csak az kaphat motherIndexet, akinek az apjának több felesége volt
	{
		last = vLMX.size() - 1;
		if (last == -1 || vLMX.at(last).g < vDesc->at(i).g)		// magasabb generációban van
		{
			lmx.g = vDesc->at(i).g;
			lmx.lastMotherIndex = p.parentIndex - 1;
			vLMX.push_back(lmx);
		}
		else													// alacsonyabb generációban van
		{
			// megkeresi saját generációjának utoljára listázott motherIndex-ét
			j = vLMX.size() - 1;
			for (j = vLMX.size() - 1; j > 0; --j)
			{
				if (vLMX.at(j).g > vDesc->at(i).g) // a nagyobb generációk eldobása
					vLMX.pop_back();
			}
		}
		last = vLMX.size() - 1;



		if (parentIndex)
		{
			if (vLMX.at(last).lastMotherIndex != p.parentIndex)   // ha az utoljára kiírt motherIndex más, akkor ezt kiírja
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


	str = getPlaceDateBlock(p.birth_place, p.birth_date, L"*");
	line = name.Trim();
	if (!str.IsEmpty())
		line.Format(L"%s %s", name, str);

	str = getPlaceDateBlock(p.death_place, p.death_date, L"+");
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
			queryPeople(spouse_id, &s);   // házastárs adatainak beolvasása
			marriage = createMarriageL(place, date, i, numberOfSpouses);
			spouse = createSpouseL();				// kiírás elõtt ellenõrzi, hogy szerepel-e ilyen névem már kiírt házastárs. Ha igen, akkor színezi
			vFullname.push_back(s.fullname);	// vFullname-en gyûjti a már kilistázott házastársak nevét
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
// a házastárs listázados blokkját elkészíti
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
// a házastárs személyleíró blokkját elkészíti, a neveket bold-dal kiemnelve
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

	if (!father.IsEmpty() && !mother.IsEmpty())		// ( apa és anya is meg van adva )
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
			parents.Format(L" (%s lánya", father);
		++m_listedP;
	}

	if (father.IsEmpty() && !mother.IsEmpty())
	{
		if (s.sex_id == MAN)
			parents.Format(L" (%s fia", mother);
		else
			parents.Format(L" (%s lánya", mother);
		++m_listedP;
	}


	if (s.sex_id == MAN)
		m_command.Format(L"SELECT wife_id, whichWife  FROM marriages WHERE husband_id='%s' ORDER BY whichWife", s.rowid); // a házastárs házastársai
	else
		m_command.Format(L"SELECT husband_id, whichHusband FROM marriages WHERE wife_id='%s' ORDER BY whichHusband", s.rowid); // a házastárs házastársai
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
			if (ss.rowid != p.rowid)		// a GA sorban szereplõ házastársat kihagyja
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
/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::OnDescendantLength()
{
	m_startTime = theApp.getStartTime();
	CString file;
	CString title;
	vD.clear();

	m_listedP = 0;
	m_listedD = 0;

	for (int i = 0; i < vDesc->size(); ++i)
		vDesc->at(i).procChildren = 0;
	// Az õs berakása a vDesc vektorba
	DE::DESC desc;
	desc = vDesc->at(0);

	m_rowid = desc.rowid;
	m_tablenumber = desc.tablenumber;
	vD.clear();
	vD.push_back(desc);
	m_os = desc.name;

	openHtml();
	descendants();
	printVector(0);
	closeHtml();
	ShellExecute(NULL, L"open", m_htmlPathName, NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::openHtml()
{
	CString descendantsPath;
	CString dragonjpgPathName;
	CString file;
	file = L"descendantsLength";

	descendantsPath.Format(L"%s\\%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(descendantsPath, 0))
		_wmkdir(descendantsPath);
	dragonjpgPathName.Format(L"%s\\dragon.jpg", descendantsPath);
	CopyFile(theApp.m_dragonjpg, dragonjpgPathName, false);

	m_htmlPathName.Format(L"%s\\%s_leszármazotti táblája_%s.html", descendantsPath, m_os, getTimeTag());

	if (!openFileSpec(&fhDescTable, m_htmlPathName, L"w+")) return false;

	int l = -37;
	CString today;
	today = getPresentDateTime();
	CString yesno;
	CString nok;
	if (p_womenDescendants)
		nok = L"igen";
	else
		nok = L"nem";

	CString kihagy;
	switch (p_repeated)
	{
	case 0:
		kihagy = L"Nem hagyja aki az ismétlõdõ leszármazottakat.";
		break;
	case 1:
		kihagy = L"Az elsõ leszármazottat kiírja, a többit elhagyja.";
		break;
	case 2:
		kihagy = L"Ha az apja leszármazott, akkor kiírja, ha az anyja, akkor nem";
	}

	CString maxGen;
	if (p_generationMax.IsEmpty())
		maxGen = L"minden generáció";
	else
		maxGen = p_generationMax;

	CString inputFile;
	CString created;
	m_command = L"SELECT filespec, created FROM filespec";
	if (!theApp.query(m_command)) return false;
	inputFile = theApp.m_recordset->GetFieldString(0);
	created = theApp.m_recordset->GetFieldString(1);

	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	CString connect;
	connect = p_connect ? L"igen" : L"nem";


	print(L"<!DOCTYPE html>");
	print(L"<html lang=hu>");
	print(L"<head>");
	str.Format(L"<title>%s</title>", today);
	print(str);
	print(L"<meta charset=\"UTF-8\">");
	createStyle();
	print(L"</head>");
		
	CString body;
	body.Format(L"<body bgcolor=\"%06x\">", p_colorBgrnd);
	print(body);



	str.Format(L"<b><center>%s leszármazotti táblája</center></b><br>", m_os );
	print(str);
	print(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>");
	
	print(L"<br>\n<pre>\n");
	str.Format(L"%*s Dragon v. %s", l, L"Program:", theApp.m_version);
	print(str);
	str.Format(L"%*s %s", l, L"Dragon.exe készült:", MultiToUnicode(LPCSTR(BUILD)));
	print(str);

	str.Format(L"%*s %s", l, L"Alapkönyvtár:", theApp.m_dbFolderPath);
	print(str);

	str = L"ÜRES";
	if (theApp.m_inputMode == MANUAL)
		str = L"kézi adatbevitel";
	else if (theApp.m_inputMode == GEDCOM)
		str = L"GEDCOM fájl";
	else if (theApp.m_inputMode = GAHTML)
		str = L"GA.htm fájl";

	str.Format(L"%*s %s", l, L"Adatbázis bemenete:", (CString)str);
	print(str);

	if (theApp.m_inputMode == GAHTML || theApp.m_inputMode == GEDCOM)
	{
		str.Format(L"%*s %s %s", l, L"Bementi fájl készült:", created, inputFile);
		print(str);
	}
	str.Format(L"%*s %s %s", l, L"Adatbázis készült:", dateDB, theApp.m_dbFileName);
	print(str);
	if (!theApp.m_inputVersion.IsEmpty() && theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvasás programverziója:", theApp.m_inputVersion);
		print(str);
	}
	if (!theApp.m_uniteVersion.IsEmpty())
	{
		if (theApp.m_uniteVersion != theApp.m_version)
		{
			str.Format(L"%*s %s", l, L"Összevonás programverziója:", theApp.m_uniteVersion);
			print(str);
		}
	}

	str.Format(L"%*s %s<br>", l, L"Lista készült:", theApp.getPresentDateTime());
	print(str);
	str.Format(L"%*s %s", l, L"Generációk max száma:", maxGen);
	print(str);
	str.Format(L"%*s %s", l, L"Elágazások összekötése:", connect);
	print(str);
	str.Format(L"%*s %s", l, L"Nõk leszármazottai:", nok);
	print(str);

	CString sorrend;
	sorrend = L"csökkenõ hosszúságú leszármazotti szálak";
	str.Format(L"%*s %s", l, L"Sorrend:", sorrend);
	print(str);

	if (p_repeatedColor)
	{
		if (p_repeated == 0)
			str.Format(L"%*s %s", l, L"Ismétlõdõk színezése:", L"zöld - elsõ elõfordulás, piros - újabb elõfordulás");
		else
			str.Format(L"%*s %s", l, L"Ismétlõdõk színezése:", L"zöld");
		print(str);
	}
	print(L"</pre>\n\n");


	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::createStyle()
{

	print( L"<style>");
	if (p_rowWidth != L"képernyõ" && p_rowWidth != L"0")
	{
		str.Format(L"html{width:%scm;}", p_rowWidth);
		print(str);
	}


	CString font;
	font = L"body {font-family: courier, serif;}";
	font = L"body {font-family: Arial, Helvetica, sans-serif;}";
	font = L"body {font-family: Verdana;}";
	font = L"<body class=\"w3-container\">";
	//	fwprintf( fl, L"%s\n", font );
	CString fontSize;
	fontSize.Format(L"body {font-size: %dpx;}", 10 + 2 * p_fontSize);
	str.Format( L"%s", fontSize);
	print(str);

	//	fwprintf( fl, L"body {font-size: 10px;}" );


	//	CString font;
	//	font = L"<font-family: Avenir Next, Calibri, Verdana, sans-serif>\n";
	//	print( font );

	print( L"ul {");
	print( L"display: block;");
	print( L"max-width: 200%;");
//	print( L"list-style-type: square;\n" );
	print( L"list-style-type: none;");
	print( L"margin-top: 0 em;");
	print( L"margin-bottom: 0 em;");
	print( L"margin-left: 0;");
	print( L"margin-right: 0;");
	print( L"padding-left: 40px;");
	print( L"}");

	print( L"ol {");
	print( L"display: block;");
	print( L"list-style-type: decimal;");
	print( L"margin-top: 0 em;");
	print( L"margin-bottom: 0 em;");
	print( L"margin-left: 0;");
	print( L"margin-right: 0;");
	print( L"padding-left: 40px;");
	print( L"}");
	print( L"</style>");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDescendantsLinearTable::closeHtml()
{
	print( L"</body>");
	print( L"</html>");
	int l = 46;

	if (m_actGen == -100)
	{
		str.Format(L"A generációk száma elérte az elõírt maximumot, ezért a listázás abbamaradt.<br><br>");
		print(str);
	}
	if (m_givup)
	{
		str.Format(L"A leszármazottak száma elérte a %d-t, a listázás abbamaradt.<br><br>", m_maxDesc);
		print(str);
	}
	if (m_canceled)
	{
		str = L"A leszármazottak listázása megszakítva.<br><br>";
		print(str);
	}
	int abc = 26;
	int g = m_gMax + 1;
	int cycle = g / abc;
	int bias = g % abc;
	TCHAR gen;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;


	//	if ( theApp.v_tableNumbers.size() == 1   || !theApp.v_rowid.size() == 1 )
	{

		print(L"<pre>");
		str.Format(L"%-*s %8s(%c)", l, L"Listázott generációk száma:", thousand(bias), gen);
		print(str);
		str.Format(L"%-*s %8s", l, L"Listázott leszármazottak száma:", thousand(m_listedD));
		print(str);
		if (p_repeated)
		{
			str.Format(L"%-*s %8s", l, L"Ismétlõdõ, nem listázott leszármazottak száma:", thousand(m_cntRepeated));
			print(str);
		}
		str.Format(L"%-*s %8s", l, L"Összes listázott emberek száma:", thousand(m_listedP));
		print(str);
		str.Format(L"%-*s %s", l, L"Futási idõ:", theApp.getTimeElapsed(m_startTime));
		print(str);
		print(L"</pre>");
	}
	fclose(fhDescTable);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::printVector(int tbl)
{
	//	CString tableheader;
	//	CString tableNumber;
	CString people;
	CString line;
	DE::DESC desc;
	//	tableNumber = theApp.v_tableNumbers.at(tbl);
	//	m_command.Format(L"SELECT tableHeader FROM tables WHERE rowid = '%s'", tableNumber);
	//	if (!theApp.query(m_command)) return;
	//	tableheader = theApp.m_recordset->GetFieldString(0);
	//	str.Format(L"<b>%s</b>\n\n", tableheader);
	//	print(str);

	int ix;

	if (p_numberingSystem == DE::SZLUHA) // orderd list
	{
		m_tag1 = L"<ol>";
		m_tag2 = L"</ol>\n";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>";
		m_tag2 = L"</ul>\n";
	}

	m_indent = 0;
	m_genPrev = 0;

	m_familyName.Empty();
	vLMX.clear();		// a generáció utolsó kiírt motherIndexe
	vSerial.clear();
	vSerial.push_back(1);
	str.Format(L"%s leszármazottainak tabulált listázása...", vD.at(0).name );
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, vD.size());
	wndP.SetPos(0);

	for (int i = 0; i < vD.size(); ++i)
	{
		wndP.SetPos(i);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

		desc = vD.at(i);

		if (vDesc->at(i).hidden) continue;   // apa bejegyzése, aki más táblában szerepel

		queryPeople(vD.at(i).rowid, &p);

		if (!p_womenDescendants && vD.at(i).parentSex == WOMAN) continue;

		printBegining(i);	// html kódok és generáció elkészítése; 
		people = getComplexDescriptionL1(i,true);

		line.Format(L"%s%s", m_diamond, people);
		line.Replace('|', '\'');
		print(line);
		m_genPrev = vD.at(i).g;
	}

	for (int i = 0; i < m_indent; ++i)			// a kihúzások megszüntetése, vissza az alapvonalra
	{
		fwprintf(fhDescTable, m_tag2);
		print(str);
	}
	wndP.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A genráció változástól függû behuzás és genrációs kód nyomtatása az m_sytax értékétõl függõen
void CDescendantsLinearTable::printBegining(int i)
{
	CString family;
	CString shift;
	int z;
	int g = vD.at(i).g;

	if (g > m_genPrev || m_genPrev == 0)	// jobbra tolás, új sor generációja nagyobb, (csak 1-esével nõhet a generáció)
	{
		shift = m_tag1;
		++m_indent;
	}
	else if (g < m_genPrev)					// balra tolás vissza, régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for (UINT i = 0; i < (m_genPrev - g); ++i)
		{
			shift += m_tag2;
			--m_indent;
		}
	}

	TCHAR gen;
	int abc = 26;

	int cycle = (m_indent - 1) / abc;
	int bias = (m_indent - 1) % abc;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;

	gen = get_gABC(g);

	// Kiemelt családnév //////////////////////////////////////
	if (p_descendantName == DE::RAISED)
	{
		if (m_familyName != p.last_name)
		{
			family = getTableHeader();
			str = getColoredString(family, DE::KEK);
			if (p_repeatedColor)
			{
				switch (vDesc->at(i).status)
				{
				case 0:
					str = getColoredString(family, DE::KEK);
					break;
				case 1:
					str = getColoredString(family, DE::ZOLD);
					break;
				case 2:
					str = getColoredString(family, DE::PIROS);
					break;
				}
			}
			if (g == m_genPrev) // ugyanabban  a generációban névváltozás!!
				str.Format(L"<br>%s%s", shift, (CString)str);	// hogy ugyabban a betolásban legyünk, de új sorban
			else
				str.Format(L"%s%s", shift, (CString)str);			// </ol> benne van a shiftben, ha kell
			shift.Empty();		// az ezt követõ leszármazottat már nem kell tolni
			print(str);
			m_familyName = p.last_name;
		}
	}
	/////leszármazott sorának kezedete /////////////////////////////////////////////
	if (p_numberingSystem == DE::SZLUHA)
		str.Format(L"%s<li>%c&diams;", shift, gen);
	else if (p_numberingSystem == DE::VIL)
		str.Format(L"%s<li>%c%d&diams;", shift, gen, vDesc->at(i).childorder);
	else if (p_numberingSystem == DE::TUP)
	{
		z = vSerial.size() - 1;
		if (g < z)
			++vSerial.at(g);
		else
			vSerial.push_back(1);
		str.Format(L"%s<li>%c-%d&diams;", shift, gen, vSerial.at(g));
	}
	m_diamond = str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsLinearTable::descendants()
{
	CString rowid;
	int i;
	int j;
	int numOfChildren;
	int procChildren;
	int z = 0;
	int cnt = 0;
	int maxGen = _wtoi(p_generationMax) - 2;
	int fatherIndex;
	int ixC; // gyerek index;
	m_actGen = 0;
	m_gMax = 0;
	m_givup = 0;
	m_canceled = false;
	m_cntRepeated = 0;
	DE::DESC desc;

	m_listedP = 0;
	m_listedD = 0;
	m_maxDesc = 2000000;
	if (maxGen == -2) maxGen = 0;

	str.Format(L"%s leszármazottainak összegyûjtése folyik...", m_os);
	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, 10000);
	wndP.SetPos(0);

	//	m_folyt_db = 0;
	while (true)
	{
		if (cnt == 10000)	cnt = 0;
		wndP.SetPos(cnt);
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			m_canceled = true;
			break;
		}
		++cnt;

		if (maxGen && m_actGen > maxGen)
		{
			m_actGen = -100;		// jelzi, hogy a listára írja ki, hogy a generációk magas száma miatt szakdt meg a listázás
			break;
		}

		// visszamegy az utolsó emberre, akinek még van fel nem dolgozott gyereke
		for (i = vD.size() - 1; i >= 0; --i)
		{
			numOfChildren = vD.at(i).numOfChildren;
			if (numOfChildren)
			{
				procChildren = vD.at(i).procChildren;
				if (numOfChildren != procChildren)	// még van fel nem dolgozott gyerek
					break;
			}
		}
		if (i < 0) 	break;	// vége van a programnak, nincs több fel nem dolgozott gyerek!! Kilép a ciklusból

		// a kinyomtatott ember következõ, míg ki nem nyomtatott gyerekét keresi
		// szülõ index: i;
		// gyerek index 
		ixC = vD.at(i).idC + procChildren;
		m_actGen = vD.at(i).g;
		if (m_actGen >= m_gMax)
			m_gMax = m_actGen;

		desc = vDesc->at(ixC);
		desc.procChildren = 0;
		desc.order = 0;
		desc.cntRep = 0;
		desc.numOfRep = 0;
		desc.original = vD.size();
		desc.parentIndex = i;
		desc.parentSex = vD.at(i).sex;
		desc.g = vD.at(i).g + 1;		// apja generációjánál 1-el nagyobb
		desc.fatherIndex = i;
		desc.status = 0;
		desc.hidden = false;

		vD.at(i).procChildren += 1;		// a most feldolgozott testvéreinek száma nõ egyet
		desc.childorder = vD.at(i).procChildren;
	
		if (p_repeated == 1)						// ismétlõdõ leszármazottak kihagyása
		{
			for (j = 0; j < vD.size(); ++j)
			{
				if (vD.at(j).rowid == rowid)		// szerepel-e a már meglaláltak között a rowid?
					break;
			}
			if (j == vD.size())					// csak akkor teszi el, ha nem találta ismétlõdõnek
				vD.push_back(desc);				// 
			else
				++m_cntRepeated;
		}
		else
			vD.push_back(desc);

  		if (vD.size() == m_maxDesc)
		{
			str.Format(L"%d leszármazottnál feladom!", m_maxDesc);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
			m_givup = vD.size();
			break;
		}
	}
	wndP.DestroyWindow();
}
*/