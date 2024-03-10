// ascendants.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"



IMPLEMENT_DYNAMIC(CAscendants, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendants::CAscendants(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS, pParent)
	, ujra(_T(""))
	, m_NN(FALSE)
	, m_code(FALSE)
	, m_bold(TRUE)
	, m_GList(_T(""))
	, K1S(_T("1"))
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendants::~CAscendants()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_ANYAI_TEXT, colorAnyaiText);
	DDX_Control(pDX, IDC_GROUP_APAI_TEXT, colorApaiText);
	DDX_Control(pDX, IDC_GROUP_DATA_TEXT, colorGroupDataText);
	DDX_Control(pDX, IDC_GROUP_LIST_TEXT, colorGroupList);
	DDX_Control(pDX, IDC_GROUP_KEKULE_TEXT, colorKekuleText);

	DDX_Control(pDX, IDC_UJRA, colorUjra);
	DDX_Control(pDX, IDC_STATIC_UJRA2, colorUjra2);
	DDX_Text(pDX, IDC_EDIT_UJRA, ujra);
	DDX_Control(pDX, IDC_MAXGTEXT, colorMaxGText);

	DDX_Check(pDX, IDC_CHECK_NN, m_NN);
	DDX_Radio(pDX, IDC_ANSI, m_code);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bold);
	DDX_Text(pDX, IDC_EDIT, m_GList);
	DDX_Text(pDX, IDC_EDIT_KEKULE, K1S);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendants, CDialogEx)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_UJRA, &CAscendants::OnClickedUjra)
	ON_STN_CLICKED(IDC_STATIC_UJRA2, &CAscendants::OnClickedUjra)
	ON_EN_KILLFOCUS(IDC_EDIT_KEKULE, &CAscendants::OnKillfocusEditKekule)
	ON_BN_CLICKED(IDC_BUTTON_PIRAMID, &CAscendants::OnClickedButtonPiramid)
	ON_BN_CLICKED(IDC_BUTTON_BRANCHES, &CAscendants::OnClickedButtonBranches)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString fileName(L"ascendants_statistics");
	CString fileSpec;
	fileSpec = theApp.openTextFile(&fl, fileName, L"w+");

	COLORREF piros = RGB(255, 0, 0);

	colorGroupDataText.SetTextColor(BLUE);
	colorAnyaiText.SetTextColor(BLUE);
	colorApaiText.SetTextColor(BLUE);
	colorUjra.SetTextColor(theApp.m_colorClick);
	colorUjra2.SetTextColor(theApp.m_colorClick);

	colorGroupList.SetTextColor(piros);
	colorKekuleText.SetTextColor(piros);

	colorMaxGText.SetTextColor(LIGHTRED);
	
	

	m_numOfFDA = getFG();
	m_numOfMDA = getMG();

//	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_HAZASTARSAK);
//	pButton->SetCheck(true);

//	m_lista = 0;
	m_maxG = 13;									// maximális generáció, ameddig elmegyünk
	createScreen();

	fclose(fl);
	//	theApp.showFile(fileSpec);
	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::createScreen()
{
	str.Format(L"%s felmenõi", m_name );
	SetWindowTextW(str);

	if (!statistic()) return false;

	if (!m_maxAct)
	{
		str.Format(L"A %d. generáció felett még lehetnek felmenõk!", m_maxG);
		str1.Format(L"%d", m_maxG + 1);
		GetDlgItem(IDC_EDIT_UJRA)->SetWindowTextW(str1);
	}
	else
	{
		GetDlgItem(IDC_UJRA)->EnableWindow(false);
		GetDlgItem(IDC_STATIC_UJRA2)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_UJRA)->EnableWindow(false);
		str.Format(L"A %d. generációig vannak felmenõk!", m_maxAct);
	}

	GetDlgItem(IDC_MAXGTEXT)->SetWindowText(str);

	m_numOfG = __max( m_numOfFG, m_numOfMG );
	m_GList.Format(L"%d", m_numOfG);
	if (m_numOfG > 5)
		m_GList = L"5";

	GetDlgItem(IDC_EDIT)->SetWindowTextW( m_GList);


	str.Format(L"%d", m_numOfFG);					// apai generációk száma
	GetDlgItem(IDC_NUMOFFG)->SetWindowTextW(str);


	str.Format(L"%d", m_numOfFDA);					// egyenesági apai felmenõk száam
	GetDlgItem(IDC_NUMOFFDA)->SetWindowTextW(str);

	str.Format(L"%d", m_numOfFA);					// apai felmenõk száma
	GetDlgItem(IDC_NUMOFFA)->SetWindowTextW(str);


	str.Format(L"%d", m_numOfMG);					// anyai generációk száma
	GetDlgItem(IDC_NUMOFMG)->SetWindowTextW(str);

	str.Format(L"%d", m_numOfMDA);					// egyenesági anyai felmenõk száma
	GetDlgItem(IDC_NUMOFMDA)->SetWindowTextW(str);

	str.Format(L"%d", m_numOfMA);					// anyai felmenõk száma	
	GetDlgItem(IDC_NUMOFMA)->SetWindowTextW(str);

	


	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egyenes ági apai felneõk számának meghatározása
