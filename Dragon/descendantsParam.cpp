// GaDescendants2.cpp : implementation file
//
// Ha nõk leszármazottait is listázzuk, akkor némely esetben olyan hatalmas leszármazotti lista keletkezne, ami 
// olyan hatást kelt, hogy legagyna a program. Lehet, hogy le is fagy, vagyis a html fájl összetört sorokat tartalmaz,
// kezelhetetlen az egész


#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "descendants.h"
#include <winuser.h>
#include "ProgressWnd.h"
#include "ColorButton.h"
#include <wingdi.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::parameters()
{

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
		m_combo_FontSize.InsertString( i, str);
	}

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
	colorCode.SetTextColor(theApp.m_colorClick);
	colorNum.SetTextColor(theApp.m_colorClick);
	colorOrderText.SetTextColor(theApp.m_colorClick);

	setParameters();
	updateParameters();
	if (theApp.m_inputMode == GEDCOM)
	{
		
		GetDlgItem(IDC_CHECK_FOLYT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_CONNECT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPEATED)->EnableWindow(false);
		GetDlgItem(IDC_GENMAX_TEXT)->EnableWindow(false);
		GetDlgItem(IDC_GENMAX)->EnableWindow(false);
	}

	if (theApp.m_inputMode == GAHTML)
	{

//		GetDlgItem(IDC_GROUP_ORDER_TEXT)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ORDER)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ORDER1)->EnableWindow(false);
	}

	if (theApp.v_rowid.size() < 2 && theApp.v_tableNumbers.size() < 2 )
	{
		m_radioOne = true;
		UpdateData(TOSCREEN);
		GetDlgItem(IDC_RADIO_ONE)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_ONE1)->EnableWindow(false);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::setParameters()
{
	m_checkConnect = theApp.GetProfileInt(L"dragon", L"m_checkConnect", 0);
	m_checkMother = theApp.GetProfileInt(L"dragon", L"m_checkMother", 1);
	m_checkDeleteRepeated = theApp.GetProfileInt(L"dragon", L"m_checkDeleteRepeated", 1);
	m_checkFolyt = theApp.GetProfileInt(L"dragon", L"m_checkFolyt", 0);
	m_checkCapital = theApp.GetProfileInt(L"dragon", L"m_checkCapital", 0);
	//m_check_titolo = theApp.GetProfileInt(L"dragon", L"m_check_titolo", 0);


	m_radioOne = theApp.GetProfileInt(L"dragon", L"m_radioOne", true);

	m_orderBirth = theApp.GetProfileInt(L"dragon", L"m_orderBirth", 0);

	m_editWidth = theApp.GetProfileString(L"dragon", L"m_editWidth", L"0");
	m_editGenMax = theApp.GetProfileString(L"dragon", L"m_editGenMax", L"");


	m_radioDNameX = theApp.GetProfileInt(L"dragon", L"m_radioDNameX", 1);
	m_radioNumbering = theApp.GetProfileInt(L"dragon", L"m_radioNumbering", 2);
	m_radioCode = theApp.GetProfileInt(L"dragon", L"m_radioCode", ANSI);


	m_comboDAttrib = theApp.GetProfileInt(L"dragon", L"m_comboDAttrib", 1);
	m_comboFontSize = theApp.GetProfileInt(L"dragon", L"m_comboFontSize", 3);
	m_comboComment = theApp.GetProfileInt(L"dragon", L"m_comboComment", 2);
	m_comboSpec = theApp.GetProfileInt(L"dragon", L"m_comboSpec", 0);
	m_comboOtherName = theApp.GetProfileInt(L"dragon", L"m_comboOtherName", 1);
	
	m_colorBgrnd = theApp.GetProfileInt(L"dragon", L"m_colorBgrnd", WHITE);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedDefault()
{
	m_checkConnect = 1;
	m_checkMother = 1;
	m_checkDeleteRepeated = 1;
	m_checkFolyt = 0;
	m_checkCapital = 0;
	m_radioOne = true;

	m_orderBirth = false;

	m_editWidth = L"0";
	m_editGenMax.Empty();

	m_radioDNameX = 2;
	m_radioNumbering = SZLUHA;
	m_radioCode = ANSI;

	m_comboDAttrib = 1;
	m_comboFontSize = 3;
	m_comboComment = 2;
	m_comboSpec = 0;
	m_comboOtherName = 1;

	m_colorBgrnd = WHITE;
	updateParameters();

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

	UpdateData(TOSCREEN);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRadioDName()
{
	if (m_radioDNameX == 0)									// leszármazott családnevét nem írjuk ki
	{
//		m_check_titolo = false;
		m_comboKiemeltAttrib = 0;

//		GetDlgItem(IDC_CHECK_TITOLO)->EnableWindow(false);
//		GetDlgItem(IDC_STATIC_KIEMELT)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_KIEMELT)->EnableWindow(false);

		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
	}
	else if (m_radioDNameX == 1)								// családnév a leszármazotti sorban
	{
		m_comboKiemeltAttrib = 0;

//		GetDlgItem(IDC_CHECK_TITOLO)->EnableWindow(true);
//		GetDlgItem(IDC_STATIC_KIEMELT)->EnableWindow(false);
//		GetDlgItem(IDC_COMBO_KIEMELT)->EnableWindow(false);

		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(FALSE);
	}
	else if (m_radioDNameX == 2)								// leszármazott családneve kiemelve
	{
//		m_check_titolo = false;
		m_comboKiemeltAttrib = 3;

//		GetDlgItem(IDC_STATIC_KIEMELT)->EnableWindow(true);
//		GetDlgItem(IDC_COMBO_KIEMELT)->EnableWindow(true);



		((CButton*)GetDlgItem(IDC_RADIO_NOFAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FAMILYNAMEUP))->SetCheck(TRUE);
	}
//	m_combo_KiemeltAttrib.SetCurSel(m_comboKiemeltAttrib);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::updateRadioNumbering()
{
	if (m_radioNumbering == SZLUHA)
	{
		((CButton*)GetDlgItem(IDC_SZLUHA))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_VILLERS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TUPIGNY))->SetCheck(FALSE);
	}
	else if( m_radioNumbering == VIL )
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedCheckConnect()
{
	if (m_checkMother)					// ha nõk lszrámazoittait is listázzuk, akkor mindenképpen összekötés kell
	{
		m_checkConnect = true;
		UpdateData();
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

{
	m_check_titolo = !m_check_titolo;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedSzluha()
{
	m_radioNumbering = SZLUHA;
	updateRadioNumbering();
}
void CDescendants::OnVillers()
{
	m_radioNumbering = VIL;
	updateRadioNumbering();
}
void CDescendants::OnTupigny()
{
	m_radioNumbering = TUP;
	updateRadioNumbering();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnClickedRadioNofamilyname()
{
	m_radioDNameX	= 0;
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

	m_comboDAttrib		= m_combo_DAttrib.GetCurSel();
	m_comboOtherName	= m_combo_OtherName.GetCurSel();
	m_comboSpec			= m_combo_spec.GetCurSel();
	m_comboComment		= m_combo_comment.GetCurSel();
	m_comboFontSize		= m_combo_FontSize.GetCurSel();
	
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

	theApp.WriteProfileInt(L"dragon", L"m_checkConnect", m_checkConnect);
	theApp.WriteProfileInt(L"dragon", L"m_checkMother", m_checkMother);
	theApp.WriteProfileInt(L"dragon", L"m_checkDeleteRepeated", m_checkDeleteRepeated);
	theApp.WriteProfileInt(L"dragon", L"m_checkFolyt", m_checkFolyt);
//	theApp.WriteProfileInt(L"dragon", L"m_check_titolo", m_check_titolo);

	theApp.WriteProfileInt(L"dragon", L"m_orderBirth", m_orderBirth);

	theApp.WriteProfileString(L"dragon", L"m_editWidth", m_editWidth);
	theApp.WriteProfileString(L"dragon", L"m_editGenMax", m_editGenMax);

	theApp.WriteProfileInt(L"dragon", L"m_radioDNameX", m_radioDNameX );
	theApp.WriteProfileInt(L"dragon", L"m_comboDAttrib", m_comboDAttrib );
	theApp.WriteProfileInt(L"dragon", L"m_comboOtherName", m_comboOtherName);
	theApp.WriteProfileInt(L"dragon", L"m_comboComment", m_comboComment);
	theApp.WriteProfileInt(L"dragon", L"m_comboFontSize", m_comboFontSize);
	theApp.WriteProfileInt(L"dragon", L"m_comboSpec", m_comboSpec);
	theApp.WriteProfileInt(L"dragon", L"m_radioNumbering", m_radioNumbering);
	theApp.WriteProfileInt(L"dragon", L"m_colorBgrnd", m_colorBgrnd);
	theApp.WriteProfileInt(L"dragon", L"m_radioCode", m_radioCode);
	theApp.WriteProfileInt(L"dragon", L"m_checkCapital", m_checkCapital);
	theApp.WriteProfileInt(L"dragon", L"m_radioOne", m_radioOne);
	
	CDialogEx::OnClose();

	if (theApp.v_rowid.size() )
		treePeople();
	else if ( theApp.v_tableNumbers.size() )
		treeTables();
	
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////