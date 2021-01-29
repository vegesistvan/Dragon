#pragma once
#include "listctrlex.h"


// CContractedCouples dialog

class CContractedCouples : public CDialogEx
{
	DECLARE_DYNAMIC(CContractedCouples)
	DECLARE_EASYSIZE
public:
	CContractedCouples(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractedCouples();

// Dialog Data
	enum { IDD = IDD_CONTRACTED_COUPLES };

	CString m_filter;
	BOOL    m_contracted;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	CString m_fileSpec;
	CString cLine;
	int m_columnsCount;

	std::vector<TCHAR*> vCouples; 
	std::vector<TCHAR*> vFiltered; 

	void createColumns();
	void push( CString item );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CListCtrlEx m_ListCtrl;
	afx_msg void OnKeress();
	afx_msg void OnClickedNext();
};
