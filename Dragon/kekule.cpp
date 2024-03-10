#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "kekule.h"

IMPLEMENT_DYNAMIC(CKekule, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKekule::CKekule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEKULE, pParent)
	, m_numOfFG(_T(""))
	, m_numOfFA(_T(""))
	, s_numOfAF(_T(""))
	, s_numOfAM(_T(""))
	, s_numOfGF(_T(""))
	, s_numOfGM(_T(""))
	, s_numOfGDF(_T(""))
	, s_numOfGDM(_T(""))
	, m_bold(true)
	, m_NN(false)
	, m_peerPrint(true)
	, s_editK(_T("1"))
	, s_editG(_T("4"))
	, m_checkColor(false)
{
	m_recP = new CSqliteDBRecordSet;
	m_recM = new CSqliteDBRecordSet;

	m_brush = GetSysColorBrush(COLOR_BTNFACE);
	m_ulStyle = L"<ul style=list-style-type:none;><li>";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKekule::~CKekule()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::DoDataExchange(CDataExchange* pDX)
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
	DDX_Check(pDX, IDC_CHECK_NN, m_NN);
	DDX_Check(pDX, IDC_CHECK_PEER, m_peerPrint);
	DDX_Control(pDX, IDC_GROUP_KEKULE_TEXT, colorGroupKekuleText);
	DDX_Control(pDX, IDC_GROUP_OTHER_TXT, colorGroupOtherText);
	DDX_Text(pDX, IDC_EDIT_K, s_editK);
	DDX_Text(pDX, IDC_EDIT_G, s_editG);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_checkColor);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CKekule, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DIRECT_FATHER_ASCENDANTS, &CKekule::OnClickedDirectFatherAscendants)
	ON_BN_CLICKED(IDC_DIRECT_MOTHER_ASCENDANTS, &CKekule::OnClickedDirectMotherAscendants)
	ON_BN_CLICKED(IDC_DIRECT_COUPLESF, &CKekule::OnClickedDirectCouplesF)
	ON_BN_CLICKED(IDC_DIRECT_COUPLESM, &CKekule::OnClickedDirectCouplesM)
	ON_BN_CLICKED(IDC_PIRAMID, &CKekule::OnClickedPyramid)
	ON_EN_KILLFOCUS(IDC_EDIT_G, &CKekule::OnKillfocusEditG)
	ON_EN_KILLFOCUS(IDC_EDIT_K, &CKekule::OnKillfocusEditK)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CKekule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_peerPrint = theApp.GetProfileInt(L"dragon", L"m_peer", 1);
	m_bold = theApp.GetProfileInt(L"dragon", L"m_bold", 1);
	m_NN = theApp.GetProfileInt(L"dragon", L"m_NN", 0);
	s_editG = theApp.GetProfileString(L"dragon", L"s_editG", L"4");
	s_editK = theApp.GetProfileString(L"dragon", L"s_editK", L"1");

	getPeople(m_rowid);
	str.Format(L"%s felmenõinek listázása", m_name);
	SetWindowTextW(str);

	colorGroupInfoText.SetTextColor(BLUE);;
	colorGenerationsText.SetTextColor(BLUE);
	colorAscendantsText.SetTextColor(BLUE);;

	colorGroupListText.SetTextColor(GREEN);

	colorGroupKekuleText.SetTextColor(LIGHTRED);

	colorGroupOtherText.SetTextColor(LIGHTRED);


//	CString fileName(L"ascendants");
//	fileSpec = theApp.openTextFile(&fl, fileName, L"w+");
	
	fillVector();
	if (vGKR.size() < 2)
	{
		AfxMessageBox(L"Nincs egyetlen felmenõje sem!");
		fclose(fl);
		CDialogEx::OnCancel();
	}

	calcNotableNumbers();
	s_editG.Format(L"%d", m_numOfGF);
	GetDlgItem(IDC_EDIT_G)->SetWindowTextW(s_editG);

