// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// dragonView.cpp : implementation of the CDragonView class
//

#include "pch.h"
#include "framework.h"
#include <control.h>
#include <afxcview.h>

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dragon.h"
#endif
#include "MainFrm.h"
#include "dragonDoc.h"
#include "dragonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDragonView
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDragonView, CView)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDragonView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDragonView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonView::CDragonView() noexcept
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonView::~CDragonView()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = 1024;	// height
	cs.cy = 678;	// width

	cs.x = ( 1920 - cs.cx ) / 2;
	cs.y = ( 1080 - cs.cy ) / 2;
	
	return CView::PreCreateWindow(cs);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnInitialUpdate()
{

	CView::OnInitialUpdate();
	pM = (CMainFrame*)AfxGetMainWnd();

	// Az ablak helyének és méretének meghatározása., beállítása
	int bmpWidth = 1024;
	int bmpHeight = 678;
	//	bmpWidth = 1916;
	//	bmpHeight = 884;

	// The one and only window has been initialized, so show and update it
//	ShowWindow(SW_SHOW);
//	SetWindowPos(this, x, y, bmpWidth + 16, bmpHeight + 60, SWP_NOREDRAW | SWP_SHOWWINDOW);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnDraw(CDC* pDC )
{
	CDragonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	SetWindowText(theApp.m_caption);

	CRect		rcClient;
	GetClientRect(rcClient);  // client area pixelben

	int m_w = GetDeviceCaps(*pDC, HORZRES);
	int m_h = GetDeviceCaps(*pDC, VERTRES);
	m_w = 1800;
	m_h = 900;
	m_w = rcClient.Width();
	m_h = rcClient.Height();
	CDC* pMDC = new CDC;
	CBitmap* pb = new CBitmap;

	pMDC->CreateCompatibleDC(pDC);
	pb->CreateCompatibleBitmap(pDC, m_w, m_h );

	CBitmap* pob = pMDC->SelectObject(pb);



	CImage image;
//	image.Load( L"D:\\Dragon.jpg");
	image.Load( theApp.m_dragonjpg );

	int x0 = (rcClient.Width() - image.GetWidth()) / 2;
	int y0 = (rcClient.Height() - image.GetHeight()) / 2;

	CRect rcZoom;
	rcZoom.SetRect(0, 0, 1920, 1290);

	x0 = (rcClient.Width() - rcZoom.Width()) / 2;
	y0 = (rcClient.Height() - rcZoom.Height()) / 2;



//	image.StretchBlt(pMDC->m_hDC, 250, 0, m_w-500, m_h, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
	image.StretchBlt(pMDC->m_hDC, 0, 0, m_w, m_h, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
//	image.StretchBlt(pMDC->m_hDC, x0, y0, rcZoom.Width(), rcZoom.Height(), 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);

//	CImage new_image;
//	new_image.Attach((HBITMAP)(*pb));
//	new_image.Save(_T("D:\\Másik.jpg"));
//	new_image.Detach();

	BitBlt(*pDC, 0, 0, rcClient.Width(), rcClient.Height(), pMDC->m_hDC, 0, 0, SRCCOPY);

//	CPicture pic(ID_MYPIC); // load pic 
//	CRect rc(0,0,0,0);      // use default rc  
//	pic.Render(pDC, rc);    // display it

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDragonView printing
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CDragonView diagnostics

#ifdef _DEBUG
void CDragonView::AssertValid() const
{
	CView::AssertValid();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonDoc* CDragonView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDragonDoc)));
	return (CDragonDoc*)m_pDocument;
}
#endif //_DEBUG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
