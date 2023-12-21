// Languages.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "Languages.h"
#include "afxdialogex.h"
#include "GetString.h"


// CLanguages dialog

IMPLEMENT_DYNAMIC(CLanguages, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLanguages::CLanguages(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLanguages::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLanguages::~CLanguages()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListBox);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLanguages, CDialogEx)
	ON_MESSAGE(WM_LISTBOX_MENU, OnListBoxMenu)
	ON_COMMAND(ID_EDIT_UPDATE, &CLanguages::OnEditUpdate)
	ON_COMMAND(ID_EDIT_INSERT, &CLanguages::OnEditInsert)
	ON_COMMAND(ID_EDIT_DELETE, &CLanguages::OnEditDelete)
	ON_BN_CLICKED(IDOK, &CLanguages::OnBnClickedOk)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLanguages::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	fillTable();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::fillTable()
{
	m_command = L"SELECT rowid,* FROM languages";
	if( !theApp.querySystem( m_command ) ) return;

	CString language;
	CString rowid;
	int index;

	m_ListBox.ResetContent();
	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i, theApp.m_recordsetSystem->MoveNext() )
	{
		language	= theApp.m_recordsetSystem->GetFieldString( LANG_LANGUAGE );
		rowid		= theApp.m_recordsetSystem->GetFieldString( LANG_ROWID );
		index = m_ListBox.AddString( language );
		m_ListBox.SetItemData( index, _wtoi(rowid) );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CLanguages::OnListBoxMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR = IDR_DROPDOWN_EDIT;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListBox.GetCurSel()  == LB_ERR )
		{
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::OnEditUpdate()
{
	int index = m_ListBox.GetCurSel();
	int	rowid = m_ListBox.GetItemData( index );
	CString language;

	int n = m_ListBox.GetTextLen( index );
	m_ListBox.GetText( index, language.GetBuffer(n));
	language.ReleaseBuffer();

	CGetString dlg;

	dlg.m_caption	= L"Nyelv módosítása";
	dlg.m_string	= language;
	if( dlg.DoModal() == IDCANCEL ) return;

	language = dlg.m_string;

	m_command.Format( L"UPDATE languages SET language='%s' WHERE rowid='%d'", language, rowid );
	if( !theApp.executeSys( m_command ) ) return;

	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::OnEditInsert()
{
	CString language;
	CString	rowid;
	int		index;

	CGetString dlg;

	
	dlg.m_caption = L"Add meg a nyelvet!";
	if( dlg.DoModal() == IDCANCEL ) return;
	language = dlg.m_string;

	m_command.Format( L"INSERT INTO languages (language) VALUES ('%s')", language );
	if( !theApp.executeSys( m_command ) ) return;

	m_command = L"SELECT last_insert_rowid() FROM languages";	
	if( !theApp.querySystem( m_command ) ) return;
	rowid = theApp.m_recordsetSystem->GetFieldString( 0 );

	index = m_ListBox.AddString( language );
	m_ListBox.SetItemData( index, _wtoi(rowid) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::OnEditDelete()
{
	int index = m_ListBox.GetCurSel();
	int	rowid = m_ListBox.GetItemData( index );
	CString language;

	int n = m_ListBox.GetTextLen( index );
	m_ListBox.GetText( index, language.GetBuffer(n));
	language.ReleaseBuffer();

	str.Format( L"Tényleg törölni akarod a '%s' nyelvet?", language );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_command.Format( L"DELETE FROM languages WHERE rowid = '%d'", rowid );
	if( !theApp.executeSys( m_command ) ) return;

	m_ListBox.DeleteString( index );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLanguages::OnBnClickedOk()
{
	int index = m_ListBox.GetCurSel();
	if( index == LB_ERR )
	{
		AfxMessageBox( L"Nincs kijelölve a nyelv!" );
		return;
	}

	m_language_id = m_ListBox.GetItemData( index );
	
	int n = m_ListBox.GetTextLen( index );
	m_ListBox.GetText( index, m_language.GetBuffer(n));
	m_language.ReleaseBuffer();


	CDialogEx::OnOK();
}
