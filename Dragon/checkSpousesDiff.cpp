// CheckSpouseDiff.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "CheckSpousesDiff.h"
#include "afxdialogex.h"
#include "CheckParam.h"
#include "html_Lines.h"
#include "Relations.h"

// CCheckSpousesDiff dialog

// ListCtrl oszlopok
enum
{
	L_CNT = 0,
	L_ROWID,
	L_LINENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_BIRTH,
	L_DIFF,
	L_MARRIAGE,
	L_AGE,
	L_DEATH
};
// SELECT oszlopok
enum
{
	S_LINENUMBER = 0,
	S_GENERATION,
	S_SOURCE,
	S_UNITED,
	S_LAST_NAME,
	S_FIRST_NAME,
	S_BIRTH,
	S_DEATH,
};
IMPLEMENT_DYNAMIC(CCheckSpousesDiff, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpousesDiff::CCheckSpousesDiff(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckSpousesDiff::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpousesDiff::~CCheckSpousesDiff()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckSpousesDiff, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckSpousesDiff::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CCheckSpousesDiff::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckSpousesDiff::OnHtmlNotepad)
	ON_COMMAND(ID_ROKONSAG, &CCheckSpousesDiff::OnRokonsag)
	ON_COMMAND(ID_GAHTML_LINE, &CCheckSpousesDiff::OnGahtmlLine)

	ON_COMMAND(ID_LIST, &CCheckSpousesDiff::OnList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSpousesDiff::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );

	EASYSIZE_INIT();
	
	CCheckParam dlg;
	dlg.m_title = L"Házastársak korkülönbségének ellenõrzése";
	dlg.m_function1 = L"A program listázza azokat a házaspárokat, akiknek korkülönbsége nagyobb mint egy adott szám.";
	dlg.m_function2 = L"Add meg a korkülönbséget években!";
	dlg.m_value	= 40;

	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return false;
	}

	m_diff = dlg.m_value;
	
	m_title.Format( L"Házastársak, akiknek korkülönbsége nagyobb mint %d év", m_diff );
	SetWindowTextW( m_title );

	createColumns();
	spousesDiff();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckSpousesDiff::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DIFF,		L"különbség",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MARRIAGE,	L"házasság",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_AGE,			L"életkor ",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::spousesDiff()
{
	CProgressWnd wndP(NULL,L"Házastársak korkülönbségének ellenõrzése ..." ); 
	wndP.GoModal();

#ifndef _DEBUG
	wndP.SetText( L"Házasságok lekérdezées...");
#endif


	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !theApp.query( m_command ) ) return;


#ifndef _DEBUG
	wndP.SetText( L"Házastársak korkülönbségének meghatározása..");
#endif

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	CString marriage;
	int		m;
	CString age1;
//	int		a1;
	CString age2;
