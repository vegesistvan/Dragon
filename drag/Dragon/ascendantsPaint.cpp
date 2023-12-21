// ascendantPiramyd.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendantsPaint.h"
#include "ascendantsEnum.h"

// CAscendantsPaint dialog

IMPLEMENT_DYNAMIC(CAscendantsPaint, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsPaint::CAscendantsPaint(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASCENDANTS_PAINT, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsPaint::~CAscendantsPaint()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAscendantsPaint, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()

	ON_COMMAND(ID_GRID, &CAscendantsPaint::OnGrid)
	ON_COMMAND(ID_CONNECT, &CAscendantsPaint::OnConnect)
	ON_COMMAND(ID_SAVE, &CAscendantsPaint::OnSave)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsPaint::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	memset(&logfont, 0, sizeof(LOGFONT));
	_tcsncpy_s(logfont.lfFaceName, LF_FACESIZE, L"Times new roman", 16);
	_tcsncpy_s(logfont.lfFaceName, LF_FACESIZE, L"Arial", 6);
	_tcsncpy_s(logfont.lfFaceName, LF_FACESIZE, L"Curier New", 10);

	logfont.lfHeight = -100;
	logfont.lfWidth = 0;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = PROOF_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;

	SetWindowText(theApp.m_caption);
	GRID = false;	
	CONNECT = true;
	PRINT = false;
	NAME = true;

	m_gMax = 0;
	m_kMax = 0;
	for (int i = 0; i < vGKR->size(); ++i)
	{
		if ( (which == UNIQUE || which == UNIQUE_P ) && vGKR->at(i).cnt > 1) continue;
		
		m_gMax = (vGKR->at(i).g > m_gMax) ? vGKR->at(i).g : m_gMax;
		m_kMax = (vGKR->at(i).k > m_kMax) ? vGKR->at(i).k : m_kMax;
	}

	m_radius = 4;
	m_name = vGKR->at(0).name;

	m_colorGray = RGB(240, 240, 240);

	brushWhite = CreateSolidBrush( RGB(255, 255, 255 ) );
	brushBlack = CreateSolidBrush(RGB(0, 0, 0));
	brushRed = CreateSolidBrush(RGB(255, 0, 0));
	brushBlue = CreateSolidBrush(RGB(0, 0, 255));
	brushF = CreateSolidBrush(RGB(255, 0, 0));
	brushM = CreateSolidBrush(RGB(0, 0, 255));
	brushGray = CreateSolidBrush( m_colorGray );
	xBrush = CreateSolidBrush( PINK );
	yBrush = CreateSolidBrush( YELLOW );

	penWhite = CreatePen(PS_SOLID, 0, WHITE);
	penBlack = CreatePen(PS_SOLID, 0, BLACK);
	penRed = CreatePen(PS_SOLID, 0, RED);
	penBlue = CreatePen(PS_SOLID, 0, BLUE);

	ShowWindow(SW_MAXIMIZE);
	return TRUE; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnPaint()
{
	CPoint p;
	CPoint p1;
	CPoint p2;
	CRect rect;
	int length;
	int height = 20;  // a rextCoordinares és a 

	CPaintDC hDC(this);		 // device context for painting


	GetClientRect(rcClient);

	HDC memDC = CreateCompatibleDC ( hDC );
	HBITMAP memBM = CreateCompatibleBitmap ( hDC, rcClient.Width(), rcClient.Height() );
	HGDIOBJ pOldBitmap = SelectObject ( memDC, memBM );

	SelectObject( memDC, brushRed);

	// origo
	o = CPoint(60, 50 );
	
	rectCoordinates.SetRect(0, rcClient.bottom - height, o.x, rcClient.bottom);
	FillRect( memDC, rectCoordinates, brushGray);

	rectStatus.SetRect(o.x, rcClient.bottom - height, rcClient.right, rcClient.bottom);
	FillRect( memDC, rectStatus, brushGray);

	rcDraw.SetRect(o.x, o.y, rcClient.right, rectStatus.top );
	rcDraw.NormalizeRect();
	FillRect(memDC, rcDraw, brushBlack );

	rectX.SetRect(o.x, rcClient.top, rcDraw.right, rcDraw.top);
	FillRect( memDC, rectX, brushGray);

	rectY.SetRect(rcClient.left, rcDraw.top, rcDraw.left, rcDraw.bottom);
	FillRect( memDC, rectY, brushGray);

	rectCorner.SetRect(0, 0, o.x, o.y);
	FillRect( memDC, rectCorner, brushGray);

	rcZoom = rcClient;

	m_yUnit = double(rcDraw.Height()) / double((m_gMax + 1));
	m_xUnit = double(rcDraw.Width()) / double(((m_kMax + 1) * 2));


	// a menu alatt vonalat húz
	SelectObject( memDC, penRed);
	p1.x = 0;
	p2.x = rcClient.right;
	p1.y = rcClient.top + 1;
	p2.y = p1.y;
	MoveToEx( memDC, p1.x, p1.y, NULL );
	LineTo( memDC, p2.x, p2.y );

	// az y tengely mellett vonalat húz
	p1.x = o.x;
	p2.x = p1.x;
	p1.y = rcClient.top;
	p2.y = rcClient.bottom;
	MoveToEx(memDC, p1.x, p1.y, NULL);
	LineTo(memDC, p2.x, p2.y);

	// rcDraw alatt vonalt húz
	p1.x = 0;
	p2.x = rcDraw.right;
	p1.y = rcDraw.bottom;
	p2.y = p1.y;
	MoveToEx(memDC, p1.x, p1.y, NULL);
	LineTo(memDC, p2.x, p2.y);

//	SetBackgroundColor(m_colorGray);

	////////////////////
	long fontSize = 200;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfHeight = fontSize;
	VERIFY(fontAx.CreatePointFontIndirect(&logfont, &hDC));

	SelectObject( memDC, &fontAx);
	if( vGKR->at(0).sex_id == 1)
		SetTextColor(memDC, RED);
	else
		SetTextColor( memDC, BLUE );
	str.Format(L"%s felmenõinek piramisa", getNameBirthDeath(0));
	DrawTextW( memDC, str, str.GetLength(), &rectX, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	fontSize = 100;
	VERIFY(fontAx.CreatePointFontIndirect(&logfont, &hDC));

	yAxis( &memDC );
	ascendantsXY();
	_int64 k;

//	SelectObject(memDC, penWhite);
//	SetTextColor(memDC, WHITE);
//	SetBackgroundColor( BLACK);
//	SetTextAlign( memDC, TA_LEFT | TA_TOP | TA_NOUPDATECP);
	for (int i = 0; i < vGKR->size(); ++i)
	{
		if ( (which == UNIQUE || which == UNIQUE_P) && vGKR->at(i).cnt > 1) continue;
		p.x = vGKR->at(i).x;
		p.y = vGKR->at(i).y;
		if (vGKR->at(i).sex_id == 1)
			SelectObject( memDC, brushRed);
		else
			SelectObject( memDC, brushBlue);
		if( i)
			rect.SetRect(p.x - m_radius, p.y + m_radius, p.x + m_radius, p.y - m_radius);
		else
			rect.SetRect(p.x - m_radius*2, p.y + m_radius*2, p.x + m_radius*2, p.y - m_radius*2);
		Ellipse( memDC, rect.left, rect.top, rect.right, rect.bottom );
		if (i && NAME)
		{
			str = getNameBirthDeath(i);
			str.Format(L"%s\n*%x +%s", vGKR->at(i).name, vGKR->at(i).birth, vGKR->at(i).death);
			length = str.GetLength();
			rect.SetRect(p.x - length*10, p.y - 15, p.x + length*10, p.y + 15);
			SetTextColor(memDC, BLACK);
			SetBackgroundColor( BLACK);
//			DrawTextW(memDC, str, length, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			DrawTextW(memDC, str, length, &rect, DT_VCENTER | DT_CENTER);
		}
		if (i && CONNECT)
		{
			k = vGKR->at(i).k/2;
			for (int j = 0; j < vGKR->size(); ++j)
			{
				if (vGKR->at(j).k == k)
				{
					p1.x = vGKR->at(j).x;
					p1.y = vGKR->at(j).y;
					MoveToEx(memDC, p.x, p.y, NULL);
					LineTo(memDC, p1.x, p1.y);
				}
			}
		}
	}

//	rcZoom.SetRect(800, 130, 1050, 500);
// A bitmap-et áttranszformálja a megadott téglalapból a megadott téglalapba
//	SetStretchBltMode( hDC, COLORONCOLOR);
//	StretchBlt ( hDC, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), memDC, rcZoom.left, rcZoom.top, rcZoom.Width(), rcZoom.Height()-100, SRCCOPY);
	

	if (!BitBlt(hDC, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), memDC, rcClient.left, rcClient.top, SRCCOPY))
	{
		AfxMessageBox(L"BitBlt has failed", MB_OK);
	}


	if (PRINT)
	{
		CImage new_image;
		new_image.Attach((HBITMAP)(memBM));
		new_image.Save(m_filePathName);
		new_image.Detach();
		PRINT = false;
	}



	// a dcMem-et felszabadítjuk
	DeleteObject( memBM );
	SelectObject( memDC, pOldBitmap);
	DeleteDC( memDC );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::yAxis( HDC* dc)
{
	CString g;
	CRect rect;
	CPoint p1;
	CPoint p2;
	CPoint p;


	rect.SetRect(0, 0, rcDraw.left, rcDraw.top - 4);
	DrawTextW( *dc, L"gener", 5, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	p1.x = rcDraw.left - 15;
	if (GRID)
	{
		SelectObject( *dc, penWhite);
		p2.x = rcDraw.right;
	}
	else
	{
		SelectObject( *dc,  penBlack);
		p2.x = rcDraw.left;
	}
	for (int i = 0; i <= m_gMax; ++i)
	{
		p1.y = rcDraw.top + (m_yUnit * i);
		p2.y = p1.y;
		MoveToEx( *dc, p1.x, p1.y, NULL );
		LineTo( *dc, p2.x, p2.y );
		rect.SetRect(20, p1.y + 15, 40, p1.y - 15);
		g.Format(L"%c", get_gABC(i));
		SetTextColor( *dc, BLACK);
		DrawTextW( *dc, g, g.GetLength(), & rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::xAxis(CPaintDC* dc)
{
	CPoint p1;
	CPoint p2;
	
	p1.y = rcDraw.top - 10;
	if (GRID)
	{
		dc->SelectObject(penWhite);
		p2.y = rcDraw.bottom;
	}
	else
	{
		dc->SelectObject(penBlack);
		p2.y = rcDraw.top;
	}
	
	for (int i = 0; i <= m_kMax * 2; ++i)
	{
		p1.x = rcDraw.left + (m_xUnit * i);
		p2.x = p1.x;
		dc->MoveTo(p1);
		dc->LineTo(p2);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::ascendantsXY()
{
	int g;
	int gPrev = 0;
	_int64 k;
//	_int64 kMin = 0;
//	_int64 kMax = 0;
//	int cnt;
	int x;
	int y;
//	int z;
//	int kOrd;  // 0-tól  
	int kGDB;	// generációnkénti k darabszáma
	_int64 kAct;
//	_int64 kF;
//	_int64 kM;
	_int64 kC;		// gyerek k-ja
	int xAct;
	double xUnitAct = 0;
	double xDelta = rcDraw.right - rcDraw.left;


	xAct = xDelta / 2;
	vGKR->at(0).x = xAct;
	vGKR->at(0).y = rcDraw.top;
	for (int i = 1; i < vGKR->size(); ++i)
	{
		k = vGKR->at(i).k;
		g = vGKR->at(i).g;
		if (g != gPrev)
		{
			kGDB = get_kGDB(g);
			xUnitAct = xDelta / (kGDB+1);
		}
		gPrev = g;

		y = rcDraw.top + m_yUnit * g;
		// megkeresi a gyerekét
		kC = k / 2;
		for( int j = i-1; j >= 0; --j )
		{
			if (vGKR->at(j).k == kC)
			{
				if( isOdd( k ))
					vGKR->at(i).x = vGKR->at(j).x + xUnitAct/2;		// anya a gyerek jobboldalán
				else
					vGKR->at(i).x = vGKR->at(j).x - xUnitAct/2;		// apa a gyerek baloldalán
				vGKR->at(i).y = y;
				break;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kekule számból a generációt kiszámítani
int CAscendantsPaint::getGfromK(_int64 k)
{
	_int64 kMin;
	_int64 kMax;
	UINT g;
	for (g = 0; ; ++g)
	{
		kMin = pow(2, g);
		kMax = pow(2, g + 1) - 1;
		if (kMin <= k && k <= kMax)
			break;
	}
	return g;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kekule számból a generációt kiszámítani
int CAscendantsPaint::get_kGDB( int g )
{
	int db;
	_int64 kMin;
	_int64 kMax;
	kMin = pow(2, g);
	kMax = pow(2, g + 1);
	return kMax - kMin;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendantsPaint::getNameBirthDeath( int i)
{
	GKR gkr = vGKR->at(i);
	
	if (gkr.birth.IsEmpty() && gkr.death.IsEmpty())
		str.Format(L"%c-%d %s ", gkr.gABC, gkr.k, gkr.name);
	else if (!gkr.birth.IsEmpty() && !gkr.death.IsEmpty())
		str.Format(L"%c-%d %s *%s +%s", gkr.gABC, gkr.k, gkr.name, gkr.birth, gkr.death);
	else if (gkr.birth.IsEmpty())
		str.Format(L"%c-%d %s +%s", gkr.gABC, gkr.k, gkr.name, gkr.death);
	else
		str.Format(L"%c-%d %s *%s", gkr.gABC, gkr.k, gkr.name, gkr.birth);
	return(str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kekule számból a generációt kiszámítani
int CAscendantsPaint::geOrderOfK(_int64 k, int g )
{
	_int64 kMin = pow(2, g);
	return(k - kMin);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnMouseMove(UINT nFlags, CPoint point)
{
	CString str;
	m_mousePos.Format( L"%4d,%-4d", point.x, point.y );
	GetDlgItem(IDC_COORDINATES)->SetWindowTextW(m_mousePos);
	GetDlgItem(IDC_STATUS)->SetWindowTextW( L"");
	CDialogEx::OnMouseMove(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Show information on dispayed objects 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rect;
	CPoint p;
	CString ascendants;
	GKR gkr;

	rect.SetRect(point.x - m_radius, point.y + m_radius, point.x + m_radius, point.y - m_radius );
	rect.NormalizeRect();
	for (int i = 0; i < vGKR->size(); ++i)
	{
		gkr = vGKR->at(i);
		p.x = gkr.x;
		p.y = gkr.y;
		if (rect.PtInRect(p))
		{
			ascendants = getNameBirthDeath(i);
			/*
			if (gkr.birth.IsEmpty() && gkr.death.IsEmpty())
				ascendants.Format(L"%c-%d %s ", gkr.gABC, gkr.k, gkr.name);
			else if (!gkr.birth.IsEmpty() && !gkr.death.IsEmpty())
				ascendants.Format(L"%c-%d %s *%s +%s", gkr.gABC, gkr.k, gkr.name, gkr.birth, gkr.death);
			else if ( gkr.birth.IsEmpty() )
				ascendants.Format(L"%c-%d %s +%s", gkr.gABC, gkr.k, gkr.name, gkr.death);
			else
				ascendants.Format(L"%c-%d %s *%s", gkr.gABC, gkr.k, gkr.name, gkr.birth );
				*/
			AfxMessageBox(ascendants, MB_ICONINFORMATION);
			GetDlgItem(IDC_STATUS)->SetWindowTextW(ascendants);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsPaint::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CSize ZoomFactor;

	ZoomFactor.cx = rcZoom.Width() / 20;		// x iranyu nagyitasi arany beallitasa
	ZoomFactor.cy = rcZoom.Height() / 20;	// y iranyu nagyitasi arany beallitasa

	CPoint oldPoint = pt;

	if (zDelta > 0)			// nagyítani akarunk
	{
		rcZoom.InflateRect(-ZoomFactor); //zoomolas
	}
	else					// kicsinyíteni akarunk
	{
		rcZoom.InflateRect(ZoomFactor); //zoomolas
	}

	rcZoom.OffsetRect(oldPoint - pt);

	Invalidate(FALSE);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnGrid()
{
	GRID = !GRID;
	Invalidate(false);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnConnect()
{
	CONNECT = !CONNECT;
	Invalidate(false);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR CAscendantsPaint::get_gABC(int g)
{
	TCHAR gABC;
	int ix = g / 26;		  // ix = 0, ha g kisebb mint 26, = 1 Ha nagyobb
	gABC = TCHAR('A') + ix * 6 + g;   // 6 : a 'Z' és 'a' közötti karakterek száma,
	return gABC;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnSave()
{
	CString filename;
	if ( which == FULL )
	{
		filename.Format(L"%s felmenõinek teljes piramisa", m_name);
		m_filePathName.Format(L"%s\\%s_%s.jpg", m_ascendantsPath, filename, getTimeTag());

	}
	else
	{
		filename.Format(L"%s felmenõinek ritkított piramisa", m_name);
		m_filePathName.Format(L"%s\\%s_%s.jpg", m_ascendantsPath, filename, getTimeTag());
	}
	PRINT = true;
	Invalidate(false);
	str.Format(L"Képernyõ a\n%s\nfájlba kiírva.", m_filePathName);
	AfxMessageBox(str, MB_ICONINFORMATION);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsPaint::OnMButtonDown(UINT nFlags, CPoint point)
{
//	rcZoom = rcClient;
//	Invalidate(FALSE);
	return CDialogEx::OnMButtonDown( nFlags, point);
}

