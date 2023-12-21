#pragma once
#include "colorstatic.h"


// CSelectHtmlFile dialog

class CSelectHtmlFile : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectHtmlFile)

public:
	CSelectHtmlFile(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectHtmlFile();

// Dialog Data
	enum { IDD = IDD_SELECTHTMLFILE };

	CString m_htmlPathName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CColorStatic colorHtmlFileSpec;
	afx_msg void OnClickedHtmlfilespec();
};
