
// Fa.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
#include "locale.h"
#include "ProgressWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFaApp, CWinApp)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
ON_COMMAND(ID_TABLE_BOYS, &CFaApp::OnTableFirstNames)
ON_COMMAND(ID_USERMANUAL, &CFaApp::OnUsermanual)
ON_COMMAND(ID_LANGUAGES, &CFaApp::OnLanguages)
ON_COMMAND(ID_PLACES, &CFaApp::OnPlaces)
ON_COMMAND(ID_PRIVAT_FOLYT, &CFaApp::OnPrivatFolyt)
ON_COMMAND(ID_HUSBAND_WIFE, &CFaApp::OnHusbandWife)
ON_COMMAND(ID_EMAIL, &CFaApp::OnEmail)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFaApp::CFaApp()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CFaApp theApp;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	setlocale(LC_CTYPE,"Hungarian_Hungary.1250"); 
	
	mainDB		= new CSqliteDB;
	systemDB	= new CSqliteDB;
	blobDB		= new CSqliteDB;

	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;
	m_recordsetSystem	= new CSqliteDBRecordSet;
	m_masterTable	= new CSqliteDBRecordSet;
	m_recordsetName	= new CSqliteDBRecordSet;
	m_recordsetBlob	= new CSqliteDBRecordSet;
	m_recSetX		= new CSqliteDBRecordSet;
/***************************************************************************************************************/
	static const char * fileSpec = __FILE__;   // ez a source fájl specifikációját adja meg!!
	
	GetModuleFileName(NULL,_startFileSpec,_MAX_PATH);
	splitFilespec( _startFileSpec, &drive, &path, &filename, &ext );
	m_exePath.Format( L"%s:%s", drive, path );
	CString rootDir	= m_exePath.Left( m_exePath.ReverseFind( '\\' ) );
	CString exeDir	= m_exePath.Mid( m_exePath.ReverseFind( '\\' ) + 1 );

// minden fájlnak a fa.exe könyvtárában kell lenni, kivéve, ha a Visual Studióban dolgozunk
	if( !exeDir.CompareNoCase( L"Release" ) || !exeDir.CompareNoCase( L"Debug" ) )
	{
		m_systemDatabaseSpec.Format( L"%s\\SYSTEM\\system.db", rootDir );
		m_helpFileSpec.Format( L"%s\\SYSTEM\\DragonHelp.chm", rootDir ); 
	}
	else
	{
		m_systemDatabaseSpec.Format( L"%s\\system.db", m_exePath ); 
		m_helpFileSpec.Format( L"%s\\DragonHelp.chm", m_exePath ); 
	}
/* main database ***********************************************************************************************/
	m_databaseSpec	= GetProfileString(	L"Settings", L"database", L"" );
	if( _waccess( m_databaseSpec, 0 ) )
	{
		if( !selectDatabase() ) return false;
	}
	
	if( !openDatabase() ) return false;
// system database ********************************************************************************************************/
	if( _waccess( m_systemDatabaseSpec, 0 ) )
	{
		str.Format( L"%s fájl nem létezik! Ebbõl baj is lehet!", m_systemDatabaseSpec );
		AfxMessageBox( str );
	}
	if( !openSystemDatabase() ) return false;
//*****************************************************************************************************************/
	m_htmlFileSpec		= GetProfileString(	L"Settings", L"htmlFileSpec",L"" );
	m_gedFileSpec		= GetProfileString(	L"Settings", L"gedFileSpec",L"" );
	if( _waccess( m_gedFileSpec, 0 ) )
	{
		m_gedFileSpec.Empty();
	}

	if( _waccess( m_htmlFileSpec, 0 ) )
	{
		m_htmlFileSpec.Empty();
	}
	m_gedFileName	= m_gedFileSpec.Mid( m_gedFileSpec.ReverseFind( '\\' ) + 1 );
	m_gedFolder		= m_gedFileSpec.Left( m_gedFileSpec.ReverseFind( '\\' ) );
	m_htmlFileName	= m_htmlFileSpec.Mid( m_htmlFileSpec.ReverseFind( '\\' ) + 1 );
	m_htmlPath		= m_htmlFileSpec.Left( m_htmlFileSpec.ReverseFind( '\\' ) );

