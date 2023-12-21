#pragma once
#include "ListCtrlEx.h"
#define MSG_POST_SUBCLASS_LISTVIEW (WM_APP+50)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CListCtrlColoredHeader window
class CListCtrlColoredHeader : public CListCtrlEx
{
// Construction
public:
	CListCtrlColoredHeader();
// Attributes
public:
	public:
		
	class CCustomDrawHeaderCtrl : public CHeaderCtrl
	{
	public:

		int h11;
		int h21;
		int h12;
		int h22;

		CCustomDrawHeaderCtrl();
		virtual ~CCustomDrawHeaderCtrl();
		afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
		
	protected:
		CBrush m_brBackground;
		afx_msg LRESULT OnEraseBkgndOverride( WPARAM wParam, LPARAM lParam );
		DECLARE_MESSAGE_MAP()
	};
	CCustomDrawHeaderCtrl m_ctlHeader;
public:
	protected:
	virtual void PreSubclassWindow();
// Implementation
public:
	virtual ~CListCtrlColoredHeader();
protected:
	afx_msg LRESULT OnPostSubclassListview( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};
