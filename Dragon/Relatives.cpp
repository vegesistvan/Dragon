// Relatives.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Relatives.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "EditPeople.h"
#include "EditMarriage.h"
#include "GA_descendants.h"
#include "SelectFromPeople.h"
// CRelatives dialog

IMPLEMENT_DYNAMIC(CRelatives, CDialogEx)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRelatives::CRelatives(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRelatives::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRelatives::~CRelatives()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PEOPLE, colorPeople);
	DDX_Control(pDX, IDC_STATIC_FATHER, colorFather);
	DDX_Control(pDX, IDC_STATIC_MOTHER, colorMother);
	DDX_Control(pDX, IDC_STATIC_CHILDREN, colorChildren);
	DDX_Control(pDX, IDC_STATIC_SIBLINGS, colorSiblings);
	DDX_Control(pDX, IDC_STATIC_MARRIAGES, colorMarriages);

	DDX_Control(pDX, IDC_LISTCTRL_PARENTS, m_ListCtrlP);
	DDX_Control(pDX, IDC_LISTCTRL_MARRIAGES, m_ListCtrlM);
	DDX_Control(pDX, IDC_LISTCTRL_CHILDREN, m_ListCtrlC);
	DDX_Control(pDX, IDC_LISTCTRL_SIBLINGS, m_ListCtrlS);

}
BEGIN_MESSAGE_MAP(CRelatives, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_BN_CLICKED(ID_EDIT_DELETE, &CRelatives::OnEditDelete )
	ON_BN_CLICKED(ID_EDIT_UPDATE, &CRelatives::OnEditUpdate )
	ON_BN_CLICKED(ID_EDIT_INSERT, &CRelatives::OnEditInsert )

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

	ON_COMMAND(ID_ASCESTOR, &CRelatives::OnAscestor)
	ON_COMMAND(ID_DESCENDANTS, &CRelatives::OnDescendants)
	ON_COMMAND(ID_RESET, &CRelatives::OnReset)
	ON_STN_CLICKED(IDC_STATIC_FATHER, &CRelatives::OnClickedStaticFather)
	ON_STN_CLICKED(IDC_STATIC_MOTHER, &CRelatives::OnClickedStaticMother)
	ON_STN_CLICKED(IDC_STATIC_MARRIAGES, &CRelatives::OnClickedStaticMarriages)
	ON_STN_CLICKED(IDC_STATIC_SIBLINGS, &CRelatives::OnClickedStaticSiblings)
	ON_STN_CLICKED(IDC_STATIC_CHILDREN, &CRelatives::OnClickedStaticChildren)
	ON_COMMAND(ID_EDIT_GYEREK, &CRelatives::OnInsertGyerek)

	ON_STN_DBLCLK(IDC_STATIC_PEOPLE, &CRelatives::OnDblclkStaticPeople)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_PARENTS, &CRelatives::OnDblclkListCtrlParents)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_MARRIAGES, &CRelatives::OnDblclkListCtrlMarriages)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_SIBLINGS, &CRelatives::OnDblclkListCtrlSiblings)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL_CHILDREN, &CRelatives::OnDblclkListCtrlChildren)


	ON_COMMAND(ID_INFO, &CRelatives::OnInfo)
	ON_COMMAND(ID_EDIT_MARRIAGE, &CRelatives::OnEditMarriage)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LISTCTRL_SIBLINGS,ES_BORDER,	ES_BORDER,	ES_BORDER,	ES_BORDER,	0 );
	EASYSIZE_ADD( IDOK,					ES_BORDER,ES_KEEPSIZE,	ES_KEEPSIZE,ES_BORDER,	0 );
	EASYSIZE_ADD( IDCANCEL,				ES_BORDER,ES_KEEPSIZE,	ES_KEEPSIZE,ES_BORDER,	0 );
	EASYSIZE_INIT();

	m_columns = L"rowid, tablenumber, sex_id, last_name, first_name, birth_place, birth_date, death_place, death_date, occupation, comment, father_id, mother_id";

