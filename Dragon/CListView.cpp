// CListView.cpp : implementation file
//

#include "pch.h"
#include "dragon.h"
#include "CListView.h"
#include <afxwin.h>

IMPLEMENT_DYNCREATE(CListView, CView)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListView::CListView()
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListView::~CListView()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListView, CView)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CListView::AssertValid() const
{
	CView::AssertValid();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WIN32_WCE
void CListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
/*
	// this code only works for a report-mode list view
	ASSERT(GetStyle() & LVS_REPORT);

	CListCtrl& listCtrl = GetListCtrl();

	// Insert a column. This override is the most convenient.
	listCtrl.InsertColumn(0, _T("Player Name"), LVCFMT_LEFT);

	// The other InsertColumn() override requires an initialized
	// LVCOLUMN structure.
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT;
	col.pszText = _T("Jersey Number");
	col.fmt = LVCFMT_LEFT;
	listCtrl.InsertColumn(1, &col);

	// Set reasonable widths for our columns
	listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	listCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	CHeaderCtrl* pHeaderCtrl = listCtrl.GetHeaderCtrl();
	ASSERT(NULL != pHeaderCtrl);
	//perform any needed operations on the header using pHeader
*/
}