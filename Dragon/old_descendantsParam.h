#pragma once
#include "afxdialogex.h"
#include "colorstatic.h"
#include "colorbutton.h"
#include "CDescendants.h"


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
	
	CColorButton	colorBgn;
	
	int p_descendantName;
	int p_descendantAttrib;
	int	p_otherNameAttrib;
	int	p_comboSpect;
	int	p_commentAttrib;
	int p_fontSize;
	int m_html;

	BOOL p_folyt;
	BOOL p_capital;
	BOOL p_bold;
	BOOL p_connect;
	BOOL p_womenDescendants;
	BOOL m_ceckDeleteRepeated;
	BOOL p_checkCRLF;

	CComboBox m_specAttribCombo;
	CComboBox m__commentAttribCombo;
	CComboBox m_fontSizeCombo;
	CComboBox m_otherNameAttribCombo;
	CComboBox m_descendantAttribCombo;

//	CButton m_RadioFamily;
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

	int		p_childrenOrder;
	int		p_numberingSystem;
	int		p_repeated;
	BOOL	p_oneOutputFile;

	UINT	p_colorBgrnd;
	CColorStatic	colorBgrnd;
	
	CString p_rowWidth;
	CString p_generationMax;

	
	
	bool m_printed;
	CString m_tableNumber;

	int m_generationMax;


//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedRepeatedAll();
	afx_msg void OnRepeatedFather();
	afx_msg void OnRepeatedFirst();
	BOOL p_repeatedColor;
	CColorStatic colorRepeated;
	afx_msg void OnPaint();
	CComboBox m_comboHtmlTxt;
	afx_msg void OnClickedCheckBold();

	
	afx_msg void OnClickedOrderInput();
	afx_msg void OnOrderLength();
	afx_msg void OnOrderDecreasing();
	afx_msg void OnOrderBirth();
	CButton m_descendantNameRadio;
};
