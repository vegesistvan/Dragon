#include "stdafx.h"
#include "Color.h"
#include "ListCtrlColoredHeader.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListCtrlColoredHeader::CListCtrlColoredHeader()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListCtrlColoredHeader::~CListCtrlColoredHeader()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListCtrlColoredHeader, CListCtrlEx)
	ON_MESSAGE( MSG_POST_SUBCLASS_LISTVIEW, OnPostSubclassListview )
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListCtrlColoredHeader::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	DWORD dwStyles = GetExtendedStyle();
	dwStyles |= LVS_EX_FULLROWSELECT;
	SetExtendedStyle(dwStyles);
	PostMessage( MSG_POST_SUBCLASS_LISTVIEW, 0L, 0L );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CListCtrlColoredHeader::OnPostSubclassListview( WPARAM wParam, LPARAM lParam )
{
	m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() );
	m_ctlHeader.Invalidate( TRUE );
	return 0L;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListCtrlColoredHeader::CCustomDrawHeaderCtrl::CCustomDrawHeaderCtrl()
{
	m_brBackground.CreateSolidBrush( RGB(200,100,50) );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CListCtrlColoredHeader::CCustomDrawHeaderCtrl::~CCustomDrawHeaderCtrl()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CListCtrlColoredHeader::CCustomDrawHeaderCtrl, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CListCtrlColoredHeader::CCustomDrawHeaderCtrl::OnCustomdraw)
	ON_MESSAGE( WM_ERASEBKGND, CListCtrlColoredHeader::CCustomDrawHeaderCtrl::OnEraseBkgndOverride )
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CListCtrlColoredHeader::CCustomDrawHeaderCtrl::OnCustomdraw (NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pCDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // Take the default processing unless we set this to something else below
    *pResult = CDRF_DODEFAULT;
	CDC dc;
	switch( pCDraw->dwDrawStage )
	{
	case CDDS_PREPAINT:
		// the item's prepaint stage: return CDRF_NOTIFYITEMDRAW so that we 
		// get future WM_CUSTOMDRAW messages

		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT: 
		// the item's pre-paint stage: change the DC settings to get needed text colors
		dc.Attach( pCDraw->hdc );

		if ( pCDraw->dwItemSpec < h11 )
			dc.SetTextColor( BLACK );	// fekete
		else if (h11 <= pCDraw->dwItemSpec && pCDraw->dwItemSpec <= h12)
		{
			dc.SetBkColor(LIGHTRED);
			dc.SetTextColor(LIGHTBLUE);	// kék
		}
		else if (h21 <= pCDraw->dwItemSpec && pCDraw->dwItemSpec <= h22)
			dc.SetTextColor(LIGHTRED);	// kék
						
		switch ( pCDraw->dwItemSpec )
		{
			case 0: dc.SetBkColor( RGB( 215, 0, 0) ); break;
			case 1: dc.SetBkColor( RGB( 0, 215, 0) ); break;
			case 2: dc.SetBkColor( RGB( 0, 0, 215) ); break;
		}
		dc.Detach();
		*pResult = CDRF_DODEFAULT;
		break;
	case CDDS_ITEMPOSTPAINT:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CListCtrlColoredHeader::CCustomDrawHeaderCtrl::OnEraseBkgndOverride( WPARAM wParam, LPARAM lParam )
{
	CDC dc;
	dc.Attach( (HDC) wParam );

	CHeaderCtrl::OnEraseBkgnd(&dc);
	CRect rc;
	GetClientRect( &rc );
	dc.Detach();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
