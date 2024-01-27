// ascendantsTable.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendantsLinear_Table.h"
#include "ascendants.h"
#include "ascendantsEnum.h"
#include <commctrl.h >
#include <windows.h>
#include "utilities.h"
#include "ascendantsPaint.h"


// CAscendantsLinearTable dialog

#define WM_RELOAD			WM_USER + 0x1FF9
IMPLEMENT_DYNAMIC(CAscendantsLinearTable, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsLinearTable::CAscendantsLinearTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS_LINEAR, pParent)
{
	m_recP = new CSqliteDBRecordSet;
	m_recM = new CSqliteDBRecordSet;
	m_log = false;


	

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsLinearTable::~CAscendantsLinearTable()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_SearchCtrl);
	DDX_Control(pDX, IDC_RICHEDIT, m_RichEditCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendantsLinearTable, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TAB, &CAscendantsLinearTable::OnCustomdrawList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CAscendantsLinearTable::OnSelchangeTab)
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC_NEXT, &CAscendantsLinearTable::OnClickedStaticNext)

	ON_COMMAND(ID_HTMLLISTA_FIX, &CAscendantsLinearTable::OnHtmlListaFix)
	ON_COMMAND(ID_HTMLLISTA_PRINTABLE, &CAscendantsLinearTable::OnHtmlListaPrintable)
	ON_COMMAND(ID_PIRAMIS, &CAscendantsLinearTable::OnPiramis)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FULL, &CAscendantsLinearTable::OnCustomdrawList)

	ON_MESSAGE(WM_SET_ROW_COLOR, OnSetRowColor)
	ON_MESSAGE(WM_RELOAD, OnReloadListCtrl )
	ON_COMMAND(ID_INDENTED_ASCENDANTS, &CAscendantsLinearTable::OnIndentedAscendants)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsLinearTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_brRed.CreateSolidBrush(RED);
	m_brYellow.CreateSolidBrush(YELLOW);

	if (m_log)
	{
		fs_vGKR2 = theApp.openTextFile(&flvGKR2, L"vGKR2", L"w+");  // log fájl
		fs_vSp = theApp.openTextFile(&flvSp, L"vSp", L"w+");  // log fájl
		fs_vFC = theApp.openTextFile(&flvFC, L"vCF", L"w+");  // log fájl
		fs_vSib = theApp.openTextFile(&flvSib, L"vSib", L"w+");  // log fájl		
	}

	create_vGKR2();		// vGKR kibõvítése ixF és ixF -el és togle-al vSp látrahozása


	m_RichEditCtrl.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	m_titleF.Format(L"%s teljes felmenõi listája", m_name);
	m_titleU.Format(L"%s felmenõi listája ismétlések kihagyásával", m_name);



	m_aF.Create(IDD_ASCENDANTS_FULL, this);
	m_aU.Create(IDD_ASCENDANTS_UNIQUE, this);

	int i = 0;

	TCITEM tcItem;
	tcItem = { 0 };
	tcItem.mask = TCIF_TEXT | TCIF_PARAM | TCIF_STATE;
	tcItem.pszText = L"Teljes lista";
	tcItem.lParam = 0;
	tcItem.dwStateMask = TCIS_HIGHLIGHTED;
	tcItem.dwState = TCIS_HIGHLIGHTED;

	m_TabCtrl.InsertItem(i, &tcItem);
	++i;
	m_TabCtrl.InsertItem(i, L"Ismétlések nélkül");

	attachDialogs();

	SetWindowTextW(theApp.m_caption);
	ShowWindow(SW_MAXIMIZE);

	m_RichEditCtrl.SetWindowTextW(m_titleF);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);
	m_RichEditCtrl.SetBackgroundColor( false, RGB(240, 240, 240 ));


	CMenu* menu;
	menu = GetMenu();
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);

	fullTable();
	uniqueTable();

	m_TabCtrl.SetCurSel(FULL);
	m_ListCtrl = &(m_aF.m_ListCtrlF);
	m_aF.ShowWindow(SW_SHOW);
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsLinearTable::attachDialogs()
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
void CAscendantsLinearTable::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
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
	case FULL_TAB:
//		m_ListCtrl = &(m_aF.m_ListCtrlF);
		m_aF.ShowWindow(SW_SHOW);
		m_aU.ShowWindow(SW_HIDE);

		m_title = m_titleF;
