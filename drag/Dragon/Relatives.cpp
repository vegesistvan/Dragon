// Relatives.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Relatives.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "EditPeople.h"
#include "EditMarriage.h"
#include "ascendants.h"
#include "SelectFromPeople.h"
#include "descendants.h"

enum
{
	ROWID = 0,
	TABLENUMBER,
	LINENUMBER,
	SOURCE,
	SEXID,
	LASTNAME,
	FIRSTNAME,
	BIRTHPLACE,
	BIRTHDATE,
	DEATHPLACE,
	DEATHDATE,
	PROFESSION,
	COMMENT,
	FATHERID,
	MOTHERID,
	PARENTINDEX,
	PEER,
	WHICHCHILDFATHER,
	WHICHCHILDMOTHER,
	FATHERWHICHHUSBAND,
	MOTHERWHICHWIFE,
	COLUMNSCOUNT
};



IMPLEMENT_DYNAMIC(CRelatives, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRelatives::CRelatives(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRelatives::IDD, pParent)
{
	m_columns = L"rowid, tablenumber, linenumber, source, sex_id, last_name, first_name, \
birth_place, birth_date, death_place, death_date, occupation, comment, father_id, mother_id, parentIndex, peer, whichChildFather, whichChildMother,whichHusband,whichWife";

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

	DDX_Control(pDX, IDC_STATIC_L, colorLine);
	DDX_Control(pDX, IDC_STATIC_R, colorRowid);
}
BEGIN_MESSAGE_MAP(CRelatives, CDialogEx)
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
	ON_COMMAND(ID_HTML_NOTEPAD, &CRelatives::OnHtmlNotepad)
	ON_COMMAND(ID_ASCENDANTS, &CRelatives::OnAscendants)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// people lekérdezett oszlopai
	

	colorPeople.SetTextColor( RGB( 255,51,255 ) );
	colorPeople.SetTextColor( RGB( 255,0,0 ) );

	colorFather.SetTextColor( theApp.m_colorClick );
	colorMother.SetTextColor( theApp.m_colorClick );
	colorMarriages.SetTextColor( theApp.m_colorClick );
	colorChildren.SetTextColor( theApp.m_colorClick );
	colorSiblings.SetTextColor( theApp.m_colorClick );

	colorRowid.SetBkColor(YELLOW);
	colorLine.SetBkColor(YELLOW);

	createColumns(&m_ListCtrlP);	// szülõk
	createColumns( &m_ListCtrlC );	// gyerek
	createColumns(&m_ListCtrlS);  // testvér


	m_ListCtrlM.SetExtendedStyle(m_ListCtrlM.GetExtendedStyle() | LVS_EX_GRIDLINES );
	m_ListCtrlM.InsertColumn(LM_ORDER, L"#", LVCFMT_RIGHT, 30, -1, COL_TEXT);
	m_ListCtrlM.InsertColumn(LM_LINENUMBER, L"line", LVCFMT_RIGHT, 65, -1, COL_TEXT);
	m_ListCtrlM.InsertColumn(LM_ROWID, L"rowid", LVCFMT_RIGHT, 65, -1, COL_TEXT);
	m_ListCtrlM.InsertColumn(LM_PEOPLE, L"házastárs", LVCFMT_LEFT, 700, -1, COL_TEXT);
	m_ListCtrlM.InsertColumn(LM_DATE, L"házasság", LVCFMT_LEFT, 80, -1, COL_TEXT);

	m_rowidFirst = m_rowid;
	m_rowidP = m_rowid;

	// sajnos nem mûködik owner drawn CListCtrl-ban!!!
//	m_ListCtrl->SetItemState(m_nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (!m_rowidP.IsEmpty())
	{
		if (!createScreen())
		{
			CDialog::OnCancel();
			return false;
		}
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::createColumns( CListCtrlEx* p_ListCtrl )
{
	p_ListCtrl->SetExtendedStyle(p_ListCtrl->GetExtendedStyle() | LVS_EX_GRIDLINES );

	p_ListCtrl->InsertColumn(L_PARENT_INDEX, L"#", LVCFMT_RIGHT, 30, -1, COL_NUM);
	p_ListCtrl->InsertColumn(L_LINENUMBER,		L"line",		LVCFMT_RIGHT, 65,-1,COL_NUM);
	p_ListCtrl->InsertColumn( L_ROWID,			L"rowid",		LVCFMT_RIGHT,	 65,-1,COL_TEXT);
	p_ListCtrl->InsertColumn( L_PEOPLE,			L"people",	LVCFMT_LEFT,   780,-1,COL_TEXT );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createScreen()
{
	if (!createPeople())
		return false;

	m_ListCtrlP.DeleteAllItems();
	createFather( m_father_idP );
	createMother( m_mother_idP );
	createSpouses();
	createChildren();
	createSiblings();

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createPeople()
{
	CString people;
	people = selectPeople( m_rowidP );
	if (people.IsEmpty())
	{

		str.Format(L"%s rowid bejegyzés nincs az adatbázisban!", m_rowidP);
		AfxMessageBox(str, MB_ICONEXCLAMATION);
		return false;	
	}

	m_father_idP	= m_father_id;
	m_mother_idP	= m_mother_id;
	m_sexidP		= m_sexid;

	int isource		= _wtoi(m_recordset->GetFieldString(SOURCE));


	if (theApp.m_inputMode == GAHTML)
		m_name.Format(L"%s %s (%s)", m_lastname, m_firstname, source[isource-1] );
	else
		m_name.Format( L"%s %s", m_lastname, m_firstname );

	str.Format( L"%s családi kapcsolatainak ábrázolása és szerkesztése", m_name ); 
	SetWindowTextW( str );

	GetDlgItem( IDC_STATIC_LINE )->SetWindowTextW(m_linenumber);
	GetDlgItem( IDC_STATIC_ROWID )->SetWindowTextW(m_rowidP);
	GetDlgItem( IDC_STATIC_PEOPLE )->SetWindowTextW( people );
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createFather( CString father_id )
{
	int id = _wtoi( father_id );
	if( id > 1000000 ) return true;
	if( !id ) return true;

	CString people;

	m_ListCtrlP.DeleteAllItems();
	people = selectPeople( father_id );
	
//	m_ListCtrlP.SetColumnVisibility( L_PARENT_INDEX, false );

	if( !m_ListCtrlP.GetItemCount() )
		m_ListCtrlP.InsertItem( 0, L"");
	else
		m_ListCtrlP.SetItemText( 0, L_PARENT_INDEX, L"");
	m_ListCtrlP.SetItemText(0, L_LINENUMBER, m_linenumber);
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
		m_ListCtrlP.InsertItem( 1, L"");
	else
		m_ListCtrlP.SetItemText( 1, L_PARENT_INDEX, L"");
	m_ListCtrlP.SetItemText(1, L_LINENUMBER, m_linenumber);
	m_ListCtrlP.SetItemText( 1, L_ROWID, mother_id );
	m_ListCtrlP.SetItemText( 1, L_PEOPLE, people );

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelatives::createSpouses()
{
	UINT	i;
	CString	rowid;
	CString date;
	CString	people;
	CString which;

	m_ListCtrlM.DeleteAllItems();




	if( m_sexidP == MAN )
		m_command.Format( L"SELECT wife_id, whichWife, date FROM marriages WHERE husband_id = '%s' ORDER BY whichWife", m_rowidP );
	else
		m_command.Format( L"SELECT husband_id, whichHusband, date FROM marriages WHERE wife_id = '%s' ORDER BY whichHusband", m_rowidP );

	if( !query2( m_command ) ) return false;

	m_numOfSpouses = m_recordset2->RecordsCount();


//	if( m_numOfSpouses > 1 )
//	{
//		m_ListCtrlM.SetColumnVisibility( L_PARENT_INDEX, true );
//		m_ListCtrlM.SetColumnWidth( L_PARENT_INDEX, 20 );
//	}
//	else
//		m_ListCtrlM.SetColumnVisibility( L_PARENT_INDEX, false );
	

	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{ 
		rowid	= m_recordset2->GetFieldString( 0 );
		which	= m_recordset2->GetFieldString( 1 );
		if (which.IsEmpty())
			which = L"1";
		date = m_recordset2->GetFieldString(2);

		people	= selectPeople( rowid );
		m_ListCtrlM.InsertItem( i, which );
		m_ListCtrlM.SetItemText(i, LM_LINENUMBER, m_linenumber);
		m_ListCtrlM.SetItemText(i, LM_ROWID, rowid);
		m_ListCtrlM.SetItemText(i, LM_PEOPLE, people );
		m_ListCtrlM.SetItemText(i, LM_DATE, date);
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

	if( m_sexidP == MAN )
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s' ORDER BY parentIndex, birth_date", m_rowidP );
	else
		m_command.Format( L"SELECT rowid FROM people WHERE mother_id = '%s' ORDER BY parentIndex,birth_date", m_rowidP );
	if( !query2( m_command ) ) return false;

/*
	if( m_numOfSpouses > 1 )
	{
		m_ListCtrlC.SetColumnVisibility( L_PARENT_INDEX, true );
		m_ListCtrlC.SetColumnWidth( L_PARENT_INDEX, 20 );
	}
	else
		m_ListCtrlC.SetColumnVisibility( L_PARENT_INDEX, false );
*/
	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{
		rowid	= m_recordset2->GetFieldString( 0 );
		people	= selectPeople( rowid );

//		m_ListCtrlC.InsertItem( i, m_parent_index );


		if( m_sexidP == MAN )
			m_ListCtrlC.InsertItem(i, m_motherWhichWife );
		else
			m_ListCtrlC.InsertItem(i, m_fatherWhichHusband);


		m_ListCtrlC.SetItemText(i, L_LINENUMBER, m_linenumber);
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
	CString which;


	m_ListCtrlS.DeleteAllItems();
	
	if( m_sexidP == MAN )
	{
		if( m_father_idP == L"0" || m_father_idP.IsEmpty() ) return true;
		m_command.Format( L"SELECT rowid FROM people WHERE father_id = '%s'", m_father_idP );
	}
	else
	{
		if( m_mother_idP == L"0" || m_mother_idP.IsEmpty() ) return true;
		m_command.Format( L"SELECT rowid FROM people WHERE mother_id = '%s'", m_mother_idP );
	}
	if( !query2( m_command ) ) return false;




	for( i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{
		rowid = m_recordset2->GetFieldString( 0 );
		if( rowid != m_rowidP )
		{
			people	= selectPeople( rowid );
			which.Format(L"%d", nItem+1 );

	//		which = m_recordset2->GetFieldString(0);

			m_ListCtrlS.InsertItem( nItem, which );
			m_ListCtrlS.SetItemText(nItem, L_LINENUMBER, m_linenumber);
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
/*
	CString tablenumber;
	CString sexid;
	CString lastname;
	CString firstname;
	CString birthplace;
	CString birthdate;
	CString deathplace;
	CString deathdate;

	
	CString father_id;
	CString mother_id;
*/

	CString comment;
	CString peer;
	CString profession;
	CString birth;
	CString death;

	

	m_command.Format( L"SELECT %s FROM people WHERE rowid = '%s'", m_columns, rowid );
	if( !query( m_command ) ) return L"";
//	m_tablenumber	= m_recordset->GetFieldString( TABLENUMBER );
	m_sexid		= m_recordset->GetFieldString( SEXID );
	m_lastname	= m_recordset->GetFieldString( LASTNAME );
	m_firstname	= m_recordset->GetFieldString( FIRSTNAME );
	m_birthplace	= m_recordset->GetFieldString( BIRTHPLACE );
	m_birthdate	= m_recordset->GetFieldString( BIRTHDATE );
	m_deathplace	= m_recordset->GetFieldString( DEATHPLACE );
	m_deathdate	= m_recordset->GetFieldString( DEATHDATE );
	profession	= m_recordset->GetFieldString( PROFESSION );
	comment		= m_recordset->GetFieldString( COMMENT );
	m_father_id	= m_recordset->GetFieldString( FATHERID );
	m_mother_id	= m_recordset->GetFieldString( MOTHERID );
	m_parent_index	= m_recordset->GetFieldString( PARENTINDEX );
	peer		= m_recordset->GetFieldString(PEER);


	m_linenumber = m_recordset->GetFieldString(LINENUMBER);
	m_whichChildFather = m_recordset->GetFieldString(WHICHCHILDFATHER);
	m_whichChildMother = m_recordset->GetFieldString(WHICHCHILDMOTHER);
	m_fatherWhichHusband = m_recordset->GetFieldString(FATHERWHICHHUSBAND);
	m_motherWhichWife = m_recordset->GetFieldString(MOTHERWHICHWIFE);

	birth = pack( L"*", m_birthplace, m_birthdate );
	death = pack( L"+", m_deathplace, m_deathdate );
	
	str.Format( L"%s %s %s", m_lastname, m_firstname, peer );
	str.TrimRight();
	str.Format( L" %s %s", (CString)str, birth );
	str.TrimRight() ;
	str.Format( L"%s %s", (CString)str, death );
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, comment ); 
	str.TrimRight();
	str.Format( L"%s %s", (CString)str, profession ); 
	str.TrimRight();

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
void CRelatives::OnDblclkListCtrlParents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	m_rowidP = m_ListCtrlP.GetItemText( nItem, L_ROWID );
	createScreen();
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlMarriages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	m_rowidP = m_ListCtrlM.GetItemText( nItem, L_ROWID );
	createScreen();

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlChildren(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	m_rowidP = m_ListCtrlC.GetItemText( nItem, L_ROWID );
	createScreen();
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDblclkListCtrlSiblings(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	m_rowidP = m_ListCtrlS.GetItemText( nItem, L_ROWID );
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
	
	int nItem;

	bool nincs = true;
	m_listCtrlFlag = -1;

	GetDlgItem( IDC_LISTCTRL_PARENTS )->GetWindowRect( rectP );
	GetDlgItem( IDC_LISTCTRL_MARRIAGES )->GetWindowRect( rectM );
	GetDlgItem( IDC_LISTCTRL_CHILDREN )->GetWindowRect( rectC );
	GetDlgItem( IDC_LISTCTRL_SIBLINGS )->GetWindowRect( rectS );
	
	if( rectP.top <= point->y && point->y <= rectP.bottom )					// szülõk
	{
		if (!m_ListCtrlP.GetItemCount()) return false;
		if( (nItem = m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED)) == -1 ) return false; 
		m_linenumber = m_ListCtrlP.GetItemText(nItem, 1);
		m_listCtrlFlag = 1;
	}
	else if( rectM.top <= point->y && point->y <= rectM.bottom )			// házastárs
	{
		if (!m_ListCtrlM.GetItemCount()) return false;
		if( (nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED)) == -1 ) return false;
		m_linenumber = m_ListCtrlM.GetItemText(nItem, 1);
		m_listCtrlFlag = 2;
	}
	else if( rectC.top <= point->y && point->y <= rectC.bottom )			// gyerek
	{
		if (!m_ListCtrlC.GetItemCount()) return false;
		if( (nItem = m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED)) == -1 ) return false; 
		m_linenumber = m_ListCtrlC.GetItemText(nItem, 1);
		m_listCtrlFlag = 3;
	}
	else if( rectS.top <= point->y && point->y <= rectS.bottom )			// testvérek
	{
		if (!m_ListCtrlS.GetItemCount()) return false;
		if( (nItem = m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED)) == -1 ) return false; 
		m_linenumber = m_ListCtrlS.GetItemText(nItem, 1);
		m_listCtrlFlag = 4;
	}

	if(  m_listCtrlFlag != -1 )
	{
		if (theApp.m_inputMode == GAHTML)
		{
			if (Menu.LoadMenu(IDR_DROPDOWN_RELATION_HTML))
			{
				pPopup = Menu.GetSubMenu(0);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
			}
		}
		else
		{
			if (Menu.LoadMenu(IDR_DROPDOWN_EDIT))
			{
				pPopup = Menu.GetSubMenu(0);
				pPopup->EnableMenuItem(ID_EDIT_INSERT, MF_BYCOMMAND | MF_GRAYED);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);
			}
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
			m_command.Format( L"UPDATE people SET father_id ='%s' WHERE rowid ='%s'", rowid, m_rowidP );
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
	case 1:															// szülõk
		nItem = m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlP.GetItemText( nItem, L_ROWID );
		dlg.DoModal();

		if( !nItem )
			createFather( dlg.m_rowid );
		else
			createMother( dlg.m_rowid );
		break;
	case 2:															// házastársak
		nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlM.GetItemText( nItem, L_ROWID );
		dlg.DoModal();
		createSpouses();
		break;
	case 3:															// gyerek
		nItem = m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		dlg.m_rowid = m_ListCtrlC.GetItemText( nItem, L_ROWID );
		dlg.DoModal();
		createChildren();
		break;
	case 4:															// testvér
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
		father	= m_ListCtrlP.GetItemText( 0, L_PEOPLE );
		mother	= m_ListCtrlP.GetItemText( 1, L_PEOPLE );
		str.Format( L"Tényleg töröli alkarod a %s-%s szülõket?", father, mother );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteParents();
		break;
	case 2:
		nItem	= m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlM.GetItemText( nItem, L_ROWID );
		people = m_ListCtrlM.GetItemText(nItem, L_PEOPLE);
		
		str.Format( L"Tényleg töröli alkarod %s-t a házastársak közül?", people );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteSpouse( rowidS );
		break;
	case 3:
		nItem	= m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlC.GetItemText( nItem, L_ROWID );
		people = m_ListCtrlC.GetItemText( nItem, L_PEOPLE );
		
		str.Format( L"Tényleg töröli alkarod %s-t a gyerekek közül?", people );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteChild( rowidS );
		break;
	case 4:
		nItem	= m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED); 
		rowidS = m_ListCtrlS.GetItemText( nItem, L_ROWID );
		people = m_ListCtrlS.GetItemText( nItem, L_PEOPLE );

		str.Format( L"Tényleg töröli alkarod %s-t a testvérek közül?", people );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		deleteSibling( rowidS );
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteParents()
{
	m_command.Format( L"UPDATE people SET father_id = '', mother_id = '' WHERE rowid == '%s'", m_rowidP );
	if( !theApp.execute( m_command ) ) return;
	m_father_idP = L"";
	m_mother_idP = L"";
	
	m_ListCtrlP.DeleteItem( 0 );
	m_ListCtrlP.DeleteItem( 1 );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::deleteSpouse( CString rowid )
{
	if( m_sexidP == MAN )
		m_command.Format( L"DELETE FROM marriages WHERE husband_id == '%s' AND wife_id='%s'", m_rowidP, rowid  );
	else
		m_command.Format( L"DELETE FROM marriages WHERE husband_id == '%s' AND wife_id='%s'", rowid, m_rowidP  );
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
	dlg.m_rowid = m_rowidP;
	if( dlg.DoModal() == IDCANCEL ) return;
	createPeople();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CRelatives::OnClickedStaticPeople()
//{
//	AfxMessageBox(L"alma");
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnAscestor()
{
	CString father_id	= m_father_idP;
	
	if( father_id.IsEmpty() || father_id == L"0") return;

	while( !father_id.IsEmpty() )
	{
		m_command.Format( L"SELECT father_id FROM people WHERE rowid ='%s'", father_id );
		if( !query2( m_command ) ) return;
		str = m_recordset2->GetFieldString( 0 );
		if( str.IsEmpty() || str == L"0" || str.GetLength() == 7 )
		{
			m_rowid = father_id;
			break;
		}
		father_id	= str;
	}
	m_rowidP = father_id;
	createScreen();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnReset()
{
	m_rowidP = m_rowidFirst;
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

	father = m_ListCtrlP.GetItemText( 0, L_PEOPLE );
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
	m_command.Format( L"UPDATE people SET father_id = '%s' WHERE rowid='%s'", father_id, m_rowidP );
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
	CString parent;
	CString father_id;
	CString mother_id;

	parent = m_ListCtrlP.GetItemText( 1, L_PEOPLE );
	if( !parent.IsEmpty() )
	{
		if( AfxMessageBox( L"Cserélni akarod az anyát?", MB_ICONQUESTION |MB_YESNO ) == IDNO ) 
			return;
	}
	CEditPeople dlg;
	dlg.m_rowid.Empty();
	if( dlg.DoModal() == IDCANCEL  ) return;
	mother_id = dlg.m_rowid;
	m_command.Format( L"UPDATE people SET mother_id = '%s' WHERE rowid='%s'", mother_id, m_rowidP );
	if( !theApp.execute( m_command ) ) return;
	createMother( mother_id );

	father		= m_ListCtrlP.GetItemText( 0, L_PEOPLE ); 
	mother		= m_ListCtrlP.GetItemText( 1, L_PEOPLE ); 
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
	if( m_sexidP == MAN )
		marry( m_rowidP, rowid, m_name, nameM );
	else
		marry( rowid, m_rowidP, nameM, m_name );
	createSpouses();
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnClickedStaticChildren()
{
	str = L"Gyereket úgy lehet megadni, hogy a 'házastársai' listában a gyerek anyjára/apjára kattintunk a jobb egér gombbal \
 és a megjelenõ legördülû menübõl a 'Gyerek' funkciót választjuk! ";
	AfxMessageBox( str, MB_ICONINFORMATION );
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
	m_command.Format( L"SELECT rowid FROM marriages WHERE husband_id='%s' AND wife_id='%s'", father_id, mother_id );
	if( !query3( m_command ) ) return;
	
	if( !m_recordset3->RecordsCount( ) )
	{
		m_command.Format( L"INSERT INTO marriages (husband_id,wife_id) VALUES ('%s','%s')", father_id, mother_id );
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
	m_command.Format( L"SELECT wife_id FROM marriages WHERE husband_id = '%s'", father_id );
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
	CString nameS =  m_ListCtrlM.GetItemText( nItem, L_PEOPLE );
	CEditPeople dlg;
	dlg.m_rowid.Empty();
	if( m_sexidP == MAN )
		dlg.m_last_name = m_lastname;
	else
		dlg.m_last_name = getFirstWord( nameS );
	if( dlg.DoModal() == IDCANCEL  ) return;

	m_command.Format( L"UPDATE people SET father_id='%s', mother_id='%s', parentindex = '%d'WHERE rowid = '%s'", m_rowidP, mother_id, nItem + 1,dlg.m_rowid ); 
	if( !theApp.execute( m_command ) ) return;
	createScreen();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnEditMarriage()
{
	int nItem = m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED);
	CString spouse_id =  m_ListCtrlM.GetItemText( nItem, L_ROWID );
	CString rowid;

		
	if( m_sexidP == MAN )
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id ='%s' AND wife_id='%s'", m_rowidP, spouse_id);
	else
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id ='%s' AND wife_id='%s'", spouse_id, m_rowidP );

	if( !theApp.query( m_command ) ) return;
	rowid = theApp.m_recordset->GetFieldString( DBM_ROWID );
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
////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnHtmlNotepad()
{
	theApp.editNotepad(theApp.m_htmlPathName, m_linenumber);
}
////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnAscendants()
{
	CAscendants dlg;
	dlg.m_rowid = m_rowidP;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDescendants()
{
	int nItem;

	theApp.v_rowid.clear();
	theApp.v_rowid.push_back(m_rowid);

	theApp.v_tableNumbers.clear();
	theApp.v_tableNumbers.push_back(L"1");

	CDescendants desc;
	desc.start();
}

