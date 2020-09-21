// CSameSpouses.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SameSpouses.h"
#include "afxdialogex.h"
#include "checkParam0.h"
#include "GetLastFirst.h"
#include <algorithm>
#include <vector>       // std::vector


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
	L_GENERATIONS2,
	L_SOURCES2,
	L_UNITEDS2,
	L_LINES2,
	L_ROWIDS2,
	L_SPOUSE2,
	L_BIRTHS2,
	L_DEATHS2,
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
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortBySpouse(const COUPLES &v1, const COUPLES &v2) 
{ 
	CString name1;
	name1.Format( L"%s %s", v1.spouse1, v1.spouse2 );

	CString name2;
	name2.Format( L"%s %s", v2.spouse1, v2.spouse2 );

	return( name1 < name2 );

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortBySource1(const COUPLES &v1, const COUPLES &v2) 
{
	return( v1.sourceS1 < v2.sourceS1 );
}
bool sortBySource2(const COUPLES &v1, const COUPLES &v2) 
{
	return( v1.sourceS2 < v2.sourceS2 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSameSpouses, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameSpouses::CSameSpouses(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameSpouses::IDD, pParent)
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
death_date\
";



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameSpouses::~CSameSpouses()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameSpouses, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_HTML, &CSameSpouses::OnHtml)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameSpouses::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	CString info;
	info = L"\
Azokat a házastársakat listázzuk, amelyekben a férjek neve azonos és a feleségek neve is azonos.\
\r\n\
Két vagy több azonos nevû házastárs személyes adatai közötti különbségeket sárga színnel emeli ki a program.\
\r\n\r\n\
Ha az azonos emberek összevonása elõtt készítjük el a listát, akkor csak olyankor fordulhat elõ több azonos nevû házaspár, \
ha azok egyetlen leszármazotti sorban vannak megadva házastársként.\
\r\n\
Az azonos emberek összevonása megsokszorozhatja az ilyen eseteket, ugyanis ha emberünk azonossá lett egy vagy több más azonos \
nevû emberrel, akkor ezek összevonásra kerültek, de azonos nevû házastársaik nem feltétlenül teljesítik az összevonási \
kritériumot, ezért õk bár azonos nevûek, mégis különbözõ emberként maradnak az adatbázisban.\
\r\n\
";


	CCheckParam0 dlg;
	dlg._caption = L"Azonos nevû házastársak listája";
	dlg._info = info;
	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return TRUE;
	}

	_fullname.Empty();
	if( !dlg._all )
	{
		CGetLastFirst dlg1;
		if( dlg1.DoModal() == IDCANCEL )
		{
			OnCancel();
			return false;
		}

		_fullname	= dlg1._fullname;
		_first_name	= dlg1._firstname;
		_last_name	= dlg1._lastname;
	}
	
	CString caption( L"");
	if( _fullname.IsEmpty() )
	{
		caption = L"Emberek, akiknek több azonos nevû házastársa van.";
	}
	else
		caption.Format( L"%s nevû emberek, akinknek több azonos nevû házastársa van.", _fullname );

	SetWindowTextW( caption ),

	createColumns();
	sameSpouses();
	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::createColumns()
{


	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_CNT,				L"cnt",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,			L"rowid",	LVCFMT_RIGHT,	 60,-1,COL_NUM);

	m_ListCtrl.InsertColumn( L_GENERATIONS1,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES1,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS1,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES1,			L"lineS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS1,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE1,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS1,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS1,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);

	m_ListCtrl.InsertColumn( L_GENERATIONS2,	L"G",		LVCFMT_RIGHT,	 25,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_SOURCES2,		L"S",		LVCFMT_LEFT,	 25,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_UNITEDS2,		L"U",		LVCFMT_RIGHT,	 25,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_LINES2,			L"lineS2",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_ROWIDS2,			L"rowidS1",	LVCFMT_RIGHT,	 60,-1,COL_NUM );
	m_ListCtrl.InsertColumn( L_SPOUSE2,			L"spouse1",	LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHS2,			L"birthS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATHS2,			L"deathS1",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::sameSpouses()
{
	CString fileName;
	CString first_name1;
	CString title( L"Azonos nevû házaspárok..." ); 


	UINT i;

	COUPLES vcouples;
	CString spouse;
	


//	CProgressWnd wndP(NULL, title ); 

	wndP.Create( NULL, title );
	wndP.GoModal();
	fileName += L"spousesWithSameName";
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	htmlHeader( title );

#ifndef _DEBUG
	str.Format( L"Házasságok lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vCouples.clear();
	
	for( i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		vcouples.rowidM	= m_recordset->GetFieldString( MARRIAGES_ROWID );
		vcouples.rowidS1 = m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		vcouples.rowidS2 = m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS1 );
		if( !query1( m_command ) ) return;

		vcouples.rowidS1		= m_recordset1->GetFieldString( P_ROWID );
		vcouples.sex_idS1		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS1			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS1	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS1		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS1		= m_recordset1->GetFieldString( P_UNITED );
		spouse.Format( L"%s %s", m_recordset1->GetFieldString( P_LAST_NAME), m_recordset1->GetFieldString( P_FIRST_NAME ) );
		vcouples.spouse1		= spouse;
		vcouples.birthDateS1	= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathDateS1	= m_recordset1->GetFieldString( P_DEATH_DATE );

		vcouples.rowidS1.Trim();
		vcouples.sex_idS1.Trim();
		vcouples.generationS1.Trim();;
		vcouples.sourceS1.Trim();
		vcouples.unitedS1.Trim();
		vcouples.spouse1.Trim();
		vcouples.birthDateS1.Trim();
		vcouples.deathDateS1.Trim();

		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS2 );
		if( !query1( m_command ) ) return;

		vcouples.rowidS2		= m_recordset1->GetFieldString( P_ROWID );
		vcouples.sex_idS2		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS2			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS2	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS2		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS2		= m_recordset1->GetFieldString( P_UNITED );
		spouse.Format( L"%s %s", m_recordset1->GetFieldString( P_LAST_NAME), m_recordset1->GetFieldString( P_FIRST_NAME ) );
		vcouples.spouse2		= spouse;
		vcouples.birthDateS2	= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathDateS2	= m_recordset1->GetFieldString( P_DEATH_DATE );

		vcouples.rowidS2.Trim();
		vcouples.sex_idS2.Trim();
		vcouples.generationS2.Trim();;
		vcouples.sourceS2.Trim();
		vcouples.unitedS2.Trim();
		vcouples.spouse2.Trim();
		vcouples.birthDateS2.Trim();
		vcouples.deathDateS2.Trim();



		vCouples.push_back( vcouples );
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

#ifndef _DEBUG
	str.Format( L"Lekérdezett adatok rendezése..." );
	wndP.SetText( str );
#endif

	wndP.SetPos(0);
	std::sort( vCouples.begin(), vCouples.end(), sortBySpouse );

#ifndef _DEBUG
	str.Format( L"Lekérdezett adatok listázása..." );
	wndP.SetText( str );
#endif
	fillTable();

	wndP.DestroyWindow();

	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::fillTable()
{

	CString spouse1;
	CString spouse2;
	CString name;
	CString namePrev(L"");
	bool first = true;
	nItem = 0;
	m_cnt = 1;
	
	m_deleted = 0;
	theApp.execute( L"BEGIN" );

	wndP.SetRange( 0, vCouples.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vGroup.clear();
	for( UINT i = 0; i < vCouples.size(); ++i )
	{
		spouse1 =  vCouples.at(i).spouse1;
		spouse2 =  vCouples.at(i).spouse2;
		name.Format( L"%-40s %-40s", spouse1, spouse2 );
		if( name == namePrev )
		{
			if( first )			// a csoport elsõ házaspárjának mentése
			{
				vGroup.push_back( vCouples.at(i-1) );
				first = false;
			}
			vGroup.push_back( vCouples.at(i) );
		}
		else
		{
			if( vGroup.size() )
			{
				++m_cnt;
				printGroup();
			}
			first = true;
		}
		namePrev = name;

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	theApp.execute( L"COMMIT" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::printGroup()
{
	std::sort( vGroup.begin(), vGroup.end(), sortBySource1 );
	for( UINT i = 0; i < vGroup.size();++i )
	{
		listGroup( i );
	}
	fwprintf( fh1, L"\n" );
	nItem = m_ListCtrl.InsertItem( nItem, L"" );
	++nItem;
	
	processGroup();

	vGroup.clear();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::listGroup( UINT i )
{
	CString cnt;
	cnt.Format( L"%d",	m_cnt );

	CString rowidM			= vGroup.at(i).rowidM;

	CString rowidS1			= vGroup.at(i).rowidS1;
	CString generationS1	= vGroup.at(i).generationS1;
	CString sourceS1		= vGroup.at(i).sourceS1;
	CString unitedS1		= vGroup.at(i).unitedS1;
	CString lineS1			= vGroup.at(i).lineS1;
	CString spouse1			= vGroup.at(i).spouse1;
	CString birthS1			= vGroup.at(i).birthDateS1;
	CString deathS1			= vGroup.at(i).deathDateS1;

	CString rowidS2			= vGroup.at(i).rowidS2;
	CString generationS2	= vGroup.at(i).generationS2;
	CString sourceS2		= vGroup.at(i).sourceS2;
	CString unitedS2		= vGroup.at(i).unitedS2;
	CString lineS2			= vGroup.at(i).lineS2;
	CString spouse2			= vGroup.at(i).spouse2;
	CString birthS2			= vGroup.at(i).birthDateS2;
	CString deathS2			= vGroup.at(i).deathDateS2;


	nItem = m_ListCtrl.InsertItem( nItem, cnt );
		
	m_ListCtrl.SetItemText( nItem, L_ROWID, rowidM );

	m_ListCtrl.SetItemText( nItem, L_ROWIDS1, rowidS1 );
	m_ListCtrl.SetItemText( nItem, L_GENERATIONS1, generationS1 );
	m_ListCtrl.SetItemText( nItem, L_SOURCES1, sourceS1 );
	m_ListCtrl.SetItemText( nItem, L_UNITEDS1, unitedS1 );
	m_ListCtrl.SetItemText( nItem, L_LINES1, lineS1 );
	m_ListCtrl.SetItemText( nItem, L_SPOUSE1, spouse1 );
	m_ListCtrl.SetItemText( nItem, L_BIRTHS1, birthS1 );
	m_ListCtrl.SetItemText( nItem, L_DEATHS1, deathS1 );
	
	m_ListCtrl.SetItemText( nItem, L_ROWIDS2, rowidS2 );
	m_ListCtrl.SetItemText( nItem, L_GENERATIONS2, generationS2 );
	m_ListCtrl.SetItemText( nItem, L_SOURCES2, sourceS2 );
	m_ListCtrl.SetItemText( nItem, L_UNITEDS2, unitedS2 );
	m_ListCtrl.SetItemText( nItem, L_LINES2, lineS2 );
	m_ListCtrl.SetItemText( nItem, L_SPOUSE2, spouse2 );
	m_ListCtrl.SetItemText( nItem, L_BIRTHS2, birthS2 );
	m_ListCtrl.SetItemText( nItem, L_DEATHS2, deathS2 );
	++nItem;



	str.Format( L"%4s %6s %6s %1s %1s %1s %8s %-40s %10s %10s  %6s %1s %1s %1s %8s %-40s %10s %10s",\
cnt, rowidM,\
rowidS1, generationS1, sourceS1, unitedS1, lineS1, spouse1, birthS1, deathS1,\
rowidS2, generationS2, sourceS2, unitedS2, lineS2, spouse2, birthS2, deathS2 );
	fwprintf( fh1, L"%s\n", str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Azonos nevû és születési, halálozási dátumokban nem ellentmondásos házaspárok házassáágainak egyesítése
// A spouse1-bõl és a spouse2-bõl a legkisebb source-ú embert tartja meg

void CSameSpouses::processGroup()
{
	CString rowidM			= vGroup.at(0).rowidM;			// ezt a házasságot fogja megtartani, de a spouse2 változhat, 
															// ha a spouse2 összevonása márt hoz ki!!!
	CString rowidBy			= vGroup.at(0).rowidS1;

	CString generationS10	= vGroup.at(0).generationS1;
	CString sourceS10		= vGroup.at(0).sourceS1;
	CString birthS10		= vGroup.at(0).birthDateS1;
	CString deathS10		= vGroup.at(0).deathDateS1;

	CString generationS20	= vGroup.at(0).generationS2;
	CString sourceS20		= vGroup.at(0).sourceS2;
	CString birthS20		= vGroup.at(0).birthDateS2;
	CString deathS20		= vGroup.at(0).deathDateS2;

// ha a házaspárok születési dátumában nincs ellentmondás, akkor összevonja a spouse1-et
	for( UINT i = 1; i < vGroup.size(); ++i )
	{

		if( rowidBy == vGroup.at(i).rowidS1 ) continue;  // azonos a spouse1 ember! 
//		if( !generationS10.IsEmpty() && !vGroup.at(i).generationS1.IsEmpty()  ) continue; // nem lehet két leszármazott egy csoportban!!! Az hiba! Valószínûleg.

		if( ( birthS10.IsEmpty() || vGroup.at(i).birthDateS1.IsEmpty() ) || birthS10 == vGroup.at(i).birthDateS1 )
		{
			if( ( deathS10.IsEmpty() || vGroup.at(i).deathDateS1.IsEmpty() ) ||  deathS10 == vGroup.at(i).deathDateS1 )
			{
				if( ( birthS20.IsEmpty() || vGroup.at(i).birthDateS2.IsEmpty() ) || birthS20 == vGroup.at(i).birthDateS2 )
				{
					if( ( deathS20.IsEmpty() || vGroup.at(i).deathDateS2.IsEmpty() ) ||  deathS20 == vGroup.at(i).deathDateS2 )
					{
						theApp.replaceBy( vGroup.at(i).rowidS1, rowidBy, vGroup.at(i).sex_idS1, _wtoi( vGroup.at(i).sourceS1 ) );

						m_command.Format( L"DELETE FROM marriages WHERE rowid ='%s'", vGroup.at(i).rowidM );
						if( !theApp.execute( m_command ) ) return;
						++m_deleted;
					}
				}
			}
		}
	}


	// a legkisebb source spouse2 megtartása
	std::sort( vGroup.begin(), vGroup.end(), sortBySource2 ); 
	
	rowidBy			= vGroup.at(0).rowidS2;

	generationS10	= vGroup.at(0).generationS1;
	sourceS10		= vGroup.at(0).sourceS1;
	birthS10		= vGroup.at(0).birthDateS1;
	deathS10		= vGroup.at(0).deathDateS1;

	generationS20	= vGroup.at(0).generationS2;
	sourceS20		= vGroup.at(0).sourceS2;
	birthS20		= vGroup.at(0).birthDateS2;
	deathS20		= vGroup.at(0).deathDateS2;

	
	for( UINT i = 1; i < vGroup.size(); ++i )
	{
		if( rowidBy == vGroup.at(i).rowidS2 ) continue;  // azonos a spouse1 ember! 
//		if( !generationS10.IsEmpty() && !vGroup.at(i).generationS1.IsEmpty()  ) continue; // nem lehet két leszármazott egy csoportban!!! Az hiba! Valószínûleg.

		if( ( birthS10.IsEmpty() || vGroup.at(i).birthDateS1.IsEmpty() ) || birthS10 == vGroup.at(i).birthDateS1 )
		{
			if( ( deathS10.IsEmpty() || vGroup.at(i).deathDateS1.IsEmpty() ) ||  deathS10 == vGroup.at(i).deathDateS1 )
			{
				if( ( birthS20.IsEmpty() || vGroup.at(i).birthDateS2.IsEmpty() ) || birthS20 == vGroup.at(i).birthDateS2 )
				{
					if( ( deathS20.IsEmpty() || vGroup.at(i).deathDateS2.IsEmpty() ) ||  deathS20 == vGroup.at(i).deathDateS2 )
					{
						theApp.replaceBy( vGroup.at(i).rowidS2, rowidBy, vGroup.at(i).sex_idS2, _wtoi( vGroup.at(i).sourceS2 ) );
					}
				}
			}
		}
	}
	// A megtartott házasságba a megtartott spouse2-t teszi bele

	m_command.Format( L"UPDATE marriages SET spouse2_id='%s' WHERE rowid='%s'", rowidBy, rowidM );
	theApp.execute( m_command );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameSpouses::htmlHeader( CString title )
{
	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	fwprintf( fh1, L"<center>%s</center><br><br>\n\n", title );
	fwprintf( fh1, m_explanation );
	fwprintf( fh1, L"<pre>" );
	fwprintf( fh1, L"\n%-20s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
//	fwprintf( fh1, L"%-20s %d<br>", L"Összevonások száma:", theApp._iterationCount );
	fwprintf( fh1, L"%-20s %s<br><br><br>\n", L"lista készült:", theApp.getPresentDateTime() );
	
	str.Format( L"\n%12s %6s %6s %1s %1s %1s %-25s %-15s %-15s %-25s %-25s<br>\n", L"rowid", L"line#", L"table#", L"G", L"S", L"U", L"name", L"születés", L"halál", L"apja neve", L"anyja neve" );
	fwprintf( fh1, str );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameSpouses::query( CString command )
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
BOOL CSameSpouses::query1( CString command )
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
BOOL CSameSpouses::query2( CString command )
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
BOOL CSameSpouses::query3( CString command )
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
BOOL CSameSpouses::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSameSpouses::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
