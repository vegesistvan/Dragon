// descendatsP.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "old_descendantsParam.h"
#include "old_descendants.h"

// CDescendatsP dialog

IMPLEMENT_DYNAMIC(CDescendantsParamOld, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsParamOld::CDescendantsParamOld(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTS_PARAM, pParent)
	, p_connect(true)			// táblákat összekösse-e
	, p_womenDescendants(false)			// nõk leszármazottait listázza-e
	, p_capital(false)
	, p_bold(FALSE)
	, m_comboOtherName(1)			// más náv bold
	, m_comboSpec(0)				// speciális karakterek (*+=) bold
	, m_comboComment(2)				// comment bold
	, m_comboKiemeltAttrib(3)		// leszármazott kiemelt családneve
	, m_comboFontSize(3)

	, m_radioNumbering(TUPP)		// milyen számozási rendszer legyen (0,1,2) SZLUHA/VIL/TUP

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
CDescendantsParamOld::~CDescendantsParamOld()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CONNECT, p_connect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, p_womenDescendants);

	DDX_Check(pDX, IDC_CHECK_CAPITAL, p_capital);
	DDX_Check(pDX, IDC_CHECK_BOLD, p_bold);
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
BEGIN_MESSAGE_MAP(CDescendantsParamOld, CDialogEx)

	ON_BN_CLICKED(IDC_SZLUHA, &CDescendantsParamOld::OnClickedSzluha)
	ON_COMMAND(IDC_TUPIGNY, &CDescendantsParamOld::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CDescendantsParamOld::OnVillers)

	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CDescendantsParamOld::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDescendantsParamOld::OnClickedCheckConnect)

	ON_BN_CLICKED(IDC_CHECK_CAPITAL, &CDescendantsParamOld::OnClickedCheckCapital)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND, &CDescendantsParamOld::OnClickedStaticBackground)
	ON_BN_CLICKED(IDC_RADIO_NOFAMILYNAME, &CDescendantsParamOld::OnClickedRadioNofamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAME, &CDescendantsParamOld::OnRadioFamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAMEUP, &CDescendantsParamOld::OnRadioFamilynameup)
	ON_BN_CLICKED(IDC_CHECK_FOLYT, &CDescendantsParamOld::OnClickedCheckFolyt)

	ON_BN_CLICKED(IDC_DEFAULT, &CDescendantsParamOld::OnClickedDefault)
	ON_BN_CLICKED(IDOK, &CDescendantsParamOld::OnBnClickedOk)

	ON_BN_CLICKED(IDC_REPEATED_ALL, &CDescendantsParamOld::OnClickedRepeatedAll)
	ON_COMMAND(IDC_REPEATED_FIRST, &CDescendantsParamOld::OnRepeatedFirst)
	ON_COMMAND(IDC_REPEATED_FATHER, &CDescendantsParamOld::OnRepeatedFather)

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_BOLD, &CDescendantsParamOld::OnClickedCheckBold)
	ON_BN_CLICKED(IDC_ORDER_INPUT, &CDescendantsParamOld::OnClickedOrderInput)
	ON_COMMAND(IDC_ORDER_LENGTH, &CDescendantsParamOld::OnOrderLength)
	ON_COMMAND(IDC_ORDER_DECREASING, &CDescendantsParamOld::OnOrderDecreasing)
	ON_COMMAND(IDC_ORDER_BIRTH, &CDescendantsParamOld::OnOrderBirth)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsParamOld::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < sizeof(attrib) / sizeof(ATTRIB); ++i)
	{
		m_combo_DAttrib.InsertString(i, attrib[i].text);
		m_combo_OtherName.InsertString(i, attrib[i].text);
		m_combo_spec.InsertString(i, attrib[i].text);
		m_combo_comment.InsertString(i, attrib[i].text);
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
void CDescendantsParamOld::setParameters()
{
	p_connect = theApp.GetProfileInt(L"dragon", L"p_connect", 0);
	p_womenDescendants = theApp.GetProfileInt(L"dragon", L"p_womenDescendants", 1);
	m_checkFolyt = theApp.GetProfileInt(L"dragon", L"m_checkFolyt", 0);
	p_capital = theApp.GetProfileInt(L"dragon", L"p_capital", 0);
	p_bold = theApp.GetProfileInt(L"dragon", L"p_bold", 0);

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
void CDescendantsParamOld::OnClickedDefault()
{
	m_comboHtmlTxt.SetCurSel(0);
	p_connect = true;
	p_womenDescendants = true;
	m_checkFolyt = false;
	p_capital = false;
	p_bold = false;
	m_checkCRLF = false;
	m_radioOne = true;
	
	m_editWidth = L"0";
	m_editGenMax.Empty();

	m_repeatedColor = false;
	m_radioDNameX = 2;
	m_radioNumbering = SZLUHA;
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
void CDescendantsParamOld::updateParameters()
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
void CDescendantsParamOld::updateRadioDName()
{
	if (m_radioDNameX == 0)									// leszármazott családnevét nem írjuk ki
	{
		m_comboKiemeltAttrib = 0;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
		p_bold = false;
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
		p_bold = false;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(false);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::updateRadioNumbering()
{
	if (m_radioNumbering == SZLUHA)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else if (m_radioNumbering == VIL)
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
void CDescendantsParamOld::updateRepeated()
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
void CDescendantsParamOld::OnClickedCheckConnect()
{
	if (p_womenDescendants)					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckWoman()
{
	p_womenDescendants = !p_womenDescendants;				// ha a nõk cgyerekeit is listázni akarjuk, akkor a táblákat is össze kell kötni!!
	if (p_womenDescendants)
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckFolyt()
{
	m_checkFolyt = !m_checkFolyt;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckCapital()
{
	p_capital = !p_capital;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckBold()
{
	p_bold = !p_bold;
	if (p_bold == true)
	{
		p_capital = false;
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
void CDescendantsParamOld::OnClickedSzluha()
{
	m_radioNumbering = SZLUHA;
	updateRadioNumbering();
}
void CDescendantsParamOld::OnVillers()
{
	m_radioNumbering = VIL;
	updateRadioNumbering();
}
void CDescendantsParamOld::OnTupigny()
{
	m_radioNumbering = TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedRadioNofamilyname()
{
	m_radioDNameX = 0;
	updateRadioDName();
}
void CDescendantsParamOld::OnRadioFamilyname()
{
	m_radioDNameX = 1;
	updateRadioDName();
}
void CDescendantsParamOld::OnRadioFamilynameup()
{
	m_radioDNameX = 2;
	updateRadioDName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedRepeatedAll()
{
	m_repeated = 0;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnRepeatedFirst()
{
	m_repeated = 1;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnRepeatedFather()
{
	m_repeated = 2;
	updateRepeated();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedStaticBackground()
{
	CMFCColorDialog dlgColors;
	dlgColors.SetCurrentColor(WHITE);
	if (dlgColors.DoModal() == IDCANCEL) return;

	COLORREF bgn = dlgColors.GetColor();
	colorBgn.SetColor(BLACK, bgn);

	m_colorBgrnd = GetRValue(bgn) << 16 | GetGValue(bgn) << 8 | GetBValue(bgn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnBnClickedOk()
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
	theApp.WriteProfileInt(L"dragon", L"p_connect", p_connect);
	theApp.WriteProfileInt(L"dragon", L"p_womenDescendants", p_womenDescendants);
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
	theApp.WriteProfileInt(L"dragon", L"p_capital", p_capital);
	theApp.WriteProfileInt(L"dragon", L"p_bold", p_bold);
	theApp.WriteProfileInt(L"dragon", L"m_checkCRLF", m_checkCRLF);
	theApp.WriteProfileInt(L"dragon", L"m_radioOne", m_radioOne);

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnPaint()
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
void CDescendantsParamOld::colorFrame(CPaintDC* dc, int IDC, COLORREF color)
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
void CDescendantsParamOld::OnClickedOrderInput()
{
	m_radioOrder = ORDER_INPUT;
}
void CDescendantsParamOld::OnOrderBirth()
{
	m_radioOrder = ORDER_BIRTH;
}
void CDescendantsParamOld::OnOrderLength()
{
	m_radioOrder = ORDER_INCREASING;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnOrderDecreasing()
{
	m_radioOrder = ORDER_DECREASING;
}