int CAscendants::getFG()
{
	int		i;
	CString rowid = m_rowid;

	for (i = 0; ; ++i)
	{
		m_command.Format(L"SELECT father_id FROM people WHERE rowid='%s'", rowid);
		if (!query(m_command)) return 0;
		rowid = m_recordset.GetFieldString(0);
		if (rowid.IsEmpty() || rowid == L"0" || _wtoi(rowid) > NOFATHERID)  break;
	}
	return(i);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egyenes ági anyai felmenõk számának meghatározása
int CAscendants::getMG()
{
	int		i;
	CString rowid = m_rowid;

	m_command.Format(L"SELECT mother_id FROM people WHERE rowid='%s'", rowid);
	if (!query(m_command)) return 0;
	rowid = m_recordset.GetFieldString(0);
	if (rowid.IsEmpty() || rowid == L"0" || _wtoi(rowid) > NOFATHERID)  return 0;

	for (i = 0; ; ++i)
	{
		m_command.Format(L"SELECT father_id FROM people WHERE rowid='%s'", rowid);
		if (!query(m_command)) return 0;
		rowid = m_recordset.GetFieldString(0);
		if (rowid.IsEmpty() || rowid == L"0" || _wtoi(rowid) > NOFATHERID)  break;
	}
	return(i + 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnPaint()
{
	CPaintDC dc(this);

	COLORREF	color = RGB(0, 144, 216);
	COLORREF color2;

	color = LIGHTBLUE;
	color2 = GOLD;
	color2 = RGB(255, 0, 0);
			
	colorFrame(&dc, IDC_GROUP_DATA, color);
	colorFrame(&dc, IDC_GROUP_APAI, color);
	colorFrame(&dc, IDC_GROUP_ANYAI, color);

	colorFrame(&dc, IDC_GROUP_LIST, color2 );
	colorFrame(&dc, IDC_GROUP_KEKULE, color2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color)
{
	CRect rc;

	HPEN penLine = CreatePen(PS_SOLID, 0, color);
	dc->SelectObject(penLine);

	CWnd* pW = this->GetDlgItem(IDC);
	pW->GetWindowRect(&rc);
	this->ScreenToClient(&rc);


	rc.TopLeft().x -= 1;	// a szürke keret mellett kell rajzolni, mert különben a keret felülírja a színes négyzetet
	rc.BottomRight().x += 1;	// ez szebb, mint az alábbi vonalas megoldás, de kifehérítia négyszögön belül és a szövegek háttárszíne szürke marad.
	rc.TopLeft().y += 5;	// Ezért mégis szebba vonalas rajzolás

	COLORREF colour = ::GetSysColor(COLOR_3DFACE);
	HBRUSH hbr = CreateSolidBrush(colour);
	dc->SelectObject(hbr);	// hogy a háttér olyan szürke legyen, mint a benne lévõ control-ok (szöveg, gomb )

	dc->Rectangle(&rc);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Meghatározza az alábbi értékekeket:
// m_numOfFG - apai felmenõk generációinak száma
// m_numOfMG - anyai felmenõk generációinak száma


// m_numOfFA - összes létezõ apa felmenõk száma
// m_numOfMA - összes létezõ anyai felmenõk száma
bool CAscendants::statistic()
{
	ENTRY e;
	GKR gkr;
	PARENTS p;

	_int64 i = 0;
	_int64 K = 1;	// kekulé szám
	int G = 0;		// generációk számolása

	vGKR.clear();

	gkr.g = 0;
	gkr.k = 1;
	gkr.r = m_rowid;
	gkr.p = false;
	vGKR.push_back(gkr);
	G = 1;
	K = 2;

	_int64 KCH;

	m_numOfFA = 0;			// apai felmenõk száma
	m_numOfFG = 0;			// apai felmenõk legnagyobb generációs száma
	m_numOfMA = 0;			// anyai felmenõk száma
	m_numOfMG = 0;			// apai felmenõk legnagyobb generációs száma

	UINT entriesInGeneration = 0;

	_int64 kMinG = pow(2, (G));
	_int64 kMaxG = pow(2, (G + 1)) - 1;

	_int64 apaiKMin = kMinG;
	_int64 apaiKMax = kMinG + (kMaxG - kMinG) / 2;

	str.Format(L"Felmenõk számának meghatározása maximum %d. generációig.", m_maxG);
	CProgressWnd wndP(NULL, str );
	wndP.GoModal();
	wndP.SetPos(0);

	m_maxAct = 0;

	while (true)
	{
#ifndef _DEBUG
		str.Format(L"G=%d K=%I64d size=%d", G, K, vGKR.size() );
		wndP.SetText(str);
#endif
		if (K < kMaxG)   //  még ugyanabban a generációban vagyunk
		{
			KCH = K / 2;	// a gyerek Kekule száma;
			getParents(KCH, &p);
			if (!p.father_id.IsEmpty())
			{
				gkr.g = G;
				gkr.k = K;
				gkr.r = p.father_id;
				vGKR.push_back(gkr);

				if (apaiKMin <= K && K <= apaiKMax)
				{
					++m_numOfFA;
					m_numOfFG = G;
				}
				else
				{
					++m_numOfMA;
					m_numOfMG = G;
				}

				++entriesInGeneration;
			}
			++K;
			if (!p.mother_id.IsEmpty())
			{
				gkr.g = G;
				gkr.k = K;
				gkr.r = p.mother_id;
				vGKR.push_back(gkr);
				++entriesInGeneration;

				if (apaiKMin <= K && K <= apaiKMax)
				{
					++m_numOfFA;
					m_numOfFG = G;
				}
				else
				{
					++m_numOfMA;
					m_numOfMG = G;
				}
			}
			++K;
			if (K > kMaxG)
			{
				if (!entriesInGeneration)
				{
					m_maxAct = G-1;
					break;
				}
				++G;				// következõ generációba lépünk
				kMinG = pow(2, G);
				kMaxG = pow(2, (G + 1)) - 1;
				apaiKMin = kMinG;
				apaiKMax = kMinG + (kMaxG - kMinG) / 2;

				if (G > m_maxG) break;  // eddig ás nem tovább

				++entriesInGeneration = 0;
			}
		}
	}
	wndP.DestroyWindow();
	for (UINT i = 0; i < vGKR.size(); ++i)
	{
		str.Format(L"%4d %10d % 10s\n", vGKR.at(i).g, vGKR.at(i).k, vGKR.at(i).r);
		fwprintf(fl, str);
	}

	// Az utolsó generáció már nem tartalmaz bejegyzést!!!

	str.Format(L"\n\
Generációk száma: %d\n\
Apai generációk száma: %d\n\
Apai felmenõk száma: %d\n\
Anyai generációk száam: %d\n\
Anyai felmenõk száma: %d\n\
Felmenõk száma: %d\n\
", \
G - 1, m_numOfFG, m_numOfFA, m_numOfMG, m_numOfMA, vGKR.size() - 1);

	fwprintf(fl, str);

	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::getParents(_int64 k, PARENTS* p)
{
	CString rowid;
	CString father_id;
	CString mother_id;
	p->father_id.Empty();
	p->mother_id.Empty();

	int z;
	for (UINT i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).k == k)
		{
			rowid = vGKR.at(i).r;
			m_command.Format(L"SELECT father_id, mother_id FROM people WHERE rowid ='%s'", rowid);
			if (!query3(m_command)) return false;
			if (m_recordset3.RecordsCount())
			{
				p->father_id = m_recordset3.GetFieldString(0);
				p->mother_id = m_recordset3.GetFieldString(1);
				if (p->father_id == L"0") p->father_id.Empty();
				if (p->mother_id == L"0") p->mother_id.Empty();
			}
			break;
		}
	}
	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::getEntry(CString rowid, ENTRY* e)
{

	e->g = 0;
	e->kekule = 0;
	e->processed = false;

	e->rowid = rowid;
	e->sexId = 0;
	e->name.Empty();
	e->father_id.Empty();
	e->mother_id.Empty();


	if (rowid.IsEmpty()) return false;

	CString name;

	m_command.Format(L"SELECT sex_id, last_name, first_name, father_id, mother_id FROM people WHERE rowid='%s'", rowid);
	if (!query3(m_command)) return false;

	e->sexId = _wtoi(m_recordset3.GetFieldString(0));
	name.Format(L"%s %s", m_recordset3.GetFieldString(1), m_recordset3.GetFieldString(2));
	e->name = name;

	e->father_id = m_recordset3.GetFieldString(3);
	e->mother_id = m_recordset3.GetFieldString(4);

	if (e->father_id == L"0") e->father_id.Empty();
	if (e->mother_id == L"0") e->mother_id.Empty();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::query(CString command)
{
	if (m_recordset.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::query1(CString command)
{
	if (m_recordset1.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::query2(CString command)
{
	if (m_recordset2.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::query3(CString command)
{
	if (m_recordset3.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset3.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::query4(CString command)
{
	if (m_recordset4.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset4.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedUjra()
{
	GetDlgItem(IDC_EDIT_UJRA)->GetWindowTextW(str);
	if (str.IsEmpty())
	{
		AfxMessageBox(L"Az újraszámolás maximális generációs számát meg kell adni!", MB_ICONEXCLAMATION);
		return;
	}
	int maxG = _wtoi(str);
	if (maxG <= m_maxG)
	{
		str.Format(L"Csak %d generációnál nagyobb éértékre számolok újra!", m_maxG );
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return;
	}
	m_maxG = maxG;
	createScreen();
}
/*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedRadioHazastarsak()
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);		// listázandó generációk száam
	GetDlgItem(IDC_CHECK_NN)->EnableWindow(TRUE);	// nem ismert felmenõk helyettesítése NN-el
	str.Format(L"%d", __max(m_numOfFG, m_numOfMG));
	GetDlgItem(IDC_EDIT)->SetWindowText(str);
	m_lista = 0;
}
void CAscendants::OnRadioApaianyai()
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_NN)->EnableWindow(TRUE);
	str.Format(L"%d", __max(m_numOfFG, m_numOfMG));
	GetDlgItem(IDC_EDIT)->SetWindowText(str);
	m_lista = 1;
}
void CAscendants::OnRadioApai()
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_NN)->EnableWindow(TRUE);
	str.Format(L"%d", m_numOfFG);
	GetDlgItem(IDC_EDIT)->SetWindowText(str);
	m_lista = 2;
}
void CAscendants::OnRadioAnyai()
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_NN)->EnableWindow(TRUE);
	str.Format(L"%d", m_numOfMG);
	GetDlgItem(IDC_EDIT)->SetWindowText(str);
	m_lista = 3;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////û
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CAscendants::OnBnClickedOk()
{
	UpdateData(FROMSCREEN);

	m_ulStyle = L"<ul style=list-style-type:none;><li>";
	m_numOfG = _wtoi( m_GList );
	
	//	m_fileSpec = openFile();

	switch (m_lista)
	{
	case 0:						// Kekule szám sorrendben 
//		printKekule2();
		break;
	case 1:
//		allAscendants();
		break;
	case 2:										// ascendenza paterna
		paternalAscendants();
		break;
	case 3:										// ascendenza materna
		maternalAscendants();
		break;
	}
	fclose(fh);
	ShellExecute(NULL, L"open", m_filespec, NULL, NULL, SW_SHOWNORMAL);
//	CDialogEx::OnOK();
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedButtonPiramid()
{
	UpdateData(FROMSCREEN);

	m_ulStyle = L"<ul style=list-style-type:none;><li>";
	m_numOfG = _wtoi(m_GList);

	CString title = L"felmenõinek Kekule piramisa";
	if (K1S != L"1")
		title.Format(L"%s felmenõinek Kekule piramisa a %s számtól", m_name, K1S);
	else
		title.Format(L"%s felmenõinek Kekule piramisa", m_name );
	CString filename = L"generationsKekule";
	openHtml(filename, title);

	CProgressWnd wndProgress(NULL, L"Felmenõk rendezése....");
	wndProgress.GoModal();


	CString	people;
	CString	ss;
	int		gen;
	int		g;

	_int64	kekule;
	_int64	k = 1;
	int		genPrev = -1;

	_int64 kMax = (_int64)pow(2, m_numOfG) - 1;
	m_cnt_ul = 0;

	_int64 K1 = _wtoi(K1S);		// a piramis csúcsának Kekule száma

	_int64 kMIN = K1;
	_int64 kMAX = kMIN;
	_int64 K = K1;			// futó k érték

	int z;
	int j;
	m_GPrev = -1;
	int cnt = 0;

	m_cnt = 0;
	for (int i = 0; i < vGKR.size(); ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		g = vGKR.at(i).g;
		if (k == 33)
			z = 1;
		if (kMIN <= k)								// elértük a kiirandó minimumot
		{
			if (k == K)								// ez pont ki kell írni
			{
				listPeople(i);
				++K;
			}
			else
			{
				if (k <= kMAX)					    // még ugyanabban a generációban van	
				{
					for (int j = 0; j < k - K + 1; ++j)
					{
						printNN(g, K);
						++K;
					}
					listPeople(i);
					++K;
				}
				else									// már a következõ generációban van
				{
					for (j = 0; j <= kMAX - K; ++j)    // még ugynaz a generáció
					{
						printNN(vGKR.at(i - 1).g, K + j);
					}
					if (!m_cnt)
						break;
					m_cnt = 0;
					K += j;
					for (int j = 0; j < k - kMAX - 1; ++j)    // a következõ generációban is NN
					{
						printNN(g, K + j);
					}
					listPeople(i);
					kMIN = kMIN * 2;				// új min-max értékek
					kMAX = kMAX * 2 + 1;
					K = k + 1;
				}
			}
			if (K > kMAX)						// a generáció utolsó tagját írtuk ki, új min-max értékek
			{
				if (!m_cnt) 
					break;				// a generáció üres volt!
				m_cnt = 0;						// újra kezdi számolni a generációban talált embereket
				++cnt;
//				if (cnt == m_numOfG) break;
				kMIN = kMIN * 2;				// új min-max értékek
				kMAX = kMAX * 2 + 1;
				K = kMIN;
			

			}
			if (cnt == m_numOfG)
				break;
		}
	}
	fclose(fh);
	ShellExecute(NULL, L"open", m_filespec, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printKekule1()
{
	CString title;
	title.Format(L"%s felmenõinek Kekule piramisa", m_name);
	CString filename = L"generationsKekule";
	openHtml(filename, title);

	CProgressWnd wndProgress(NULL, L"Felmenõk rendezése....");
	wndProgress.GoModal();


	CString	people;
	CString	ss;
	int		gen;
	int		g;

	_int64	kekule;
	_int64	k = 1;
	int		genPrev = -1;

	_int64 kMax = (_int64)pow(2, m_numOfG) - 1;
	m_cnt_ul = 0;

	_int64 K1 = _wtoi(K1S);		// a piramis csúcsának Kekule száma

	_int64 kMIN = K1;
	_int64 kMAX = kMIN;
	_int64 K = K1;			// futó k érték

	int z;
	int j;
	m_GPrev = -1;
	int cnt = 0;


	for (UINT i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).g <= m_numOfG)
		{
			kekule = vGKR.at(i).k;
			if (kekule != k)
			{
				if (m_NN)
				{
					people = L"N N";
					kekule = k;
					str.Format(L"%s%I64d &diams;%s<br>\n", ss, kekule, people);
					if (m_code == UTF8) str = UnicodeToUtf8(str);
					fwprintf(fh, L"%s", str);


					--i;  // ennek az embernek a kiírását próbálja megismételni, ha sorra kerül
					++k;
					continue;
				}
			}
			gen = vGKR.at(i).g;
			ss = indent(gen, genPrev);
			genPrev = gen;

			people = getPeopleString(i);
			str.Format(L"%s%I64d &diams;%s<br>\n", ss, kekule, people);
			if (m_code == UTF8) str = UnicodeToUtf8(str);
			fwprintf(fh, L"%s", str);
			++k;
		}
	}

	if (m_NN && k < kMax)
	{
		for (; k <= kMax; ++k)
		{
			str.Format(L"%s%I64d &diams;%s<br>\n", ss, k, L"N N");
			if (m_code == UTF8) str = UnicodeToUtf8(str);
			fwprintf(fh, L"%s", str);
		}
	}

	for (int i = 0; i < m_cnt_ul; ++i)
		fwprintf(fh, L"</ul>\n");
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedButtonBranches()
{
	UINT k;

	if ((k = apja(0)) == -1)
	{
		AfxMessageBox(L"Nincs apja!");
	}

	m_ulStyle = L"<ul style=list-style-type:none;><li>";
	m_numOfG = _wtoi(m_GList);


	CString title;
	title.Format(L"%s felmenõi", m_name);
	CString filename = L"felmenok";
	openHtml(filename, title);

	m_GPrev = -1;
	listPeople(0);
	ascendants(0, false );
	fclose(fh);
	ShellExecute(NULL, L"open", m_filespec, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza paterna
void CAscendants::paternalAscendants()
{
	UINT k;

	if ((k = apja(0)) == -1)
	{
		AfxMessageBox(L"Nincs apja!");
	}

	CString title;
	title.Format(L"%s apai felmenõi", m_name);
	CString filename = L"apaiFelmenok";

	openHtml(filename, title);

	m_GPrev = -1;
	listPeople(0);
	ascendants( 0, true );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza materna
void CAscendants::maternalAscendants()
{
	UINT k;

	if ((k = anyja(0)) == -1)
	{
		AfxMessageBox(L"Nincs anyja!");
	}

	CString title;
	title.Format(L"%s anyai felmenõi", m_name);
	CString filename = L"anyaiFelmenok";

	openHtml(filename, title);

	m_GPrev = -1;
	listPeople(0);
	listPeople(k);
	ascendants(k, false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i: vGKR indexe, akinek elsõként az apját keressük
// fatherBranch: csak az apai ágat listázzuk
void CAscendants::ascendants( UINT i, bool fatherBranch )
{
	// minden felmenõ nem listázott státusba kerül
	for (UINT i = 0; i < vGKR.size(); ++i)
		vGKR.at(i).p = false;

	UINT G;
	UINT k;
	while( true)
	{
		if ( (k= apja(i)) != -1 )  // van apja
		{
			listPeople(k);
			i = k;
			continue;
		}
		if( isOdd(i) && (k=felesege(i)) != -1 )  // van feleség
		{
			listPeople(k);
			i = k;
			continue;
		}
		// visszafele megkeresi az alacsonyabb generációkban az elsõ már kiírt férfit és ha van felesége, akkor azt kiírja
		G = vGKR.at(i).g;
		for (; i > 0; --i)
		{
			k = vGKR.at(i).k;
			if (vGKR.at(i).g < G )
			{ 
				if( isEven( vGKR.at(i).k ) )
				{
					if( vGKR.at(i).p )			// megvan a férj, a feleségét keressük
					{
						if ( fatherBranch == 1 && vGKR.at(i).k == 2)  // csak az apai ágat listázzuk
							break;
						// felesége
						if (i + 1 < vGKR.size())
						{
							if (vGKR.at(i + 1).k == vGKR.at(i).k + 1)  // felesége megvan
							{
								if (!vGKR.at(i + 1).p)					// még nem írtuk ki
								{
									i = i + 1;
									listPeople(i);
									break;
								}
							}
						}
					}
				}
			}
		}
		if( !i )break;  // nincs már 
		if ( fatherBranch == 1 && k == 2) break;  // csak az apai ágat listázzuk
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az vGKR.at(i) bejegyzés apját keresi. Ha megtalálta, visszadja a vGKR-beli indexét, ha nem , akkor -1-et
UINT CAscendants::apja(UINT i)
{
	UINT k = vGKR.at(i).k * 2;
	for (UINT j = i; j < vGKR.size(); ++j)  // elég j-tõl keresni, mert az apja biztos feljebb van
	{
		if (vGKR.at(j).k == k)
			return j;
	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az vGKR.at(i) bejegyzés anyját keresi. Ha megtalálta, visszadja a vGKR-beli indexét, ha nem , akkor -1-et
UINT CAscendants::anyja(UINT i)
{
	UINT k = vGKR.at(i).k * 2 + 1;
	for (UINT j = i; j < vGKR.size(); ++j)  // elég j-tõl keresni, mert az anyja biztos feljebb van
	{
		if (vGKR.at(j).k == k)
			return j;
	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az vGKR.at(i) bejegyzés feleségét keresi. Ha megtalálta, visszadja a vGKR-beli indexét, ha nem , akkor -1-et
UINT CAscendants::felesege(UINT i)
{
	if (i + 1 < vGKR.size())
	{
		if (vGKR.at(i + 1).k == vGKR.at(i).k + 1)
			return(i + 1);
	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A  vGKR i. bejegyzését listázza
void CAscendants::listPeople( int i)
{
	CString ss;
	int		g = vGKR.at(i).g;
	_int64	k = vGKR.at(i).k;

	ss = indent(g, m_GPrev);
	m_GPrev = g;

	CString people = getPeopleString(i);
	str.Format(L"%s%I64d &diams;%s\n", ss, k, people);
	print(str);
	vGKR.at(i).p = true;
	++m_cnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A  vGKR i. bejegyzését listázza
void CAscendants::printNN( int g, _int64 k)
{
	if (!m_NN) return;
	CString ss;

	ss = indent(g, m_GPrev);
	m_GPrev = g;
	CString people(L"N N");
	str.Format(L"%s%I64d &diams;%s\n", ss, k, people);

	print(str);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print(CString)
{
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::indent(int gen, int genPrev)
{
	CString ss;


	if (gen == genPrev)			// azonos generáció, azonos behúzás
		ss = L"<li>";
	else if (gen > genPrev)
	{
		ss = m_ulStyle;
	//	ss = L"<ul><li>";			// új generáció, beljebb nyomja, default style
		++m_cnt_ul;
	}
	else if (gen < genPrev)        // régi generáció, kijebb hozza a generáció-különbség-szeresen
	{
		for (int j = 0; j < (genPrev - gen); ++j)  // behúzások törlése
		{
			fwprintf(fh, L"</ul>");
		}
		ss = L"<li>";
	}
	return ss;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::openHtml( CString file, CString title )
{
	CString filename;
	filename.Format(L"%s_%s", m_name, file);
	m_filespec = theApp.openHtmlFile(&fh, filename, L"w+");
	if (m_filespec.IsEmpty() ) return false;

	CString program;
	program.Format(L"Dragon version %s", theApp.m_version);

	CString head;
	head = L"<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>";
	if (m_code == UTF8)
	{
		head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>";
		head = UnicodeToUtf8(head);
	}
	fwprintf(fh, L"%s\n", head);

	str = title;
	if (m_code == UTF8) str = UnicodeToUtf8(title);
	fwprintf(fh, L"<center>%s</center>\n", str);



	fwprintf(fh, L"<pre>");

	str.Format(L"%s %s\n", L"program                           :", program);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %s\n", L"adatbázis                         :", theApp.m_databaseSpec);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %s\n", L"listafájl                         :", m_filespec);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %s\n", L"lista készült                     :", theApp.getPresentDateTime());
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %d\n", L"Egyenesági apai felmenõk száma    :", m_numOfFDA);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %d\n", L"Egyenesági anyai felmenõk száma   :", m_numOfMDA);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s\n", str);

	str.Format(L"%s %s\n", L"Listázásra elõírt generációk száma:", m_GList);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s\n", str);



	if (!m_maxAct)
	{
		str.Format(L"%s %d %s\n", L"Az alábbi adatokat ", m_maxG, L" generáció vizsgálata alapján számoltuk:" );
		if (m_code == UTF8) str = UnicodeToUtf8(str);
		fwprintf(fh, L"%s\n", str);
	}

	str.Format(L"%s %d\n", L"Apai generációk száma             :", m_numOfFG);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %d\n", L"Apai felmenõk száma               :", m_numOfFA);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %d\n", L"Anyai generációk száma            :", m_numOfMG);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	str.Format(L"%s %d\n", L"Anyai felmenõk száma              :", m_numOfMA);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);

	

	fwprintf(fh, L"</pre>\n");
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::getPeopleString(int i)
{
	CString rowid = vGKR.at(i).r;
	CString people;
	CString spouse_id;

	CString sp(L" ");
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid);
	if (!query3(m_command)) return(L"");

	CString name;
	CString	str;
	CString str1;
	CString generation;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	int		parentIndex;
	int		parentIndexCalc;
	CString comment;
	CString arm;
	CString peer;
	CString csalad;

	generation = m_recordset3.GetFieldString(PEOPLE_GENERATION);
	title = m_recordset3.GetFieldString(PEOPLE_TITLE);
	titolo = m_recordset3.GetFieldString(PEOPLE_TITOLO);
	last_name = m_recordset3.GetFieldString(PEOPLE_LAST_NAME);
	first_name = m_recordset3.GetFieldString(PEOPLE_FIRST_NAME);
	birth_place = m_recordset3.GetFieldString(PEOPLE_BIRTH_PLACE);
	birth_date = m_recordset3.GetFieldString(PEOPLE_BIRTH_DATE);
	death_place = m_recordset3.GetFieldString(PEOPLE_DEATH_PLACE);
	death_date = m_recordset3.GetFieldString(PEOPLE_DEATH_DATE);
	father_id = m_recordset3.GetFieldString(PEOPLE_FATHER_ID);
	mother_id = m_recordset3.GetFieldString(PEOPLE_MOTHER_ID);
	parentIndex = _wtoi(m_recordset3.GetFieldString(PEOPLE_MOTHER_INDEX));
	parentIndexCalc = _wtoi(m_recordset3.GetFieldString(PEOPLE_MOTHER_INDEX2));
	comment = m_recordset3.GetFieldString(PEOPLE_COMMENT);
	arm = m_recordset3.GetFieldString(PEOPLE_ARM);
	csalad = m_recordset3.GetFieldString(PEOPLE_CSALAD);
	peer = m_recordset3.GetFieldString(PEOPLE_PEER);

//	name.Format(L"%s %s", last_name, first_name);

	if (peer == L"gróf")
		peer = L"gr";
	else if (peer == L"báró")
		peer = L"br";
	else if (peer == L"herceg")
		peer = L"hg";

	if (m_bold)
		str = L"<b>";
	else
		str.Empty();

	if (!peer.IsEmpty()) { str += peer, str += " "; }
	if (!title.IsEmpty()) { str += title, str += " "; }
	if (!titolo.IsEmpty()) { str += titolo, str += L" "; }
	if (!last_name.IsEmpty()) { str += last_name, str += L" "; };
	if (!first_name.IsEmpty()) { str += first_name, str += L" "; };
	str.TrimRight();

	if (m_bold) str += L"</b>";
/*
	if (parentIndex)
	{
		str1.Format(L"/%d", parentIndex);
		str += str1;
	}
*/
	if (!birth_place.IsEmpty() || !birth_date.IsEmpty())
	{
		str += L" *";
		if (!birth_place.IsEmpty()) str += birth_place;
		if (!birth_date.IsEmpty())
		{
			if (!birth_place.IsEmpty()) str += sp;
			str += birth_date;
		}
	}

	if (!death_place.IsEmpty() || !death_date.IsEmpty())
	{
		str += L" +";
		if (!death_place.IsEmpty()) str += death_place;
		if (!death_date.IsEmpty())
		{
			if (!death_place.IsEmpty()) str += sp;
			str += death_date;
		}
	}

	if (!comment.IsEmpty()) { str += " "; str += comment; }

	if (i < vGKR.size() - 1)
	{
		if (vGKR.at(i + 1).k == vGKR.at(i).k + 1)
		{
			spouse_id = vGKR.at(i + 1).r;
			str += L" ";
			str += getMarriage(rowid, spouse_id);
			str.TrimRight();
		}
	}



	if (!arm.IsEmpty())
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	if (!csalad.IsEmpty())
	{
		str += L" [";
		str += csalad;
		str += L" család]";
	}
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::getMarriage(CString rowid, CString rowidS)
{
	if (rowidS.IsEmpty()) return L"";

	CString marriage(L"");

	m_command.Format(L"SELECT place, date FROM marriages WHERE spouse1_id ='%s' AND spouse2_id= '%s'", rowid, rowidS);
	if (!query4(m_command)) return L"";

	for (UINT i = 0; i < m_recordset4.RecordsCount(); ++i, m_recordset4.MoveNext())
	{
		if (m_recordset4.RecordsCount() == 1)
			marriage = L"=";
		else
		{
			str.Format(L" %d=", i + 1);
			marriage += str;
		}
		marriage += m_recordset4.GetFieldString(0);
		marriage.TrimRight();
		marriage += L" ";
		marriage += m_recordset4.GetFieldString(1);
		marriage.Trim();
	}
	int n = marriage.GetLength();
	if (n < 3) marriage.Empty();
	return marriage;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnKillfocusEditKekule()
{
	GetDlgItem(IDC_EDIT_KEKULE)->GetWindowTextW(str);
	int k = _wtoi(str);
	for (UINT i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).k == k) return;
	}
	str.Format(L"%s %d Kekule számú felmenõje nem ismert!", m_name, k);
	AfxMessageBox(str, MB_ICONEXCLAMATION);
	GetDlgItem(IDC_EDIT_KEKULE)->SetWindowTextW(L"1");
}
