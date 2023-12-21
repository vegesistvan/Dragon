// TableFiles.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_files.h"
#include "afxdialogex.h"
#include "Table_GaTables.h"
#include "Table_html.h"
#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDisplayFilestable()
//{
//	CTableFiles dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	L_ROWID = 0,
	L_TYPE,
	L_FILENAME,
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
	SetWindowTextW(theApp.m_caption);

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );


	m_ListCtrl.InsertColumn( L_ROWID,		L"#",				LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TYPE,		L"típus",			LVCFMT_LEFT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_FILENAME,	L"fájl név",		LVCFMT_LEFT,	450,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_CREATED,		L"létrehozva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MODIFIED,	L"módosítva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LOADED,		L"beolvasva",		LVCFMT_LEFT,	120,-1,COL_TEXT);


	m_command.Format( L"SELECT rowid,* FROM filespec");
	if( !theApp.query( m_command ) ) return FALSE;

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
	{
		m_ListCtrl.InsertItem( i, theApp.m_recordset->GetFieldString( DBF_ROWID ) );
		m_ListCtrl.SetItemText( i, L_TYPE, theApp.m_recordset->GetFieldString( DBF_TYPE));
		m_ListCtrl.SetItemText( i, L_FILENAME, theApp.m_recordset->GetFieldString( DBF_FILENAME));
		m_ListCtrl.SetItemText( i, L_CREATED, theApp.m_recordset->GetFieldString( DBF_CREATED ));
		m_ListCtrl.SetItemText( i, L_MODIFIED, theApp.m_recordset->GetFieldString( DBF_MODIFIED ));
		m_ListCtrl.SetItemText( i, L_LOADED, theApp.m_recordset->GetFieldString( DBF_LOADED ));
		theApp.m_recordset->MoveNext();
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);


	CMenu* pMenu = GetMenu();
	if( theApp.m_inputMode != GAHTML )
	{
		pMenu->EnableMenuItem( 0, MF_BYPOSITION | MF_GRAYED);
		pMenu->EnableMenuItem( 2,MF_BYPOSITION| MF_GRAYED );
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
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
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
	
	CString filename;
	filename = m_ListCtrl.GetItemText(n,L_FILENAME);
	CString ext;
	int pos;
	if( (pos = filename.ReverseFind( '.' )) != -1 );
	{
		ext = filename.Mid( pos+1 );
		ext.MakeUpper();
		if( ext != L"HTML" && ext != L"HTM" )
		{
			AfxMessageBox( L"Csak html fájlt lehet a browserban megnézni!" );
			return;
		}
	}
	CString filespec;
	filespec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);
	theApp.showHtmlFile( filespec );
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
	
	CString filename;
	CString filespec;
	filename = m_ListCtrl.GetItemText(n, L_FILENAME);
	filespec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);
	theApp.showFile( filespec );
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
	if( type != HTML_FILE )
	{
		AfxMessageBox( L"Táblázat megjelenítése csak GA.html fájlból lehetséges!" );
		return;
	}

	CString filename;
	CString filespec;
	filename = m_ListCtrl.GetItemText(n,L_FILENAME);
	filespec.Format(L"%s\\%s", theApp.m_dbFolderPath, filename);

	CTableGaTables dlg;
	dlg.m_fileName = filename;
	dlg.m_fileSpec = filespec;
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
