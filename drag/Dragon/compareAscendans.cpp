
#include "pch.h"
#include "Dragon.h"
#include "ListCtrlEx.h"
#include "afxdialogex.h"
#include "compareAscendants.h"
enum
{
	CA_GENERATION = 0,
	CA_KEKULE,
	CA_ROWID1,
	CA_SOURCE1,
	CA_NAME1,
	CA_BIRTH1,
	CA_DEATH1,
	CA_ROWID2,
	CA_SOURCE2,
	CA_NAME2,
	CA_BIRTH2,
	CA_DEATH2,
};
IMPLEMENT_DYNAMIC(CCompareAsc, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCompareAsc::CCompareAsc(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPARE_ASC, pParent)
{
	m_recP = new CSqliteDBRecordSet;

	m_ListCtrl.m_ctlHeader.h11 = 2;
	m_ListCtrl.m_ctlHeader.h12 = 6;
	m_ListCtrl.m_ctlHeader.h21 = 7;
	m_ListCtrl.m_ctlHeader.h22 = 11;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCompareAsc::~CCompareAsc()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCompareAsc, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCompareAsc::OnCustomdrawList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCompareAsc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	m_ListCtrl.InsertColumn(CA_GENERATION, L"gen", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrl.InsertColumn(CA_KEKULE, L"kekule", LVCFMT_RIGHT, 50, -1, COL_NUM);

	m_ListCtrl.InsertColumn(CA_ROWID1, L"rowid1", LVCFMT_RIGHT, 55, -1, COL_NUM);
	m_ListCtrl.InsertColumn(CA_SOURCE1, L"s1", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrl.InsertColumn(CA_NAME1, L"name1", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(CA_BIRTH1, L"birth1", LVCFMT_RIGHT, 80, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(CA_DEATH1, L"death1", LVCFMT_RIGHT, 80, -1, COL_TEXT);

	m_ListCtrl.InsertColumn(CA_ROWID2, L"rowid2", LVCFMT_RIGHT, 55, -1, COL_NUM);
	m_ListCtrl.InsertColumn(CA_SOURCE2, L"s2", LVCFMT_RIGHT, 30, -1, COL_NUM);
	m_ListCtrl.InsertColumn(CA_NAME2, L"name2", LVCFMT_LEFT, 200, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(CA_BIRTH2, L"birth2", LVCFMT_RIGHT, 80, -1, COL_TEXT);
	m_ListCtrl.InsertColumn(CA_DEATH2, L"death2", LVCFMT_RIGHT, 80, -1, COL_TEXT);

	CString fileName(L"ascendants");
	CString fileSpec = theApp.openTextFile(&fl, fileName, L"w+");

	if (!fillVector(rowid1, &vGKR1)) { fclose(fl); CDialog::OnOK(); return false; }
	if (!fillVector(rowid2, &vGKR2)) { fclose(fl); CDialog::OnCancel(); return false; }
	fclose(fl);
//	theApp.showFile(fileSpec);


	listVectors();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCompareAsc::fillVector( CString rowid, std::vector<CA::GKR>* vGKR)
{
	int g = 0;		// generációk sorszáma
	_int64	k = 1;		// kekülé szám
	int cnt = 1;		// az aktuális generációban létezõ emberek száma
	int i1 = 0;
	int max;
	_int64 K;

	vGKR->clear();

	if (!getPeople(rowid)) return false;
	
	gkr.g = g;
	gkr.k = k;
	vGKR->push_back(gkr);

	g = 1;
	while (cnt)
	{
		cnt = 0;
		max = vGKR->size();
		for (int i = i1; i < max; ++i)			// az elõzõ generáció vGKR bejegyzésein végigszalad
		{
			if( !getParentsId(vGKR->at(i).r)) return false;

			K = vGKR->at(i).k;
			if (!m_father_id.IsEmpty())		// létezõ apa
			{
				++cnt;
				if (!getPeople(m_father_id)) return false;
				gkr.g = g;
				gkr.k = K * 2;
				vGKR->push_back(gkr);
			}
			if (!m_mother_id.IsEmpty())		// létezõ anya
			{
				++cnt;
				if (!getPeople(m_mother_id)) return false;
				gkr.g = g;
				gkr.k = K * 2 + 1;
				vGKR->push_back(gkr);
			}
		}
		++g;						// az új generációt fogja végignézni
		i1 = vGKR->size() - cnt;		// az elõzõ generáció utolsó bejegyzése 
	}
	fwprintf(fl, L"\n\n");
	for (UINT i = 0; i < vGKR->size(); ++i)
	{
		str.Format(L"%*s %4d %10d % 10s %s\n", vGKR->at(i).g * 4, L" ", vGKR->at(i).g, vGKR->at(i).k, vGKR->at(i).r, vGKR->at(i).name);
		fwprintf(fl, str);
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCompareAsc::getParentsId(CString rowid)
{
	m_command.Format(L"SELECT father_id, mother_id FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	m_father_id = m_recP->GetFieldString(0);
	m_mother_id = m_recP->GetFieldString(1);
	if (m_father_id == L"0") m_father_id.Empty();
	if (m_mother_id == L"0") m_mother_id.Empty();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCompareAsc::getPeople( CString rowid)
{
	CString name;
	m_command.Format(L"SELECT source, peer, title, titolo, last_name, first_name, birth_place,birth_date, death_place, death_date, comment FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	if (m_recP->RecordsCount() == 0)
	{
		str.Format(L"A %s rowid bejegyzés nem létezik az adatbázisban!", rowid);
		AfxMessageBox( str, MB_ICONEXCLAMATION);
		return false;
	}
	gkr.r = rowid;
	gkr.s = m_recP->GetFieldString(0);
	gkr.peer = m_recP->GetFieldString(1);
	gkr.title = m_recP->GetFieldString(2);
	gkr.titolo = m_recP->GetFieldString(3);
	name.Format(L"%s %s", m_recP->GetFieldString(4), m_recP->GetFieldString(5));
	gkr.name = name;
	gkr.birthPlace = m_recP->GetFieldString(6);
	gkr.birth = m_recP->GetFieldString(7);
	gkr.birthPlace = m_recP->GetFieldString(8);
	gkr.death = m_recP->GetFieldString(9);
	gkr.comment= m_recP->GetFieldString(10);
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::listVectors()
{
	int k1;
	int k2;
	int i1 = 0;
	int i2 = 0;

	nItem = 0;

	int kMax1 = vGKR1.at( vGKR1.size() - 1 ).k;
	int kMax2 = vGKR2.at( vGKR2.size() - 1 ).k;
	if (kMax1 >= kMax2)
	{
		vP1 = &vGKR1;
		vP2 = &vGKR2;
	}
	else
	{
		vP1 = &vGKR2;
		vP2 = &vGKR1;
	}

//	for (i1 = 0; i1 < vP1->size() && i2 < vP2->size() ; )  // vP1 maximális Kewkulé száma nagyobb, mint Vp2-é, de a vektork mérete lehet fordítva is!!!
	while( i1 < vP1->size() && i2 < vP2->size() )
	{
		m_color = 0;
		k1 = vP1->at(i1).k;
		k2 = vP2->at(i2).k;
		if (k1 == k2)
		{
			coloredCells(i1, i2);
			listBoth(i1, i2);
			++i1;
			++i2;
		}
		else if (k1 < k2)
		{
			list1(i1);
			++i1;
		}
		else
		{
			list2(i2);
			++i2;
		}
	}
	for (; i1 < vP1->size(); ++i1)
	{
		list1(i1);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::coloredCells(int i1, int i2)
{
	DWORD colorBits;

	CString str1;
	CString str2;

	colorBits = 0;

	str1 = vP1->at(i1).name.Trim();
	str2 = vP2->at(i2).name.Trim();
	if (!str1.IsEmpty())
	{
		if (!str2.IsEmpty())
		{
			if (str1 != str2)
			{
				colorBits = colorBits | (1 << CA_NAME1);
				colorBits = colorBits | (1 << CA_NAME2);
			}
		}
	}

	str1 = vP1->at(i1).birth;
	str2 = vP2->at(i2).birth;
	if (!str1.IsEmpty() && !str2.IsEmpty() && str1 != str2)
	{
		colorBits = colorBits | (1 << CA_BIRTH1);
		colorBits = colorBits | (1 << CA_BIRTH2);
	}

	str1 = vP1->at(i1).death;
	str2 = vP2->at(i2).death;
	if (!str1.IsEmpty() && !str2.IsEmpty() && str1 != str2)
	{
		colorBits = colorBits | (1 << CA_DEATH1);
		colorBits = colorBits | (1 << CA_DEATH2);
	}
	m_color = colorBits;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::listBoth( int i1, int i2 )
{
	int gen = vP1->at(i1).g;

	str.Format(L"%d", gen);
	nItem = m_ListCtrl.InsertItem(nItem, str);
	m_ListCtrl.SetItemData(nItem, m_color);
	str.Format(L"%d", vP1->at(i1).k);
	m_ListCtrl.SetItemText(nItem, CA_KEKULE, str);
	list_vP1( i1);
	list_vP2( i2);
	++nItem;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::list1(int i)
{
	int gen = vP1->at(i).g;
	str.Format(L"%d", gen);
	nItem = m_ListCtrl.InsertItem(nItem, str);
	m_ListCtrl.SetItemData(nItem, 0 );			// Csak 1 ember van, színezés nincs
	str.Format(L"%d", vP1->at(i).k);
	m_ListCtrl.SetItemText(nItem, CA_KEKULE, str);
	
	list_vP1(i);
	++nItem;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::list2(int i)
{
	int gen = vP2->at(i).g;
	str.Format(L"%d", gen);
	nItem = m_ListCtrl.InsertItem(nItem, str);
	m_ListCtrl.SetItemData(nItem, 0 );

	str.Format(L"%d", vP2->at(i).k);
	m_ListCtrl.SetItemText(nItem, CA_KEKULE, str);
	
	list_vP2(i);
	++nItem;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::list_vP1( int i )
{
	m_ListCtrl.SetItemText(nItem, CA_ROWID1, vP1->at(i).r);
	m_ListCtrl.SetItemText(nItem, CA_SOURCE1, vP1->at(i).s);
	m_ListCtrl.SetItemText(nItem, CA_NAME1, vP1->at(i).name);
	m_ListCtrl.SetItemText(nItem, CA_BIRTH1, vP1->at(i).birth);
	m_ListCtrl.SetItemText(nItem, CA_DEATH1, vP1->at(i).death);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::list_vP2( int i )
{
	m_ListCtrl.SetItemText(nItem, CA_ROWID2, vP2->at(i).r);
	m_ListCtrl.SetItemText(nItem, CA_SOURCE2, vP2->at(i).s);
	m_ListCtrl.SetItemText(nItem, CA_NAME2, vP2->at(i).name);
	m_ListCtrl.SetItemText(nItem, CA_BIRTH2, vP2->at(i).birth);
	m_ListCtrl.SetItemText(nItem, CA_DEATH2, vP2->at(i).death);
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
BOOL CCompareAsc::queryP(CString command)
{
	if (m_recP->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recP->GetLastError());
		theApp.message(L"Query", str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompareAsc::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem;
	int nCol;
	int mask;
	int iData;

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
		mask = 1 << nCol;
		iData = m_ListCtrl.GetItemData(nItem);
		if (iData & mask)
		{
			if (nCol == CA_NAME1 || nCol == CA_NAME2)
				pLVCD->clrTextBk = YELLOW;
			else if (nCol == CA_BIRTH1 || nCol == CA_BIRTH2)
				pLVCD->clrTextBk = RGB(153, 255, 255);
			else if (nCol == CA_DEATH1 || nCol == CA_DEATH2)
				pLVCD->clrTextBk = RGB(255, 204, 229);
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
