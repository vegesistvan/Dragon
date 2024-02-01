// descendatsP.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CDescendants.h"

IMPLEMENT_DYNAMIC(CDescendants, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendants::CDescendants(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTS, pParent)
	, p_connect(true)			// táblákat összekösse-e
	, p_womenDescendants(false)			// nõk leszármazottait listázza-e
	, p_otherNameAttrib(1)			// más náv bold
	, p_specAttrib(0)				// speciális karakterek (*+=) bold
	, p_commentAttrib(2)				// comment bold
	, p_familyNameAttrib(0)
	, p_bold(0)
	, p_capital(0)
	, p_fontSize(3)
	, p_numberingSystem(DE::SZLUHA)		// milyen számozási rendszer legyen (0,1,2) SZLUHA/VIL/TUP
	, m_tableNumber(L"")			// tablenumber, ha a táblázat leszármazotti listáját kérjük
	, p_rowWidth(L"képernyõ")
	, m_printed(FALSE)
	, p_folyt(FALSE)
	, p_generationMax(_T(""))
	, p_oneOutputFile(true)
	, p_repeated(0)
	, p_repeatedColor(FALSE)
	, p_checkCRLF(FALSE)
	, p_childrenOrder(1)
	, p_titololower(FALSE)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendants::~CDescendants()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CONNECT, p_connect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, p_womenDescendants);

	DDX_Check(pDX, IDC_CHECK_FOLYT, p_folyt);

	DDX_Control(pDX, IDC_COMBO_COMMENT, m__commentAttribCombo);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_specAttribCombo);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_fontSizeCombo);
	DDX_Control(pDX, IDC_COMBO_NAME, m_otherNameAttribCombo);
	DDX_Control(pDX, IDC_COMBO_DESCATTRIB, m_descendantAttribCombo);
	DDX_Control(pDX, IDC_COMBO_FAMILYNAME, m_comboFamilyName);

	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Text(pDX, IDC_EDIT_WIDTH, p_rowWidth);
	DDX_Control(pDX, IDC_STATIC_NAME, colorName);
	DDX_Control(pDX, IDC_STATIC_NUM, colorNum);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_ATTRIB, colorAttrib);
	DDX_Text(pDX, IDC_GENMAX, p_generationMax);

	DDX_Radio(pDX, IDC_RADIO_ONE, p_oneOutputFile);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, colorOutput);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_CONTENT, colorContent);
	DDX_Check(pDX, IDC_REPEATED_COLOR, p_repeatedColor);
	DDX_Control(pDX, IDC_STATIC_REPEATED, colorRepeated);
	DDX_Check(pDX, IDC_CHECK_CRLF, p_checkCRLF);
	DDX_Control(pDX, IDC_ORDER_TXT, colorOrder);
	DDX_Control(pDX, IDC_GROUP_ALL_TEXT, groupAllText);
	DDX_Control(pDX, IDC_GROUP_DONTWORK_TEXT, groupDontworkText);
	DDX_Control(pDX, IDC_GROUP_INDENTED_TEXT, groupIndentedText);
	DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_descendantNameRadio);
	DDX_Radio(pDX, IDC_ORDER_INPUT, p_childrenOrder);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, m_backgroundColor);
	DDX_Check(pDX, IDC_TITOLOLOWER, p_titololower);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendants, CDialogEx)

	ON_BN_CLICKED(IDC_SZLUHA, &CDescendants::OnClickedSzluha)
	ON_COMMAND(IDC_TUPIGNY, &CDescendants::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CDescendants::OnVillers)

	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CDescendants::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDescendants::OnClickedCheckConnect)

	ON_STN_CLICKED(IDC_STATIC_BACKGROUND, &CDescendants::OnClickedStaticBackground)
	ON_BN_CLICKED(IDC_RADIO_NOFAMILYNAME, &CDescendants::OnClickedRadioNofamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAME, &CDescendants::OnRadioFamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAMEUP, &CDescendants::OnRadioFamilynameup)
	ON_BN_CLICKED(IDC_CHECK_FOLYT, &CDescendants::OnClickedCheckFolyt)

	ON_BN_CLICKED(IDC_DEFAULT, &CDescendants::OnClickedDefault)
	ON_BN_CLICKED(IDOK, &CDescendants::OnBnClickedOk)

	ON_BN_CLICKED(IDC_REPEATED_ALL, &CDescendants::OnClickedRepeatedAll)
	ON_COMMAND(IDC_REPEATED_FIRST, &CDescendants::OnRepeatedFirst)
	ON_COMMAND(IDC_REPEATED_FATHER, &CDescendants::OnRepeatedFather)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ORDER_INPUT, &CDescendants::OnClickedOrderInput)
	ON_COMMAND(IDC_ORDER_LENGTH, &CDescendants::OnOrderLength)
	ON_COMMAND(IDC_ORDER_DECREASING, &CDescendants::OnOrderDecreasing)
	ON_COMMAND(IDC_ORDER_BIRTH, &CDescendants::OnOrderBirth)
	ON_BN_CLICKED(IDC_TITOLOLOWER, &CDescendants::OnClickedTitololower)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < sizeof(DE::attrib) / sizeof(DE::ATTRIB); ++i)
	{
		m_descendantAttribCombo.InsertString(i, DE::attrib[i].text);
		m_otherNameAttribCombo.InsertString(i, DE::attrib[i].text);
		m_specAttribCombo.InsertString(i, DE::attrib[i].text);
		m__commentAttribCombo.InsertString(i, DE::attrib[i].text);
	}
	m_comboFamilyName.InsertString(0, L"normál");
	m_comboFamilyName.InsertString(1, L"nagybetû");

	for (INT i = 0; i < 6; ++i)
	{
		str.Format(L"%d", 10 + i * 2);
		m_fontSizeCombo.InsertString(i, str);
	}
	
	COLORREF color = RGB(255, 169, 169);
	color = RGB(255, 0, 0);

	groupIndentedText.SetTextColor(LIGHTBLUE);
	groupDontworkText.SetTextColor(LIGHTBLUE);
	colorAttrib.SetTextColor(theApp.m_colorClick);
	colorPrint.SetTextColor(theApp.m_colorClick);
	colorOutput.SetTextColor(theApp.m_colorClick);
	colorNum.SetTextColor(theApp.m_colorClick);
	colorRepeated.SetTextColor(theApp.m_colorClick);

	groupAllText.SetTextColor(RED);
	colorFamilyInline.SetTextColor(RED);
	colorFamilynameNo.SetTextColor(RED);
	colorFamilynameUp.SetTextColor(RED);
	colorName.SetTextColor(RED);
	colorContent.SetTextColor(RED);
	colorOrder.SetTextColor(RED);


	m_descendantsPath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_descendantsPath, 0))
		_wmkdir(m_descendantsPath);
	
	setParameters();
	updateParameters();
	
	if (theApp.m_inputMode == GEDCOM)
	{

		GetDlgItem(IDC_CHECK_FOLYT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_CONNECT)->EnableWindow(false);
	}
	
