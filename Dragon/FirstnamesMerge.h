#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"


// CFirstnamesMerge dialog

class CFirstnamesMerge : public CDialogEx
{
	DECLARE_DYNAMIC(CFirstnamesMerge)

public:
	CFirstnamesMerge(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFirstnamesMerge();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIRSTNAMES_MERGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_systemdb_out;
	afx_msg void OnClickedDatabaseIn();
	virtual BOOL OnInitDialog();
protected:
	CSqliteDB* systemDB2;
	CSqliteDBRecordSet* m_rsetSystemOut;
	CSqliteDBRecordSet* m_rsetSystemIn;
	CString str;

	FILE* fl;

	BOOL CFirstnamesMerge::executeSys2(CString command);
	BOOL CFirstnamesMerge::query(CString command);


public:
	CColorStatic colorSystemdbIn;
	afx_msg void OnBnClickedOk();
};
