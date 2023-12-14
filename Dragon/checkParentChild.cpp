// CheckParentChild.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "CheckParentChild.h"
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
//void CDragAppDlg::OnFatherchildGreater()
//{
//	CCheckParentChild dlg;
//
//	dlg.m_function = FCG;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnFatherchildLess()
//{
//	CCheckParentChild dlg;
//
//	dlg.m_function = FCL;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnMotherchildGreater()
//{
//	CCheckParentChild dlg;
//
//	dlg.m_function = MCG;
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnMotherchildLess()
//{
//	CCheckParentChild dlg;
//
//	dlg.m_function = MCL;
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
	L_DIFF,
	L_DEATH,
	L_MARRIAGES,
};
// gyerek SELECT oszlopok
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
	S_FATHER_ID,
	S_MOTHER_ID,
	S_MOTHER_INDEX,
};

// szülõ SELECT oszlopok
enum
{
	P_ROWID =0,
	P_LINENUMBER,
	P_GENERATION,
	P_SOURCE,
	P_UNITED,
	P_LAST_NAME,
	P_FIRST_NAME,
	P_BIRTH,
	P_DEATH,
	P_NUMOFMARRIAGES,
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
	DDX_Control(pDX, IDC_NEXT, colorNext);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckParentChild, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckParentChild::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckParentChild::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckParentChild::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckParentChild::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckParentChild::OnDbEdit)

	ON_COMMAND(ID_LIST, &CCheckParentChild::OnList)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CCheckParentChild::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CCheckParentChild::OnClickedNext)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParentChild::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
//		m_parent_id_name = L"father_id";
		break;
	case FCL:
		m_title.Format( L"Apa és gyermeke, akikek a korkülönbsége kisebb mint %d év", m_diff );
//		m_parent_id_name = L"father_id";
		break;
	case MCG:
		m_title.Format( L"Anya és gyermeke, akikek a korkülönbsége nagyobb mint %d év", m_diff );
//		m_parent_id_name = L"mother_id";
		break;
	case MCL:
		m_title.Format( L"Anya és gyermeke, akikek a korkülönbsége kisebb mint %d év", m_diff );
//		m_parent_id_name = L"mother_id";
		break;
	}

	SetWindowText( m_title );
	
	createColumns();
	fillColumns();
	return TRUE;
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
	m_ListCtrl.InsertColumn( L_MARRIAGES,	L"házasságok",	LVCFMT_LEFT,	 80,-1,COL_NUM);
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

	CString father_id;
	CString mother_id;
	int		mother_index; 
	CString numOfSpouses;

	CString diffS;
	CString cntS;
	int nItem = 0;

	CProgressWnd wndP(NULL,L"Szülõ és gyermeke korülönbségének ellenõrzése ..." ); 
	wndP.GoModal();

#ifndef _DEBUG
	wndP.SetText( L"Gyerekek lekérdezése az adatbázisból...");
#endif

//	m_command.Format( L"SELECT rowid, lineNumber, generation, source, united, last_name, first_name, birth_date, death_date, %s FROM people", m_parent_id_name );
	m_command.Format( L"SELECT rowid, lineNumber, generation, source, united, last_name, first_name, birth_date, death_date, father_id, mother_id, parentIndex %s FROM people" );
	if( !theApp.query1( m_command ) ) return;
	 



#ifndef _DEBUG
	wndP.SetText( L"Gyerekek és szüleik korkülönbségének meghatározása..");
#endif

	wndP.SetRange(0, theApp.m_recordset1->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		father_id = theApp.m_recordset1->GetFieldString( S_FATHER_ID );
		mother_id = theApp.m_recordset1->GetFieldString( S_MOTHER_ID );
		mother_index = _wtoi( theApp.m_recordset1->GetFieldString( S_MOTHER_INDEX ));

		switch( m_function ) 
		{
		case FCG:
			rowidP	= father_id;
			break;
		case FCL:
			rowidP	= father_id;
			break;
		case MCG:
			rowidP	= mother_id;
			break;
		case MCL:
			rowidP	= mother_id;
			break;
		}


		rowidC = theApp.m_recordset1->GetFieldString( S_ROWID );
		birthC = theApp.m_recordset1->GetFieldString( S_BIRTH );
		birthDateC = getYearFromDate( birthC );
		if( birthDateC  && checkDate( birthC ))
		{
			if( !rowidP.IsEmpty() )
			{
				// szülõ lekérdezése
				m_command.Format( L"SELECT rowid, lineNumber, generation, source, united, last_name, first_name, birth_date, death_date, numOfSpouses FROM people WHERE rowid='%s'", rowidP );
				if( !theApp.query2( m_command ) ) return;

				birthP = theApp.m_recordset2->GetFieldString( S_BIRTH );
				birthDateP = getYearFromDate( birthP ); 
				if( birthDateP && checkDate( birthP ) )
				{
					diff = birthDateC - birthDateP;
					if( ( (m_function == FCG || m_function == MCG ) && diff > m_diff ) || ( ( m_function == FCL || m_function == MCL ) && diff < m_diff ) )
					{
						m_command.Format( L"SELECT numOfSpouses FROM people WHERE rowid='%s'", father_id );
						if( !theApp.query3( m_command ) ) return;
						numOfSpouses = theApp.m_recordset3->GetFieldString( 0 );


						diffS.Format( L"%d", diff );
						// szülõ
						lineNumberP = theApp.m_recordset2->GetFieldString( P_LINENUMBER );
						generationP	= theApp.m_recordset2->GetFieldString( P_GENERATION );
						sourceP		= theApp.m_recordset2->GetFieldString( P_SOURCE );
						unitedP		= theApp.m_recordset2->GetFieldString( P_UNITED );
						deathP		= theApp.m_recordset2->GetFieldString( P_DEATH );
						nameP.Format( L"%s %s",theApp. m_recordset2->GetFieldString( P_LAST_NAME ), theApp.m_recordset2->GetFieldString( P_FIRST_NAME ) );

						// gyerek
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
						m_ListCtrl.SetItemText( nItem, L_MARRIAGES,  numOfSpouses );

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnList()
{
	CString	logFile(L"spousesdiff"); 
	
	theApp.exportAll( logFile, m_title, &m_ListCtrl );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckParentChild::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az alsó return-re debug módban hibát jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnClickedKeress()
{
	keress( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_NAME, start, true);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnHtmlEditLines()
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
void CCheckParentChild::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckParentChild::OnHtmlFatherAndSiblings()
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
void CCheckParentChild::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}