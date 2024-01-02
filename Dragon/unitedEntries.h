#pragma once

#include "afxdialogex.h"
#include "afxbutton.h"
#include "unitedEntriesUnited.h"
#include "unitedEntriesDifferent.h"
#include "unitedEntriesErrors.h"
#include "unitedEntriesX.h"
#include "ColorStatic.h"
#include "ListCtrlEx.h"

enum
{
	U_DUMMY = 0,
	U_LOOP,
	U_GROUP,
	U_MATCH,
	U_STATUS,
	U_UNITED,
	U_INDEX,
	U_SOURCE,

	U_LINE,
	U_ROWID,
	U_NAME,
	U_SEX_ID,
	U_BIRTH,
	U_DEATH,

	U_LINEF,
	U_ROWIDF,
	U_FATHER,
	U_BIRTHF,
	U_DEATHF,

	U_LINEM,
	U_ROWIDM,
	U_MOTHER,
	U_BIRTHM,
	U_DEATHM,

	U_SPOUSES,
	U_CHILDREN,
	U_COLUMNSCOUNT,
};

class CUnitedEntries : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitedEntries)
	DECLARE_MESSAGE_MAP()
public:
	CUnitedEntries(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUnitedEntries();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITED_ENTRIES };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;

	CString m_title;
	CString m_title0;
	CString m_title1;
	CString m_title2;
	CString m_title3;

	CTabCtrl		m_TabCtrl;
	CColorStatic	colorCaption;
	CColorStatic	colorNext;
	CEdit			m_SearchCtrl;

	CComboBox m_comboCtrl;
	CRichEditCtrl m_RichEditCtrl;
	
	CButton m_ButtonCaption;
	int m_comboCur;

	

	CHARFORMAT cf = { 0 };

	CListCtrlEx*	m_ListCtrl;
	std::vector<TCHAR*>* vPeople;
	std::vector<TCHAR*>* vFiltered;

	CUnitedEntriesDifferent		m_ceD;
	CUnitedEntriesUnited		m_ceU;
	CUnitedEntriesErrors		m_ceE;
	CUnitedEntriesX				m_ceX;

	BOOL CUnitedEntries::attachDialogs();
	void CUnitedEntries::keress(int start);
	void CUnitedEntries::filter(int iter);
	void CUnitedEntries::clearRichEdit();
		
//	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClickedStaticNext();
	afx_msg void OnFilterUnfiltered();
	afx_msg void OnFilterIteration1();
	afx_msg void OnFilterIteration2();
	afx_msg void OnAtvett();
	afx_msg void OnInfoUnion();
	afx_msg void OnInfoUnited();

};
