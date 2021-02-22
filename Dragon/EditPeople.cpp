// Input.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "utilities.h"
#include "EditPeople.h"
#include "afxdialogex.h"
#include "Table_tables.h"
#include "EditTable.h"
#include "SelectMother.h"
#include "GetLastFirst.h"
#include "GetPeople.h"
#include "GetMarriage.h"
#include "Pictures.h"


enum
{
	L_ROWID = 0,
	L_ROWIDS, 
	L_ORDERH,
	L_PLACE,
	L_DATE,
	L_SPOUSE,
	L_ORDERW,
};

enum
{
	B_ROWID = 0,
	B_CNT,
	B_TITLE, 
	B_DATE,
	B_EXT,
	B_COMMENT,
};


IMPLEMENT_DYNAMIC(CEditPeople, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditPeople::CEditPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditPeople::IDD, pParent)
	, m_table(_T(""))
	, m_titolo(_T(""))
	, m_last_name(_T(""))
	, m_first_name(_T(""))
	, m_comment(_T(""))
	, m_csalad(_T(""))
	, m_death_date(_T(""))
	, m_death_place(_T(""))
	, m_birth_date(_T(""))
	, m_generation(_T(""))
	, m_folyt(_T(""))
	, m_nameChanged(_T(""))
	, m_rowid(_T(""))
	, m_birth_place(_T(""))
	, m_mother(_T(""))
	, m_father(_T(""))
	, m_csalad_ose(_T(""))
	, m_tableAncestry(FALSE)
	, m_title(_T(""))
	, m_posterior(_T(""))
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEditPeople::~CEditPeople()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TABLE, colorTable);
	DDX_Control(pDX, IDC_STATIC_FATHER, colorFather);
	DDX_Control(pDX, IDC_STATIC_MOTHER, colorMother);
	DDX_Control(pDX, IDC_COMBO_SEX, m_ComboSex);
	DDX_Control(pDX, IDC_COMBO_HIERARCHY, m_ComboRole);
	DDX_Text(pDX, IDC_TITOLO, m_titolo);
	DDX_Text(pDX, IDC_LAST_NAME, m_last_name);
	DDX_Text(pDX, IDC_FIRST_NAME, m_first_name);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_CSALAD, m_csalad);
	DDX_Text(pDX, IDC_DEATH_DATE, m_death_date);
	DDX_Text(pDX, IDC_DEATH_PLACE, m_death_place);
	DDX_Text(pDX, IDC_BIRTH_DATE, m_birth_date);
	DDX_Text(pDX, IDC_GENERATION, m_generation);
	DDX_Text(pDX, IDC_FOLYT, m_folyt);
	DDX_Text(pDX, IDC_NAMECHANGED, m_nameChanged);
	DDX_Text(pDX, IDC_TABLE, m_table);
	DDX_Text(pDX, IDC_ROWID, m_rowid);
	DDX_Text(pDX, IDC_BIRTH_PLACE, m_birth_place);
	DDX_Text(pDX, IDC_MOTHER, m_mother);
	DDX_Text(pDX, IDC_FATHER, m_father);
	DDX_Text(pDX, IDC_CSALAD_OSE, m_csalad_ose);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);

	DDX_Control(pDX, IDC_DELETE_FATHER, colorDeleteFather);
	DDX_Control(pDX, IDC_DELETE_MOTHER, colorDeleteMother);
	DDX_Check(pDX, IDC_TABLE_ANCESTRY, m_tableAncestry);
	DDX_Control(pDX, IDC_BLOBS, m_ListCtrlB);
	DDX_Control(pDX, IDC_PHOTOS, colorPhotos);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	DDX_Text(pDX, IDC_EDIT_POSTERIOR, m_posterior);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEditPeople, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_TABLE, &CEditPeople::OnClickedStaticTable)
	ON_BN_CLICKED(IDOK, &CEditPeople::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_FATHER, &CEditPeople::OnClickedStaticFather)
	ON_STN_CLICKED(IDC_STATIC_MOTHER, &CEditPeople::OnClickedStaticMother)
	ON_EN_KILLFOCUS(IDC_FIRST_NAME, &CEditPeople::OnKillfocusEditFirstname)
	ON_EN_KILLFOCUS(IDC_BIRTH_DATE, &CEditPeople::OnKillfocusEditBirthd)
	ON_EN_KILLFOCUS(IDC_DEATH_DATE, &CEditPeople::OnKillfocusEditDeathd)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_STN_CLICKED(IDC_DELETE_FATHER, &CEditPeople::OnClickedDeleteFather)
	ON_STN_CLICKED(IDC_DELETE_MOTHER, &CEditPeople::OnClickedDeleteMother)
	ON_CBN_KILLFOCUS(IDC_COMBO_SEX, &CEditPeople::OnKillfocusComboSex)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CEditPeople::OnDblclkList)
	ON_EN_KILLFOCUS(IDC_LAST_NAME, &CEditPeople::OnKillfocusLastName)
	ON_BN_CLICKED(IDC_TABLE_ANCESTRY, &CEditPeople::OnClickedTableAncestry)

	ON_NOTIFY(NM_DBLCLK, IDC_BLOBS, &CEditPeople::OnDblclkBlobs)

	ON_BN_CLICKED(ID_EDIT_UPDATE, &CEditPeople::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_DELETE, &CEditPeople::OnEditDelete )
	ON_BN_CLICKED(ID_EDIT_INSERT, &CEditPeople::OnEditInsert )

	ON_BN_CLICKED(ID_EDIT_UPDATEM, &CEditPeople::OnEditUpdateM )
	ON_BN_CLICKED(ID_EDIT_DELETEM, &CEditPeople::OnEditDeleteM )
	ON_BN_CLICKED(ID_EDIT_INSERTM, &CEditPeople::OnEditInsertM )

	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PHOTOS, &CEditPeople::OnClickedPhotos)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CEditPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetWindowTextW( m_caption );

	CString birth;
	CString death;


	colorTable.SetTextColor( theApp.m_colorClick );
	colorFather.SetTextColor( theApp.m_colorClick );
	colorMother.SetTextColor( theApp.m_colorClick );
	colorDeleteFather.SetTextColor( theApp.m_colorClick );
	colorDeleteMother.SetTextColor( theApp.m_colorClick );
	colorPhotos.SetTextColor( theApp.m_colorClick );


	m_ComboSex.AddString( L"" );
	m_ComboSex.AddString( L"férfi" );
	m_ComboSex.AddString( L"nõ" );
	m_ComboSex.SetCurSel( 0 );

	m_ComboRole.AddString( L"leszármazott" );
	m_ComboRole.AddString( L"leszármazott házastársa" );
	m_ComboRole.AddString( L"leszármazott házastársának apja" );
	m_ComboRole.AddString( L"leszármazott házastársának anyja" );
	m_ComboRole.AddString( L"leszármazott házastársának további házastársa" );
	m_ComboRole.SetCurSel( 0 );

	m_orderix = 1;

	initialScreen();

	if( theApp.m_inputMode == GAHTML )
	{
		GetDlgItem( IDC_TABLE_ANCESTRY )->EnableWindow( false ); 
		GetDlgItem( IDC_COMBO_HIERARCHY )->EnableWindow( false ); 
	}


	displayPicture();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::displayPicture()
{

	m_command.Format( L"SELECT rowid FROM pictures WHERE id='%s' AND table_id=%d AND primaryPic=1", m_rowid, PEOPLEX );
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::initialScreen()
{
	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", m_rowid );
	if( !theApp.query( m_command ) ) return;

	m_tableNumber	= theApp.m_recordset->GetFieldString( PEOPLE_TABLENUMBER );
	m_generation	= theApp.m_recordset->GetFieldString( PEOPLE_GENERATION );
	m_title			= theApp.m_recordset->GetFieldString( PEOPLE_TITLE );
	m_titolo		= theApp.m_recordset->GetFieldString( PEOPLE_TITOLO);
	m_last_name		= theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME );
	m_first_name	= theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME );
	m_posterior		= theApp.m_recordset->GetFieldString( PEOPLE_POSTERIOR );
	m_birth_place	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
	m_birth_date	= theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	m_death_place	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
	m_death_date	= theApp.m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	m_comment		= theApp.m_recordset->GetFieldString( PEOPLE_COMMENT );
	m_occupation	= theApp.m_recordset->GetFieldString( PEOPLE_OCCUPATION );
	m_nameChanged	= theApp.m_recordset->GetFieldString( PEOPLE_KNOWN_AS );
	m_csalad		= theApp.m_recordset->GetFieldString( PEOPLE_CSALAD );
	m_folyt			= theApp.m_recordset->GetFieldString( PEOPLE_FOLYT );
	m_father_id		= theApp.m_recordset->GetFieldString( PEOPLE_FATHER_ID );
	m_mother_id		= theApp.m_recordset->GetFieldString( PEOPLE_MOTHER_ID );
	m_tableAncestry	= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_TABLEANCESTRY ) );

	m_role			= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_SOURCE ) );
	m_sex_id		= _wtoi( theApp.m_recordset->GetFieldString( PEOPLE_SEX_ID ) );

	m_name.Format( L"%s %s", m_last_name, m_first_name );

	if( !m_rowid.IsEmpty() )
		m_table			= getTableName( m_tableNumber );



	m_ComboSex.SetCurSel( m_sex_id );
	m_ComboRole.SetCurSel( m_role - 1);

	m_father = getParent( m_father_id );
	m_mother = getParent( m_mother_id );


	UpdateData( TOSCREEN );
	
	m_husband.Format( L"%s %s", m_last_name, m_first_name );
	m_husband +=getPlaceDateBlock( m_birth_place, m_birth_date, '*' );
	m_husband +=getPlaceDateBlock( m_death_place, m_death_date, '+' );
	m_husband +=getPlaceDateBlock( m_comment, m_occupation, ' ' );

	// házastársak
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_ROWID,			L"rowid",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_ROWIDS,			L"rowidS",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_ORDERH,			L"#",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ORDERW,			L"#",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_PLACE,			L"hely",		LVCFMT_LEFT,	100,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DATE,			L"dátum",		LVCFMT_RIGHT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SPOUSE,			L"házastárs",	LVCFMT_LEFT,	350,-1,COL_TEXT);

	m_ListCtrlB.SortByHeaderClick(TRUE);
	m_ListCtrlB.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrlB.InsertColumn( B_ROWID,			L"rowid",		LVCFMT_RIGHT,	40,-1,COL_HIDDEN );
	m_ListCtrlB.InsertColumn( B_CNT,			L"",			LVCFMT_RIGHT,	30,-1,COL_NUM );
	m_ListCtrlB.InsertColumn( B_TITLE,			L"cím",			LVCFMT_LEFT,	270,-1,COL_TEXT );
	m_ListCtrlB.InsertColumn( B_DATE,			L"dátum",		LVCFMT_LEFT,	80,-1,COL_HIDDEN );
	m_ListCtrlB.InsertColumn( B_EXT,			L"ext",			LVCFMT_LEFT,	30,-1,COL_HIDDEN );
	m_ListCtrlB.InsertColumn( B_COMMENT,		L"leírás",		LVCFMT_LEFT,	500,-1,COL_HIDDEN );

	fillMarriageTable();
	fillBlobTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::fillBlobTable()
{
	m_command.Format( L"SELECT rowid,* FROM pictures WHERE table_id=1 AND id='%s'", m_rowid );
	if( !theApp.queryBlob( m_command ) ) return;

	u_int64 sizeTotal = 0;
	u_int64	sizeBlob;
	m_ListCtrlB.DeleteAllItems();
	int nItem = 0;
	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i, theApp.m_recordsetBlob->MoveNext() )
	{
		str.Format( L"%d", i+1 );
		nItem = m_ListCtrlB.InsertItem( nItem, theApp.m_recordsetBlob->GetFieldString( PIC_ROWID) );
		m_ListCtrlB.SetItemText( nItem, B_CNT, str );
		m_ListCtrlB.SetItemText( nItem, B_DATE, theApp.m_recordsetBlob->GetFieldString( PIC_DATE) );
		m_ListCtrlB.SetItemText( nItem, B_EXT, theApp.m_recordsetBlob->GetFieldString( PIC_EXT) );
		m_ListCtrlB.SetItemText( nItem, B_TITLE, theApp.m_recordsetBlob->GetFieldString( PIC_TITLE) );
		m_ListCtrlB.SetItemText( nItem, B_COMMENT, theApp.m_recordsetBlob->GetFieldString( PIC_COMMENT) );
		sizeBlob = _wtoi( theApp.m_recordsetBlob->GetFieldString( PIC_SIZE) );
		sizeTotal += sizeBlob;

	}
	m_ListCtrlB.SetSortOrder( 1, 2 );

	if( m_ListCtrlB.GetItemCount() )
		GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
	else
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::fillMarriageTable()
{
	int nItem = 0;
	CString spouse_id;
	CString first_name;
	CString last_name;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString orderH;
	CString orderW;

	if( m_sex_id == MAN )
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id='%s' ORDER BY orderWife", m_rowid );
	else
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse2_id='%s' ORDER BY orderHusband", m_rowid );
	if( !theApp.query( m_command ) ) return;

	m_numberOfWifes = theApp.m_recordset->RecordsCount();

	m_ListCtrl.DeleteAllItems();

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		orderH = theApp.m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		orderW = theApp.m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
		m_orderW = _wtoi( orderW );
		m_orderH = _wtoi( orderH );
		
		nItem = m_ListCtrl.InsertItem( nItem, theApp.m_recordset->GetFieldString( MARRIAGES_ROWID ) );
		if( m_sex_id == MAN )
		{
			spouse_id = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
			m_ListCtrl.SetItemText( nItem, L_ORDERH, orderW );
			m_ListCtrl.SetItemText( nItem, L_ORDERW, orderH );
			m_orderH = _wtoi( orderW );
			m_orderW = _wtoi( orderH );
		}
		else
		{
			spouse_id = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
			m_ListCtrl.SetItemText( nItem, L_ORDERH, orderH );
			m_ListCtrl.SetItemText( nItem, L_ORDERW, orderW );
			m_orderH = _wtoi( orderH );
			m_orderW = _wtoi( orderW );
		}

		
		m_ListCtrl.SetItemText( nItem, L_ROWIDS, spouse_id );
		
		m_ListCtrl.SetItemText( nItem, L_PLACE, theApp.m_recordset->GetFieldString( MARRIAGES_PLACE ) );
		m_ListCtrl.SetItemText( nItem, L_DATE, theApp.m_recordset->GetFieldString( MARRIAGES_DATE ) );


		m_wife = getPeopleString( spouse_id );
		m_ListCtrl.SetItemText( nItem, L_SPOUSE, m_wife );
		++nItem;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeople::getTableName( CString tableNumber )
{
	m_command.Format( L"SELECT rowid,* FROM tables WHERE rowid='%s'", tableNumber );
	if( !theApp.query( m_command ) ) return L"";
	m_table = theApp.m_recordset->GetFieldString( TABLES_TABLEHEADER );
	return( m_table );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedStaticTable()
{
	CTableTables dlg;
	dlg.m_select = true;			// csak táblát választ
	dlg.m_caption = L"Válaszd ki a családot!";
	if( dlg.DoModal() == IDCANCEL ) return;
	
	m_table			= dlg.m_tableHeader;
	m_tableNumber	= dlg.m_id;
	m_last_name		= dlg.m_last_name;
	m_mother_id.Empty();
	m_father_id.Empty();
	GetDlgItem( IDC_TABLE )->SetWindowText( m_table );
	GetDlgItem( IDC_LAST_NAME )->SetWindowText( m_last_name );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedStaticFather()
{
	CGetPeople dlg;
	dlg.m_caption		= L"Válaszd ki az apát!";
	dlg.m_sex_id = MAN;
	if( dlg.DoModal() == IDCANCEL )
		return;

	m_father_id	= dlg.m_rowid;
	m_father	= dlg.m_people;

	m_generation = dlg.m_generation;

	m_mother_id.Empty();

	m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id='%s'", m_father_id );
	if( !theApp.query( m_command ) ) return ;
	if( theApp.m_recordset->RecordsCount() < 2 )
	{
		m_mother_id = theApp.m_recordset->GetFieldString( 0 );
	}
	else
	{
		CSelectMother dlg;
		dlg.m_rowidF	= m_father_id;
		if( dlg.DoModal() != IDCANCEL )
		{
			m_mother_id = dlg.m_rowidM;
			m_mother_index = dlg.m_mother_index;
			m_numberOfWifes = dlg.m_numberOfWifes;
		}
	}

	m_mother = getParent( m_mother_id );
	GetDlgItem( IDC_FATHER )->SetWindowTextW( m_father );
	GetDlgItem( IDC_MOTHER )->SetWindowTextW( m_mother );

	GetDlgItem( IDC_GENERATION )->SetWindowTextW( m_generation );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedDeleteFather()
{
	if( m_father.IsEmpty() ) return;

	str.Format( L"Tényleg törölni akarod %s apát?", m_father );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_father.Empty();
	m_father_id.Empty();
	GetDlgItem( IDC_FATHER )->SetWindowTextW( m_father );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedStaticMother()
{
	CSelectMother dlg;
	dlg.m_rowidF = m_father_id;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_mother_id = dlg.m_rowidM;
	m_mother = getParent( m_mother_id );
	m_father_id = dlg.m_rowidF;
	m_father = getParent( m_father_id );

	GetDlgItem( IDC_MOTHER )->SetWindowTextW( m_mother );
	GetDlgItem( IDC_FATHER )->SetWindowTextW( m_father );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedDeleteMother()
{
	if( m_mother.IsEmpty() ) return;

	str.Format( L"Tényleg törölni akarod %s anyát?", m_mother );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	m_mother.Empty();
	m_mother_id.Empty();
	GetDlgItem( IDC_MOTHER )->SetWindowTextW( m_mother );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeople::getParent( CString rowid )
{
	CString birth;
	CString death;
	CString parent;
	CString comment;

	m_command.Format( L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid );
	if( !theApp.query( m_command ) ) return L"";
	parent.Format( L"%s %s", theApp.m_recordset->GetFieldString( PEOPLE_LAST_NAME ), theApp.m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
	birth = theApp.m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	death = theApp.m_recordset->GetFieldString( PEOPLE_DEATH_DATE );
	comment = theApp.m_recordset->GetFieldString( PEOPLE_COMMENT );
	
	if( !birth.IsEmpty() )
	{
		parent += " *";
		parent += birth;
	}
	if( !death.IsEmpty() )
	{
		parent += " +";
		parent += death;
	}
	parent += L" ";
	parent += comment;
	parent.TrimRight();
	return parent;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnKillfocusComboSex()
{
	m_sex_id = m_ComboSex.GetCurSel();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnKillfocusEditFirstname()
{
	GetDlgItem( IDC_FIRST_NAME )->GetWindowText( m_first_name );
	CStringArray A;
	int n;

	if( m_first_name.IsEmpty() )
	{
		AfxMessageBox( L"A keresztnév megadása kötelezõ!" );
	}

	n = wordList( &A, m_first_name, ' ', FALSE );
	for( int i = 0; i < n; ++i )
	{
		m_sex_id = theApp.isFirstName( A[i] );
		if( m_sex_id == -1 )
		{
			str.Format( L"%s keresztnév nincs a nyilvántartásban!", A[i] );
			m_first_name.Empty();
			AfxMessageBox( str );
			GetDlgItem( IDC_FIRST_NAME )->SetWindowText( m_first_name );
		}
		m_ComboSex.SetCurSel( m_sex_id );
	}
	m_husband.Format( L"%s %s", m_last_name, m_first_name );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnKillfocusLastName()
{
	GetDlgItem( IDC_LAST_NAME )->GetWindowText( m_last_name );
	m_husband.Format( L"%s %s", m_last_name, m_first_name );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnKillfocusEditBirthd()
{
	GetDlgItem( IDC_BIRTH_DATE )->GetWindowTextW( m_birth_date );
	if( !isDateOK( m_birth_date ) )
	{
		str.Format( L"%s dátum szabálytalan!", m_birth_date );
		AfxMessageBox( str );
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnKillfocusEditDeathd()
{
	GetDlgItem( IDC_DEATH_DATE )->GetWindowTextW( m_death_date );
	if( !isDateOK( m_death_date ) )
	{
		str.Format( L"%s dátum szabálytalan!", m_death_date );
		AfxMessageBox( str );
		return;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CEditPeople::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	CRect rect;

	GetDlgItem( IDC_BLOBS )->GetWindowRect( rect );

	if( rect.top <= point->y && point->y <= rect.bottom )
	{
		if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
		{
			pPopup = Menu.GetSubMenu(0);
			if(m_ListCtrlB.GetNextItem(-1,LVNI_SELECTED) < 0 )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
			}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
    }
	else
	{
		if(Menu.LoadMenu( IDR_DROPDOWN_EDITM ))
		{
			pPopup = Menu.GetSubMenu(0);
			if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATEM,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETEM, MF_BYCOMMAND | MF_GRAYED);
			}
			if( m_sex_id == WOMAN )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATEM,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_INSERTM, MF_BYCOMMAND | MF_GRAYED);
			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
	 }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditInsertM()
{
	if( !m_sex_id )
	{
		AfxMessageBox( L"Az emberünk nemét meg kell adni!" );
		return;
	}

	m_husband.Format( L"%s %s", m_last_name, m_first_name );
	m_husband +=getPlaceDateBlock( m_birth_place, m_birth_date, '*' );
	m_husband +=getPlaceDateBlock( m_death_place, m_death_date, '+' );
	m_husband +=getPlaceDateBlock( m_comment, m_occupation, ' ' );

	CGetMarriage dlg;

	dlg.m_rowidM.Empty();
	dlg.m_husband	= m_husband;
	dlg.m_orderW	= m_numberOfWifes + 1;
	dlg.m_orderH	= 1;

	dlg.m_sex_id = m_sex_id;
	if( m_sex_id == MAN )
		dlg.m_rowidS1 = m_rowid;
	else
		dlg.m_rowidS2 = m_rowid;

	if( dlg.DoModal() == IDCANCEL ) return;

	fillMarriageTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditUpdateM()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve egy sor sem!" );
		return;
	}
	updateM( nItem );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditDeleteM()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 )
	{
		AfxMessageBox( L"Nincs kijelölve egy sor sem!" );
		return;
	}
	CString spouse;
	spouse	= m_ListCtrl.GetItemText( nItem, L_SPOUSE );
	str.Format( L"Tényleg törölni akarod %s-t a házastársak közül?", spouse );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	m_command.Format( L"DELETE FROM marriages WHERE rowid='%s'", rowid );
	theApp.execute( m_command );
	m_ListCtrl.DeleteItem( nItem );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnDblclkBlobs(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	EditUpdate( false );
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditUpdate()
{
	EditUpdate( false );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedPhotos() 
{
	EditUpdate( true );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::EditUpdate( bool all )
{
	CString rowidB(L"");
	if( !all )
	{
		int nItem	= m_ListCtrlB.GetNextItem(-1, LVNI_SELECTED);
		if( nItem == -1 ) return;
		rowidB = m_ListCtrlB.GetItemText( nItem, B_ROWID );
	}

	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLEX, m_rowid ); 
	dlg.m_name		= m_name;	// az ember neve
	dlg.m_rowidP	= m_rowid;	// ember rowid-ja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja:  Az ember összes képeit a rowidB-vel kezdve mutassa be

	dlg.DoModal();
	fillBlobTable();
	displayPicture();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditDelete()
{
	int nItem	= m_ListCtrlB.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -1 ) return;

	CString rowid;
	CString title;
	rowid = m_ListCtrlB.GetItemText( nItem, B_ROWID );
	title = m_ListCtrlB.GetItemText( nItem, B_TITLE );

	str.Format( L"Tényleg törölni akarod a '%s' fájlt?", title );
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;


	m_command.Format( L"DELETE FROM pictures WHERE rowid='%s'", rowid );
	theApp.executeBlob( m_command );
	m_ListCtrlB.DeleteItem( nItem );
	if( !m_ListCtrlB.GetItemCount() ) 
		GetDlgItem( IDC_PHOTOS )->EnableWindow( false );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnEditInsert()
{
	if( !theApp.selectFiles() ) return;

	BLOBSTAT stat;
	int rowid;
	
	int sorsz = m_ListCtrlB.GetItemCount() + 1;
	int	nItem = m_ListCtrlB.GetItemCount();

	theApp.executeBlob( L"BEGIN" );
	for( UINT i = 0; i < theApp.vBlobs.size(); ++i )
	{
		stat.ext = theApp.vBlobs.at(i).ext;
		stat.filename	= theApp.vBlobs.at(i).filename;
		stat.fileSpec	= theApp.vBlobs.at(i).fileSpec;
		stat.size		= theApp.vBlobs.at(i).size;

		rowid = theApp.blobDB->blobInsert( "pictures", "picture", &stat );
		if( !rowid ) return;
	
		m_command.Format( L"UPDATE pictures SET id='%s', table_id='%d', title='%s', filename= '%s', ext='%s', size='%d' WHERE rowid='%d'", m_rowid, PEOPLEX, stat.filename, stat.filename, stat.ext, stat.size, rowid );
		if( !theApp.executeBlob( m_command ) ) return;

		str.Format( L"%d", rowid );
		nItem = m_ListCtrlB.InsertItem( nItem, str );

		str.Format( L"%d", sorsz );
		m_ListCtrlB.SetItemText( nItem, B_CNT, str );

		m_ListCtrlB.SetItemText( nItem, B_EXT, theApp.vBlobs.at(i).ext ) ;
		m_ListCtrlB.SetItemText( nItem, B_TITLE, theApp.vBlobs.at(i).filename );
		++sorsz;
	}
	theApp.executeBlob( L"COMMIT" );
	m_ListCtrlB.EnsureVisible( nItem, FALSE );

	if( m_ListCtrlB.GetItemCount() ) GetDlgItem( IDC_PHOTOS )->EnableWindow( true );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::updateM( int nItem )
{
	m_husband.Format( L"%s %s", m_last_name, m_first_name );
	m_husband +=getPlaceDateBlock( m_birth_place, m_birth_date, '*' );
	m_husband +=getPlaceDateBlock( m_death_place, m_death_date, '+' );
	m_husband +=getPlaceDateBlock( m_comment, m_occupation, ' ' );

	CGetMarriage dlg;

	dlg.m_husband	= m_husband;
		
	dlg.m_sex_id		= m_sex_id;
	if( m_sex_id == MAN )
	{
		dlg.m_rowidS1 = m_rowid;
		dlg.m_rowidS2 = m_ListCtrl.GetItemText( nItem, L_ROWIDS );
		dlg.m_orderH	= _wtoi( m_ListCtrl.GetItemText( nItem, L_ORDERH ) );
		dlg.m_orderW	= _wtoi( m_ListCtrl.GetItemText( nItem, L_ORDERW ) );
	}
	else
	{
		dlg.m_rowidS1 = m_ListCtrl.GetItemText( nItem, L_ROWIDS );
		dlg.m_rowidS2 = m_rowid;
		dlg.m_orderH	= _wtoi( m_ListCtrl.GetItemText( nItem, L_ORDERW ) );
		dlg.m_orderW	= _wtoi( m_ListCtrl.GetItemText( nItem, L_ORDERH ) );
	}

	dlg.m_rowidM	= m_ListCtrl.GetItemText( nItem, L_ROWID );
	
	
	dlg.m_date		= m_ListCtrl.GetItemText( nItem, L_DATE );
	dlg.m_place		= m_ListCtrl.GetItemText( nItem, L_PLACE );
	dlg.m_spouse	= m_ListCtrl.GetItemText( nItem, L_SPOUSE );


	if( dlg.DoModal() == IDCANCEL ) return;

	fillMarriageTable();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kijelölt feleség szerkesztése
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if( m_sex_id == WOMAN ) return;
	

	int nItem = pNMItemActivate->iItem;
	updateM( nItem );
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeople::getPeopleString( CString rowid )
{
	CString last_name;
	CString first_name;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString occupation;


	m_command.Format( L"SELECT rowid, * FROM people WHERE rowid='%s'", rowid );
	if( !theApp.query1( m_command ) ) return L"";
	last_name	= theApp.m_recordset1->GetFieldString( PEOPLE_LAST_NAME );
	first_name	= theApp.m_recordset1->GetFieldString( PEOPLE_FIRST_NAME );
	birth_place	= theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_PLACE );
	birth_date	= theApp.m_recordset1->GetFieldString( PEOPLE_BIRTH_DATE );
	death_place	= theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_PLACE );
	death_date	= theApp.m_recordset1->GetFieldString( PEOPLE_DEATH_DATE );
	comment		= theApp.m_recordset1->GetFieldString( PEOPLE_COMMENT );
	occupation	= theApp.m_recordset1->GetFieldString( PEOPLE_OCCUPATION );

	str.Format( L"%s %s", last_name, first_name );
	str += getPlaceDateBlock( birth_place, birth_date, '*' );
	str += getPlaceDateBlock( death_place, death_date, '+' );
	str += getPlaceDateBlock( comment, occupation, ' ' );

	str.Trim();
	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeople::getPlaceDateBlock( CString place, CString date, TCHAR jel )
{
	
	CString block(L"");

	if( !place.IsEmpty() || !date.IsEmpty() )
	{
		block.Format( L" %c", jel );
		if( !place.IsEmpty() )
		{
			block += place;
			block += L" ";
		}
		if( !date.IsEmpty() )
		{
			block += date;
		}
		block.TrimRight();
		
	}
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CEditPeople::getCommentBlock( CString comment, CString occupation  )
{
	CString block(L"");

	if( !occupation.IsEmpty() || !comment.IsEmpty() )
	{
		if( !comment.IsEmpty() )
		{
			block += comment;
			block += L" ";
		}
		if( !occupation.IsEmpty() )
		{
			block += occupation;
		}
		block.TrimRight();
	}
	return block;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnClickedTableAncestry()
{
	m_tableAncestry = !m_tableAncestry;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnPaint()
{
	if( !m_paint ) 
	{
		CDialogEx::OnPaint();
		return;
	}
	CPaintDC dc(this);
	CDC dcMemory;
	CImage image;
	CBitmap bitmap;


	image.Load( m_fileSpec ); // just change extension to load jpg
	bitmap.Attach(image.Detach());

	dcMemory.CreateCompatibleDC( &dc);
    dcMemory.SelectObject(&bitmap);

	BITMAP bm;						// bitmap in memory
	bitmap.GetBitmap( &bm );
	int bmWidth		= bm.bmWidth;
	int bmHeight	= bm.bmHeight;
	float bmRatio	= (float)bm.bmWidth/(float)bmHeight;

	CRect rect;		// a dlg ablak méretei 0,0 pontból ( Windows koordinátákban )
//	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!
	CRect rectE;	

	GetClientRect(&rect);
	GetWindowRect( rectW );
//	GetDlgItem( IDC_TITLE )->GetWindowRect( rectE );

	int height = rectW.Height()/2;				// a transzformát kép magassága az ablak feléig!
//	height = rectE.top - rectW.top - 72 ;
	height = 160;

	int width = (int) (height * bmRatio );		// a transzformált kép szélessége a bmRatio-val arányos

	int cW = (rectW.right - rectW.left)/2;  // ablak középpontjának x koordinátája
	int cB = width/2;						// a sarokban lévõ kép középpontjának x koordinátája
	int x = cW - cB;						// az ablak új sarka

	x = rect.right - width - 10;

	int	y = 10;

	dc.SetStretchBltMode( STRETCH_HALFTONE );
	dc.StretchBlt( x, y, width, height, &dcMemory, 0,0, bmWidth, bmHeight, SRCCOPY);
	CDialogEx::OnPaint();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEditPeople::writeBlockToFile( void* block, int blob_size  )
{
	m_fileSpec.Format( L"%s\\tmp.jpg", theApp.m_workingDirectory );
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CEditPeople::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CEditPeople::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEditPeople::OnBnClickedOk()
{
	UpdateData( FROMSCREEN );

	theApp.WriteProfileInt( L"Settings", L"tableNumber", _wtoi(m_tableNumber) );
	theApp.WriteProfileStringW( L"Settings", L"father_id", m_father_id );
	theApp.WriteProfileStringW( L"Settings", L"mother_id", m_mother_id );

	m_sex_id = m_ComboSex.GetCurSel();

	if( m_last_name.IsEmpty() )
	{
		AfxMessageBox( L"Családnevet meg kell adni!" );
		return;
	}
	if( m_first_name.IsEmpty() )
	{
		AfxMessageBox( L"Keresztnevet meg kell adni!" );
		return;
	}

	CString fields;
	CString values;
	if( m_rowid.IsEmpty() )
	{

		fields.Format( L"\
tableNumber,\
generation,\
sex_id,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
father_id,\
mother_id,\
known_as,\
csalad,\
tableAncestry" );
		values.Format( L"'%s','%s', '%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d'",
		m_tableNumber,
		m_generation,\
		m_sex_id,\
		m_title,\
		m_titolo,\
		m_first_name,\
		m_last_name,\
		m_posterior,\
		m_birth_place,\
		m_birth_date,\
		m_death_place,\
		m_death_date,\
		m_comment,\
		m_father_id,\
		m_mother_id,\
		m_nameChanged,\
		m_csalad,\
		m_tableAncestry\
		);
		m_command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
		if( !theApp.execute( m_command ) ) return;

		++theApp.m_cntPeople;
		m_command = L"SELECT last_insert_rowid() FROM people";
		if( ! theApp.query( m_command ) ) return;
		m_rowid = theApp.m_recordset->GetFieldString(0);
	}
	else
	{
		m_command.Format( L"UPDATE people SET \
		tableNumber='%s',\
		sex_id='%d',\
		title='%s',\
		titolo='%s',\
		first_name='%s',\
		last_name='%s',\
		posterior='%s',\
		birth_place='%s',\
		birth_date='%s',\
		death_place='%s',\
		death_date='%s',\
		comment='%s',\
		father_id='%s',\
		mother_id='%s',\
		known_as='%s',\
		csalad='%s',\
		tableAncestry='%d' \
		WHERE rowid='%s'\
		",\
		m_tableNumber,
		m_sex_id,\
		m_title,\
		m_titolo,\
		m_first_name,\
		m_last_name,\
		m_posterior,\
		m_birth_place,\
		m_birth_date,\
		m_death_place,\
		m_death_date,\
		m_comment,\
		m_father_id,\
		m_mother_id,\
		m_nameChanged,\
		m_csalad,\
		m_tableAncestry,\
		m_rowid\
		);
		if( !theApp.execute( m_command ) ) return;
	}
	m_m = m_rowid;   // m_rowid rossz lesz a TableIndividual.ban!! m_m jó!!! Érthetetlen!
	CDialogEx::OnOK();
}