// TableFiles.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "Table_files.h"
#include "afxdialogex.h"
#include "Table_GaTables.h"
#include "Table_html.h"
#include "utilities.h"


enum
{
	L_ROWID = 0,
	L_TYPE,
	L_FILESPEC,
	L_CREATED,
	L_MODIFIED,
	L_LOADED,
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTableFiles, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableFiles::CTableFiles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableFiles::IDD, pParent)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableFiles::~CTableFiles()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableFiles, CDialogEx)
	ON_COMMAND(ID_BROWSER, &CTableFiles::OnBrowser)
	ON_COMMAND(ID_TEXTEDITOR, &CTableFiles::OnTexteditor)
	ON_COMMAND(ID_SELECT_TABLE, &CTableFiles::OnSelectTable)

	ON_UPDATE_COMMAND_UI(ID_SELECT_TABLE, &CTableFiles::OnUpdateSelectTable)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );


	m_ListCtrl.InsertColumn( L_ROWID,		L"#",				LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TYPE,		L"típus",			LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FILESPEC,	L"fájl specifikáció",		LVCFMT_LEFT,	450,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_CREATED,		L"létrehozva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MODIFIED,	L"módosítva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LOADED,		L"beolvasva",		LVCFMT_LEFT,	120,-1,COL_TEXT);


	m_command.Format( L"SELECT rowid,* FROM filespec");
	if( !theApp.query( m_command ) ) return FALSE;

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
	{
		m_ListCtrl.InsertItem( i, theApp.m_recordset->GetFieldString( FS_ROWID ) );
		m_ListCtrl.SetItemText( i, L_TYPE, theApp.m_recordset->GetFieldString( FS_TYPE));
		m_ListCtrl.SetItemText( i, L_FILESPEC, theApp.m_recordset->GetFieldString( FS_FILESPEC));
		m_ListCtrl.SetItemText( i, L_CREATED, theApp.m_recordset->GetFieldString( FS_CREATED ));
		m_ListCtrl.SetItemText( i, L_MODIFIED, theApp.m_recordset->GetFieldString( FS_MODIFIED ));
		m_ListCtrl.SetItemText( i, L_LOADED, theApp.m_recordset->GetFieldString( FS_LOADED ));
		theApp.m_recordset->MoveNext();
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);


	CMenu* pMenu = GetMenu();
	if( theApp.m_inputMode != GAHTML )
	{
		pMenu->EnableMenuItem( 2,MF_BYPOSITION| MF_GRAYED );
		pMenu->EnableMenuItem( 3,MF_BYPOSITION| MF_GRAYED );
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFiles::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
    return CWnd::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnBrowser()
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}
	
	CString fileSpec;
	fileSpec = m_ListCtrl.GetItemText(n,L_FILESPEC);
	CString ext;
	int pos;
	if( (pos = fileSpec.ReverseFind( '.' )) != -1 );
	{
		ext = fileSpec.Mid( pos+1 );
		ext.MakeUpper();
		if( ext != L"HTML" && ext != L"HTM" )
		{
			AfxMessageBox( L"Csak html fájlt lehet a browserban megnézni!" );
			return;
		}
	}
	theApp.showHtmlFile( fileSpec );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnTexteditor()
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}
	
	CString fileSpec;
	fileSpec = m_ListCtrl.GetItemText(n,L_FILESPEC);

	theApp.showFile( fileSpec );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnSelectTable()
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}
	int type = _wtoi( m_ListCtrl.GetItemText(n,L_TYPE) );
	if( type != GA_HTML )
	{
		AfxMessageBox( L"Táblázat megjelenítése csak GA.html fájlból lehetséges!" );
		return;
	}

	CString fileSpec;
	fileSpec = m_ListCtrl.GetItemText(n,L_FILESPEC);
	CString fileName;
	int pos = fileSpec.ReverseFind( '\\' );
	fileName = fileSpec.Mid( pos + 1 );


	CTableGaTables dlg;
	dlg.m_fileName =
	dlg.m_fileSpec = fileSpec;
	dlg.DoModal();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnUpdateSelectTable(CCmdUI *pCmdUI)
{
	int	nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( nItem < 0 )	return;
	int type = _wtoi( m_ListCtrl.GetItemText( nItem, L_TYPE) );
	pCmdUI->Enable( !type );
	CMenu menu;
	menu.LoadMenuW(IDR_TABLE_FILES );
	menu.EnableMenuItem( ID_SELECT_TABLE, MF_BYCOMMAND | MF_GRAYED);
}