/**************************************************************************************************************/
	m_texteditor_default = L"c:\\Program Files\\Windows Nt\\Kellékek\\WordPad.exe";
	m_texteditor	= GetProfileString( L"Settings", L"texteditor", m_texteditor_default );
	if( _waccess( m_texteditor, 0 ) )
	{
		selectTextEditor();
	}
	splitFilespec( m_texteditor, &drive,&path,&filename,&ext );
	m_editorName.Format( L"%s.%s", filename, ext );
	m_editorFolder.Format( L"%s:%s", drive, path );
	NOTEPAD = false;
	if( filename == L"notepad++" ) NOTEPAD = true;

/**************************************************************************************************************/
	m_viewerSpec = GetProfileString(	L"Settings", L"viewerSpec",L"" );
	if( _waccess( m_viewerSpec, 0 ) )
	{
		selectViewer();
	}
	splitFilespec( m_viewerSpec, &drive,&path,&filename,&ext );
	m_viewerFolder.Format( L"%s:%s", drive, path ); 
	m_viewerName = filename;
/**************************************************************************************************************/
	m_pdfName	= L"AcroRd32.exe";
	m_pdfFolder	= L"C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader" ;
	m_pdfSpec.Format( L"%s\\%s", m_pdfFolder, m_pdfName );
/**************************************************************************************************************/
	if( _waccess( m_helpFileSpec, 0 ) )
	{
		str.Format( L"%s fájl nem létezik!", m_helpFileSpec );
		AfxMessageBox( str );
	}
/**************************************************************************************************************/
	aSexes.Add( L"" );
	aSexes.Add( L"férfi" );
	aSexes.Add( L"nõ" );

	aRelations.Add( L"házasság" );
	aRelations.Add( L"élettárs" );


	CODE.Add( L"UTF-8" );
	CODE.Add( L"ISO-8859-2" );

	sourceA.RemoveAll();
	sourceA.Add( L"");
	sourceA.Add( L"leszármazott" );
	sourceA.Add( L"leszármazott házastársa" );
	sourceA.Add( L"leszármazott házastársának apja" );
	sourceA.Add( L"leszármazott házastársának anyja" );
	sourceA.Add( L"leszármazott további házastársa" );

	createColumnList();

	DWORD username_size = sizeof( m_username);
	GetUserName( (LPTSTR)m_username, &username_size ); 
	

	m_fatherAgeMin	= GetProfileInt( L"Settigs", L"fatherAgeMin", 18 );
	m_fatherAgeMax	= GetProfileInt( L"Settigs", L"fatherAgeMax", 70 );

	m_motherAgeMin	= GetProfileInt( L"Settigs", L"motherAgeMin", 16 );
	m_motherAgeMax	= GetProfileInt( L"Settigs", L"motherAgeMax", 50 );

	HDC hdc = GetDC( NULL );
	_w = GetDeviceCaps(hdc, HORZRES);
	_h = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(NULL, hdc);

