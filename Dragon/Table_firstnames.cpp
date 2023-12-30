// Sexes.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "Table_firstnames.h"
#include "afxdialogex.h"
#include "EditText.h"
#include "ListBoxEx.h"
#include "SelectOne.h"
#include "SelectString.h"
#include "EditFirstName.h"
#include "Languages.h"
#include "NamesParam.h"
#include "ProgressWnd.h"

// CTableFirstnames dialog
enum
{
	L_CNT = 0,
	L_ROWID,
	L_FIRST_NAME,
	L_SEX,
	L_LANGUAGE,
	L_ORIGIN,
	L_MEANING,
	L_OCCURANCE,
	L_COMMENT,
};
IMPLEMENT_DYNAMIC(CTableFirstnames, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableFirstnames::CTableFirstnames(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableFirstnames::IDD, pParent)
{
	m_firstname.Empty();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableFirstnames::~CTableFirstnames()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableFirstnames, CDialogEx)
	ON_BN_CLICKED(ID_EDIT_UPDATE, &CTableFirstnames::OnClickedUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CTableFirstnames::OnClickedDelete )
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_LIST_SELECTED, &CTableFirstnames::OnListTable)
	ON_COMMAND(ID_UNFILTER, &CTableFirstnames::OnUnfilter)
	ON_COMMAND(ID_FILTER_INCIDENCE, &CTableFirstnames::OnFilterOccurance)
	ON_COMMAND(ID_FILTER_MAN, &CTableFirstnames::OnFilterMan)
	ON_COMMAND(ID_FILTER_WOMEN, &CTableFirstnames::OnFilterWomen)
