// ListBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx

CListBoxEx::CListBoxEx()
{
}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	//{{AFX_MSG_MAP(CListBoxEx)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx message handlers

void CListBoxEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
//	if((point.x == -1) && (point.y == -1))
//  {
    // Keystroke invocation
//    CRect rect;

//    GetClientRect(rect);
//    ClientToScreen(rect);

//    point = rect.TopLeft();
//    point.Offset(5, 5);
//  }


		GetParent()->SendMessage(WM_LISTBOX_MENU, (WPARAM)0, (LPARAM)&point);

}
