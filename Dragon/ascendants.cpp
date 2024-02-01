#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"
#include "ascendantsLinear_Table.h"

IMPLEMENT_DYNAMIC(CAscendants, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendants::CAscendants(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS, pParent)
	, m_numOfFG(_T(""))
	, m_numOfFA(_T(""))
	, s_numOfAF(_T(""))
	, s_numOfAM(_T(""))
	, s_numOfGF(_T(""))
	, s_numOfGM(_T(""))
	, s_numOfGDF(_T(""))
	, m_bold(true)
	, m_firstBold(false)
	, m_NN(false)
	, m_peerPrint(true)
	, s_editK(_T("1"))
	, s_editG(_T("4"))
	, m_checkColor(true)
	, p_titololower(FALSE)
{
	m_recP = new CSqliteDBRecordSet;
	m_recM = new CSqliteDBRecordSet;
	m_brush = GetSysColorBrush(COLOR_BTNFACE);
	m_ulStyle = L"<ul style=list-style-type:none;><li>";
	m_sep = L"---------------------------------------------------------------------------------------------------------------------------------";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendants::~CAscendants()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_INFO_TEXT, colorGroupInfoText);
	DDX_Control(pDX, IDC_GROUP_ASCENDANTS_TEXT, colorAscendantsText);
	DDX_Control(pDX, IDC_GROUP_GENERATIONS_TEXT, colorGenerationsText);

	DDX_Text(pDX, IDC_NUMOFAF, s_numOfAF);
	DDX_Text(pDX, IDC_NUMOFAM, s_numOfAM);
	DDX_Text(pDX, IDC_NUMOFGF, s_numOfGF);
	DDX_Text(pDX, IDC_NUMOFGM, s_numOfGM);
	DDX_Text(pDX, IDC_NUMOFGDF, s_numOfGDF);
	DDX_Text(pDX, IDC_NUMOFGDM, s_numOfGDM);
	DDX_Control(pDX, IDC_GROUP_LIST_TEXT, colorGroupListText);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bold);
	DDX_Check(pDX, IDC_CHECK_FIRST_BOLD, m_firstBold);
	DDX_Check(pDX, IDC_CHECK_NN, m_NN);
	DDX_Check(pDX, IDC_CHECK_PEER, m_peerPrint);
	DDX_Control(pDX, IDC_GROUP_KEKULE_TEXT, colorGroupKekuleText);
	DDX_Control(pDX, IDC_GROUP_OTHER_TXT, colorGroupOtherText);
	DDX_Text(pDX, IDC_EDIT_K, s_editK);
	DDX_Text(pDX, IDC_EDIT_G, s_editG);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_checkColor);
	DDX_Control(pDX, IDC_LINEAR_TITLE, colorLinearTitle);
	DDX_Check(pDX, IDC_TITOLOLOWER, p_titololower);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendants, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DIRECT_FATHER_ASCENDANTS, &CAscendants::OnClickedDirectFatherAscendants)
	ON_BN_CLICKED(IDC_DIRECT_MOTHER_ASCENDANTS, &CAscendants::OnClickedDirectMotherAscendants)
	ON_BN_CLICKED(IDC_DIRECT_COUPLESF, &CAscendants::OnClickedDirectCouplesF)
	ON_BN_CLICKED(IDC_DIRECT_COUPLESM, &CAscendants::OnClickedDirectCouplesM)
	ON_BN_CLICKED(IDC_CHECK_FIRST_BOLD, &CAscendants::OnClickedCheckFirstBold)
	ON_BN_CLICKED(IDC_PIRAMID, &CAscendants::OnClickedPyramid)
	ON_EN_KILLFOCUS(IDC_EDIT_G, &CAscendants::OnKillfocusEditG)
	ON_EN_KILLFOCUS(IDC_EDIT_K, &CAscendants::OnKillfocusEditK)
	ON_BN_CLICKED(IDC_LINEAR, &CAscendants::OnClickedLinear)
	ON_BN_CLICKED(IDC_LINEAR_SQLITETABLE, &CAscendants::OnClickedLinearSqlitetable)
	ON_BN_CLICKED(IDC_GROUP_OTHER, &CAscendants::OnBnClickedGroupOther)
	ON_BN_CLICKED(IDC_TITOLOLOWER, &CAscendants::OnClickedTitololower)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_show = true;
	m_log = false;		// true = vektorok nyomtat�sa text f�jlba

	CString dragonjpgPathName;
	m_ascendantsPath.Format(L"%s\\ascendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_ascendantsPath, 0))
		_wmkdir(m_ascendantsPath);

	dragonjpgPathName.Format(L"%s\\dragon.jpg", m_ascendantsPath);
	CopyFile(theApp.m_dragonjpg, dragonjpgPathName, false);

	getPeople(m_rowid);
	m_rootName = m_name;
	str.Format(L"%s felmen�inek list�z�sa", m_rootName);
	SetWindowTextW(str);

	colorGroupInfoText.SetTextColor(BLUE);;
	colorGenerationsText.SetTextColor(BLUE);
	colorAscendantsText.SetTextColor(BLUE);;
	colorGroupListText.SetTextColor(GREEN);
	colorGroupKekuleText.SetTextColor(LIGHTRED);
	colorGroupOtherText.SetTextColor(LIGHTRED);
	colorLinearTitle.SetTextColor(LIGHTRED);

	create_vGKR();			// beh�z�s lista
	if (vGKR.size() < 2)
	{
		AfxMessageBox(L"Nincs egyetlen felmen�je sem!");
		CDialogEx::OnCancel();
	}

	parameters();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vGKR
// {
// CString	rowid;					rowid
// CString	name;				n�v
// CString	birth;				sz�let�s
// CString	death;				hal�l
// int		sex_id;
// int		g;					gener�ci�
// int		gcnt				geber�ci�n bel�li sorsz�m
// _int64	k;					Kekule sz�m
// int		cnt;				a rowid ism�tl�d�s�nek sorsz�ma 
// int		cntMax;			ism�tl�d�sek sz�ma
// int		ixC					a gyereke indexe a vGKR-ben;
// int		ixF					apa indexe a vGKR-ben
//	};
void CAscendants::create_vGKR()
{
	
	if( m_log )
		fs_vGKR = theApp.openTextFile(&flvGKR, L"vGKR", L"w+");  // log f�jl

	m_term = false;
	CProgressWnd wndP(NULL, L"Felmen�k keres�se folyik...");
	wndP.GoModal();

	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	int		g = 0;		// gener�ci�k sorsz�ma
	_int64	k = 1;		// kek�l� sz�m
	int		gcnt = 1;	// az aktu�lis gener�ci�ban l�tez� emberek sz�ma
	int		i1 = 0;
	int		max;
	int		cnt;
	int		gPrev = g;
	_int64	gOrder = 1;
	bool	husband;


	vGKR.clear();
	gkr.g = g;
	gkr.k = k;
	gkr.gABC = TCHAR('A');
	gkr.rowidC = L"";
	gkr.rowid = m_rowid;		// saj�t rowidja
	gkr.familyName = m_familyName;
	gkr.name = m_name;
	gkr.cnt = 0;
	gkr.ixC = -1;				// gyerek�nek indexe
	gkr.ixSp = -1;				// h�zast�rsa indexe
	gkr.ixF = 0;				// apja indexe
	gkr.ixM = 0;				// anya indexe
	gkr.gcnt = gcnt;
	gkr.sex_id = m_sex_id;
	gkr.cntMax = 0;
	gkr.numOfSp = 0;
	gkr.birth = m_birth;
	gkr.death = m_death;
	gkr.x = 0;
	gkr.y = 0;
	gkr.linenumber = m_linenumber;
	vGKR.push_back(gkr);
	g = 1;


	gPrev = 1;

	while (gcnt)									// a gener�ci�ban l�v� embereken megy v�gig, hogy felt�ltse az k�vetkez� (el�z�) gener�ci�t az ap�kkal �s any�kkal
	{
		gcnt = 0;								// az aktu�lis gener�ci�ban l�tez� emberek sz�ma
		max = vGKR.size();						// vGKR.size a ciklusban v�ltozik!! Ez�rt kell a max!

		husband = false;
		for (int i = i1; i < max; ++i)			// az el�z� gener�ci� vGKR bejegyz�sein v�gigszalad. Ebben keresi a sz�l�ket.
		{
			k = vGKR.at(i).k;				// 
			getParentsId(i);
			if (!m_father_id.IsEmpty())		// l�tez� apa
			{
				if (g == gPrev)
					++gOrder;
				else
					gOrder = 1;
				++gcnt;
				getPeople(m_father_id);
				gkr.rowidC = vGKR.at(i).rowid;
				gkr.rowid = m_father_id;
				gkr.g = g;
				gkr.k = k * 2;
				gkr.cnt = 0;
				gkr.ixC = 0;
				gkr.ixF = 0;
				gkr.ixSp = 0;
				gkr.gcnt = gcnt;
				gkr.familyName = m_familyName;
				gkr.name = m_name;
				gkr.sex_id = m_sex_id;
				gkr.cntMax = 1;
				gkr.numOfSp = 0;
				gkr.gABC = get_gABC(g);
				gkr.x = 0;
				gkr.y = 0;
				gkr.birth = m_birth;
				gkr.death = m_death;
				gkr.linenumber = m_linenumber;
				vGKR.push_back(gkr);
				husband = true;
			}
			if (!m_mother_id.IsEmpty())		// l�tez� anya
			{
				++gcnt;
				++gOrder;
				getPeople(m_mother_id);
				gkr.rowidC = vGKR.at(i).rowid;
				gkr.rowid = m_mother_id;
				gkr.g = g;
				gkr.k = k * 2 + 1;
//				gkr.gOrder = gOrder;
				gkr.cnt = 0;
				gkr.ixC = 0;
				gkr.ixF = 0;
				gkr.ixSp = 0;
				gkr.gcnt = gcnt;
				gkr.name = m_name;
				gkr.sex_id = m_sex_id;
				gkr.cntMax = 1;
				gkr.numOfSp = 0;
				gkr.gABC = get_gABC(g);
				gkr.x = 0;
				gkr.y = 0;
				gkr.linenumber = m_linenumber;
				if (husband)
				{
					gkr.numOfSp = 1;
					gkr.ixSp = vGKR.size()-1;					// az � f�rj�nek indexe
					++vGKR.at(vGKR.size() - 1).numOfSp;			// ha az ap�nak van h�zast�rsa
					vGKR.at(vGKR.size() - 1).ixSp = vGKR.size();	// f�rje h�zast�rs�nak indexe saj�t indexe
				}
				vGKR.push_back(gkr);
			}
			gPrev = g;
		}
		++g;						// az �j gener�ci�t fogja v�gign�zni
		i1 = vGKR.size() - gcnt;		// az most kigy�jt�tt gener�ci� els� bejegyz�s�nek indexe 
#ifndef _DEBUG
		str.Format(L"Gener�ci�: %d Kekule sz�m: %I64d", g, gkr.k);
		wndP.SetText(str);
#endif
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			m_term = true;
			break;
		}
	}
	wndP.DestroyWindow();

	CProgressWnd wndP2(NULL, L"T�bbsz�r el�fordul� felmen�k sorsz�moz�sa...");
	wndP2.GoModal();
	wndP2.SetRange(0, vGKR.size());
	wndP2.SetStep(1);
	CString rowid;
	int cntPrev;
	for (int i = 0; i < vGKR.size(); ++i)
	{
		cntPrev = 1;
		rowid = vGKR.at(i).rowid;
		vGKR.at(i).original = i;	// eredeti sorrend		
		if (vGKR.at(i).cnt == 0)	// a m�r megvizsg�lt felmen�t nem vizsg�lja �jra
		{
			for (int j = i + 1; j < vGKR.size(); ++j)
			{
				if (vGKR.at(j).rowid == rowid && !vGKR.at(j).cnt)
				{
					++cntPrev;
					vGKR.at(i).cnt = 1;								// els� el�fordul�s 
					vGKR.at(j).cnt = cntPrev;						// tov�bbi el�fordul�s
				}
			}
			if (cntPrev != 1)										// ha volt t�bbsz�r�s el�fordul�s, akkor beteszio azok sz�m�t a cntMax.ba
			{
				vGKR.at(i).cntMax = cntPrev;
				for (int j = i + 1; j < vGKR.size(); ++j)
				{
					if (vGKR.at(j).rowid == rowid)
						vGKR.at(j).cntMax = cntPrev;

				}
			}
		}
		wndP2.StepIt();
		wndP2.PeekAndPump();
		if (wndP2.Cancelled()) break;
	}
	wndP2.DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::parameters()
{

	m_numOfG = vGKR.at(vGKR.size() - 1).g;		// gener�ci�k sz�ma
	m_numOfA = vGKR.size();						// felmen�k sz�ma
	m_maxK = vGKR.at(vGKR.size() - 1).k;

	int g;
	_int64 k;
	_int64 n;
	_int64 m;
	/*
		fwprintf(fl, L"Felmen�k sz�ma a g gener�ci�ban:\n");
		for (int g = 0; g < m_numOfG; ++g)
		{
			n = pow(2, g + 1) - pow(2, g);
			m = pow(2, g);
			fwprintf(fl, L"%3d %I64d %I64d\n", g, n, m);

		}


		fwprintf(fl, L"Apai felmen�k kekule sz�ma a g gener�ci�ban:\n");
		for (int g = 1; g < m_numOfG; ++g)
		{
			n = pow(2, g);
			m = n + n / 2 - 1;
			fwprintf(fl, L"%3d %I64d-%I64d\n", g, n, m);
		}

		fwprintf(fl, L"Anyai felmen�k kekule sz�ma a g gener�ci�ban:\n");
		for (int g = 1; g < m_numOfG; ++g)
		{
			m = pow(2, g);
			n = m + m / 2;
0			m = pow(2, g + 1) - 1;

			fwprintf(fl, L"%3d %I64d-%I64d\n", g, n, m);
		}
	*/
	// Visszafele keresi a vGKR-ben azt a k sz�mot, ami a g-hez tartoz� min �s max k�z� esik
	// Ez a g az apai gener�ci�k sz�ma
	m_numOfGF = 0;
	for (int i = vGKR.size() - 1; i > 0; --i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;
		n = pow(2, g);
		m = n + n / 2 - 1;
		if (n <= k && k <= m)
		{
			m_numOfGF = g;
			break;
		}
	}
	//	fwprintf(fl, L"Apai felmen�k gener�ci� sz�ma: %d\n", m_numOfGF);

		// Visszafele keresi a vGKR-ben azt a k sz�mot, ami a g-hez tartoz� min �s max k�z� esik
		// Ez a g az anyai gener�ci�k sz�ma
	m_numOfGM = 0;
	for (int i = vGKR.size() - 1; i > 0; --i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;

		m = pow(2, g);
		n = m + m / 2;
		m = pow(2, g + 1) - 1;
		if (n <= k && k <= m)
		{
			m_numOfGM = g;
			break;
		}
	}
	//	fwprintf(fl, L"Anyai felmen�k gener�ci� sz�ma: %d\n", m_numOfGM);

	m_numOfGDF = 0;
	for (int i = vGKR.size() - 1; i > 0; --i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;

		n = pow(2, g);
		if (k == n)
		{
			m_numOfGDF = g;
			break;
		}
	}
	//	fwprintf(fl, L"Egyenes �gi apai felmen�k gener�ci� sz�ma: %d\n", m_numOfGDF);

	m_numOfGDM = 0;
	for (int i = vGKR.size() - 1; i > 0; --i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;

		m = pow(2, g);
		n = m + m / 2;
		if (k == n)
		{
			m_numOfGDM = g;
			break;
		}
	}
	//	fwprintf(fl, L"Egyenes �gi apai felmen�k gener�ci� sz�ma: %d\n", m_numOfGDM);



	m_numOfAF = 0;
	m_numOfAM = 0;
	for (int i = 1; i < vGKR.size(); ++i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;
		n = pow(2, g);
		m = n + n / 2 - 1;
		if (n <= k && k <= m)
		{
			++m_numOfAF;
		}
//	}

//	for (int i = 1; i < vGKR.size(); ++i)
//	{
//		g = vGKR.at(i).g;
//		k = vGKR.at(i).k;

		m = pow(2, g);
		n = m + m / 2;
		m = pow(2, g + 1) - 1;
		if (n <= k && k <= m)
		{
			++m_numOfAM;
		}
	}
	s_numOfGF.Format(L"%d", m_numOfGF);
	s_numOfGM.Format(L"%d", m_numOfGM);
	s_numOfGDF.Format(L"%d", m_numOfGDF);
	s_numOfGDM.Format(L"%d", m_numOfGDM);

	s_numOfAF.Format(L"%d", m_numOfAF);
	s_numOfAM.Format(L"%d", m_numOfAM);

	getParameters();

	m_editK = _wtoi(s_editK);
	m_editG = _wtoi(s_editG);

	s_editG.Format(L"%d", m_numOfGF);
	GetDlgItem(IDC_EDIT_G)->SetWindowTextW(s_editG);
	UpdateData(TOSCREEN);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::saveParameters()
{
	theApp.WriteProfileString(L"dragon", L"s_editK", s_editK);
	theApp.WriteProfileString(L"dragon", L"s_editG", s_editG);
	theApp.WriteProfileInt(L"dragon", L"m_firstBold", m_firstBold);
	theApp.WriteProfileInt(L"dragon", L"m_NN", m_NN);
	theApp.WriteProfileInt(L"dragon", L"m_peerPrint", m_peerPrint);
	theApp.WriteProfileInt(L"dragon", L"m_checkColor", m_checkColor);
	theApp.WriteProfileInt(L"dragon", L"p_titololower", p_titololower);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::getParameters()
{
	s_editK = theApp.GetProfileString(L"dragon", L"s_editK", L"");
	s_editG = theApp.GetProfileString(L"dragon", L"s_editG", L"");
	m_firstBold = theApp.GetProfileInt(L"dragon", L"m_firstBold", 1);
	m_NN = theApp.GetProfileInt(L"dragon", L"m_NN", 1);
	m_peerPrint = theApp.GetProfileInt(L"dragon", L"m_peerPrint", 1);
	m_checkColor = theApp.GetProfileInt(L"dragon", L"m_checkColor", 1);
	p_titololower = theApp.GetProfileInt(L"dragon", L"p_titololower", p_titololower);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet: k - ez lenne az i. rowid apj�nak a kekeule sz�ma
//			i - a VGKR indexe, amelybne van az ember�nk rowid-ja.
bool CAscendants::getParentsId(int i)
{
	CString rowid = vGKR.at(i).rowid;
	m_command.Format(L"SELECT father_id, mother_id FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	m_father_id = m_recP->GetFieldString(0);
	m_mother_id = m_recP->GetFieldString(1);
	if (m_father_id == L"0") m_father_id.Empty();
	if (m_mother_id == L"0") m_mother_id.Empty();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::getPeople(CString rowid)
{
	m_command.Format(L"SELECT last_name, first_name, sex_id, birth_date, death_date, linenumber FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	m_familyName = m_recP->GetFieldString(0);
	m_name.Format(L"%s %s", m_recP->GetFieldString(0), m_recP->GetFieldString(1));
	m_birth = m_recP->GetFieldString(3);
	m_death = m_recP->GetFieldString(4);
	m_sex_id = _wtoi(m_recP->GetFieldString(2));
	m_linenumber = m_recP->GetFieldString(5);
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendants::queryP(CString command)
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
BOOL CAscendants::queryM(CString command)
{
	if (m_recM->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recM->GetLastError());
		theApp.message(L"Query", str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnPaint()
{
	CPaintDC dc(this);

	COLORREF	color = RGB(0, 144, 216);

	colorFrame(&dc, IDC_GROUP_INFO, LIGHTBLUE);
	colorFrame(&dc, IDC_GROUP_GENERATIONS, LIGHTBLUE);
	colorFrame(&dc, IDC_GROUP_ASCENDANTS, LIGHTBLUE);

	colorFrame(&dc, IDC_GROUP_LIST, LIGHTGREEN);

	colorFrame(&dc, IDC_GROUP_KEKULE, LIGHTRED);

	colorFrame(&dc, IDC_GROUP_OTHER, LIGHTRED);
	colorFrame(&dc, IDC_BOX_LINEAR, LIGHTRED);
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


	rc.TopLeft().x -= 1;	// a sz�rke keret mellett kell rajzolni, mert k�l�nben a keret fel�l�rja a sz�nes n�gyzetet
	rc.BottomRight().x += 1;	// ez szebb, mint az al�bbi vonalas megold�s, de kifeh�r�tia n�gysz�g�n bel�l �s a sz�vegek h�tt�rsz�ne sz�rke marad.
	rc.TopLeft().y += 5;	// Ez�rt m�gis szebba vonalas rajzol�s

	COLORREF colour = ::GetSysColor(COLOR_3DFACE);
	HBRUSH hbr = CreateSolidBrush(colour);
	dc->SelectObject(hbr);	// hogy a h�tt�r olyan sz�rke legyen, mint a benne l�v� control-ok (sz�veg, gomb )

	dc->Rectangle(&rc);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnKillfocusEditG()
{
	UpdateData(FROMSCREEN);
	m_editG = _wtoi(s_editG);
}
void CAscendants::OnKillfocusEditK()
{
	UpdateData(FROMSCREEN);
	m_editK = _wtoi(s_editK);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedCheckFirstBold()
{
	m_firstBold = !m_firstBold;
	if (m_firstBold == true)
	{
		m_bold = false;
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(true);
	}
	UpdateData(TOSCREEN);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int == 0 fh-ba �r (piramid)
// int == 1 fhLinRep-be �r  (teljes line�ris lista)
// int == 2 fhLinNoRep-be �r	( line�ris lista ism�tl�d�k kihagy�s�val )
// int == 3 fhLinTab
// int == 4 fhLinTabNoRep
void CAscendants::printOnly(CString str, int i)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	switch (i)
	{
	case 0:
		fwprintf(fh, L"%s\n", str);
		fflush(fh);
		break;
	case 1:
		fwprintf(fhLinListRep, L"%s\n", str);
		fflush(fhLinListRep);
		break;
	case 2:
		fwprintf(fhLinListNoRep, L"%s\n", str);
		fflush(fhLinListNoRep);
		break;
	case 3:
		fwprintf(fhLinTabRep, L"%s\n", str);
		fflush(fhLinTabRep);
		break;
	case 4:
		fwprintf(fhLinTabNoRep, L"%s\n", str);
		fflush(fhLinTabNoRep);
		break;
	case 5:
		fwprintf(fhSticky, L"%s\n", str);
		fflush(fhSticky);
		break;
	case 6:
		fwprintf(fhfixHeaderRep, L"%s\n", str);
		fflush(fhfixHeaderRep);
		break;
	case 7:
		fwprintf(fhfixHeaderNoRep, L"%s\n", str);
		fflush(fhfixHeaderNoRep);
		break;
	case 8:
		fwprintf(fhfixHeaderRep, L"%s\n", str);
		fflush(fhfixHeaderRep);
		break;
	case 9:
		fwprintf(fhfixHeaderNoRep, L"%s\n", str);
		fflush(fhfixHeaderNoRep);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print0(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s\n", str);
	fflush(fh);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print1(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fhLinListRep, L"%s\n", str);
	fflush(fhLinListRep);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print2(CString str)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(fhLinListNoRep, L"%s\n", str);
	fflush(fhLinListNoRep);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print3(CString str)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(fhLinTabRep, L"%s\n", str);
	fflush(fhLinTabRep);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::print4(CString str)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(fhLinTabNoRep, L"%s\n", str);
	fflush(fhLinTabNoRep);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printS(CString str)
{
	//	str.Replace('|', '\'');
	str = UnicodeToUtf8(str);
	fwprintf(fhSticky, L"%s\n", str);
	fflush(fhSticky);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printvGKR()
{
	int gPrev;
	GKR gkr;		// egy ember g-k-r-p adatai	
	GKR gkrC;
	GKR gkrF;
	int j;
	int x;
	fwprintf(flvGKR, L"%10s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s|\
				   %7s %-30s %-3s %8s %2s %4s %10s %4s %4s %10s %10s\n", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"num", L"cnt", L"ixC", L"ixF", \
		L"ix", L"name", L"sex", L"rowid", L"g", L"gcnt", L"k", L"num", L"cnt", L"ixC", L"ixF");
	fwprintf(flvGKR, L"-------------------------------------------------------------------------------------------\n");

	gPrev = vGKR.at(0).g;
	gkr = vGKR.at(0);
	str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %4d %10d %4d %10d\n", \
		0, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.cntMax, gkr.gcnt, gkr.k, gkr.cnt, gkr.ixC);
	fwprintf(flvGKR, str);

	for (int i = 1; i < vGKR.size(); ++i)
	{
		gkr = vGKR.at(i);

		j = gkr.ixC;			// gyereke
		if (j < 0 || j > vGKR.size())
		{
			x = 1;
			return;
		}
		gkrC = vGKR.at(j);

		j = gkr.ixF;
		if (j < 0 || j > vGKR.size())
		{
			x = 1;
			return;
		}
		gkrF = vGKR.at(j);

		if (gkr.g != gPrev)
			fwprintf(flvGKR, L"-------------------------------------------------------------------------------------------\n");
		str.Format(L"%5d %4d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d|\
					 %5d %-30s %3d %8s %2d %4d %10d %4d %4d %10d %10d\n", \
			i, gkr.cntMax, gkr.name, gkr.sex_id, gkr.rowid, gkr.g, gkr.gcnt, gkr.k, gkr.cntMax, gkr.cnt, gkr.ixC, gkr.ixF, \
			j, gkrC.name, gkr.sex_id, gkrC.rowid, gkrC.g, gkrC.gcnt, gkrC.k, gkr.cntMax, gkrC.cnt, gkrC.ixC, gkr.ixF);
		fwprintf(flvGKR, str);
		gPrev = gkr.g;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::printTopContainer(CString title, int which)
{
	CString yesno;


	str.Format(L"<b><center>%s</center></b><br>", title);
	printOnly(str, which );

//	printOnly(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>", which);

	str.Format(L"<b><center>Dragon v. %s</center></b>", theApp.m_version);
	printOnly(str, which);


	printOnly(L"<div class=\"top-container\">", which);
	printOnly(L"<pre>", which);


	int l = -37;
	CString dateHtml = theApp.getFileCreationDate(theApp.m_htmlPathName);
	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

	str.Format(L"%*s %s", l, L"Alapk�nyvt�r:", theApp.m_dbFolderPath);
	printOnly(str, which);
	if (theApp.m_inputMode == GAHTML)
	{
		str.Format(L"%*s %s %s", l, L"Bementi f�jl k�sz�lt:", dateHtml, theApp.m_htmlFileName);
		printOnly(str, which);
	}
	str.Format(L"%*s %s %s", l, L"Adatb�zis k�sz�lt:", dateDB, theApp.m_dbFileName);
	printOnly(str, which);
	if (theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvas�s programverzi�ja:", theApp.m_inputVersion);
		printOnly(str, which);
	}
	if (theApp.m_uniteVersion.IsEmpty())
	{
		str = "A bejegyz�sek nincsenek �sszevonva!!!\n";
		printOnly(str, which);
	}
	if (theApp.m_uniteVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"�sszevon�s programverzi�ja:", theApp.m_uniteVersion);
		printOnly(str, which);
	}

	str.Format(L"%*s %sn", l, L"Lista k�sz�lt:", theApp.getPresentDateTime());
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Apai gener�ci�k sz�ma:", m_numOfGF);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Anyai gener�ci�k sz�ma:", m_numOfGM);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Egyenes�gi apai genr�ci�k sz�ma:", m_numOfGDF);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Egyenes�gi anyai gener�ci�k sz�ma:", m_numOfGDM);
	printOnly(str, which);
	str.Format(L"%*s %d", l, L"Apai felmen�k sz�ma:", m_numOfAF);
	printOnly(str, which);
	str.Format(L"%*s %d\n", l, L"Anyai felmen�k sz�ma:", m_numOfAM);
	printOnly(str, which );
	printOnly(L"</pre>", which);
	printOnly(L"</div>", which);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedLinearSqlitetable()
{
	UpdateData(FROMSCREEN);

	m_show = false;
	OnClickedPyramid();

	CAscendantsLinearTable dlg;
	dlg.p_titololower = p_titololower;
	dlg.m_indentedFile = m_piramidFile;
	dlg.m_name = m_rootName;
	dlg.vGKR = &vGKR;

	dlg.m_bold = m_bold;
	dlg.m_firstBold = m_firstBold;
	dlg.m_peerPrint = m_peerPrint;
	dlg.s_editK = s_editK;
	dlg.s_editG = s_editG;
	dlg.m_checkColor = m_checkColor;
	dlg.m_ascendantsPath = m_ascendantsPath;


	dlg.DoModal();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedTitololower()
{
	p_titololower = !p_titololower;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnBnClickedGroupOther()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


