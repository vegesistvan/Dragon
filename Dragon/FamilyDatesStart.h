#pragma once
#include "afxwin.h"


// CFamilyDatesStart dialog

class CFamilyDatesStart : public CDialogEx
{
	DECLARE_DYNAMIC(CFamilyDatesStart)

public:
	CFamilyDatesStart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFamilyDatesStart();

// Dialog Data
	enum { IDD = IDD_FAMILYDATES_START };

//	int m_type;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

//	CButton m_RadioFamily;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedRadioNochildren();
	afx_msg void OnRadioMorewifes();
	afx_msg void OnRadioOnewife();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int m_type;
};
