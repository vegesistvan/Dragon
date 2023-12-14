// conractEntriesErrors.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "unitedEntries.h"
#include "unitedEntriesErrors.h"
#include "Relatives.h"
#include "compareEntries.h"
#include "compareAscendants.h"

IMPLEMENT_DYNAMIC(CUnitedEntriesErrors, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesErrors::CUnitedEntriesErrors(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITED_ENTRIES_ERRORS, pParent)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUnitedEntriesErrors::~CUnitedEntriesErrors()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ERROR, m_ListCtrlE);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUnitedEntriesErrors, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ERROR, &CUnitedEntriesErrors::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ERROR, &CUnitedEntriesErrors::OnDblclkList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_NOTEPAD, &CUnitedEntriesErrors::OnHtmlNotepad)
	ON_COMMAND(ID_COMPARE_2PEOPLE, &CUnitedEntriesErrors::OnCompare2People)
	ON_COMMAND(ID_COMPARE_ASCENDANTS, &CUnitedEntriesErrors::OnCompareAscendants)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUnitedEntriesErrors::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlE.KeepSortOrder(TRUE);
	m_ListCtrlE.SetExtendedStyle(m_ListCtrlE.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrlE.InsertColumn(U_DUMMY, L"", LVCFMT_RIGHT, 35, -1, COL_HIDDEN);
	m_ListCtrlE.InsertColumn(U_LOOP, L"loop", LVCFMT_RIGHT, 35, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_GROUP, L"gr", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_MATCH, L"m#", LVCFMT_RIGHT, 30, -1, COL_HIDDEN);
	m_ListCtrlE.InsertColumn(U_STATUS, L"st", LVCFMT_RIGHT, 30, -1, COL_NUM);

	m_ListCtrlE.InsertColumn(U_UNITED, L"u", LVCFMT_LEFT, 20, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_SOURCE, L"s", LVCFMT_RIGHT, 20, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_INDEX, L"i", LVCFMT_RIGHT, 20, -1, COL_NUM);

	m_ListCtrlE.InsertColumn(U_LINE, L"line#", LVCFMT_RIGHT, 50, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_ROWID, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_NAME, L"ember", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_SEX_ID, L"sex", LVCFMT_LEFT, 30, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_BIRTH, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_DEATH, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlE.InsertColumn(U_LINEF, L"line", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlE.InsertColumn(U_ROWIDF, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_FATHER, L"apa", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_BIRTHF, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_DEATHF, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlE.InsertColumn(U_LINEM, L"", LVCFMT_RIGHT, 50, -1, COL_HIDDEN);
	m_ListCtrlE.InsertColumn(U_ROWIDM, L"rowid", LVCFMT_RIGHT, 60, -1, COL_NUM);
	m_ListCtrlE.InsertColumn(U_MOTHER, L"anya", LVCFMT_LEFT, 150, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_BIRTHM, L"birth", LVCFMT_LEFT, 70, -1, COL_TEXT);
	m_ListCtrlE.InsertColumn(U_DEATHM, L"death", LVCFMT_LEFT, 70, -1, COL_TEXT);

	m_ListCtrlE.InsertColumn(U_SPOUSES, L"házastársak", LVCFMT_LEFT, 700, -1, COL_TEXT);

	inputFile();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int iData;
	UINT mask;

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

		// a különbözõ adatok jelzáse sárgával
		mask = 1 << nCol;
		iData = _wtoi( m_ListCtrlE.GetItemText(nItem, 0) );
		if (iData & mask)
		{
			pLVCD->clrTextBk = RGB(255, 255, 0);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitedEntriesErrors::inputFile()
{
	CString fileSpec;
	CString filename;

	m_command.Format(L"SELECT filespec FROM filespec WHERE type=%d", ERROR_FILE);
	if (!theApp.query(m_command)) return false;
	fileSpec = theApp.m_recordset->GetFieldString(0);
	if (fileSpec.IsEmpty())
	{
		str.Format(L"A hiba text fájl nincs specifikálva!" );
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return false;
	}
	filename = fileSpec.Mid(fileSpec.ReverseFind('\\') + 1);
	fileSpec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);

	if (_waccess(fileSpec, 0))
	{
		str.Format(L"CUnitedEntriesErrors\n%s fájl nem létezik!", fileSpec);
		AfxMessageBox(str, MB_ICONERROR);
		return false;
	}

	CStdioFile file(fileSpec, CFile::modeRead);   // input csv fájl
	int fileLength = (int)file.GetLength();

	file.ReadString(str);

	int n;
	int m;
	int cnt = 0;
	UINT col;
	CString w1;
	CString w2;
	CString cLine;

	vPeople.clear();
	B.RemoveAll();

	str = L"Hibás bejegyzések beolvasása...";

	CProgressWnd wndP(NULL, str);
	wndP.GoModal();
	wndP.SetRange(0, fileLength);
	wndP.SetPos(0);

	while (file.ReadString(cLine))
	{
		++cnt;
		A.RemoveAll();
		n = wordList(&A, cLine, '\t', true);
		if (n != U_COLUMNSCOUNT)
		{
			str.Format(L"A %s\nfájl %d. sorban az elemek száma %d.\n%d kellen lenni.", fileSpec, cnt, n, U_COLUMNSCOUNT);
			AfxMessageBox(str, MB_ICONERROR);
			return false;
		}


		//Az aktuális és az elõzõ sor adatainak összehasonlítása, a különbözõk megjelölése szinezésre 
		col = 0;
		if (B.GetCount())   // van-e már elõzõs sor?
		{
			for (UINT i = 0; i < A.GetSize(); ++i)
			{
				if (i > U_ROWID && i != U_ROWIDF && i != U_ROWIDM)
				{
					w1 = A[i];
					w2 = B[i];
					if (!w1.IsEmpty() && !w2.IsEmpty() && w1 != w2)
						col = col | 1 << i;
				}
			}
		}
		// a sor feltöltése a vPeople vektorba
		str.Format(L"%d", col);
		A[0] = str;
		for (UINT i = 0; i < A.GetSize(); ++i)
			push(i);

		copyAtoB();		// a sor megõrzése a következõvel való összehasonlításhoz

		wndP.SetPos(file.GetPosition());
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	file.Close();

	m_ListCtrlE.DeleteAllItems();
	m_ListCtrlE.DetachDataset();
	m_ListCtrlE.SetItemCountEx(vPeople.size() + 1);
	m_ListCtrlE.AttachDataset(&vPeople);

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::push(int i)
{
	CString item = A[i];
	int tLen;
	TCHAR* sT;
	tLen = item.GetLength() + 1;
	sT = new TCHAR[tLen];
	_tcscpy_s(sT, tLen, item.GetBuffer());
	vPeople.push_back(sT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::copyAtoB()
{
	B.RemoveAll();
	for (UINT i = 0; i < A.GetSize(); ++i)
	{
		B.Add(A.GetAt(i));
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;

	CString status = m_ListCtrlE.GetItemText(nItem, U_STATUS);
	if (status.IsEmpty()) return;  // üres sor

	if (status == L"0")
	{
		AfxMessageBox(L"A bejegyzést már töröltük!");
		return;
	}


	CRelatives  dlg;
	dlg.m_rowid = m_ListCtrlE.GetItemText(nItem, U_ROWID);
	if (dlg.DoModal() == IDCANCEL) return;

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// D R O P D O W N   F U N C I Ó K  /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
LRESULT CUnitedEntriesErrors::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point = (CPoint*)lParam;
	CMenu	Menu;
	CMenu* pPopup;

	if (Menu.LoadMenu(IDR_DROPDOWN_UNITED_ERRORS))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::OnHtmlNotepad()
{
	int nItem = m_ListCtrlE.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrlE.GetItemText(nItem, U_LINE);
	if (!lineNumber.IsEmpty())
		theApp.editNotepad(theApp.m_htmlPathName, lineNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::OnCompare2People()
{
	int cnt = m_ListCtrlE.GetSelectedCount();
	if (cnt != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni");
		return;
	}
	POSITION	pos = m_ListCtrlE.GetFirstSelectedItemPosition();

	int nItem1 = m_ListCtrlE.GetNextSelectedItem(pos);
	int nItem2 = m_ListCtrlE.GetNextSelectedItem(pos);

	CCompareEntries dlg;
	dlg.rowid1 = m_ListCtrlE.GetItemText(nItem1, U_ROWID);
	dlg.rowid2 = m_ListCtrlE.GetItemText(nItem2, U_ROWID);
	if (dlg.rowid1.IsEmpty() || dlg.rowid2.IsEmpty())
	{
		AfxMessageBox(L"2 létezõ bejegyzést jelölj meg!");
		return;
	}
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntriesErrors::OnCompareAscendants()
{
	int db = m_ListCtrlE.GetSelectedCount();
	if (db != 2)
	{
		AfxMessageBox(L"2 bejegyzést kell kijelölni!");
		return;
	}

	POSITION	pos = m_ListCtrlE.GetFirstSelectedItemPosition();
	int nItem;

	CString rowid1;
	CString rowid2;

	nItem = m_ListCtrlE.GetNextSelectedItem(pos);
	rowid1 = m_ListCtrlE.GetItemText(nItem, U_ROWID);

	nItem = m_ListCtrlE.GetNextSelectedItem(pos);
	rowid2 = m_ListCtrlE.GetItemText(nItem, U_ROWID);

	CCompareAsc dlg;
	dlg.rowid1 = rowid1;
	dlg.rowid2 = rowid2;
	dlg.DoModal();
}
