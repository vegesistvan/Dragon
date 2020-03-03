#pragma once
#include "colorstatic.h"
#include "afxwin.h"


// CSelectedFiles dialog

class CSelectedFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectedFiles)

public:
	CSelectedFiles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectedFiles();

// Dialog Data
	enum { IDD = IDD_SELECTED_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString	str;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_texteditor;
	CString	m_databaseSpec;
	CString	m_workingDirectory;

	CColorStatic colorGedFile;
	CColorStatic colorViewer;
	CColorStatic m_color_database;
	CColorStatic m_color_htmlFileSpec;
	CColorStatic m_color_texteditor;

protected:

public:
	afx_msg void OnClickedStaticDatabase();
	afx_msg void OnClickedStaticHtml();
	afx_msg void OnClickedStaticGed();
	afx_msg void OnClickedStaticViewer();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedStaticTexteditor();
	afx_msg void OnClickedButtonDefault();

};
