// CSameCouples.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SameCouples.h"
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
IMPLEMENT_DYNAMIC(CSameCouples, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::CSameCouples(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameCouples::IDD, pParent)
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



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::~CSameCouples()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameCouples, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_HTML, &CSameCouples::OnHtml)
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_LINE_HUSBAND, &CSameCouples::OnLineHusband)
	ON_COMMAND(ID_LINE_WIFE, &CSameCouples::OnLineWife)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CSameCouples::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CSameCouples::OnInfo)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	CString info;
	info = L"\
Az azonos nevû házastársakat listázzuk, amelyekben a férjek neve azonos és a feleségek neve is azonos.\n\n\
Ezek az esetek többségéban nem csak azonos nevû, de ténylegesen is azonos emberek házasságai, amelyek \
a html fájlban többször vannak megadva különbözõ helyeken: a házasságok lehetnek leszármazottak házasságai, \
vagy házastárs szüleinek házasságai, házastárs további házassságai.\n\
Természetesen az sem kizárt, hogy az azonos nevû házaspárok különbözõ emberek.\n\n\
Az emberek azonosítására a születési és halálozási dátumukat valamint az anyjuk nevét használja a program.\n\n\
Minden azonos nevû házaspárt csoportokban listázunk, és sárga színnel jelöljük az azonosításra használt \
adatok eltérését.\n\n\
Az azonosnak talált embereket és házasságokat összevonjuk.\
";

	AfxMessageBox( info, MB_OKCANCEL|MB_ICONINFORMATION );

	CString caption = L"Azonos nevû házaspárok";
	SetWindowTextW( caption ),

	createColumns();
	sameSpouses();

	if( m_deleted )
	{
		str.Format( L"%d ember összevonásra került", m_deleted );
		AfxMessageBox( str );
	}
	else
		AfxMessageBox( L"Nincs összevonható ember!" );

	return TRUE;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::createColumns()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::sameSpouses()
{
	CString fileName;
	CString first_name1;
	CString title( L"Azonos nevû házaspárok..." ); 


	UINT i;

	COUPLES vcouples;
	CString spouse;
	CString mother_id;


//	CProgressWnd wndP(NULL, title ); 

	wndP.Create( NULL, title );
	wndP.GoModal();
	fileName += L"couplesWithSameName";
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

		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;

		str.Empty();
		if( m_recordset1->RecordsCount() )
			str.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), m_recordset1->GetFieldString(1) );
		vcouples.motherS1 = str;

		vcouples.rowidS1.Trim();
		vcouples.sex_idS1.Trim();
		vcouples.generationS1.Trim();
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
		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;

		str.Empty();
		if( m_recordset1->RecordsCount() )
			str.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), m_recordset1->GetFieldString(1) );
		vcouples.motherS2 = str;


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
	str.Format( L"Azonos házastársak és házasságok összevonása..." );
	wndP.SetText( str );
