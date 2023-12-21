#pragma once
#include "afxwin.h"
#include "resource.h"

class CDisplayInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayInfo)

public:
	CDisplayInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayInfo();

	enum { IDD = IDD_DISPLAY_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	CString* m_text;
	CString  m_title;
private:
	CEdit m_editCtrl;
};
