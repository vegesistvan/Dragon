#pragma once
#include "afxdialogex.h"


// CUniteParam dialog

class CUniteParam : public CDialogEx
{
	DECLARE_DYNAMIC(CUniteParam)

public:
	CUniteParam(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUniteParam();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITE_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit m_editCtrl;
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedRadioNotenough();
	afx_msg void OnRadioEnough();
	BOOL m_snameEnough;
};
