#pragma once
#include "afxwin.h"
#include "colorstatic.h"



class CFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CFilter)

public:
	CFilter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilter();

// Dialog Data
	enum { IDD = IDD_FILTER };
	CString m_relation;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString m_filterText1;
	CString m_filterText2;
	CString m_filter1;
	CString m_filter2;
	CString m_filterNew;
	CString m_filterTextNew;
	afx_msg void OnClickedFilterAll();
	afx_msg void OnClickedFilterLast();
	afx_msg void OnClickedStaticOrand();
	CColorStatic colorOrAnd;
	CColorStatic colorCondi1;
	CColorStatic colorCondi2;
};