//	if (theApp.v_rowid.size() < 2 && theApp.v_tableNumbers.size() < 2)
	if ( theApp.v_tableNumbers.size() < 2)
	{
		p_oneOutputFile = true;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_RADIO_ONE)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ONE1)->EnableWindow(false);
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::setParameters()
{
	p_descendantName	= theApp.GetProfileInt(L"dragon", L"p_descendantName", DE::RAISED);
	p_connect			= theApp.GetProfileInt(L"dragon", L"p_connect", p_connect);
	p_womenDescendants	= theApp.GetProfileInt(L"dragon", L"p_womenDescendants", p_womenDescendants);
	p_childrenOrder		= theApp.GetProfileInt(L"dragon", L"p_childrenOrder", p_childrenOrder);
	p_titololower		= theApp.GetProfileInt(L"dragon", L"p_titololower", p_titololower);
	p_repeated			= theApp.GetProfileInt(L"dragon", L"p_repeated", p_repeated);
	p_repeatedColor		= theApp.GetProfileInt(L"dragon", L"p_repeatedColor", p_repeatedColor);
	p_folyt				= theApp.GetProfileInt(L"dragon", L"p_folyt", p_folyt);
	p_descendantAttrib	= theApp.GetProfileInt(L"dragon", L"p_descendantAttrib", p_descendantAttrib);
	p_familyNameAttrib	= theApp.GetProfileInt(L"dragon", L"p_familyNameAttrib", p_familyNameAttrib);
	p_otherNameAttrib	= theApp.GetProfileInt(L"dragon", L"p_otherNameAttrib", p_otherNameAttrib);
	p_commentAttrib		= theApp.GetProfileInt(L"dragon", L"p_commentAttrib", p_commentAttrib);
	p_fontSize			= theApp.GetProfileInt(L"dragon", L"p_fontSize", p_fontSize);
	p_specAttrib		= theApp.GetProfileInt(L"dragon", L"p_specAttrib", p_specAttrib);
	p_numberingSystem	= theApp.GetProfileInt(L"dragon", L"p_numberingSystem", p_numberingSystem);
	p_colorBgrnd		= theApp.GetProfileInt(L"dragon", L"p_colorBgrnd", WHITE);
	p_checkCRLF			= theApp.GetProfileInt(L"dragon", L"p_checkCRLF", WHITE);
	p_oneOutputFile		= theApp.GetProfileInt(L"dragon", L"p_oneOutputFile", p_oneOutputFile);

	p_rowWidth			= theApp.GetProfileString(L"dragon", L"p_rowWidth", L"0");
	p_generationMax		= theApp.GetProfileString(L"dragon", L"p_generationMax", p_generationMax);

	UpdateData(TOSCREEN);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedDefault()
{
	p_connect = true;
	p_womenDescendants = true;
	p_titololower = false;
	p_folyt = false;
	p_capital = false;
	p_checkCRLF = false;
	p_oneOutputFile = true;
	p_folyt = false;

	p_rowWidth = L"0";
	p_generationMax.Empty();

	p_repeatedColor = false;
	p_descendantName = DE::RAISED;
	p_numberingSystem = DE::SZLUHA;
	p_childrenOrder = 0;

	p_familyNameAttrib = 0;
	p_descendantAttrib = 1;
	p_fontSize = 3;
	p_commentAttrib = 2;
	p_specAttrib = 0;
	p_otherNameAttrib = 1;

	p_colorBgrnd = WHITE;

	p_repeated =  0;
	p_repeatedColor = 0;


	updateParameters();




	GetDlgItem(IDC_CHECK_CAPITAL)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_DESCATTRIB)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_NAME)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_COMMENT)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_SPEC)->EnableWindow(true);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateParameters()
{
	m_otherNameAttribCombo.SetCurSel(p_otherNameAttrib);
	m_specAttribCombo.SetCurSel(p_specAttrib);
	m__commentAttribCombo.SetCurSel(p_commentAttrib);
	m_descendantAttribCombo.SetCurSel(p_descendantAttrib);
	m_fontSizeCombo.SetCurSel(p_fontSize);
	m_comboFamilyName.SetCurSel(p_familyNameAttrib);


	m_backgroundColor.SetColor(BLACK, p_colorBgrnd);


	updateRadioDName();
	updateRadioNumbering();
	updateRepeated();
	UpdateData(TOSCREEN);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRadioDName()
{

	// Set the check state to the next state
	// (i.e. BST_UNCHECKED changes to BST_CHECKED
	// BST_CHECKED changes to BST_INDETERMINATE
	// BST_INDETERMINATE changes to BST_UNCHECKED).
//	m_descendantNameRadio.SetCheck(((m_descendantNameRadio.GetCheck() + 1) % 3));

	if (p_descendantName == DE::NOINLINE)									// leszármazott családnevét nem írjuk ki
	{
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);

		m_comboFamilyName.SetCurSel(0);
		GetDlgItem(IDC_COMBO_FAMILYNAME)->EnableWindow(false);
	}
	else if (p_descendantName == DE::INLINE)								// családnév a leszármazotti sorban
	{
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);


		GetDlgItem(IDC_COMBO_FAMILYNAME)->EnableWindow(true);
	}
	else if (p_descendantName == DE::RAISED)								// leszármazott családneve kiemelve
	{
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(TRUE);

		
		m_comboFamilyName.SetCurSel(0);
		GetDlgItem(IDC_COMBO_FAMILYNAME)->EnableWindow(false);
	}
	UpdateData(TOSCREEN);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRadioNumbering()
{
	if (p_numberingSystem == DE::SZLUHA)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else if (p_numberingSystem == DE::VIL)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(TRUE);
	}
	UpdateData(TOSCREEN);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRepeated()
{
	switch (p_repeated)
	{
	case 0:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(FALSE);
//		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(FALSE);
		break;
	case 1:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(TRUE);
//		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(FALSE);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(FALSE);
//		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(TRUE);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckConnect()
{
	if (p_womenDescendants)					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckWoman()
{
	p_womenDescendants = !p_womenDescendants;				// ha a nõk cgyerekeit is listázni akarjuk, akkor a táblákat is össze kell kötni!!
	if (p_womenDescendants)
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckFolyt()
{
	p_folyt = !p_folyt;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDescendants::OnClickedCheckCapital()
//{
//	p_capital = !p_capital;
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDescendants::OnClickedCheckBold()
//{
//	p_bold = !p_bold;
//	if (p_bold == true)
//	{
//		p_capital = false;
//		p_commentAttrib = 0;
//		p_descendantAttrib = 0;
//		p_otherNameAttrib = 0;
//		p_specAttrib = 0;
//		m_otherNameAttribCombo.SetCurSel(p_otherNameAttrib);
//		m_specAttribCombo.SetCurSel(p_specAttrib);
//		m__commentAttribCombo.SetCurSel(p_commentAttrib);
//		m_descendantAttribCombo.SetCurSel(p_descendantAttrib);
//
//		GetDlgItem(IDC_CHECK_CAPITAL)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_DESCATTRIB)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_NAME)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_COMMENT)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_SPEC)->EnableWindow(false);
//	}
//	else
//	{
//		GetDlgItem(IDC_CHECK_CAPITAL)->EnableWindow(true);
//		GetDlgItem(IDC_COMBO_DESCATTRIB)->EnableWindow(true);
//		GetDlgItem(IDC_COMBO_NAME)->EnableWindow(true);
//		GetDlgItem(IDC_COMBO_COMMENT)->EnableWindow(true);
//		GetDlgItem(IDC_COMBO_SPEC)->EnableWindow(true);
//	}
//	UpdateData(TOSCREEN);
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedSzluha()
{
	p_numberingSystem = DE::SZLUHA;
	updateRadioNumbering();
}
void CDescendants::OnVillers()
{
	p_numberingSystem = DE::VIL;
	updateRadioNumbering();
}
void CDescendants::OnTupigny()
{
	p_numberingSystem = DE::TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedRadioNofamilyname()
{
	p_descendantName = DE::NOINLINE;
	updateRadioDName();
}
void CDescendants::OnRadioFamilyname()
{
	p_descendantName = DE::INLINE;
	updateRadioDName();
}
void CDescendants::OnRadioFamilynameup()
{
	p_descendantName = DE::RAISED;
	updateRadioDName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedRepeatedAll()
{
	p_repeated = 0;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnRepeatedFirst()
{
	p_repeated = 1;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnRepeatedFather()
{
	p_repeated = 2;
	updateRepeated();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedStaticBackground()
{
	CMFCColorDialog dlgColors;
	dlgColors.SetCurrentColor(WHITE);
	if (dlgColors.DoModal() == IDCANCEL) return;

	COLORREF bgn = dlgColors.GetColor();
	m_backgroundColor.SetColor(BLACK, bgn);

	p_colorBgrnd = GetRValue(bgn) << 16 | GetGValue(bgn) << 8 | GetBValue(bgn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnPaint()
{
	CPaintDC dc(this);
	COLORREF	color = LIGHTBLUE;
	colorFrame(&dc, IDC_GROUPBOX_DESC, RED);
	colorFrame(&dc, IDC_GROUP_CONTENT, RED);
	colorFrame(&dc, IDC_GROUP_REPEATED, color);
	
	colorFrame(&dc, IDC_GROUPBOX_ATTRIB, color);
	colorFrame(&dc, IDC_GROUPBOX_NUM, color);
	colorFrame(&dc, IDC_GROUPBOX_PRINT, color);
	colorFrame(&dc, IDC_GROUPBOX_OUTPUT, color);
	colorFrame(&dc, IDC_GROUP_ORDER, color);

	colorFrame(&dc, IDC_GROUP_INDENTED, LIGHTBLUE);
	colorFrame(&dc, IDC_GROUP_ALL, RED);
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color)
{
	CRect rc;

	HPEN penLine = CreatePen(PS_SOLID, 0, color);
	dc->SelectObject(penLine);

	CWnd* pW = this->GetDlgItem(IDC);
	pW->GetWindowRect(&rc);
	this->ScreenToClient(&rc);


	rc.TopLeft().x -= 1;	// a szürke keret mellett kell rajzolni, mert különben a keret felülírja a színes négyzetet
	rc.BottomRight().x += 1;	// ez szebb, mint az alábbi vonalas megoldás, de kifehérítia négyszögön belül és a szövegek háttérszíne szürke marad.
	rc.TopLeft().y += 5;	// Ezért mégis szebb a vonalas rajzolás

	COLORREF colour = ::GetSysColor(COLOR_3DFACE);
	HBRUSH hbr = CreateSolidBrush(colour);
	dc->SelectObject(hbr);	// hogy a háttér olyan szürke legyen, mint a benne lévõ control-ok (szöveg, gomb )

	dc->Rectangle(&rc);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedOrderInput()
{
	p_childrenOrder = DE::ORDER_INPUT;
}
void CDescendants::OnOrderBirth()
{
	p_childrenOrder = DE::ORDER_BIRTH;
}
void CDescendants::OnOrderLength()
{
	p_childrenOrder = DE::ORDER_INCREASING;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnOrderDecreasing()
{
	p_childrenOrder = DE::ORDER_DECREASING;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getComplexDescription(int i, bool parentIndex)
{
	CString people = createDescendant(i, parentIndex);
	CString spouses = createSpouses(i);
	CString arm;
	CString csalad;
	CString folyt;

	people.Trim();
	people += L" ";
	people += spouses.Trim();
	people.Trim();

	if (!p.arm.IsEmpty())
	{
		arm.Format(L" <font color='blue'>[%s]</font>", p.arm);
		people += arm;
	}
	if (!p.csalad.IsEmpty())
	{
		csalad.Format(L" <font color='blue'>[%s]</font>", p.csalad);
		people += csalad;
	}
	if (!p.folyt.IsEmpty() && p_folyt)
	{
		folyt.Format(L"<font color='blue'> %c%c%c folyt %s</font>", '%', '%', '%', p.folyt);
		people += folyt;
	}
	return people;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnBnClickedOk()
{
	UpdateData(FROMSCREEN);

	p_descendantAttrib = m_descendantAttribCombo.GetCurSel();
	p_familyNameAttrib = m_comboFamilyName.GetCurSel();
	p_otherNameAttrib = m_otherNameAttribCombo.GetCurSel();
	p_specAttrib = m_specAttribCombo.GetCurSel();
	p_commentAttrib = m__commentAttribCombo.GetCurSel();
	p_fontSize = m_fontSizeCombo.GetCurSel();

	if (p_familyNameAttrib == 1) p_capital = true;
	else p_capital = false;
	if (p_generationMax.IsEmpty())
		m_generationMax = INT_MAX;
	else
	{
		m_generationMax = _wtoi(p_generationMax);
		if (m_generationMax < 3)
		{
			AfxMessageBox(L"A maximális listázandó generációk száma 2-nél nagyobbnak kell lenni!", MB_ICONEXCLAMATION);
			return;
		}
	}
	theApp.WriteProfileInt(L"dragon", L"p_descendantName", p_descendantName);
	theApp.WriteProfileInt(L"dragon", L"p_connect", p_connect);
	theApp.WriteProfileInt(L"dragon", L"p_womenDescendants", p_womenDescendants);
	theApp.WriteProfileInt(L"dragon", L"p_childrenOrder", p_childrenOrder);
	theApp.WriteProfileInt(L"dragon", L"p_titololower", p_titololower);
	theApp.WriteProfileInt(L"dragon", L"p_repeated", p_repeated);
	theApp.WriteProfileInt(L"dragon", L"p_repeatedColor", p_repeatedColor);
	theApp.WriteProfileInt(L"dragon", L"p_folyt", p_folyt);
	theApp.WriteProfileInt(L"dragon", L"p_descendantAttrib", p_descendantAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_familyNameAttrib", p_familyNameAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_otherNameAttrib", p_otherNameAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_commentAttrib", p_commentAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_fontSize", p_fontSize);
	theApp.WriteProfileInt(L"dragon", L"p_specAttrib", p_specAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_numberingSystem", p_numberingSystem);
	theApp.WriteProfileInt(L"dragon", L"p_colorBgrnd", p_colorBgrnd);
	theApp.WriteProfileInt(L"dragon", L"p_checkCRLF", p_checkCRLF);
	theApp.WriteProfileInt(L"dragon", L"p_oneOutputFile", p_oneOutputFile);

	theApp.WriteProfileString(L"dragon", L"p_rowWidth", p_rowWidth);
	theApp.WriteProfileString(L"dragon", L"p_generationMax", p_generationMax);


	int x = p_descendantName;
	create_vDesc();

	CDialog::OnCancel();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedTitololower()
{
	p_titololower = !p_titololower;
	UpdateData(TOSCREEN);
}