//	colorPeople.SetTextColor( RIVERBLUE );
//	colorPeople.SetTextColor( RGB( 255,255,0 ) );
	colorPeople.SetTextColor( RGB( 255,51,255 ) );
	colorPeople.SetTextColor( RGB( 255,0,0 ) );

	colorFather.SetTextColor( theApp.m_colorClick );
	colorMother.SetTextColor( theApp.m_colorClick );
	colorMarriages.SetTextColor( theApp.m_colorClick );
	colorChildren.SetTextColor( theApp.m_colorClick );
	colorSiblings.SetTextColor( theApp.m_colorClick );

	createColumns( &m_ListCtrlP );
	createColumns( &m_ListCtrlM );
	createColumns( &m_ListCtrlC );
	createColumns( &m_ListCtrlS );

	m_rowidFirst = m_rowid;

	if( !m_rowid.IsEmpty() )
		createScreen();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::createColumns( CListCtrlEx* p_ListCtrl )
{
	p_ListCtrl->KeepSortOrder(TRUE);
	p_ListCtrl->SetExtendedStyle( m_ListCtrlS.GetExtendedStyle()| LVS_EX_GRIDLINES );	

	p_ListCtrl->InsertColumn( L_NAME,	L"name",		LVCFMT_LEFT,	150,-1,COL_HIDDEN );
	p_ListCtrl->InsertColumn( L_ROWID,	L"rowid",		LVCFMT_RIGHT,	 50,-1,COL_TEXT);
	p_ListCtrl->InsertColumn( L_PEOPLE,	L"people",		LVCFMT_LEFT,   2000,-1,COL_TEXT );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createScreen()
{
	createPeople();

	m_ListCtrlP.DeleteAllItems();
	createFather( m_father_id );
	createMother( m_mother_id );
	createSpouses();
	createChildren();
	createSiblings();

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createPeople()
{
	CString people;
	people = selectPeople( m_rowid );
	
	m_tablenumber	= m_recordset->GetFieldString( TABLENUMBER );
	m_sexid			= m_recordset->GetFieldString( SEXID );
	m_lastname		= m_recordset->GetFieldString( LASTNAME );
	m_firstname		= m_recordset->GetFieldString( FIRSTNAME );
	m_birthplace	= m_recordset->GetFieldString( BIRTHPLACE );
	m_birthdate		= m_recordset->GetFieldString( BIRTHDATE );
	m_deathplace	= m_recordset->GetFieldString( DEATHPLACE );
	m_deathdate		= m_recordset->GetFieldString( DEATHDATE );
	m_profession	= m_recordset->GetFieldString( PROFESSION );
	m_comment		= m_recordset->GetFieldString( COMMENT );
	m_father_id		= m_recordset->GetFieldString( FATHERID );
	m_mother_id		= m_recordset->GetFieldString( MOTHERID );

	m_name.Format( L"%s %s", m_lastname, m_firstname );

	str.Format( L"%s családi kapcsolatainak ábrázolása és szerkesztése", m_name ); 
	SetWindowTextW( str );

	str.Format( L"%13s     %s", m_rowid, people );

	GetDlgItem( IDC_STATIC_PEOPLE )->SetWindowTextW( str );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createFather( CString father_id )
{
	CString people;

	people = selectPeople( father_id );
	if( !m_ListCtrlP.GetItemCount() )
		m_ListCtrlP.InsertItem( 0, m_name );
	else
		m_ListCtrlP.SetItemText( 0, L_NAME, m_name );
	m_ListCtrlP.SetItemText( 0, L_ROWID, father_id );
	m_ListCtrlP.SetItemText( 0, L_PEOPLE, people );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createMother( CString mother_id )
{
	CString people;

	people = selectPeople( mother_id );
	if( m_ListCtrlP.GetItemCount() < 2  )
		m_ListCtrlP.InsertItem( 1, m_name );
	else
		m_ListCtrlP.SetItemText( 1, L_NAME, m_name );
	m_ListCtrlP.SetItemText( 1, L_ROWID, mother_id );
	m_ListCtrlP.SetItemText( 1, L_PEOPLE, people );

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createSpouses()
{
	UINT	i;
	CString	rowid;
	CString	people;

	m_ListCtrlM.DeleteAllItems();
	if( m_sexid == L"1" )
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id = '%s'", m_rowid );
	else
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse2_id = '%s'", m_rowid );
	if( !query2( m_command ) ) return false;

	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{ 
		rowid	= m_recordset2->GetFieldString( 0 );
		people	= selectPeople( rowid );
		
		m_ListCtrlM.InsertItem( i, m_name );
		m_ListCtrlM.SetItemText( i, L_ROWID, rowid ); 
		m_ListCtrlM.SetItemText( i, L_PEOPLE, people );
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createChildren()
{
	UINT i;
	CString rowid;
	CString people;

	m_ListCtrlC.DeleteAllItems();

	if( m_sexid == L"1" )
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s'", m_rowid );
	else
		m_command.Format( L"SELECT rowid FROM people WHERE mother_id = '%s'", m_rowid );
	if( !query2( m_command ) ) return false;

	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{
		rowid	= m_recordset2->GetFieldString( 0 );
		people	= selectPeople( rowid );
		m_ListCtrlC.InsertItem( i, m_name );
		m_ListCtrlC.SetItemText( i, L_ROWID, rowid ); 
		m_ListCtrlC.SetItemText( i, L_PEOPLE, people );
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createSiblings()
{
	UINT i;
	int nItem = 0;
	CString name;
	CString rowid;
	CString people;

	m_ListCtrlS.DeleteAllItems();
	
	if( m_sexid == L"1" )
	{
		if( m_father_id == L"0" || m_father_id == L"" ) return true ;
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s'", m_father_id );
	}
	else
	{
		if( m_mother_id == L"0" || m_mother_id == L"" ) return true ;
		m_command.Format( L"SELECT rowid FROM people WHERE mother_id = '%s'", m_mother_id );
	}
	if( !query2( m_command ) ) return false;

	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{
		rowid = m_recordset2->GetFieldString( 0 );
		if( rowid != m_rowid )
		{
			people	= selectPeople( rowid );
			m_ListCtrlS.InsertItem( nItem, m_name );
			m_ListCtrlS.SetItemText( nItem, L_ROWID, rowid ); 
			m_ListCtrlS.SetItemText( nItem, L_PEOPLE, people );
			++nItem;
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CRelatives::selectPeople( CString rowid )
{
	CString tablenumber;
	CString sexid;
	CString lastname;
	CString firstname;
	CString birthplace;
	CString birthdate;
	CString deathplace;
	CString deathdate;
	CString profession;
	CString comment;
	CString father_id;
	CString mother_id;

	CString birth;
	CString death;
	

	m_command.Format( L"SELECT %s FROM people WHERE rowid = '%s'", m_columns, rowid );
	if( !query( m_command ) ) return L"";
	tablenumber	= m_recordset->GetFieldString( TABLENUMBER );
	sexid		= m_recordset->GetFieldString( SEXID );
	lastname	= m_recordset->GetFieldString( LASTNAME );
	firstname	= m_recordset->GetFieldString( FIRSTNAME );
	birthplace	= m_recordset->GetFieldString( BIRTHPLACE );
	birthdate	= m_recordset->GetFieldString( BIRTHDATE );
	deathplace	= m_recordset->GetFieldString( DEATHPLACE );
	deathdate	= m_recordset->GetFieldString( DEATHDATE );
	profession	= m_recordset->GetFieldString( PROFESSION );
	comment		= m_recordset->GetFieldString( COMMENT );
	father_id	= m_recordset->GetFieldString( FATHERID );
	mother_id	= m_recordset->GetFieldString( MOTHERID );

	birth = pack( L"*", birthplace, birthdate );
	death = pack( L"+", deathplace, deathdate );
	
	str.Format( L"%s %s", lastname, firstname );
	str.Format( L"%s %s", (CString)str, birth );
	str.TrimRight() ;
	str.Format( L"%s %s", (CString)str, death );
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, comment ); 
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, profession ); 
	str.TrimRight();

	m_name.Format( L"%s %s", lastname, firstname );
	m_name.Trim();

	return  str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CRelatives::pack( CString tag, CString place, CString date )
{
	str.Format( L"%s %s", place, date );
	str.Trim();
	if( !str.IsEmpty() )
		str.Format( L"%s%s", tag, (CString)str );
	return str;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		theApp.message( L"Query", str );
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlParents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString rowid;
	m_rowid = m_ListCtrlP.GetItemText( nItem, L_ROWID );
	createScreen();
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlMarriages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString rowid;
	m_rowid = m_ListCtrlM.GetItemText( nItem, L_ROWID );
	createScreen();

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlChildren(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString rowid;
	m_rowid = m_ListCtrlC.GetItemText( nItem, L_ROWID );
	createScreen();
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlSiblings(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString rowid;
	m_rowid = m_ListCtrlS.GetItemText( nItem, L_ROWID );
	createScreen();
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CRelatives::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	CRect rectP;
	CRect rectM;
	CRect rectC;
	CRect rectS;
	
	bool nincs = true;
	m_listCtrlFlag = -1;

	GetDlgItem( IDC_LISTCTRL_PARENTS )->GetWindowRect( rectP );
	GetDlgItem( IDC_LISTCTRL_MARRIAGES )->GetWindowRect( rectM );
	GetDlgItem( IDC_LISTCTRL_CHILDREN )->GetWindowRect( rectC );
	GetDlgItem( IDC_LISTCTRL_SIBLINGS )->GetWindowRect( rectS );

	if( rectP.top <= point->y && point->y <= rectP.bottom )			// testvérek
	{
		if( m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		m_listCtrlFlag = 1;
	}
	else if( rectM.top <= point->y && point->y <= rectM.bottom )			// testvérek
	{
		//if( m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false;
		if( m_ListCtrlM.GetItemCount() ) nincs = false;
		if(Menu.LoadMenu( IDR_DROPDOWN_SPOUSE ))
		{
			pPopup = Menu.GetSubMenu(0);
			if( nincs )
			{
				pPopup->EnableMenuItem(ID_EDIT_UPDATE,MF_BYCOMMAND | MF_GRAYED);
				pPopup->EnableMenuItem(ID_EDIT_DELETE,MF_BYCOMMAND | MF_GRAYED);
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
		m_listCtrlFlag = 2;
		return true;
	}
	else if( rectC.top <= point->y && point->y <= rectC.bottom )			// testvérek
	{
		if( m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		m_listCtrlFlag = 3;
	}
	else if( rectS.top <= point->y && point->y <= rectS.bottom )			// testvérek
	{
		if( m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		m_listCtrlFlag = 4;
	}

	if(  m_listCtrlFlag != -1 )
	{
		if(Menu.LoadMenu( IDR_DROPDOWN_EDIT ))
		{
			pPopup = Menu.GetSubMenu(0);
			pPopup->EnableMenuItem(ID_EDIT_INSERT,MF_BYCOMMAND | MF_GRAYED);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnEditInsert()
{

	int		nIndex;
	CString	rowid;
	CString	name;
	CEditPeople dlg;

	dlg.m_rowid.Empty();
	if( dlg.DoModal() == IDCANCEL ) return;
	rowid = dlg.m_rowid;

	switch( m_listCtrlFlag )
	{
	case 1:
		if( dlg.m_sex_id == MAN )
		{
			m_command.Format( L"UPDATE people SET father_id ='%s' WHERE rowid ='%s'", rowid, m_rowid );
			if( !theApp.execute( m_command ) ) return;
			createFather( rowid );
		}
		else
			createMother( rowid );
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;

	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnEditUpdate()
{

	int nItem;
	int		nIndex;
	DWORD	rowid;
	CString	name;

	CEditPeople dlg;

	switch( m_listCtrlFlag )
	{
	case 1:
/*
		nIndex	= m_ListBoxP.GetCurSel();
		if( nIndex == LB_ERR )
		{
			AfxMessageBox( L"Ki kell jelölni egy szülõt!" );
			return;
		}
		rowid	= m_ListBoxP.GetItemData( nIndex );
		dlg.m_rowid.Format( L"%d", rowid );
		if( dlg.DoModal() == IDCANCEL ) return;

		m_ListBoxP.DeleteString( nIndex );
		if( nIndex == 0 )
			createFather( dlg.m_rowid );
		else
			createMother( dlg.m_rowid );
		break;
*/
		nItem = m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlP.GetItemText( nItem, L_ROWID );
		dlg.DoModal();

		if( !nItem )
			createFather( dlg.m_rowid );
		else
			createMother( dlg.m_rowid );
		break;
	case 2:
		nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlM.GetItemText( nItem, L_ROWID );
		dlg.DoModal();
		createSpouses();
		break;
	case 3:
		nItem = m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlC.GetItemText( nItem, L_ROWID );
		dlg.DoModal();
		createChildren();
		break;
	case 4:
		nItem = m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlS.GetItemText( nItem, L_ROWID );
		dlg.DoModal();
		createSiblings();
		break;
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnEditDelete()
{
	int nItem;
	int		nIndex;
	CString name;
	CString	people;
	CString rowid;
	CString father;
	CString mother;
	CString rowidS;
	switch( m_listCtrlFlag )
	{
	case 1:
		father	= m_ListCtrlP.GetItemText( 0, L_NAME );
		mother	= m_ListCtrlP.GetItemText( 1, L_NAME );
		str.Format( L"Tényleg töröli alkarod a %s-%s szülõket?", father, mother );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteParents();
		break;
	case 2:
		nItem	= m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlM.GetItemText( nItem, L_ROWID );
		name = m_ListCtrlM.GetItemText( nItem, L_NAME );
		
		str.Format( L"Tényleg töröli alkarod %s-t a házastársak közül?", name );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteSpouse( rowidS );
		break;
	case 3:
		nItem	= m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlC.GetItemText( nItem, L_ROWID );
		name = m_ListCtrlC.GetItemText( nItem, L_NAME );
		
		str.Format( L"Tényleg töröli alkarod %s-t a gyerekek közül?", name );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteChild( rowidS );
		break;
	case 4:
		nItem	= m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlS.GetItemText( nItem, L_ROWID );
		name = m_ListCtrlS.GetItemText( nItem, L_NAME );

		str.Format( L"Tényleg töröli alkarod %s-t a testvérek közül?", name );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteSibling( rowidS );
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteParents()
{
	m_command.Format( L"UPDATE people SET father_id = '', mother_id = '' WHERE rowid == '%s'", m_rowid );
	if( !theApp.execute( m_command ) ) return;
	m_father_id = L"";
	m_mother_id = L"";
	
	m_ListCtrlP.DeleteItem( 0 );
	m_ListCtrlP.DeleteItem( 1 );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteSpouse( CString rowid )
{
	if( m_sexid == L"1" )
		m_command.Format( L"DELETE FROM marriages WHERE spouse1_id == '%s' AND spouse2_id='%s'", m_rowid, rowid  );
	else
		m_command.Format( L"DELETE FROM marriages WHERE spouse1_id == '%s' AND spouse2_id='%s'", rowid, m_rowid  );
	if( !theApp.execute( m_command ) ) return;
	createSpouses();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteChild( CString rowid )
{
	m_command.Format( L"UPDATE people SET father_id = '', mother_id = '' WHERE rowid == '%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	createChildren();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteSibling( CString rowid )
{
	m_command.Format( L"UPDATE people SET father_id = '', mother_id = '' WHERE rowid == '%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
	createSiblings();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkStaticPeople()
{
	CEditPeople dlg;
	dlg.m_rowid = m_rowid;
	if( dlg.DoModal() == IDCANCEL ) return;
	createPeople();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnAscestor()
{
	CString father_id	= m_father_id;
	CString rowid		= m_rowid;
	
	if( father_id.IsEmpty() || father_id == L"0") return;

	while( !father_id.IsEmpty() )
	{
		m_command.Format( L"SELECT father_id FROM people WHERE rowid ='%s'", father_id );
		if( !query2( m_command ) ) return;
		str = m_recordset2->GetFieldString( 0 );
		if( str.IsEmpty() || str == L"0")
		{
			m_rowid = father_id;
			break;
		}
		father_id	= str;
	}
	createScreen();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDescendants()
{
	CGaDescendants dlg;
	dlg.m_numbering		= 0;
	dlg.m_code			= ANSI;
	dlg.m_rowid1		= m_rowid;
	dlg.m_source		= 0;
	dlg.m_name			= m_name;
	dlg.m_tableNumber	= m_tablenumber;
	if( dlg.DoModal() == IDCANCEL ) return;

	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnReset()
{
	m_rowid = m_rowidFirst;
	createScreen();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Apa nevét. adatait kell megadni 
// Ha az apának van már felesége, akkor  
void CRelatives::OnClickedStaticFather()
{
	CString father;
	CString father_id;
	CString mother_id;

	father = m_ListCtrlP.GetItemText( 0, L_NAME );
	if( !father.IsEmpty() )
	{
		if( AfxMessageBox( L"Cserélni akarod az apát?", MB_ICONQUESTION |MB_YESNO ) == IDNO ) 
			return;
	}

	CEditPeople dlg;
	dlg.m_rowid.Empty();
	dlg.m_last_name = m_lastname;
	if( dlg.DoModal() == IDCANCEL  ) return;

	father_id = dlg.m_rowid;
	m_command.Format( L"UPDATE people SET father_id = '%s' WHERE rowid='%s'", father_id, m_rowid );
	if( !theApp.execute( m_command ) ) return;
	createFather( father_id );

	mother_id = whichSpouse( father_id );
	if( !mother_id.IsEmpty() )
	{
		createMother( mother_id );
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnClickedStaticMother()
{
	CString mother;
	CString father;
	CString father_id;
	CString mother_id;

	mother = m_ListCtrlP.GetItemText( 1, L_NAME );
	if( !mother.IsEmpty() )
	{
		if( AfxMessageBox( L"Cserélni akarod az anyát?", MB_ICONQUESTION |MB_YESNO ) == IDNO ) 
			return;
	}
	CEditPeople dlg;
	dlg.m_rowid.Empty();
	if( dlg.DoModal() == IDCANCEL  ) return;
	mother_id = dlg.m_rowid;
	m_command.Format( L"UPDATE people SET mother_id = '%s' WHERE rowid='%s'", mother_id, m_rowid );
	if( !theApp.execute( m_command ) ) return;
	createMother( mother_id );

	father		= m_ListCtrlP.GetItemText( 0, L_NAME ); 
	mother		= m_ListCtrlP.GetItemText( 1, L_NAME ); 
	father_id	= m_ListCtrlP.GetItemText( 0, L_ROWID ); 
	if( father_id.IsEmpty()) return;
	marry( father_id, mother_id, father, mother );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnClickedStaticMarriages()
{
	CString rowid;
	CString nameF;
	CString nameM;
	CEditPeople dlg;
	dlg.m_rowid.Empty();
	if( dlg.DoModal() == IDCANCEL  ) return;
	rowid = dlg.m_rowid;
	nameM = dlg.m_name;
	nameF = ( m_name );
	if( m_sexid == L"1" )
		marry( m_rowid, rowid, m_name, nameM );
	else
		marry( rowid, m_rowid, nameM, m_name );
	createSpouses();
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnClickedStaticChildren()
{
	AfxMessageBox( L"Gyereket a jobb egérgombbal a 'házastárs'-ra kattintva és a 'Gyerek' menüpontot választva lehet bevinni" );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnClickedStaticSiblings()
{
	AfxMessageBox( L"A testvéreket a szülõk gyermekeinként lehet megadni!" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// father_id-t és mother_id-t házasságra kényszeríti, ha még nem voltak házasok 
void CRelatives::marry( CString father_id, CString mother_id, CString nameF, CString nameM )
{
	m_command.Format( L"SELECT rowid FROM marriages WHERE spouse1_id='%s' AND spouse2_id='%s'", father_id, mother_id );
	if( !query3( m_command ) ) return;
	
	if( !m_recordset3->RecordsCount( ) )
	{
		m_command.Format( L"INSERT INTO marriages (spouse1_id,spouse2_id) VALUES ('%s','%s')", father_id, mother_id );
		if( !theApp.execute( m_command ) ) return;
	}

	CEditMarriage dlg;
	dlg.m_rowid.Empty();
	dlg.m_rowidH = father_id;
	dlg.m_rowidW = mother_id;
	if( dlg.DoModal() == IDCANCEL ) return;



//	str.Format( L"%s és %s házasságot kötött!", nameF, nameM );
//	AfxMessageBox( str );

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visszadja az anya rowid-ját, vagyis az apa feleségének rowid-ját.
// Ha több felesége volt, akkor megkérdazi, hogy melyik az anyja és azt adja vissza
CString CRelatives::whichSpouse( CString father_id )
{
	CString	mother_id(L"");
	int		count;
	m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id = '%s'", father_id );
	if( !query2( m_command ) ) return L""; 
	count =  m_recordset2->RecordsCount();
	switch( count )
	{
	case 0:
		break;
	case 1:
		mother_id = m_recordset2->GetFieldString( 0 );
		break;
	default:
		CSelectFromPeople dlg;
		dlg.m_rowid = mother_id;
		if( dlg.DoModal() == IDCANCEL ) return(L"");
		mother_id = dlg.m_rowidSel;
		break;
	}
	return mother_id;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnInsertGyerek()
{
	int nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED);
	CString mother_id =  m_ListCtrlM.GetItemText( nItem, L_ROWID );
	CString nameS =  m_ListCtrlM.GetItemText( nItem, L_NAME );
	CEditPeople dlg;
	dlg.m_rowid.Empty();
	if( m_sexid == L"1" )
		dlg.m_last_name = m_lastname;
	else
		dlg.m_last_name = getFirstWord( nameS );
	if( dlg.DoModal() == IDCANCEL  ) return;

	m_command.Format( L"UPDATE people SET father_id='%s', mother_id='%s', parentindex = '%d' WHERE rowid = '%s'", m_rowid, mother_id, nItem + 1, dlg.m_rowid ); 
	if( !theApp.execute( m_command ) ) return;
	createScreen();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnEditMarriage()
{
	int nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED);
	CString spouse_id =  m_ListCtrlM.GetItemText( nItem, L_ROWID );
	CString rowid;

		
	if( m_sexid == L"1" )
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE spouse1_id ='%s' AND spouse2_id='%s'", m_rowid, spouse_id);
	else
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE spouse1_id ='%s' AND spouse2_id='%s'", spouse_id, m_rowid );

	if( !theApp.query( m_command ) ) return;
	rowid = theApp.m_recordset->GetFieldString( MARRIAGES_ROWID );
	if( rowid.IsEmpty() )
	{
		AfxMessageBox( L"Nincs házasság!" );
		return;
	}
	CEditMarriage dlg;
	dlg.m_rowid = rowid;
	if( dlg.DoModal() == IDCANCEL ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnInfo()
{
 CString info = L"\
Az adatbázisban már létezõ ember családi kapcsolatait hozhatjuk létre ezen az oldalon.\n\
A rokonok már létezhetnek az adatbázisban de itt is létrehozhatjuk a bejegyzéseiket\
";

 AfxMessageBox( info, MB_ICONINFORMATION );

}
