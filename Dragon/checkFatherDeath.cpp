#include "stdafx.h"
#include "Dragon.h"
#include "CheckFatherDeath.h"
#include "afxdialogex.h"
#include "html_EditLines.h"
#include "ProgressWnd.h"
#include "utilities.h"
#include "Relations.h"
// CCheckFatherDeath9 dialog
// txt fájl oszlopok
enum
{
	_WHO = 0,
	_DIFF,
	_ROWID,
	_LINENUMBER,
	_TABLENUMBER,
	_SOURCE,
	_UNITED,
	_MOTHERINDEX,
	_MARRIAGEDATE,
	_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_COLUMNS
};


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

enum 
{
	ROWIDC = 0,
	LINEC,
	TABLEC,
	SOURCEC,
	UNITEDC,
	MOTHERINDEXC, 
	LASTNAMEC,
	FIRSTNAMEC,
	BIRTHDATEC,
	DEATHDATEC,
	FATHERIDC,
	MOTHERIDC,
};

enum
{
	LINEP = 0,
	TABLEP,
	SOURCEP,
	UNITEDP,
	LASTNAMEP,
	FIRSTNAMEP,
	BIRTHDATEP,
	DEATHDATEP
};

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
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFatherDeath9::OnCustomdrawList)
	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFatherDeath9::OnHtmlShows)
	ON_COMMAND(ID_HTML_LINE, &CCheckFatherDeath9::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFatherDeath9::OnHtmlNotepad)
	ON_COMMAND(ID_DB_EDIT, &CCheckFatherDeath9::OnRokonsag)

	ON_COMMAND(ID_LIST, &CCheckFatherDeath9::OnList)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFatherDeath9::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();
