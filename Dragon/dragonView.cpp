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

// dragonView.cpp : implementation of the CDragView class
//

#include "pch.h"
#include "framework.h"
#include <control.h>

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


// CDragView
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDragView, CView)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDragView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDragView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragView::CDragView() noexcept
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragView::~CDragView()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	return CView::PreCreateWindow(cs);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	pM = (CMainFrame*)AfxGetMainWnd();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnDraw(CDC* pDC )
{
	CDragDoc* pDoc = GetDocument();
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
/*
	CImage new_image;
	new_image.Attach((HBITMAP)(*pb));
	new_image.Save(_T("D:\\Másik.jpg"));
	new_image.Detach();
*/
	BitBlt(*pDC, 0, 0, rcClient.Width(), rcClient.Height(), pMDC->m_hDC, 0, 0, SRCCOPY);

//	CPicture pic(ID_MYPIC); // load pic 
//	CRect rc(0,0,0,0);      // use default rc  
//	pic.Render(pDC, rc);    // display it
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDragView printing
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CDragView diagnostics

#ifdef _DEBUG
void CDragView::AssertValid() const
{
	CView::AssertValid();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragDoc* CDragView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDragDoc)));
	return (CDragDoc*)m_pDocument;
}
#endif //_DEBUG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
