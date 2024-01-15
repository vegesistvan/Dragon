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
	, m_checkConnect(true)			// táblákat összekösse-e
	, m_checkMother(false)			// nõk leszármazottait listázza-e
	, m_checkCapital(false)
	, m_checkBold(FALSE)
	, m_comboOtherName(1)			// más náv bold
	, m_comboSpec(0)				// speciális karakterek (*+=) bold
	, m_comboComment(2)				// comment bold
	, m_comboKiemeltAttrib(3)		// leszármazott kiemelt családneve
	, m_comboFontSize(3)

	, m_radioNumbering(DE::TUPP)		// milyen számozási rendszer legyen (0,1,2) SZLUHA/VIL/TUP

	, m_tableNumber(L"")			// tablenumber, ha a táblázat leszármazotti listáját kérjük
	, m_editWidth(L"képernyõ")
	, m_printed(FALSE)
	, m_checkFolyt(FALSE)
	, m_editGenMax(_T(""))
	, m_radioOne(true)
	, m_repeated(0)
	, m_repeatedColor(FALSE)
	, m_checkCRLF(FALSE)
	, m_radioOrder(0)
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
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_checkConnect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, m_checkMother);

	DDX_Check(pDX, IDC_CHECK_CAPITAL, m_checkCapital);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_checkBold);
	DDX_Check(pDX, IDC_CHECK_FOLYT, m_checkFolyt);

	DDX_Control(pDX, IDC_COMBO_COMMENT, m_combo_comment);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_combo_spec);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_combo_FontSize);

	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, colorBgn);

	DDX_Control(pDX, IDC_COMBO_NAME, m_combo_OtherName);
	DDX_Control(pDX, IDC_COMBO_DESCATTRIB, m_combo_DAttrib);
	DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_RadioFamily);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_STATIC_NAME, colorName);
	DDX_Control(pDX, IDC_STATIC_NUM, colorNum);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_ATTRIB, colorAttrib);