#endif
	fillTable();

	wndP.DestroyWindow();

	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::fillTable()
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
void CSameCouples::printGroup()
{
	std::sort( vGroup.begin(), vGroup.end(), sortBySource1 );


	data0();
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
void CSameCouples::data0()
{
	rowidS1_0		= vGroup.at(0).rowidS1;
	generationS1_0	= vGroup.at(0).generationS1;
	sourceS1_0		= vGroup.at(0).sourceS1;
	unitedS1_0		= vGroup.at(0).unitedS1;
	lineS1_0		= vGroup.at(0).lineS1;
	spouse1_0		= vGroup.at(0).spouse1;
	birthS1_0		= vGroup.at(0).birthDateS1;
	deathS1_0		= vGroup.at(0).deathDateS1;
	motherS1_0		= vGroup.at(0).motherS1;
	
	rowidS2_0		= vGroup.at(0).rowidS2;
	generationS2_0	= vGroup.at(0).generationS2;
	sourceS2_0		= vGroup.at(0).sourceS2;
	unitedS2_0		= vGroup.at(0).unitedS2;
	lineS2_0		= vGroup.at(0).lineS2;
	spouse2_0		= vGroup.at(0).spouse2;
	birthS2_0		= vGroup.at(0).birthDateS2;
	deathS2_0		= vGroup.at(0).deathDateS2;
	motherS2_0		= vGroup.at(0).motherS2;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listGroup( UINT i )
{
	int		col = 0;
	int		empty = 0;
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
	CString motherS1		= vGroup.at(i).motherS1;

	CString rowidS2			= vGroup.at(i).rowidS2;
	CString generationS2	= vGroup.at(i).generationS2;
	CString sourceS2		= vGroup.at(i).sourceS2;
	CString unitedS2		= vGroup.at(i).unitedS2;
	CString lineS2			= vGroup.at(i).lineS2;
	CString spouse2			= vGroup.at(i).spouse2;
	CString birthS2			= vGroup.at(i).birthDateS2;
	CString deathS2			= vGroup.at(i).deathDateS2;
	CString motherS2		= vGroup.at(i).motherS2;

	str.Format( L"%4s %6s %6s ", cnt, rowidM, rowidS1 );
	fwprintf( fh1, L"%s", str );

// generation
	if( generationS1_0.IsEmpty() || generationS1.IsEmpty() )
	{
		++empty;
	}
	if( i && !generationS1_0.IsEmpty() && !generationS1.IsEmpty() )
	{
		fwprintf( fh1, L"<span style=\"background:yellow\">%1s</span> ", generationS1 );
		col = col | 1 << L_GENERATIONS1;
	}
	else
		fwprintf( fh1, L"%1s ", generationS1 );

	str.Format( L"%1s %1s %8s %-30s ", sourceS1, unitedS1, lineS1, spouse1 );
	fwprintf( fh1, L"%s", str );


// birthDate1
	if( birthS1_0.IsEmpty() || birthS1.IsEmpty() )
	{
		++empty;
	}
	if( !birthS1_0.IsEmpty() && !birthS1.IsEmpty() && birthS1_0 != birthS1 )
	{
		fwprintf( fh1, L"<span style=\"background:yellow\">%-13s</span> ", birthS1 );
		col = col | 1 << L_BIRTHS1;
	}
	else
		fwprintf( fh1, L"%-13s ", birthS1 );


// deathDate1
		if( deathS1_0.IsEmpty() || deathS1.IsEmpty() )
		{
			++empty;
		}
		if( !deathS1_0.IsEmpty() && !deathS1.IsEmpty() && deathS1_0 != deathS1 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-13s</span> ", deathS1 );
			col = col | 1 << L_DEATHS1;
		}
		else
			fwprintf( fh1, L"%-13s ", deathS1 );

// mother1
		if( motherS1_0.IsEmpty() || motherS1.IsEmpty() )
		{
			++empty;
		}
		if( !motherS1_0.IsEmpty() && !motherS1.IsEmpty() && motherS1_0 != motherS1 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-30s</span> ", motherS1 );
			col = col | 1 << L_MOTHERS1;
		}
		else
			fwprintf( fh1, L"%-30s ", motherS1 );


	str.Format( L"%6s ", rowidS2 );
	fwprintf( fh1, L"%s", str );

// generation
	if( generationS2_0.IsEmpty() || generationS2.IsEmpty() )
	{
		++empty;
	}
	if( i & !generationS2_0.IsEmpty() && !generationS2.IsEmpty() )
	{
		fwprintf( fh1, L"<span style=\"background:yellow\">%1s</span> ", generationS2 );
		col = col | 1 << L_GENERATIONS2;
	}
	else
		fwprintf( fh1, L"%-1s ", generationS2 );

	str.Format( L"%1s %1s %8s %-30s ", sourceS2, unitedS2, lineS2, spouse2 );
	fwprintf( fh1, L"%s", str );

// birthDate2
	if( birthS2_0.IsEmpty() || birthS2.IsEmpty() )
	{
		++empty;
	}
	if( !birthS2_0.IsEmpty() && !birthS2.IsEmpty() && birthS2_0 != birthS2 )
	{
		fwprintf( fh1, L"<span style=\"background:yellow\">%-13s</span> ", birthS2 );
		col = col | 1 << L_BIRTHS2;
	}
	else
		fwprintf( fh1, L"%-13s ", birthS2 );


// deathDate2
		if( deathS2_0.IsEmpty() || deathS2.IsEmpty() )
		{
			++empty;
		}
		if( !deathS2_0.IsEmpty() && !deathS2.IsEmpty() && deathS2_0 != deathS2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-13s</span> ", deathS2 );
			col = col | 1 << L_DEATHS2;
		}
		else
			fwprintf( fh1, L"%-13s ", deathS2 );

// mother2
		if( motherS2_0.IsEmpty() || motherS2.IsEmpty() )
		{
			++empty;
		}
		if( !motherS2_0.IsEmpty() && !motherS2.IsEmpty() && motherS2_0 != motherS2 )
		{
			fwprintf( fh1, L"<span style=\"background:yellow\">%-30s</span> ", motherS2 );
			col = col | 1 << L_MOTHERS2;
		}
		else
			fwprintf( fh1, L"%-30s ", motherS2 );


	fwprintf( fh1, L"\n");


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
	m_ListCtrl.SetItemText( nItem, L_MOTHERS1, motherS1 );
	
	m_ListCtrl.SetItemText( nItem, L_ROWIDS2, rowidS2 );
	m_ListCtrl.SetItemText( nItem, L_GENERATIONS2, generationS2 );
	m_ListCtrl.SetItemText( nItem, L_SOURCES2, sourceS2 );
	m_ListCtrl.SetItemText( nItem, L_UNITEDS2, unitedS2 );
	m_ListCtrl.SetItemText( nItem, L_LINES2, lineS2 );
	m_ListCtrl.SetItemText( nItem, L_SPOUSE2, spouse2 );
	m_ListCtrl.SetItemText( nItem, L_BIRTHS2, birthS2 );
	m_ListCtrl.SetItemText( nItem, L_DEATHS2, deathS2 );
	m_ListCtrl.SetItemText( nItem, L_MOTHERS2, motherS2 );

	m_ListCtrl.SetItemData( nItem, col );

	++nItem;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Azonos nevû és születési, halálozási dátumokban nem ellentmondásos házaspárok házassáágainak egyesítése
// A spouse1-bõl és a spouse2-bõl a legkisebb source-ú embert tartja meg

void CSameCouples::processGroup()
{
	CString rowidM			= vGroup.at(0).rowidM;			// ezt a házasságot fogja megtartani, de a spouse2 változhat, 
															// ha a spouse2 összevonása márt hoz ki!!!

// ha a házaspárok születési dátumában nincs ellentmondás, akkor összevonja a spouse1-et
	UINT i;
	for( i = 1; i < vGroup.size(); ++i )
	{
		if( rowidS1_0 == vGroup.at(i).rowidS1 ) continue;  // azonos a spouse1 ember! 
		if( !generationS1_0.IsEmpty() && !vGroup.at(i).generationS1.IsEmpty()  )
		{
			break; // nem lehet két leszármazott egy csoportban!!! Nem foglalkozik ezzel a csoporttal
		}
		if( ( birthS1_0.IsEmpty() || vGroup.at(i).birthDateS1.IsEmpty() ) || birthS1_0 == vGroup.at(i).birthDateS1 )
		{
			if( ( deathS1_0.IsEmpty() || vGroup.at(i).deathDateS1.IsEmpty() ) ||  deathS1_0 == vGroup.at(i).deathDateS1 )
			{
				if( ( motherS1_0.IsEmpty() || vGroup.at(i).motherS1.IsEmpty() ) ||  motherS1_0 == vGroup.at(i).motherS1 )
				{
					if( ( birthS2_0.IsEmpty() || vGroup.at(i).birthDateS2.IsEmpty() ) || birthS2_0 == vGroup.at(i).birthDateS2 )
					{
						if( ( deathS2_0.IsEmpty() || vGroup.at(i).deathDateS2.IsEmpty() ) ||  deathS2_0 == vGroup.at(i).deathDateS2 )
						{
							if( ( motherS2_0.IsEmpty() || vGroup.at(i).motherS2.IsEmpty() ) ||  motherS2_0 == vGroup.at(i).motherS2 )
							{
								theApp.replaceBy( vGroup.at(i).rowidS1, rowidS1_0, vGroup.at(i).sex_idS1, _wtoi( vGroup.at(i).sourceS1 ) );
								m_command.Format( L"DELETE FROM marriages WHERE rowid ='%s'", vGroup.at(i).rowidM );
								if( !theApp.execute( m_command ) ) return;
								++m_deleted;
							}
						}
					}
				}
			}
		}
	}
	if( i != vGroup.size() ) return;		// nem foglalkozik ezzel a csoporttal
	
	// a legkisebb source spouse2 megtartása
	std::sort( vGroup.begin(), vGroup.end(), sortBySource2 ); 
	
	CString spouse2i; 
	CString spouse20		= vGroup.at(0).spouse2;
	CString rowidBy			= vGroup.at(0).rowidS2;

	CString generationS10	= vGroup.at(0).generationS1;
	CString sourceS10		= vGroup.at(0).sourceS1;
	CString birthS10		= vGroup.at(0).birthDateS1;
	CString deathS10		= vGroup.at(0).deathDateS1;

	CString generationS20	= vGroup.at(0).generationS2;
	CString sourceS20		= vGroup.at(0).sourceS2;
	CString birthS20		= vGroup.at(0).birthDateS2;
	CString deathS20		= vGroup.at(0).deathDateS2;
	
	// a legnagyobb hierarhiájú feleség lesz a legnagyobb hierarhiájó fárfi házastársa 
//	m_command.Format( L"UPDATE marriages SET spouse2_id='%s' WHERE rowid='%s'", rowidBy, rowidM );
//	if( !theApp.execute( m_command ) ) return;


	for( i = 1; i < vGroup.size(); ++i )
	{
		spouse2i = vGroup.at(i).spouse2;
		if( rowidBy == vGroup.at(i).rowidS2 ) continue;  // azonos a spouse1 ember! 
		if( !generationS10.IsEmpty() && !vGroup.at(i).generationS1.IsEmpty()  ) continue; // nem lehet két leszármazott egy csoportban!!! Az hiba! Valószínûleg.

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
void CSameCouples::htmlHeader( CString title )
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
	fwprintf( fh1, L"%-20s %s<br><br><br>\n", L"lista készült:", theApp.getPresentDateTime() );

//	str.Format( L"%4s %6s %6s %1s %1s %1s %8s %-30s", cnt, rowidM, rowidS1, generationS1, sourceS1, unitedS1, lineS1, spouse1 );

	str.Format( L"\n<b>%4s %6s %6s %1s %1s %1s %8s %-30s %-13s %-13s %-30s %6s %1s %1s %1s %8s %-30s %-13s %-13s %-30s</b>\n", L"cnt", L"marri", L"rowid", L"G", L"S", L"U", L"line", L"husband", L"birth", L"death", L"mother", L"rowid", L"G", L"S", L"U", L"line", L"wife", L"birth", L"death", L"mother" );

	fwprintf( fh1, str );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::query( CString command )
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
BOOL CSameCouples::query1( CString command )
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
BOOL CSameCouples::query2( CString command )
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
BOOL CSameCouples::query3( CString command )
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
BOOL CSameCouples::query4( CString command )
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

void CSameCouples::OnHtml()
{
	theApp.showHtmlFile( fileSpec );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSameCouples:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_SAMECOUPLES ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_SHOWS, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnLineHusband()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINES1 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnLineWife()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINES2 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;
	UINT mask;

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
		if( iData == - 1 )
		{
			pLVCD->clrText = RGB( 255,0,0 );
		}
		else if( iData & mask )
		{
//			pLVCD->clrText	 = RGB( 0,0,255 );
			pLVCD->clrTextBk = YELLOW;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnInfo()
{
	CString info = L"\
Az azonos nevû házaspárok csoportokban vannak listázva.\n\n\
Az emeber azonosítására születési és halálozási dátumukat valamint anyjuk nevét használjuk. \
Ha az azonosításra használt adatokban eltérés van, azt sárga színnel jelöljük. \
Ha egy csoportban több leszármazott van, akor azt a csoportot nem vonjuk össze. \
Azokat a csoportok, amelyekben nincs sárga szín, összevonásra kerülnek. \
A legmagassabb hierarhiájú emberek maradnak meg, és õk alkotnak házaspárt.\n\n\
A mennyiben a sárgával jelzett eltérések valóban hibás adatok, javítsuk azokat a htm fájlban,\
majd mindent kezdjünk elõlrõl a htm fájl beolvasásásaval.\
";
	AfxMessageBox( info, MB_ICONINFORMATION );
	
}
