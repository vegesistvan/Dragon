// Same.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "Same.h"
#include "afxdialogex.h"
#include <algorithm>

enum
{
	L_CNT = 0,
	L_ROWID, 
	L_GENERATIONS1,
	L_SOURCES1,
	L_UNITEDS1,
	L_LINES1,
	L_ROWIDS1,
	L_SPOUSE1,
	L_BIRTHS1,
	L_DEATHS1,
	L_MOTHERS1,
	L_GENERATIONS2,
	L_SOURCES2,
	L_UNITEDS2,
	L_LINES2,
	L_ROWIDS2,
	L_SPOUSE2,
	L_BIRTHS2,
	L_DEATHS2,
	L_MOTHERS2,
};

enum
{
	P_ROWID = 0,
	P_LINE,
	P_GENERATION,
	P_SOURCE,
	P_UNITED,
	P_SEX_ID,
	P_LAST_NAME,
	P_FIRST_NAME,
	P_BIRTH_DATE,
	P_DEATH_DATE,
	P_FATHER_ID,
	P_MOTHER_ID,
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortByCnt(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.cnt < v2.cnt );
}
bool sortByGroup(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.group < v2.group );
}
bool sortByMother(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.mother < v2.mother );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSame, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSame::CSame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSame::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

		p_fields = L"\
rowid,\
lineNumber,\
generation,\
source,\
united,\
sex_id,\
last_name,\
first_name,\
birth_date,\
death_date,\
father_id,\
mother_id\
";
	m_description = L"\
Az oszlopok jelentése:\n\n\
gr     - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\
st     - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
line   - a bejegyzés sorszáma a GA html fájlban.\n\
u      - united, az ember u számú realizáció összevonása.\n\
m      - married, X: házasember, üres: nem házas.\n\
G      - generáció, az ember generációs jele a GA fájlban.\n\
rowid  - a realizáció azonosítója.\n\
name   - az ember neve.\n\
birth  - születési dátum.\n\
death  - halálozás dátuma.\n\
mother - az ember anyjának neve.\n\n";


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSame::~CSame()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSame, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSame::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	if( _wtoi( theApp.m_user_version ) == 0 )
	{
		AfxMessageBox( L"Elõször hajts végre az\n'Azonos nevû házaspárok'\nmenüpontot!" );
		OnCancel();
		return FALSE;
	}

	openDifferent();
	openUnited();

	m_deleted = 0;
	createColumns();
	collectPeople();

	m_command.Format( L"PRAGMA user_version='2'" );
	theApp.execute( m_command );

	fclose( fU );
	fclose( fD );

	if( m_deleted )
	{
		theApp.execute( L"VACUUM");
		str.Format( L"%d ember összevonása történt", m_deleted );
		AfxMessageBox( str );
		theApp.showHtmlFile( unitedSpec );
		
		OnCancel();
	}
	else
	{
		AfxMessageBox( L"Nincs összevonható ember." );
	}
	theApp.showHtmlFile( differentSpec );
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,				L"cnt",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,			L"marriage",	LVCFMT_RIGHT,	 60,-1,COL_NUM);

	m_ListCtrl.InsertColumn( L_GENERATIONS1,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES1,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS1,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES1,			L"lineS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS1,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE1,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS1,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS1,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERS1,		L"motherS1",LVCFMT_LEFT,	150,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_GENERATIONS2,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES2,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS2,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES2,			L"lineS2",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS2,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE2,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS2,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS2,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_MOTHERS2,		L"motherS1",LVCFMT_LEFT,	150,-1,COL_TEXT);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::collectPeople()
{
	CString namePrev;
	CString name;
	bool	first = true;;
	
	
	wndP.Create( NULL, L"Azonos nevû emberek..." );
	wndP.GoModal();

	

#ifndef _DEBUG
	str.Format( L"Azonos nevû emberek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name", p_fields );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	theApp.execute( L"BEGIN" );

	vPeople.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		name = m_recordset->GetFieldString( P_LAST_NAME );
		if( name.IsEmpty() ) goto cont;
		name.Format( L"%s %s", m_recordset->GetFieldString( P_LAST_NAME ), m_recordset->GetFieldString( P_FIRST_NAME ) );
		if( name.TrimRight().IsEmpty() ) goto cont;
		
		if( name == namePrev )
		{
			if( first )
			{
				putPeople( i-1 );
				first = false;
			}
			putPeople( i );
		}
		else if( vPeople.size() )
		{
			processPeople();
			vPeople.clear();
			first = true;
		}
		namePrev = name;
cont:	m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	theApp.execute( L"COMMIT" );

	if( vPeople.size() )
		processPeople();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::putPeople( UINT i )
{
	m_recordset->MoveTo( i );
	SAMENAMES vpeople;
	CString name;
	CString rowid = m_recordset->GetFieldString( P_ROWID );

	int z;
	if( rowid == L"136437" )
		z = 1;


	name.Format( L"%s %s", m_recordset->GetFieldString( P_LAST_NAME ), m_recordset->GetFieldString( P_FIRST_NAME ) );
	vpeople.name		= name.TrimRight();
	
	vpeople.group		= 0;
	vpeople.status		= 0;
	vpeople.rowid		= rowid;

	vpeople.line		= m_recordset->GetFieldString( P_LINE );
	vpeople.generation	= m_recordset->GetFieldString( P_GENERATION );
	vpeople.source		= m_recordset->GetFieldString( P_SOURCE );
	vpeople.united		= m_recordset->GetFieldString( P_UNITED );
	vpeople.sex_id		= m_recordset->GetFieldString( P_SEX_ID );
	vpeople.birth		= m_recordset->GetFieldString( P_BIRTH_DATE );
	vpeople.death		= m_recordset->GetFieldString( P_DEATH_DATE );

	CString mother_id = m_recordset->GetFieldString( P_MOTHER_ID );
	m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid = '%s'", mother_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), m_recordset1->GetFieldString( 1 ) );
	vpeople.mother = name.TrimRight();


	if( vpeople.sex_id == L"1" )
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query1( m_command ) ) return;
	int married = _wtoi( m_recordset1->GetFieldString( 0 ) );
	vpeople.married = married;
	vPeople.push_back( vpeople );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::processPeople()
{
	UINT group = 1;
	UINT db = 0;
	int z;
	CString gen1;
	CString gen2;

	if( vPeople.at(0).name == L"Abaffy Adeodat-Arisztid" )
		z = 1;
	m_contracted = 0;
	for( UINT i1 = 0; i1 < vPeople.size(); ++i1 )
	{
		if( db ) ++group;
		db = 0;
		if( vPeople.at(i1).group == 0 )		// az i1. házaspárt még nem rendelték hozzá egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vPeople.size(); ++i2 )
			{
				if( i1 == 1 && i2 == 3 )
					z = 2;
				if( i1 != i2 && vPeople.at(i2).group == 0 )	// természetesen csak különbözõ házaspárokat vizsgál, amelyeket még nem redneltek hozzá egyik csooprthoz sem
				{
					if( identical( i1, i2 ) )
					{
						vPeople.at( i1 ).group = group;
						vPeople.at( i2 ).group = group;

						if( vPeople.at(i1).generation.IsEmpty() )
							gen1 = L"Z";
						else
							gen1 = vPeople.at( i1 ).generation;
						if( vPeople.at(i2).generation.IsEmpty() )
							gen2 = L"Z";
						else
							gen2 = vPeople.at( i2 ).generation;

						if( gen1 < gen2 )
						{
							vPeople.at( i1 ).status = -1;
							vPeople.at( i2 ).status = 1;
							contract( i2, i1 );
						}
						else
						{
							vPeople.at( i1 ).status = 1;
							vPeople.at( i2 ).status = -1;
							contract( i1, i2 );
						}
						
						m_contracted = 1;
						++m_deleted;
						++db;
					}
				}
			}
		}
	}
	m_numOfGroups = getNumOfGroups();
	listPeople();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
int CSame::identical( UINT i1, UINT i2 )
{
	getData( i1 );
	getData2( i2 );

	int z;



	if( vPeople.at(i1).name == L"Amadé Anna Mária" )
		z = 1;

	if( (vPeople.at(i1).rowid == L"87423" && vPeople.at(i2).rowid == L"87491") || (vPeople.at(i2).rowid == L"87423" && vPeople.at(i1).rowid == L"87491")  )
		z = 1;
	int	empty = 0;
	int b;
	int d;
	int m;

	if( ( b = birth() ) == -1 ) return false;  // ellentmondás
	else
	{
		if( b == 0 ) ++empty;
	}
	if( ( d = death() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( d == 0 ) ++empty;
	}
	if( ( m = mother() ) == -1 ) return false;	// ellentmondás
	else
	{
		if( m == 0 ) ++empty;
	}
	if( empty > 1 ) return false;				// legalább egy egyezés ellentmondás nélkül 
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  1 : mindkettõ meg van adva és egyezik;
//  0 : csak az egyik van megadva, íg ynincs ellentmondás
//  1 : mindkettõ meg van adva és nem egyezik
int CSame::birth( )
{
	if( !_birth.IsEmpty() && _birth == _birth2 ) return 1;
	if( !_birth.IsEmpty() && !_birth2.IsEmpty() && _birth != _birth2 ) return -1;
	return 0;
}
int CSame::death( )
{
	if(  !_death.IsEmpty() && _death == _death2 ) return 1;
	if(  !_death.IsEmpty() && !_death2.IsEmpty() && _death != _death2 ) return -1;
	return 0;
}
int CSame::mother( )
{
	if( !_mother.IsEmpty() && _mother == _mother2 ) return 1;
	if(  !_mother.IsEmpty() && !_mother2.IsEmpty() && _mother != _mother2 ) return -1;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az iBy indexû emberrel helyettesíti az iDel indexû embert
void CSame::contract( UINT iBy, UINT iDel )
{
	CString rowid	= vPeople.at( iDel ).rowid;
	CString rowidBy	= vPeople.at(iBy).rowid;
	CString sex_id	= vPeople.at(iBy).sex_id;
	int source		= _wtoi( vPeople.at(iBy).source );

	theApp.replaceBy( rowid, rowidBy, sex_id, source );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::listPeople()
{
	std::sort( vPeople.begin(), vPeople.end(), sortByMother );
	CString married;
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		married.Empty();
		getData(i);
		if( _married ) married = L"X";

str.Format( L"%2d %2d %9s %2s %1s %1s %8s %-30s %12s %12s %-30s",\
_group, _status, _line, _united, married, _gen,\
_rowid, _name, _birth, _death, _mother, married ); 

		if( m_contracted )
		{
			if( vPeople.at(i).status == 1 )
			{
				fwprintf( fU, L"<span style=\"background:aquamarine\">%s</span>\n", str );
			}
			else if( vPeople.at(i).status == -1 )
				fwprintf( fU, L"<span style=\"background:lightgray\">%s</span>\n", str );
			else
				fwprintf( fU, L"%s\n", str );
		}
		else
			fwprintf( fD, L"%s\n", str );
	}

	if( m_contracted )
		fwprintf( fU, L"\n" );
	else
		fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::getData( UINT i )
{
	_group		= vPeople.at(i).group;
	_status		= vPeople.at(i).status;
	_line		= vPeople.at(i).line;
	_united		= vPeople.at(i).united;
	_gen		= vPeople.at(i).generation;
	_rowid		= vPeople.at(i).rowid;
	_name		= vPeople.at(i).name;
	_birth		= vPeople.at(i).birth;
	_death		= vPeople.at(i).death;
	_mother		= vPeople.at(i).mother;
	_married	= vPeople.at(i).married;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::getData2( UINT i )
{
	_group2		= vPeople.at(i).group;
	_status2	= vPeople.at(i).status;
	_line2		= vPeople.at(i).line;
	_united2	= vPeople.at(i).united;
	_gen2		= vPeople.at(i).generation;
	_rowid2		= vPeople.at(i).rowid;
	_name2		= vPeople.at(i).name;
	_birth2		= vPeople.at(i).birth;
	_death2		= vPeople.at(i).death;
	_mother2	= vPeople.at(i).mother;
	_married2	= vPeople.at(i).married;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CSame::getNumOfGroups()
{
	UINT max = 0;
	for( UINT i = 0; i < vPeople.size(); ++i )
	{ 
		max = __max( max, vPeople.at(i).group );
	}
	return max;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::openUnited()
{
	CString fileName = L"peopleUnited";
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	CString title1( L"AZONOS NEVÛ EMBEREK, AKIK AZONOSAK IS, EZÉRT ÖSSZEVONHATÓAK" ); 
	
	fwprintf( fU, L"<HEAD>\n" );
	fwprintf( fU, L"<style>\n" );
	fwprintf( fU, L"</style>\n" );
	fwprintf( fU, L"</HEAD>\n" );
	fwprintf( fU, L"<BODY>\n" );
	fwprintf( fU, L"<center>%s</center><br><br>\n\n", title1 );
	fwprintf( fU, L"<pre>" );
	fwprintf( fU, L"\n%-20s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
	fwprintf( fU, L"%-20s %s<br><br>", L"lista készült:", theApp.getPresentDateTime() );

	fwprintf( fU, m_description );

	str = L"A szürke hátterû bejegyzéseket egyesítettük a zöld hátterû bejegyzéssel.\n\
Ha egy azonos nevû csoportban több különbözõ egyesítés lehetséges, akkor azok a 'g'-group oszlopba található számmal vannak meglülönböztetve.\n\n";
	fwprintf( fU, str );


str.Format( L"\
\n<b>\
%2s %2s %9s %2s %1s %1s \
%8s %-30s %12s %12s %-30s\
</b>\n", \
L"gr", L"st", L"line", L"u", L"m", L"G",\
L"rowid", L"name", L"birth", L"death", L"mother"\
);
	fwprintf( fU, str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSame::openDifferent()
{
	CString fileName = L"peopleDifferent";
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	CString title2( L"AZONOS NEVÛ EMBEREK, AKIK KÜLÖNBÖZNEK EGYMÁSTÓL" ); 

	fwprintf( fD, L"<HEAD>\n" );
	fwprintf( fD, L"<style>\n" );
	fwprintf( fD, L"</style>\n" );
	fwprintf( fD, L"</HEAD>\n" );
	fwprintf( fD, L"<BODY>\n" );
	fwprintf( fD, L"<center>%s</center><br><br>\n\n", title2 );
	fwprintf( fD, L"<pre>" );
	fwprintf( fD, L"\n%-20s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
	fwprintf( fD, L"%-20s %s<br><br>", L"lista készült:", theApp.getPresentDateTime() );


	fwprintf( fD, m_description );

	str.Format( L"\
\n<b>\
%2s %2s %9s %2s %1s %1s \
%8s %-30s %12s %12s %-30s\
</b>\n", \
L"gr", L"st", L"line", L"u", L"m", L"G",\
L"rowid", L"name", L"birth", L"death", L"mother"\
);
	fwprintf( fD, str );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSame::query( CString command )
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
BOOL CSame::query1( CString command )
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
BOOL CSame::query2( CString command )
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
BOOL CSame::query3( CString command )
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
BOOL CSame::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////