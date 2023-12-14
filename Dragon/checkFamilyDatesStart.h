#pragma once
#include "afxwin.h"
#include "colorstatic.h"


// CCheckFamilyDatesStart dialog

class CCheckFamilyDatesStart : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFamilyDatesStart)

public:
	CCheckFamilyDatesStart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFamilyDatesStart();
	enum { IDD = IDD_CHECK_FAMILYDATES_START };

	// átadott adatok
	CString m_info;

    int	m_maxLifespan;
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
	CString m_extrem;
	CString m_normal;
	CString str;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedButtonSelect();
};
