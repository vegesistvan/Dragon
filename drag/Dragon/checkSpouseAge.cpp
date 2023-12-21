// CheckSpouseAge.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "CheckSpouseAge.h"
#include "afxdialogex.h"
#include "CheckParam.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "ProgressWnd.h"
#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnHusbandAgeL()
//{
//	CCheckSpouseAge dlg;
//
//	dlg.m_function = HL;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnWifeAgeG()
//{
//	CCheckSpouseAge dlg;
//
//	dlg.m_function = WG;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnWifeAgeL()
//{
//	CCheckSpouseAge dlg;
//
//	dlg.m_function = WL;
//	dlg.DoModal();
//}
//void CDragApp::OnHusbandAgeG()
//{
//	CCheckSpouseAge dlg;
//
//	dlg.m_function = HG;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	L_MARRIAGE,
	L_DIFF,
	L_DEATH
};
// SELECT oszlopok
enum
{
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

IMPLEMENT_DYNAMIC(CCheckSpouseAge, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpouseAge::CCheckSpouseAge(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckSpouseAge::IDD, pParent)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpouseAge::~CCheckSpouseAge()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckSpouseAge, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckSpouseAge::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckSpouseAge::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckSpouseAge::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckSpouseAge::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckSpouseAge::OnDbEdit)

	ON_COMMAND(ID_LIST, &CCheckSpouseAge::OnList)

END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSpouseAge::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CCheckParam dlg;
	
	switch( m_function ) 
	{
	case HG:
		dlg.m_title		= L"Férjek, akik házasságkötéskor idõsebbek voltak, mint egy magadott kor.";
		dlg.m_function1 = L"Add meg, hogy hány évesnél idõsebb férjeket listázzunk";
		dlg.m_value		= 60;
		break;
	case HL:
		dlg.m_title		= L"Férjek, akik házasságkötéskor fiatalabbak voltak, mint egy magadott kor.";
		dlg.m_function1 = L"Add meg, hogy hány évesnél fiatalabb férjeket listázzunk";
		dlg.m_value		= 18;
		break;
	case WG:
		dlg.m_title		= L"Feleségek, akik házasságkötéskor idõsebbek voltak, mint egy magadott kor.";
		dlg.m_function1 = L"Add meg, hogy hány évesnél idõsebb menyasszonyokat listázzunk";
		dlg.m_value		= 55;
		break;
	case WL:
		dlg.m_title		= L"Feleségek, akik házasságkötéskor fiatalabbak voltak, mint egy magadott kor.";
		dlg.m_function1 = L"Add meg, hogy hány évesnél fiatalabb menyasszonyokat listázzunk";
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
	case HG:
		m_title.Format( L"Férjek, akik házasságkötéskor idõsebbek voltak, mint %d év.", m_diff );
		break;
	case HL:
		m_title.Format( L"Férjek, akik házasságkötéskor fiatalabbak voltak, mint %d év.", m_diff );
		break;
	case WG:
		m_title.Format( L"Feleségek, akik házasságkötéskor idõsebbek voltak, mint %d év.", m_diff );
		break;
	case WL:
		m_title.Format( L"Feleségek, akik házasságkötéskor fiatlabbak voltak, mint %d év.", m_diff );
		break;
	}

	SetWindowText( m_title );

	createColumns();
	fillColumns();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::createColumns()
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
	m_ListCtrl.InsertColumn( L_MARRIAGE,	L"házasság",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DIFF,		L"kora",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::fillColumns()
{
	CString marriage;
	int		m;
	CString age;
//	int		a;


	CString cntS;
	CString rowid;
	CString birth;
	CString death;

	int		b;
	CString name;
	CString lineNumber;
	CString	source;
	CString united;
	CString generation;
	CString diffS;

	int		diff = 0;
	int		cnt = 0;
	int		nItem = 0;

	CProgressWnd wndP(NULL,L"Házastárs kora  a házasság idõpontjánan ..." ); 
	wndP.GoModal();

#ifndef _DEBUG
	wndP.SetText( L"Házaspárok lekérdezése az adatbázisból...");
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !theApp.query( m_command ) ) return;


#ifndef _DEBUG
	wndP.SetText( L"Házastársak korának meghatározása a házasságkötéskor...");
#endif

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		if( m_function == HG || m_function == HL )
			rowid		= theApp.m_recordset->GetFieldString( DBM_HUSBAND_ID );
		else
			rowid		= theApp.m_recordset->GetFieldString( DBM_WIFE_ID );

		marriage	= theApp.m_recordset->GetFieldString( DBM_DATE );
		m			= getYearFromDate( marriage );
		if( !rowid.IsEmpty() && !marriage.IsEmpty() && checkDate( marriage )) 
		{
			m_command.Format( L"SELECT lineNumber, generation, source, united, last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", rowid );
			if( !theApp.query1( m_command ) ) return;
			birth	= theApp.m_recordset1->GetFieldString( S_BIRTH );
			b		= getYearFromDate( birth );
			if( !birth.IsEmpty() && checkDate( birth ))
			{
				diff = m- b;
				if( ( (m_function == HG || m_function == WG ) && diff > m_diff ) || ( ( m_function == HL || m_function == WL ) && diff < m_diff ) )
				{
					diffS.Format( L"%d", diff );
					lineNumber	= theApp.m_recordset1->GetFieldString( S_LINENUMBER );
					generation	= theApp.m_recordset1->GetFieldString( S_GENERATION );
					source		= theApp.m_recordset1->GetFieldString( S_SOURCE );
					united		= theApp.m_recordset1->GetFieldString( S_UNITED );
					death		= theApp.m_recordset1->GetFieldString( S_DEATH );
					name.Format( L"%s %s",theApp. m_recordset1->GetFieldString( S_LAST_NAME), theApp.m_recordset1->GetFieldString( S_FIRST_NAME ) );

					cntS.Format( L"%d", cnt+1 ); 
					nItem = m_ListCtrl.InsertItem( nItem, cntS );
					m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
					m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );
					m_ListCtrl.SetItemText( nItem, L_GENERATION, generation );
					m_ListCtrl.SetItemText( nItem, L_SOURCE, source );
					m_ListCtrl.SetItemText( nItem, L_UNITED, united );
					m_ListCtrl.SetItemText( nItem, L_NAME, name );
					m_ListCtrl.SetItemText( nItem, L_BIRTH, birth );
					m_ListCtrl.SetItemText( nItem, L_MARRIAGE, marriage );
					m_ListCtrl.SetItemText( nItem, L_DIFF, diffS );
					m_ListCtrl.SetItemText( nItem, L_DEATH, death );
					++nItem;
					++cnt;
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
		str.Format( L"\n\nMinden házastárs kora a házasságkötéskor\nnagyobb mint %d év\n", m_diff );
		AfxMessageBox( str );
		CDialogEx::OnOK();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnList()
{
	CString	logFile(L"spousesdiff"); 
	
	theApp.exportAll( logFile, m_title, &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnGahtmlLine()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckSpouseAge:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckSpouseAge::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt sor a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpouseAge::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

