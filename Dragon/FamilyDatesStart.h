#pragma once
#include "afxwin.h"
#include "colorstatic.h"


// CFamilyDatesStart dialog

class CFamilyDatesStart : public CDialogEx
{
	DECLARE_DYNAMIC(CFamilyDatesStart)

public:
	CFamilyDatesStart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFamilyDatesStart();
	enum { IDD = IDD_FAMILYDATES_START };

	// átadott adatok
	CString m_info;

	int	m_type;
    int	m_maxLifespan;
	int	m_maxDiffBetweenHW;
	int	m_maxAgeHAtWedd;
	int	m_minAgeHAtWedd;
	int	m_maxAgeWAtWedd;
	int	m_minAgeWAtWedd;
	int	m_maxDiffFC;
	int	m_minDiffFC;
	int	m_minDiffMC;
	int	m_maxDiffMC;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CColorStatic colorTitle;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedInfo();
	afx_msg void OnBnClickedOk();
};
