// CheckSpousesSex.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckSpousesSex.h"
#include "afxdialogex.h"
#include "html_EditLines.h"
#include "Relatives.h"
#include "ProgressWnd.h"

// CCheckSpousesSex dialog

enum
{
	L_CNT = 0,
	L_ROWID,
	L_LINENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_NAME,
	L_SEX
};
// SELECT oszlopok
enum
{
	S_LINENUMBER,
	S_GENERATION,
	S_SOURCE,
	S_UNITED,
	S_SEX,
	S_LAST_NAME,
	S_FIRST_NAME,
};

IMPLEMENT_DYNAMIC(CCheckSpousesSex, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpousesSex::CCheckSpousesSex(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckSpousesSex::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckSpousesSex::~CCheckSpousesSex()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckSpousesSex, CDialogEx)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckSpousesSex::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckSpousesSex::OnHtmlNotepad)

	ON_COMMAND(ID_LIST, &CCheckSpousesSex::OnList)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSpousesSex::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
//	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
//	EASYSIZE_INIT();

	CString info = L"\
Al�bb azokat a h�zasp�rokat list�zzuk p�rba szedve, amelykban a h�zast�rsak neme azonos.\r\n\r\n\
Ennek lehet az az oka, hogy a programban nyilv�ntartott nevekhez rendelt nem helytelen, de lehet hogy \
hiba van a GA.html f�jlban.\r\n\
A jobb eg�rgombbal egy sorra klikkelve egy leg�rd�l� men�b�l v�laszthatunk k�l�nb�z� funkci�kat a \
GA-html f�jl vizsg�lat�ra, jav�t�s�ra.\r\n\
Fontos, hogy a hib�kat kijav�tsuk, mert az azonos emeberhez tartzoz� bejegyz�sek �sszevon�s�nak \r\n\
sikere nagym�rt�kben f�gg az emeberek nev�nek �s nem�nek helyess�g�t�l.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	createColumns();
	fillColumns();
	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Minden h�zasp�r f�rfi-n�", MB_ICONINFORMATION );
		OnCancel();
	}
	return TRUE;
}
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
//	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 55,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_GENERATION,	L"G",			LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITED,		L"U",			LVCFMT_LEFT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_NAME,		L"n�v",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_SEX,			L"nem",			LVCFMT_LEFT,	 35,-1,COL_TEXT );
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::fillColumns()
{

	CString title(L"Azonos nem� h�zast�rsak");
	CString logFile( L"checkSexes");

	int nItem = 0;
	int cnt = 0;
	CString rowid1;
	CString rowid2;
	
	CString lineNumber1;
	CString lineNumber2;

	CString generation1;
	CString generation2;

	CString source1;
	CString source2;

	CString united1;
	CString united2;


	CString name1;
	CString name2;

	CString	sex1;
	CString sex2;

	CString cntS;


	CProgressWnd wndP(NULL,L"H�zast�rsak nem�nek ellen�rz�se ..." ); 
	wndP.GoModal();

	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid1 = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		rowid2 = theApp.m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		if( !rowid1.IsEmpty() && !rowid2.IsEmpty() )
		{
			m_command.Format( L"SELECT lineNumber, generation, source, united, sex_id, last_name, first_name FROM people WHERE rowid = '%s'", rowid1 );
			if( !theApp.query1( m_command ) ) return;
			sex1 = theApp.m_recordset1->GetFieldString( S_SEX );
			if( sex1 == MANS )	sex1 = L"ff";
			else				sex1 = L"n�";

			m_command.Format( L"SELECT lineNumber, generation, source, united, sex_id, last_name, first_name FROM people WHERE rowid = '%s'", rowid2 );
			if( !theApp.query2( m_command ) ) return;
			sex2 = theApp.m_recordset2->GetFieldString( S_SEX );
			if( sex2 == MANS )	sex2 = L"ff";
			else				sex2 = L"n�";


			if( sex1 == sex2 )
			{
				lineNumber2	= theApp.m_recordset2->GetFieldString( S_LINENUMBER );
				generation2	= theApp.m_recordset2->GetFieldString( S_GENERATION );
				source2		= theApp.m_recordset2->GetFieldString( S_SOURCE );
				united2		= theApp.m_recordset2->GetFieldString( S_UNITED );
				name2.Format( L"%s %s",theApp. m_recordset2->GetFieldString( S_LAST_NAME ), theApp.m_recordset2->GetFieldString( S_FIRST_NAME ) );

				lineNumber1	= theApp.m_recordset1->GetFieldString( S_LINENUMBER );
				generation1	= theApp.m_recordset1->GetFieldString( S_GENERATION );
				source1		= theApp.m_recordset1->GetFieldString( S_SOURCE );
				united1		= theApp.m_recordset1->GetFieldString( S_UNITED );
				name1.Format( L"%s %s",theApp. m_recordset1->GetFieldString( S_LAST_NAME), theApp.m_recordset1->GetFieldString( S_FIRST_NAME ) );

				cntS.Format( L"%d", cnt+1 ); 
				nItem = m_ListCtrl.InsertItem( nItem, cntS );
				m_ListCtrl.SetItemText( nItem, L_ROWID, rowid1 );
				m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber1 );
				m_ListCtrl.SetItemText( nItem, L_GENERATION, generation1 );
				m_ListCtrl.SetItemText( nItem, L_SOURCE, source1 );
				m_ListCtrl.SetItemText( nItem, L_UNITED, united1 );
				m_ListCtrl.SetItemText( nItem, L_NAME, name1 );
				m_ListCtrl.SetItemText( nItem, L_SEX, sex1 );

				++nItem;
				nItem = m_ListCtrl.InsertItem( nItem, cntS );
				m_ListCtrl.SetItemText( nItem, L_ROWID, rowid2 );
				m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber2 );
				m_ListCtrl.SetItemText( nItem, L_GENERATION, generation2 );
				m_ListCtrl.SetItemText( nItem, L_SOURCE, source2 );
				m_ListCtrl.SetItemText( nItem, L_UNITED, united2 );
				m_ListCtrl.SetItemText( nItem, L_NAME, name2 );
				m_ListCtrl.SetItemText( nItem, L_SEX, sex2 );

				++nItem;
				nItem = m_ListCtrl.InsertItem( nItem, L"");
				++nItem;
				++cnt;
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnList()
{
	CString	logFile(L"spousessex"); 
	
	theApp.exportAll( logFile, L"Azomos nem� h�zast�rsak", &m_ListCtrl );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckSpousesSex:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
		
		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS, MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_DB_EDIT, MF_BYCOMMAND | MF_GRAYED);


		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
