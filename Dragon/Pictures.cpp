// Pictures.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Pictures.h"
#include "afxdialogex.h"
#include "utilities.h"

enum
{
	P_ROWID = 0,
	P_EXT,
	P_TITLE,
};



// egy ember képeinek megjelenítése és kezelése

IMPLEMENT_DYNAMIC(CPictures, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPictures::CPictures(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPictures::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPictures::~CPictures()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN, m_SpinCtrl);
	//  DDX_Control(pDX, IDC_TITLE, m_TitleCtrl);
	DDX_Control(pDX, IDC_PRIMARY, m_PrimaryCtrl);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_TITLE, m_TitleCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPictures, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, &CPictures::OnDeltaposSpin)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_TITLE, &CPictures::OnChangeTitle)
	ON_BN_CLICKED(IDC_VIEW, &CPictures::OnClickedView)
	ON_BN_CLICKED(IDC_PRIMARY, &CPictures::OnClickedPrimary)

	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDOK, &CPictures::OnBnClickedOk)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST, &CPictures::OnRdblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CPictures::OnClickList)

	ON_BN_CLICKED(ID_EDIT_UPDATE, &CPictures::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CPictures::OnEditDelete )
	ON_BN_CLICKED(ID_EDIT_INSERT, &CPictures::OnEditInsert )
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPictures::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	EASYSIZE_ADD( IDC_DELETE,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
//	EASYSIZE_ADD( IDC_VIEW,		ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,		ES_BORDER,	0 );

	EASYSIZE_ADD( IDC_VIEW,	ES_BORDER,	ES_KEEPSIZE,	ES_KEEPSIZE,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_TITLE,	ES_BORDER,	ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	ES_HCENTER );
	EASYSIZE_ADD( IDC_PRIMARY,	ES_BORDER,	ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	ES_HCENTER );
	EASYSIZE_ADD( IDC_LIST,		ES_BORDER,	ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	ES_HCENTER );
	EASYSIZE_ADD( IDC_SPIN,		ES_BORDER,	ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	ES_HCENTER );
//	EASYSIZE_ADD( IDOK,			ES_BORDER,	ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	ES_HCENTER );
	EASYSIZE_ADD( IDOK,			ES_KEEPSIZE,ES_KEEPSIZE,	ES_BORDER,	ES_BORDER,	0 );




	EASYSIZE_INIT();
	int		errno_t;
	FILE* fl;
	CString str;

	m_paint = false;
	
	CString command;
	CString title;
	CString comment;
	CString rowidB;
	CString date;
	int		primary;
/*
	command.Format( L"SELECT rowid, * FROM pictures %s", m_filter );   //WHERE table_id = '%d' AND id ='%s' ", m_table, m_rowidP );
	if( !theApp.queryBlob( command ) ) return false;
	if( !theApp.m_recordsetBlob->RecordsCount() ) return false;

	m_SpinCtrl.SetRange( 1, theApp.m_recordsetBlob->RecordsCount() );

	m_TitleCtrl.SetSel( -1, -1, true );
*/

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrl.InsertColumn( P_ROWID,	L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( P_EXT,		L"ext",			LVCFMT_RIGHT,	 40,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( P_TITLE,	L"cím",			LVCFMT_LEFT,	700,-1,COL_TEXT );

	fillBlobTable();
	if( theApp.m_recordsetBlob->RecordsCount() )
	{
		theApp.m_recordsetBlob->MoveFirst();

		m_cnt = 1;
		caption( m_cnt );
		paintBlob( 0 );

//	str.Format( L"%s képei", m_name);
//	SetWindowTextW( str );

		GetDlgItem( IDC_SPIN )->SetFocus();
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::fillBlobTable()
{
	u_int64 sizeTotal = 0;
	u_int64	sizeBlob;
	m_ListCtrl.DeleteAllItems();
	int nItem = 0;


	m_command.Format( L"SELECT rowid, * FROM pictures %s", m_filter );   //WHERE table_id = '%d' AND id ='%s' ", m_table, m_rowidP );
	if( !theApp.queryBlob( m_command ) ) return;
//	if( !theApp.m_recordsetBlob->RecordsCount() ) return;

/*
	if( !m_rowidB.IsEmpty() )  // megadott képpel kezdi a megjelenítést
	{
		for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i )
		{
			if( theApp.m_recordsetBlob->GetFieldString( PIC_ROWID ) == m_rowidB ) break;
			theApp.m_recordsetBlob->MoveNext();
		}
	}
*/
	m_SpinCtrl.SetRange( 1, theApp.m_recordsetBlob->RecordsCount() );

//	m_TitleCtrl.SetSel( -1, -1, true );

	theApp.m_recordsetBlob->MoveFirst();
	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i, theApp.m_recordsetBlob->MoveNext() )
	{
		nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordsetBlob->GetFieldString( PIC_ROWID) );
		m_ListCtrl.SetItemText( nItem, P_EXT, theApp.m_recordsetBlob->GetFieldString( PIC_EXT) );
		m_ListCtrl.SetItemText( nItem, P_TITLE, theApp.m_recordsetBlob->GetFieldString( PIC_TITLE) );
		sizeBlob = _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_SIZE) );
		sizeTotal += sizeBlob;
		++nItem;
	}
	m_ListCtrl.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPictures::paintBlob( int nItem )
{
	CString rowidP;		// az ember rowid-ja
	CString command;
	CString title;
	CString comment;
	CString date;
	CString caption;
	int		primary;

	m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible( nItem, FALSE );
	theApp.m_recordsetBlob->MoveTo( nItem );

	rowidP		= theApp.m_recordsetBlob->GetFieldString( PIC_ID );
	date		= theApp.m_recordsetBlob->GetFieldString( PIC_DATE );
	comment		= theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT );
	m_ext		= theApp.m_recordsetBlob->GetFieldString( PIC_EXT );	
	primary		= _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_PRIMARY ) );
	title		= theApp.m_recordsetBlob->GetFieldString( PIC_TITLE );
	m_rowidB	= theApp.m_recordsetBlob->GetFieldString( PIC_ROWID );
		

	
