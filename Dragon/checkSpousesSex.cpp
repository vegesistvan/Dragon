// CheckSpousesSex.cpp : implementation file
//

#include "stdafx.h"
#include "dragon.h"
#include "CheckSpousesSex.h"
#include "afxdialogex.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "ProgressWnd.h"
#include "Table_firstnames.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragApp::OnCheckSpousesSex()
//{
//	CCheckSpousesSex dlg;
//	dlg.DoModal();
//}
//void CDragApp::OnSpousesSex()
//{
//	CCheckSpousesSex dlg;
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
//	ON_COMMAND(ID_HTML_EDIT, &CCheckSpousesSex::OnHtmlEditLines)
	ON_COMMAND(ID_EDIT_NOTEPAD, &CCheckSpousesSex::OnEditNotepad)

	ON_COMMAND(ID_LIST, &CCheckSpousesSex::OnList)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckSpousesSex::OnDblclkList)
	ON_COMMAND(ID_FIRSTNAMES, &CCheckSpousesSex::OnFirstnames)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckSpousesSex::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString info = L"\
Alább azokat a házaspárokat listázzuk párba szedve, amelykban a házastársak neme azonos.\r\n\r\n\
Ennek lehet az az oka, hogy a programban nyilvántartott nevekhez rendelt nem helytelen, de lehet hogy \
hiba van a GA.html fájlban.\r\n\
A bal egérgombbal duplán klikkelünk egy sort, akkor a notepad++-ban megmutatj azt a sort, amit aztán \
szükség estén szerkeszthetünk, javíthatunk.\r\n\
Fontos, hogy a hibákat kijavítsuk, mert az azonos emeberhez tartzozó bejegyzések összevonásának \
sikere nagymértékben függ az emberek nevének és nemének helyességétõl.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	createColumns();
	fillColumns();
	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Minden házaspár férfi-nõ", MB_ICONINFORMATION );
		OnCancel();
	}
	return TRUE;
}
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
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_SEX,			L"nem",			LVCFMT_LEFT,	 35,-1,COL_TEXT );
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::fillColumns()
{

	CString title(L"Azonos nemû házastársak");
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


	CProgressWnd wndP(NULL,L"Házastársak nemének ellenõrzése ..." ); 
	wndP.GoModal();

	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !theApp.query( m_command ) ) return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	int z;
	for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid1 = theApp.m_recordset->GetFieldString( DBM_HUSBAND_ID );
		rowid2 = theApp.m_recordset->GetFieldString( DBM_WIFE_ID );
		if (rowid2 == L"347222")
			z = 1;
		if( !rowid1.IsEmpty() && !rowid2.IsEmpty() )
		{
			m_command.Format( L"SELECT lineNumber, generation, source, united, sex_id, last_name, first_name FROM people WHERE rowid = '%s'", rowid1 );
			if( !theApp.query1( m_command ) ) return;
			sex1 = theApp.m_recordset1->GetFieldString( S_SEX );
			if( sex1 == MAN )	sex1 = L"ff";
			else				sex1 = L"nõ";

			m_command.Format( L"SELECT lineNumber, generation, source, united, sex_id, last_name, first_name FROM people WHERE rowid = '%s'", rowid2 );
			if( !theApp.query2( m_command ) ) return;
			sex2 = theApp.m_recordset2->GetFieldString( S_SEX );
			if( sex2 == MAN )	sex2 = L"ff";
			else				sex2 = L"nõ";


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
	
	theApp.exportAll( logFile, L"Azomos nemû házastársak", &m_ListCtrl );
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


	if(Menu.LoadMenu( IDR_DROPDOWN_SEX ))
    {
		pPopup = Menu.GetSubMenu(0);
		
//		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS, MF_BYCOMMAND | MF_GRAYED);
//		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS, MF_BYCOMMAND | MF_GRAYED);
//		pPopup->EnableMenuItem(ID_DB_EDIT, MF_BYCOMMAND | MF_GRAYED);


		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnEditNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CCheckSpousesSex::OnHtmlEditLines()
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
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	CString linenumber = m_ListCtrl.GetItemText(nItem, L_LINENUMBER);
	theApp.editNotepad(theApp.m_htmlPathName, linenumber);

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckSpousesSex::OnFirstnames()
{
	CTableFirstnames dlg;
	dlg.DoModal();
}
