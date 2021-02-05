#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CTableCouples dialog

class CTableCouples : public CDialogEx
{
	DECLARE_DYNAMIC(CTableCouples)
	DECLARE_EASYSIZE
public:
	CTableCouples(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableCouples();

// Dialog Data
	enum { IDD = IDD_TABLECOUPLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeres;
	CColorStatic colorNext;
};
