#pragma once
#include "afxdialogex.h"
#include "ascendants.h"


typedef struct
{
	int g;
	float xGFirst;
	float xGDelta;
}XGFIRST;

// CAscendantsPaint dialog

class CAscendantsPaint : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendantsPaint)

public:
	CAscendantsPaint(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendantsPaint();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASCENDANTS_PAINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;

	CPoint o;		// origo

	CRect rcClient;
	CRect rcDraw;
	CRect rectY;
	CRect rectX;
	CRect rectCorner;
	CRect rectStatus;
	CRect rectCoordinates;
	CRect rcZoom;

	CStatic status;
	CStatic coordinates;
	CString m_mousePos;
	CString m_filePathName;

	HPEN penWhite;
	HPEN penBlack;
	HPEN penRed;
	HPEN penBlue;

	CFont	fontAx;

	int		m_width;
	int		m_height;
	double	m_yUnit;
	double	m_xUnit;

	bool	GRID;
	bool	CONNECT;
	bool	PRINT;
	bool	NAME;
	int		m_radius;

	bool DISPLAY;

	COLORREF m_colorGray;

	HBRUSH xBrush;
	HBRUSH yBrush;

	HBRUSH brushWhite;
	HBRUSH brushBlack;
	HBRUSH brushRed;
	HBRUSH brushBlue;
	HBRUSH brushF;
	HBRUSH brushM;
	HBRUSH brushGray;

	LOGFONT		logfont;

	CBitmap		MemBitmap;
	
	CString m_name;

	void CAscendantsPaint::ascendantsXY();
	void CAscendantsPaint::xAxis(CPaintDC* dc);
	void CAscendantsPaint::yAxisX(CPaintDC* dc);
	void CAscendantsPaint::yAxis(HDC* dc);
	int CAscendantsPaint::getGfromK(_int64 k);
	int CAscendantsPaint::geOrderOfK(_int64 k, int g);
	int CAscendantsPaint::get_kGDB(int g);
	TCHAR CAscendantsPaint::get_gABC(int g);
	CString CAscendantsPaint::getNameBirthDeath( int i );

	DECLARE_MESSAGE_MAP()
public:
	int which;
	CString m_ascendantsPath;
	int		m_gMax;
	_int64  m_kMax;

	std::vector<GKR>* vGKR;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void CAscendantsPaint::OnPaintX();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void CAscendantsPaint::OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGrid();
	afx_msg void OnConnect();
	afx_msg void OnSave();
};
