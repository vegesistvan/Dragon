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

// dragonView.h : interface of the CDragonView class
//

#pragma once

#include "dragonDoc.h"

class CDragonView : public CView
{
protected: // create from serialization only
	CDragonView() noexcept;
	DECLARE_DYNCREATE(CDragonView)

// Attributes
public:
	CDragonDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnInitialUpdate();								//!< called by framework before view is displayed 
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	CMainFrame* pM;
	CDragonView* pV;

// Implementation
public:
	virtual ~CDragonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dragonView.cpp
inline CDragonDoc* CDragonView::GetDocument() const
   { return reinterpret_cast<CDragonDoc*>(m_pDocument); }
#endif

