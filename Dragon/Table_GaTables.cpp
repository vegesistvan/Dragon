// TablesGaTables.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Table_GaTables.h"
#include "afxdialogex.h"
//#include "GA_input.h"
#include "inputGA.h"
#include "utilities.h"
#include "ProgressWnd.h"

enum
{
	L_ORDER = 0,
	L_FAMILY_NAME,
	L_BRANCH_TABLE,
	L_TABLENAME,
	L_ROOT
};
// CTableGaTables dialog
IMPLEMENT_DYNAMIC(CTableGaTables, CDialogEx)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableGaTables::CTableGaTables(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTableGaTables::IDD, pParent)
{
	m_fileName = theApp.m_htmlFileName;
	m_fileSpec = theApp.m_htmlPathName;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableGaTables::~CTableGaTables()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	//	DDX_Text(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeress);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableGaTables, CDialogEx)
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_COMMAND(ID_EXPORT_ALL, &CTableGaTables::OnExportAll)
	ON_COMMAND(ID_EXPORT_SELECTED, &CTableGaTables::OnExportSelected)
	ON_COMMAND(ID_EXTRACTTABLE, &CTableGaTables::OnExtractTables)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CTableGaTables::OnDblclkList)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CTableGaTables::OnClickedKeress)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableGaTables::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	str.Format( L"Táblák a %s fájlban", m_fileName );
	SetWindowTextW( str );

	m_ListCtrl.SortByHeaderClick( TRUE );
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_ORDER,		L"#",			LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_FAMILY_NAME,	L"családnév",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BRANCH_TABLE,L"tábla#",		LVCFMT_LEFT,	 60,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_TABLENAME,	L"tábla",		LVCFMT_LEFT,	400,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROOT,		L"törzs",		LVCFMT_LEFT,	100,-1,COL_TEXT);

	colorKeress.SetTextColor( theApp.m_colorClick );
	fillTable();
	m_orderix = 1;
	m_ListCtrl.SetSortOrder( 1, m_orderix );
		
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::fillTable( )
{
	CInputGA	split;
	CString		cLine;
	CString		sLine;
	CString 	str1;
	int			nItem = 0;
	int			i = 0;
	int fileSize;
	int filePos;

	std::vector<MARRIAGES> vM;
	std::vector<CString> v_Spouses;

	CString logFile;
	logFile.Format( L"splitTableHeader.txt" );
	str.Format( L"\"%s\" tablename felbontása", m_fileSpec ); 
	if (( theApp.fl = theApp.openLogFile( logFile, str )) == NULL)return;



	CStdioFile file( m_fileSpec, CFile::modeRead); 

	fileSize = (int)file.GetLength();
	


//	while(file.ReadString(cLine)) ++nItem; 
	
	CProgressWnd wndProgress(NULL, L"GA.html fájlban lévõ tábla azonosítók beolvasása..." );
	wndProgress.GoModal();
	wndProgress.SetRange(0, fileSize );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);


