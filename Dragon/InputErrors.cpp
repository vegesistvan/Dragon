// InputErrors.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "InputErrors.h"
#include "afxdialogex.h"
#include "html_Lines.h"


// CInputErrors dialog

IMPLEMENT_DYNAMIC(CInputErrors, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInputErrors::CInputErrors(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputErrors::IDD, pParent)
{
	fileSpec1.Empty();		// házastársak neme
	fileSpec2.Empty();		// anya index
	fileSpec3.Empty();		// értelmezési problémák
	fileSpec4.Empty();		// ismeretelen keresztnevek
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInputErrors::~CInputErrors()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CInputErrors, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_ERRORFILE1, &CInputErrors::OnErrorfile1)
	ON_COMMAND(ID_ERRORFILE2, &CInputErrors::OnErrorfile2)
	ON_COMMAND(ID_ERRORFILE3, &CInputErrors::OnErrorfile3)
	ON_COMMAND(ID_ERRORFILE4, &CInputErrors::OnErrorfile4)


	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CInputErrors::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CInputErrors::OnHtmlNotepad)

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputErrors::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()

	CString caption;
	caption.Format( L"Beolvasási hibák a %s GA.htm fájlból", theApp.m_htmlFileName );
	SetWindowTextW( caption );

	CString info = L"\
A GA-html fájl beolvasása során a menüben látható 4 féle hibát figyeli a program és ezeket összegyûjti.\
\r\n\
A menübõl kiválaszthtajuk, hogy milyen hibákat kívánunk megvizsgálni. A hibásnak talált vagy nem értelmezhetõ sorokat ill. \
sor részleteket listázza a program, megadva, hogy ez a ga.html fájl hanyadik sora.\
\r\n\
Ha a probléma a ga.html fájl megfelleõ sorának szerkesztésével megoldható, akkor ezt interaktív módon megtehetjük.\
A jobb egérgombbal a kiválasztott sorra kattintva egy legördülõ menü jelenik meg, amibõl választhatjuk 'A sor szerkesztése' \n\
ill. 'A sor a Notepad-ben' funkciókat. Bármelyikban javíthatjuk a ga.html fájl megfelelõ sorát.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );



	CMenu* pMenu = GetMenu();

	if( fileSpec1.IsEmpty() )
		pMenu->EnableMenuItem( 0, MF_BYPOSITION|MF_GRAYED);
	if( fileSpec2.IsEmpty() )
		pMenu->EnableMenuItem( 1, MF_BYPOSITION|MF_GRAYED);
	if( fileSpec3.IsEmpty() )
		pMenu->EnableMenuItem( 2, MF_BYPOSITION|MF_GRAYED);
	if( fileSpec4.IsEmpty() )
		pMenu->EnableMenuItem( 3, MF_BYPOSITION|MF_GRAYED);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnErrorfile1()	// Házastársak neme
{
	CString info = L"\
Az alábbi sorokban azonos nemû házastársak vannak.\
\r\n\
Ez az esetek többségében valamilyen hibát jelez, de természetesen nem mindig, hiszen ilyenek elõfordulhatnak.\
\r\n\
A hiba oka lehet, hogy a program keresztnév nyilvántartásában van, de a ga.html fájlban is elõfordulhat hiba.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );
	
	int		nItem = 0;
	int		pos;
	CString cLine;
	CString number;
	CString lineNumber;
	CString str;
	CString cntS;
	int cnt = 0;

	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"",		LVCFMT_LEFT,	500,-1,COL_TEXT);


	CStdioFile file( fileSpec1, CFile::modeRead);
	while(file.ReadString(cLine)) 
	{
		if( ( pos = cLine.Find( L"<br>" ) ) != -1 )
			cLine = cLine.Left( pos );
		cLine.Trim();

		if( cLine.IsEmpty() )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
		}
		else if( !isNumeric( cLine ) ) continue;

		++cnt;
		str = dropFirstWord( cLine );
		number = getWord( str, 1, &pos );
		str = cLine.Mid( pos );

		lineNumber = dropLastCharacter( number );

		cntS.Format( L"%d", cnt );
		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText( nItem, 1, lineNumber );
		m_ListCtrl.SetItemText( nItem, 2, str );
		++nItem;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
    m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnErrorfile2()	// anya index
{
	CString info = L"\
Az alábbi leszármazottak anya-indexe nagyobb mint apja feleségeinek száma.\
\r\n\
A legördülõ menübõl kiválasztva a sor Notepad-el történõ megjelenításét, meggyõzõdhetünk az anya és gyermeke indexeinek értékeirõl.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	int		nItem = 0;
	int		pos;
	CString cLine;
	CString number;
	CString lineNumber;
	CString str;
	int cnt = 0;
	CString cntS;

	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"",		LVCFMT_LEFT,	500,-1,COL_TEXT);


	CStdioFile file( fileSpec2, CFile::modeRead);
	while(file.ReadString(cLine)) 
	{
		if( ( pos = cLine.Find( L"<br>" ) ) != -1 )
			cLine = cLine.Left( pos );
		cLine.Trim();

		if( cLine.IsEmpty() )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
		}
		else if( !isNumeric( cLine ) ) continue;

		++cnt;

		lineNumber	= getFirstWord( cLine );
		lineNumber	= dropLastCharacter( lineNumber );

		str			= dropFirstWord( cLine );
		cntS.Format( L"%d", cnt );
		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText( nItem, 1, lineNumber );
		m_ListCtrl.SetItemText( nItem, 2, str );
		++nItem;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
    m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnErrorfile3() // Értelmezési problémák
{
	CString info = L"\
Alább sor-párokat láthatunk felsorolva. A sor-pár második sorában olyan ember személyleírását találjuk, akinek a vezeték és/vagy \
keresztneve hiányzik. Ennek oka az, hogy a program valamilyen oknál fogva nem tudta ezt/ezeket meghatározni az elsõ sorban \
ábrázolt ga.html sorból.\
\r\n\
A hibákat többnyire az értelmezhetetlen keresztnevek okozzák, de más oka is lehet. Ha felismerjük a hibát és az javítható. akkor a \
legördülõ menübõl szerkeszthetjük a kérdéses ga.html sort.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	int		nItem = 0;
	int		pos;
	CString cLine;
	CString number;

	CString lineNumber;
	CString rowid;

	int		cnt = 0;
	CString cntS;

	CString str;
	CStdioFile file( fileSpec3, CFile::modeRead);

	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 3,	L"",		LVCFMT_LEFT,	500,-1,COL_TEXT);
	while(file.ReadString(cLine)) 
	{
		if( ( pos = cLine.Find( L"<br>" ) ) != -1 )
			cLine = cLine.Left( pos );
		cLine.Trim();

		if( cLine.IsEmpty() )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
		}
		else if( !isNumeric( cLine ) ) continue;

		number = getWord( cLine, 1, &pos );
		if( pos == -1 )
			str.Empty();
		else
			str = cLine.Mid( pos );

		if( ( pos= number.Find( 'L' ) ) != -1  )
		{
			lineNumber = dropLastCharacter( number );
			rowid.Empty();
		}
		else
		{
			lineNumber.Empty();
			rowid = dropLastCharacter( number );
		}
		if( !lineNumber.IsEmpty() )
		{
			++cnt;
			cntS.Format( L"%d", cnt );
			nItem = m_ListCtrl.InsertItem( nItem, cntS );
		}
		else
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
		
		m_ListCtrl.SetItemText( nItem, 1, lineNumber );
		m_ListCtrl.SetItemText( nItem, 2, rowid );
		m_ListCtrl.SetItemText( nItem, 3, str );
		++nItem;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
    m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnErrorfile4()	// Ismeretlen kertesztnevek
{
	CString info = L"\
Az alábbi sorokban ismeretlen keresztnevû embereket talált.\
\r\n\
Ha felismerjük a hibát és az javítható. akkor a legördülõ menübõl szerkeszthetjük a kérdéses ga.html sort.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	int		nItem = 0;
	int		pos;
	CString cLine;
	CString lineNumber;
	CString str;
	int cnt = 0;
	CString cntS;

	m_ListCtrl.DeleteAllItems();
	for( int i = m_ListCtrl.GetHeaderCtrl()->GetItemCount(); i >= 0; i-- )
	{
		m_ListCtrl.DeleteColumn(i);
	}

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( 0,	L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( 2,	L"",		LVCFMT_LEFT,	500,-1,COL_TEXT);


	CStdioFile file( fileSpec4, CFile::modeRead);
	while(file.ReadString(cLine)) 
	{
		if( ( pos = cLine.Find( L"<br>" ) ) != -1 )
			cLine = cLine.Left( pos );
		cLine.Trim();

		if( cLine.IsEmpty() )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
		}
		else if( !isNumeric( cLine ) ) continue;

		lineNumber	= getFirstWord( cLine );
		str			= dropFirstWord( cLine );
		
		++cnt;
		cntS.Format( L"%d", cnt );
		nItem = m_ListCtrl.InsertItem( nItem, cntS );
		m_ListCtrl.SetItemText( nItem, 1, lineNumber );
		m_ListCtrl.SetItemText( nItem, 2, str );
		++nItem;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
    m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CInputErrors:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_EDIT ))
    {
		pPopup = Menu.GetSubMenu(0);

		if( m_ListCtrl.GetItemText( nItem, 1 ).IsEmpty() )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	1 ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputErrors::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	1 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
