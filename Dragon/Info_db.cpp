// Info_db.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "Info_db.h"
#include "afxdialogex.h"


enum
{
	L_ROWID = 0,
	L_FILENAME,
	L_SIZE,
	L_CREATED,
	L_MODIFIED,
	L_LOADED,
};


// CInfoDb dialog

IMPLEMENT_DYNAMIC(CInfoDb, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInfoDb::CInfoDb(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoDb::IDD, pParent)
{

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInfoDb::~CInfoDb()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATABASE, colorDatabase);
	DDX_Control(pDX, IDC_SYSTEMDATABASE, colorSystemDatabase);
	DDX_Control(pDX, IDC_BLOBDB, colorBlobSpec);

	DDX_Control(pDX, IDC_FREELIST_COUNT, colorFreeCount);
	DDX_Control(pDX, IDC_FREELISTBLOB_COUNT, colorFreeCountBlob);
	DDX_Control(pDX, IDC_FREELISTSYSTEM_COUNT, colorFreeCountSystem);

	DDX_Control(pDX, IDC_INTEGRITY, colorIntegrity);
	DDX_Control(pDX, IDC_INTEGRITYBLOB, colorIntegrityBlob);
	DDX_Control(pDX, IDC_INTEGRITYSYSTEM, colorIntegritySystem);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CInfoDb, CDialogEx)
	ON_BN_CLICKED(IDC_REINDEX_BLOB, &CInfoDb::OnClickedReindexBlob)
	ON_BN_CLICKED(IDC_REINDEX, &CInfoDb::OnClickedReindex)
	ON_BN_CLICKED(IDC_VACUUM, &CInfoDb::OnClickedVacuum)
	ON_BN_CLICKED(IDC_VACUUM_BLOB, &CInfoDb::OnClickedVacuumBlob)
	ON_BN_CLICKED(IDC_VACUUM_SYSTEM, &CInfoDb::OnClickedVacuumSystem)
	ON_BN_CLICKED(IDC_REINDEX_SYSTEM, &CInfoDb::OnClickedReindexSystem)
	ON_BN_CLICKED(IDOK, &CInfoDb::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInfoDb::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString command;
	CString people;
	CString families;
	CString tables;
	CString marriages;
	CString freeListCount;
	CString integrity;
	CString fileSpec;
	FILE*	fl;

	fileSpec.Format( L"%s\\integrityError.txt", theApp.m_workingDirectory );

	colorDatabase.SetTextColor( theApp.m_colorClick );
	colorSystemDatabase.SetTextColor( theApp.m_colorClick );
	colorBlobSpec.SetTextColor( theApp.m_colorClick );
	
	CProgressWnd pW(NULL, L"Az adatbázis integritásának ellenõrzése...");
	pW.GoModal();


	command =  L"PRAGMA freelist_count";
	if( !theApp.query( command ) ) return 0;
	freeListCount = theApp.m_recordset->GetFieldString( 0 );
	if( _wtoi( freeListCount) )
		colorFreeCount.SetTextColor( theApp.m_colorClick );
	GetDlgItem( IDC_FREELIST_COUNT )->SetWindowTextW( freeListCount );

	if( !theApp.queryBlob( command ) ) return 0;
	freeListCount = theApp.m_recordsetBlob->GetFieldString( 0 );
	if( _wtoi( freeListCount) )
		colorFreeCountBlob.SetTextColor( theApp.m_colorClick );
	GetDlgItem( IDC_FREELISTBLOB_COUNT )->SetWindowTextW( freeListCount );

	if( !theApp.querySystem( command ) ) return 0;
	freeListCount = theApp.m_recordsetSystem->GetFieldString( 0 );
	if( _wtoi( freeListCount) )
		colorFreeCountSystem.SetTextColor( theApp.m_colorClick );
	GetDlgItem( IDC_FREELISTSYSTEM_COUNT )->SetWindowTextW( freeListCount );


	command = L"PRAGMA integrity_check";
	if( !theApp.query( command ) ) return 0;
	integrity = theApp.m_recordset->GetFieldString( 0 );
	GetDlgItem( IDC_INTEGRITY )->SetWindowTextW( integrity );
	if( integrity != L"ok" )
		colorIntegrity.SetTextColor( theApp.m_colorWarning );


	if( !theApp.queryBlob( command ) ) return 0;
	integrity = theApp.m_recordsetBlob->GetFieldString( 0 );
	if( integrity == L"ok" )
		GetDlgItem( IDC_INTEGRITYBLOB )->SetWindowTextW( integrity );
	else
	{
		GetDlgItem( IDC_INTEGRITYBLOB )->SetWindowTextW( L"wrong!!!" );
		colorIntegrityBlob.SetTextColor( theApp.m_colorWarning );
		openFileSpec( &theApp.fl, fileSpec, L"w+" );
		fwprintf( theApp.fl, L"%s adatbázis integritási ellenõrzése az alábbi hibát jelezte:\n\n", theApp.m_blobSpec );
		fwprintf( theApp.fl, L"%s", integrity );
		fclose( theApp.fl );
		theApp.showFile( fileSpec);

	}
	


	if( !theApp.querySystem( command ) ) return 0;
	integrity = theApp.m_recordsetSystem->GetFieldString( 0 );
	if( integrity != L"ok" )
	{
		GetDlgItem( IDC_INTEGRITYSYSTEM )->SetWindowTextW( L"wrong!!!" );
		colorIntegritySystem.SetTextColor( theApp.m_colorWarning );
	}
	else
		GetDlgItem( IDC_INTEGRITYSYSTEM )->SetWindowTextW( integrity );

	pW.DestroyWindow();


	if( !theApp.query( L"SELECT count() FROM people" ) ) return FALSE;
	people = thousend( _wtoi( theApp.m_recordset->GetFieldString(0) ));
	if( !theApp.query( L"SELECT count() FROM tables" ) ) return FALSE;
	tables = thousend( _wtoi( theApp.m_recordset->GetFieldString(0) ));
	if( !theApp.query( L"SELECT count() FROM tables WHERE percent='%%%'" ) ) return FALSE;
	families = thousend( _wtoi( theApp.m_recordset->GetFieldString(0) )) ;
	if( !theApp.query( L"SELECT count() FROM marriages" ) ) return FALSE;
	marriages = thousend( _wtoi( theApp.m_recordset->GetFieldString(0) ));


	GetDlgItem( IDC_DATABASE )->SetWindowTextW( theApp.m_databaseSpec );
	GetDlgItem( IDC_PEOPLES )->SetWindowTextW( people );
	GetDlgItem( IDC_FAMILIES )->SetWindowTextW( families );
	GetDlgItem( IDC_TABLES )->SetWindowTextW( tables );
	GetDlgItem( IDC_MARRIAGES )->SetWindowTextW( marriages );

/*
	CString str;
	if( theApp.m_inputMode == GEDCOM )
		str = L"GEDOM files read";
	else if( theApp.m_inputMode == GAHTML )
		str = L"Beolvasott ga.htm fájlok";
	else
		str.Empty();

	GetDlgItem( IDC_STATIC_LOADED )->SetWindowTextW( str );
*/

	CFileStatus status;
	CString mtime(L"");
	CString ctime(L"");
	CString atime(L"");
	CString size = 0;;

	if( CFile::GetStatus( theApp.m_databaseSpec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size.Format( L"%s bájt", thousend( status.m_size ) );
	}

	GetDlgItem( IDC_DBSIZE )->SetWindowTextW( size );
	GetDlgItem( IDC_CREATED )->SetWindowTextW( ctime );
	GetDlgItem( IDC_MODIFIED )->SetWindowTextW( mtime );
	GetDlgItem( IDC_ACCESSED )->SetWindowTextW( atime );

	int iterationCount = theApp.getUserVersion();
	str.Format( L"%d", iterationCount );
	GetDlgItem( IDC_ITERATIONS_COUNT )->SetWindowTextW( str );

	GetDlgItem( IDC_BLOBDB )->SetWindowTextW( theApp.m_blobSpec );
	if( CFile::GetStatus( theApp.m_blobSpec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size.Format( L"%s bájt", thousend( status.m_size ) );
	}

	GetDlgItem( IDC_DBSIZEBLOB )->SetWindowTextW( size );
	GetDlgItem( IDC_CREATEDBLOB )->SetWindowTextW( ctime );
	GetDlgItem( IDC_MODIFIEDBLOB )->SetWindowTextW( mtime );
	GetDlgItem( IDC_ACCESSEDBLOB )->SetWindowTextW( atime );


	command = L"SELECT count() FROM pictures";
	if( !theApp.queryBlob( command ) ) return false;
	CString blobs = theApp.m_recordsetBlob->GetFieldString( 0 );
	GetDlgItem( IDC_BLOBS )->SetWindowTextW( blobs );

/*
	if( theApp.m_inputMode != MANUAL )
	{
		m_ListCtrl.SortByHeaderClick( TRUE );
		m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

		m_ListCtrl.InsertColumn( L_ROWID,		L"#",				LVCFMT_RIGHT,	 20,-1,COL_NUM);
		m_ListCtrl.InsertColumn( L_FILENAME,	L"input fájl",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_SIZE,		L"hossz",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_CREATED,		L"létrehozva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_MODIFIED,	L"módosítva",		LVCFMT_LEFT,	120,-1,COL_TEXT);
		m_ListCtrl.InsertColumn( L_LOADED,		L"beolvasva",		LVCFMT_LEFT,	120,-1,COL_TEXT);

		command = L"SELECT rowid,* FROM inputFiles";
		if( theApp.query( command ) )
		{
			for( UINT i = 0; i < theApp.m_recordset->RecordsCount(); ++i )
			{
				m_ListCtrl.InsertItem( i, theApp.m_recordset->GetFieldString( F_ROWID ) );
				m_ListCtrl.SetItemText( i, L_FILENAME, theApp.m_recordset->GetFieldString( F_FILENAME));
				m_ListCtrl.SetItemText( i, L_SIZE, theApp.m_recordset->GetFieldString( F_SIZE ));
				m_ListCtrl.SetItemText( i, L_CREATED, theApp.m_recordset->GetFieldString( F_CREATED ));
				m_ListCtrl.SetItemText( i, L_MODIFIED, theApp.m_recordset->GetFieldString( F_MODIFIED ));
				m_ListCtrl.SetItemText( i, L_LOADED, theApp.m_recordset->GetFieldString( F_LOADED ));

				theApp.m_recordset->MoveNext();
			}
		}
	}
*/

	GetDlgItem( IDC_SYSTEMDATABASE )->SetWindowTextW( theApp.m_systemDatabaseSpec );

	if( CFile::GetStatus( theApp.m_systemDatabaseSpec, status) )
	{
		mtime	= status.m_mtime.Format( L"%Y.%m.%d %H:%M:%S" );
		ctime	= status.m_ctime.Format( L"%Y.%m.%d %H:%M:%S" );
		atime	= status.m_atime.Format( L"%Y.%m.%d %H:%M:%S" );
		size.Format( L"%s bájt", thousend( status.m_size ) );
	}

	GetDlgItem( IDC_DBSIZE2 )->SetWindowTextW( size );
	GetDlgItem( IDC_CREATED2 )->SetWindowTextW( ctime );
	GetDlgItem( IDC_MODIFIED2 )->SetWindowTextW( mtime );
	GetDlgItem( IDC_ACCESSED2 )->SetWindowTextW( atime );

	CString db;
	if( !theApp.querySystem( L"SELECT count() FROM firstnames" ) ) return FALSE;
	db = thousend( _wtoi( theApp.m_recordsetSystem->GetFieldString(0) ));
	GetDlgItem( IDC_STATIC_NAMES )->SetWindowTextW( db );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedReindex()
{
	CProgressWnd wndProgress(NULL, L"Adatbázis újraindexelése folyik..." ); 
	wndProgress.GoModal();
	for( int i = 0; i < numberOfIndexes; ++i )
	{
		m_command.Format( L"DROP INDEX IF EXISTS %s", databaseIndexes[i].name );
		if( !theApp.execute( m_command ) ) return;
		m_command.Format( L"CREATE INDEX IF NOT EXISTS %s ON %s (%s)", databaseIndexes[i].name, databaseIndexes[i].table, databaseIndexes[i].column ); 
		if( !theApp.execute( m_command ) ) return;
	}
	wndProgress.DestroyWindow();
	str.Format( L"%s adatbázis\nújraindexelése sikeresen megtörtént!", theApp.m_databaseName );
	AfxMessageBox( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedReindexBlob()
{
	theApp.executeBlob( L"REINDEX" );

	str.Format( L"%s adatbázis\nújraindexelése sikeresen megtörtént!", theApp.m_blobName );
	AfxMessageBox( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedReindexSystem()
{
	CProgressWnd wndProgress(NULL, L"Adatbázis újraindexelése folyik..." ); 
	wndProgress.GoModal();
	for( int i = 0; i < sizeof(systemIndexes)/sizeof( INDEXES ); ++i )
	{
		m_command.Format( L"DROP INDEX IF EXISTS %s", systemIndexes[i].name );
		if( !theApp.executeSys( m_command ) ) return;
		m_command.Format( L"CREATE INDEX IF NOT EXISTS %s ON %s (%s)", systemIndexes[i].name, systemIndexes[i].table, systemIndexes[i].column ); 
		if( !theApp.executeSys( m_command ) ) return;
	}
	wndProgress.DestroyWindow();
	str.Format( L"%s adatbázis\nújraindexelése sikeresen megtörtént!", L"system" );
	AfxMessageBox( str );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedVacuum()
{
	CString free;
	GetDlgItem( IDC_FREELIST_COUNT )->GetWindowTextW( free );
	if( _wtoi( free ) )
	{
		if( !theApp.execute( L"VACUUM") ) return;

		if( !theApp.query( L"PRAGMA freelist_count" )  ) return;

		CString freeListCount;	
		freeListCount = theApp.m_recordset->GetFieldString( 0 );
		GetDlgItem( IDC_FREELIST_COUNT )->SetWindowTextW( freeListCount );
		AfxMessageBox( L"A felhasználatlan részeket töröltem az adatbázisból!"); 
	}
	else
		AfxMessageBox( L"Nincsenek törlendõ felhasználatlan részeket az adatbázisban!"); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedVacuumBlob()
{
	CString free;
	GetDlgItem( IDC_FREELISTBLOB_COUNT )->GetWindowTextW( free );
	if( _wtoi( free ) )
	{
		if( !theApp.executeBlob( L"VACUUM") ) return;

		if( !theApp.queryBlob( L"PRAGMA freelist_count" )  ) return;

		CString freeListCount;	
		freeListCount = theApp.m_recordsetBlob->GetFieldString( 0 );
		GetDlgItem( IDC_FREELISTBLOB_COUNT )->SetWindowTextW( freeListCount );
		AfxMessageBox( L"A felhasználatlan részeket töröltem az adatbázisból!"); 
	}
	else
		AfxMessageBox( L"Nincsenek törlendõ felhasználatlan részeket az adatbázisban!"); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnClickedVacuumSystem()
{
	CString free;
	GetDlgItem( IDC_FREELISTSYSTEM_COUNT )->GetWindowTextW( free );
	if( _wtoi( free ) )
	{
		if( !theApp.executeSys( L"VACUUM") ) return;

		if( !theApp.querySystem( L"PRAGMA freelist_count" )  ) return;

		CString freeListCount;	
		freeListCount = theApp.m_recordsetSystem->GetFieldString( 0 );
		GetDlgItem( IDC_FREELISTSYSTEM_COUNT )->SetWindowTextW( freeListCount );
		AfxMessageBox( L"A felhasználatlan részeket töröltem az adatbázisból!"); 
	}
	else
		AfxMessageBox( L"Nincsenek törlendõ felhasználatlan részeket az adatbázisban!"); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfoDb::OnBnClickedOk()
{

	CDialogEx::OnOK();
}
