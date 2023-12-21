#pragma once
#include "colorstatic.h"
#include "listctrlex.h"



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

	HBRUSH m_brush;
	CColorStatic colorFreeCountBlob;
	CColorStatic colorInterity;
	CColorStatic colorFreeCount;


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedReindexBlob();
	afx_msg void OnClickedReindex();
	afx_msg void OnClickedVacuum();
	afx_msg void OnClickedVacuumBlob();
	afx_msg void OnClickedVacuumSystem();
	afx_msg void OnClickedReindexSystem();

	CColorStatic colorFreeCountSystem;
	CColorStatic colorIntegrityBlob;
	CColorStatic colorIntegritySystem;
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CColorStatic colorIntegrity;
	CStatic m_inputCtrl;
	CStatic m_uniteCtrl;
//	CStatic m_unitedCtrl2;
	CStatic m_uniteCtrl2;
//	CStatic m_inputVersionCtrl;
};