/*
		m_TabCtrl.GetItem(0, &curItem);
		curItem.mask = TCIF_STATE;
		curItem.dwState = TCIS_BUTTONPRESSED;
		curItem.dwStateMask = TCIS_BUTTONPRESSED;
		m_TabCtrl.SetItem(0, &curItem);

		m_TabCtrl.GetItem(0, &curItem);
		curItem.mask = TCIF_STATE;
		curItem.dwState = TCIS_BUTTONPRESSED;
		curItem.dwStateMask = TCIS_BUTTONPRESSED; 
		m_TabCtrl.SetItem(0, &curItem);
*/

		break;
	case UNIQUE_TAB:
//		m_ListCtrl = &(m_aU.m_ListCtrlU);

		m_aU.ShowWindow(SW_SHOW);
		m_aF.ShowWindow(SW_HIDE);

		m_title = m_titleU;
		break;
	}
	
/*
	m_TabCtrl.GetItem(nItem, &curItem);
	curItem.mask = TCIF_STATE;
	curItem.dwState = TCIS_BUTTONPRESSED;
	curItem.dwStateMask = TCIS_BUTTONPRESSED;
	m_TabCtrl.SetItem(nItem, &curItem);
*/

	

	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.Clear();

	m_RichEditCtrl.SetWindowTextW(m_title);
	m_RichEditCtrl.SetSel(0, -1);
	m_RichEditCtrl.SetSelectionCharFormat(cf);

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	attachDialogs();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::fullTable()
{
	int nItem = 0;
	CString name;

	CProgressWnd wndP(NULL, L"Lineáris teljes lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR->size());

	m_familyNamePrev.Empty();
	m_ListCtrl = &(m_aF.m_ListCtrlF);
	for (int i = 0; i < vGKR->size(); ++i)
	{
		getAscendant(i, FULL );

	

		str.Format(L"%d", vGKR->at(i).toggle);		// hidden: generációk háttér színezésére
		m_ListCtrl->InsertItem(nItem, str);


		m_ListCtrl->SetItemText(nItem, F_LINENUMBER, vGKR->at(i).linenumber);	// hidden

		str.Format(L"%d", vGKR->at(i).sex_id);
		m_ListCtrl->SetItemText(nItem, F_SEX, str);	// hidden

		str.Format(L"%d", i );
		m_ListCtrl->SetItemText(nItem, F_CNT, str);

		m_ListCtrl->SetItemText(nItem, F_ISM, col.rep);
		m_ListCtrl->SetItemText(nItem, F_IDF, col.idF);
		m_ListCtrl->SetItemText(nItem, F_IDC, col.idC);
		m_ListCtrl->SetItemText(nItem, F_ID, col.idAL);

		name = col.name;
		name += col.description;
		m_ListCtrl->SetItemText(nItem, F_ASCENDANT, name );

		m_ListCtrl->SetItemText(nItem, F_NAME, col.name );
		m_ListCtrl->SetItemText(nItem, F_DESCRIPTION, col.description );
		m_ListCtrl->SetItemText(nItem, F_COLOR, L"");
		++nItem;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::uniqueTable()
{
	int nItem = 0;
	CString name;
	CString familyName;

	correctReferences();
	
	CProgressWnd wndP(NULL, L"Lineáris ritkított lista készítése...");
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR->size());

	m_ListCtrl = &(m_aU.m_ListCtrlU);

	for (int i = 0; i < vGKR->size(); ++i)
	{
		if (vGKR->at(i).cnt > 1) continue;		// kihagyja az ismétléseket
		/*
		familyName = vGKR->at(i).familyName;
		if ( familyName != m_familyNamePrev)
		{
			m_ListCtrl->InsertItem(nItem, L"");
			m_ListCtrl->SetItemText(nItem, U_MOREC, L"");
			m_ListCtrl->SetItemText(nItem, U_CNT, L"");
			m_ListCtrl->SetItemText(nItem, U_ISM, L"");
			m_ListCtrl->SetItemText(nItem, U_SIBLINGS, L"");
			m_ListCtrl->SetItemText(nItem, U_SPOUSES, L"");
			m_ListCtrl->SetItemText(nItem, U_IDM, L"");
			m_ListCtrl->SetItemText(nItem, U_IDF, L"");
			m_ListCtrl->SetItemText(nItem, U_IDC, L"");
			m_ListCtrl->SetItemText(nItem, U_ID, L"");
			m_ListCtrl->SetItemText(nItem, U_ASCENDANT, vGKR->at(i).familyName);
			m_ListCtrl->SetItemText(nItem, U_NAME, L"");
			m_ListCtrl->SetItemText(nItem, U_DESCRIPTION, L"");
			m_ListCtrl->SetItemText(nItem, U_COLOR, L"4");
			++nItem;
			m_familyNamePrev = familyName;
		}
		*/
		getAscendant(i, UNIQUE );

		str.Format(L"%d", vGKR->at(i).toggle);
		m_ListCtrl->InsertItem(nItem, str);
		
		str.Format(L"%d", vGKR->at(i).moreC);
		m_ListCtrl->SetItemText(nItem, U_MOREC, str);

		m_ListCtrl->SetItemText(nItem, U_LINENUMBER, vGKR->at(i).linenumber);

		str.Format(L"%d", nItem);
		m_ListCtrl->SetItemText(nItem, U_CNT, str);

		m_ListCtrl->SetItemText(nItem, U_ISM, col.rep);
		m_ListCtrl->SetItemText(nItem, U_SIBLINGS, col.siblings);
		m_ListCtrl->SetItemText(nItem, U_SPOUSES, col.spouses);
		m_ListCtrl->SetItemText(nItem, U_IDM, col.idM);
		m_ListCtrl->SetItemText(nItem, U_IDF, col.idF);
		m_ListCtrl->SetItemText(nItem, U_IDC, col.children);
		m_ListCtrl->SetItemText(nItem, U_ID, col.idAL);

		name = col.name;
		name += L" ";
		name += col.description;
		name.Trim();
		m_ListCtrl->SetItemText(nItem, U_ASCENDANT, name);

		m_ListCtrl->SetItemText(nItem, U_NAME, col.name);
		m_ListCtrl->SetItemText(nItem, U_DESCRIPTION, col.description);
		m_ListCtrl->SetItemText(nItem, U_COLOR, L"");
		++nItem;

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;

	}
/*
	m_ListCtrl->SetColumnWidth(U_SIBLINGS, -1);
	m_ListCtrl->SetColumnWidth(U_SPOUSES, -1);
	m_ListCtrl->SetColumnWidth(U_IDM, -1);
	m_ListCtrl->SetColumnWidth(U_IDC, -1);
	m_ListCtrl->SetColumnWidth(U_IDF, -1);
	m_ListCtrl->SetColumnWidth(U_ID, -1);
*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR CAscendantsLinearTable::get_gABC(int g)
{
	TCHAR gABC;
	int ix = g / 26;		  // ix = 0, ha g kisebb mint 26, = 1 Ha nagyobb
	gABC = TCHAR('A') + ix * 6 + g;   // 6 : a 'Z' és 'a' közötti karakterek száma,
	return gABC;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::getAscendant(int i, int which )
{
	GKR gkr = vGKR->at(i);


	CString color;
	bool lineY = false;
	int kPrev = 0;
	int g = gkr.g;
	_int64 k = gkr.k;
	_int64 kFirstG;
	bool gFlag;
	bool moreC;
	int z;
	int ix;
	int ixC;
	int ixSib;
	int ixSp;
	int pos;

	CString gcC;

	col.cnt = 0;
	col.rep.Empty();
	col.idA.Empty();
	col.idC.Empty();
	col.idF.Empty();
	col.idM.Empty();
	col.idSp.Empty();
	col.idSib.Empty();
	col.spouses.Empty();
	col.siblings.Empty();
	col.children.Empty();

	moreC = false;


	
	if (i != 0)
	{
		// elsõ gyerek
		ixC = gkr.ixC;
		col.children.Format(L"%c-%-d", vGKR->at(ixC).gABC, vGKR->at(ixC).gcnt);
		col.children += L",";

		if (which == UNIQUE || which == UNIQUE_P)
		{
			// testvérek és gyerekek listája
			for (int j = 0; j < vSib.size(); ++j)
			{
				// gyerekek
				if (vSib.at(j).ixF == i && vGKR->at(vSib.at(j).ix).cnt < 2)		// testvér apja, aki nem ismétlõdõ
				{
					ix = vSib.at(j).ix;
					gcC.Format(L"%c-%-d", vGKR->at(ix).gABC, vGKR->at(ix).gcnt);  // van gyerek
					
					if ((pos = col.children.Find(gcC)) == -1)
					{
						col.children += gcC;		// ha még nincs ez a gyerek, akkor hozzáadja
						col.children += L",";
						moreC = true;
						vGKR->at(i).moreC = 1;

					}
				}

				// testvérek
				ixSib = vSib.at(j).ixSib;
				if (vSib.at(j).ix == i && vGKR->at(ixSib).cnt < 2)  // õ maga a leszármazott gyereke, testvére nem ismétlõdõ
				{
					str.Format(L"%c-%-d", vGKR->at(ixSib).gABC, vGKR->at(ixSib).gcnt);
					col.siblings += str;
					col.siblings += L",";
				}


			}
			col.siblings = dropLastCharacter(col.siblings );
		}
		col.children = dropLastCharacter(col.children );
	}


	// további házastársak listája
	if (gkr.numOfSp > 1 && (which == UNIQUE || which == UNIQUE_P))
	{
		for (int j = 0; j < vSp.size(); ++j)
		{
			if (vSp.at(j).ix == i && vGKR->at(vSp.at(j).ixSp).cnt < 2)		// a hozzá tartozó házaspárok keresése)	
			{
				ixSp = vSp.at(j).ixSp;
				str.Format(L"%c-%-d", vGKR->at(ixSp).gABC, vGKR->at(ixSp).gcnt);
				if ((pos = col.spouses.Find(str)) == -1)
				{
					col.spouses += str;
					col.spouses += L",";
				}
			}

			if (vSp.at(j).ixSp == i && vGKR->at(i).cnt < 2)		// a hozzá tartozó házaspárok keresése
			{
				ixSp = vSp.at(j).ix;
				str.Format(L"%c-%-d", vGKR->at(ixSp).gABC, vGKR->at(ixSp).gcnt);
				
				if ((pos = col.spouses.Find(str)) == -1)
				{
					col.spouses += str;
					col.spouses += L",";
				}
			}
		}
		col.spouses = dropLastCharacter(col.spouses );
	}
	else
	{
		if (i && gkr.numOfSp == 0)
			col.spouses = L"n/a";
	}


	getPeopleString(i);  // meghatározza col.ascString-et és col.ascStringL-t


	// a felmenõ elõfordulásának száma
	if (gkr.cnt)
		col.rep.Format(L"%d/%-d", gkr.cnt, gkr.cntMax);

	col.sex = vGKR->at(i).sex_id;


	for (int j = 0; j < vSib.size(); ++j)
	{
		if (vSib.at(j).ix == i)
		{
			col.idSib.Format(L"%d-%-d", vGKR->at(vSib.at(j).ixSib).gABC, vGKR->at(vSib.at(j).ixSib).gcnt);
			vSib.at(j).listed = true;
			break;		// csak az elsõ testvérét teszi bele
		}
	}


	// apja
	if (!i || vGKR->at(gkr.ixF).g)
	{
		col.idF.Format(L"%c-%-I64d", vGKR->at(gkr.ixF).gABC, vGKR->at(gkr.ixF).gcnt); // apa g-cnt elkészítése
	}

	// anyja
	col.idM = L"";
	if (which == UNIQUE || which == UNIQUE_P)
	{
		if (!i || vGKR->at(gkr.ixM).g)
		{
			if (vGKR->at(gkr.ixF).gcnt + 1 != vGKR->at(gkr.ixM).gcnt)   ///nme a stadard anya
			{
				col.idM.Format(L"%c-%-I64d", vGKR->at(gkr.ixM).gABC, vGKR->at(gkr.ixM).gcnt); // apa g-cnt elkészítése
			}
		}
	}

	// felmenõ azonosítója
	col.idAL.Format(L"%c-%-I64d", gkr.gABC, gkr.gcnt);
	col.idA.Format(L"<b>%s</b>", col.idAL);

	if (gkr.g != m_gPrev)
		m_gFlag = !m_gFlag;

	if (gkr.g && m_gFlag)
		col.idA.Format(L"<font color=\"blue\">%s</font>", (CString)col.idA);
	else
		col.idA.Format(L"<font color=\"red\">%s</font>", (CString)col.idA);


	// házastársa. Csak akkor, ha nem a k, k+1 házastár

	if (gkr.g == 9 && gkr.cnt == 94)
		z = 1;
	if (i == 117)
		z = 1;
	if (gkr.numOfSp)
	{
		if (i && i < vGKR->size() - 1)
		{
			if ((gkr.sex_id == 1) && ((gkr.k + 1) != vGKR->at(gkr.ixSp).k) || (gkr.sex_id == 2) && (gkr.k != vGKR->at(gkr.ixSp).k + 1))
			{
				col.idSp.Format(L"<font color=\"blue\">%c-%-d</font>", vGKR->at(gkr.ixSp).gABC, vGKR->at(gkr.ixSp).gcnt);  // házastárs g-cnt elkészítése
			}
		}
	}
	else
		col.idSp = L"n/a   ";

	// gyereke
	if (gkr.ixC > -1)
	{
		col.idC.Format(L"%c-%-I64d", vGKR->at(gkr.ixC).gABC, vGKR->at(gkr.ixC).gcnt); // gyerek g-cnt elkészítése
	}


	m_gPrev = gkr.g;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::getPeopleString(int i)
{
	CString people;
	CString spouse_id;
	CString rowid = vGKR->at(i).rowid;

	CString sp(L" ");
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return;

	
	CString	str;
	CString str1;
	CString generation;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString posterior;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	int		parentIndex;
	CString comment;
	CString arm;
	CString peer;
	CString csalad;
	
	CString name;
	CString desc;

	generation = m_recP->GetFieldString(DBP_GENERATION);
	title = m_recP->GetFieldString(DBP_TITLE);
	titolo = m_recP->GetFieldString(DBP_TITOLO);
	last_name = m_recP->GetFieldString(DBP_LAST_NAME);
	first_name = m_recP->GetFieldString(DBP_FIRST_NAME);
	posterior = m_recP->GetFieldString(DBP_POSTERIOR);
	birth_place = m_recP->GetFieldString(DBP_BIRTH_PLACE);
	birth_date = m_recP->GetFieldString(DBP_BIRTH_DATE);
	death_place = m_recP->GetFieldString(DBP_DEATH_PLACE);
	death_date = m_recP->GetFieldString(DBP_DEATH_DATE);
	father_id = m_recP->GetFieldString(DBP_FATHER_ID);
	mother_id = m_recP->GetFieldString(DBP_MOTHER_ID);
	parentIndex = _wtoi(m_recP->GetFieldString(DBP_PARENTINDEX));
	comment = m_recP->GetFieldString(DBP_COMMENT);
	arm = m_recP->GetFieldString(DBP_ARM);
	csalad = m_recP->GetFieldString(DBP_CSALAD);
	peer = m_recP->GetFieldString(DBP_PEER);

	name.Empty();
	if (m_peerPrint && !peer.IsEmpty() )
	{
		name = peer;
		name += L" ";

	}
	if (last_name != L"N;" && !titolo.IsEmpty()) { name += titolo; name += L" "; }
	name += last_name;
	if (!last_name.IsEmpty()) { name += L" "; }


	if (!first_name.IsEmpty()) { name += first_name; str += L" "; }
	if (!posterior.IsEmpty())
	{
		name += L" ";  name += posterior;
	}
	name.Trim();
	

	desc.Empty();
	if (!birth_place.IsEmpty() || !birth_date.IsEmpty())
	{
		desc += L" *";
		if (!birth_place.IsEmpty()) desc += birth_place;
		if (!birth_date.IsEmpty())
		{
			if (!birth_place.IsEmpty()) desc += sp;
			desc += birth_date;
		}
	}

	if (!death_place.IsEmpty() || !death_date.IsEmpty())
	{
		desc += L" +";
		if (!death_place.IsEmpty()) desc += death_place;
		if (!death_date.IsEmpty())
		{
			if (!death_place.IsEmpty()) desc += sp;
			desc += death_date;
		}
	}

	if (!comment.IsEmpty()) { desc += " "; desc += comment; }

	if (i + 1 < vGKR->size())
	{
		if (vGKR->at(i + 1).k == vGKR->at(i).k + 1)
		{
			spouse_id = vGKR->at(i + 1).rowid;
			desc += L" ";
			desc += getMarriage(rowid, spouse_id);
			desc.TrimRight();
		}
	}
	if (!arm.IsEmpty())
	{
		desc += L" [";
		desc += arm;
		desc += L"]";
	}
	if (!csalad.IsEmpty())
	{
		desc += L" [";
		desc += csalad;
		desc += L" család]";
	}
	col.name = name;
	col.description = desc;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendantsLinearTable::getMarriage(CString rowid, CString rowidS)
{
	if (rowidS.IsEmpty()) return L"";

	CString marriage;
	CString place;
	CString date;

	m_command.Format(L"SELECT place, date FROM marriages WHERE husband_id ='%s' AND wife_id= '%s'", rowid, rowidS);
	if (!queryM(m_command)) return L"";

	if (!m_recM->RecordsCount()) return marriage;

	place = m_recM->GetFieldString(0);
	date = m_recM->GetFieldString(1);
	if (!place.IsEmpty() && !date.IsEmpty())
		marriage.Format(L"=%s %s", place, date);
	else if (!place.IsEmpty())
		marriage.Format(L"=%s", place);
	else if (!date.IsEmpty())
		marriage.Format(L"=%s", date);

	return marriage;
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsLinearTable::queryP(CString command)
{
	if (m_recP->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recP->GetLastError());
		theApp.message(L"Query", str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsLinearTable::queryM(CString command)
{
	if (m_recM->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recM->GetLastError());
		theApp.message(L"Query", str);
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vGKR
// {
// CString	r;					rowid
// CString	name;				név
// CString	birth;				születés
// CString	death;				halál
// int		sex_id				sex
// int		g;					generáció
// _int64	k;					Kekule szám
// int		gcnt				generáción belüli sorszám
// int		cnt;				a rowid ismétlõdésének sorszámaszáma  ( 0 ha nincs ismétlõdés  
// int		cntMax			ismétlõdések száma
// int		ixC					gyerek indexe a vGKR-ben	
// int		ixF					apa indexe a vGKR-ben
//	};
void CAscendantsLinearTable::create_vGKR2()
{
	if (m_log)
		fs_vGKR3 = theApp.openTextFile(&flvGKR3, L"vGKR3", L"w+");  // log fájl
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;

	int	gPrev = 0;
	_int64	k = 1;		// kekülé szám
	bool	toggle = false;
	int x;
	CProgressWnd wndP3(NULL, L"Apa és elsõ gyerek indexeinek keresése...");
	wndP3.GoModal();
	wndP3.SetRange(0, vGKR->size());
	wndP3.SetStep(1);

	vGKR->at(0).ixF = 1;   /// 1 kellene!!!
	// minden felmenõbe beírjuk elsõ gyerekét és apját
	for (int i = 0; i < vGKR->size(); ++i)
	{
		gkr = vGKR->at(i);
		k = gkr.k / 2;					// gyerek Kekule száma
		for (int j = 0; j < i; ++j)		// megkeressük az elsõ gyerekét, és betesszük annak indexét 
		{
			if (vGKR->at(j).k == k)
			{
				vGKR->at(i).ixC = j;
				break;
			}
		}

		k = gkr.k * 2;								// apa Kekülé száma
		for (int j = i + 1; j < vGKR->size(); ++j)	// megkeressük az apját
		{
			if (vGKR->at(j).k == k)
			{
				vGKR->at(i).ixF = j;						// beírjuk az apa indexét
				vGKR->at(i).ixM = vGKR->at(j).ixSp;		// beírjuk az anya indexét	
			}
		}

		if (gPrev != vGKR->at(i).g)
				toggle = !toggle;
		vGKR->at(i).toggle = toggle;
		vGKR->at(i).moreC = 0;

		gPrev = vGKR->at(i).g;
		wndP3.StepIt();
		wndP3.PeekAndPump();
		if (wndP3.Cancelled()) break;
	}
	wndP3.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::correctReferences()
{
	int x;
	int z;


	// Az ismételt, tehát törlendõ felmenõkre való hivatkozást ( apa, házastársa, gyerek ) átirányítjuk a megmaradó bejegyzésre
//	CProgressWnd wndP(NULL, L"Ismételt felmenõkre hivatkozások korrigálása...");
	CProgressWnd wndP(NULL, L"Törlendõ felmenõkre történõ hivatkozások korrogálása...");
	wndP.SetRange(0, vGKR->size() *3 );
	wndP.SetStep(1);

	bool volt;
	for (int i = 1; i < vGKR->size() - 1; ++i)
	{
		if (vGKR->at(i).cnt == 1)				// elsõ elõfordulás, ami megmarad
		{
			for (int j = i; j < vGKR->size(); ++j)
			{
				if (vGKR->at(j).rowid == vGKR->at(i).rowid)		// j a következõ elõfordulás, amit törölni fogunk, ezért j-t töröljük, ezért
				{												// j-re történõ hivatkozásokat keresi, hogy azokat i-re változtassa
					volt = false;
					for (int k = 0; k < vGKR->size(); ++k)		// törlendõ apákra, házastársra, gyerekre történõ hivatkozás korrigálása
					{
						if (vGKR->at(k).ixF == j)
						{
							vGKR->at(k).ixF = i;
							volt = true;
						}
						if (vGKR->at(k).ixC == j)
						{
							vGKR->at(k).ixC = i;
							volt = true;

						}
						if (vGKR->at(k).ixSp == j)
						{
							vGKR->at(k).ixSp = i;
							++vGKR->at(k).numOfSp;
							volt = true;
						}
						//						if (volt)
						//							break;
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}


	SP spouse;
	CString rowid1;
	CString rowid2;
	CString rowidSp1;
	CString rowidSp2;
	int ip1;
	int ip2;
	int ix;
	_int64 k;

	vSp.clear();
	for (int i = 1; i < vGKR->size() - 1; ++i)
	{
		if (vGKR->at(i).cntMax < 2) continue;				//  Ha csak egyszer fordul elõ, akkor annak nem lehet több házastársa
		if (vGKR->at(i).cnt > 1) continue;					// csak az elsõ elõfordulást vizsgálja
		//		if (vGKR.at(vGKR.at(i).ixSp).ixSp != i) continue;		// a házastársa nem õrá mutat, vagy nincs is házastárs tovább
		if (vGKR->at(i).ixSp == 0) continue;					// nincs házastársa


		if (i == 11)
			z = 1;
		rowid1 = vGKR->at(i).rowid;							// ember rowid-ja
		if (rowid1 == L"33")
			z = 2;

		rowidSp1 = vGKR->at(vGKR->at(i).ixSp).rowid;			// házastárs rowid-je
		spouse.ix = i;
		spouse.ixSp = vGKR->at(i).ixSp;
		//		vSp.push_back(spouse);

				// megkeresi az ember (rowid1) további elõfordulásait
		for (int j = i + 1; j < vGKR->size() - 1; ++j)
		{
			rowid2 = vGKR->at(j).rowid;
			if (rowid2 == L"82335")
				ix = 2;

			if (rowid2 == rowid1)						// ez ember újabb elõfordulása
			{
				k = vGKR->at(j).k;
				if (k % 2 == 0)							// páros, tehát férj
				{
					ip2 = j + 1;						// házastársa utána van, indexe a vGKR-ben
					if (vGKR->at(ip2).k != k + 1)		// nem az õ felesége
						continue;
				}
				else
				{
					ip2 = j - 1;						// páratlan, tehát feleség, házastársa elõtte van
					if (vGKR->at(ip2).k != k - 1)		// nem a férje
						continue;
				}

				rowidSp2 = vGKR->at(ip2).rowid;
				if (rowidSp1 != rowidSp2)				// más házastárs!
				{
					spouse.ix = i;
					spouse.ixSp = ip2;
					vSp.push_back(spouse);
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	int n;

	GKR gkri;
	GKR gkrj;
	SIBLINGS sib;
	bool first;

	vSib.clear();

	ip1 = vSp.size();
	for (int i = 1; i < vGKR->size() - 1; ++i)
	{
		gkri = vGKR->at(i);
		if (i == 1091)
			z = 1;
		for (int j = i + 1; j < vGKR->size(); ++j)
		{
			gkrj = vGKR->at(j);
			if (gkrj.cnt < 2 && gkrj.ixF != 0)		// nem ugyanaz, nem ismételt és van apja
			{
				if ((vGKR->at(gkri.ixF).rowid == vGKR->at(gkrj.ixF).rowid) && gkri.rowid != gkrj.rowid)	// testvérek, ha az apák azonosak, és nem ismétlõdõ 
				{
					sib.ix = i;					// testvér vica-versa
					sib.ixSib = j;
					sib.ixF = gkri.ixF;
					sib.listed = false;
					vSib.push_back(sib);

					sib.ix = j;
					sib.ixSib = i;
					sib.ixF = gkrj.ixF;
					sib.listed = false;
					vSib.push_back(sib);
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

	if (m_log)
	{
		printvGKR2();
		fclose(flvGKR2);
		theApp.notepad(fs_vGKR2, L"");

		printvSib();
		fclose(flvSib);
		theApp.notepad(fs_vSib, L"");

		printvSp();
		fclose(flvSp);
		theApp.notepad(fs_vSp, L"");

		printvFC();
		fclose(flvFC);
		theApp.notepad(fs_vFC, L"");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printvSib()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvSib, L"%5s %5s %5s\n", L"ix", L"ixSib", L"ixF");
	for (int i = 0; i < vSib.size(); ++i)
	{
		fwprintf(flvSib, L"%5d.%5d %5d %5d\n", i, vSib.at(i).ix, vSib.at(i).ixSib, vSib.at(i).ixF);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printvSp()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvSp, L"%6s %5s %5s\n", L" ", L"ix", L"ixSp");
	for (int i = 0; i < vSp.size(); ++i)
	{
		fwprintf(flvSp, L"%5d.%5d %5d\n", i, vSp.at(i).ix, vSp.at(i).ixSp);
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printvGKR2()
{

	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvGKR2, L"%10s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s %10s %10s|\
				   %7s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s %10s\n", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"maxc", L"cnt", L"ixC", L"ixF", L"ixSp", L"numOfSp", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"maxc", L"cnt", L"ixC", L"ixF", L"ixSp");
	fwprintf(flvGKR2, L"-------------------------------------------------------------------------------------------\n");

	gPrev = vGKR->at(0).g;
	gkr = vGKR->at(0);
	str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %4d %10d %4d %10d %10d\n", \
		0, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.cntMax, gkr.gcnt, gkr.k, gkr.cnt, gkr.ixC, gkr.numOfSp);
	fwprintf(flvGKR2, str);

	for (int i = 1; i < vGKR->size(); ++i)
	{
		gkr = vGKR->at(i);

		j = gkr.ixC;			// gyereke
		if (j < 0 || j > vGKR->size())
		{
			x = 1;
			return;
		}
		gkrC = vGKR->at(j);

		j = gkr.ixF;
		if (j < 0 || j > vGKR->size())
		{
			x = 1;
			return;
		}
		gkrF = vGKR->at(j);

		if (gkr.g != gPrev)
			fwprintf(flvGKR2, L"-------------------------------------------------------------------------------------------\n");
		str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d %10d %10d\
					 %5d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d %10d\n", \
			i, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.gcnt, gkr.k, gkr.cntMax, gkr.cnt, gkr.ixC, gkr.ixF, gkr.ixSp, gkr.numOfSp, \
			j, gkrC.name, gkr.sex_id, gkrC.rowid, gkrC.g, gkrC.gcnt, gkrC.k, gkr.cntMax, gkrC.cnt, gkrC.ixC, gkr.ixF, gkr.ixSp);
		fwprintf(flvGKR2, str);
		gPrev = gkr.g;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::printvFC()
{
	fwprintf(flvFC, L"%5s %5s %5s %3s %6s %6s %-30s %s\n", L"i", L"ixF", L"ixC", L"cnt", L"rowidF", L"rowidC", L"apa", L"gyerek");
	for (int i = 0; i < vFC.size(); ++i)
	{
		if (vFC.at(i).ixF == -1)
			fwprintf(flvFC, L"%4d. %5d %5d %3d %6s %6s\n", i, vFC.at(i).ixF, vFC.at(i).ixC, vFC.at(i).cnt, vFC.at(i).rowidF, vFC.at(i).rowidC);
		else
			fwprintf(flvFC, L"%4d. %5d %5d %3d %6s %6s %-30s %s\n", i, vFC.at(i).ixF, vFC.at(i).ixC, vFC.at(i).cnt, vFC.at(i).rowidF, vFC.at(i).rowidC, vGKR->at(vFC.at(i).ixF).name, vGKR->at(vFC.at(i).ixC).name);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsLinearTable::PreTranslateMessage(MSG* pMsg)
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
void CAscendantsLinearTable::OnClickedStaticNext()
{
	int nItem = m_ListCtrl->GetNextItem(-1, LVNI_SELECTED);
	keress(nItem + 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::keress(int start)
{
	CString	search;
	m_SearchCtrl.GetWindowTextW(search);
	int curSel = m_TabCtrl.GetCurSel();
	
	if (curSel == FULL_TAB)
	{
		theApp.keress(search, m_ListCtrl, F_ASCENDANT, start, false);
	}
	else
		theApp.keress(search, m_ListCtrl, U_ASCENDANT, start, false);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW		lvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	*pResult = CDRF_DODEFAULT;

	switch (lvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		lvcd->clrTextBk = lvcd->nmcd.lItemlParam;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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
		case 0:
//			cbr = &m_brRed;
			txtColor = RGB(255, 0, 0, );
			pC = L" Teljes táblázat";
			break;

		case 1:
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CAscendantsLinearTable::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CAscendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam)//wparam: F vagy Q tábla, lparam: az CLIstCtrl*
{
	if (wParam == FULL)
	{
		m_ListCtrl = &(m_aF.m_ListCtrlF);
		fullTable();
	}
	else
	{
		m_ListCtrl = &(m_aU.m_ListCtrlU);
		uniqueTable();
	}
	return TRUE;
}
LRESULT CAscendantsLinearTable::OnSetRowColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor = (CELLCOLOR*)lParam;
	cColor->bg	= YELLOW;
	cColor->tx	= BLACK;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// M E N U F U N K C I Ó K  ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnHtmlListaPrintable()
{
	if (m_TabCtrl.GetCurSel() == FULL)
	{
		m_ListCtrl = &(m_aF.m_ListCtrlF);
		createHtmlFile(FULL_P);
	}
	else
	{
		m_ListCtrl = &(m_aU.m_ListCtrlU);
		createHtmlFile(UNIQUE_P);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnHtmlListaFix()
{
	if (m_TabCtrl.GetCurSel() == FULL)
	{
		m_ListCtrl = &(m_aF.m_ListCtrlF);
		createHtmlFile(FULL);
	}
	else
	{
		m_ListCtrl = &(m_aU.m_ListCtrlU);
		createHtmlFile(UNIQUE);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnIndentedAscendants()
{
	ShellExecute(NULL, L"open", m_indentedFile, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsLinearTable::OnPiramis()
{
	CAscendantsPaint dlg;
	dlg.vGKR = vGKR;
	dlg.m_ascendantsPath = m_ascendantsPath;
	if (m_TabCtrl.GetCurSel() == FULL)
		dlg.which = FULL;
	else
		dlg.which = UNIQUE;

	dlg.DoModal();
}