//	caption.Format( L"%s %s *%s", theApp.m_recordset2->GetFieldString(0), theApp.m_recordset2->GetFieldString(1), theApp.m_recordset2->GetFieldString(2) );
//	SetWindowTextW( caption );

	InvalidateRect( NULL, true );
	m_PrimaryCtrl.SetCheck( primary );    ///????
	GetDlgItem( IDC_TITLE )->SetWindowTextW( title );

	m_modified = false;

	m_ext.MakeLower();
	if( m_ext == L"jpg" || m_ext == L"png" || m_ext == L"bmp" || m_ext == L"jpeg" || m_ext == L"gif" )
		GetDlgItem( IDC_VIEW )->SetWindowTextW( theApp.m_viewerName );
	else if( m_ext == L"pdf" )
		GetDlgItem( IDC_VIEW )->SetWindowTextW( L"Acrobat Reader" );
	else if( m_ext == L"doc" || m_ext == L"docx" || m_ext == L"xls" || m_ext == L"xlsx" ) 
		GetDlgItem( IDC_VIEW )->SetWindowTextW( L"Office" );

	if( m_rowidB.IsEmpty() ) 
	{
		CDialog::EndDialog( IDCANCEL );
		return false;
	}
	// blob fájlba írása, hogy az ábrázoláshoz fájlból tudjuk beolvasni (image.Load( m_fileSpec ); 
	block = theApp.blobDB->blobRead( "pictures", "picture", m_rowidB, &blob_size );
	if( block == NULL ) return false;
	if( !writeBlockToFile( block, blob_size ) ) return false;
	m_paint = true;
	Invalidate( false );

	free( block );
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnPaint()
{
	if( !m_paint ) return;
	
	if( m_ext != L"jpg" && m_ext != L"png" ) 
	{
		GetDlgItem( IDC_TITLE )->GetWindowTextW( str );
		str.Format( L"%s %s file!", (CString)str, m_ext ); 
		AfxMessageBox( str );
//		GetDlgItem( IDC_TITLE )->SetWindowTextW( str );
		CDialogEx::OnPaint();
		return;
	}

	Invalidate( FALSE );  // fölöslegesnek látszik

	CPaintDC dc(this); // device context for painting

	CDC dcMemory;
	CImage image;
	CBitmap bitmap;


	image.Load( m_fileSpec );
	bitmap.Attach(image.Detach());

	dcMemory.CreateCompatibleDC( &dc);

//	dcMemory.SelectStockObject(NULL_BRUSH);
//	if( bitmap.m_hObject != NULL) bitmap.DeleteObject();

    dcMemory.SelectObject(&bitmap);

	BITMAP bm;						// bitmap in memory
	bitmap.GetBitmap( &bm );

	int bmWidth		= bm.bmWidth;
	int bmHeight	= bm.bmHeight;
	float bmRatio	= (float)bm.bmWidth/(float)bmHeight;

	CRect rect;		// a dlg ablak méretei 0,0 pontból ( Windows koordinátákban )
//	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!
//	CRect rectE;	

	GetClientRect(&rect);
	
//	GetWindowRect( rectW );
//	GetDlgItem( IDC_SPIN )->GetWindowRect( rectE );


	int height	= int(rect.Height()*0.7);				// a transzformát kép magassága az ablak feléig!
	height	= int(rect.Height()*0.6);				// a transzformát kép magassága az ablak feléig!

	int width = (int) (height * bmRatio );		// a transzformált kép szélessége a bmRatio-val arányos

	int cW = (rect.right - rect.left)/2;  // ablak középpontjának x koordinátája
	int cB = width/2;						// a sarokban lévõ kép középpontjának x koordinátája
	int x = cW - cB;						// az ablak új sarka
	int	y = 10;

	dc.SetStretchBltMode( STRETCH_HALFTONE );
	dc.StretchBlt( x, y, width, height, &dcMemory, 0,0, bmWidth, bmHeight, SRCCOPY);
//	dc.BitBlt(0,0, bmWidth, bmHeight, &dcMemory, 0, 0, SRCCOPY);	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPictures::writeBlockToFile( void* block, int blob_size  )
{
	int		errno_t;
	CString str;

	m_fileSpec.Format( L"%s\\tmp.%s", theApp.m_workingDirectory, m_ext );

	if( !openFileSpec( &theApp.fl, m_fileSpec, L"wb" ) ) return false;

	if( fwrite( block, blob_size, 1, theApp.fl ) != 1 )
	{
		AfxMessageBox( L"fwrite failed!" );
		fclose( theApp.fl );
		return false;
	}
	fclose( theApp.fl );
	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;


	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int iPos = pNMUpDown->iPos;
	int iDelta = pNMUpDown->iDelta;

	if( iDelta == -1 )  // kisebb sorszámó képek felé megyünk
	{
		if( nItem == 0 )  // mér az elején vagyunk
			return;
//			nItem = theApp.m_recordsetBlob->RecordsCount();
		else
			--nItem;
	}
	else
	{
		if( nItem == theApp.m_recordsetBlob->RecordsCount() - 1 )  // már az utolsónál vagyunk
			return;
//			nItem = 0;
		else
			++nItem;
	}

	InvalidateRect( NULL, true );  // törli az ablakban lévõ korábbi képet!
	if( m_modified ) saveValues();

	m_paint = false;
	paintBlob(nItem);
	GetDlgItem( IDC_SPIN )->SetFocus();  // hogy ne legyen az IDC_TITLE-ban a kurzor!!
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::caption( int sorsz )
{
	str.Format( L"%s képei (%d/%d)", m_name, sorsz, theApp.m_recordsetBlob->RecordsCount() );
	SetWindowTextW( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
void CPictures::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnChangeTitle()
{
	m_modified = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnClickedPrimary()
{
	int primary = m_PrimaryCtrl.GetCheck();
	if( primary )		// törli az esetleges korábbi kijelöléseket 
	{
		saveValues();

	}
	m_modified = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::saveValues()
{
	CString title;
	GetDlgItem( IDC_TITLE )->GetWindowTextW( title );
	CString comment;
//	GetDlgItem( IDC_COMMENT )->GetWindowTextW( comment );
	
	int primary = m_PrimaryCtrl.GetCheck();

	if( primary )		// törli az esetleges korábbi kijelöléseket 
	{
		m_command.Format( L"UPDATE pictures SET primaryPic=0 WHERE id = '%s'", m_rowidP );
		theApp.executeBlob( m_command );
	}

	m_command.Format( L"UPDATE pictures SET title='%s', comment='%s', primaryPic = '%d' WHERE rowid = '%s'", title, comment, primary, m_rowidB );
	theApp.executeBlob( m_command );
//	m_command.Format( L"SELECT rowid, * FROM pictures WHERE table_id = '%s' AND id ='%s' ", m_table, m_rowidP );
	m_command.Format( L"SELECT rowid, * FROM pictures %s", m_filter );
	theApp.queryBlob( m_command );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnClickedView()
{
	CString title;

	if( m_ext == L"jpg" || m_ext == L"png" || m_ext == L"bmp" || m_ext == L"jpeg" || m_ext == L"gif" )
	{
		title	= theApp.m_recordsetBlob->GetFieldString( PIC_TITLE );
		m_command.Format(  L"/file=%s /bf /title=%s", m_fileSpec, title );
		ShellExecute(NULL,L"open", theApp.m_viewerName, m_command,theApp.m_viewerFolder,SW_SHOW);
	}
	else if( m_ext == L"pdf" )
	{
		ShellExecute(NULL,L"open", theApp.m_pdfName, m_fileSpec, theApp.m_pdfFolder, SW_SHOW);
	}
	else if( m_ext == L"doc" || m_ext == L"docx" || m_ext == L"xls" || m_ext == L"xlsx" ) 
		ShellExecute(NULL, L"open", m_fileSpec,NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CPictures::OnClickedDelete()
//{
//	CString str;
//	str.Format( L"Tényleg törölni akarod a dokumentumot?" ); 
//	if( ( AfxMessageBox( str, MB_YESNO ) ) == IDNO ) return;
//
//	str.Format( L"DELETE FROM pictures WHERE rowid='%s'", m_rowidB );
//	if( !theApp.executeBlob( str ) ) return;
//
//	m_command.Format( L"SELECT rowid, * FROM pictures WHERE table_id='%s' AND id ='%s' ", m_table, m_rowidP );
//	m_command.Format( L"SELECT rowid, * FROM pictures %s", m_filter );
//	if( !theApp.queryBlob( m_command ) ) return;
//	if( !theApp.m_recordsetBlob->RecordsCount() )
//	{
//		InvalidateRect( NULL, true );
//	}
//
//	m_SpinCtrl.SetRange( 1, theApp.m_recordsetBlob->RecordsCount() );
//
//	theApp.m_recordsetBlob->MoveFirst();
//	m_cnt= 1;
//	caption( m_cnt );
//	m_paint = false;
//	paintBlob(0);
//	GetDlgItem( IDC_SPIN )->SetFocus();  // hogy ne legyen az IDC_TITLE-ban a cursor és a kiválasztás
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnSysCommand(UINT nID, LPARAM lParam)
{

	if( nID == SC_CLOSE ) saveValues();

	CDialogEx::OnSysCommand(nID, lParam);
}
void CPictures::OnBnClickedOk()
{
	saveValues();
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnRdblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	paintBlob( nItem );

	*pResult = 0;
}


void CPictures::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	paintBlob( nItem );
	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::displayPicture()
{

	m_command.Format( L"SELECT rowid FROM pictures WHERE id='%s' AND table_id=%d AND primaryPic=1", m_rowidP, PEOPLEX );
	if( !theApp.queryBlob( m_command ) ) return;

	InvalidateRect( NULL, true );
	m_paint = false;
	CString rowid = theApp.m_recordsetBlob->GetFieldString( 0 );
	if( !rowid.IsEmpty() )
	{
		_int64 blob_size;
		void* block = theApp.blobDB->blobRead( "pictures", "picture", rowid, &blob_size );
		if( block == NULL ) return;
		if( !writeBlockToFile( block, blob_size ) ) return;
		m_paint = true;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CPictures::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
	{
		pPopup = Menu.GetSubMenu(0);
		pPopup->EnableMenuItem(ID_EDIT_UPDATE, MF_BYCOMMAND | MF_GRAYED);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
		}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnEditUpdate()
{
	EditUpdate( false );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::EditUpdate( bool all )
{
	CString rowidB(L"");
	if( !all )
	{
		int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if( nItem == -1 ) return;
		rowidB = m_ListCtrl.GetItemText( nItem, P_ROWID );
	}

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLEX, m_rowidP ); 
	dlg.m_name		= m_name;	// az ember neve
	dlg.m_rowidP	= m_rowidP;	// ember rowid-ja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  Az ember összes képeit a rowidB-vel kezdve mutassa be

	dlg.DoModal();
	fillBlobTable();
	displayPicture();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnEditDelete()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid;
	CString title;
	rowid = m_ListCtrl.GetItemText( nItem, P_ROWID );
	title = m_ListCtrl.GetItemText( nItem, P_TITLE );

	str.Format( L"Tényleg törölni akarod a '%s' fájlt?", title );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM pictures WHERE rowid='%s'", rowid );
	theApp.executeBlob( m_command );
	m_ListCtrl.DeleteItem( nItem );

//	if( !m_ListCtrl.GetItemCount() ) 
//	{
//		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );
//		return;
//	}


	m_command.Format( L"SELECT rowid, * FROM pictures %s", m_filter );
	if( !theApp.queryBlob( m_command ) ) return;
	if( !theApp.m_recordsetBlob->RecordsCount() )
	{
		InvalidateRect( NULL, true );
	}

	m_SpinCtrl.SetRange( 1, theApp.m_recordsetBlob->RecordsCount() );

	theApp.m_recordsetBlob->MoveFirst();
	m_cnt= 1;
	caption( m_cnt );
	m_paint = false;
	paintBlob(0);
	GetDlgItem( IDC_SPIN )->SetFocus();  // hogy ne legyen az IDC_TITLE-ban a cursor és a kiválasztás

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPictures::OnEditInsert()
{
	if( !theApp.selectFiles() ) return;

	BLOBSTAT stat;
	int rowid;
	
	int sorsz = m_ListCtrl.GetItemCount() + 1;
	int	nItem = m_ListCtrl.GetItemCount();

	theApp.executeBlob( L"BEGIN" );
	for( UINT i = 0; i < theApp.vBlobs.size(); ++i )
	{
		stat.ext = theApp.vBlobs.at(i).ext;
		stat.filename	= theApp.vBlobs.at(i).filename;
		stat.fileSpec	= theApp.vBlobs.at(i).fileSpec;
		stat.size		= theApp.vBlobs.at(i).size;

		rowid = theApp.blobDB->blobInsert( "pictures", "picture", &stat );
		if( !rowid ) return;
	
		m_command.Format( L"UPDATE pictures SET id='%s', table_id='%d', title='%s', filename= '%s', ext='%s', size='%d' WHERE rowid='%d'", m_rowidP, PEOPLEX, stat.filename, stat.filename, stat.ext, stat.size, rowid );
		if( !theApp.executeBlob( m_command ) ) return;
/*
		str.Format( L"%d", rowid );
		nItem = m_ListCtrl.InsertItem( nItem, str );

//		str.Format( L"%d", sorsz );
//		m_ListCtrl.SetItemText( nItem, P_CNT, str );

		m_ListCtrl.SetItemText( nItem, P_EXT, theApp.vBlobs.at(i).ext ) ;
		m_ListCtrl.SetItemText( nItem, P_TITLE, theApp.vBlobs.at(i).filename );
		++sorsz;
*/
	}
	theApp.executeBlob( L"COMMIT" );

	fillBlobTable();
	theApp.m_recordsetBlob->MoveFirst();

	m_cnt = 1;
	caption( m_cnt );
	paintBlob( 0 );
}
