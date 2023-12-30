#pragma once


// CListView view

class CListView : public CView
{
	DECLARE_DYNCREATE(CListView)

protected:
	CListView();           // protected constructor used by dynamic creation
	virtual ~CListView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


