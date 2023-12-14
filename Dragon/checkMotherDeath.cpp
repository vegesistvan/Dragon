// CheckMotherDeathListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckMotherDeath.h"
#include "afxdialogex.h"
#include "Relatives.h"
#include "editHtmlLines.h"
#include "ProgressWnd.h"
#include "utilities.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckMotherDeath()
//{
//	CCheckMotherDeath dlg;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ListCtrl oszlopok
enum
{
	L_WHO = 0,
	L_DIFF,
	L_ROWID, 
	L_LINENUMBER,
	L_TABLENUMBER,
	L_SOURCE,
	L_UNITED,
	L_MOTHERINDEX,
	L_MARRIAGEDATE,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_ITEMDATA,
};




IMPLEMENT_DYNAMIC(CCheckMotherDeath, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherDeath::CCheckMotherDeath(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckMotherDeath::IDD, pParent)
{
	m_F		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;
	
	m_columns = L"rowid,lineNumber,tableNumber,source,united,last_name,first_name,birth_date,death_date"; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherDeath::~CCheckMotherDeath()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckMotherDeath, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckMotherDeath::OnCustomdrawList)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckMotherDeath::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckMotherDeath::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckMotherDeath::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckMotherDeath::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckMotherDeath::OnDbEdit)

	ON_COMMAND(ID_LIST, &CCheckMotherDeath::OnList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherDeath::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetWindowText( L"Családok, amelyekben az anya halálozási dátuma a gyerek születési dátumánál korábbi.       piros: apa        kék: anya      sárga: kritikus dátumok" );

	createColumns();
	motherDeathChildBirth();

	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Nem találtam az anya halála után született gyerekeket!", MB_ICONINFORMATION );
		CDialogEx::OnOK();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_WHO,			L"who",			LVCFMT_RIGHT,	 60,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_DIFF,		L"diff",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TABLENUMBER,	L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_MOTHERINDEX,	L"X",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_MARRIAGEDATE,L"házasság",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ITEMDATA,	L"itemData",	LVCFMT_LEFT,	 80,-1,COL_HIDDEN );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::motherDeathChildBirth()
{
	CString father_id;
	CString mother_id;

	CString birthC;
	CString deathM;

	int		ret;
	int		diff;
	UINT	j;
	UINT	k;
	MC		mc;
	int		z;

	nItem = 0;
	CProgressWnd wndP(NULL, L"Any halála után született gyerekek... "); 
	wndP.GoModal();

	// apák lekérdezése
	m_command.Format( L"SELECT rowid FROM people WHERE sex_id=1 ORDER BY last_name, first_name" );
	if( !queryF( m_command ) ) return;

	wndP.SetRange(0, m_F->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
	// apák ciklusa
	for( UINT i = 0; i < m_F->RecordsCount(); ++i, m_F->MoveNext() )
	{
		v_MC.clear();
		father_id	= m_F->GetFieldString( 0 );
		// apa feleségei
		m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id='%s' ORDER BY whichWife", father_id );
		if( !query2( m_command ) ) return;

		// feleségek ciklusa
		for( j = 0; j < m_recordset2->RecordsCount(); ++j, m_recordset2->MoveNext() )
		{
			mother_id	= m_recordset2->GetFieldString( DBM_WIFE_ID );
			// anya adatai
			m_command.Format( L"SELECT death_date FROM people WHERE rowid='%s'", mother_id );
			if( !query3( m_command ) ) return;

			deathM = checkDate( m_recordset3->GetFieldString( 0 ) );
			if( deathM.IsEmpty() ) continue;		// az anyának nincs vag yszabálytalan a halálozási dátuma

			m_command.Format( L"SELECT birth_date FROM people WHERE father_id='%s' AND mother_id = '%s'", father_id, mother_id );
			if( !query4( m_command ) ) return;

			// gyerekek ciklusa
			for( k = 0; k < m_recordset4->RecordsCount(); m_recordset4->MoveNext(), ++k )
			{
				birthC = checkDate( m_recordset4->GetFieldString( 0 ) );
				if( !birthC.IsEmpty() ) 
				{
					if( ( ret = diffD( deathM, birthC, &diff ) ) != INT_MAX )	// összehasonlítható a 2 dátum
					if( ret < 0 ) 
					{ 
						mc.mother	= j;
						mc.child	= k;
						v_MC.push_back( mc );
					}
				}
			}
			if( v_MC.size() )
			{
				listFamily( father_id );
				break;
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::listFamily( CString father_id )
{
	CString mother_id;


	CString birthC;
	CString deathM;

	int		ret;
	int		diff;
	UINT	j;
	UINT	k;

	// apák lekérdezése
	m_command.Format( L"SELECT %s FROM people WHERE rowid = '%s'", m_columns, father_id );
	if( !query1( m_command ) ) 
		return;

	m_rowid		= father_id;
	m_line		= m_recordset1->GetFieldString( LINE );
	m_table		= m_recordset1->GetFieldString( TABLE );
	m_source	= m_recordset1->GetFieldString( SOURCE );
	m_united	= m_recordset1->GetFieldString( UNITED );
	m_birth		= m_recordset1->GetFieldString( BIRTH );
	m_death		= m_recordset1->GetFieldString( DEATH );
	m_wedding	= L"";
	m_orderWife	= L"";
	m_diff		= L"0";
	m_name.Format( L"%s %s", m_recordset1->GetFieldString( LASTNAME ), m_recordset1->GetFieldString( FIRSTNAME ) );

	listPeople( nItem, 1 );
	++nItem;
				// anya adatai
				// anya kiírása

	m_command.Format( L"SELECT rowid, * FROM marriages WHERE husband_id='%s' ORDER BY whichWife", father_id );
	if( !query2( m_command ) ) return;

	// feleségek ciklusa
	for( j = 0; j < m_recordset2->RecordsCount(); ++j, m_recordset2->MoveNext() )
	{
		mother_id	= m_recordset2->GetFieldString( DBM_WIFE_ID );
		m_wedding	= m_recordset2->GetFieldString( DBM_DATE );
		m_orderWife	= m_recordset2->GetFieldString( DBM_WHICHWIFE );
		// anya adatai
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", m_columns, mother_id );
		if( !query3( m_command ) ) return;


		m_diff = L"0";
		for( UINT i = 0; i < v_MC.size(); ++i )
		{
			if( v_MC.at(i).mother == j )
			{
				m_diff = L"1";
				break;
			}
		}
		m_rowid		= mother_id;
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
		m_command.Format( L"SELECT %s FROM people WHERE father_id='%s' AND mother_id='%s'", m_columns, father_id, mother_id );
		if( !query4( m_command ) ) return;

		for( UINT k = 0; k < m_recordset4->RecordsCount(); ++k, m_recordset4->MoveNext() )
		{
			m_diff = L"0";
			for( UINT i = 0; i < v_MC.size(); ++i )
			{
				if( v_MC.at(i).mother == j && v_MC.at(i).child == k  )
				{
					m_diff = L"1";
					break;
				}
			}
			m_wedding	= L"";
			m_rowid		= m_recordset4->GetFieldString( ROWID );
			m_line		= m_recordset4->GetFieldString( LINE );
			m_table		= m_recordset4->GetFieldString( TABLE );
			m_source	= m_recordset4->GetFieldString( SOURCE );
			m_united	= m_recordset4->GetFieldString( UNITED );
			m_birth		= m_recordset4->GetFieldString( BIRTH );
			m_death		= m_recordset4->GetFieldString( DEATH );
			m_name.Format( L"%s %s", m_recordset4->GetFieldString( LASTNAME ), m_recordset4->GetFieldString( FIRSTNAME ) );		

			listPeople( nItem, 3 );
			++nItem;
		}
	}
	m_ListCtrl.InsertItem( nItem, L"" );
	++nItem;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::listPeople( int nItem, int who )
{

	str.Format( L"%d", who );
	nItem = m_ListCtrl.InsertItem( nItem, str );
	m_ListCtrl.SetItemText( nItem, L_DIFF, m_diff );
	m_ListCtrl.SetItemText( nItem, L_ROWID, m_rowid );
	m_ListCtrl.SetItemText( nItem, L_LINENUMBER, m_line );
	m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, m_table );
	m_ListCtrl.SetItemText( nItem, L_SOURCE, m_source );
	m_ListCtrl.SetItemText( nItem, L_UNITED, m_united );
	m_ListCtrl.SetItemText( nItem, L_MOTHERINDEX, m_orderWife );
	m_ListCtrl.SetItemText( nItem, L_MARRIAGEDATE, m_wedding );
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
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherDeath::queryF( CString command )
{
	if( m_F->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_F->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherDeath::query1( CString command )
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
BOOL CCheckMotherDeath::query2( CString command )
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
BOOL CCheckMotherDeath::query3( CString command )
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
BOOL CCheckMotherDeath::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::OnList()
{
	CString	logFile(L"checkMotherdeath"); 
	CString	title(L"Anya halálozási dátuma a gyerek szüeltési dátumánál korábbi");;
	
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elfogadott formátum:
// az évnek 1000 és az aktuális év közé kell esni

// 1944
// 1944.12.
// 1944.12.17

// hibás dátum esetén üres stringet ad vissza, jó dátrum esetén a dátumot
CString CCheckMotherDeath::checkDate( CString datum)
{
	CString ures(L"");

	datum.Trim();
	int length = datum.GetLength();
	if( length < 4 ) return ures;

	int	year = _wtoi( datum.Left( 4 ) );
	if( year < 1000 ) return ures;

// év ellenõrzése
	char	sTime[50];
	time_t	now = time(0);
	ctime_s( sTime, sizeof( sTime ), &now);
	CString st;
	st = sTime;
	int	yearCurr = _wtoi( st.Right( 6 ) );		// "\n" miatt 6 karaktert vesz le, ami konverzinál ugyis lemarad
	if( year > yearCurr ) return ures;

	// év utáni '.' ellenõrzése
	if( length == 4 ) return datum;	// csak 2001 a dátum

// hónap ellenõrzése a 2019.11 
	if( length <= 7 ) return ures;
	if( datum.GetAt(4) != '.' ) return ures;
	if( !iswdigit( datum.GetAt( 5 ) ) ) 
		return ures;
	if( !iswdigit( datum.GetAt( 6 ) ) ) 
		return ures;
	int	month = _wtoi( datum.Mid( 5, 2 ) );
	if( month < 1 || month > 12 ) return ures;
	if( length == 8  ) return datum;	// csak 2019.11.  a dátum

// nap ellenõrzése 2019.11.12
	if( datum.GetAt( 7 ) != '.' ) return ures;
	if( length < 10 ) return ures;

	if( !iswdigit( datum.GetAt( 8 ) ) ) 
		return ures;
	if( !iswdigit( datum.GetAt( 9 ) ) ) 
		return ures;
	int	day = _wtoi( datum.Mid( 8, 2 ) );
	if( day < 1 ) return ures;
	if( month == 2 && day > 29 ) return false;
	if(( month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 ) return ures;
	if( day > 31 ) return ures; 
	if( length > 10 ) return ures;  // dátum után még van valami
	return datum;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckMotherDeath:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::OnHtmlEditLines()
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
void CCheckMotherDeath::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherDeath::OnHtmlFatherAndSiblings()
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
void CCheckMotherDeath::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

