#pragma once
#include "listctrlex.h"
#include "afxwin.h"
#include "colorstatic.h"


class CContractedPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CContractedPeople)
	DECLARE_EASYSIZE

public:
	CContractedPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractedPeople();

	enum { IDD = IDD_CONTRACTED_PEOPLE };

	CString m_filter;
	BOOL    m_contracted;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void createColumns();
	void push( CString item );
	void keress( int start );
	CMenu menu;
	CListCtrlEx		m_ListCtrl;
	CEdit			m_search;
	CColorStatic	colorKeress;
	CColorStatic	colorNext;
	int		m_numOfGroups;
	CString m_fileSpec;
	CString str;
	CString cLine;
	CString m_command;
	int m_azonos;
	bool UNITED;

	std::vector<TCHAR*> vPeople; 
	std::vector<TCHAR*> vFiltered; 


	void inputFile( int subType );
	void getFileSpec( int type, int subtype );

	void OnEdit2lines();
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClickedKeress();
	afx_msg void OnClickedNext();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInfo();
	afx_msg void OnFilterAll();
	afx_msg void OnFilter1();
	afx_msg void OnFilter2();
	afx_msg void OnInputDifferent();
	afx_msg void OnInputUnited();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHtml1D();
	afx_msg void OnHtml1U();
	afx_msg void OnHtml2D();
	afx_msg void OnHtml2U();

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnDbEdit();
};
