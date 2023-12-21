// TableBlobs.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "utilities.h"
#include "Table_blobs.h"
#include "afxdialogex.h"
#include "Filter.h"
#include "Pictures.h"
#include "ProgressWnd.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDisplayBlob()
//{
//	CTableBlobs dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define PEOPLE		1
#define	MARRIAGE	2
#define TABLE		3

enum
{
	B_DUMMY = 0,
	B_ROWID,
	B_TABLE_ID,
	B_TABLE,
	B_ID,
	B_NAME,
	B_BIRTH,
	B_TITLE,
	B_FILENAME, 
	B_EXT,
	B_SIZE,
	B_DATE,
	B_COMMENT,
};

// CTableBlobs dialog

IMPLEMENT_DYNAMIC(CTableBlobs, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableBlobs::CTableBlobs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableBlobs::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableBlobs::~CTableBlobs()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableBlobs, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_COMMAND(ID_FILTER_PEOPLE, &CTableBlobs::OnFilterPeople)
	ON_COMMAND(ID_FILTER_TABLES, &CTableBlobs::OnFilterTables)
	ON_COMMAND(ID_FILTER_MARRIAGES, &CTableBlobs::OnFilterMarriages)
	ON_COMMAND(ID_UNFILTER, &CTableBlobs::OnUnfilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableBlobs::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_BN_CLICKED(ID_EDIT_UPDATE, &CTableBlobs::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CTableBlobs::OnEditDelete )

	ON_COMMAND(ID_FILTER_ONEPEOPLE, &CTableBlobs::OnFilterOnepeople)
	ON_COMMAND(ID_MENT_ALL, &CTableBlobs::OnMentAll)
	ON_COMMAND(ID_MENT_SELECTED, &CTableBlobs::OnMentSelected)
	ON_COMMAND(ID_ATTESZ, &CTableBlobs::OnAttesz)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableBlobs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( B_DUMMY,			L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_ROWID,			L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_NUM );
	m_ListCtrl.InsertColumn( B_TABLE_ID,		L"tábla",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_TABLE,			L"tábla",		LVCFMT_LEFT,	 60,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_ID,				L"id",			LVCFMT_RIGHT,	 40,-1,COL_NUM );
	m_ListCtrl.InsertColumn( B_NAME,			L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_BIRTH,			L"születés",	LVCFMT_LEFT,	 80,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_TITLE,			L"cím",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_FILENAME,		L"fájlnév",		LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_EXT,				L"ext",			LVCFMT_LEFT,	 30,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_SIZE,			L"hossz",		LVCFMT_RIGHT,	 70,-1,COL_NUM );
	m_ListCtrl.InsertColumn( B_DATE,			L"dátum",		LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_COMMENT,			L"leírás",	LVCFMT_LEFT,    1000,-1,COL_TEXT );

	m_orderix = 1;
	OnUnfilter();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableBlobs::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CTableBlobs::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = (int)wParam;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnUnfilter()
{
	m_filterTextNew.Format(L"Fényképek, dokumentumok az adatbázisban");
	m_filterNew.Format( L"" );
	m_AndOr = false;
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnFilterPeople()
{
	m_filterTextNew.Format(L"Emberek fényképei, dokumentumai");
	m_filterNew.Format( L"table_id=%d", PEOPLE );
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnFilterTables()
{
	m_filterTextNew.Format(L"Táblák fényképei, dokumentumai");
	m_filterNew.Format( L"table_id=%d", TABLE );
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnFilterMarriages()
{
	m_filterTextNew.Format(L"Házasságok fényképei, dokumentumai");
	m_filterNew.Format( L"table_id=%d", MARRIAGE );
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnFilterOnepeople()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString id;
	CString name;

	id		= m_ListCtrl.GetItemText( nItem, B_ID );
	name	= m_ListCtrl.GetItemText( nItem, B_NAME );

	m_filterTextNew.Format(L"%s fényképei, dokumentumai", name );
	m_filterNew.Format( L"id=%s", id );
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::fillBlobTable()
{
	CFilter dlg;
	int		ret;

	int table_id;
	CString rowid;
	CString id;
	CString table;
	CString name;
	CString relation;

	if( m_filterNew.IsEmpty() )
	{
		m_filterText = m_filterTextNew;
		m_filter.Empty();
		m_command = L"SELECT rowid,* FROM pictures";
	
	}
	else
	{
		if( !m_filter.IsEmpty() && m_AndOr )					// már van egy szûrés)					// már van egy szûrés
		{
			dlg.m_filterText1 = m_filterText; 
			dlg.m_filterText2 = m_filterTextNew; 
			dlg.m_filter1 = m_filter;
			dlg.m_filter2 = m_filterNew;

			if( dlg.DoModal() == IDCANCEL ) return;
			m_filterNew = m_filter;
			m_filterTextNew = m_filterText;

			
		}
		m_filter		= m_filterNew;
		m_filterText	= m_filterTextNew;
		m_command.Format( L"SELECT rowid,* FROM pictures WHERE %s", m_filter );
	}

	SetWindowText( m_filterText );
	if( !theApp.queryBlob( m_command ) ) return;

	u_int64 sizeBlob;
	u_int64 sizeTotal = 0;
	CString spouse_id;
	CString birth;

	m_ListCtrl.DeleteAllItems();
	int nItem = 0;
	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i, theApp.m_recordsetBlob->MoveNext() )
	{
		rowid	= theApp.m_recordsetBlob->GetFieldString( PIC_ID);
		table_id = _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_TABLE_ID) );
		switch( table_id )
		{
		case PEOPLE:
			table = L"ember";
			m_command.Format( L"SELECT last_name, first_name, birth_date FROM people WHERE rowid='%s'", rowid  );
			if( !theApp.query( m_command ) ) return;
			name.Format( L"%s %s", theApp.m_recordset->GetFieldString( 0), theApp.m_recordset->GetFieldString( 1 ) );
			birth = theApp.m_recordset->GetFieldString( 2 );

			break;
		case TABLE:
			table = L"tábla";
			m_command.Format( L"SELECT tableHeader FROM tables WHERE rowid='%s'", rowid  );
			if( !theApp.query( m_command ) ) return;
			name = theApp.m_recordset->GetFieldString( 0 );
			break;
		case MARRIAGE:
			table = L"házasság";
			m_command.Format( L"SELECT husband_id, wife_id FROM marriages WHERE rowid='%s'", rowid  );
			if( !theApp.query( m_command ) ) return;
			spouse_id = theApp.m_recordset->GetFieldString( 0 );
			m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", spouse_id  );
			if( !theApp.query( m_command ) ) return;
			name.Format( L"%s %s házassága", theApp.m_recordset->GetFieldString( 0 ), theApp.m_recordset->GetFieldString( 1 ) ); 
			break;
		default:
			table= L"";
			name = L"";
		}

		rowid		= theApp.m_recordsetBlob->GetFieldString( PIC_ROWID);
		id			= theApp.m_recordsetBlob->GetFieldString( PIC_ID);
		sizeBlob	= _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_SIZE ) ); 

		nItem = m_ListCtrl.InsertItem( nItem, rowid );
		m_ListCtrl.SetItemText( nItem, B_ROWID, rowid );
		m_ListCtrl.SetItemText( nItem, B_TABLE_ID, getString( table_id ) );
		m_ListCtrl.SetItemText( nItem, B_ID, id );
		m_ListCtrl.SetItemText( nItem, B_TABLE, table );
		m_ListCtrl.SetItemText( nItem, B_ID, id );
		m_ListCtrl.SetItemText( nItem, B_NAME, name );
		m_ListCtrl.SetItemText( nItem, B_BIRTH, birth );
		m_ListCtrl.SetItemText( nItem, B_TITLE, theApp.m_recordsetBlob->GetFieldString( PIC_TITLE) );
		m_ListCtrl.SetItemText( nItem, B_FILENAME, theApp.m_recordsetBlob->GetFieldString( PIC_FILENAME ) );
		m_ListCtrl.SetItemText( nItem, B_EXT, theApp.m_recordsetBlob->GetFieldString( PIC_EXT) );
		m_ListCtrl.SetItemText( nItem, B_SIZE, thousand( sizeBlob ) );
		m_ListCtrl.SetItemText( nItem, B_DATE, theApp.m_recordsetBlob->GetFieldString( PIC_DATE) );
		m_ListCtrl.SetItemText( nItem, B_COMMENT, theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT) );
		++nItem;
		sizeTotal += sizeBlob;
	}
	m_ListCtrl.SetSortOrder( 1, m_orderix+1 );
	str.Format( L"%s (%s bájt)", m_filterText, thousand( sizeTotal ) ); 
	SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableBlobs::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	MENU_IDR = IDR_DROPDOWN_EDIT;
	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->EnableMenuItem(ID_EDIT_INSERT,MF_BYCOMMAND | MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTableBlobs::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnEditUpdate();
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnEditUpdate()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid = m_ListCtrl.GetItemText( nItem, B_ROWID );
	CString id = m_ListCtrl.GetItemText( nItem, B_ID );
	if( rowid.IsEmpty() ) return;
	CString ext = m_ListCtrl.GetItemText( nItem, B_EXT );
	CString rowidB = m_ListCtrl.GetItemText( nItem, B_ROWID );

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLE, rowid ); 
	dlg.m_filter.Format( L"WHERE rowid='%s'", rowid ); 
	dlg.m_name		= L"Képek az adatbázisban";
	dlg.m_rowidP	= id;	// ember rowid-ja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:

//	dlg.m_filter.Empty();
/*
	dlg.m_rowidP.Empty();
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  A tábla összes képeit a rowidB-vel kezdve mutassa be
*/
	dlg.DoModal();

	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnEditDelete()
{
	UINT i, uSelectedCount = m_ListCtrl.GetSelectedCount();
	int  nItem = -1;
	
	str.Format( L"Tényleg töröni akarsz %d képet?", uSelectedCount );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
	
	std::vector<CString> vRowid;
	// Update all of the selected items.
	vRowid.clear();
	if (uSelectedCount > 0)
	{
		for (i=0; i < uSelectedCount; i++)
		{
			nItem = m_ListCtrl.GetNextItem(nItem, LVNI_SELECTED);
			vRowid.push_back( m_ListCtrl.GetItemText( nItem, B_ROWID ) );
		}
	}

	for( i = 0; i < uSelectedCount; ++i )
	{
		m_command.Format( L"DELETE FROM pictures WHERE rowid='%s'", vRowid.at(i) );
		if( !theApp.executeBlob( m_command ) ) return;
	}
	fillBlobTable();

/*
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid;
	CString title;
	rowid = m_ListCtrl.GetItemText( nItem, B_ROWID );
	title = m_ListCtrl.GetItemText( nItem, B_FILENAME );

	str.Format( L"Tényleg törölni akarod a '%s' fájlt?", title );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_command.Format( L"DELETE FROM pictures WHERE rowid='%s'", rowid );
	theApp.executeBlob( m_command );
	m_ListCtrl.DeleteItem( nItem );
*/
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnMentAll()
{
	theApp.selectAll( &m_ListCtrl );
	OnMentSelected();
	theApp.unselectAll( &m_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnMentSelected()
{
	CString path1;
	CString path2;
	CString path3;
	CString name;
	CString firstName;
	CString lastName;
	CString birthDate;
	CString fileName;
	CString ext;
	int p;

	CProgressWnd wndProgress(NULL,L"Könyvtárak létrehozása..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_ListCtrl.GetItemCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	path1 = theApp.m_blobPathName;
	if( ( p = path1.ReverseFind( '.' ) ) != -1 )
		path1 = path1.Left( p );
	path1 += L"_picture";
	if( _waccess( path1, 0 ) )	_wmkdir( path1 );


	CString fileSpec;
	CString rowid;
	CString id;
	int		errno_t;
	_int64 blob_size;
	void* block;
	int nItem;
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while( pos )
	{
		nItem	= m_ListCtrl.GetNextSelectedItem( pos );
		rowid	= m_ListCtrl.GetItemText( nItem, B_ROWID );
		id		= m_ListCtrl.GetItemText( nItem, B_ID );
		name	= m_ListCtrl.GetItemText( nItem, B_NAME );
		if( ( p = name.Find( ' ' ) ) != -1 )
		{
			lastName = name.Left( p );
			firstName = name.Mid( p+1 );
		}
		else
		{
			lastName = name;
			firstName.Empty();
		}
			
		birthDate	= m_ListCtrl.GetItemText( nItem, B_BIRTH );
		fileName	= m_ListCtrl.GetItemText( nItem, B_FILENAME );
		ext			= m_ListCtrl.GetItemText( nItem, B_EXT );
		path2.Format( L"%s\\%s", path1, lastName );
		if( _waccess( path2, 0 ) )	_wmkdir( path2 );

		path3.Format( L"%s\\%s_%s_%s", path2, firstName,id,birthDate );
		if( _waccess( path3, 0 ) )	_wmkdir( path3 );

		block = theApp.blobDB->blobRead( "pictures", "picture", rowid, &blob_size );
		if( block == NULL ) continue;


		char error_msg[100];

		fileSpec.Format( L"%s\\%s.%s", path3, fileName, ext );
			
		if( !openFileSpec( &theApp.fl, fileSpec, L"wb" ) ) continue;
/*
		if((errno_t=_wfopen_s(&fl, fileSpec,L"wb") )!=0)
		{	
			strerror_s( error_msg, sizeof(error_msg));
			str.Format(L"%s\nmegnyitási hiba!\n%s", fileSpec, error_msg );
			AfxMessageBox(str);
			continue;
		}
*/
		if( fwrite( block, blob_size, 1, theApp.fl ) != 1 )
		{
			AfxMessageBox( L"fwrite failed!" );
			fclose( theApp.fl );
			continue;
		}
		fclose( theApp.fl );
	
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableBlobs::OnAttesz()
{
	m_command.Format( L"UPDATE pictures SET title=filename");
	if( !theApp.executeBlob( m_command ) )return; 



}
