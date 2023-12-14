#include "stdafx.h"
#include "Dragon.h"
#include "CheckFatherDeath.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relatives.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckFather9()
//{
//	CCheckFatherDeath9 dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	L_WHO = 0,
	L_DIFF,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_SOURCE,
	L_UNITED,
	L_ORDERWIFE,
	L_MARRIAGEDATE,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_ITEMDATA,
};

// SELECT oszlopok

CString m_columns = L"rowid,lineNumber,tableNumber,source,united,last_name,first_name,birth_date,death_date"; 


IMPLEMENT_DYNAMIC(CCheckFatherDeath9, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFatherDeath9::CCheckFatherDeath9(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFatherDeath9::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFatherDeath9::~CCheckFatherDeath9()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFatherDeath9, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFatherDeath9::OnCustomdrawList)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFatherDeath9::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFatherDeath9::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckFatherDeath9::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckFatherDeath9::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckFatherDeath9::OnDbEdit)

	ON_COMMAND(ID_LIST, &CCheckFatherDeath9::OnList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherDeath9::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString title = L"Az apa halála után több mint 9 hónapra született gyerekek.                               piros: apa            kék: anya             sárga: kritikus dátumok";
	SetWindowTextW( title );
	
	createColumns();
	fatherDeathChildBirth();

	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Nem találtam olyan gyereket,\naki apja halála után több mint 9 hónapra született." );
		OnCancel();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherDeath9::query( CString command )
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
BOOL CCheckFatherDeath9::query1( CString command )
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
BOOL CCheckFatherDeath9::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherDeath9::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherDeath9::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_WHO,			L"who",			LVCFMT_RIGHT,	 60,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_DIFF,		L"diff",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_MARRIAGEDATE,L"házasság",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_ORDERWIFE,	L"ix",			LVCFMT_LEFT,	 25,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ITEMDATA,	L"itemData",	LVCFMT_LEFT,	 80,-1,COL_HIDDEN );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::fatherDeathChildBirth()
{
	CString birthC;
	CString deathF;

	CString rowidF;
	CString rowidM;

	UINT	j;
	int		nItem = 0;
	
	CProgressWnd wndP(NULL, L"Az apa halála után több mint 9 hónapra született gyerekek... "); 
	wndP.GoModal();

	// apák lekérdezése
	m_command.Format( L"SELECT %s FROM people WHERE sex_id=1 ORDER BY last_name, first_name", m_columns );
	if( !query( m_command ) ) return;

	wndP.SetRange(0, m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		rowidF = m_recordset->GetFieldString( ROWID );
		deathF = m_recordset->GetFieldString( DEATH );
		if( deathF.IsEmpty() || !checkDate( deathF ) ) goto cont;

		// van olyan gyereke aki 9 hónap után született?
		m_command.Format( L"SELECT birth_date FROM people WHERE father_id ='%s'", rowidF );
		if( !query1( m_command ) ) return;
		for( j = 0; j < m_recordset1->RecordsCount(); ++j, m_recordset1->MoveNext() )
		{
			birthC = m_recordset1->GetFieldString( 0 );
			if( !birthC.IsEmpty() && checkDate( birthC ) )
			{
				if( theApp.dateDiff( deathF, birthC, 9 ) ) break;
			}
		}
		if( j == m_recordset1->RecordsCount() ) goto cont;  // minden gyereke születési ideje OK

		// apa adatai
		m_rowid		= rowidF;
		m_line		= m_recordset->GetFieldString( LINE );
		m_table		= m_recordset->GetFieldString( TABLE );
		m_source	= m_recordset->GetFieldString( SOURCE );
		m_united	= m_recordset->GetFieldString( UNITED );
		m_birth		= m_recordset->GetFieldString( BIRTH );
		m_death		= m_recordset->GetFieldString( DEATH );
		m_wedding	= L"";
		m_orderWife	= L"";
		m_diff		= L"1";
		m_name.Format( L"%s %s", m_recordset->GetFieldString( LASTNAME ), m_recordset->GetFieldString( FIRSTNAME ) );

		listPeople( nItem, 1 );
		++nItem;

		// apa feleségei
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id='%s' ORDER BY whichWife", rowidF );
		if( !query2( m_command ) ) return;
		for( UINT j = 0; j < m_recordset2->RecordsCount(); ++j, m_recordset2->MoveNext() )
		{
			rowidM		= m_recordset2->GetFieldString( DBM_WIFE_ID );
			m_orderWife = m_recordset2->GetFieldString( DBM_WHICHWIFE );
			m_wedding	= m_recordset2->GetFieldString( DBM_DATE );

			// anya adatai
			m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", m_columns, rowidM );
			if( !query3( m_command ) ) return;

			m_diff		= L"0";
			m_rowid		= rowidM;
			m_line		= m_recordset3->GetFieldString( LINE );
			m_table		= m_recordset3->GetFieldString( TABLE );
			m_source	= m_recordset3->GetFieldString( SOURCE );
			m_united	= m_recordset3->GetFieldString( UNITED );
			m_birth		= m_recordset3->GetFieldString( BIRTH );
			m_death		= m_recordset3->GetFieldString( DEATH );
			m_name.Format( L"%s %s", m_recordset3->GetFieldString( LASTNAME ), m_recordset3->GetFieldString( FIRSTNAME ) );

			listPeople( nItem, 2 );
			++nItem;

			// apa-anya gyermeke adatai
			m_command.Format( L"SELECT %s FROM people WHERE father_id='%s' AND mother_id='%s'", m_columns, rowidF, rowidM );
			if( !query4( m_command ) ) return;

			for( UINT k = 0; k < m_recordset4->RecordsCount(); ++k, m_recordset4->MoveNext() )
			{
				m_diff		= L"0";
				m_wedding	= L"";
				m_rowid		= m_recordset4->GetFieldString( ROWID );
				m_line		= m_recordset4->GetFieldString( LINE );
				m_table		= m_recordset4->GetFieldString( TABLE );
				m_source	= m_recordset4->GetFieldString( SOURCE );
				m_united	= m_recordset4->GetFieldString( UNITED );
				m_birth		= m_recordset4->GetFieldString( BIRTH );
				m_death		= m_recordset4->GetFieldString( DEATH );
				m_name.Format( L"%s %s", m_recordset4->GetFieldString( LASTNAME ), m_recordset4->GetFieldString( FIRSTNAME ) );		
				

				if( theApp.dateDiff( deathF, m_birth, 9 ) ) m_diff = L"1";
				listPeople( nItem, 3 );
				++nItem;
			}
		}
		m_ListCtrl.InsertItem( nItem, L"" );
		++nItem;
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::listPeople( int nItem, int who )
{

	str.Format( L"%d", who );
	nItem = m_ListCtrl.InsertItem( nItem, str );
	m_ListCtrl.SetItemText( nItem, L_DIFF, m_diff );
	m_ListCtrl.SetItemText( nItem, L_ROWID, m_rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, m_line );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, m_table );
	m_ListCtrl.SetItemText( nItem, L_UNITED, m_united );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, m_source );
	m_ListCtrl.SetItemText( nItem, L_MARRIAGEDATE, m_wedding );
	m_ListCtrl.SetItemText( nItem, L_ORDERWIFE, m_orderWife );
	m_ListCtrl.SetItemText( nItem, L_NAME, m_name );
	m_ListCtrl.SetItemText( nItem, L_BIRTH, m_birth );
	m_ListCtrl.SetItemText( nItem, L_DEATH, m_death );

	int col = 0;
	int diff = _wtoi( m_diff );
	switch( who )
	{
		case 1:
			col = 1 << L_NAME;
			if( diff )
				col = col | 1 << L_DEATH;
			break;
		case 2:
			col = 1 << L_NAME;
			if( diff )
				col = col | 1 << L_DEATH;
			break;
		default:
			if( diff )
				col = 1 << L_BIRTH;
			break;
	};

	m_ListCtrl.SetItemData( nItem, col );
//	str.Format( L"%04X", col );
//	m_ListCtrl.SetItemText( nItem, L_ITEMDATA, str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;
	UINT mask;
	CString	who;
	int diff;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		nCol	= pLVCD->iSubItem;
		mask	= 1 << nCol;
		iData	= m_ListCtrl.GetItemData( nItem );
		who		= m_ListCtrl.GetItemText( nItem, L_WHO );
		diff	= _wtoi( m_ListCtrl.GetItemText( nItem, L_DIFF ) );
		if( iData & mask )	// a cella jelölve van szinezésre
		{
			if( who == MAN )
			{
				if( mask == 1 << L_NAME )
					pLVCD->clrText	 = RGB( 255,0,0 );
				if( mask == (1 << L_DEATH ) && diff )
					pLVCD->clrTextBk = YELLOW;
			}
			else if( who == WOMAN )
			{
				pLVCD->clrText	 = RGB( 0,0,255);
				if( mask == (1 << L_DEATH ) && diff )
					pLVCD->clrTextBk = YELLOW;
			}
			else
			{
				if( diff )
					pLVCD->clrTextBk = YELLOW;
			}
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnList()
{
	CString	logFile(L"checkFatherDeath9"); 
	CString	title(L"Apa halála után több mint 9 hónapra született gyerekek");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFatherDeath9::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
void CCheckFatherDeath9::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnHtmlEditLines()
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
void CCheckFatherDeath9::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnHtmlFatherAndSiblings()
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
void CCheckFatherDeath9::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