//	int		a2;

	CString cntS;
	CString rowid1;
	CString rowid2;
	CString birth1;
	CString birth2;
	CString death1;
	CString death2;

	int		b1;
	int		b2;
	CString name1;
	CString name2;
	CString lineNumber1;
	CString lineNumber2;
	CString	source1;
	CString source2;
	CString united1;
	CString united2;
	CString generation1;
	CString generation2;
	CString diffS;

	int		diff = 0;
	int		cnt = 0;
	int		nItem = 0;


	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid1		= theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		rowid2		= theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		marriage	= theApp.m_recordset->GetFieldString( MARRIAGES_DATE );
		m			= theApp.getYearFromDate( marriage );
		if( !rowid1.IsEmpty() && !rowid2.IsEmpty() && checkDate( marriage ) )
		{
			m_command.Format( L"SELECT lineNumber, generation, source, united, last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", rowid1 );
			if( !theApp.query1( m_command ) ) return;
			birth1 = theApp.m_recordset1->GetFieldString( S_BIRTH );
			if( !birth1.IsEmpty() && checkDate( birth1 ))
			{
				b1	= theApp.getYearFromDate( birth1 );
				m_command.Format( L"SELECT lineNumber, generation, source, united, last_name, first_name, birth_date,death_date FROM people WHERE rowid = '%s'", rowid2 );
				if( !theApp.query2( m_command ) ) return;
	
				birth2		= theApp.m_recordset2->GetFieldString( S_BIRTH );
				if( !birth2.IsEmpty() && checkDate( birth2 ) )
				{
					b2	= theApp.getYearFromDate( birth2 );
					diff = abs( b1-b2 );
					if( diff > m_diff )
					{
						if( m != 0 )
						{
							age1.Format( L"%d", m - b1);
							age2.Format( L"%d", m - b2);
						}
						else
						{
							age1.Empty();
							age2.Empty();
						}

						diffS.Format( L"%d", diff );
						lineNumber2	= theApp.m_recordset2->GetFieldString( S_LINENUMBER );
						generation2	= theApp.m_recordset2->GetFieldString( S_GENERATION );
						source2		= theApp.m_recordset2->GetFieldString( S_SOURCE );
						united2		= theApp.m_recordset2->GetFieldString( S_UNITED );
						death2		= theApp.m_recordset2->GetFieldString( S_DEATH );
						name2.Format( L"%s %s",theApp. m_recordset2->GetFieldString( S_LAST_NAME ), theApp.m_recordset2->GetFieldString( S_FIRST_NAME ) );

						lineNumber1	= theApp.m_recordset1->GetFieldString( S_LINENUMBER );
						generation1	= theApp.m_recordset1->GetFieldString( S_GENERATION );
						source1		= theApp.m_recordset1->GetFieldString( S_SOURCE );
						united1		= theApp.m_recordset1->GetFieldString( S_UNITED );
						death1		= theApp.m_recordset1->GetFieldString( S_DEATH );
						name1.Format( L"%s %s",theApp. m_recordset1->GetFieldString( S_LAST_NAME), theApp.m_recordset1->GetFieldString( S_FIRST_NAME ) );

						cntS.Format( L"%d", cnt+1 ); 
						nItem = m_ListCtrl.InsertItem( nItem, cntS );
						m_ListCtrl.SetItemText( nItem, L_ROWID, rowid1 );
						m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber1 );
						m_ListCtrl.SetItemText( nItem, L_GENERATION, generation1 );
						m_ListCtrl.SetItemText( nItem, L_SOURCE, source1 );
						m_ListCtrl.SetItemText( nItem, L_UNITED, united1 );
						m_ListCtrl.SetItemText( nItem, L_NAME, name1 );
						m_ListCtrl.SetItemText( nItem, L_BIRTH, birth1 );
						m_ListCtrl.SetItemText( nItem, L_DIFF, diffS );
						m_ListCtrl.SetItemText( nItem, L_MARRIAGE, marriage );
						m_ListCtrl.SetItemText( nItem, L_AGE, age1 );
						m_ListCtrl.SetItemText( nItem, L_DEATH, death1 );

						++nItem;
						nItem = m_ListCtrl.InsertItem( nItem, cntS );
						m_ListCtrl.SetItemText( nItem, L_ROWID, rowid2 );
						m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber2 );
						m_ListCtrl.SetItemText( nItem, L_GENERATION, generation2 );
						m_ListCtrl.SetItemText( nItem, L_SOURCE, source2 );
						m_ListCtrl.SetItemText( nItem, L_UNITED, united2 );
						m_ListCtrl.SetItemText( nItem, L_NAME, name2 );
						m_ListCtrl.SetItemText( nItem, L_BIRTH, birth2 );
						m_ListCtrl.SetItemText( nItem, L_AGE, age2 );
						m_ListCtrl.SetItemText( nItem, L_DEATH, death2 );

						++nItem;
						nItem = m_ListCtrl.InsertItem( nItem, L"");
						++nItem;
						++cnt;
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

	if( !cnt )
	{
		str.Format( L"\n\nMinden házastárs korkülönbsége kisebb mint %d év\n", m_diff );
		AfxMessageBox( str );
		CDialogEx::OnOK();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckSpousesDiff:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_SHOWS, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, L_NAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, L_NAME );
			++cnt;
		}
	

	}

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnList()
{
	CString	logFile(L"spousesdiff"); 
	
	theApp.exportAll( logFile, m_title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnGahtmlLine()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if( nItem == - 1 )
	{
		theApp.message( L"Házastársak korkülönbsége", L"Nincs kijelölve ember!" );
		return;
	}
	

	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );

	theApp.listHtmlLine( lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesDiff::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}

