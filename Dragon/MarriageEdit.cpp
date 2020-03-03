// MarriageEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "MarriageEdit.h"
#include "afxdialogex.h"
#include "Pictures.h"

// CMarriageEdit dialog


enum
{
	B_ROWID = 0,
	B_CNT,
	B_TITLE, 
	B_DATE,
	B_EXT,
	B_COMMENT,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMarriageEdit, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarriageEdit::CMarriageEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMarriageEdit::IDD, pParent)
	, m_husband(_T(""))
	, m_wife(_T(""))
	, m_birthH(_T(""))
	, m_birthW(_T(""))
	, m_cntH(_T(""))
	, m_cntW(_T(""))
	, m_place(_T(""))
	, m_date(_T(""))
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarriageEdit::~CMarriageEdit()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_HUSBAND, m_husband);
	DDX_Text(pDX, IDC_WIFE, m_wife);
	DDX_Text(pDX, IDC_BIRTHH, m_birthH);
	DDX_Text(pDX, IDC_BIRTHW, m_birthW);
	DDX_Text(pDX, IDC_CNT_H, m_cntH);
	DDX_Text(pDX, IDC_CNT_W, m_cntW);
	DDX_Control(pDX, IDC_CNT_H, colorCntH);
	DDX_Control(pDX, IDC_CNT_W, colorCntW);
	DDX_Text(pDX, IDC_PLACE, m_place);
	DDX_Text(pDX, IDC_DATE, m_date);
	DDX_Control(pDX, IDC_PHOTOS, colorPhotos);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMarriageEdit, CDialogEx)
ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CMarriageEdit::OnDblclkList)
ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
ON_BN_CLICKED(ID_EDIT_UPDATE, &CMarriageEdit::OnEditUpdate )
ON_BN_CLICKED(ID_EDIT_DELETE, &CMarriageEdit::OnEditDelete )
ON_BN_CLICKED(ID_EDIT_INSERT, &CMarriageEdit::OnEditInsert )
ON_STN_CLICKED(IDC_PHOTOS, &CMarriageEdit::OnClickedPhotos)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMarriageEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( B_ROWID,			L"rowid",		LVCFMT_RIGHT,	40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_CNT,			L"",			LVCFMT_RIGHT,	30,-1,COL_NUM );
	m_ListCtrl.InsertColumn( B_TITLE,			L"cím",			LVCFMT_LEFT,	100,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_DATE,			L"dátum",		LVCFMT_LEFT,	80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_EXT,			L"ext",			LVCFMT_LEFT,	30,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_COMMENT,		L"megjegyzés",	LVCFMT_LEFT,	500,-1,COL_TEXT );

	colorPhotos.SetTextColor(theApp.m_colorClick );
	fillScreen();
	fillBlobTable();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::fillScreen()
{
	m_command.Format( L"SELECT rowid,* FROM marriages WHERE rowid ='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;
	CString rowidH;
	CString rowidW;
	CString birth(L"");
	CString death(L"");
	CString birthDate;

	int		birthI;
	int		dateI;
	int age;

	rowidH = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
	rowidW = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );


	m_place = theApp.m_recordset->GetFieldString( MARRIAGES_PLACE );
	m_date = theApp.m_recordset->GetFieldString( MARRIAGES_DATE );
	dateI	= theApp.getYearFromDate( m_date );


	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowidH );
	if( !theApp.query1( m_command ) ) return;
	m_husband.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME ), theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME ) );

	birthDate = theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_DATE );
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_PLACE ), birthDate );
	str.Trim();
	if( !str.IsEmpty() ) birth.Format( L"*%s", str );
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_PLACE ), theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) death.Format( L"+%s", str );

	m_birthH.Format( L"%s %s", birth, death );
	m_birthH.Trim();


	birthI = theApp.getYearFromDate( birthDate );
	if( !m_date.IsEmpty()  && !birthDate.IsEmpty() )
	{
		age =  dateI - birthI;
		m_cntH.Format( L"%s. házassága %d éves korában", theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE ), age );
	}
	else
		m_cntH.Format( L"%s. házassága", theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE ) );




	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'", rowidW );
	if( !theApp.query1( m_command ) ) return;
	m_wife.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME ), theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME ) );
	
	birthDate = theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_DATE );
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_PLACE ), birthDate );
	str.Trim();
	if( !str.IsEmpty() ) birth.Format( L"*%s", str );
	
	str.Format( L"%s %s", theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_PLACE ), theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_DATE ) );
	str.Trim();
	if( !str.IsEmpty() ) death.Format( L"+%s", str );

	m_birthW.Format( L"%s %s", birth, death );
	m_birthW.Trim();

	birthI = theApp.getYearFromDate( birthDate );
	if( !m_date.IsEmpty()  && !birthDate.IsEmpty() )
	{
		age =  dateI - birthI;
		m_cntW.Format( L"%s. házassága %d éves korában", theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND ), age );
	}
	else
		m_cntW.Format( L"%s. házassága", theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND ) );

	colorCntH.SetTextColor( theApp.m_colorClick );
	colorCntW.SetTextColor( theApp.m_colorClick );
	UpdateData( TOSCREEN );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::fillBlobTable()
{
	m_command.Format( L"SELECT rowid,* FROM pictures WHERE table_id='%d' AND id='%s'", MARRIAGE, m_rowid );
	if( !theApp.queryBlob( m_command ) ) return;

	u_int64 sizeTotal = 0;
	u_int64	sizeBlob;
	m_ListCtrl.DeleteAllItems();
	int nItem = 0;
	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i, theApp.m_recordsetBlob->MoveNext() )
	{
		str.Format( L"%d", i+1 );
		nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordsetBlob->GetFieldString( PIC_ROWID) );
		m_ListCtrl.SetItemText( nItem, B_CNT, str );
		m_ListCtrl.SetItemText( nItem, B_DATE, theApp.m_recordsetBlob->GetFieldString( PIC_DATE) );
		m_ListCtrl.SetItemText( nItem, B_EXT, theApp.m_recordsetBlob->GetFieldString( PIC_EXT) );
		m_ListCtrl.SetItemText( nItem, B_TITLE, theApp.m_recordsetBlob->GetFieldString( PIC_FILENAME) );
		m_ListCtrl.SetItemText( nItem, B_COMMENT, theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT) );
		sizeBlob = _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_SIZE) );
		sizeTotal += sizeBlob;
	}
	m_ListCtrl.SetSortOrder( 1, 2 );

	if( m_ListCtrl.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
	else
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMarriageEdit::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMarriageEdit::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	EditUpdate( false );
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::OnClickedPhotos()
{
	EditUpdate( true );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::OnEditUpdate()
{
	EditUpdate( false );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::EditUpdate( bool all )
{
	CString rowidB(L"");

	if( !all )
	{
		int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if( nItem == -1 ) return;
		rowidB = m_ListCtrl.GetItemText( nItem, B_ROWID );
	}

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id = '%d' AND id='%s'", MARRIAGE, m_rowid );
	dlg.m_name		= L"Házasság";	// a házasság neve
	dlg.m_rowidP	= m_rowid;	// a házasság rowid-ja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  A házasság összes képeit a rowidB-vel kezdve mutassa be
	dlg.DoModal();

	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::OnEditDelete()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid;
	CString title;
	rowid = m_ListCtrl.GetItemText( nItem, B_ROWID );
	title = m_ListCtrl.GetItemText( nItem, B_TITLE );

	str.Format( L"Tényleg törölni akarod a '%s' fájlt?", title );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM pictures WHERE rowid='%s'", rowid );
	theApp.executeBlob( m_command );
	m_ListCtrl.DeleteItem( nItem );

	if( !m_ListCtrl.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMarriageEdit::OnEditInsert()
{
	if( !theApp.selectFiles() ) return;

	BLOBSTAT stat;
	int rowid;
	UINT i;

	int sorsz = m_ListCtrl.GetItemCount() + 1;
	int	nItem = m_ListCtrl.GetItemCount();

	for( i = 0; i < theApp.vBlobs.size(); ++i )
	{
		stat.ext = theApp.vBlobs.at(i).ext;
		stat.filename	= theApp.vBlobs.at(i).filename;
		stat.fileSpec	= theApp.vBlobs.at(i).fileSpec;
		stat.size		= theApp.vBlobs.at(i).size;

		rowid = theApp.blobDB->blobInsert( "pictures", "picture", &stat );
		if( !rowid ) return;
	
		m_command.Format( L"UPDATE pictures SET id='%s', table_id='%d', title='%s', filename= '%s', ext='%s', size='%d' WHERE rowid='%d'", m_rowid, MARRIAGE, stat.filename, stat.filename, stat.ext, stat.size, rowid );
		if( !theApp.executeBlob( m_command ) ) return;

		str.Format( L"%d", rowid );
		nItem = m_ListCtrl.InsertItem( nItem, str );

		str.Format( L"%d", sorsz );
		m_ListCtrl.SetItemText( nItem, B_CNT, str );

		m_ListCtrl.SetItemText( nItem, B_EXT, theApp.vBlobs.at(i).ext ) ;
		m_ListCtrl.SetItemText( nItem, B_TITLE, theApp.vBlobs.at(i).filename );
		++sorsz;
	}
	m_ListCtrl.EnsureVisible( nItem, FALSE );
	
	GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