/*
	CString info = L"\
Azokat a családokat (apa,anya, gyerekek) listázzuk, akinél az apa halála után több mint 9 hónapra született gyerek.\
";
	if( AfxMessageBox( info, MB_OKCANCEL|MB_ICONINFORMATION ) == IDCANCEL )
	{
		OnCancel();
		return FALSE;
	}
*/
	CString title = L"Az apa halála után több mint 9 hónapra született gyerekek.                               piros: apa            kék: anya             sárga: kritikus dátumok";
	SetWindowTextW( title );
	
	createColumns();
	m_cnt = 0;
	fatherDeathChildBirth();

	if( m_cnt )
		fillTable();
	else
	{
		AfxMessageBox( L"Nem találtam olyan gyereket, aki apja halála után több mint 9 hónapra született." );
		OnCancel();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::fatherDeathChildBirth()
{
	int	diff;

	// gyerek adatai
	CString child;
	CString rowidC;
	CString lineNumberC;
	CString tableNumberC;
	CString	sourceC;
	CString unitedC;
	CString birth_dateC;
	CString death_dateC;
	CString mother_indexC;


	CString rowidF_Prev(L"");
	CString rowidF;
	CString father;
	CString lineNumberF;
	CString tableNumberF;
	CString	sourceF;
	CString unitedF;
	CString marriage_indexF;
	CString birth_dateF;
	CString death_dateF;

	CString rowidM;
	CString mother;
	CString lineNumberM;
	CString tableNumberM;
	CString	sourceM;
	CString unitedM;
	CString marriage_indexM;
	CString birth_dateM;
	CString death_dateM;
	CString death_dateM1;

	CString marriageDate;

	CString fileName = L"checkFatherDeath";
	m_fileSpec.Format(L"%s\\%s_%s.text",theApp.m_workingDirectory,fileName, getTimeTag() );
	if( !openFileSpec( &fl, m_fileSpec, L"w+" ) ) return;

//	if( (m_fileSpec = theApp.openTextFile( &fl, fileName, L"w+" ) ) == L"" ) return;

	
	CProgressWnd wndP(NULL, L"Az apa halála után több mint 9 hónapra született gyerekek... "); 
	wndP.GoModal();


	// gyerekek lekérdezése
	m_command = L"SELECT rowid, lineNumber, tableNumber, source, united, parentIndex, last_name, first_name, birth_date, death_date, father_id, mother_id FROM people ORDER BY last_name, first_name";
	if( !query( m_command ) ) return;

	wndP.SetRange(0, m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);


	wndP.SetRange(0, m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);


	m_recordset->MoveFirst();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		birth_dateC = m_recordset->GetFieldString( BIRTHDATEC );
		if( !birth_dateC.IsEmpty() && checkDate( birth_dateC ) )
		{
			rowidF	= m_recordset->GetFieldString( FATHERIDC );
			if( !rowidF.IsEmpty() )
			{
				
				// apa lekérdezése
				m_command.Format( L"SELECT death_date FROM people WHERE rowid='%s'", rowidF );
				if( !query1( m_command ) ) return;
				death_dateF = m_recordset1->GetFieldString( 0 );
				if( !death_dateF.IsEmpty() && checkDate( death_dateF )  )
				{
					if( theApp.dateDiff( death_dateF, birth_dateC, 9 ) )
					{

						++m_cnt;
						// apa adatai
						rowidF = m_recordset->GetFieldString( FATHERIDC );
						if( !rowidF.Compare( rowidF_Prev ) ) continue;
						rowidF_Prev = rowidF;

						m_command.Format( L"SELECT lineNumber, tableNumber, source, united, last_name, first_name, birth_date, death_date FROM people WHERE rowid='%s'", rowidF );
						if( !query1( m_command ) ) return;
						lineNumberF		= m_recordset1->GetFieldString( LINEP );
						tableNumberF	= m_recordset1->GetFieldString( TABLEP );
						sourceF			= m_recordset1->GetFieldString( SOURCEP );
						unitedF			= m_recordset1->GetFieldString( UNITEDP );
						birth_dateF		= m_recordset1->GetFieldString( BIRTHDATEP );
						death_dateF		= m_recordset1->GetFieldString( DEATHDATEP );
						father.Format( L"%s %s", m_recordset1->GetFieldString( LASTNAMEP ), m_recordset1->GetFieldString( FIRSTNAMEP ) );

						diff = 1;
						str.Format( L"1\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t", diff, rowidF,lineNumberF,tableNumberF, sourceF,unitedF, L"",L"",father,birth_dateF, death_dateF );
						fwprintf( fl, L"%s\n", str );

						// apa feleségei

						m_command.Format( L"SELECT rowid, * FROM marriages WHERE spouse1_id='%s'", rowidF );
						if( !query2( m_command ) ) return;
						for( UINT j = 0; j < m_recordset2->RecordsCount(); ++j, m_recordset2->MoveNext() )
						{
							diff = 0;

							rowidM			= m_recordset2->GetFieldString( MARRIAGES_SPOUSE2_ID );
							marriage_indexM = m_recordset2->GetFieldString( MARRIAGES_ORDERWIFE );
							marriageDate	= m_recordset2->GetFieldString( MARRIAGES_DATE );
							// anya adatai
							m_command.Format( L"SELECT lineNumber, tableNumber, source, united, last_name, first_name, birth_date, death_date FROM people WHERE rowid='%s'", rowidM );
							if( !query3( m_command ) ) return;
							lineNumberM		= m_recordset3->GetFieldString( LINEP );
							tableNumberM	= m_recordset3->GetFieldString( TABLEP );
							sourceM			= m_recordset3->GetFieldString( SOURCEP );
							unitedM			= m_recordset3->GetFieldString( UNITEDP );
							mother.Format( L"%s %s", m_recordset3->GetFieldString( LASTNAMEP ), m_recordset3->GetFieldString( FIRSTNAMEP ) );
							birth_dateM = m_recordset3->GetFieldString( BIRTHDATEP );
							death_dateM = m_recordset3->GetFieldString( DEATHDATEP );
							mother.Format( L"%s %s", m_recordset3->GetFieldString( LASTNAMEP ), m_recordset3->GetFieldString( FIRSTNAMEP ) );
							
							str.Format( L"2\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t", diff, rowidM,lineNumberM,tableNumberM, sourceM,unitedM, marriage_indexM, marriageDate, mother,birth_dateM, death_dateM );
							fwprintf( fl, L"%s\n", str );
						
							// apa-anya gyermeke adatai

							m_command.Format( L"SELECT rowid, lineNumber, tableNumber, source, united, parentIndexCalc, last_name, first_name, birth_date, death_date FROM people WHERE father_id='%s' AND mother_id='%s'", rowidF, rowidM );
							if( !query3( m_command ) ) return;

							for( UINT k = 0; k < m_recordset3->RecordsCount(); ++k, m_recordset3->MoveNext() )
							{
								rowidC			= m_recordset3->GetFieldString( ROWIDC );
								lineNumberC		= m_recordset3->GetFieldString( LINEC );
								tableNumberC	= m_recordset3->GetFieldString( TABLEC );
								sourceC			= m_recordset3->GetFieldString( SOURCEC );
								unitedC			= m_recordset3->GetFieldString( UNITEDC );
								mother_indexC	= m_recordset3->GetFieldString( MOTHERINDEXC );
								if( !mother_indexC.Compare( L"0" ) ) mother_indexC.Empty();
								birth_dateC		= m_recordset3->GetFieldString( BIRTHDATEC );
								death_dateC		= m_recordset3->GetFieldString( DEATHDATEC );
								child.Format( L"%s %s", m_recordset3->GetFieldString( LASTNAMEC ), m_recordset3->GetFieldString( FIRSTNAMEC ) );		
							
								diff = 0;
								if( theApp.dateDiff( death_dateF, birth_dateC, 9 ) ) diff = 1;
								str.Format( L"3\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t", diff, rowidC,lineNumberC,tableNumberC, sourceC,unitedC, mother_indexC, L"", child,birth_dateC, death_dateC );
								fwprintf( fl, L"%s\n", str );
							}
						}
						fwprintf( fl, L"\n" );
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

	fclose( fl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::fillTable()
{
	int	nItem = 0;
	int col;
	int who;
	int diff;

	CString			cLine;
	CStringArray	A;
	int				n;

	CStdioFile file( m_fileSpec, CFile::modeRead); 
	while(file.ReadString(cLine)) 
	{
		if( cLine.IsEmpty() )
		{	
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
			continue;
		}

		A.RemoveAll();
		n = wordList( &A, cLine, '\t', TRUE );
		if( n != _COLUMNS )
		{
			str.Format( L"Oszlopok száma %d !=%d\n%s\n'%s'", n, _COLUMNS, cLine, A[n-1] );
			AfxMessageBox( str );
			return;
		}

		nItem = m_ListCtrl.InsertItem( nItem, L"" );
		m_ListCtrl.SetItemText( nItem, L_WHO, A[_WHO] );
		m_ListCtrl.SetItemText( nItem, L_DIFF, A[_DIFF] );
		m_ListCtrl.SetItemText( nItem, L_ROWID, A[_ROWID] );
		m_ListCtrl.SetItemText( nItem, L_LINENUMBER, A[_LINENUMBER] );
		m_ListCtrl.SetItemText( nItem, L_TABLENUMBER, A[_TABLENUMBER]);
		m_ListCtrl.SetItemText( nItem, L_UNITED, A[_UNITED] );
		m_ListCtrl.SetItemText( nItem, L_SOURCE, A[_SOURCE] );
		m_ListCtrl.SetItemText( nItem, L_MOTHERINDEX, A[_MOTHERINDEX] );
		m_ListCtrl.SetItemText( nItem, L_MARRIAGEDATE, A[_MARRIAGEDATE] );
		m_ListCtrl.SetItemText( nItem, L_NAME, A[_NAME] );
		m_ListCtrl.SetItemText( nItem, L_BIRTH, A[_BIRTH_DATE] );
		m_ListCtrl.SetItemText( nItem, L_DEATH, A[_DEATH_DATE] );

		col		= 0;
		who		= _wtoi( A[_WHO] );
		diff	= _wtoi( A[_DIFF] );

		switch( who )
		{
			case 1:
				col = 1 <<_NAME;
				if( diff )
					col = col | 1 << _DEATH_DATE;
				break;
			case 2:
				col = 1 <<_NAME;
				if( diff )
					col = col | 1 << _DEATH_DATE;
				break;
			default:
				if( diff )
					col = 1 << _BIRTH_DATE;
				break;
		};

		m_ListCtrl.SetItemData( nItem, col );
		str.Format( L"%04X", col );
		m_ListCtrl.SetItemText( nItem, L_ITEMDATA, str );

		++nItem;
	}
	file.Close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;
	UINT mask;
	int	who;
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
		who		= _wtoi( m_ListCtrl.GetItemText( nItem, _WHO ) );
		diff	= _wtoi( m_ListCtrl.GetItemText( nItem, _DIFF ) );
		if( iData & mask )	// a cella jelölve van szinezésre
		{
			if( who == MAN )
			{
				if( mask == 1 << _NAME )
					pLVCD->clrText	 = RGB( 255,0,0 );
				if( mask == (1 << _DEATH_DATE) && diff )
					pLVCD->clrTextBk = YELLOW;
			}
			else if( who == WOMAN )
			{
				pLVCD->clrText	 = RGB( 0,0,255);
				if( mask == (1 << _DEATH_DATE) && diff )
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CCheckFatherDeath9::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	CString rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );

	CRelations dlg;
	dlg.nItem		= nItem;
	dlg.m_rowid		= rowid;
	if( dlg.DoModal() == IDCANCEL ) return;

	*pResult = 0;
}
*/
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
void CCheckFatherDeath9::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnHtmlShows()
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

	CHtmlEditLines dlg;


	if( !name.IsEmpty() )
	{
		str.Format( L"%s kijelölt sora a html fájlban", name ); 
		dlg.m_title = str;
	}
	else
		dlg.m_title = L"Kijelölt sorok a htm fájlban";


	dlg.vLines = &vLines;

	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFatherDeath9::OnRokonsag()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}

