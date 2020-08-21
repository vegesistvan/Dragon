// CheckParentChild.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckParentChild.h"
#include "afxdialogex.h"
#include "CheckParam.h"
#include "html_Lines.h"
#include "Relations.h"


// CCheckParentChild dialog

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
	L_DEATH
};
// SELECT oszlopok
enum
{
	S_ROWID =0,
	S_LINENUMBER,
	S_GENERATION,
	S_SOURCE,
	S_UNITED,
	S_LAST_NAME,
	S_FIRST_NAME,
	S_BIRTH,
	S_DEATH,
	S_PARENT_ID,
};


IMPLEMENT_DYNAMIC(CCheckParentChild, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParentChild::CCheckParentChild(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckParentChild::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckParentChild::~CCheckParentChild()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParentChild, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckParentChild::OnHtmlEdit)
	ON_COMMAND(ID_HTML_SHOWS, &CCheckParentChild::OnHtmlShows)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckParentChild::OnHtmlNotepad)
	ON_COMMAND(ID_ROKONSAG, &CCheckParentChild::OnRokonsag)
	ON_COMMAND(ID_GAHTML_LINE, &CCheckParentChild::OnGahtmlLine)

	ON_COMMAND(ID_LIST, &CCheckParentChild::OnList)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParentChild::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	CCheckParam dlg;
	
	switch( m_function ) 
	{
	case FCG:
		dlg.m_title		= L"Apa és gyermeke, akikek a korkülönbsége nagyobb mint egy magadott szám.";
		dlg.m_function1 = L"Add meg minél nagyobb korkülönbségû apa-gyerek párt listázzunk:";
		dlg.m_value		= 60;
		break;
	case FCL:
		dlg.m_title		= L"Apa és gyermeke, akikek a korkülönbsége kisebb mint egy magadott szám.";
		dlg.m_function1 = L"Add meg minél kisebb korkülönbségû apa-gyerek párt listázzunk:";
		dlg.m_value		= 18;
		break;
	case MCG:
		dlg.m_title		= L"Anya és gyermeke, akikek a korkülönbsége nagyobb mint egy magadott szám.";
		dlg.m_function1 = L"Add meg minél nagyobb korkülönbségû anya-gyerek párt listázzunk:";
		dlg.m_value		= 55;
		break;
	case MCL:
		dlg.m_title		= L"Anya és gyermeke, akikek a korkülönbsége kisebb mint egy magadott szám.";
		dlg.m_function1 = L"Add meg minél kisebb korkülönbségû anya-gyerek párt listázzunk:";
		dlg.m_value		= 16;
		break;
	}
	dlg.m_function2.Empty();
	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return false;
	}

	m_diff = dlg.m_value;

	switch( m_function ) 
	{
	case FCG:
		m_title.Format( L"Apa és gyermeke, akikek a korkülönbsége nagyobb mint %d év", m_diff );
		m_parent_id_name = L"father_id";
		break;
	case FCL:
		m_title.Format( L"Apa és gyermeke, akikek a korkülönbsége kisebb mint %d év", m_diff );
		m_parent_id_name = L"father_id";
		break;
	case MCG:
		m_title.Format( L"Anya és gyermeke, akikek a korkülönbsége nagyobb mint %d év", m_diff );
		m_parent_id_name = L"mother_id";
		break;
	case MCL:
		m_title.Format( L"Anya és gyermeke, akikek a korkülönbsége kisebb mint %d év", m_diff );
		m_parent_id_name = L"mother_id";
		break;
	}

	SetWindowText( m_title );
	
	createColumns();
	fillColumns();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::createColumns()
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
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::fillColumns()
{
	int diff;
	int birthDateP;
	int birthDateC;
	int cnt = 0;

	CString rowidP;
	CString rowidC;
	CString nameP;
	CString nameC;
	CString nC;
	CString birthP;
	CString birthC;
	CString deathP;
	CString deathC;
	CString lineNumberP;
	CString lineNumberC;
	CString generationP;
	CString generationC;
	CString sourceP;
	CString sourceC;
	CString unitedP;
	CString unitedC;

	CString diffS;
	CString cntS;
	int nItem = 0;

	CProgressWnd wndP(NULL,L"Szülõ és gyermeke korülönbségének ellenõrzése ..." ); 
	wndP.GoModal();

#ifndef _DEBUG
	wndP.SetText( L"Gyerekek lekérdezése az adatbázisból...");
#endif

	m_command.Format( L"SELECT rowid, lineNumber, generation, source, united, last_name, first_name, birth_date, death_date, %s FROM people", m_parent_id_name );
	if( !theApp.query1( m_command ) ) return;

#ifndef _DEBUG
	wndP.SetText( L"Gyerekek és szüleik korkülönbségének meghatározása..");
#endif

	wndP.SetRange(0, theApp.m_recordset1->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		rowidC = theApp.m_recordset1->GetFieldString( S_ROWID );
		birthC = theApp.m_recordset1->GetFieldString( S_BIRTH );
		birthDateC = theApp.getYearFromDate( birthC );
		if( birthDateC  && checkDate( birthC ))
		{
			rowidP	= theApp.m_recordset1->GetFieldString( S_PARENT_ID );
			if( !rowidP.IsEmpty() )
			{
				// szülõ lekérdezése
				m_command.Format( L"SELECT rowid, lineNumber, generation, source, united, last_name, first_name, birth_date, death_date FROM people WHERE rowid='%s'", rowidP );
				if( !theApp.query2( m_command ) ) return;

				birthP = theApp.m_recordset2->GetFieldString( S_BIRTH );
				birthDateP = theApp.getYearFromDate( birthP ); 
				if( birthDateP && checkDate( birthP ) )
				{
					diff = birthDateC - birthDateP;
					if( ( (m_function == FCG || m_function == MCG ) && diff > m_diff ) || ( ( m_function == FCL || m_function == MCL ) && diff < m_diff ) )
					{
						diffS.Format( L"%d", diff );
						lineNumberP = theApp.m_recordset2->GetFieldString( S_LINENUMBER );
						generationP	= theApp.m_recordset2->GetFieldString( S_GENERATION );
						sourceP		= theApp.m_recordset2->GetFieldString( S_SOURCE );
						unitedP		= theApp.m_recordset2->GetFieldString( S_UNITED );
						deathP		= theApp.m_recordset2->GetFieldString( S_DEATH );
						nameP.Format( L"%s %s",theApp. m_recordset2->GetFieldString( S_LAST_NAME ), theApp.m_recordset2->GetFieldString( S_FIRST_NAME ) );

						lineNumberC	= theApp.m_recordset1->GetFieldString( S_LINENUMBER );
						generationC	= theApp.m_recordset1->GetFieldString( S_GENERATION );
						sourceC		= theApp.m_recordset1->GetFieldString( S_SOURCE );
						unitedC		= theApp.m_recordset1->GetFieldString( S_UNITED );
						deathC		= theApp.m_recordset1->GetFieldString( S_DEATH );
						nameC.Format( L"%s %s",theApp. m_recordset1->GetFieldString( S_LAST_NAME), theApp.m_recordset1->GetFieldString( S_FIRST_NAME ) );

						cntS.Format( L"%d", cnt+1 ); 
						nItem = m_ListCtrl.InsertItem( nItem, cntS );
						m_ListCtrl.SetItemText( nItem, L_ROWID, rowidP );
						m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumberP );
						m_ListCtrl.SetItemText( nItem, L_GENERATION, generationP );
						m_ListCtrl.SetItemText( nItem, L_SOURCE, sourceP );
						m_ListCtrl.SetItemText( nItem, L_UNITED, unitedP );
						m_ListCtrl.SetItemText( nItem, L_NAME, nameP );
						m_ListCtrl.SetItemText( nItem, L_BIRTH, birthP );
						m_ListCtrl.SetItemText( nItem, L_DIFF, diffS );
						m_ListCtrl.SetItemText( nItem, L_DEATH, deathP );

						++nItem;
						nItem = m_ListCtrl.InsertItem( nItem, cntS );
						m_ListCtrl.SetItemText( nItem, L_ROWID, rowidC );
						m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumberC );
						m_ListCtrl.SetItemText( nItem, L_GENERATION, generationC );
						m_ListCtrl.SetItemText( nItem, L_SOURCE, sourceC );
						m_ListCtrl.SetItemText( nItem, L_UNITED, unitedC );
						m_ListCtrl.SetItemText( nItem, L_NAME, nameC );
						m_ListCtrl.SetItemText( nItem, L_BIRTH, birthC );
						m_ListCtrl.SetItemText( nItem, L_DEATH, deathC );

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
		str.Format( L"\n\nMinden szülõ-gyermek korkülönbsége a megadott %d éven belül van\n", m_diff );
		AfxMessageBox( str );
		CDialogEx::OnOK();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckParentChild:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckParentChild::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnHtmlShows()
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
void CCheckParentChild::OnList()
{
	CString	logFile(L"spousesdiff"); 
	
	theApp.exportAll( logFile, m_title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnGahtmlLine()
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
void CCheckParentChild::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}

