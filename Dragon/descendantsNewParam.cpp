// descendatsP.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "descendantsNewParam.h"
#include "descendantsNew.h"

// CDescendatsP dialog

IMPLEMENT_DYNAMIC(CDescendantsP, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsP::CDescendantsP(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCENDANTSP, pParent)
	, m_checkConnect(true)		// táblákat összekösse-e
	, m_checkMother(false)			// nõk leszármazottait listázza-e
	, m_checkCapital(false)

	, m_comboOtherName(1)			// más náv bold
	, m_comboSpec(0)			// speciális karakterek (*+=) bold
	, m_comboComment(2)		// comment bold
	, m_comboKiemeltAttrib(3)		// leszármazott kiemelt családneve
	, m_comboFontSize(3)

	, m_radioNumbering(TUPP)		// milyen számozási rendszer legyen (0,1,2) SZLUHA/VIL/TUP

	, m_tableNumber(L"")		// tablenumber, ha a táblázat leszármazotti listáját kérjük
	, m_editWidth(L"képernyõ")
	, m_printed(FALSE)
	, m_checkFolyt(FALSE)
	, m_editGenMax(_T(""))
	, m_radioOne(true)
	, m_repeated(0)
	, m_repeatedColor(FALSE)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendantsP::~CDescendantsP()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_checkConnect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, m_checkMother);

	DDX_Check(pDX, IDC_CHECK_CAPITAL, m_checkCapital);


	DDX_Control(pDX, IDC_COMBO_COMMENT, m_combo_comment);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_combo_spec);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_combo_FontSize);

	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, colorBgn);

	DDX_Control(pDX, IDC_COMBO_NAME, m_combo_OtherName);
	DDX_Control(pDX, IDC_COMBO_DESCATTRIB, m_combo_DAttrib);
	DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_RadioFamily);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Check(pDX, IDC_CHECK_FOLYT, m_checkFolyt);
	DDX_Control(pDX, IDC_STATIC_NAME, colorName);
	DDX_Control(pDX, IDC_STATIC_NUM, colorNum);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_ATTRIB, colorAttrib);
	DDX_Control(pDX, IDC_STATIC_FAMILY_INLINE, colorFamilyInline);
	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_NO, colorFamilynameNo);
	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_UP, colorFamilynameUp);
	DDX_Text(pDX, IDC_GENMAX, m_editGenMax);

	DDX_Radio(pDX, IDC_RADIO_ONE, m_radioOne);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, colorOutput);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_CONTENT, colorContent);
	DDX_Check(pDX, IDC_REPEATED_COLOR, m_repeatedColor);
	DDX_Control(pDX, IDC_STATIC_REPEATED, colorRepeated);
	DDX_Control(pDX, IDC_COMBO_HTMTXT, m_comboHtmlTxt);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendantsP, CDialogEx)

	ON_BN_CLICKED(IDC_SZLUHA, &CDescendantsP::OnClickedSzluha)
	ON_COMMAND(IDC_TUPIGNY, &CDescendantsP::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CDescendantsP::OnVillers)

	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CDescendantsP::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDescendantsP::OnClickedCheckConnect)

	ON_BN_CLICKED(IDC_CHECK_CAPITAL, &CDescendantsP::OnClickedCheckCapital)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND, &CDescendantsP::OnClickedStaticBackground)
	ON_BN_CLICKED(IDC_RADIO_NOFAMILYNAME, &CDescendantsP::OnClickedRadioNofamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAME, &CDescendantsP::OnRadioFamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAMEUP, &CDescendantsP::OnRadioFamilynameup)
	ON_BN_CLICKED(IDC_CHECK_FOLYT, &CDescendantsP::OnClickedCheckFolyt)

	ON_BN_CLICKED(IDC_DEFAULT, &CDescendantsP::OnClickedDefault)
	ON_BN_CLICKED(IDOK, &CDescendantsP::OnBnClickedOk)

	ON_BN_CLICKED(IDC_REPEATED_ALL, &CDescendantsP::OnClickedRepeatedAll)
	ON_COMMAND(IDC_REPEATED_FIRST, &CDescendantsP::OnRepeatedFirst)
	ON_COMMAND(IDC_REPEATED_FATHER, &CDescendantsP::OnRepeatedFather)

	ON_WM_PAINT()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendantsP::OnInitDialog()
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


	setParameters();
	updateParameters();
	
	if (theApp.m_inputMode == GEDCOM)
	{

		GetDlgItem(IDC_CHECK_FOLYT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_CONNECT)->EnableWindow(false);
//		GetDlgItem(IDC_CHECK_REPEATED)->EnableWindow(false);
//		GetDlgItem(IDC_GENMAX_TEXT)->EnableWindow(false);
//		GetDlgItem(IDC_GENMAX)->EnableWindow(false);
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
void CDescendantsP::setParameters()
{
	m_checkConnect = theApp.GetProfileInt(L"dragon", L"m_checkConnect", 0);
	m_checkMother = theApp.GetProfileInt(L"dragon", L"m_checkMother", 1);
	m_checkFolyt = theApp.GetProfileInt(L"dragon", L"m_checkFolyt", 0);
	m_checkCapital = theApp.GetProfileInt(L"dragon", L"m_checkCapital", 0);

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

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedDefault()
{
	m_comboHtmlTxt.SetCurSel(0);
	m_checkConnect = true;
	m_checkMother = true;
	m_checkFolyt = false;
	m_checkCapital = false;
	m_radioOne = true;
	
	m_editWidth = L"0";
	m_editGenMax.Empty();

	m_repeatedColor = false;
	m_radioDNameX = 2;
	m_radioNumbering = SZLUHA;
	
	m_comboDAttrib = 1;
	m_comboFontSize = 3;
	m_comboComment = 2;
	m_comboSpec = 0;
	m_comboOtherName = 1;

	m_colorBgrnd = WHITE;

	m_repeated =  0;
	m_repeatedColor = 0;
	updateParameters();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::updateParameters()
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
void CDescendantsP::updateRadioDName()
{
	if (m_radioDNameX == 0)									// leszármazott családnevét nem írjuk ki
	{
		m_comboKiemeltAttrib = 0;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
	}
	else if (m_radioDNameX == 1)								// családnév a leszármazotti sorban
	{
		m_comboKiemeltAttrib = 0;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
	}
	else if (m_radioDNameX == 2)								// leszármazott családneve kiemelve
	{
		m_RadioFamily.SetCheck(2);
		m_comboKiemeltAttrib = 3;
		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(TRUE);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::updateRadioNumbering()
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
void CDescendantsP::updateRepeated()
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
void CDescendantsP::OnClickedCheckConnect()
{
	if (m_checkMother)					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
	{
		m_checkConnect = true;
		UpdateData(TOSCREEN);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedCheckWoman()
{
	m_checkMother = !m_checkMother;				// ha a nõk cgyerekeit is listázni akarjuk, akkor a táblákat is össze kell kötni!!
	if (m_checkMother)
	{
		m_checkConnect = true;
		UpdateData(TOSCREEN);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedCheckFolyt()
{
	m_checkFolyt = !m_checkFolyt;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedCheckCapital()
{
	m_checkCapital = !m_checkCapital;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedSzluha()
{
	m_radioNumbering = SZLUHA;
	updateRadioNumbering();
}
void CDescendantsP::OnVillers()
{
	m_radioNumbering = VIL;
	updateRadioNumbering();
}
void CDescendantsP::OnTupigny()
{
	m_radioNumbering = TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedRadioNofamilyname()
{
	m_radioDNameX = 0;
	updateRadioDName();
}
void CDescendantsP::OnRadioFamilyname()
{
	m_radioDNameX = 1;
	updateRadioDName();
}
void CDescendantsP::OnRadioFamilynameup()
{
	m_radioDNameX = 2;
	updateRadioDName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedRepeatedAll()
{
	m_repeated = 0;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnRepeatedFirst()
{
	m_repeated = 1;
	updateRepeated();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnRepeatedFather()
{
	m_repeated = 2;
	updateRepeated();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnClickedStaticBackground()
{
	CMFCColorDialog dlgColors;
	dlgColors.SetCurrentColor(WHITE);
	if (dlgColors.DoModal() == IDCANCEL) return;

	COLORREF bgn = dlgColors.GetColor();
	colorBgn.SetColor(BLACK, bgn);

	m_colorBgrnd = GetRValue(bgn) << 16 | GetGValue(bgn) << 8 | GetBValue(bgn);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnBnClickedOk()
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
	theApp.WriteProfileInt(L"dragon", L"m_colorBgrnd", m_colorBgrnd);
	theApp.WriteProfileInt(L"dragon", L"m_checkCapital", m_checkCapital);
	theApp.WriteProfileInt(L"dragon", L"m_radioOne", m_radioOne);

	CDialogEx::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::OnPaint()
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
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendantsP::colorFrame(CPaintDC* dc, int IDC, COLORREF color)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
HBRUSH CDescendantsP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int IDC = pWnd->GetDlgCtrlID();
	return hbr;

}
*/

