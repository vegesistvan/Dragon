
// Fa.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "utilities.h"
#include "locale.h"
#include "ProgressWnd.h"
#include "OpenDatabase.h"
#include "checkIntegrity.h"
#include "CheckGenerations.h"
#include "ContractedPeople.h"
#include "CheckFamilyDates.h"
#include "checkFirstNames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDragonApp, CWinApp)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
ON_COMMAND(ID_TABLE_BOYS, &CDragonApp::OnTableFirstNames)
ON_COMMAND(ID_USERMANUAL, &CDragonApp::OnUsermanual)
ON_COMMAND(ID_LANGUAGES, &CDragonApp::OnLanguages)
ON_COMMAND(ID_PRIVAT_FOLYT, &CDragonApp::OnPrivatFolyt)
ON_COMMAND(ID_HUSBAND_WIFE, &CDragonApp::OnHusbandWife)
ON_COMMAND(ID_EMAIL, &CDragonApp::OnEmail)
ON_COMMAND(ID_CHECK_INEGRITY, &CDragonApp::OnCheckIntegrity)
ON_COMMAND(ID_CHECK_GENERATIONS, &CDragonApp::OnCheckGenerations)
ON_COMMAND(ID_FAMILYDATES, &CDragonApp::OnFamilyDates)
ON_COMMAND(ID_UNKNOWN_FIRSTNAMES, &CDragonApp::OnUnknownFirstnames)

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonApp::CDragonApp()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDragonApp theApp;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::InitInstance()
{
/*
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
*/
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("Dragon"));

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
	static const char * fileSpec = __FILE__;   // ez a source f�jl specifik�ci�j�t adja meg!!
	
	GetModuleFileName(NULL,_startFileSpec,_MAX_PATH);
	splitFilespec( _startFileSpec, &drive, &path, &filename, &ext );
	m_exePath.Format( L"%s:%s", drive, path );
	CString rootDir	= m_exePath.Left( m_exePath.ReverseFind( '\\' ) );
	CString exeDir	= m_exePath.Mid( m_exePath.ReverseFind( '\\' ) + 1 );

// minden f�jlnak a fa.exe k�nyvt�r�ban kell lenni, kiv�ve, ha a Visual Studi�ban dolgozunk
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
	m_databaseSpec	= GetProfileString(	L"Settings", L"databasespec", L"" );
	if( _waccess( m_databaseSpec, 0 ) )
	{
		if( !selectDatabase() ) return FALSE;
	}

	if( !openDatabase() ) return false;
	
//	while( !openDatabase() )
//		if( !selectDatabase() ) break;


// system database ********************************************************************************************************/
	if( _waccess( m_systemDatabaseSpec, 0 ) )
	{
		str.Format( L"%s f�jl nem l�tezik! Ebb�l baj is lehet!", m_systemDatabaseSpec );
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
	m_texteditor_default = L"c:\\Program Files\\Windows Nt\\Kell�kek\\WordPad.exe";
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
		str.Format( L"%s f�jl nem l�tezik!", m_helpFileSpec );
		AfxMessageBox( str );
	}
