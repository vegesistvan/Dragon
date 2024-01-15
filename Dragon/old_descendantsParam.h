#pragma once
#include "afxdialogex.h"
#include "colorstatic.h"
#include "colorbutton.h"


enum
{
	SZLUHAP = 0,
	VILP,
	TUPP,

};

class CDescendantsParamOld : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendantsParamOld)

public:
	CDescendantsParamOld(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendantsParamOld();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;

	void CDescendantsParamOld::setParameters();
	void CDescendantsParamOld::OnClickedDefault();
	void CDescendantsParamOld::updateParameters();
	void CDescendantsParamOld::updateRadioDName();
	void CDescendantsParamOld::updateRepeated();
	void CDescendantsParamOld::updateRadioNumbering();
	void CDescendantsParamOld::OnClickedCheckConnect();
	void CDescendantsParamOld::OnClickedCheckWoman();
	void CDescendantsParamOld::OnClickedCheckFolyt();
	void CDescendantsParamOld::OnClickedCheckCapital();
	void CDescendantsParamOld::OnClickedSzluha();
	void CDescendantsParamOld::OnVillers();
	void CDescendantsParamOld::OnTupigny();
	void CDescendantsParamOld::OnClickedRadioNofamilyname();
	void CDescendantsParamOld::OnRadioFamilyname();
	void CDescendantsParamOld::OnRadioFamilynameup();
	void CDescendantsParamOld::OnClickedStaticBackground();
	void CDescendantsParamOld::OnBnClickedOk();
	void CDescendantsParamOld::colorFrame(CPaintDC* dc, int IDC, COLORREF color);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_radioDNameX;
	int m_comboDAttrib;
	int	m_comboOtherName;
	int	m_comboSpec;
	int	m_comboComment;
	int	m_comboKiemeltAttrib;
	int m_comboFontSize;
	int m_html;

	BOOL m_checkFolyt;
	BOOL m_checkCapital;
	BOOL m_checkBold;
	BOOL m_checkConnect;
	BOOL m_checkMother;
	BOOL m_ceckDeleteRepeated;
	BOOL m_checkCRLF;

	CComboBox m_combo_spec;
	CComboBox m_combo_comment;
	CComboBox m_combo_FontSize;
	CComboBox m_combo_OtherName;
	CComboBox m_combo_DAttrib;

	CButton m_RadioFamily;
	CColorStatic colorFamilyInline;
	CColorStatic colorFamilynameNo;
	CColorStatic colorFamilynameUp;

	CColorStatic colorName;
	CColorStatic colorNum;
	CColorStatic colorAttrib;
	CColorStatic colorContent;
	CColorStatic colorOutput;
	CColorStatic colorPrint;
	CColorStatic colorOrder;

	int		m_radioOrder;
	int		m_radioNumbering;
	int		m_repeated;
	BOOL	m_oneFile;

	UINT	m_colorBgrnd;
	CColorStatic	colorBgrnd;
	CColorButton	colorBgn;
	CString m_editWidth;
	CString m_editGenMax;

	BOOL m_radioOne;
	
	bool m_printed;
	CString m_tableNumber;

	int m_generationMax;


//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRepeatedAll();
	afx_msg void OnRepeatedFather();
	afx_msg void OnRepeatedFirst();
	BOOL m_repeatedColor;
	CColorStatic colorRepeated;
	afx_msg void OnPaint();
	CComboBox m_comboHtmlTxt;
	afx_msg void OnClickedCheckBold();

	
	afx_msg void OnClickedOrderInput();
	afx_msg void OnOrderLength();
	afx_msg void OnOrderDecreasing();
	afx_msg void OnOrderBirth();
};
