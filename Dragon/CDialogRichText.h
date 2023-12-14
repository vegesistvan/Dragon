#pragma once
#include "afxdialogex.h"


// CDialogRichText dialog

class CDialogRichText : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRichText)

public:
	CDialogRichText(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogRichText();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDialogRichText };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CDialogRichText::InsertText(CString text, COLORREF color, bool bold, bool italic);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CRichEditCtrl m_editCtrl;
//	afx_msg void OnSetfocusRichEdit();
};