/**************************************************************************************************************/
	aSexes.Add( L"" );
	aSexes.Add( L"f�rfi" );
	aSexes.Add( L"n�" );

	aRelations.Add( L"h�zass�g" );
	aRelations.Add( L"�lett�rs" );


	CODE.Add( L"UTF-8" );
	CODE.Add( L"ISO-8859-2" );

	sourceA.RemoveAll();
	sourceA.Add( L"");
	sourceA.Add( L"lesz�rmazott" );
	sourceA.Add( L"lesz�rmazott h�zast�rsa" );
	sourceA.Add( L"lesz�rmazott h�zast�rs�nak apja" );
	sourceA.Add( L"lesz�rmazott h�zast�rs�nak anyja" );
	sourceA.Add( L"lesz�rmazott tov�bbi h�zast�rsa" );

	m_pszAppName=_tcsdup(_T("Dragon - csal�dfa adatb�zis"));

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

	CDragonDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::selectDatabase()
{
	CString initialDir;
	CString newFile;

	str.Format(L"Adatb�zis f�jl (*.db)|*.db|Minden f�jl (*.*)|*.*|" );
	CFileDialog dlg(TRUE,L".*",NULL,OFN_HIDEREADONLY|OFN_EXPLORER,str );

	dlg.m_ofn.lpstrTitle = L"V�laszd ki vagy adj meg egy �j csal�dfa adatb�zis f�jlt!";
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
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::openDatabase() 
{
	m_databaseName = m_databaseSpec.Mid( m_databaseSpec.ReverseFind( '\\' ) + 1 );
	str.Format( L"%s adatb�zis megnyit�sa �s ellen�rz�se...", m_databaseName );
	CProgressWnd pWin(NULL, str ); 
	pWin.GoModal();

	int userVersion;
	COpenDatabase open;

	open.m_databaseSpec		= m_databaseSpec;
	open.m_connDB			= mainDB;
	open.m_databaseTables	= databaseTables;
	open.m_databaseIndexes	= databaseIndexes;
	open.m_numberOfTables	= numberOfTables;
	open.m_numberOfIndexes	= numberOfIndexes;

	if( !open.openDatabase() ) 
	{
		WriteProfileString( L"Settings",L"databasespec", L"" ); 
		return false;
	}

	
	WriteProfileString( L"Settings",L"databasespec", m_databaseSpec ); // j� adatb�zis, eltenni

	splitFilespec( m_databaseSpec, &drive, &path,&filename,&ext );
	m_baseName = filename;
	m_databasePath.Format( L"%s:\\%s", drive, path );

	m_blobName.Format( L"%s_blob.db", m_baseName );
	m_blobSpec.Format( L"%s\\%s", m_databasePath, m_blobName );

	m_workingDirectory.Format( L"%s\\lists", m_databasePath );
	if( _waccess( m_workingDirectory, 0 ) )
		_wmkdir( m_workingDirectory );

	userVersion		= getUserVersion(); 
	m_loop			= userVersion >> 16;
	m_azonos		= userVersion & 0X0000FFFF;

	m_inputMode = getInputMode();
	if( m_inputMode.IsEmpty() )
		return FALSE;

	pWin.DestroyWindow();

	while( !openBlob() )
	{
		if( !selectDatabase() ) return false;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::openBlob() 
{
	COpenDatabase open;

	open.m_databaseSpec		= m_blobSpec;
	open.m_connDB			= blobDB;
	open.m_databaseTables	= blobStructure;
	open.m_databaseIndexes	= blobIndexes;
	open.m_numberOfTables	= blobNumberOfTables;
	open.m_numberOfIndexes	= numberOfBlobIndexes;

	if( !open.openDatabase() ) return false;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::openSystemDatabase() 
{
	COpenDatabase open;

	open.m_databaseSpec		= m_systemDatabaseSpec;
	open.m_connDB			= systemDB;
	open.m_databaseTables	= systemTables;
	open.m_databaseIndexes	= systemIndexes;
	open.m_numberOfTables	= numberOfSystemTables;
	open.m_numberOfIndexes	= numberOfSystemIndexes;

	if( !open.openDatabase() ) return false;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDragonApp::getInputMode()
{
	int type;
	CString filename;
	CString ext;
	CString linenumber(L"");
	/*
	if( !query( L"SELECT count() FROM people" ) ) return FALSE;
	m_cntPeople = _wtoi( m_recordset->GetFieldString( 0 ) );
	if( !m_cntPeople ) 
	{
		m_inputMode = URES;
	}
	else
	{
		if( !query( L"SELECT linenumber FROM people LIMIT 1" ) ) return FALSE;
		linenumber =  m_recordset->GetFieldString(0);
		if( !linenumber.IsEmpty() )
			m_inputMode = GAHTML;
		else
			m_inputMode = MANUAL;
	}
	*/

	


	if( !query( L"SELECT count() FROM people" ) ) return FALSE;
	m_cntPeople = _wtoi( m_recordset->GetFieldString( 0 ) );

	if( !m_cntPeople ) 
	{
		m_inputMode = URES;
	}
	else
	{
		m_command = L"SELECT type FROM filespec WHERE rowid == 1";
		if( !query( m_command ) ) return L"";
		if( !m_recordset->RecordsCount() ) 
		{
			m_inputMode = MANUAL;
		}
		else
		{
			type = _wtoi( m_recordset->GetFieldString( 0 ) );
			if( type == GA_HTML )
				m_inputMode = GAHTML;
			else if( type == GEDCOM_FILE )
				m_inputMode = GEDCOM;
			else
//			{
				m_inputMode = MANUAL;
//					AfxMessageBox( L"Ismeretlen eredet� adatb�zis!" );
//			}
		}
	}
	
	return m_inputMode;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::createColumnList()
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
CString CDragonApp::getColumns( COLUMN* table, int n )
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
void CDragonApp::createCommand( CString tablename, COLUMN *tableStruct, int n )
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
BOOL CDragonApp::selectHtml( BOOL same )
{
	CString initialDir;
	CString newFile;
	CString databaseSpec;
	

	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
//		L"html files (*.html)|*.html|All Files (*.*)|*.*||" );
	L"html files (*.htm)|*.htm|(*.html)|*.html|All Files (*.*)|*.*||" );
	dlg.m_ofn.lpstrTitle = L"V�laszd ki a k�v�nt GA.html f�jlt";
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
BOOL CDragonApp::selectGedcom( BOOL sameDB )
{
	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"gedcom files (*.ged)|*.ged|All Files (*.*)|*.*||" );

	dlg.m_ofn.lpstrTitle = L"V�laszd ki a k�v�nt GEDCOM f�jlt";
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
BOOL CDragonApp::selectTextEditor()
{
	CString drive;
	CString path;
	CString filename;
	CString ext;

	CFileDialog dlg( TRUE, L".*", NULL, OFN_HIDEREADONLY | OFN_EXPLORER,
		L"exe files (*.exe)|*.exe|All Files (*.*)|*.*||" );

	dlg.m_ofn.lpstrTitle = L"V�laszd ki a k�v�nt text editort!";
	if( dlg.DoModal( ) == IDCANCEL ) return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	m_texteditor = dlg.GetNextPathName( pos );

	theApp.WriteProfileStringW( L"Settings", L"texteditor", m_texteditor );
	splitFilespec( m_texteditor, &drive,&path,&filename,&ext );
	m_editorName.Format( L"%s.%s", filename, ext );
	m_editorFolder.Format( L"%s:%s", drive, path );

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::selectViewer()
{
	CString newFile;

	str.Format(L"Viewer f�jl (*.exe)|*.exe|Minden f�jl (*.*)|*.*|" );
	CFileDialog dlg(TRUE,L".*",NULL,OFN_HIDEREADONLY|OFN_EXPLORER,str );
	dlg.m_ofn.lpstrTitle = L"V�laszd ki a viewer f�jlt!";

	if( dlg.DoModal( ) == IDCANCEL ) return FALSE;

	POSITION pos = dlg.GetStartPosition( );
	m_viewerSpec = dlg.GetNextPathName( pos );

	theApp.WriteProfileStringW( L"Settings", L"viewerSpec", m_viewerSpec );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::clearDatabase()
{
	if( AfxMessageBox( L"T�nyleg t�r�lni karod az adatb�zis a blob f�jjal egy�tt?", MB_YESNO ) == IDNO ) return;

	if(mainDB->IsConnected()) mainDB->Close();
	CFile::Remove(  m_databaseSpec );
	if(blobDB->IsConnected()) blobDB->Close();
	CFile::Remove(  m_blobSpec );

	if( theApp.openDatabase() ) return;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragonApp::query( CString command )
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
BOOL CDragonApp::query1( CString command )
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
BOOL CDragonApp::query2( CString command )
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
BOOL CDragonApp::query3( CString command )
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
BOOL CDragonApp::query4( CString command )
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
BOOL CDragonApp::querySystem( CString command )
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
BOOL CDragonApp::queryBlob( CString command )
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
BOOL CDragonApp::queryName( CString command )
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
BOOL CDragonApp::queryX( CSqliteDB* db, CString command )
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
BOOL CDragonApp::execute( CString command)
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
BOOL CDragonApp::executeSys( CString command)
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
BOOL CDragonApp::executeBlob( CString command)
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
BOOL CDragonApp::executeX( CSqliteDB* db, CString command)
{
	if( db->Execute(command))
	{
		str.Format(L"%s\n%s",command, db->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnCheckIntegrity()
{
	CCheckIntegrity cp;
	cp.integrity();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnCheckGenerations()
{
	CCheckGenerations dlg;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnFamilyDates()
{
	CCheckFamilyDates dlg;
	dlg.m_always = false;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::OnUnknownFirstnames()
{
	CCheckFirstNames dlg;
	dlg.m_message = true;
	dlg.DoModal();
}
