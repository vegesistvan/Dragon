#pragma once
#include "afxwin.h"


// CContractInfo dialog

class CContractInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CContractInfo)

public:
	CContractInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractInfo();

// Dialog Data
	enum { IDD = IDD_SAME_PEOPLE_INFO };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_info;
	CString m_title;
	CEdit EditCtrl;
};
