// EditTable.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "EditTable.h"
#include "afxdialogex.h"
#include "SqliteDB.h"
#include "Pictures.h"

enum
{
	B_ROWID = 0,
	B_CNT,
	B_TITLE, 
	B_DATE,
	B_EXT,
	B_COMMENT,
};


// CEditTable dialog

IMPLEMENT_DYNAMIC(CEditTable, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditTable::CEditTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditTable::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditTable::~CEditTable()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROMAN, m_comboRoman);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_JOINT, m_ComboJoint);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_PHOTOS, colorPhotos);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditTable, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_AUXNAME, &CEditTable::OnKillfocusEditAuxname)
	ON_EN_KILLFOCUS(IDC_EDIT_COMMENT, &CEditTable::OnKillfocusEditComment)
	ON_EN_KILLFOCUS(IDC_EDIT_FAMILYNAME, &CEditTable::OnKillfocusEditFamilyname)
	ON_EN_KILLFOCUS(IDC_EDIT_ROOT, &CEditTable::OnKillfocusEditRoot)
//	ON_EN_KILLFOCUS(IDC_EDIT_OTHERNAME, &CEditTable::OnKillfocusEditOthername)
	ON_BN_CLICKED(IDOK, &CEditTable::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIAS, &CEditTable::OnKillfocusEditAlias)
	ON_EN_KILLFOCUS(IDC_EDIT_AG, &CEditTable::OnKillfocusEditAg)
	ON_CBN_SELCHANGE(IDC_COMBO_ROMAN, &CEditTable::OnSelchangeComboRoman)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CEditTable::OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_JOINT, &CEditTable::OnSelchangeComboJoint)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CEditTable::OnDblclkList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_BN_CLICKED(ID_EDIT_UPDATE, &CEditTable::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CEditTable::OnEditDelete )
	ON_BN_CLICKED(ID_EDIT_INSERT, &CEditTable::OnEditInsert )

	ON_STN_CLICKED(IDC_PHOTOS, &CEditTable::OnClickedPhotos)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	  SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetWindowTextW( m_caption );

	m_comboType.AddString( L"%%" );
	m_comboType.AddString( L"%%%" );
	m_comboType.SetCurSel( 1 );

	romanA.Add( L"" );
	romanA.Add( L"I" );
	romanA.Add( L"II" );
	romanA.Add( L"III" );
	romanA.Add( L"IV" );
	romanA.Add( L"V" );
	romanA.Add( L"VI" );
	romanA.Add( L"VII" );
	romanA.Add( L"VIII" );
	romanA.Add( L"IX" );
	romanA.Add( L"X" );
	romanA.Add( L"XI" );
	romanA.Add( L"XII" );
	romanA.Add( L"XIII" );
	romanA.Add( L"XIV" );
	romanA.Add( L"XV" );
	romanA.Add( L"XVI" );

	for( int i = 0; i < romanA.GetCount(); ++i )
		m_comboRoman.AddString( romanA[i] );
	m_comboRoman.SetCurSel( 0 );

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( B_ROWID,			L"rowid",		LVCFMT_RIGHT,	40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_CNT,			L"",			LVCFMT_RIGHT,	30,-1,COL_NUM );
	m_ListCtrl.InsertColumn( B_TITLE,			L"cím",			LVCFMT_LEFT,	100,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_DATE,			L"dátum",		LVCFMT_LEFT,	80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( B_EXT,			L"ext",			LVCFMT_LEFT,	30,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( B_COMMENT,		L"leírás",	LVCFMT_LEFT,	500,-1,COL_TEXT );


	if( !m_tableNumber.IsEmpty() )
		colorP();
//		createScreen();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::colorP()
{
	m_command.Format( L"SELECT rowid, * FROM tables WHERE rowid='%s'", m_tableNumber );
	if( !theApp.query( m_command ) ) return;

	m_type			= theApp.m_recordset->GetFieldString( DBT_PERCENT );
	m_familyName	= theApp.m_recordset->GetFieldString( DBT_FAMILY_NAME );
	m_alias			= theApp.m_recordset->GetFieldString( DBT_ALIAS );
	m_titolo		= theApp.m_recordset->GetFieldString( DBT_TITOLO );
	m_roman			= theApp.m_recordset->GetFieldString( DBT_TABLENUMBERROMAN );
	m_comment		= theApp.m_recordset->GetFieldString( DBT_COMMENT );
	m_root			= theApp.m_recordset->GetFieldString( DBT_TORZS );
	m_ag			= theApp.m_recordset->GetFieldString( DBT_ARM );
	m_joint			= theApp.m_recordset->GetFieldString( DBT_JOINT );    // 1: kapcsolódik, 0: nem kapcsolódik
	m_tableHeader	= theApp.m_recordset->GetFieldString( DBT_TABLEHEADER );



	GetDlgItem( IDC_TABLEHEADER_IN )->SetWindowTextW( m_tableHeader );
	GetDlgItem( IDC_EDIT_FAMILYNAME )->SetWindowTextW( m_familyName );
	GetDlgItem( IDC_EDIT_ALIAS )->SetWindowTextW( m_alias );	
	GetDlgItem( IDC_EDIT_AUXNAME )->SetWindowTextW( m_titolo );
	GetDlgItem( IDC_EDIT_AG )->SetWindowTextW( m_ag );
	GetDlgItem( IDC_EDIT_ROOT )->SetWindowTextW( m_root );
	GetDlgItem( IDC_EDIT_COMMENT )->SetWindowTextW( m_comment );	

	if( m_type == L"%%" )
		m_comboType.SetCurSel( 0 );
	else
		m_comboType.SetCurSel( 1 );

	int i;
	for( i = 0; i < romanA.GetSize(); ++i )
	{
		if( !m_roman.Compare( romanA[i] ) )
		{
			break;
		}
	}
	if( i == romanA.GetSize() ) i = 0;
	m_comboRoman.SetCurSel( i );

	m_ComboJoint.AddString( L"nem");			// 
	m_ComboJoint.AddString( L"" );
	m_ComboJoint.SetCurSel( _wtoi( m_joint ));

	colorPhotos.SetTextColor( theApp.m_colorClick );

	fillBlobTable();


	createTableHeader();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::fillBlobTable()
{
	m_command.Format( L"SELECT rowid,* FROM pictures WHERE table_id='%d' AND id='%s'", TABLE, m_tableNumber );
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEditTable::OnSelchangeComboType()
{
	m_comboType.GetLBText( m_comboType.GetCurSel(), m_type );
	createTableHeader();
}
void CEditTable::OnSelchangeComboJoint()
{
	m_joint.Format( L"%d", m_ComboJoint.GetCurSel() );
//	m_ComboJoint.GetLBText( m_comboType.GetCurSel(), m_joint );
	createTableHeader();
}
void CEditTable::OnSelchangeComboRoman()
{
	m_comboRoman.GetLBText( m_comboRoman.GetCurSel(), m_roman );
	createTableHeader();
}
void CEditTable::OnKillfocusEditAuxname()
{
	GetDlgItem( IDC_EDIT_AUXNAME )->GetWindowTextW( m_titolo );
	createTableHeader();
}
void CEditTable::OnKillfocusEditFamilyname()
{
	GetDlgItem( IDC_EDIT_FAMILYNAME )->GetWindowTextW( m_familyName );
	createTableHeader();
}
void CEditTable::OnKillfocusEditRoot()
{
	GetDlgItem( IDC_EDIT_ROOT )->GetWindowTextW( m_root );
	createTableHeader();
}
//void CEditTable::OnKillfocusEditOthername()
//{
//	GetDlgItem( IDC_EDIT_OTHERNAME )->GetWindowTextW( m_otherName );
//	createTableHeader();
//}
void CEditTable::OnKillfocusEditAlias()
{
	GetDlgItem( IDC_EDIT_ALIAS )->GetWindowTextW( m_alias );
	createTableHeader();
}
void CEditTable::OnKillfocusEditAg()
{
	GetDlgItem( IDC_EDIT_AG )->GetWindowTextW( m_ag );
	createTableHeader();
}
void CEditTable::OnKillfocusEditComment()
{
	GetDlgItem( IDC_EDIT_COMMENT )->GetWindowTextW( m_comment );
	createTableHeader();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditTable::PreTranslateMessage(MSG* pMsg)
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
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::createTableHeader()
{
	m_tableHeader2.Format( L"%s %s", m_type, m_familyName );

	if( !m_alias.IsEmpty() )
	{
		m_tableHeader2 += L" alias ";
		m_tableHeader2 += m_alias;
	}

	if( !m_titolo.IsEmpty() )
	{
		m_tableHeader2 += L", ";
		m_tableHeader2 += m_titolo;
	}

	if( !m_roman.IsEmpty() )
	{
		m_tableHeader2 += L" ";
		m_tableHeader2 += m_roman;
	}

	if( !m_ag.IsEmpty() )
	{
		m_tableHeader2 += L" [";
		m_tableHeader2 += m_ag;
		m_tableHeader2 += L"]";
	}

	if( !m_root.IsEmpty() )
	{
		m_tableHeader2 += L" [törzs: ";
		m_tableHeader2 += m_root;
		m_tableHeader2 += L"]";
	}

	if( !m_comment.IsEmpty() )
	{
		m_tableHeader2 += L" ";
		m_tableHeader2 += m_comment;
	}

	if( m_joint == L"0" ) 
	{
		m_tableHeader2 += L" nem kapcs";
	}


	GetDlgItem( IDC_TABLEHEADER_OUT )->SetWindowTextW( m_tableHeader2 );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::OnBnClickedOk()
{

	m_comboType.GetLBText( m_comboType.GetCurSel(), m_type );
	m_comboRoman.GetLBText( m_comboRoman.GetCurSel(), m_roman );

	m_command = L"SELECT max(familyNumber) FROM tables";
	if( !theApp.query( m_command ) ) return;
	
	str = theApp.m_recordset->GetFieldString( 0 );

	if( m_type == L"%%%" )
		m_familyNumber = _wtoi( theApp.m_recordset->GetFieldString( 0 ) ) + 1; 
	else
		m_familyNumber = _wtoi( theApp.m_recordset->GetFieldString( 0 ) );
	

	CString tableHeader;
	tableHeader.Format( L"%s %s", m_type, m_familyName );
	if( !m_titolo.IsEmpty() )
	{
		tableHeader += L", ";
		tableHeader += m_titolo;
	}
	if( !m_roman.IsEmpty() )
	{
		tableHeader += L" ";
		tableHeader += m_roman;
	}
	if( !m_ag.IsEmpty() )
	{
		tableHeader += "[";
		tableHeader += m_ag;
		tableHeader += "]";
	}

	if( !m_comment.IsEmpty() )
	{
		tableHeader += L" ";
		tableHeader += m_comment;
	}
	if( !m_root.IsEmpty() )
	{
		tableHeader += "[";
		tableHeader += m_root;
		tableHeader += " törzs]";
	}


	CString fields = L"familyNumber, tableHeader, percent, familyName, titolo, tableRoman, comment, torzs, alias, arm, joint";
	CString values;
	values.Format( L"'%d', '%s', '%s','%s','%s','%s','%s','%s','%s','%s','%s'", m_familyNumber, tableHeader, m_type, m_familyName, m_titolo, m_roman, m_comment, m_root, m_alias, m_ag, m_joint ); 
	

	if( m_tableNumber.IsEmpty() )
	{
		m_command.Format( L"INSERT INTO tables ( %s ) VALUES ( %s )", fields, values );
		if( !theApp.execute( m_command ) ) return;
		m_command = L"SELECT last_insert_rowid() FROM tables";
		if( !theApp.query( m_command ) ) return;
		m_tableNumber = theApp.m_recordset->GetFieldString( 0 );
	}
	else
	{
		m_command.Format( L"UPDATE tables SET tableHeader='%s', percent='%s',familyName='%s',titolo='%s',tableRoman='%s',comment='%s', torzs='%s', alias='%s',arm='%s',joint='%s' WHERE rowid='%s'",\
		tableHeader,\
		m_type,\
		m_familyName,\
		m_titolo,\
		m_roman,\
		m_comment,\
		m_root,\
		m_alias,\
		m_ag,\
		m_joint,\
		m_tableNumber );
		if( !theApp.execute( m_command ) ) return;
	}
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CEditTable::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
	CMenu	Menu;
	CMenu*	pPopup;

	if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
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
void CEditTable::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	EditUpdate( false );
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::OnClickedPhotos()
{
	EditUpdate( true );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::OnEditUpdate()
{
	EditUpdate( false );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::EditUpdate( bool all )
{
	CString rowidB(L"");
	if( !all )
	{
		int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if( nItem == -1 ) return;
		rowidB = m_ListCtrl.GetItemText( nItem, B_ROWID );
	}

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", TABLE, m_tableNumber ); 
	str.Format( L"%s tábla", m_familyName );
	dlg.m_name		=str;					// a tábla neve
	dlg.m_rowidP	= m_tableNumber;		// a tábla rowi
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  Az ember összes képeit a rowidB-vel kezdve mutassa be

	dlg.DoModal();
	fillBlobTable();
	if( m_ListCtrl.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
	else
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditTable::OnEditDelete()
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
void CEditTable::OnEditInsert()
{
	if( !theApp.selectFiles() ) return;

	BLOBSTAT stat;
	int rowid;
	
	int sorsz = m_ListCtrl.GetItemCount() + 1;
	int	nItem = m_ListCtrl.GetItemCount();

	for( UINT i = 0; i < theApp.vBlobs.size(); ++i )
	{
		stat.ext = theApp.vBlobs.at(i).ext;
		stat.filename	= theApp.vBlobs.at(i).filename;
		stat.fileSpec	= theApp.vBlobs.at(i).fileSpec;
		stat.size		= theApp.vBlobs.at(i).size;

		rowid = theApp.blobDB->blobInsert( "pictures", "picture", &stat );
		if( !rowid ) return;
	
		m_command.Format( L"UPDATE pictures SET id='%s', table_id='%d', title='%s', filename= '%s', ext='%s', size='%d' WHERE rowid='%d'", m_tableNumber, TABLE, stat.filename, stat.filename, stat.ext, stat.size, rowid );
		if( !theApp.executeBlob( m_command ) ) return;

		str.Format( L"%d", rowid );
		nItem = m_ListCtrl.InsertItem( nItem, str );

		str.Format( L"%d", sorsz );
		m_ListCtrl.SetItemText( nItem, B_CNT, str );

		m_ListCtrl.SetItemText( nItem, B_EXT, theApp.vBlobs.at(i).ext ) ;
		m_ListCtrl.SetItemText( nItem, B_TITLE, theApp.vBlobs.at(i).filename );
		++sorsz;
	}
	theApp.showItem( nItem, &m_ListCtrl );
	if( m_ListCtrl.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
}


