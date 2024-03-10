// HtmlEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_EditLines.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "EditItem.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Az MFC Dynamic Layout-al  Debug m�dban lerobban a program, ez�rt EasySize-al t�rt�nik az �tm�retez�s.
//  Egyszer�bb dialog ablakokn�l m�k�dik DEbug m�dban is.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implicit vagy explicit megadott GA.html sorokat list�zza.
// B�rmelyik sorra duplakattint�ssal a sort a szerkeszt� ablakba helyezi. Ott szerkeszthetj�k, majd vissza�rhatjuk a ga-html f�jlba.
// Ezt megism�telhetj�k b�rmelyik sorral, ak�r h�nyszor.
// A "Kil�p" gombra kattintva l�phet�nk ki.
//
// Public v�toz�k:
//
// CString m_type			implicit megadott sorok list�z�sa
//							F_SIBLINGS		- az m_rowid-val megadott ember apj�t �s testv�reit list�zza
//							FATHERMOTHERHE	- az m_rowid-val megadott emeber sz�leit �s testv�reit list�zza
//
// vector<int>*vLines		a list�zand� sorok sorsz�m�t tartalmaz� vektor, ha nincs m_type megadva
//
// CString m_title			az ablak felirat

IMPLEMENT_DYNAMIC(CHtmlEditLines, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLines::CHtmlEditLines(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlEditLines::IDD, pParent)
{
	m_linenumber = 0;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlEditLines::~CHtmlEditLines()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlEditLines, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlEditLines::OnDblclkList)
	ON_COMMAND(ID_LISTA, &CHtmlEditLines::OnListLines)
	ON_BN_CLICKED(IDC_MODIFY, &CHtmlEditLines::OnClickedModify)
	ON_EN_CHANGE(IDC_EDIT, &CHtmlEditLines::OnChangeEdit)
	ON_COMMAND(ID_INFO, &CHtmlEditLines::OnInfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CHtmlEditLines::OnClickList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlEditLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	IDC_LIST,		ES_BORDER,		ES_BORDER,		0 );
	EASYSIZE_ADD( IDC_STATIC_GYUJTO,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_STATIC_EDITBOX,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );

	EASYSIZE_ADD( IDC_MODIFY,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_ADD( IDCANCEL,	ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,		0);
	EASYSIZE_INIT()


	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	

	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );

	if( m_type == L"F_SIBLINGS" )
	{
		fatherAndSiblings();
		return true;
	}
	if( m_type == L"F_CHILDREN" )
	{
		children();
		return true;
	}
	if( m_linenumber )
	{
		displayLine();
		return true;
	}

	if( vLines->size() )
	{
		displayLines();
		return true;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CHtmlEditLines::getHtmlLine( CString lineNumber )
{
	if( lineNumber.IsEmpty() ) return L"";
	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );
	CStdioFile file( theApp.m_htmlPathName, CFile::modeRead); 
	
	CString cLine;
	int ln = _wtoi( lineNumber );

	for( int i = 0; i < ln - 2; ++i ) file.ReadString( cLine );  // el�z� sor el� megy
	file.ReadString( cLine );											// el�z� sor

	file.ReadString( cLine );											// k�rd�ses sor
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	
	cLine = cleanHtmlLine( cLine );
	file.Close();
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CHtmlEditLines::cleanHtmlLine( CString cLine )
{
	CString htmlLine = cLine;
	int		pos;
	TCHAR	gen;

	if( ( pos = cLine.ReverseFind('>') ) != -1 )
	{
		cLine = cLine.Mid( pos+1 );
		gen = cLine[0];
		if( ( pos = cLine.ReverseFind(';') ) != -1 )
		{
			cLine = cLine.Mid( pos+1 );
			htmlLine.Format( L"%c %s", gen, cLine ); 
		}
	}
	return htmlLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nItem			= pNMItemActivate->iItem;
	CString line;

	m_linenumber = _wtoi( m_ListCtrl.GetItemText( m_nItem, 1 ) );
	line = m_ListCtrl.GetItemText( m_nItem, 2 );
	line = line.Mid( 2 );		// geer�ci�s k�d eldob�sa

	GetDlgItem( IDC_EDIT )->SetWindowTextW( line );

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnListLines()
{
	CString	logFile(L"probl�m�s_html_Sorok"); 
	CString	title = logFile;
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnClickedModify()
{
	GetDlgItem( IDC_EDIT )->GetWindowTextW( m_line );

	theApp.saveHtmlLine( m_linenumber, m_line );

	m_ListCtrl.SetItemText( m_nItem, 2, m_line );
	GetDlgItem( IDC_MODIFY )->EnableWindow( FALSE );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( L"" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnChangeEdit()
{
	GetDlgItem( IDC_MODIFY )->EnableWindow( TRUE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::children()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;

	if( !theApp.m_recordset->RecordsCount() )
	{
		str.Format( L"Nincs %s rowid-j� ember az adatb�zisban!", m_rowid );
		AfxMessageBox( str );
		CDialog::OnCancel();
		return;
	}

	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	nItem = m_ListCtrl.InsertItem( 0, L"sz�l�k" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' OR mother_id='%s' ORDER BY linenumber", m_rowid, m_rowid  );
	if( !theApp.query( m_command ) ) return;

	for( INT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		linenumber	= theApp.m_recordset->GetFieldString( 0 );
		line		= getHtmlLine( linenumber );
		nItem = m_ListCtrl.InsertItem( i+1, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumber );
		m_ListCtrl.SetItemText( nItem, 2, line );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::fatherAndSiblings()
{
	if( m_rowid.IsEmpty() ) return;

	SetWindowTextW( m_title );

	int nItem;
	CString line;
	CString linenumber;
	CString father_id;
	CString linenumberC;

	m_command.Format( L"SELECT father_id, linenumber FROM people WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	father_id	= theApp.m_recordset->GetFieldString( 0 );
	if( father_id.IsEmpty() || father_id == L"0" )
	{
		AfxMessageBox( L"Nincs apja" );
		CDialog::OnCancel();
		return;
	}

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);


	CProgressWnd wndP(NULL, L"Keresem a sz�l�ket �s testv�reket..." ); 
	wndP.GoModal();

	linenumberC	= theApp.m_recordset->GetFieldString( 1 );

	m_command.Format( L"SELECT linenumber FROM people WHERE rowid ='%s'", father_id );
	if( !theApp.query( m_command ) ) return;
	linenumber	= theApp.m_recordset->GetFieldString( 0 );
	line		= getHtmlLine( linenumber );
	
	
	nItem = m_ListCtrl.InsertItem( 0, L"sz�l�k" );
	m_ListCtrl.SetItemText( nItem, 1, linenumber );
	m_ListCtrl.SetItemText( nItem, 2, line );

	m_command.Format( L"SELECT linenumber FROM people WHERE father_id ='%s' ORDER BY linenumber", father_id );
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange( 0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( INT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		linenumber	= theApp.m_recordset->GetFieldString( 0 );
		line		= getHtmlLine( linenumber );
		nItem = m_ListCtrl.InsertItem( 2+i, L"gyerek" );
		m_ListCtrl.SetItemText( nItem, 1, linenumber );
		m_ListCtrl.SetItemText( nItem, 2, line );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled())
		{
			CDialog::OnCancel();
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::displayLine( )
{
	CString linenumber;
	CString line;

	linenumber.Format( L"%d", m_linenumber ); 
	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);
	
	line = getHtmlLine( linenumber );

	m_ListCtrl.InsertItem( 0, L"" );
	m_ListCtrl.SetItemText( 0, 1, linenumber );
	m_ListCtrl.SetItemText( 0, 2, line );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::displayLines( )
{
	int nItem;
	CString line;

	CProgressWnd wndP(NULL, L"Keresem a kijel�lt sorokat..." ); 
	wndP.GoModal();
	wndP.SetRange( 0, vLines->size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	m_ListCtrl.InsertColumn( 0,	L"",		LVCFMT_RIGHT,	 120,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	  80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"ga.line",	LVCFMT_LEFT,    1500,-1,COL_EDIT);

	for( UINT i = 0; i < vLines->size(); ++ i )
	{
		nItem = m_ListCtrl.InsertItem( i, L"" );
		m_ListCtrl.SetItemText( nItem, 1, vLines->at(i) );
		line = getHtmlLine( vLines->at(i) );
		m_ListCtrl.SetItemText( nItem, 2, line );

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	SetWindowTextW( m_title );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnInfo()
{
	CString info = L"\
A GA.html f�jl kijel�lt sorait a gy�jt� ablakba olvassa be. A bal eg�rgombbal 1x r�klikkelve egy sorra, \
a sort �thelyezhetj�k a szerkeszt� ablakba.\r\n\r\n\
A szerkeszt� ablakban m�dos�thatjuk a sort. A gy�jt� ablakban 2x kattintva a bal eg�rgombbal, a sor \
szerkeszthet�v� v�lik, de csak arra haszn�ljuk, hogy Ctrl-C, Ctrl-V-vel �thelyezhet�nk sz�vegr�szeket \
a szerkesztend� sorba.\r\n\
A \"M�dos�t\" gomb csak a szerkeszt� ablakban l�v� sort �rja vissza  a GA.htm f�jlba!!!\r\n\r\n\
A \"M�dos�t\" gomb visszateszi a m�dos�tott sort a f�jlba �s a gy�jt� ablakba is.\r\n\
Ha sz�ks�ges, a fenti m�don folytathatjuk a szerkeszt�s�t egy m�sik sorral.\
";
	AfxMessageBox( info, MB_ICONINFORMATION );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlEditLines::OnLista()
{




}