/**************************************************************************************************************/
	CFaDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::selectDatabase()
{
	CString initialDir;
	CString newFile;

	m_databaseSpec.Empty();

	str.Format(L"Adatbázis fájl (*.db)|*.db|Minden fájl (*.*)|*.*|" );
	CFileDialog dlg(TRUE,L".*",NULL,OFN_HIDEREADONLY|OFN_EXPLORER,str );

	dlg.m_ofn.lpstrTitle = L"Válaszd ki vagy adj meg egy új családfa adatbázis fájlt!";
	if( !theApp.m_databaseSpec.IsEmpty() )
	{
		splitFilespec( theApp.m_databaseSpec, &drive, &path,&filename,&ext );
		initialDir.Format( L"%s:%s", drive,path );
		dlg.m_ofn.lpstrInitialDir = initialDir;
	}

	if( dlg.DoModal( ) == IDCANCEL ) 
		return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	newFile = dlg.GetNextPathName( pos );

	if( theApp.m_databaseSpec != newFile )
	{
		setCreationTime( newFile );
	}


	m_databaseSpec = newFile;

//	WriteProfileStringW( L"Settings", L"database", m_databaseSpec );

	m_databaseName = m_databaseSpec.Mid( m_databaseSpec.ReverseFind( '\\' ) + 1 );
	splitFilespec( m_databaseSpec, &drive, &path,&filename,&ext );
	m_baseName = filename;

	m_databasePath.Format( L"%s:\\%s", drive, path );
	m_workingDirectory.Format( L"%s\\lists", m_databasePath );
	if( _waccess( m_workingDirectory, 0 ) )
		_wmkdir( m_workingDirectory );

	m_blobName.Format( L"%s_blob.db", m_baseName );
	m_blobSpec.Format( L"%s\\%s", m_databasePath, m_blobName );
	m_inputMode = getInputMode();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::openDatabase() 
{
	m_databaseName = m_databaseSpec.Mid( m_databaseSpec.ReverseFind( '\\' ) + 1 );
	str.Format( L"%s adatbázis megnyitása és ellenõrzése...", m_databaseName );
	CProgressWnd pWin(NULL, str ); 
	pWin.GoModal();

	splitFilespec( m_databaseSpec, &drive, &path,&filename,&ext );
	m_baseName = filename;
	m_databasePath.Format( L"%s:\\%s", drive, path );

	m_workingDirectory.Format( L"%s\\lists", m_databasePath );
	if( _waccess( m_workingDirectory, 0 ) )
		_wmkdir( m_workingDirectory );

/*********************************************************************************************************************/
	if(mainDB->IsConnected()) mainDB->Close();
	if( mainDB->Connect( m_databaseSpec, L"" ) )
	{
		str.Format( L"%s (key:%s)\nconnect error\n%s", m_databaseSpec, L"", mainDB->GetLastError() );
		AfxMessageBox(str);
		return FALSE;
	}

	m_command = L"SELECT * FROM sqlite_master WHERE type == 'table' ORDER BY tbl_name"; 
	if( !query( m_command ) ) return FALSE;
/**********************************************************************************************************************/
	if( !m_recordset->RecordsCount() )		// az adatbázisban még nincsenek táblák!! Létre kell hozni õket!
	{
		m_command.Format(L"PRAGMA encoding = 'UTF-8'" );
		if( !execute( m_command ) ) return FALSE;

		for( int i = 0; i < sizeof(mainStructure)/sizeof( DB ); ++i )
		{
			createCommand( mainStructure[i].name, (COLUMN*)mainStructure[ i ].columns, mainStructure[ i ].size );
			if( !execute( m_command ) ) return FALSE;
		}
	
		for( int i = 0; i < sizeof(mainIndexes)/sizeof( INDEXES ); ++i )
		{
			m_command.Format( L"CREATE INDEX %s ON %s (%s)", mainIndexes[i].name, mainIndexes[i].table, mainIndexes[i].column ); 
			if( !execute( m_command ) ) return FALSE;
		}
	}
	else						// már létezõ adatbázist nyitunk meg, elõtte ellenõrizzük
	{
		#ifndef _DEBUG
		pWin.SetText( L"Az adatbázis integritásának ellenõrzése..." ); 
		#endif

		if( !checkIntegrity( mainDB ) ) return false;
		if( !checkStructure( m_recordset, mainStructure, mainNumberOfTables, m_databaseSpec ) ) return false;

	}
/****************************************************************************************************************/
	WriteProfileStringW( L"Settings", L"database", m_databaseSpec );

	if( !query( L"SELECT count() FROM people" ) ) return FALSE;
	m_cntPeople = _wtoi( m_recordset->GetFieldString( 0 ) );

	if( !m_cntPeople )
		setUserVersion( 0 );

	_iterationCount = getUserVersion();
	m_inputMode = getInputMode();
	if( m_inputMode.IsEmpty() )
		return FALSE;

	pWin.DestroyWindow();

	m_blobName.Format( L"%s_blob.db", m_baseName );
	m_blobSpec.Format( L"%s\\%s", m_databasePath, m_blobName );

	while( !openBlob() )
	{
		if( !selectDatabase() ) return false;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::openBlob() 
{
	if(blobDB->IsConnected()) blobDB->Close();
	if( blobDB->Connect( m_blobSpec, L"" ) )
	{
		str.Format( L"%s \nconnect error\n%s", m_blobSpec, blobDB->GetLastError() );
		AfxMessageBox(str);
		return FALSE;
	}

	m_command = L"SELECT * FROM sqlite_master WHERE type='table' ORDER BY tbl_name";
	if( !queryBlob( m_command ) ) return FALSE;

	if( !m_recordsetBlob->RecordsCount() )		// az adatbázisban még nincsenek táblák!!
	{
		m_command.Format(L"PRAGMA encoding = 'UTF-8'" );
		if( !executeBlob( m_command ) ) return FALSE;
		for( int i = 0; i < sizeof(blobStructure)/sizeof( DB ); ++i )
		{
			createCommand( blobStructure[i].name, (COLUMN*)blobStructure[ i ].columns, blobStructure[ i ].size );
			if( !executeBlob( m_command ) ) return FALSE;
		}
		for( int i = 0; i < sizeof(blobIndexes)/sizeof( INDEXES ); ++i )
		{
			m_command.Format( L"CREATE INDEX %s ON %s (%s)", blobIndexes[i].name, blobIndexes[i].table, blobIndexes[i].column ); 
			if( !executeBlob( m_command ) ) return FALSE;
		}
	}
	else
	{
		if( !checkIntegrity( blobDB ) ) return false;
		if( !checkStructure( m_recordsetBlob, blobStructure, blobNumberOfTables, m_blobSpec ) ) return false;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::openSystemDatabase() 
{
	if(systemDB->IsConnected()) systemDB->Close();
	if( systemDB->Connect( m_systemDatabaseSpec, L"" ) )
	{
		str.Format( L"%s (key:%s)\nconnect error\n%s", m_systemDatabaseSpec, L"", systemDB->GetLastError() );
		AfxMessageBox(str);
		return FALSE;
	}

	m_command = L"SELECT * FROM sqlite_master WHERE type='table' ORDER BY tbl_name";
	if( !querySystem( m_command ) ) return FALSE;

	if( !m_recordsetSystem->RecordsCount() )		// az adatbázisban még nincsenek táblák!!
	{
		m_command.Format(L"PRAGMA encoding = 'UTF-8'" );
		if( !executeSys( m_command ) ) return FALSE;
		for( int i = 0; i < sizeof(systemStructure)/sizeof( DB ); ++i )
		{
			createCommand( systemStructure[i].name, (COLUMN*)systemStructure[ i ].columns, systemStructure[ i ].size );
			if( !executeSys( m_command ) ) return FALSE;
		}
		for( int i = 0; i < sizeof(systemIndexes)/sizeof( INDEXES ); ++i )
		{
			m_command.Format( L"CREATE INDEX IF NOT EXISTS %s ON %s (%s)", systemIndexes[i].name, systemIndexes[i].table, systemIndexes[i].column ); 
			if( !executeSys( m_command ) ) return FALSE;
		}
	}
	else
	{
		if( !checkIntegrity( systemDB ) ) return false;
		if( !checkStructure( m_recordsetSystem, systemStructure, systemNumberOfTables, m_systemDatabaseSpec ) ) return false;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFaApp::checkStructure( CSqliteDBRecordSet* rec, const DB* fileStructure, int tableNumberDB, CString fileSpec )
{
	int tableCnt = rec->RecordsCount();
	
	if( tableCnt != tableNumberDB )
	{
		 str.Format( L"%s\nadatbázis fájlban %d tábla van, pedig %d-nek kellen lenni!\n", fileSpec, tableCnt, tableNumberDB );
		 str +=  L"\nJelölj ki egy másik adatbázis fájlt vagy adj meg egy újat!";
		 AfxMessageBox( str );
		 return false;
	}


	CString createCommand;
	CString columnName;
	int		numOfColumnsDB;
	std::vector<CString> vColumns;
	COLUMN* columns;
	CString tableName;

	for( UINT i = 0; i < rec->RecordsCount(); ++i, rec->MoveNext() )
	{
		tableName = rec->GetFieldString( M_TABLE_NAME );
		if( tableName != fileStructure[i].name )
		{
			str.Format( L"%s\nadatbázis fájlban a %d. tábla név '%s', pedig '%s'-nek kellen lenni!\n", fileSpec, i+1, tableName, fileStructure[i].name );
			str +=  L"\nJelölj ki egy másik adatbázis fájlt vagy adj meg egy újat!";
			AfxMessageBox( str );
			return false;
		}

		createCommand	= rec->GetFieldString( M_SQL );
		createColumnVector( createCommand, &vColumns );
		numOfColumnsDB	= fileStructure[i].size;

		if( numOfColumnsDB != vColumns.size() )
		{
			str.Format( L"%s\nadatbázis fájlban '%s' táblájában %d oszlop van,\n pedig %d-nek kellen lenni!\n", fileSpec, tableName, vColumns.size(), numOfColumnsDB );
			str +=  L"\nJelölj ki egy másik adatbázis fájlt vagy adj meg egy újat!";
			AfxMessageBox( str );
			return false;
		}

		for( UINT j = 0 ; j < vColumns.size(); ++j )
		{
			if( toLower( vColumns.at(j) ) != toLower( fileStructure[i].columns[j].name ) )
			{
				str.Format( L"A %s\nadatbázis fájl '%s' táblájában a %d. oszlop '%s'\npedig '%s'-nek kellen lenni!\n", fileSpec, tableName, j+1, vColumns.at(j), fileStructure[i].columns[j].name );
				str +=  L"\nJelölj ki egy másik adatbázis fájlt vagy adj meg egy újat!";
				AfxMessageBox( str );
				return false;
			}
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFaApp::checkIntegrity( CSqliteDB* db )
{
	bool hiba = false;

	CString fileSpec;
	fileSpec.Format( L"%s\\databaseCheck.txt", m_workingDirectory );

	if( (openFileSpec( &theApp.fl, fileSpec, L"w+" ) ) == NULL )
		return false;

	if( !queryX( db, L"PRAGMA integrity_check" ) ) goto z;
	str = m_recSetX->GetFieldString( 0 );
	if( str != L"ok" )
	{
		hiba = true;
		fwprintf( theApp.fl, L"Elsõ integrity check eredménye:\n\n" );
		fwprintf( theApp.fl, L"%s\n\n", str );
		if( !executeX( db, L"REINDEX" ) ) goto z;
		
		if( !queryX( db, L"PRAGMA integrity_check" ) ) goto z;
		str = m_recSetX->GetFieldString( 0 );
		if( str != L"ok" )
		{
			if( !executeX( db, L"VACUUM" ) ) goto z;
			if( !queryX( db, L"PRAGMA integrity_check" ) ) goto z;
			str = m_recSetX->GetFieldString( 0 );
			if( str != L"ok" )
			{
				fwprintf( theApp.fl, L"REINDEX után is probléma van. VACUUM utána integrity check-ben is probléma van:\n\n" );
				fwprintf( theApp.fl, L"Adatvesztés történhetett!!" );
				fwprintf( theApp.fl, str );
			}
		}
		else
		{
			fwprintf( theApp.fl, L"REINDEX megoldotta a problémát!\n\n" );
		}
	}

z:	fclose( theApp.fl );
	if( hiba )
	{
		showFile( fileSpec );
		return false;
	}
	else
	{
		CFile::Remove( fileSpec );
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CFaApp::getInputMode()
{
	CString filename;
	CString ext;

	m_command = L"SELECT rowid FROM people";
	if( !query( m_command ) ) return L"";
	if( !m_recordset->RecordsCount() ) 
	{
		m_inputMode = URES;
	}
	else
	{
		m_command = L"SELECT filename FROM inputFiles";
		if( !query( m_command ) ) return L"";
		if( !m_recordset->RecordsCount() ) 
		{
			m_inputMode = MANUAL;
		}
		else
		{
			filename = m_recordset->GetFieldString( 0 );
			ext		= filename.Right( filename.GetLength() - filename.ReverseFind( '.' ) -1 );
			if( !ext.CompareNoCase( L"html" ) || !ext.CompareNoCase( L"htm" ) ) 
				m_inputMode = GAHTML;
			else if( !ext.CompareNoCase( L"ged" ) )
				m_inputMode = GEDCOM;
			else
			{
				m_inputMode = L"";
				AfxMessageBox( L"Ismeretlen eredetû adatbázis!" );
			}
		}
	}
	return m_inputMode;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaApp::createColumnList()
{
	int n;

	n = sizeof( people )/ sizeof( COLUMN );
	m_columnsP = getColumns( (COLUMN*) &people, n );

	n = sizeof( marriages )/ sizeof( COLUMN );
	m_columnsM = getColumns( (COLUMN*) &marriages, n );

	n = sizeof( tables )/ sizeof( COLUMN );
	m_columnsT = getColumns( (COLUMN*) &tables, n );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CFaApp::getColumns( COLUMN* table, int n )
{
	CString fields;

	fields.Empty();
	for( int i=1; i < n; ++i )
	{
		fields += table[i].name;
		if( i < n-1 )	fields += L",";
	}
	return fields;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaApp::createCommand( CString tablename, COLUMN *tableStruct, int n )
{
	str.Format( L"CREATE TABLE IF NOT EXISTS %s (", tablename );
	for( int i=0;  i < n; ++i )
	{
		m_command=str;
		str.Format(L"%s '%s' %s",m_command, tableStruct[i].name,tableStruct[i].type);
		if(i < n-1 ) str = str +L","; 
	}
	m_command = str + L")"; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::selectHtml( BOOL same )
{
	CString initialDir;
	CString newFile;
	CString databaseSpec;
	

	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
//		L"html files (*.html)|*.html|All Files (*.*)|*.*||" );
	L"html files (*.htm)|*.htm|(*.html)|*.html|All Files (*.*)|*.*||" );
	dlg.m_ofn.lpstrTitle = L"Válaszd ki a kívánt GA.html fájlt";
	if( !m_htmlFileSpec.IsEmpty() )
	{
		splitFilespec( m_htmlFileSpec, &drive, &path,&filename,&ext );
		initialDir.Format( L"%s:%s", drive,path );
		dlg.m_ofn.lpstrInitialDir = initialDir;
	}

	if( dlg.DoModal( ) == IDCANCEL ) return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	m_htmlFileSpec	= dlg.GetNextPathName( pos );
	m_htmlPath		= m_htmlFileSpec.Left( m_htmlFileSpec.ReverseFind( '\\' ) );
	m_htmlFileName	= m_htmlFileSpec.Mid( m_htmlFileSpec.ReverseFind( '\\' ) + 1 );

	theApp.WriteProfileStringW( L"Settings", L"htmlFileSpec", m_htmlFileSpec );

	if( same )
	{
		databaseSpec.Format( L"%s:%s\\%s.db", drive, path, filename );
		theApp.m_databaseSpec = databaseSpec;
		theApp.m_databaseName = m_databaseSpec.Mid( m_databaseSpec.ReverseFind( '\\' ) + 1 );
		theApp.WriteProfileStringW( L"Settings", L"database", theApp.m_databaseSpec );
		if( !openDatabase() ) return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::selectGedcom( BOOL sameDB )
{
	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"gedcom files (*.ged)|*.ged|All Files (*.*)|*.*||" );

	dlg.m_ofn.lpstrTitle = L"Válaszd ki a kívánt GEDCOM fájlt";
	if( dlg.DoModal( ) == IDCANCEL ) return FALSE ;

	POSITION pos	= dlg.GetStartPosition( );
	m_gedFileSpec	= dlg.GetNextPathName( pos );
	m_gedFileName	= m_gedFileSpec.Mid( m_gedFileSpec.ReverseFind( '\\' ) + 1 );
	m_gedFolder		= m_gedFileSpec.Left( m_gedFileSpec.ReverseFind( '\\' ) );
	theApp.WriteProfileStringW( L"Settings", L"gedFileSpec", theApp.m_gedFileSpec );

	splitFilespec( m_gedFileSpec, &drive, &path,&filename,&ext );
	m_gedFileName.Format( L"%s.%s", filename,ext );

	if( sameDB )
	{
		theApp.m_databaseSpec.Format( L"%s:%s\\%s.db", drive, path, filename );
		theApp.m_databaseName = m_databaseSpec.Mid( m_databaseSpec.ReverseFind( '\\' ) + 1 );
		theApp.WriteProfileStringW( L"Settings", L"database", theApp.m_databaseSpec );
		if( !openDatabase() ) return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::selectTextEditor()
{
	CString drive;
	CString path;
	CString filename;
	CString ext;
	CString initialDir;
	CString newFile;

	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"exe files (*.exe)|*.exe|All Files (*.*)|*.*||" );

	dlg.m_ofn.lpstrTitle = L"Válaszd ki a kívánt text editort!";
	if( !m_texteditor.IsEmpty() )
	{
		splitFilespec( m_texteditor, &drive, &path,&filename,&ext );
		initialDir.Format( L"%s:%s", drive,path );
		dlg.m_ofn.lpstrInitialDir = initialDir;
	}

	if( dlg.DoModal( ) == IDCANCEL ) return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	newFile = dlg.GetNextPathName( pos );

	if( m_texteditor != newFile )
	{
		m_texteditor = newFile;
		theApp.WriteProfileStringW( L"Settings", L"texteditor", m_texteditor );
		splitFilespec( m_texteditor, &drive,&path,&filename,&ext );
		m_editorName.Format( L"%s.%s", filename, ext );
		m_editorFolder.Format( L"%s:%s", drive, path );
		NOTEPAD = false;
		if( filename == L"notepad++" ) NOTEPAD = true;

		return TRUE;
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::selectViewer()
{
	CString newFile;

	str.Format(L"Viewer fájl (*.exe)|*.exe|Minden fájl (*.*)|*.*|" );
	CFileDialog dlg(TRUE,L".*",NULL,OFN_HIDEREADONLY|OFN_EXPLORER,str );
	dlg.m_ofn.lpstrTitle = L"Válaszd ki a viewer fájlt!";

	if( dlg.DoModal( ) == IDCANCEL ) return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	m_viewerSpec = dlg.GetNextPathName( pos );

	theApp.WriteProfileStringW( L"Settings", L"viewerSpec", m_viewerSpec );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaApp::clearDatabase()
{
	if( AfxMessageBox( L"Tényleg törölni karod az adatbázis a blob fájjal együtt?", MB_YESNO ) == IDNO ) return;

	if(mainDB->IsConnected()) mainDB->Close();
	CFile::Remove(  m_databaseSpec );
	if(blobDB->IsConnected()) blobDB->Close();
	CFile::Remove(  m_blobSpec );

	if( theApp.openDatabase() ) return;
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaApp::createColumnVector( CString list, std::vector<CString>* vColumns )
{
	CString column;
	int pos = list.Find( '[' );
	int pos2;
	char term1 = '[';
	char term2 = ']';

	if( pos == -1 )
	{
		term1 = '\'';
		term2 = '\'';
	}
	vColumns->clear();
	while( ( pos = list.Find( term1, pos + 1 ) ) != -1 )
	{
		if( ( pos2 = list.Find( term2, pos + 1 ) ) != -1 )
		{
			column = list.Mid( pos + 1, pos2-pos-1 );
			vColumns->push_back( column );
			pos = pos2 + 1;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::query( CString command )
{
	if( m_recordset->Query(command,mainDB))
	{
		str.Format(L"%s\n\n%s\n%s",m_databaseSpec, command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::query1( CString command )
{
	if( m_recordset1->Query(command,mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::query2( CString command )
{
	if( m_recordset2->Query(command,mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::query3( CString command )
{
	if( m_recordset3->Query(command,mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::query4( CString command )
{
	if( m_recordset4->Query(command,mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::querySystem( CString command )
{
	if( m_recordsetSystem->Query(command,systemDB))
	{
		str.Format(L"%s\n%s",command,m_recordsetSystem->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::queryBlob( CString command )
{
	if( m_recordsetBlob->Query(command,blobDB))
	{
		str.Format(L"%s\n%s",command,m_recordsetBlob->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::queryName( CString command )
{
	if( m_recordsetName->Query(command,systemDB))
	{
		str.Format(L"%s\n%s",command,m_recordsetName->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::queryX( CSqliteDB* db, CString command )
{
	if( m_recSetX->Query(command, db ))
	{
		str.Format(L"%s\n%s",command, db->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::execute( CString command)
{
	if( mainDB->Execute(command))
	{
		str.Format(L"%s\n%s",command,mainDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::executeSys( CString command)
{
	if( systemDB->Execute(command))
	{
		str.Format(L"%s\n%s",command,systemDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::executeBlob( CString command)
{
	if( blobDB->Execute(command))
	{
		str.Format(L"%s\n%s",command,blobDB->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFaApp::executeX( CSqliteDB* db, CString command)
{
	if( db->Execute(command))
	{
		str.Format(L"%s\n%s",command, db->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
