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

class CDescendantsP : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendantsP)

public:
	CDescendantsP(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendantsP();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTSP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;

	void CDescendantsP::setParameters();
	void CDescendantsP::OnClickedDefault();
	void CDescendantsP::updateParameters();
	void CDescendantsP::updateRadioDName();
	void CDescendantsP::updateRepeated();
	void CDescendantsP::updateRadioNumbering();
	void CDescendantsP::OnClickedCheckConnect();
	void CDescendantsP::OnClickedCheckWoman();
	void CDescendantsP::OnClickedCheckFolyt();
	void CDescendantsP::OnClickedCheckCapital();
	void CDescendantsP::OnClickedSzluha();
	void CDescendantsP::OnVillers();
	void CDescendantsP::OnTupigny();
	void CDescendantsP::OnClickedRadioNofamilyname();
	void CDescendantsP::OnRadioFamilyname();
	void CDescendantsP::OnRadioFamilynameup();
	void CDescendantsP::OnClickedStaticBackground();
	void CDescendantsP::OnBnClickedOk();
	void CDescendantsP::colorFrame(CPaintDC* dc, int IDC, COLORREF color);

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
	BOOL m_checkConnect;
	BOOL m_checkMother;
	BOOL m_ceckDeleteRepeated;

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
//	CColorStatic colorCode;
	CColorStatic colorAttrib;
	CColorStatic colorContent;
	CColorStatic colorOutput;
	CColorStatic colorPrint;

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
};
