// TableFiles.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
#include "Table_files.h"
#include "afxdialogex.h"
#include "Table_GaTables.h"
#include "Table_html.h"


enum
{
	L_ROWID = 0,
	L_FILENAME,
	L_SIZE,
	L_CREATED,
	L_MODIFIED,
	L_LOADED,
};

enum
{
	D_ROWID = 0,
	D_FILENAME,
	D_SIZE,
	D_SOURCE,
	D_GEDCOM,
	D_CREATED,
	D_MODIFIED,
	D_LOADED,
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
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_BROWSER, &CTableFiles::OnBrowser)
	ON_COMMAND(ID_TEXTEDITOR, &CTableFiles::OnTexteditor)
	ON_COMMAND(ID_SELECT_TABLE, &CTableFiles::OnSelectTable)
	ON_COMMAND(ID_TO_TABLE, &CTableFiles::OnToTable)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );


	if( theApp.m_inputMode == GEDCOM )
	{
		m_ListCtrl.InsertColumn( D_ROWID,		L"#",				LVCFMT_RIGHT,	 20,-1,COL_NUM);
		m_ListCtrl.InsertColumn( D_FILENAME,	L"input fájl",		LVCFMT_LEFT,	300,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_SIZE,		L"hossz",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_SOURCE,		L"forrás",			LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_GEDCOM,		L"GEDCOM",			LVCFMT_LEFT,	 60,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_CREATED,		L"létrehozva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_MODIFIED,	L"módosítva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( D_LOADED,		L"beolvasva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	}
	else
	{
		m_ListCtrl.InsertColumn( L_ROWID,		L"#",				LVCFMT_RIGHT,	 20,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_FILENAME,	L"input fájl",		LVCFMT_LEFT,	300,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_SIZE,		L"hossz",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_CREATED,		L"létrehozva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_MODIFIED,	L"módosítva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_LOADED,		L"beolvasva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	}
	



	CString command;
	command = L"SELECT rowid,* FROM inputFiles";
	if( !theApp.query( command ) ) return FALSE;

	if( theApp.m_inputMode == GEDCOM )
	{
		for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
		{
			m_ListCtrl.InsertItem( i, theApp.m_recordset->GetFieldString( F_ROWID ) );
			m_ListCtrl.SetItemText( i, D_FILENAME, theApp.m_recordset->GetFieldString( F_FILENAME));
			m_ListCtrl.SetItemText( i, D_SIZE, theApp.m_recordset->GetFieldString( F_SIZE ));
			m_ListCtrl.SetItemText( i, D_SOURCE, theApp.m_recordset->GetFieldString( F_SOURCE ));
			m_ListCtrl.SetItemText( i, D_GEDCOM, theApp.m_recordset->GetFieldString( F_VERSION ));
			m_ListCtrl.SetItemText( i, D_CREATED, theApp.m_recordset->GetFieldString( F_CREATED ));
			m_ListCtrl.SetItemText( i, D_MODIFIED, theApp.m_recordset->GetFieldString( F_MODIFIED ));
			m_ListCtrl.SetItemText( i, D_LOADED, theApp.m_recordset->GetFieldString( F_LOADED ));

			theApp.m_recordset->MoveNext();
		}
	}
	else 
	{
		for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
		{
			m_ListCtrl.InsertItem( i, theApp.m_recordset->GetFieldString( F_ROWID ) );
			m_ListCtrl.SetItemText( i, L_FILENAME, theApp.m_recordset->GetFieldString( F_FILENAME));
			m_ListCtrl.SetItemText( i, L_SIZE, theApp.m_recordset->GetFieldString( F_SIZE ));
			m_ListCtrl.SetItemText( i, L_CREATED, theApp.m_recordset->GetFieldString( F_CREATED ));
			m_ListCtrl.SetItemText( i, L_MODIFIED, theApp.m_recordset->GetFieldString( F_MODIFIED ));
			m_ListCtrl.SetItemText( i, L_LOADED, theApp.m_recordset->GetFieldString( F_LOADED ));
			theApp.m_recordset->MoveNext();
		}
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableFiles::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu(IDR_DROPDOWN_DELETE))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnEditDelete()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}

	CString	rowid;
	CString filename;
	rowid		= m_ListCtrl.GetItemText( nItem, L_ROWID );
	filename	= m_ListCtrl.GetItemText( nItem, L_FILENAME );

	str.Format( L"Tényleg törölni akarod a %s fájlt?\na 'people','marriage' és 'files' táblákból?", filename );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM people WHERE fileNumber='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_command.Format( L"DELETE FROM marriages WHERE fileNumber='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_command.Format( L"DELETE FROM tables WHERE fileNumber='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_command.Format( L"DELETE FROM inputFiles WHERE rowid ='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	m_ListCtrl.DeleteItem( nItem );
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
	fileSpec = m_ListCtrl.GetItemText(n,L_FILENAME);

	theApp.showHtmlFile( fileSpec );
//	ShellExecute(NULL, L"open", fileSpec,NULL, NULL, SW_SHOWNORMAL);
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
	fileSpec = m_ListCtrl.GetItemText(n,L_FILENAME);

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

	CString fileSpec;
	fileSpec = m_ListCtrl.GetItemText(n,L_FILENAME);

	CString ext;
	int pos = fileSpec.ReverseFind( '.' );
	ext = fileSpec.Mid( pos + 1 );

	if( ext.CompareNoCase( L"html" ) && ext.CompareNoCase( L"htm" ) )
	{
		AfxMessageBox( L"Tábla kiválasztása csak GA.html fájlból lehetséges!" );
		return;
	}


	CString fileName;
	pos = fileSpec.ReverseFind( '\\' );
	fileName = fileSpec.Mid( pos + 1 );


	CTableGaTables dlg;
	dlg.m_fileName =
	dlg.m_fileSpec = fileSpec;
	dlg.DoModal();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFiles::OnToTable()
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölve semmi!"); 
		return;
	}

	CString fileSpec;
	fileSpec = m_ListCtrl.GetItemText(n,L_FILENAME);

	CString ext;
	int pos = fileSpec.ReverseFind( '.' );
	ext = fileSpec.Mid( pos + 1 );

	if( ext.CompareNoCase( L"html" ) && ext.CompareNoCase( L"htm" ) )
	{
		AfxMessageBox( L"Táblázat megjelenítése csak GA.html fájlból lehetséges!" );
		return;
	}


	CString fileName;
	pos = fileSpec.ReverseFind( '\\' );
	fileName = fileSpec.Mid( pos + 1 );

	CTableHtml dlg;

	dlg.m_fileSpec = fileSpec;
	dlg.m_fileName = fileName;
	dlg.DoModal();
}
