#pragma once
#include "afxwin.h"


// CUniteInfo dialog

class CUniteInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUniteInfo)

public:
	CUniteInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUniteInfo();

// Dialog Data
	enum { IDD = IDD_UNITE_INFO };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_info;
	CString m_title;
	CEdit EditCtrl;
};
