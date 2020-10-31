#pragma once
#include "afxwin.h"


// CSamePeopleInfo dialog

class CSamePeopleInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSamePeopleInfo)

public:
	CSamePeopleInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamePeopleInfo();

// Dialog Data
	enum { IDD = IDD_SAME_PEOPLE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_info;
	CEdit EditCtrl;
};
