#pragma once
#include "afxdialogex.h"

class CUniteParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CUniteParameters)

public:
	CUniteParameters(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUniteParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITE_PARAMETERS };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInfo();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
private:
	CEdit m_nameCtrl;
	
public:
	CString m_name;
	int		m_cycle;
	CColorStatic colorWarning;
	CString m_rowid1;
	CString m_rowid2;
	CComboBox m_comboCycleCtrl;
	afx_msg void OnClickedClear();
	CComboBox ComboAlgoCtrl;
	int m_algorithm;
	afx_msg void OnClickedCheckSpouse();
//	bool m_snameEnough;
	BOOL m_snameEnough;
};