//	ON_COMMAND(ID_MAN, &CTableFirstnames::OnMan)
//	ON_COMMAND(ID_MANANDWOMAN, &CTableFirstnames::OnBiSex)
//	ON_COMMAND(ID_WOMEN, &CTableFirstnames::OnWomen)
	ON_COMMAND(ID_FILTER_BISEX, &CTableFirstnames::OnFilterBisex)
	ON_COMMAND(ID_CHECK_USED, &CTableFirstnames::OnCheckUsed)
	ON_COMMAND(ID_FILTER_CORRECT, &CTableFirstnames::OnFilterCorrect)
	ON_COMMAND(ID_READTXTFILE, &CTableFirstnames::OnReadtxtfile)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableFirstnames::OnDblclkList)
	ON_COMMAND(ID_EDIT_INSERT, &CTableFirstnames::OnEditInsert)
	ON_COMMAND(ID_FILTER_LANGUAGE, &CTableFirstnames::OnFilterLanguage)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CTableFirstnames::OnClickedKeress)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFirstnames::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nItem = 1;

	CString name;
	
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_LEFT,   40,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,  40,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FIRST_NAME,	L"utónév",		LVCFMT_LEFT, 120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SEX,			L"nem",			LVCFMT_LEFT,  40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_LANGUAGE,	L"nyelv",		LVCFMT_LEFT,  80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ORIGIN,		L"eredet",		LVCFMT_LEFT,  80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MEANING,		L"jelentés",	LVCFMT_LEFT, 200,-1,COL_TEXT);	
	m_ListCtrl.InsertColumn( L_COMMENT,		L"leírás",	LVCFMT_LEFT, 120,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_OCCURANCE,	L"elõfordult",	LVCFMT_RIGHT, 80,-1,COL_NUM);

	colorKeress.SetTextColor( theApp.m_colorClick );


	m_command = L"UPDATE firstnames SET sex_id='0' WHERE typeof(sex_id) = 'null'";
	if(!theApp.executeSys(m_command)) return false;
	

	m_sexLast = 1;
	OnUnfilter();
	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnUnfilter()
{
	m_filter.Empty();
	m_filterText = L"Utónevek";
	fillTable();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterOccurance()
{
	CSelectString dlg;

	if( dlg.DoModal() == IDCANCEL ) return;

	m_filter.Format( L"WHERE occurance='%s'", dlg.m_selected );
	m_filterText = L"gyakoriság";
	fillTable();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterMan()
{
	m_filter = L"WHERE sex_id='1'";
	m_filterText = L"Férfi nevek";
	fillTable();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterWomen()
{
	m_filter = L"WHERE sex_id='2'";
	m_filterText = L"Nõi nevek";
	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterBisex()
{
	m_filter = L"WHERE sex_id='0'";
	m_filterText = L"Bisex nevek";
	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterCorrect()
{
	m_filter = L"WHERE comment != ''";
	m_filterText = L"Javítandó nevek";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnFilterLanguage()
{
	CLanguages dlg;

	if( dlg.DoModal() == IDCANCEL ) return;
	m_filter.Format( L"WHERE language_id = '%d'", dlg.m_language_id );
	m_filterText.Format( L"%s nyelv utónevei", dlg.m_language );
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::fillTable()
{
	m_ListCtrl.DeleteAllItems();	

	if( m_filter.IsEmpty() )
		m_command.Format( L"SELECT rowid, *  FROM firstnames" );
	else
		m_command.Format( L"SELECT rowid, *  FROM firstnames %s",  m_filter );

	if( !theApp.queryName( m_command ) ) return;


	int nItem;
	for( UINT i = 0; i < theApp.m_recordsetName->RecordsCount(); ++i, theApp.m_recordsetName->MoveNext() )
	{
		str.Format( L"%d", i+1 );
		nItem = m_ListCtrl.InsertItem( i, str);
		fillRow( nItem );
	}
	m_orderix = 2;
	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );


	str.Format( L"%s (%d db)", m_filterText, m_ListCtrl.GetItemCount() );
	SetWindowTextW( str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::fillRow( int nItem )
{
	m_ListCtrl.SetItemText( nItem, L_ROWID, theApp.m_recordsetName->GetFieldString( SEX_ROWID ));
	m_ListCtrl.SetItemText( nItem, L_FIRST_NAME, theApp.m_recordsetName->GetFieldString( SEX_FIRST_NAME ));
	str.Empty();
	switch ( _wtoi( theApp.m_recordsetName->GetFieldString( SEX_SEX_ID ) ) )
	{
	case 0:
		str = L"bisex";
		break;
	case 1:
		str =L"férfi";
		break;
	case 2:
		str =L"nõi";
		break;
	}
	m_ListCtrl.SetItemText( nItem, L_SEX,str );

	int language_id;
	CString language;
	language_id = _wtoi( theApp.m_recordsetName->GetFieldString( SEX_LANGUAGE_ID ) );
	language = theApp.getLanguage( language_id );
	m_ListCtrl.SetItemText( nItem, L_LANGUAGE,	language );

	m_ListCtrl.SetItemText( nItem, L_ORIGIN,	theApp.m_recordsetName->GetFieldString( SEX_ORIGIN ));
	m_ListCtrl.SetItemText( nItem, L_MEANING,	theApp.m_recordsetName->GetFieldString( SEX_MEANING ));
	m_ListCtrl.SetItemText( nItem, L_COMMENT,	theApp.m_recordsetName->GetFieldString( SEX_COMMENT ));
	m_ListCtrl.SetItemText( nItem, L_OCCURANCE,	theApp.m_recordsetName->GetFieldString( SEX_OCCURANCE ));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Név módosítása
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	OnClickedUpdate();
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnEditInsert()
{
	CString search;
	while (TRUE)
	{
		GetDlgItemTextW(IDC_SEARCH, search);


		CEditFirstName dlg;
		dlg.m_firstname = search;
		dlg.m_rowid = L"";
		dlg.m_sexLast = m_sexLast;
		if (dlg.DoModal() == IDCANCEL)
			break;
		m_sexLast = dlg.m_sexLast;

		m_command.Format(L"SELECT rowid,* FROM firstnames WHERE rowid = '%s'", dlg.m_rowid);
		if (!theApp.queryName(m_command)) return;

		if (dlg.m_rowid.IsEmpty())
			fillTable();
		else
		{
			int nItem = m_ListCtrl.GetItemCount();
			nItem = m_ListCtrl.InsertItem(nItem, theApp.m_recordsetName->GetFieldString(SEX_FIRST_NAME));
			fillRow(nItem);
			m_ListCtrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ListCtrl.EnsureVisible(nItem, FALSE);
		}
		GetDlgItem( IDC_SEARCH)->SetWindowTextW(L"");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnClickedUpdate()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}


	CEditFirstName dlg;
	dlg.m_sexLast	= m_sexLast;
	dlg.m_rowid		= m_ListCtrl.GetItemText( nItem, L_ROWID );
	dlg.m_firstname = m_ListCtrl.GetItemText(nItem, L_FIRST_NAME);
	if( dlg.DoModal() == IDCANCEL ) return;
	m_sexLast = dlg.m_sexLast;

	m_command.Format( L"SELECT rowid,* FROM firstnames WHERE rowid = '%s'", dlg.m_rowid );
	if( !theApp.queryName( m_command ) ) return;

	fillRow( nItem );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnClickedDelete()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve senki!" );
		return;
	}
	CString first_name;

	first_name = m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );

	str.Format( L"Tényleg törölni akarod a '%s' utónevet?", first_name );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_command.Format( L"DELETE FROM firstnames WHERE first_name='%s'", first_name );
	if( !theApp.executeSys( m_command ) ) return;
	m_ListCtrl.DeleteItem( nItem );
	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableFirstnames::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
/*
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu(IDR_FIRSTNAMES))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
*/
	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFirstnames::firstNameExists( CString first_name )
{
	m_command.Format( L"SELECT rowid FROM firstnames WHERE first_name = '%s'", first_name );
	if( !theApp.querySystem( m_command ) ) return TRUE;
	if( theApp.m_recordsetSystem->RecordsCount() ) return TRUE;
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableFirstnames::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableFirstnames::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnListTable()
{
	CString	logFile(L"firstNames"); 
	CString title;
	int nItem;
	CString first_name;
	CString comment;
	CString sex;
	CString occurance;
	
	if( theApp.openLogFile( logFile, m_filterText ) == NULL)return;
	fwprintf( theApp.fl, L"\n%37s %s\n", L"nem", L"elõfordulás" );
	for( nItem = 0; nItem < m_ListCtrl.GetItemCount(); ++nItem )
	{
		first_name	= m_ListCtrl.GetItemText( nItem, L_FIRST_NAME );
		comment		= m_ListCtrl.GetItemText( nItem, L_COMMENT );
		sex			= m_ListCtrl.GetItemText( nItem, L_SEX );
		occurance	= m_ListCtrl.GetItemText( nItem, L_OCCURANCE );

		fwprintf( theApp.fl, L"%-15s %-15s %5s %3s\n", first_name, comment, sex, occurance );
	}
	fclose( theApp.fl );
	theApp.showLogFile();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnMan()
{
	CString rowid;
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve semmi!" );
		return;
	}
	m_ListCtrl.SetItemText( nItem, L_SEX, L"férfi" );
	rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	m_command.Format( L"UPDATE firstnames SET sex_id = '1' WHERE rowid='%s'", rowid );
	if( !theApp.executeSys( m_command ) ) return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnBiSex()
{
	CString rowid;
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve semmi!" );
		return;
	}
	m_ListCtrl.SetItemText( nItem, L_SEX, L"bisex" );
	rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	m_command.Format( L"UPDATE firstnames SET sex_id = '0' WHERE rowid='%s'", rowid );
	if( !theApp.executeSys( m_command ) ) return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnWomen()
{
	CString rowid;
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve semmi!" );
		return;
	}
	m_ListCtrl.SetItemText( nItem, L_SEX, L"nõi" );
	rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	m_command.Format( L"UPDATE firstnames SET sex_id = '2' WHERE rowid='%s'", rowid );
	if( !theApp.executeSys( m_command ) ) return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnCheckUsed()
{
	CString	rowid;
	CString	first_name;
	CString count;

	CProgressWnd wndProgress(NULL,L"Utónevek gyakoriságának meghatározása..." ); 
	wndProgress.GoModal();

	for( int i = 0; i < m_ListCtrl.GetItemCount(); ++i )
	{
		m_ListCtrl.SetItemText( i, L_OCCURANCE, L"" );
	}


	m_command.Format( L"SELECT rowid, first_name FROM firstnames" );
	if( !theApp.queryName( m_command ) ) return;

	m_command.Format( L"UPDATE firstnames SET occurance=''" );
	if( !theApp.executeSys( m_command ) ) return;

	wndProgress.SetRange(0, theApp.m_recordsetName->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	if( !theApp.executeSys( L"BEGIN" ) ) return;
	for( UINT i = 0; i < theApp.m_recordsetName->RecordsCount(); ++i, theApp.m_recordsetName->MoveNext() )
	{
		rowid = theApp.m_recordsetName->GetFieldString( 0 );
		first_name	= theApp.m_recordsetName->GetFieldString( 1 );

		m_command.Format( L"SELECT count() FROM people WHERE first_name= '%s' OR first_name LIKE'%s-%c' OR first_name LIKE'%c-%s'", first_name, first_name, '%', '%', first_name );
		if( !theApp.query2( m_command ) ) return;
		count = theApp.m_recordset2->GetFieldString(0 );
		m_command.Format( L"UPDATE firstnames SET occurance='%s' WHERE rowid='%s'", count, rowid );
		if( !theApp.executeSys( m_command ) ) return;

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( !theApp.executeSys( L"COMMIT" ) ) return;
	wndProgress.DestroyWindow();
	fillTable();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnReadtxtfile()
{
	CString fileSpec;
	CString cLine;
	CString first_name;
	CString sex;
	CString origin;
	CString meaning;
	int		nItem;
	CString	sex_id;
	int		language_id;
	int		sp;
	int		brace1;
	int		brace2;

	CNamesParam dlg;

	if( dlg.DoModal() == IDCANCEL ) return; 

	sex_id = dlg.m_sex_id;
	if( sex_id == MAN )
		sex = L"férfi";
	else
		sex = L"nõi";

	fileSpec	= dlg.m_fileSpec;
	language_id = dlg.m_language_id;
	

	CStdioFile file(fileSpec, CFile::modeRead); 
	if( !theApp.executeSys( L"BEGIN" ) ) return;
	int i = 0;

	m_ListCtrl.DeleteAllItems();
	while(file.ReadString(cLine)) 
	{
		cLine.Trim();
		if( cLine.IsEmpty() ) continue;

		if( ( sp = cLine.Find( ' ' ) ) != -1 )
		{
			first_name = cLine.Left( sp );
			first_name.MakeLower();
			LPTSTR buff = first_name.GetBuffer();
			buff[0] = towupper(buff[0]);
		}
		
		if( ( brace1 = cLine.Find( '(' ) ) != -1  && ( brace2 = cLine.Find( ')' ) ) != -1 	)
		{
			origin = cLine.Mid( brace1 + 1, brace2 - brace1 - 1 );
		}
		if( brace2 != -1 )
		{
			meaning = cLine.Mid( brace2 + 1 );
		}


		nItem = m_ListCtrl.InsertItem( i, first_name );
		m_ListCtrl.SetItemText( nItem, L_SEX, sex );
		m_ListCtrl.SetItemText( nItem, L_ORIGIN, origin );
		m_ListCtrl.SetItemText( nItem, L_MEANING, meaning );


		m_command.Format( L"DELETE FROM firstnames WHERE first_name = '%s'", first_name );
		if( !theApp.executeSys( m_command ) ) return;

		m_command.Format( L"INSERT INTO firstnames (first_name, sex_id, origin, meaning, language_id ) VALUES ('%s', %d, '%s', '%s', '%d')", first_name, sex_id, origin, meaning, language_id  );
		if( !theApp.executeSys( m_command ) ) return;
	}
	if( !theApp.executeSys( L"COMMIT" ) ) return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableFirstnames::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableFirstnames::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////