//	split.m_error_cnt1 = 0;
	nItem = 0;
	file.SeekToBegin();
	while(file.ReadString(cLine)) 
	{
		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		if( cLine.Left(2) != L"%%" ) continue;

		split.splitTableHeader( cLine );


		str1.Format( L"%d", i+1 );
		nItem = m_ListCtrl.InsertItem( i, str1 );
		m_ListCtrl.SetItemText( nItem, L_FAMILY_NAME,  split.m_tableHeader.familyName );
		m_ListCtrl.SetItemText( nItem, L_BRANCH_TABLE,  split.m_tableHeader.tableRoman );
		m_ListCtrl.SetItemText( nItem, L_TABLENAME,  cLine );
		m_ListCtrl.SetItemText( nItem, L_ROOT,  split.m_tableHeader.torzs );
		++i;
		filePos = (int)file.GetPosition();

		wndProgress.SetPos( filePos );
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	file.Close();
	wndProgress.DestroyWindow();

	fclose( theApp.fl );
/*
	if( split.m_error_cnt1 )
	{
		theApp.showLogFile();
	}
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::OnExportAll()
{
	CString	logFile(L"gatables"); 
	CString	title(L"i");
	
	title.Format( L"Táblák a %s fájlban", m_fileName );
	theApp.exportAll( logFile, title, &m_ListCtrl );
}
void CTableGaTables::OnExportSelected()
{
	CString	logFile(L"gatables_selected"); 
	CString	title( L"Kijelölt házastársak rokonai" );

	title.Format( L"Kijelölt táblák a %s fájlban", m_fileName );
	theApp.exportSelected( logFile, title, &m_ListCtrl );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnExtractTables();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::OnExtractTables()
{
	CString fileName;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if( !pos )
	{
		AfxMessageBox( L"Nincs kijelölve egyetlen tábla sem!" );
		return;
	}

	int		nItem;
	int		tableNumber;
	int		tableCounter = 0;
	CString cLine;
	CString familyName;
	CString branchTable;
	CString outputFileSpec;

	int		startPosition = 0;
	int		endPosition;
	int		tableLength;

//	FILE*	flOut;
	TCHAR	error_msg[100];
	int		cnt = 0;


//	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		if( !cnt )
		{
			familyName = m_ListCtrl.GetItemText( nItem, L_FAMILY_NAME );
			branchTable= m_ListCtrl.GetItemText( nItem, L_BRANCH_TABLE );
		}
		++cnt;
	}
	str.Format( L"%s_%s", familyName, branchTable );
	str.Replace( '%', ' ' );
	str.Trim();
	if( cnt > 1 )
	{
		fileName.Format( L"%s és még %d tábla", str, cnt );
	}
	else
		fileName = str;

	outputFileSpec.Format( L"%s\\%s_%I64d.html", theApp.m_workingDirectory, fileName, theApp.getStartTime() );

	if( !openFileSpec( &theApp.fl, outputFileSpec, L"wb" ) )return;
/*
	if(( _wfopen_s( &flOut, outputFileSpec, L"wb" ) ))
	{
		_wcserror_s( error_msg, sizeof( error_msg )/2, errno );	
		str.Format( L"%s\nfile open error!\n%s", outputFileSpec,error_msg );
		AfxMessageBox( str );
		return;
	}
*/

	CStdioFile file( m_fileSpec, CFile::modeRead); 
	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		tableNumber = _wtoi( m_ListCtrl.GetItemText( nItem, L_ORDER ) );
		tableCounter = 0;
		while( file.ReadString( cLine ) )
		{
			cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
			if( cLine.Left(2) == L"%%" )
			{
				++tableCounter;
				if( tableCounter == tableNumber )
				{
					endPosition = (int) file.GetPosition();
					while( file.ReadString( cLine ) )
					{
						cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
						if( cLine.Left(2) == L"%%" ) break;
						endPosition = (int)file.GetPosition();
					}
					tableLength = endPosition - startPosition;
					file.Close();
					if( !extract( theApp.fl, startPosition, tableLength ) ) break;
					file.Open( m_fileSpec, CFile::modeRead );
//					file.Seek( startPosition + tableLength, SEEK_SET );
					break;
				}
			}
			startPosition = (int)file.GetPosition();
		}
	}
	file.Close();
	fclose( theApp.fl );
	ShellExecute(NULL, L"open", outputFileSpec,NULL, NULL, SW_SHOWNORMAL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CTableGaTables::extract( FILE* flOut, int tablePosition, int tableLength )
{
	TCHAR		error_msg[100];
	char *buf;
	int read;
	int written;


	if( !openFileSpec( &fl1, m_fileSpec, L"rb" ) ) return 0;
/*
	if(( _wfopen_s( &flIn, m_fileSpec, L"rb" ) ))
	{
		_wcserror_s( error_msg, sizeof( error_msg )/2, errno );
		str.Format( L"%s file open error!\n%s", m_fileSpec, error_msg );
		AfxMessageBox( str );
		return 0;
	}
*/

	buf = new char[ tableLength];

	fseek( fl1, tablePosition, SEEK_SET );
	read = fread( buf, 1, tableLength, fl1 );
	if( read != tableLength )
	{
		AfxMessageBox( L"read error " );
		fclose( fl1 );
		delete[] buf;
		return 0;
	}
	if(( written = fwrite( buf, 1, tableLength,  theApp.fl ) ) != 1 )
	{
		if( ferror( theApp.fl ) )
		{
			_wcserror_s( error_msg, sizeof( error_msg )/2, errno );
			str.Format( L"Error writing to output file\n%s\nwritten data is %d bytes insted of %d", error_msg, written, tableLength );
			AfxMessageBox( str );
			fclose( fl1 );
			delete[] buf;
			return 0;
		}
	}
	fclose( fl1 );
	delete[] buf;
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableGaTables::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if(m_orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTableGaTables::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	m_orderix = wParam;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S É S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableGaTables::PreTranslateMessage(MSG* pMsg)
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::OnClickedKeress()
{
	keress(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableGaTables::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, m_orderix, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////