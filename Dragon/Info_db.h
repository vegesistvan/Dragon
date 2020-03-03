#pragma once
#include "colorstatic.h"
#include "listctrlex.h"


// CInfoDb dialog

class CInfoDb : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDb)

public:
	CInfoDb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfoDb();

// Dialog Data
	enum { IDD = IDD_INFO_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;

//	CColorStatic colorIntegrityBlob;
	CColorStatic colorFreeCountBlob;
//	CColorStatic colorInterity;
	CColorStatic colorFreeCount;


	CColorStatic colorSystemDatabase;
	CColorStatic colorBlobSpec;
	CColorStatic colorDatabase;
//	CColorStatic colorHtml;

//	CListCtrlEx m_ListCtrl;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedReindexBlob();
	afx_msg void OnClickedReindex();
	afx_msg void OnClickedVacuum();
	afx_msg void OnClickedVacuumBlob();
	CColorStatic colorIntegrity;
//	CColorStatic colorIntegritySystem;
//	CColorStatic colorFreeCountSystem;
	afx_msg void OnClickedVacuumSystem();
	afx_msg void OnClickedReindexSystem();
//	CColorStatic colorIntegityBlob;
//	CColorStatic colorIntegritySystem;
	CColorStatic colorFreeCountSystem;
	CColorStatic colorIntegrityBlob;
	CColorStatic colorIntegritySystem;
	afx_msg void OnBnClickedOk();
};
