// SelectOne.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "SelectOne.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
	LM_EMPTY = 0,
	LM_ROWID,
	LM_VALUE,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSelectOne dialog
CSelectOne::CSelectOne(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectOne::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectOne)
	m_filter = L""; 
	m_search = L""; 
	m_recordset = new CSqliteDBRecordSet;
	m_dbfile = 1;
	//}}AFX_DATA_INIT
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectOne::~CSelectOne()
{
	delete m_recordset;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectOne)
	DDX_Text(pDX, IDC_SEARCH, m_search);
	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SELECTONE, m_ListCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSelectOne, CDialog)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)// az oszlopszint beallito fuggveny hozzarendelese az uzenethez
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)//a cella megvaltozasat jelento fv. hozzarendelese az uzenethez
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSelectOne::OnClickedButtonClear)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CSelectOne::OnClickedKeress)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSelectOne::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
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
LRESULT CSelectOne::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSelectOne message handlers
BOOL CSelectOne::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowText(m_caption);


	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);	
	m_ListCtrl.InsertColumn( LM_EMPTY,L"",LVCFMT_RIGHT, 40, -1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( LM_ROWID,L"",LVCFMT_RIGHT, 40, -1,COL_NUM );
	m_ListCtrl.InsertColumn( LM_VALUE,		L"",LVCFMT_LEFT, 295, -1,COL_TEXT);
	
	m_orderix = 2;

	fillTable();
	
	
	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetSortOrder(1,m_orderix+1);	
	for( int i=0;i<m_ListCtrl.GetItemCount();++i)
	{
		if( m_ListCtrl.GetItemText(i,LM_VALUE) == m_selected )		// a kurzort a kiválasztott könyvhöz tartozó étékre állítja
		{
			m_ListCtrl.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			m_ListCtrl.EnsureVisible(i,FALSE);
			break;
		}
	}
	
	if( m_selected.IsEmpty() )
		GetDlgItem(IDC_SEARCH )->SetFocus();
	else
		GetDlgItem(IDC_SELECTONE )->SetFocus();

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectOne::fillTable() 
{
	UINT	i;
	int		ix;
	m_ListCtrl.DeleteAllItems();

		if( m_select_id.IsEmpty() )
			m_select_id = L"rowid";

	if( m_filter.IsEmpty() )
		m_command.Format(L"SELECT %s,%s FROM %s", m_select_id, m_column,m_table);
	else
		m_command.Format(L"SELECT %s,%s FROM %s %s",m_select_id, m_column,m_table,m_filter);

	if( m_dbfile == 1 )
	{
		if( !query( m_command ) ) return;
		for(i=0;i<m_recordset->RecordsCount();++i,m_recordset->MoveNext())
		{
			ix = m_ListCtrl.InsertItem( i,(LPTSTR) L" " );
			m_ListCtrl.SetItemText( ix, LM_ROWID, (LPTSTR)m_recordset->GetFieldString(0) );
			m_ListCtrl.SetItemText( ix, LM_VALUE, (LPTSTR)m_recordset->GetFieldString(1) );
		}
	}
	else
	{
		if( !theApp.querySystem( m_command ) ) return;
		for(i=0;i< theApp.m_recordsetSystem->RecordsCount();++i, theApp.m_recordsetSystem->MoveNext())
		{
			ix = m_ListCtrl.InsertItem( i,(LPTSTR) L" " );
			m_ListCtrl.SetItemText( ix, LM_ROWID, (LPTSTR)theApp.m_recordsetSystem->GetFieldString(0) );
			m_ListCtrl.SetItemText( ix, LM_VALUE, (LPTSTR)theApp.m_recordsetSystem->GetFieldString(1) );
		}
	}
	m_ListCtrl.SetSortOrder( (int)m_orderix );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectOne::OnClickedButtonClear()
{
	m_selected.Empty();
	m_id.Empty();
	CDialog::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectOne::query( CString command )
{
	CSqliteDB*	sqliteDB;

	sqliteDB = theApp.mainDB;

	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectOne::OnOK() 
{
	int	n = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	if( n < 0 )
	{
		AfxMessageBox(L"Nincs kijelölés!"); 
		return;
	}
	m_selected	= m_ListCtrl.GetItemText(n,LM_VALUE);
	m_id		= m_ListCtrl.GetItemText(n,LM_ROWID);
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSelectOne::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}	return CDialog::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////4
void CSelectOne::OnClickedKeress()
{
	keress( 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSelectOne::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