//	fclose(fl);
//	ShellExecute(NULL, L"open", fileSpec, NULL, NULL, SW_SHOWNORMAL);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::fillVector()
{
	m_repeated = 0;
	m_term = false;
	CProgressWnd wndP(NULL, L"Felmenõk keresése folyik...");
	wndP.GoModal();

	CK::GKR gkr;		// egy ember g-k-r-p adatai	
	int		g = 0;		// generációk sorszáma
	_int64	k = 1;		// kekülé szám
	int		cnt = 1;		// az aktuális generációban létezõ emberek száma
	int		i1 = 0;
	int		max;

	vGKR.clear();
	gkr.g = g;
	gkr.k = k;
	gkr.r = m_rowid;
	gkr.repeated = 0;
	gkr.cnt = 0;
	vGKR.push_back(gkr);
	g = 1;
	while (cnt)
	{
		cnt = 0;								// az aktuális generációban létezõ emberek száma
		max = vGKR.size();						// vGKR.size a ciklusban változik!! Ezért kell a max!


		for (int i = i1; i < max; ++i)			// az elõzõ generáció vGKR bejegyzésein végigszalad. Ebben keresi a szülõket.
		{
			k = vGKR.at(i).k;				// 
			getParentsId(k * 2, i);
			if (!m_father_id.IsEmpty())		// létezõ apa
			{
				++cnt;
				getPeople(m_father_id);
				gkr.g = g;
				gkr.k = k * 2;
				gkr.r = m_father_id;
				gkr.repeated = getRepeated(m_father_id );
				vGKR.push_back(gkr);
			}
			if (!m_mother_id.IsEmpty())		// létezõ anya
			{
				++cnt;
				getPeople(m_mother_id);
				gkr.g = g;
				gkr.k = k * 2 + 1;
				gkr.r = m_mother_id;
				gkr.repeated = getRepeated(m_mother_id);
				vGKR.push_back(gkr);
			}
		}
		++g;						// az új generációt fogja végignézni
		i1 = vGKR.size() - cnt;		// az most kigyûjtött generáció elsõ bejegyzésének indexe 
#ifndef _DEBUG
		str.Format(L"Generáció: %d Kekule szám: %I64d", g, gkr.k );
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

	// az elsõ ismételt felmenõt besorszámozza, azaz a cnt-t egyesével nõveli a felmenõiben 
	CString rowid;
	for (int i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).cnt == 1)
		{
			cnt = 1;
			rowid = vGKR.at(i).r;
			for (int j = i + 1; j < vGKR.size(); ++j)
			{
				if (vGKR.at(j).r == rowid)
				{
					++cnt;
					vGKR.at(j).cnt = cnt;
				}
			}
		}
	}

	// a gyökerek root változójába kiterjeszti a repetaed-et
	int repeated;
	cnt = 1;
	for (int i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).cnt == 1 )
		{
			k = vGKR.at(i).k;
			repeated = vGKR.at(i).repeated;
			vGKR.at(i).root = repeated;
			k = k / 2;
			for (int j = i-1; j > 0 && k > 1; --j)
			{
				if (vGKR.at(j).k == k)
				{
					vGKR.at(j).root = repeated;
					k = k / 2;
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::calcNotableNumbers()
{
	m_numOfG = vGKR.at(vGKR.size() - 1).g;		// generációk száma
	m_numOfA = vGKR.size();						// felmenõk száma
	m_maxK = vGKR.at(vGKR.size() - 1).k;

	int g;
	_int64 k;
	_int64 n;
	_int64 m;
/*
	fwprintf(fl, L"Felmenõk száma a g generációban:\n");
	for (int g = 0; g < m_numOfG; ++g)
	{
		n = pow(2, g + 1) - pow(2, g);
		m = pow(2, g);
		fwprintf(fl, L"%3d %I64d %I64d\n", g, n, m);

	}


	fwprintf(fl, L"Apai felmenõk kekule száma a g generációban:\n");
	for (int g = 1; g < m_numOfG; ++g)
	{
		n = pow(2, g);
		m = n + n / 2 - 1;
		fwprintf(fl, L"%3d %I64d-%I64d\n", g, n, m);
	}

	fwprintf(fl, L"Anyai felmenõk kekule száma a g generációban:\n");
	for (int g = 1; g < m_numOfG; ++g)
	{
		m = pow(2, g);
		n = m + m / 2;
		m = pow(2, g + 1) - 1;

		fwprintf(fl, L"%3d %I64d-%I64d\n", g, n, m);
	}
*/
	// Visszafele keresi a vGKR-ben azt a k számot, ami a g-hez tartozó min és max közé esik
	// Ez a g az apai generációk száma
	m_numOfGF = 0;
	for (int i = vGKR.size() -1; i > 0; --i)
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
//	fwprintf(fl, L"Apai felmenõk generáció száma: %d\n", m_numOfGF);

	// Visszafele keresi a vGKR-ben azt a k számot, ami a g-hez tartozó min és max közé esik
	// Ez a g az anyai generációk száma
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
//	fwprintf(fl, L"Anyai felmenõk generáció száma: %d\n", m_numOfGM);

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
//	fwprintf(fl, L"Egyenes ági apai felmenõk generáció száma: %d\n", m_numOfGDF);

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
//	fwprintf(fl, L"Egyenes ági apai felmenõk generáció száma: %d\n", m_numOfGDM);


	
	m_numOfAF = 0;
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
	}
//	fwprintf(fl, L"Apai felmenõk száma: %d\n", m_numOfAF);

	m_numOfAM = 0;
	for (int i = 1; i < vGKR.size(); ++i)
	{
		g = vGKR.at(i).g;
		k = vGKR.at(i).k;

		m = pow(2, g);
		n = m + m / 2;
		m = pow(2, g + 1) - 1;
		if (n <= k && k <= m)
		{
			++m_numOfAM;
		}
	}
//	fwprintf(fl, L"Anyai felmenõk száma: %d\n", m_numOfAM);


	s_numOfGF.Format(L"%d", m_numOfGF);
	s_numOfGM.Format(L"%d", m_numOfGM);
	s_numOfGDF.Format(L"%d", m_numOfGDF);
	s_numOfGDM.Format(L"%d", m_numOfGDM);

	s_numOfAF.Format(L"%d", m_numOfAF);
	s_numOfAM.Format(L"%d", m_numOfAM);

	m_editK = _wtoi(s_editK);
//	s_editG = (m_numOfGF < m_numOfGM) ? s_numOfGM : s_numOfGF;  // A defeult G az apa-anya generációinak maximuma
	m_editG = _wtoi(s_editG);

	UpdateData(TOSCREEN);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bemenet: k - ez lenne az i. rowid apjának a kekeule száma
//			i - a VGKR indexe, amelybne van az emberünk rowid-ja.
bool CKekule::getParentsId( _int64 k, int i )   
{
	CString rowid = vGKR.at(i).r;
	m_command.Format(L"SELECT father_id, mother_id FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	m_father_id = m_recP->GetFieldString(0);
	m_mother_id = m_recP->GetFieldString(1);
	if (m_father_id == L"0") m_father_id.Empty();
	if (m_mother_id == L"0") m_mother_id.Empty();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// input:	rowid
//			ix - a vizsgált felemenõ vGKR indexe
// output: az ismétlés sorszáma, has volt ismétlés, 0: nem volt ismétlés
int CKekule::getRepeated(CString rowid )
{
	

	int repeated = 0;
	if (rowid == L"40656")
		repeated = 0;
	for (int i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).r == rowid)
		{
			repeated = vGKR.at(i).repeated;		// már jelöltük ismétlõdõnek, ugyanzt a sorszámot adja vissza
			if (repeated == 0 )					// még nem jelöltük ismétlõdõnek
			{
				++m_repeated;					// új ismétlõdõ sorszámot kap 
				repeated = m_repeated;
				vGKR.at(i).repeated = repeated;	// az elsõ elõfordulást is bejelöljük 
				vGKR.at(i).cnt = 1;
				break;
			}
		}
	}
	return repeated;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CKekule::getPeople(CString rowid)
{
	m_command.Format(L"SELECT last_name, first_name FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return false;
	m_name.Format(L"%s %s", m_recP->GetFieldString(0), m_recP->GetFieldString(1));
}
/////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////
BOOL CKekule::queryP(CString command)
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
BOOL CKekule::queryM(CString command)
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
void CKekule::OnPaint()
{
	CPaintDC dc(this);

	COLORREF	color = RGB(0, 144, 216);
			
	colorFrame(&dc, IDC_GROUP_INFO, LIGHTBLUE );
	colorFrame(&dc, IDC_GROUP_GENERATIONS, LIGHTBLUE );
	colorFrame(&dc, IDC_GROUP_ASCENDANTS, LIGHTBLUE );
	
	colorFrame(&dc, IDC_GROUP_LIST, LIGHTGREEN);

	colorFrame(&dc, IDC_GROUP_KEKULE, LIGHTRED );

	colorFrame(&dc, IDC_GROUP_OTHER, LIGHTRED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::colorFrame(CPaintDC* dc, int IDC, COLORREF color)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CKekule::openHtml(CString filename, CString title)
{
	CString yesno;
	CString ascendantsPath;
	CString dragonjpgPathName;
	ascendantsPath.Format(L"%s\\ascendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(ascendantsPath, 0))
		_wmkdir(ascendantsPath);

	dragonjpgPathName.Format(L"%s\\dragon.jpg", ascendantsPath );
	CopyFile(theApp.m_dragonjpg, dragonjpgPathName, false );
	m_htmlFile.Format(L"%s\\%s_%s.html", ascendantsPath, filename, getTimeTag());

	if (!openFileSpec(&fh, m_htmlFile, L"w+")) return false;

	CString program;
	program.Format(L"Dragon v. %s", theApp.m_version);

	CString head;
	head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>";
	print(head);
	str.Format( L"<b><center>%s</center><pre></b>", title );
	print(str);
	print(L"<center><img src = \"dragon.jpg\" alt = \"Dragon\" width = \"100\" height = \"70\"></center>");
	str.Format(L"<b><center>%s</center><pre></b>", program);
	print(str);

	int l = -37;
	CString dateHtml = theApp.getFileCreationDate( theApp.m_htmlPathName );
	CString dateDB = theApp.getFileCreationDate(theApp.m_dbPathName);

//	str.Format(L"%*s %s", l, L"Program:", program);
//	print(str);
	str.Format(L"%*s %s", l, L"Alapkönyvtár:", theApp.m_dbFolderPath);
	print(str);
	if (theApp.m_inputMode == GAHTML)
	{
		str.Format(L"%*s %s %s", l, L"Bementi fájl készült:", dateHtml, theApp.m_htmlFileName);
		print(str);
	}
	str.Format(L"%*s %s %s", l, L"Adatbázis készült:", dateDB, theApp.m_dbFileName);
	print(str);
	if (theApp.m_inputVersion != theApp.m_version)
	{
		str.Format(L"%*s %s", l, L"Beolvasás programverziója:", theApp.m_inputVersion);
		print(str);
	}
	if (theApp.m_uniteVersion.IsEmpty())
		str = "A bejegyzések nincsenek összevonva!!!";
	else
		if (theApp.m_snameEnough)
			yesno = L"igen";
		else
			yesno = L"nem";
		str.Format(L"%*s %s", l, L"Azonos nevõ házaspárok összevonása:", yesno);
		print(str);
		if (theApp.m_uniteVersion != theApp.m_version)
		{
			str.Format(L"%*s %s", l, L"Összevonás programverziója:", theApp.m_uniteVersion);
			print(str);
		}

	str.Format(L"%*s %s", l, L"Lista készült:", theApp.getPresentDateTime());
	print(str);
	str.Format(L"%*s %d", l, L"Apai generációk száma:", m_numOfGF);
	print(str);
	str.Format(L"%*s %d", l, L"Anyai generációk száma:", m_numOfGM);
	print(str);
	str.Format(L"%*s %d", l, L"Egyenesági apai genrációk száma:", m_numOfGDF);
	print(str);
	str.Format(L"%*s %d", l, L"Egyenesági anyai generációk száma:", m_numOfGDM);
	print(str);
	str.Format(L"%*s %d", l, L"Apai felmenõk száma:", m_numOfAF);
	print(str);
	str.Format(L"%*s %d", l, L"Anyai felmenõk száma:", m_numOfAM);
	print(str);
	print(L"</pre><br>");

	int pos;
	if ((pos = title.Find(L"piramis") != -1) )
	{
		CString color;
		
		if (m_checkColor == true)
			print(L"Ismétlõdõ felmenõk kék színûek.<br><br>\n");
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A  vGKR i. bejegyzését listázza
void CKekule::listPeople( int i)
{
	CString ss;
	CString people;
	CString color;


	int ix;
	int j;
	int		g = vGKR.at(i).g;
	_int64	k = vGKR.at(i).k;
	TCHAR gen = TCHAR('A') + g;

	ss = indent(g, m_GPrev);
	m_GPrev = g;

	people = getPeopleString( i );
	str.Format(L"%s%I64d. %c&diams;%s", ss, k, gen, people);
	
	if (m_checkColor == true )													// ismétlõdõk kékkel
	{
		ix = vGKR.at(i).repeated - 1;
//		ix = vGKR.at(i).cnt - 1;
		if (ix >= 0 )
		{
//			j = ix % m_numOfColors;
//			color = CK::colors[j].color;
//			str.Format(L"<font color='%s'>%s</font>", color, (CString)str);
			str.Format(L"<font color=\"blue\">%s</font>", (CString)str);
		}
	}
/*
	else if( m_color == 2)												// csak az elsõ elõfordulás színes
//	{ 
		
	//	ix = vGKR.at(i).repeated - 1;
	//	ix = vGKR.at(i).root - 1;
	//	if (ix >= 0)
	//	{
	//		j = ix % m_numOfColors;
	//		color = CK::colors[j].color;
	//		str.Format(L"<font color='%s'>%s</font>", color, (CString)str);
	//	}
		
	//	ix = vGKR.at(i).cnt - 1;
	//	if (ix >= 0)
	//	{
	//		j = ix % m_numOfColors;
	//		color = CK::colors[j].color;
	//		str.Format(L"<font color='%s'>%s-%d</font>", color, (CString)str, j);
	//	}
 
	//	if( vGKR.at(i).cnt == 1)
	//		str.Format(L"<font color='blue'> %s</font>",(CString)str);
		
	//	if( vGKR.at(i).cnt == 2)
	//	str.Format(L"<font color='purple'> %s</font>", (CString)str);
	}
*/
	print(str);
	++m_cnt;
	m_kLast = k;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::listNN(_int64 k)
{
	CString ss;
	CString people;
	int		g = getGfromK( k);
	TCHAR gen = TCHAR('A') + g;

	ss = indent(g, m_GPrev);
	m_GPrev = g;

	people = L"N N";
	str.Format(L"%s%I64d. %c&diams;%s\n", ss, k, gen, people);
	print(str);
	++m_cnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::print(CString str)
{
	str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s\n", str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CKekule::indent(int gen, int genPrev)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CKekule::getPeopleString( int i )
{
	CString people;
	CString spouse_id;
	CString rowid = vGKR.at(i).r;

	CString sp(L" ");
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return(L"");

	CString name;
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

	//	name.Format(L"%s %s", last_name, first_name);

	str.Empty();
	if (m_peerPrint)
	{
		if (peer == L"gróf")
			peer = L"gr";
		else if (peer == L"báró")
			peer = L"br";
		else if (peer == L"herceg")
			peer = L"hg";

		if (!peer.IsEmpty())
		{
			str = peer;
			str += L" ";
		}
	}
	if (m_bold) str += L"<b>";

	if (last_name != L"N;" && !titolo.IsEmpty()) { str += titolo; str += L" "; }
	if (!last_name.IsEmpty()) { str += last_name; str += L" "; }
	if (!first_name.IsEmpty()) { str += first_name; str += L" "; }
	if (!posterior.IsEmpty()) str += posterior;
	str.Trim();
	if (m_bold) str += L"</b>";

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

	if (i + 1 < vGKR.size())
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
CString CKekule::getMarriage(CString rowid, CString rowidS)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kekule számból a generációt kiszámítani
UINT CKekule::getGfromK(_int64 k)
{
	_int64 kMin;
	_int64 kMax;
	UINT g;
	for (g = 0; ; ++g)
	{
		kMin = pow(2, g);
		kMax = pow(2, g + 1) - 1;
		if (kMin <= k && k <= kMax)
			break;
	}
	return g;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A GKR.at(I) felmenõit listázza
void CKekule::listAscendants(int I)
{
	m_kMinAct = vGKR.at(I).k * 2;			// I apjának Kekule száma
	m_kMaxAct = vGKR.at(I).k * 2 + 1;		// I anyjának Kekule száma
	int gPrev = vGKR.at(I).g + 1;			// I apjának-anyajának generációja
	_int64 k;
	m_cnt = 0;
	int g;
	m_kLast = vGKR.at(I).k - 1;
	for (int i = I + 1; i < vGKR.size(); ++i)
	{
		g = vGKR.at(i).g;				// a fölötte lévõ genereációban keres
		if (g > m_editG) break;				// elérte e maximális generációt
		if (gPrev < g)						// magasabb generációba léptünk
		{									// az aktuális listázandó k intervellum
			m_kMinAct = m_kMinAct * 2;		// legkisebb listázandó k az új genrációban
			m_kMaxAct = m_kMaxAct * 2 + 1;	// legnagybb listázandó k az új generációban 
			gPrev = g;
		}
		k = vGKR.at(i).k;
		if (m_kMinAct <= k && k <= m_kMaxAct)
		{
			if (m_NN)							// nem ismert felmenõk helyett N N kiírása
			{
				while (k != m_kLast + 1)
				{
					listNN(m_kLast + 2 );
					++m_kLast;
				}
			}
			listPeople(i);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// P I R A M I D ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////
void CKekule::OnClickedPyramid()
{
	int i;	// futó index

	_int64 K = m_editK;	// csúcs Kekule száma
	int I;				// csúcs indexe
	int G;				// csúcs genrációja

	UpdateData(FROMSCREEN);
	OnCancel();

	// elõírt paraméterek ellenõrzése
	for ( i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).k == K )
		{
			I = i;
			G = vGKR.at(i).g;
			break;
		}
	}
	if (i == vGKR.size())
	{
		str.Format(L"k=%I64d Kekule számú felmenõ nem ismert!", m_editK );
		AfxMessageBox(str);
		return;
	}
/*
	// összes lehetséges felmenõk száma
	if (m_NN && m_editG > 14 )
	{
		_int64 numOfK = pow(2, m_editG+1)-1;
		str.Format(L"%d generáció listázása %I64d felmenõt jelent.\nEz már nem kezelhetõ!", m_editG, numOfK);
	//	AfxMessageBox(str, MB_ICONEXCLAMATION);
	//	return;
	}
*/
	int gMax = G + _wtoi(s_editG);	// maximális listázandó generáció

	getPeople(vGKR.at(0).r);
	CString title;
	title.Format(L"%s felmenõinek %d generációs piramisa a %I64d Kekule számtól", m_name, m_editG, K);

	CProgressWnd wndP(NULL, title);
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR.size());

	openHtml(title, title);
	m_GPrev = G-1;
	m_cnt = 0;
	listPeople(I);		// csúcs kiírása


	_int64	kMin = K;
	_int64	kMax = K;
	_int64	kLast = K;
	_int64	k;
	int		g;
	int gLast = G;
	int db = 1;
	gLast = G;		// elõzõ listázott felmenõ generációja	

	for (int i = I + 1; i < vGKR.size(); ++i)
	{
		g = vGKR.at(i).g;
		if (g > gMax) break;				// elérte e maximális listázandó generációt
		k = vGKR.at(i).k;					// a következõ felmenõ 
		if (kMin <= k && k <= kMax)			// a generáció kiirandó tartományában van
		{
			while (m_NN && kLast + 1 < k)	// ha a generáción belül luk van az ismeret felmenõkben, akkor azt NN-el tölti ki, ha kell
			{
				listNN(kLast + 1);
				++kLast;
			}
			listPeople(i);
			kLast = k;
		}
		else								// k kívül esik a tartományon, új generációba kell lépni
		{
			while (m_NN && kLast < kMax)	// az elhagyni szándékozott generációt kiegészíti NN-el kMax-ig, ha kell.
			{
				listNN(kLast + 1);
				++kLast;
			}
			if (g == gLast + 1)				// generáció váltás, ha nincs generáció váltás egyszerõen kihagyja	
			{
				kMin = kMin * 2;			// az új generáció tartománya
				kMax = kMin + db;
				db = db * 2 + 1;
				kLast = kMin - 1;
				
				while (m_NN && k > kLast+1 && kLast < kMax)		// ha az új generáció nem az elsõ felmenõvel kezdõdik, akkor NN-el kezdeni amíg kell
				{
					listNN(kLast + 1);
					++kLast;
				}
				
				if (kMin <= k && k <= kMax)			// a generáció kiirandó tartományában van
				{
					listPeople(i);
					kLast = vGKR.at(i).k;
				}
			}

		}
		gLast = g;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if (m_NN)					// NN-el kell helyettesíteni a nem létezõ felmenõket
	{
		if ( K == 1)
		{
			while (m_kLast < kMax)			// az utolsó, nem teljes  generáció befejezése
			{
				listNN(m_kLast + 1);
				++m_kLast;
			}
		}

		k = m_kLast + 1;
		for (; g < gMax; ++g)				// az elírt, de nem ismert generációk listázása
		{
			kMin = kMin * 2;				// az új generáció tartománya
			kMax = kMin + db;
			db = db * 2 + 1;
			kLast = kMin - 1;
			k = kMin;
			while (kMin <= k && k <= kMax)
			{
				listNN(k);
				++k;
			}
		}
	}
	wndP.DestroyWindow();

	for (int j = 0; j < m_GPrev; ++j)  // behúzások törlése
	{
		fwprintf(fh, L"</ul>");
	}
	if (m_term)
		print(L"<br>A felmenõk beolvasása megszakítva!!<br>");

	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek felmenõi %s-nek a %d Kekule számtól", m_name, K );
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// D I R E C T   A S C E N D A N T S ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnClickedDirectFatherAscendants()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági apai felmenõi", m_name);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();

	getPeople(vGKR.at(0).r);
	openHtml(title, title);

	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 2;
	for (int i = 0; i < vGKR.size(); ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnClickedDirectMotherAscendants()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági anyai felmenõi", m_name);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();

	getPeople(vGKR.at(0).r);
	openHtml(title, title);
	
	m_kMin = 1;
	m_kMax = 1;

	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 3;
	for (int i = 0; i < vGKR.size(); ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnClickedDirectCouplesF()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági apai felmenõ házastársak", m_name);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();
	getPeople(vGKR.at(0).r);
	openHtml(title, title);


	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 2;
	for (int i = 1; i < vGKR.size()-1; ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			if (vGKR.at(i + 1).k == k + 1)  // feleség kiírása, ha van
			{
				listPeople(i + 1);
				++i;
			}
			else
			{
				if( m_NN )
					listNN(k + 1);
			}
			kF *= 2;;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnClickedDirectCouplesM()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format( L"%s egyenes ági anyai felmenõ házastársak", m_name);
	CProgressWnd wndP(NULL, title );
	wndP.GoModal();
	getPeople(vGKR.at(0).r);
	openHtml(title, title);

	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 3;
	for (int i = 1; i < vGKR.size() - 1; ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			if (k != 3 && vGKR.at(i + 1).k == k + 1)  // feleség kiírása, ha van
			{
				listPeople(i + 1);
				++i;
			}
			else
			{
				if (m_NN)
					listNN(k + 1);
			}
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_htmlFile, NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnKillfocusEditG()
{
	UpdateData(FROMSCREEN);
	m_editG = _wtoi(s_editG);
}
void CKekule::OnKillfocusEditK()
{
	UpdateData(FROMSCREEN);
	m_editK = _wtoi(s_editK);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKekule::OnCancel()
{
	UpdateData(FROMSCREEN);
	theApp.WriteProfileInt(L"dragon", L"m_peer", m_peerPrint);
	theApp.WriteProfileInt(L"dragon", L"m_bold", m_bold);
	theApp.WriteProfileInt(L"dragon", L"m_NN", m_NN);
	theApp.WriteProfileString(L"dragon", L"s_editG", s_editG);
	theApp.WriteProfileString(L"dragon", L"s_editK", s_editK);
	CDialogEx::OnCancel();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
