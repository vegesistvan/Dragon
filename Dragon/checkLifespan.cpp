// LifespanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "checkLifespan.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "checkParam.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "editHtmlLine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckLifespan()
//{
//	CLifeSpan dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	L_CNT,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_LIFESPAN,
};

// adatbázis oszlopok
enum
{
	D_ROWID = 0,
	D_LINENUMBER,
	D_TABLENUMBER,
	D_GENERATION,
	D_SOURCE,
	D_UNITED,
	D_LAST_NAME,
	D_FIRST_NAME,
	D_BIRTH_DATE,
	D_DEATH_DATE,
};


IMPLEMENT_DYNAMIC(CLifeSpan, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifeSpan::CLifeSpan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLifeSpan::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifeSpan::~CLifeSpan()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLifeSpan, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CLifeSpan::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CLifeSpan::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CLifeSpan::OnDbEdit)
	ON_COMMAND(ID_LIST, &CLifeSpan::OnList)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifeSpan::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW( _caption );


	CCheckParam dlg;

	dlg.m_title		= L"Azokat az embereket listázzuk, akiknek az életkora nagyobb az elõírtnál vagy kisebb mint 0.";
	dlg.m_function1 = L"Írd elõ a kort!";
	dlg.m_value		= 100;

	if( dlg.DoModal() == IDCANCEL )
	{
		CDialog::OnCancel();
	}

	_lifespan = dlg.m_value;

	str.Format( L"%d évnél idõsebb emeberek, és akiknek a halálozási dátuma korábban van, mint a születési dátumouk.", _lifespan );
	SetWindowText( str );

	createColumns();
	fillTable();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::createColumns()
{
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LIFESPAN,	L"élettartam",	LVCFMT_LEFT,	100,-1,COL_NUM);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::fillTable()
{

	CString fields = L"\
rowid,\
linenumber,\
tablenumber,\
generation,\
source,\
united,\
last_name,\
first_name,\
birth_date,\
death_date\
";


	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString	generation;
	CString united;
	CString source;
	CString name;
	CString birth_date;
	CString death_date;

	int birth;
	int death;
	int lifespan;
	int cnt = 0;
	int	diff;
	int	nItem = 0;

	CProgressWnd wndProgress(NULL,L"Az emberek életkorának ellenõrzése ..." ); 
	wndProgress.GoModal();

	m_command.Format( L"SELECT %s FROM people WHERE length(birth_date)!=0 AND length(death_date)!=0 ORDER BY last_name, first_name", fields );
	if( !query( m_command ) ) return;

	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{

		birth_date = m_recordset->GetFieldString( D_BIRTH_DATE );
		birth = getYearFromDate( birth_date );
		death_date = m_recordset->GetFieldString( D_DEATH_DATE );
		death = getYearFromDate( death_date );
		if( birth == 0 || !checkDate( birth_date ) || death == 0 || !checkDate( death_date ) )
			continue;

		lifespan = death - birth;
		diff = lifespan - _lifespan;

		rowid		= m_recordset->GetFieldString( D_ROWID );
		lineNumber	= m_recordset->GetFieldString( D_LINENUMBER );
		tableNumber	= m_recordset->GetFieldString( D_TABLENUMBER );
		generation	= m_recordset->GetFieldString( D_GENERATION );
		source		= m_recordset->GetFieldString( D_SOURCE );
		united		= m_recordset->GetFieldString( D_UNITED );
		name.Format( L"%s %s", m_recordset->GetFieldString( D_LAST_NAME ), m_recordset->GetFieldString( D_FIRST_NAME )  );

		if( diff > 0 || lifespan < 0)
		{
			str.Format( L"%d", nItem + 1 );
			nItem = m_ListCtrl.InsertItem( nItem, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
			m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, tableNumber );
			m_ListCtrl.SetItemText( nItem, L_UNITED, united );
			m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
			m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
			m_ListCtrl.SetItemText( nItem, L_NAME, name );
			m_ListCtrl.SetItemText( nItem, L_BIRTH, birth_date );
			m_ListCtrl.SetItemText( nItem, L_DEATH, death_date );	
			str.Format( L"%d", lifespan );
			m_ListCtrl.SetItemText( nItem, L_LIFESPAN, str );
			++nItem;
			++cnt;
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( !cnt )
	{
		str.Format( L"Minden ember élettartama kisebb mint %d év.", _lifespan );
		AfxMessageBox( str );
		CDialog::OnCancel();
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::OnList()
{
	CString	logFile(L"lifespan"); 
	CString	title(L"Emberek élettartama");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifeSpan::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifeSpan::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifeSpan::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CLifeSpan::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CEditHtmlLine dlg;
	dlg.m_linenumber	= m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifeSpan::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}