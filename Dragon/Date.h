#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CDate dialog

class CDate : public CDialogEx
{
	DECLARE_DYNAMIC(CDate)

public:
	CDate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDate();

	CString	m_caption;
	CString m_datum;

	enum { IDD = IDD_DATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	SYSTEMTIME sysTime;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButtonClear();
	CDateTimeCtrl m_DateTimeCtrl;
	afx_msg void OnBnClickedOk();
};
