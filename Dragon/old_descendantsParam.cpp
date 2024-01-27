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
	, p_connect(true)			// t�bl�kat �sszek�sse-e
	, p_womenDescendants(false)			// n�k lesz�rmazottait list�zza-e
	, p_capital(false)
	, p_bold(FALSE)
	, p_otherNameAttrib(1)			// m�s n�v bold
	, p_comboSpect(0)				// speci�lis karakterek (*+=) bold
	, p_commentAttrib(2)				// comment bold
	, p_fontSize(3)

	, p_numberingSystem(DE::TUPP)		// milyen sz�moz�si rendszer legyen (0,1,2) SZLUHA/VIL/TUP

	, m_tableNumber(L"")			// tablenumber, ha a t�bl�zat lesz�rmazotti list�j�t k�rj�k
	, p_rowWidth(L"k�perny�")
	, m_printed(FALSE)
	, p_folyt(FALSE)
	, p_generationMax(_T(""))
	, p_oneOutputFile(true)
	, p_repeated(0)
	, p_repeatedColor(FALSE)
	, p_checkCRLF(FALSE)
	, p_childrenOrder(0)
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
	DDX_Check(pDX, IDC_CHECK_FOLYT, p_folyt);

	DDX_Control(pDX, IDC_COMBO_COMMENT, m__commentAttribCombo);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_specAttribCombo);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_fontSizeCombo);

	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, colorBgn);

	DDX_Control(pDX, IDC_COMBO_NAME, m_otherNameAttribCombo);
	DDX_Control(pDX, IDC_COMBO_DESCATTRIB, m_descendantAttribCombo);
	//  DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_RadioFamily);
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
	DDX_Control(pDX, IDC_COMBO_HTMTXT, m_comboHtmlTxt);
	DDX_Check(pDX, IDC_CHECK_CRLF, p_checkCRLF);
	DDX_Radio(pDX, IDC_ORDER_INPUT, p_childrenOrder);
	DDX_Control(pDX, IDC_ORDER_TXT, colorOrder);
	DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_descendantNameRadio);
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
		m_descendantAttribCombo.InsertString(i, attrib[i].text);
		m_otherNameAttribCombo.InsertString(i, attrib[i].text);
		m_specAttribCombo.InsertString(i, attrib[i].text);
		m__commentAttribCombo.InsertString(i, attrib[i].text);
		//		m_combo_KiemeltAttrib.InsertString(i, attrib[i].text);
	}
	for (INT i = 0; i < 6; ++i)
	{
		str.Format(L"%d", 10 + i * 2);
		m_fontSizeCombo.InsertString(i, str);
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
		p_oneOutputFile = true;
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
	p_folyt = theApp.GetProfileInt(L"dragon", L"p_folyt", 0);
	p_capital = theApp.GetProfileInt(L"dragon", L"p_capital", 0);
	p_bold = theApp.GetProfileInt(L"dragon", L"p_bold", 0);

	p_oneOutputFile = theApp.GetProfileInt(L"dragon", L"p_oneOutputFile", true);
	p_descendantName = theApp.GetProfileInt(L"dragon", L"p_descendantName", 1);
	p_numberingSystem = theApp.GetProfileInt(L"dragon", L"p_numberingSystem", 2);

	p_rowWidth = theApp.GetProfileString(L"dragon", L"p_rowWidth", L"0");
	p_generationMax = theApp.GetProfileString(L"dragon", L"p_generationMax", L"");


	p_repeated = theApp.GetProfileInt(L"dragon", L"p_repeated", 0);
	p_repeatedColor = theApp.GetProfileInt(L"dragon", L"p_repeatedColor", 0);

	p_descendantAttrib = theApp.GetProfileInt(L"dragon", L"p_descendantAttrib", 1);
	p_fontSize = theApp.GetProfileInt(L"dragon", L"p_fontSize", 3);
	p_commentAttrib = theApp.GetProfileInt(L"dragon", L"p_commentAttrib", 2);
	p_comboSpect = theApp.GetProfileInt(L"dragon", L"p_comboSpect", 0);
	p_otherNameAttrib = theApp.GetProfileInt(L"dragon", L"p_otherNameAttrib", 1);

	p_colorBgrnd = theApp.GetProfileInt(L"dragon", L"p_colorBgrnd", WHITE);
	p_checkCRLF = theApp.GetProfileInt(L"dragon", L"p_checkCRLF", WHITE);

	p_childrenOrder = theApp.GetProfileInt(L"dragon", L"p_childrenOrder", WHITE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedDefault()
{
	m_comboHtmlTxt.SetCurSel(0);
	p_connect = true;
	p_womenDescendants = true;
	p_folyt = false;
	p_capital = false;
	p_bold = false;
	p_checkCRLF = false;
	p_oneOutputFile = true;
	
	p_rowWidth = L"0";
	p_generationMax.Empty();

	p_repeatedColor = false;
	p_descendantName = RAISED;
	p_numberingSystem = SZLUHA;
	p_childrenOrder = 0;

	p_descendantAttrib = 1;
	p_fontSize = 3;
	p_commentAttrib = 2;
	p_comboSpect = 0;
	p_otherNameAttrib = 1;

	p_colorBgrnd = WHITE;

	p_repeated =  0;
	p_repeatedColor = 0;

	p_childrenOrder = 0;

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
	m_otherNameAttribCombo.SetCurSel(p_otherNameAttrib);
	m_specAttribCombo.SetCurSel(p_comboSpect);
	m__commentAttribCombo.SetCurSel(p_commentAttrib);
	m_descendantAttribCombo.SetCurSel(p_descendantAttrib);
	m_fontSizeCombo.SetCurSel(p_fontSize);


	colorBgn.SetColor(BLACK, p_colorBgrnd);

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
	if (p_descendantName == NOINLINE)									// lesz�rmazott csal�dnev�t nem �rjuk ki
	{
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
		p_bold = false;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(false);
	}
	else if (p_descendantName == INLINE)								// csal�dn�v a lesz�rmazotti sorban
	{
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(true);
	}
	else if (p_descendantName == RAISED)								// lesz�rmazott csal�dneve kiemelve
	{
		m_descendantNameRadio.SetCheck(2);
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
	if (p_numberingSystem == SZLUHA)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else if (p_numberingSystem == VIL)
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
	switch (p_repeated)
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
	if (p_womenDescendants)					// ha n�k lszr�mazoittait is list�zzuk, akkor mindenk�ppen �sszek�t�s kell
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckWoman()
{
	p_womenDescendants = !p_womenDescendants;				// ha a n�k cgyerekeit is list�zni akarjuk, akkor a t�bl�kat is �ssze kell k�tni!!
	if (p_womenDescendants)
	{
		p_connect = true;
		UpdateData(TOSCREEN);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedCheckFolyt()
{
	p_folyt = !p_folyt;
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
		p_commentAttrib = 0;
		p_descendantAttrib = 0;
		p_otherNameAttrib = 0;
		p_comboSpect = 0;
		m_otherNameAttribCombo.SetCurSel(p_otherNameAttrib);
		m_specAttribCombo.SetCurSel(p_comboSpect);
		m__commentAttribCombo.SetCurSel(p_commentAttrib);
		m_descendantAttribCombo.SetCurSel(p_descendantAttrib);

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
	p_numberingSystem = SZLUHA;
	updateRadioNumbering();
}
void CDescendantsParamOld::OnVillers()
{
	p_numberingSystem = VIL;
	updateRadioNumbering();
}
void CDescendantsParamOld::OnTupigny()
{
	p_numberingSystem = TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedRadioNofamilyname()
{
	p_descendantName = NOINLINE;
	updateRadioDName();
}
void CDescendantsParamOld::OnRadioFamilyname()
{
	p_descendantName = INLINE;
	updateRadioDName();
}
void CDescendantsParamOld::OnRadioFamilynameup()
{
	p_descendantName = RAISED;
	updateRadioDName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedRepeatedAll()
{
	p_repeated = 0;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnRepeatedFirst()
{
	p_repeated = 1;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnRepeatedFather()
{
	p_repeated = 2;
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

	p_colorBgrnd = GetRValue(bgn) << 16 | GetGValue(bgn) << 8 | GetBValue(bgn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnBnClickedOk()
{
	UpdateData(FROMSCREEN);


	m_html = m_comboHtmlTxt.GetCurSel();
	p_descendantAttrib = m_descendantAttribCombo.GetCurSel();
	p_otherNameAttrib = m_otherNameAttribCombo.GetCurSel();
	p_comboSpect = m_specAttribCombo.GetCurSel();
	p_commentAttrib = m__commentAttribCombo.GetCurSel();
	p_fontSize = m_fontSizeCombo.GetCurSel();

	p_descendantName = m_descendantNameRadio.GetCheck();

	if (p_generationMax.IsEmpty())
		m_generationMax = INT_MAX;
	else
	{
		m_generationMax = _wtoi(p_generationMax);
		if (m_generationMax < 3)
		{
			AfxMessageBox(L"A maxim�lis list�zand� gener�ci�k sz�ma 2-n�l nagyobbnak kell lenni!", MB_ICONEXCLAMATION);
			return;
		}
	}

	p_oneOutputFile = p_oneOutputFile;

	int z = p_repeated;
	theApp.WriteProfileInt(L"dragon", L"p_connect", p_connect);
	theApp.WriteProfileInt(L"dragon", L"p_womenDescendants", p_womenDescendants);
	theApp.WriteProfileInt(L"dragon", L"p_repeated", p_repeated);
	theApp.WriteProfileInt(L"dragon", L"p_repeatedColor", p_repeatedColor);
	theApp.WriteProfileInt(L"dragon", L"p_folyt", p_folyt);

	theApp.WriteProfileString(L"dragon", L"p_rowWidth", p_rowWidth);
	theApp.WriteProfileString(L"dragon", L"p_generationMax", p_generationMax);

	theApp.WriteProfileInt(L"dragon", L"p_descendantName", p_descendantName);
	theApp.WriteProfileInt(L"dragon", L"p_descendantAttrib", p_descendantAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_otherNameAttrib", p_otherNameAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_commentAttrib", p_commentAttrib);
	theApp.WriteProfileInt(L"dragon", L"p_fontSize", p_fontSize);
	theApp.WriteProfileInt(L"dragon", L"p_comboSpect", p_comboSpect);
	theApp.WriteProfileInt(L"dragon", L"p_numberingSystem", p_numberingSystem);
	theApp.WriteProfileInt(L"dragon", L"p_childrenOrder", p_childrenOrder);
	theApp.WriteProfileInt(L"dragon", L"p_colorBgrnd", p_colorBgrnd);
	theApp.WriteProfileInt(L"dragon", L"p_capital", p_capital);
	theApp.WriteProfileInt(L"dragon", L"p_bold", p_bold);
	theApp.WriteProfileInt(L"dragon", L"p_checkCRLF", p_checkCRLF);
	theApp.WriteProfileInt(L"dragon", L"p_oneOutputFile", p_oneOutputFile);

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


	rc.TopLeft().x -= 1;	// a sz�rke keret mellett kell rajzolni, mert k�l�nben a keret fel�l�rja a sz�nes n�gyzetet
	rc.BottomRight().x += 1;	// ez szebb, mint az al�bbi vonalas megold�s, de kifeh�r�tia n�gysz�g�n bel�l �s a sz�vegek h�tt�rsz�ne sz�rke marad.
	rc.TopLeft().y += 5;	// Ez�rt m�gis szebb a vonalas rajzol�s

	COLORREF colour = ::GetSysColor(COLOR_3DFACE);
	HBRUSH hbr = CreateSolidBrush(colour);
	dc->SelectObject(hbr);	// hogy a h�tt�r olyan sz�rke legyen, mint a benne l�v� control-ok (sz�veg, gomb )

	dc->Rectangle(&rc);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnClickedOrderInput()
{
	p_childrenOrder = ORDER_INPUT;
}
void CDescendantsParamOld::OnOrderBirth()
{
	p_childrenOrder = ORDER_BIRTH;
}
void CDescendantsParamOld::OnOrderLength()
{
	p_childrenOrder = ORDER_INCREASING;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsParamOld::OnOrderDecreasing()
{
	p_childrenOrder = ORDER_DECREASING;
}



