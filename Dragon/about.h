#pragma once
#include "resource.h"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:

	afx_msg void OnStatic();
};
