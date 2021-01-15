#pragma once
#include "listctrlex.h"
#include "afxwin.h"
#include "colorstatic.h"


// CContractedPeople dialog

class CContractedPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CContractedPeople)
	DECLARE_EASYSIZE

public:
	CContractedPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractedPeople();

// Dialog Data
	enum { IDD = IDD_CONTRACTEDPEOPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void createColumns();
	void push( CString item );
	void keress( int start );

	CListCtrlEx m_ListCtrl;
	CEdit m_search;
	CColorStatic colorKeress;
	CColorStatic colorNext;
	int m_columnsCount;

	CString m_fileSpec;
	CString str;
	CString cLine;
	CString m_command;


	std::vector<int> vColor;
	std::vector<TCHAR*> vTablePeople; 
	std::vector<TCHAR*> vFiltered; ;

	std::vector<int> vLoops;
	SAMENAMES r;				// referencia adatok az összehasonlításnál
	
	int		m_loop;				// szûrõ feltétel: 0=nincs, 1=1, 2=2
	CStringArray m_colors;
	UINT	m_rgb[10];

	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();
	afx_msg void OnInputCsvtext();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInfo();
	afx_msg void OnFilterAll();
	afx_msg void OnFilter1();
	afx_msg void OnFilter2();
};