//	DDX_Control(pDX, IDC_STATIC_FAMILY_INLINE, colorFamilyInline);
//	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_NO, colorFamilynameNo);
//	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_UP, colorFamilynameUp);
	DDX_Text(pDX, IDC_GENMAX, m_editGenMax);

	DDX_Radio(pDX, IDC_RADIO_ONE, m_radioOne);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, colorOutput);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_CONTENT, colorContent);
	DDX_Check(pDX, IDC_REPEATED_COLOR, m_repeatedColor);
	DDX_Control(pDX, IDC_STATIC_REPEATED, colorRepeated);
	DDX_Control(pDX, IDC_COMBO_HTMTXT, m_comboHtmlTxt);
	DDX_Check(pDX, IDC_CHECK_CRLF, m_checkCRLF);
	DDX_Radio(pDX, IDC_ORDER_INPUT, m_radioOrder);
	DDX_Control(pDX, IDC_ORDER_TXT, colorOrder);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendants, CDialogEx)

	ON_BN_CLICKED(IDC_SZLUHA, &CDescendants::OnClickedSzluha)
	ON_COMMAND(IDC_TUPIGNY, &CDescendants::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CDescendants::OnVillers)

	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CDescendants::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDescendants::OnClickedCheckConnect)

	ON_BN_CLICKED(IDC_CHECK_CAPITAL, &CDescendants::OnClickedCheckCapital)
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
	ON_BN_CLICKED(IDC_CHECK_BOLD, &CDescendants::OnClickedCheckBold)
	ON_BN_CLICKED(IDC_ORDER_INPUT, &CDescendants::OnClickedOrderInput)
	ON_COMMAND(IDC_ORDER_LENGTH, &CDescendants::OnOrderLength)
	ON_COMMAND(IDC_ORDER_DECREASING, &CDescendants::OnOrderDecreasing)
	ON_COMMAND(IDC_ORDER_BIRTH, &CDescendants::OnOrderBirth)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < sizeof(DE::attrib) / sizeof(DE::ATTRIB); ++i)
	{
		m_combo_DAttrib.InsertString(i, DE::attrib[i].text);
		m_combo_OtherName.InsertString(i, DE::attrib[i].text);
		m_combo_spec.InsertString(i, DE::attrib[i].text);
		m_combo_comment.InsertString(i, DE::attrib[i].text);
		//		m_combo_KiemeltAttrib.InsertString(i, attrib[i].text);
	}
	for (INT i = 0; i < 6; ++i)
	{
		str.Format(L"%d", 10 + i * 2);
		m_combo_FontSize.InsertString(i, str);
	}

	m_comboHtmlTxt.AddString(L"htm");
	m_comboHtmlTxt.AddString(L"txt");
	m_comboHtmlTxt.SetCurSel(0);

	COLORREF color = RGB(255, 169, 169);
	color = RED;
	color = RGB(255, 0, 0);

	colorFamilyInline.SetTextColor(color);
	colorFamilynameNo.SetTextColor(color);
	colorFamilynameUp.SetTextColor(color);

	colorContent.SetTextColor(theApp.m_colorClick);
	colorName.SetTextColor(theApp.m_colorClick);
	colorAttrib.SetTextColor(theApp.m_colorClick);
	colorPrint.SetTextColor(theApp.m_colorClick);
	colorOutput.SetTextColor(theApp.m_colorClick);
	colorNum.SetTextColor(theApp.m_colorClick);
	colorRepeated.SetTextColor(theApp.m_colorClick);
	colorOrder.SetTextColor(theApp.m_colorClick);

	setParameters();
	updateParameters();
	
	if (theApp.m_inputMode == GEDCOM)
	{

		GetDlgItem(IDC_CHECK_FOLYT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_CONNECT)->EnableWindow(false);
	}
	
	if (theApp.v_rowid.size() < 2 && theApp.v_tableNumbers.size() < 2)
	{
		m_radioOne = true;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_RADIO_ONE)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ONE1)->EnableWindow(false);
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::setParameters()
{
	m_checkConnect = theApp.GetProfileInt(L"dragon", L"m_checkConnect", 0);
	m_checkMother = theApp.GetProfileInt(L"dragon", L"m_checkMother", 1);
	m_checkFolyt = theApp.GetProfileInt(L"dragon", L"m_checkFolyt", 0);
	m_checkCapital = theApp.GetProfileInt(L"dragon", L"m_checkCapital", 0);
	m_checkBold = theApp.GetProfileInt(L"dragon", L"m_checkBold", 0);

	m_radioOne = theApp.GetProfileInt(L"dragon", L"m_radioOne", true);
	m_radioDNameX = theApp.GetProfileInt(L"dragon", L"m_radioDNameX", 1);
	m_radioNumbering = theApp.GetProfileInt(L"dragon", L"m_radioNumbering", 2);

	m_editWidth = theApp.GetProfileString(L"dragon", L"m_editWidth", L"0");
	m_editGenMax = theApp.GetProfileString(L"dragon", L"m_editGenMax", L"");


	m_repeated = theApp.GetProfileInt(L"dragon", L"m_repeated", 0);
	m_repeatedColor = theApp.GetProfileInt(L"dragon", L"m_repeatedColor", 0);

	m_comboDAttrib = theApp.GetProfileInt(L"dragon", L"m_comboDAttrib", 1);
	m_comboFontSize = theApp.GetProfileInt(L"dragon", L"m_comboFontSize", 3);
	m_comboComment = theApp.GetProfileInt(L"dragon", L"m_comboComment", 2);
	m_comboSpec = theApp.GetProfileInt(L"dragon", L"m_comboSpec", 0);
	m_comboOtherName = theApp.GetProfileInt(L"dragon", L"m_comboOtherName", 1);

	m_colorBgrnd = theApp.GetProfileInt(L"dragon", L"m_colorBgrnd", WHITE);
	m_checkCRLF = theApp.GetProfileInt(L"dragon", L"m_checkCRLF", WHITE);

	m_radioOrder = theApp.GetProfileInt(L"dragon", L"m_radioOrder", WHITE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedDefault()
{
	m_comboHtmlTxt.SetCurSel(0);
	m_checkConnect = true;
	m_checkMother = true;
	m_checkFolyt = false;
	m_checkCapital = false;
	m_checkBold = false;
	m_checkCRLF = false;
	m_radioOne = true;
	m_folyt = false;

	m_editWidth = L"0";
	m_editGenMax.Empty();

	m_repeatedColor = false;
	m_radioDNameX = 2;
	m_radioNumbering = DE::SZLUHA;
	m_radioOrder = 0;

	m_comboDAttrib = 1;
	m_comboFontSize = 3;
	m_comboComment = 2;
	m_comboSpec = 0;
	m_comboOtherName = 1;

	m_colorBgrnd = WHITE;

	m_repeated =  0;
	m_repeatedColor = 0;

	m_radioOrder = 0;

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
	m_combo_OtherName.SetCurSel(m_comboOtherName);
	m_combo_spec.SetCurSel(m_comboSpec);
	m_combo_comment.SetCurSel(m_comboComment);
	m_combo_DAttrib.SetCurSel(m_comboDAttrib);
	m_combo_FontSize.SetCurSel(m_comboFontSize);


	colorBgn.SetColor(BLACK, m_colorBgrnd);

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
	if (m_radioDNameX == 0)									// leszármazott családnevét nem írjuk ki
	{
		m_comboKiemeltAttrib = 0;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
		m_checkBold = false;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(false);
	}
	else if (m_radioDNameX == 1)								// családnév a leszármazotti sorban
	{
		m_comboKiemeltAttrib = 0;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(true);
	}
	else if (m_radioDNameX == 2)								// leszármazott családneve kiemelve
	{
		m_RadioFamily.SetCheck(2);
		m_comboKiemeltAttrib = 3;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(TRUE);
		m_checkBold = false;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(false);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRadioNumbering()
{
	if (m_radioNumbering == DE::SZLUHA)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else if (m_radioNumbering == DE::VIL)
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
	switch (m_repeated)
	{
	case 0:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(FALSE);
		break;
	case 1:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(FALSE);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_REPEATED_ALL))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FIRST))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_REPEATED_FATHER))->SetCheck(TRUE);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckConnect()
{
	if (m_checkMother)					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
	{
		m_checkConnect = true;
		UpdateData(TOSCREEN);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckWoman()
{
	m_checkMother = !m_checkMother;				// ha a nõk cgyerekeit is listázni akarjuk, akkor a táblákat is össze kell kötni!!
	if (m_checkMother)
	{
		m_checkConnect = true;
		UpdateData(TOSCREEN);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckFolyt()
{
	m_checkFolyt = !m_checkFolyt;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckCapital()
{
	m_checkCapital = !m_checkCapital;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckBold()
{
	m_checkBold = !m_checkBold;
	if (m_checkBold == true)
	{
		m_checkCapital = false;
		m_comboComment = 0;
		m_comboDAttrib = 0;
		m_comboOtherName = 0;
		m_comboSpec = 0;
		m_combo_OtherName.SetCurSel(m_comboOtherName);
		m_combo_spec.SetCurSel(m_comboSpec);
		m_combo_comment.SetCurSel(m_comboComment);
		m_combo_DAttrib.SetCurSel(m_comboDAttrib);

		GetDlgItem(IDC_CHECK_CAPITAL)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_DESCATTRIB)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_NAME)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_COMMENT)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_SPEC)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_CHECK_CAPITAL)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_DESCATTRIB)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_NAME)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_COMMENT)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_SPEC)->EnableWindow(true);
	}
	UpdateData(TOSCREEN);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedSzluha()
{
	m_radioNumbering = DE::SZLUHA;
	updateRadioNumbering();
}
void CDescendants::OnVillers()
{
	m_radioNumbering = DE::VIL;
	updateRadioNumbering();
}
void CDescendants::OnTupigny()
{
	m_radioNumbering = DE::TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedRadioNofamilyname()
{
	m_radioDNameX = 0;
	updateRadioDName();
}
void CDescendants::OnRadioFamilyname()
{
	m_radioDNameX = 1;
	updateRadioDName();
}
void CDescendants::OnRadioFamilynameup()
{
	m_radioDNameX = 2;
	updateRadioDName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedRepeatedAll()
{
	m_repeated = 0;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnRepeatedFirst()
{
	m_repeated = 1;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnRepeatedFather()
{
	m_repeated = 2;
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
	colorBgn.SetColor(BLACK, bgn);

	m_colorBgrnd = GetRValue(bgn) << 16 | GetGValue(bgn) << 8 | GetBValue(bgn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnBnClickedOk()
{
	UpdateData(FROMSCREEN);


	m_html = m_comboHtmlTxt.GetCurSel();
	m_comboDAttrib = m_combo_DAttrib.GetCurSel();
	m_comboOtherName = m_combo_OtherName.GetCurSel();
	m_comboSpec = m_combo_spec.GetCurSel();
	m_comboComment = m_combo_comment.GetCurSel();
	m_comboFontSize = m_combo_FontSize.GetCurSel();


	if (m_editGenMax.IsEmpty())
		m_generationMax = INT_MAX;
	else
	{
		m_generationMax = _wtoi(m_editGenMax);
		if (m_generationMax < 3)
		{
			AfxMessageBox(L"A maximális listázandó generációk száma 2-nél nagyobbnak kell lenni!", MB_ICONEXCLAMATION);
			return;
		}
	}

	m_oneFile = m_radioOne;

	int z = m_repeated;
	theApp.WriteProfileInt(L"dragon", L"m_checkConnect", m_checkConnect);
	theApp.WriteProfileInt(L"dragon", L"m_checkMother", m_checkMother);
	theApp.WriteProfileInt(L"dragon", L"m_repeated", m_repeated);
	theApp.WriteProfileInt(L"dragon", L"m_repeatedColor", m_repeatedColor);
	theApp.WriteProfileInt(L"dragon", L"m_checkFolyt", m_checkFolyt);

	theApp.WriteProfileString(L"dragon", L"m_editWidth", m_editWidth);
	theApp.WriteProfileString(L"dragon", L"m_editGenMax", m_editGenMax);

	theApp.WriteProfileInt(L"dragon", L"m_radioDNameX", m_radioDNameX);
	theApp.WriteProfileInt(L"dragon", L"m_comboDAttrib", m_comboDAttrib);
	theApp.WriteProfileInt(L"dragon", L"m_comboOtherName", m_comboOtherName);
	theApp.WriteProfileInt(L"dragon", L"m_comboComment", m_comboComment);
	theApp.WriteProfileInt(L"dragon", L"m_comboFontSize", m_comboFontSize);
	theApp.WriteProfileInt(L"dragon", L"m_comboSpec", m_comboSpec);
	theApp.WriteProfileInt(L"dragon", L"m_radioNumbering", m_radioNumbering);
	theApp.WriteProfileInt(L"dragon", L"m_radioOrder", m_radioOrder);
	theApp.WriteProfileInt(L"dragon", L"m_colorBgrnd", m_colorBgrnd);
	theApp.WriteProfileInt(L"dragon", L"m_checkCapital", m_checkCapital);
	theApp.WriteProfileInt(L"dragon", L"m_checkBold", m_checkBold);
	theApp.WriteProfileInt(L"dragon", L"m_checkCRLF", m_checkCRLF);
	theApp.WriteProfileInt(L"dragon", L"m_radioOne", m_radioOne);

	create_vDesc();

//	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnPaint()
{
	CPaintDC dc(this);
	COLORREF	color = LIGHTBLUE;
	colorFrame(&dc, IDC_GROUPBOX_DESC, color);
	colorFrame(&dc, IDC_GROUP_CONTENT, color);
	colorFrame(&dc, IDC_GROUP_REPEATED, color);
	
	colorFrame(&dc, IDC_GROUPBOX_ATTRIB, color);
	colorFrame(&dc, IDC_GROUPBOX_NUM, color);
	colorFrame(&dc, IDC_GROUPBOX_PRINT, color);
	colorFrame(&dc, IDC_GROUPBOX_OUTPUT, color);
	colorFrame(&dc, IDC_GROUP_ORDER, color);
	
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
	m_radioOrder = DE::ORDER_INPUT;
}
void CDescendants::OnOrderBirth()
{
	m_radioOrder = DE::ORDER_BIRTH;
}
void CDescendants::OnOrderLength()
{
	m_radioOrder = DE::ORDER_INCREASING;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnOrderDecreasing()
{
	m_radioOrder = DE::ORDER_DECREASING;
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
	people += spouses;
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