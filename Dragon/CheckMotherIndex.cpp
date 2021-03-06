// CheckMotherIndex.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckMotherIndex.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relatives.h"
#include "html_EditLine.h"
#include "html_EditLines.h"

enum
{
	L_CNT = 0,
	L_ROWID,
	L_LINENUMBER,
	L_INDEX,
	L_LINE
};

IMPLEMENT_DYNAMIC(CCheckMotherIndex, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherIndex::CCheckMotherIndex(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckMotherIndex::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMotherIndex::~CCheckMotherIndex()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckMotherIndex, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckMotherIndex::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckMotherIndex::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CCheckMotherIndex::OnDbEdit)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckMotherIndex::OnHtmlFatherAndSiblings)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMotherIndex::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_EDIT,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()

/*
	if( theApp.getUserVersion() )
	{
		AfxMessageBox( L"Csak �sszevon�s el�tt van �rtelme ezt az ellen�rz�st elv�gezni!" );
		CDialog::OnCancel();
		return false;
	}
*/
	int nItem = 0;
	CString rowid;
	CString name;
	CString lineNumber;
	CString lineNumberP;
	CString lineP;
	CString gaLine;
	CString father_id;
	CString mother_id;
	CString parent_id;
	CString parent;

	CString rowidC;

	int		z;
	int		pos;
	int		sex_id;
	int		source;
	int		parentIndex;
	int		count;
	int		cnt = 0;
	int		numOfMarriages;

	CString info = L"\
Sorp�rokat l�thatunk az al�bbi list�ban. Az els� sor a sz�l�, a m�sodik a gyermek sora a ga-html f�jlban.\
A gyermek annya-indexe �s az apa feles�geinek sz�ma k�z�tt van ellentmond�s.\r\n\n\
Csak �sszevon�s el�tt van �rtelem ezt a list�t elemezni, mert �sszevon�s ut�n a GA.htl-ben val� sorok sz�ma \
�sszekeveredik.\r\n\r\n\
Ha le�ny�gi lesz�rmazottak gyeremekei is vannak a GA-html-ben, akkor az apa �s az anya felcser�l�dik. Erre jelenleg nincs felk�sz�lve a program.\
";
	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	CProgressWnd wndP( NULL, L"Sz�l�-indexek ellen�rz�se..." );
	wndP.GoModal();

#ifndef _DEBUG
	str.Format( L"GA-html f�jl sorindexeinek elk�sz�t�se..." );
	wndP.SetText( str );
#endif

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( L_CNT,			L"#",		LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_INDEX,		L"X",		LVCFMT_RIGHT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINE,		L"n�v",		LVCFMT_LEFT,	200,-1,COL_TEXT);

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	gafile.Open( theApp.m_htmlFileSpec, CFile::modeRead );
	vPos.clear();
	vPos.push_back(0);
	while( 	gafile.ReadString( cLine ) )
		vPos.push_back( gafile.GetPosition() );

// gyerekek lek�rdez�se	
	m_command = L"SELECT rowid, lineNumber, parentIndex, father_id, mother_id, first_name, last_name, numOfMarriages FROM people WHERE source='1' ORDER BY lineNumber";
	if( !theApp.query( m_command ) )
	{
		OnCancel();
//		return false;
	}

#ifndef _DEBUG
	str.Format( L"Gyermek-sz�l� rekordok vizsg�lata..." );
	wndP.SetText( str );
#endif

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);

	for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowid			= theApp.m_recordset->GetFieldString( 0 );
		lineNumber		= theApp.m_recordset->GetFieldString( 1 );
		parentIndex	= _wtoi( theApp.m_recordset->GetFieldString( 2 ) );
		if( parentIndex == 0 ) goto cont;

		father_id		= theApp.m_recordset->GetFieldString( 3 );
		mother_id		= theApp.m_recordset->GetFieldString( 4 );

		if( mother_id == L"42691" )
			z = 1;
