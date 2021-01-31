#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	CString cLine;
	int m_columnsCount;
	int m_numOfGroups;
	CListCtrlEx m_ListCtrl;
	bool UNITED;
	CMenu menu;
	std::vector<TCHAR*> vCouples; 
	std::vector<TCHAR*> vFiltered; 

	bool inputFile( int subType );
	void createColumns();
	void push( CString item );
	void keress( int start );

	void OnHtmlEdit();
	void OnHtmlShows();
	void OnEdit2lines();
	void OnHtmlNotepad();
	void OnHtmlPeoplefather();
	void OnEditNotepadParents();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	CColorStatic colorKeress;
	CColorStatic colorNext;
	afx_msg void OnInputDifferent();
	afx_msg void OnInputUnited();
	afx_msg void OnFilterAll();
	afx_msg void OnFilter1();
	afx_msg void OnFilter2();
	afx_msg void OnInfo();
};