//		name.Format( L"%s %s", theApp.m_recordset->GetFieldString(6), theApp.m_recordset->GetFieldString(5) );
		// a sz�l�k html sora
		m_command.Format( L"SELECT lineNumber FROM marriages WHERE spouse1_id='%s' AND spouse2_id= '%s'", father_id, mother_id );  // az ember apj�nak h�zass�gai
		if( !theApp.query1( m_command ) ) OnCancel();
		if( !theApp.m_recordset1->RecordsCount() ) goto cont;
		
		lineNumberP	= theApp.m_recordset1->GetFieldString( 0 );
		lineP = getHtmlLine( lineNumberP );
		
		// h�ny h�zass�ga van az ap�nak?
		m_command.Format( L"SELECT lineNumber FROM marriages WHERE spouse1_id='%s'", father_id );  // az ember apj�nak h�zass�gai
		if( !theApp.query1( m_command ) ) OnCancel();
		count =theApp.m_recordset1->RecordsCount();
		if( !count ) goto cont;

		parent_id = father_id;
		parent = getWord( lineP,2, &pos );
		if( (pos = parent.Find( '/' )) != -1 )
			parent = parent.Left( pos );
		parent.Remove( ',' );
		parent.TrimRight();
		sex_id = theApp.isFirstName( parent );
		if( sex_id == -1 ) goto cont;  // nem tudja milyen nem�

		if( sex_id == 2 ) // h�ny h�zast�rsa van az any�nak
		{
			m_command.Format( L"SELECT count() FROM marriages WHERE spouse2_id='%s'", mother_id );  // az ember apj�nak h�zass�gai
			if( !theApp.query1( m_command ) ) OnCancel();
			count = _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
			parent_id = mother_id;
			if( !count ) goto cont;
		}
		if( parentIndex > count )
		{
			// sz�l�
			++cnt;
			str.Format( L"%d", cnt );
			nItem = m_ListCtrl.InsertItem( nItem, str );
		
			str.Format( L"%d", count );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, parent_id );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumberP);
			m_ListCtrl.SetItemText( nItem, L_LINE, lineP );
			++nItem;

			// gyermek 
			str.Format( L"%d-%d", cnt, parentIndex );
			nItem = m_ListCtrl.InsertItem( nItem, str );

			str.Format( L"%d", parentIndex );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );

			str = getHtmlLine( lineNumber );
			m_ListCtrl.SetItemText( nItem, L_LINE, str );
			++nItem;

			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		theApp.m_recordset->MoveFirst();
		for( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
		{
			numOfMarriages = _wtoi( theApp.m_recordset->GetFieldString( 7 ) );
			if( numOfMarriages < 2 ) continue;

			rowid			= theApp.m_recordset->GetFieldString( 0 );
			lineNumberP		= theApp.m_recordset->GetFieldString( 1 );
			lineP			= getHtmlLine( lineNumberP );

			m_command.Format( L"SELECT rowid  FROM people WHERE father_id = '%s'", rowid ); 
			if( !theApp.query1( m_command ) ) return false;
			if( !theApp.m_recordset1->RecordsCount() ) continue;  // nincs gyereke

			rowidC			= theApp.m_recordset1->GetFieldString( 0 );
			m_command.Format( L"SELECT parentIndex, linenumber  FROM people WHERE rowid ='%s'", rowidC );
			if( !theApp.query1( m_command ) ) return false;
			parentIndex = _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
			if( parentIndex ) continue;

			lineNumber		= theApp.m_recordset1->GetFieldString( 1 );


			++cnt;
			str.Format( L"%d", cnt );
			nItem = m_ListCtrl.InsertItem( nItem, str );
		
			str.Format( L"%d", count );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, parent_id );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumberP);
			m_ListCtrl.SetItemText( nItem, L_LINE, lineP );
			++nItem;

			// gyermek 
			str.Format( L"%d-%d", cnt, parentIndex );
			nItem = m_ListCtrl.InsertItem( nItem, str );

			str.Format( L"%d", parentIndex );
			m_ListCtrl.SetItemText( nItem, L_INDEX, str );
			m_ListCtrl.SetItemText( nItem, L_ROWID, rowid );
			m_ListCtrl.SetItemText( nItem, L_LINENUMBER, lineNumber );

			str = getHtmlLine( lineNumber );
			m_ListCtrl.SetItemText( nItem, L_LINE, str );
			++nItem;

			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
		}
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
		m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
	wndP.DestroyWindow();
	vPos.clear();
	gafile.Close();

	if( !m_ListCtrl.GetItemCount() )
	{
		AfxMessageBox( L"Nincs hiba az anya-indexekkel!" );
		CDialog::OnCancel();
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CCheckMotherIndex::getHtmlLine( CString lineNumber )
{
	int ln = _wtoi( lineNumber );

	gafile.Seek( vPos.at( ln - 1 ), SEEK_SET );
	gafile.ReadString( cLine );		// �tugrik cnt sort
	cLine.Trim();
	if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
	cLine = cleanHtmlLine( cLine );
	return cLine;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckMotherIndex:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
    {
		pPopup = Menu.GetSubMenu(0);
/*
		if( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ).IsEmpty() )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_NOTEPAD, MF_BYCOMMAND | MF_GRAYED);
		}
*/
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CHtmlEditLine dlg;
	dlg.m_linenumber	= m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	dlg.DoModal();
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );
	CRelatives dlg;

	dlg.m_rowid = rowid;
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"Sz�l�k �s testv�rek" );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMotherIndex::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}