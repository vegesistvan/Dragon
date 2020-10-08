// SameP.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SameP.h"
#include "afxdialogex.h"
#include "FirstLetter.h"
#include <algorithm>

#include "checkParam3.h"
#include "checkParam4.h"
#include "GetLastFirst.h"
#include "html_Lines.h"

#include "version.h"
enum
{
	_ROWID = 0,
	_TABLENUMBER,
	_LINENUMBER,
	_SOURCE,
	_UNITED,
	_GENERATION,
	_SEX_ID,
	_FIRST_NAME,
	_LAST_NAME,
	_BIRTH_DATE,
	_DEATH_DATE,
	_FATHER_ID,
	_MOTHER_ID,
};

enum
{
	S_FIRST_NAME = 0,
	S_LAST_NAME,
	S_SOURCE,
	S_BIRTH_DATE,\
	S_DEATH_DATE,\
};

enum
{
	K_ROWID = 0,
	K_MOTHER_INDEX,
	K_LAST_NAME,
	K_FIRST_NAME,
	K_BIRTH_DATE,
	K_DEATH_DATE,
};

// txt fájl oszlopai
enum
{
	T_SEX_ID = 0,
	T_VOLT,
	T_FIRST,
	T_GROUP,
	T_ROWID,
	T_LINENUMBER,
	T_TABLENUMBER,
	T_SOURCE,
	T_UNITED,
	T_GENERATION,
	T_NAME,
	T_BIRTH_DATE,
	T_DEATH_DATE,
	T_FATHER,
	T_FATHER_BIRTH,
	T_FATHER_DEATH,
	T_MOTHER,
	T_MOTHER_BIRTH,
	T_MOTHER_DEATH,
	T_SPOUSES,
	T_KIDS,
	T_LINENUMBERF,
	T_COLUMNS
};

// CListCtrl táblázat oszlopai
enum
{
	S_SEX_ID,
	S_VOLT,
	S_CNT_PEOPLE,
	S_CNT_SAMENAME,
	S_GROUP,
	S_ROWID,
	S_LINENUMBER,
	S_TABLENUMBER,
	S_UNITED,
	S_SOURCE2,
	S_GENERATION,
	S_NAME,
	S_BIRTH,
	S_DEATH,
	S_FATHER,
	S_FATHER_BIRTH,
	S_FATHER_DEATH,
	S_MOTHER,
	S_MOTHER_BIRTH,
	S_MOTHER_DEATH,
	S_SPOUSES,
	S_KIDS,
	S_LINENUMBERF,
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonApp::OnDeleteSamemarriages()   // privát funkció
//{
//	CSameP identical;
//
//	identical.sameMarriages( 0 );
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySourceS(const SAME &v1, const SAME &v2);
bool sortByGroupS(const SAME &v1, const SAME &v2) ;

IMPLEMENT_DYNAMIC(CSameP, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameP::CSameP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSameP::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

	m_colors.Add( L"aquamarine" );
	m_colors.Add( L"yellow" );
	m_colors.Add( L"deepskyblue" );
	m_colors.Add( L"bisque" );
	m_colors.Add( L"greenyellow" );
	m_colors.Add( L"thistle" ); 
	m_colors.Add( L"hotpink" );
	m_colors.Add( L"lightyellow" );
	m_colors.Add( L"aqua" );
	m_colors.Add( L"moccasin" );

	m_rgb[0] = 0;
	m_rgb[1] = RGB( 255, 255, 102 );
	m_rgb[2] = RGB( 102, 255, 255 );
	m_rgb[3] = RGB( 255, 204, 255 );
	m_rgb[4] = RGB( 153, 255, 204 );
	m_rgb[5] = RGB( 255, 255, 204 );
	m_rgb[6] = RGB( 255, 255, 102 );
	m_rgb[7] = RGB( 102, 255, 255 );
	m_rgb[8] = RGB( 255, 204, 255 );
	m_rgb[9] = RGB( 153, 255, 204 );

	_contractions = theApp.contractions();

	p_fields = L"\
rowid,\
tableNumber,\
lineNumber,\
source,\
united,\
generation,\
sex_id,\
first_name,\
last_name,\
birth_date,\
death_date,\
father_id,\
mother_id\
";
	s_fields = L"\
first_name,\
last_name,\
source,\
birth_date,\
death_date\
";
	k_fields = L"\
rowid,\
mother_index2,\
last_name,\
first_name,\
birth_date,\
death_date\
";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameP::~CSameP()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameP, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()

ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CSameP::OnCustomdrawList)
ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)

ON_COMMAND(ID_HTML_EDIT, &CSameP::OnHtmlEdit)
ON_COMMAND(ID_HTML_SHOWS, &CSameP::OnHtmlShows)
ON_COMMAND(ID_HTML_PEOPLEFATHER, &CSameP::OnHtmlPeoplefather)
ON_COMMAND(ID_HTML_NOTEPAD, &CSameP::OnHtmlNotepad)
ON_COMMAND(ID_CONTRACTED, &CSameP::OnContracted)
ON_COMMAND(ID_NOTCONTRACTED, &CSameP::OnNotcontracted)
ON_COMMAND(ID_ALL, &CSameP::OnAll)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameP::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT();

	CString fileName1( L"sameNameSamePeople" );
	CString fileSpec1;
	CString fileName2( L"sameNameDiffPeople" );
	CString fileSpec2;

	fileName1.Format( L"%s_contracted", theApp.m_databaseName );
	fileName2.Format( L"%s_differenet", theApp.m_databaseName );

	CString name;

	CString info;
	info = L"\
Az alábbi adatokat vizsgáljuk az azonos nevû emberek azonosságának eldöntéséhez:\r\n\r\n\
1. Születési dátumuk\r\n\
2. Halálozási dátumuk\r\n\
3. Apjuk neve és születési/halálozási dátuma\r\n\
4. Anyjuk neve és születési/halálozási dátuma\r\n\
5. Házastáraik között van azonos nevû és születési/halálozási dátumú.\r\n\
\r\n\
Az apa, anya és házastárs nevébõl csak a családnevet és az elsõ keresztnevet használjuk az összehasoonlításhoz.\r\n\r\n\
Két azonos nevû embert azonosnak tekintünk és összevonásra jelölünk, ha a vizsgált adataik között nincs ellentmondás \
és ha a mindkettõnél megadott adatpárok közül legalább paraméterként megadható számú megegyezik.\
\r\n\r\n\
";
	CString extension3 = L"\
A program mûködését az alábbi beállításokkal határozhatjuk meg:\
\r\n\r\n\
1. Az azonosításhoz hány adatpár azonosságát követeljük meg.\r\n\
2. Az összes embert vizsgáljuk, vagy csak a megadott nevû emberekét.\r\n\
3. A kimenet html fájl vagy MFC lista legyen.\r\n\
\r\n\
Ha MFC listát kérünk, akkor a jobb egérgombbal történõ kattintással megjelenõ legördülõ menübõl választható \
funkciókkal lehetõség van a kijelölt emberek eredeti html fájl-sorainak megjelenítésére és azok szerkesztésére.\
\r\n\
";
	CString extension4 = L"\
A program mûködését az alábbi beállításokkal határozhatjuk meg:\
\r\n\r\n\
1. Az azonosításhoz hány adatpár azonosságát követeljük meg.\r\n\
2. Az összes embert vizsgáljuk, vagy csak a megadott nevû emberekét.\r\n\
3. A kimenet html fájl vagy MFC lista legyen.\r\n\
4. 1 összevonási ciklust csináljon vagy addig ismételje a ciklust, amíg van összevonni való ember.\
\r\n\
\r\n\
Ha MFC listát kérünk, akkor a jobb egérgombbal történõ kattintással megjelenõ legördülõ menübõl választható \
funkciókkal lehetõség van a kijelölt emberek eredeti html fájl-sorainak megjelenítésére és azok szerkesztésére.\
\r\n\
";


	BOOL all;
	_azonos = 4;

	nItem = 0;

	if( _onlyList )
	{
		CCheckParam3 dlg;
		dlg._caption = L"Azonos nevû emberek azonosítási listája";
		dlg._info = info + extension3;
		if( dlg.DoModal() == IDCANCEL )
		{
			OnCancel();
			return false;
		}

		_unite	= FALSE;
		_html	= dlg._html;
		_loop	= 0;			// 0: 1 ciklus
		_azonos	= dlg._azonos;
		all		= dlg._all;
	}
	else
	{
		CCheckParam4 dlg;
		dlg._info	= info + extension4;
		dlg._azonos = _azonos;
		if( dlg.DoModal() == IDCANCEL )
		{
			OnCancel();
			return false;
		}


		_unite	= TRUE;
		_html	= dlg.m_list;
		_loop	= dlg.m_loop;	// 1: amíg van egyesítés	0: 1 ciklus
		_azonos	= dlg._azonos;
		all		= dlg.m_all;
	}

	_fullname.Empty();
	if( !all )
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

	if( !_fullname.IsEmpty() )
	{
		if( _unite )
			str.Format( L"'%s' nevû emberek azonosítása és az azonosak összevonása", _fullname );
		else
			str.Format( L"'%s' nevû emberek azonosítása", _fullname );
	}
	else
	{
		if( _unite )
			str.Format( L"Azonos nevû emberek azonosítása és az azonos emberek összevonása" );
		else
			str.Format( L"Azonos nevû emberek azonosítása" );
	}
	CProgressWnd wndP(NULL, str ); 
	wndP.GoModal();

	if( !_unite )
	{
		fileName1 += L"_list";
		fileName2 += L"_list";
	}
	else
	{
		if( !_fullname.IsEmpty() )
			_tag.Format( L"P_%s", _fullname );
		else
			_tag = L"P";
	}

	std::vector<CString> vPeople;
	theApp.setStartTime();

	for( int i = 0; ; ++i )		// cilusok az egyesítésre
	{
		m_allP		= 0;
		m_unitedP	= 0;
		m_maradtP	= 0;
		m_deletedP	= 0;
		
		m_deletedM	= 0;
		m_maradtM	= 0;

		_contractions = theApp.contractions();
		if( _unite ) 	theApp.newDatabase(_tag );

		fileName1.Format( L"%s_contracted", theApp.m_databaseName );
		fileName2.Format( L"%s_differenet", theApp.m_databaseName );

		if( !_unite )
		{
			fileName1 += L"_list";
			fileName2 += L"_list";
		}

		fileSpec1 = theApp.openHtmlFile( &fh1, fileName1, L"w+" );
		fileSpec2 = theApp.openHtmlFile( &fh2, fileName2, L"w+" );
		htmlHeader();

		if( (fileSpec = theApp.openTextFile( &theApp.fl, fileName1, L"w+" ) ) == L"" )
		{
			OnCancel();
			return false;
		}

		createColumns();
	
#ifndef _DEBUG
		str.Format( L"Házastársak számának meghatározása (%d. ciklus)", i+1 );
		wndP.SetText( str );
#endif
		m_command = L"SELECT count() FROM marriages";
		if( !query( m_command ) )
		{
			OnCancel();
			return false;
		}
		m_inputM = _wtoi( m_recordset->GetFieldString( 0 ) );

		if( _fullname.IsEmpty() )
			m_command.Format( L"SELECT %s FROM people WHERE last_name != '' AND first_name != '' ORDER BY last_name, first_name, source", p_fields );
		else if( _first_name.IsEmpty() )
			m_command.Format( L"SELECT %s FROM people WHERE last_name LIKE '%s%c' ORDER BY last_name, first_name, source", p_fields, _last_name,'%' );
		else
			m_command.Format( L"SELECT %s FROM people WHERE last_name = '%s' AND first_name = '%s' ORDER BY last_name, first_name, source", p_fields, _last_name, _first_name );

#ifndef _DEBUG
		str.Format( L"Emberek belvasása az adatbázisból (%d. ciklus)", i+1 );
		wndP.SetText( str );
#endif
		if( !query( m_command ) )
		{
			OnCancel();
			return false;
		}
		m_inputP	= m_recordset->RecordsCount();;

		wndP.SetRange(0, m_recordset->RecordsCount());
		wndP.SetPos(0);
		wndP.SetStep(1);

#ifndef _DEBUG
		str.Format( L"Emberek azonosítása (%d. ciklus)", i+1 );
		wndP.SetText( str );
#endif

		vSame.clear();
		v_spouses1.clear();
		v_spouses2.clear();
		v_kids1.clear();
		v_kids2.clear();
		v_Replace.clear();

		// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
		m_cnt = 0;
		pushSame();			// beeszi az elsõ embert
		m_recordset->MoveNext();
		for( UINT j = 1; j < m_recordset->RecordsCount(); ++j, m_recordset->MoveNext() )
		{
			if( sameLastName() )
			{
				pushSame();			// gyûjti az azonos nevû embereket
			}
			else
			{
				if( vSame.size() > 1 )	processSame();		// ha új ember jön, akkor elõtte feldolgozza az eddigieket
				// akár volt vSame akár nem, mindent töröl
				vSame.clear();
				v_spouses1.clear();
				v_spouses2.clear();
				v_kids1.clear();
				v_kids2.clear();
				pushSame();
			}
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled()) break;
		}
		if( wndP.Cancelled() ) 
		{
			htmlFooter();
			fclose( fh1 );
			fclose( fh2 );
			break;
		};

		if( vSame.size() > 1 ) processSame();   // ha az utolsó gyûjtésben több ember van
		vSame.clear();
		v_spouses1.clear();
		v_spouses2.clear();
		v_kids1.clear();
		v_kids2.clear();

		if( _unite )
		{
			unite( i );
			theApp.setUserVersion( theApp._iterationCount + 1 );
			str = fileSpec1.Right( 17 );
			m_command.Format( L"INSERT INTO united ( date, mode, input, united, deleted, marriages) VALUES ( '%s','%d','%d','%d', '%d',  '%d')", str.Left(12), _azonos, m_inputP, m_unitedP, m_deletedP, m_deletedM );
			theApp.execute( m_command );
		}
		v_Replace.clear();

		htmlFooter();
		fclose( fh1 );
		fclose( fh2 );
		fclose( theApp.fl );

		if( !_loop )	break;
		if( ! m_unitedP ) break;			// már nem vont össze emebereket
	}
	wndP.DestroyWindow();

	if( _html )
	{
		theApp.showHtmlFile( fileSpec1 );
		theApp.showHtmlFile( fileSpec2 );
		OnCancel();
	}
//	else
//		fillTable( 2 );

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::createColumns()
{
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	
	m_ListCtrl.InsertColumn( S_SEX_ID,			L"sex",			LVCFMT_RIGHT,	 20,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( S_VOLT,			L"volt",		LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_CNT_PEOPLE,		L"#P",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_CNT_SAMENAME,	L"#S",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GROUP,			L"grp",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_ROWID,			L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_LINENUMBER,		L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_TABLENUMBER,		L"table#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_UNITED,			L"U",			LVCFMT_LEFT,	 20,-1,COL_NUM );
	m_ListCtrl.InsertColumn( S_SOURCE2,			L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_GENERATION,		L"G",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_NAME,			L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( S_BIRTH,			L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_DEATH,			L"halálozás",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_FATHER,			L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( S_FATHER_BIRTH,	L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_FATHER_DEATH,	L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_MOTHER,			L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( S_MOTHER_BIRTH,	L"születés",	LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_MOTHER_DEATH,	L"halál",		LVCFMT_LEFT,	 70,-1,COL_NUM);
	m_ListCtrl.InsertColumn( S_SPOUSES,			L"házastársak",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( S_KIDS,			L"gyerekek",	LVCFMT_LEFT,	500,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( S_LINENUMBERF,		L"line#F",		LVCFMT_LEFT,	 60,-1,COL_HIDDEN );
	m_numberOfColumns = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// az aktuális rekordban lévõ ember neve azonos-e az utoljára push-olt emnberével?
BOOL CSameP::sameLastName()
{
	if( !vSame.size() ) return FALSE;		// nincs benne semmi

	CString name;
	CString nameLast;
	CString firstName;
	CString lastName;
	int last = vSame.size() - 1;

//	m_recordset->MoveLast();
	firstName	= getFirstWord( m_recordset->GetFieldString( _FIRST_NAME ) );
	lastName	= m_recordset->GetFieldString( _LAST_NAME );
	name.Format( L"%s %s", lastName, firstName );

	nameLast	= vSame.at( last ).name; 

	if( !name.Compare( nameLast ) )
		return TRUE;  //  azonos nevû emberek
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::pushSame()
{
	SAME	same;
	SPOUSES2	sp;
//	SCHILDREN	sChildren;

	CString		father_id;
	CString		mother_id;
	
	sameClear( &same );

	same.repleceBy.Empty();
	same.rowid.Empty();
	same.same = ' ';
	same.group = 0;
	same.identical = L"";

//	same.index	= j + 1;
	same.rowid			= m_recordset->GetFieldString( _ROWID );
	same.lineNumber		= m_recordset->GetFieldString( _LINENUMBER );
	same.tableNumber	= m_recordset->GetFieldString( _TABLENUMBER );
	same.generation		= m_recordset->GetFieldString( _GENERATION );
	same.source			= _wtoi( m_recordset->GetFieldString( _SOURCE ) );
	same.united			= m_recordset->GetFieldString( _UNITED );
	same.sex_id			= m_recordset->GetFieldString( _SEX_ID );
	same.birth_date		= m_recordset->GetFieldString( _BIRTH_DATE );
	same.death_date		= m_recordset->GetFieldString( _DEATH_DATE );

	same.name.Format( L"%s %s", m_recordset->GetFieldString(_LAST_NAME), getFirstWord( m_recordset->GetFieldString( _FIRST_NAME ) ) );

	father_id = m_recordset->GetFieldString( _FATHER_ID );
	m_command.Format( L"SELECT last_name, first_name, lineNumber, birth_date, death_date FROM people WHERE rowid ='%s'", father_id );
	if( !query1( m_command ) ) return;
	same.father.Format( L"%s %s", m_recordset1->GetFieldString(0), getFirstWord( m_recordset1->GetFieldString( 1 ) ) );
	same.father.Trim();  // ez üres név miatt kell!
	same.lineNumberF	= m_recordset1->GetFieldString( 2 );
	same.birth_father	= m_recordset1->GetFieldString( 3 );
	same.death_father	= m_recordset1->GetFieldString( 4 );

	mother_id = m_recordset->GetFieldString( _MOTHER_ID );
	m_command.Format( L"SELECT last_name, first_name, lineNumber, birth_date, death_date FROM people WHERE rowid ='%s'", mother_id );
	if( !query1( m_command ) ) return;
	same.mother.Format( L"%s %s", m_recordset1->GetFieldString(0), getFirstWord( m_recordset1->GetFieldString( 1 ) ));
	same.mother.Trim();   // ez üres név miatt kell!
	same.lineNumberM	= m_recordset1->GetFieldString( 2 );
	same.birth_mother	= m_recordset1->GetFieldString( 3 );
	same.death_mother	= m_recordset1->GetFieldString( 4 );

	vSame.push_back( same );

	getSpouses( same.rowid, same.sex_id );
	getKids( same.rowid, same.sex_id );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::processSame()
{
// azonos nevû emberek listázása ás gyûjtése a vReplace vektornba 

	std::sort( vSame.begin(), vSame.end(), sortBySourceS );
	std::sort( v_spouses1.begin(), v_spouses1.end(), sortByName );
	std::sort( v_spouses2.begin(), v_spouses2.end(), sortByName );
	std::sort( v_kids1.begin(), v_kids1.end(), sortByNameKids );
	std::sort( v_kids2.begin(), v_kids2.end(), sortByNameKids );

	CString name = vSame.at(0).name;
	if( !vSame.at(0).name.Compare( L"Foglár Anna" ) )
		int z = 1;

	int volt = markGroup();		// a vSame vektorban megjelöli az összevonható emebereket csoportba osztva, vSame.group.al megjelölve
								// volt == TRUE, ha volt összevonható, megjelölt elem
								// a volt alapján különbözõ fájlokba lesz a nyomtatás!
	std::sort( vSame.begin(), vSame.end(), sortByGroupS );

	int cnt = 1;
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		listRecord( i, cnt, volt );
		++cnt;
	}

	if( volt )
		fwprintf( fh1, L"<br>" );
	else
		fwprintf( fh2, L"<br>" );
	fwprintf( theApp.fl, L"\n" );

	if( _unite )
	{
// azonos embercsoportok összegyûjtése az összevonáshoz, törléshez
		BOOL FIRST = TRUE;
		int indexFirst;
		for( UINT group = 1; group < vSame.size(); ++group )
		{
			for( UINT i = 0; i < vSame.size(); ++i )
			{
				if( vSame.at(i).group == group )
				{
					if( FIRST )
					{
						FIRST = FALSE;
						indexFirst = i;
					}
					else
					{
						to_v_Replace( i, indexFirst );
					}
				}
			}
			FIRST = TRUE;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos emeberek megjelölése a vSame.group-ban
//
//	group == 0 nem tartoznak csoporthoz ( alapbeállítás )
//	group != 0 az i. group-hoz tartozóak azonos emberek
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSameP::markGroup()
{
	int		group = 1;
	int		cnt;
	int		volt = 0;
	CString rowid;
	CString name;

	for( UINT i = 0; i < vSame.size(); ++i )
	{
		cnt = 0;

		name = vSame.at(i).name;

//		if( !vSame.at(i).name.Compare( L"Barna Júlia" ) )
//			int x = 0;
		if( !vSame.at(i).group )
		{
			for( UINT k = i + 1; k < vSame.size(); ++k )
			{

				if( !vSame.at(k).group )		// még nincs kiválasztva
				{
//					if( !vSame.at(k).name.Compare( L"Barna Júlia" ) )
//						int z = 0;
					if( identical( i, k ) )
					{
						++cnt;
						++volt;
						vSame.at(i).group = group;		// i ésa k a group-adik csooportba tartozik
						vSame.at(k).group = group;
					}
				}
			}
		}
		if( cnt ) ++group;								// ha volt csoportba sorolás, akkor megnöveli group-ot
	}
	return volt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i,j a vSame vektor indexei, amiket vizsgálunk
BOOL CSameP::identical( int i, int j )
{
	if( !vSame.at(i).name.Compare( L"Lipthay Sándor" ) && vSame.at(i).birth_date == 1767)
	{
		int z =0;
//		if( (i == 18 && j == 19) || (i == 19 && j == 18) )
//			int k = 0;
	}
	m_identical.Empty();
	m_timeconflict.Empty();

	CString rowidI = vSame.at(i).rowid;
	CString rowidJ = vSame.at(j).rowid;

//	if( !rowidI.Compare(L"103053") && !rowidJ.Compare( L"83135" ) || !rowidJ.Compare(L"103053") && !rowidI.Compare( L"83135" ))
//		int k = 3;


	int	source1 = vSame.at(i).source;;
	int	source2 = vSame.at(j).source;;;
	if( source1 == 1 && source2 == 1 )	return FALSE;			// mindkét ember leszármazott, nem lehetnek azonosak

	m_azonos = 0;
	if( sameBirth(i,j ) != -1 )
	{
		if( sameDeath(i,j ) != -1 )
		{
			if( sameFather(i,j ) != -1 )
			{
				if( sameMother(i,j ) != -1 )
				{
					if( sameSpouse(i,j ) != -1 )
					{
//						if( sameKids(i,j ) != -1 )		// a gyerekek mindkét embernél való elõfordulását nem jó vizsgálni,
														// mert vannak 3-4-es típusú emberek, akinek csak 1 és különbözõ 2-es
														// típusú gyerekekik annak.
						{
							if( timeConflict( i, j ) != -1 )
							{
								vSame.at( j ).identical = m_identical;
								if( m_azonos >= _azonos ) return TRUE;
//								if( !m_identical.IsEmpty() ) return TRUE;		// volt legalább 1 összehasonlítható adat-pár
							}
							else
							{
								vSame.at(i).timeConflict = L"T";
								vSame.at(j).timeConflict = L"T";
							}
						}
					}
				}
			}
		}
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ret:
// 0:	nincs mindkettõnek dátuma, az azonosítás nem végezhetõ el
// -1:	mindkkettõnek van dátuma, és azok különbözzõk
// 1:	mindkettõnek van dátuma és azok egyenlõk
int CSameP::sameBirth( int i, int j )
{
	CString birthDate1 = vSame.at(i).birth_date;
	CString birthDate2 = vSame.at(j).birth_date;

	if( birthDate1.IsEmpty() || birthDate2.IsEmpty() )	return 0;	// az azonosítás nem végezhetõ el, folytassuk
	if( !birthDate1.Compare( birthDate2 ) )
	{
		m_identical += L"B";
		++m_azonos;
		return 1;													// azonosak;
	}
	return -1; //FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ret:
// 0:	nincs mindkettõnek dátuma, az azonosítás nem végezhetõ el
// -1:	mindkkettõnek van dátuma, és azok különbözzõk
// 1:	mindkettõnek van dátuma és azok egyenlõk
int CSameP::sameDeath( int i, int j )
{
	CString deathDate1 = vSame.at(i).death_date;
	CString deathDate2 = vSame.at(j).death_date;

	if( deathDate1.IsEmpty() || deathDate2.IsEmpty() ) 
		return 0;											// az azonosítás nem végezhetõ el, folytassuk
	if( !deathDate1.Compare( deathDate2 ) )
	{
		m_identical += L"D";
		++m_azonos;
		return 1;											// mindkettõnél meg van adva és azonosak
	}
	return -1;												// mindkettõnél meg vanb adva, de különböznek
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ret:
// 0:	nincs mindkettõnek megadva, az azonosítás nem végezhetõ el
// -1:	mindkettõ meg van adva, és azok különbözzõk
// 1:	mindkettõ meg van adva és azok egyenlõk
int CSameP::sameFather( int i, int j )
{
	CString father1;
	CString father2;
	CString birth1;
	CString birth2;
	CString death1;
	CString death2;

//	if( !vSame.at(i).name.Compare( L"Barna Júlia" ) && i == 2 && j == 5 )
//		int z = 7;

	father1 = vSame.at(i).father;
	father2 = vSame.at(j).father;


	if( father1.IsEmpty() || father2.IsEmpty() )
		return 0; 										// nincsenek vagy nem összehasonlítható, elfogadjuk
	if( father1.Compare( father2 ) )
		return -1;										// mindkét szülõ van, nem azonos a szülõk neve 
														// mindkét embernek meg van adva az apjuk és azok azonos nevûek
	birth1 = vSame.at(i).birth_father;
	birth2 = vSame.at(j).birth_father;
	death1 = vSame.at(i).death_father;
	death2 = vSame.at(j).death_father;
	if( birth1 == -1 || death1 == -1 )					// a születés vagy a halál összehasonlítható és különbözik  !!ez megnézni!!!!!!!!!!!!!
		return -1;

	m_identical += L"F";								
	++m_azonos;
	return 1;											// mindkét embernek meg van adva az apja és azok azonos nevûek és a 
														// születési halálozási dátumuk sem mond ellent egymásnak
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ret:
// 0:	nincs mindkettõnek megadva, az azonosítás nem végezhetõ el
// -1:	mindkettõ meg van adva, és azok különbözzõk
// 1:	mindkettõ meg van adva és azok egyenlõk
int CSameP::sameMother( int i, int j )
{

//	if( !vSame.at(i).name.Compare( L"Barna Júlia" ) && i == 2 && j == 5 )
//		int z = 7;

	CString mother1 = vSame.at(i).mother;
	CString mother2 = vSame.at(j).mother;

	if( mother1.IsEmpty() || mother2.IsEmpty() )
		return 0; 										// nincsenek vagy nem összehasonlítható, elfogadjuk
	if( mother1.Compare( mother2 ) )
		return -1;										// mindkét szülõ van, nem azonos a szülõk neve 

	CString birth1 = vSame.at(i).birth_mother;
	CString birth2 = vSame.at(j).birth_mother;
	CString death1 = vSame.at(i).death_mother;
	CString death2 = vSame.at(j).death_mother;

	if( birth1 == -1 || death1 == -1 )					// a születés vagy a halál összehasonlítható és különbözik
		return -1;

	++m_azonos;
	m_identical += L"M";
	return 1;											// mindkét embernek meg van adva az anyja és azok neve azonos,
														// valamint születési és halálozási dátumuk is azonos, ha meg vannak adva
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszármazott házastársai fel vannak sorolva, tehát egy embernek több házastársa is lehet
// Leszármazott házastásainak további házasságai is fel vannak sorolva, neki is több házastársa lehet
// Leszármazott házastársa szüleinél nincsenek az esetleges további házasságok megadva, nekik csak 1 házastárs van megadva
// Nem vizsgájuk, hogy milyen emebereket hasonlítunk össze, ezért az összes házastársak között keressük az egyformát
// Ha két vizsgált azonos nevû ember leszármazott házastársának egyik szülõje, de más házastársa van, akkor azonosságuk nem 
// állapítható meg.
//
// AZ i-vel és j-vel megadott emberek házastársainak összehasonlítása
// Ha a házastársak vektoraiban van ugyanolyan nevû házastárs, akkor azonos emeberek
// return	0 - nem végezhetõ el az azonosítás, mert nincs házastársa
//			1 - elvégezhetõ az azonosítás és van azonos házastárs, tehát azonos ember
//			2 - elvégezhetõ az azonosítás de nincs egyforma házastárs, tehát nem azono sember
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSameP::sameSpouse( int i, int j )
{
	SPOUSES2 sp;
	CString rowid1 = vSame.at(i).rowid;
	CString rowid2 = vSame.at(j).rowid;

	CString sex_id1 = vSame.at(i).sex_id;
	CString sex_id2 = vSame.at(j).sex_id;

	CString name1;
	CString name2;
	CString birthDate1;
	CString deathDate1;
	CString birthDate2;
	CString deathDate2;
	int birthR;
	int deathR;

//	if( i == 2 && j == 5 )
//		int p = 1;

	CString name = vSame.at(i).name;
	// v_spouses1-ben az azonos nevû emberek összes házastársa benne van. A férj azonosítja a rowid jeköki ki az adott emberhez tartozó 
	// házastársat. 
	// v_spouses2 a v_spouses1 másolata
	int cnt1 = 0;
	int cnt2 = 0;
	for( UINT i = 0; i < v_spouses1.size(); ++i )
	{
		if( v_spouses1.at(i).rowid != rowid1 ) continue;

		name1		= v_spouses1.at(i).name;
		birthDate1	= v_spouses1.at(i).birth_date;
		deathDate1	= v_spouses1.at(i).death_date;
		++cnt1;
//		if( !name1.Compare( L"Kvassay Franciska" ) )
//			int z = 1;
		for( UINT j = 0; j < v_spouses2.size(); ++j )
		{
			if( v_spouses2.at(j).rowid != rowid2 ) continue;

			++cnt2;
			name2 = v_spouses2.at(j).name;
//			if( !name1.Compare( L"Kapy Anna" ) && !name2.Compare( L"Kapy Anna" ) )
//				int z = 1;
			if( !name1.Compare( name2 ) )
			{
				birthDate2	= v_spouses2.at(j).birth_date;
				deathDate2	= v_spouses2.at(j).death_date;
				birthR = dateSame( birthDate1, birthDate2 );
				deathR = dateSame( deathDate1, deathDate2 );
				if( birthR != -1 && deathR != -1 )
				{
					++m_azonos;
					m_identical += L"S";
					return 1;												// van közös házastárs
				}
				return -1; 
			}
		}
	}
	if( cnt1 == 0 || cnt2 == 0 ) return 0;								// az azonosítás nem volt elvégezhetõ
	return -1;															// mindkettõnél meg van adva, de nincs közös házastárs
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSameP::sameKids( int i, int j )
{
	KIDS kids;
	CString rowid1 = vSame.at(i).rowid;
	CString rowid2 = vSame.at(j).rowid;

	CString sex_id1 = vSame.at(i).sex_id;
	CString sex_id2 = vSame.at(j).sex_id;

	CString name1;
	CString name2;
	CString birthDate1;
	CString deathDate1;
	CString birthDate2;
	CString deathDate2;
	int birthR;
	int deathR;

//	if( i == 2 && j == 5 )
//		int p = 1;

	CString name = vSame.at(i).name;

	int cnt1 = 0;
	int cnt2 = 0;
	for( UINT i = 0; i < v_kids1.size(); ++i )
	{
		if( v_kids1.at(i).rowid != rowid1 ) continue;

		name1		= v_kids1.at(i).name;
		birthDate1	= v_kids1.at(i).birth_date;
		deathDate1	= v_kids1.at(i).death_date;
		++cnt1;
//		if( !name1.Compare( L"Kvassay Franciska" ) )
//			int z = 1;
		for( UINT j = 0; j < v_kids2.size(); ++j )
		{
			if( v_kids2.at(j).rowid != rowid2 ) continue;

			++cnt2;
			name2 = v_kids2.at(j).name;
//			if( !name1.Compare( L"Kapy Anna" ) && !name2.Compare( L"Kapy Anna" ) )
//				int z = 1;
			if( !name1.Compare( name2 ) )
			{
				birthDate2	= v_kids2.at(j).birth_date;
				deathDate2	= v_kids2.at(j).death_date;
				birthR = dateSame( birthDate1, birthDate2 );
				deathR = dateSame( deathDate1, deathDate2 );
				if( birthR != -1 && deathR != -1 )
				{
					++m_azonos;
					m_identical += L"C";
					return 1;												// van közös házastárs
				}
				return -1; 
			}
		}
	}
	if( cnt1 == 0 || cnt2 == 0 ) return 0;								// az azonosítás nem volt elvégezhetõ
	return -1;															// mindkettõnél meg van adva, de nincs közös házastárs
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSameP::timeConflict( int i, int j )
{
	return 0;
	int ret;
	CString rowidI = vSame.at(i).rowid;
	CString birthI = vSame.at(i).birth_date;
	CString deathI = vSame.at(i).death_date;

	CString rowidJ = vSame.at(j).rowid;
	CString birthJ = vSame.at(j).birth_date;
	CString deathJ = vSame.at(j).death_date;

	CString birthK;
	CString deathK;

	//az i. ember gyereke lehet-e a j. ember gyereke is a születési és a halálozási dátum alapján?
	for( UINT ix = 0; ix < v_kids1.size(); ++ix )
	{
		if( !v_kids1.at(ix).rowid.Compare( rowidI ) )
		{
			birthK = v_kids1.at(ix).birth_date;
			if( !birthK.IsEmpty() )
			{
				ret = dateDiff( birthK, birthJ, 0 );  
				if( ret == 1  )
					return -1;		// birthK < birthJ   nem lehet jó

				ret = dateDiff( birthK, deathJ, 0 );  
				if( ret == 2 ) 
					return -1;		// birthK > deathJ   nem lehet jó  
			}
		}
	}
	//az j. ember gyereke lehet-e az i. ember gyereke is a születési és a halálozási dátum alapján?
	for( UINT ix = 0; ix < v_kids1.size(); ++ix )
	{
		if( !v_kids1.at(ix).rowid.Compare( rowidJ ) )
		{
			birthK = v_kids1.at(ix).birth_date;
			if( !birthK.IsEmpty() )
			{
				ret = dateDiff( birthK, birthI, 0 );  
				if( ret == 1 )	
					return -1;		// birthK < birthJ   nem lehet jó

				ret = dateDiff( birthK, deathI, 0 );  
				if( ret == 2  ) 
					return -1;		// birthK > deathJ   nem lehet jó  
			}
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellenõrzi, hogy date1 kisebb-e mint date2
// return	0 - nem lehet összehasonlítani, mert egyik vagy másik nem szabályos dátum
//			1 - date1 < date2 
//          2 - date1 > date2;
//          3 - date1 == date2
//
// ha csak az év van megadva, akkor azt yyyy.00.00 ra egészíti ki
//
int CSameP::dateDiff( CString date1, CString date2, int month  )
{
	CString str1;
	CString str2;
	CString d1;
	CString d2;

	int length1 = date1.GetLength();
	int length2 = date2.GetLength();

	if( length1 == 8 )
		date1 += "01";
	if( length2 == 8 )
		date2 += "01";

	if( length1 != 12  && length2 != 12 ) return 0;
	

	str1 = getDateI( date1, month );
	str2 = getDateI( date2, 0 );

	_int64 time1 = _wtoi64( str1 );
	_int64 time2 = _wtoi64( str2 );


	d1 = theApp.getDateStrFromI( str1 );
	d2 = theApp.getDateStrFromI( str2 );

	if( time1 < time2 )			// apa halála + 9 hónap kisebb mint a gyerek születése
	{
		return 1;
	}
	if( time1 == time2 ) return 3;
	return 2;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSameP::getDateI( CString dateS, int month ) 
{
	CString dateI;
	CString datum;
	CString modifier;

	dateI.Empty();

	int length = dateS.GetLength();
	if( length == 4 )
	{
		str.Format( L"%s.01.01", dateS );
		dateS = str;
	}

	dateS.Replace('.','-');
	datum.Format( L"%s 00:00:00.000", dateS );
	modifier.Format( L"+%d month", month );
	m_command.Format(L"SELECT strftime('%%s', '%s 00:00:00.000', '%s')",dateS, modifier);  // pontos datum-idõ kell, hogy 1970.01.01 -et == 0-ra 
	if( !query2( m_command ) ) return dateI;
	dateI = m_recordset2->GetFieldString(0);

	dateI.Trim();
	if(dateI.IsEmpty()) dateI=L"0";
	return dateI;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CSameP::dateSame( CString date1, CString date2 )
{
	if( date1.IsEmpty() || date2.IsEmpty() )
	{
		return 0;							// nem összehasonlítható, talán jó lesz
	}
	if( !date1.Compare( date2 ) )			
	{
		return 1;							// összehasonlítható és azonos
	}
	return -1;								// összehasonlítható és nem azonos;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A vSame vektor i. elemének kiírása az fh1 vagy fh2 fájlba
//
// i	a vSame vektor indexe
// cnt	a sor sorszáma
// volt	TRUE	= összevont emberek, kiírása af fh1 fájlba
//		FALSE	= nem összevont emberek, kiírsa az fh2 fájlba
void CSameP::listRecord(  int i, int cnt, int volt )
{
	CString color;
	int colorIndex;
	int col;
	CString same( L"  ");

	int		group		= vSame.at(i).group;		// az ember összevont-e?
	CString rowid		= vSame.at(i).rowid;
	CString lineNumber	= vSame.at(i).lineNumber;
	CString tableNumber = vSame.at(i).tableNumber;
	int		source		= vSame.at(i).source;
	CString united		= vSame.at(i).united;
	CString generation	= vSame.at(i).generation;

	CString sex_id		= vSame.at(i).sex_id;
	CString name		= vSame.at(i).name;
	CString birth_date	= vSame.at(i).birth_date;
	CString death_date	= vSame.at(i).death_date;
	CString father		= vSame.at(i).father;
	CString mother		= vSame.at(i).mother;

	CString birth_father = vSame.at(i).birth_father;
	CString death_father = vSame.at(i).death_father;
	CString birth_mother = vSame.at(i).birth_mother;	
	CString death_mother = vSame.at(i).death_mother;

	CString voltS;
	CString sourceS;
	CString groupS;
	CString cntS ;
	CString iS;
	CString grp;

	if( group )
		same.Format( L"%d%s", group, vSame.at(i).identical );
	else
		same.Format( L"%s", vSame.at(i).timeConflict );


	CString spouse;
	CString spouses;

	CString kid;
	CString kids;

	col = 0;
	for( UINT j = 0; j < v_spouses1.size(); ++ j )
	{
		if( v_spouses1.at(j).rowid == rowid )
		{
			spouse.Format( L"%-25s(%s) %-10s %-10s ", v_spouses1.at(j).name, v_spouses1.at(j).oder, v_spouses1.at(j).birth_date, v_spouses1.at(j).death_date );
			spouse.Format( L"%-25s %-10s %-10s ", v_spouses1.at(j).name, v_spouses1.at(j).birth_date, v_spouses1.at(j).death_date );
			spouses += spouse;
		}
	}
	spouses.TrimRight();

	for( UINT j = 0; j < v_kids1.size(); ++ j )
	{
		if( v_kids1.at(j).rowid == rowid )
		{
			kid.Format( L"%s(%s),%s,%s ", v_kids1.at(j).name, v_kids1.at(j).mother_index, v_kids1.at(j).birth_date, v_kids1.at(j).death_date );
			kids += kid;
		}
	}
	kids.TrimRight();

/*
	if( !name.Compare( L"Újfalussy Mihály" ))
	{
		listSameVector( name );
		int f = 8;
	}
*/

	father.Replace( '|', '\'' );
	mother.Replace( '|', '\'' );
	name.Replace( '|', '\'' );

//	if( _html )
	{
		str.Format( L"%-6s %6d %6s %6s %6s %4d %1s %1s %-25s %-10s %-10s %-25s %-10s %-10s %-25s %-10s %-10s %-47s | %s",\
		same,\
		cnt,\
		rowid,\
		lineNumber,\
		tableNumber,\
		source,\
		united,\
		generation,\
		name.Left(25),\
		birth_date.Left(10),\
		death_date.Left(10),\
		father,\
		birth_father,\
		death_father,\
		mother,\
		birth_mother,\
		death_mother,\
		spouses,\
		kids\
		);

		if( group )
		{
			colorIndex = group % 10;
			CString grp;
			grp.Format( L"%d", colorIndex );

			colorIndex = group % 10;

			color = m_colors.GetAt( colorIndex );
			if( volt )
			{
				++m_allP;
				fwprintf( fh1, L"<span style=\"background:%s\">%s</span><br>", color, str );	// volt a csoportban összevonás
				++m_unitedP;
				if( vSame.at(i).identical.IsEmpty() )
					++m_maradtP;		// az összevont csoportból 1 ember marad!
				else
					++m_deletedP;
			}
			else
			{
				fwprintf( fh2, L"<span style=\"background:%s\">%s</span><br>", color, str );	// nem volt a csoportban összevonás
			}
		}
		else
		{
			if( volt )
			{
				++m_allP;
				fwprintf( fh1, L"%s<br>", str );
				++m_maradtP;			// nem vonható össze
			}
			else
			{
				fwprintf( fh2, L"%s<br>", str );
			}
		}
	}

	if( !_html )
	{
		cntS.Format( L"%d", cnt );
		voltS.Format( L"%d", volt );
		sourceS.Format( L"%d", source );
		groupS.Format( L"%d", group );
		iS.Format( L"%d", i );

		if( !i )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
		}


		nItem = m_ListCtrl.InsertItem( nItem, sex_id );
		m_ListCtrl.SetItemText( nItem, S_VOLT, voltS );
		m_ListCtrl.SetItemText( nItem, S_CNT_PEOPLE, cntS );
		m_ListCtrl.SetItemText( nItem, S_CNT_SAMENAME, iS );
		if( groupS != L"0" ) 
			m_ListCtrl.SetItemText( nItem, S_GROUP, groupS );
		m_ListCtrl.SetItemText( nItem, S_ROWID, rowid );
		m_ListCtrl.SetItemText( nItem, S_LINENUMBER, lineNumber );
		m_ListCtrl.SetItemText( nItem, S_TABLENUMBER, tableNumber );
		m_ListCtrl.SetItemText( nItem, S_UNITED, united );
		
		m_ListCtrl.SetItemText( nItem, S_SOURCE, sourceS );
		m_ListCtrl.SetItemText( nItem, S_GENERATION, generation );
		m_ListCtrl.SetItemText( nItem, S_NAME, name );
		m_ListCtrl.SetItemText( nItem, S_BIRTH, birth_date );
		m_ListCtrl.SetItemText( nItem, S_DEATH, death_date );
		m_ListCtrl.SetItemText( nItem, S_FATHER, father );
		m_ListCtrl.SetItemText( nItem, S_FATHER_BIRTH, birth_father );
		m_ListCtrl.SetItemText( nItem, S_FATHER_DEATH, death_father );
		m_ListCtrl.SetItemText( nItem, S_MOTHER, mother );
		m_ListCtrl.SetItemText( nItem, S_MOTHER_BIRTH, birth_mother );
		m_ListCtrl.SetItemText( nItem, S_MOTHER_DEATH, death_mother );
		m_ListCtrl.SetItemText( nItem, S_SPOUSES, spouses );
		m_ListCtrl.SetItemText( nItem, S_KIDS, kids );
		m_ListCtrl.SetItemText( nItem, S_LINENUMBERF, vSame.at(i).lineNumberF );
		++cnt;

		m_ListCtrl.SetItemData( nItem, 0 );
		if( group )
		{
			colorIndex = group % 10;
			grp.Format( L"%d", colorIndex );
			m_ListCtrl.SetItemText( nItem, S_GROUP, grp );
			m_ListCtrl.SetItemData( nItem, colorIndex );
		}
		++nItem;
	}


	str.Format( L"%s\t%d\t%d\t%d\t%s\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",\
	sex_id,\
	volt,\
	i,\
	group,\
	rowid,\
	lineNumber,\
	tableNumber,\
	source,\
	united,\
	generation,\
	name,\
	birth_date,\
	death_date,\
	father,\
	birth_father,\
	death_father,\
	mother,\
	birth_mother,\
	death_mother,\
	spouses,\
	kids,\
	vSame.at(i).lineNumberF\
	);
	fwprintf( theApp.fl, L"%s\n", str );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// törlendõ és megtartandó ember gyûjtése a vReplace vektorban 
void CSameP::to_v_Replace( int replace, int by )
{
	REPLACE R;

	R.rowid		= vSame.at( replace ).rowid;	// törlndõ
	R.sex_id	= vSame.at(replace).sex_id;
	R.source	= vSame.at(replace).source;

	R.rowidBy	= vSame.at( by ).rowid;			// megtartandó

	v_Replace.push_back( R );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_Replace vektorban összegyûjtött emberek törlése 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a v_Replace vektorban összegyûjtött emberek törlése 
void CSameP::unite( int iter )
{
	CString	sex_id;
	CString rowid;
	int		source;
	CString rowidBy;

	str.Format( L"Azonos emberek összevonása... " );
	CProgressWnd wndProgress(NULL, str ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, v_Replace.size() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_Replace.size(); ++i )
	{
		source	= v_Replace.at(i).source ;
		sex_id	= v_Replace.at(i).sex_id;
		rowid	= v_Replace.at(i).rowid;
		rowidBy	= v_Replace.at(i).rowidBy;

		theApp.replaceBy( rowid, rowidBy, sex_id, source );

//		++m_deletedP;
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	wndProgress.DestroyWindow();

	sameMarriages( iter );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::sameMarriages( int iter )
{
	CString spouse1_id;
	CString spouse2_id;
	CString spouses1;
	CString spouses2;
	CString place1;
	CString date1;
	CString place2;
	CString date2;
	CString rowid1;
	CString rowid2;

	CString rowid;

	std::vector<CString> v_rowid;

	str.Format( L"Azonos házasságok keresése..." );
	CProgressWnd wndProgress(NULL, str ); 
	wndProgress.GoModal();


	m_command = L"SELECT rowid, spouse1_id, spouse2_id, place, date FROM marriages ORDER BY spouse1_id, spouse2_id, date";
	if( !query( m_command ) ) return;
	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	// azonos rowid-párral rendelkezõ házasságok összegyõjtése
	v_rowid.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		rowid1		= m_recordset->GetFieldString( 0 );
		spouse1_id	= m_recordset->GetFieldString( 1 );
		spouse2_id	= m_recordset->GetFieldString( 2 );
		place1		= m_recordset->GetFieldString( 3 );
		date1		= m_recordset->GetFieldString( 4 );
		spouses1.Format( L"%s%s", spouse1_id, spouse2_id );

		if( spouse1_id == L"28675" )
			int z = 0;

		m_recordset->MoveNext();

		rowid2		= m_recordset->GetFieldString( 0 );
		spouse1_id	= m_recordset->GetFieldString( 1 );
		spouse2_id	= m_recordset->GetFieldString( 2 );
		place2		= m_recordset->GetFieldString(3 );
		date2		= m_recordset->GetFieldString(4 );
		spouses2.Format( L"%s%s", spouse1_id, spouse2_id );

		if( spouse1_id == L"28675" )
			int x = 0;

		if( !place1.IsEmpty() || !date1.IsEmpty() )
			rowid = rowid2;
		else
			rowid = rowid1;

		rowid = m_recordset->GetFieldString( 0 );
		if( !spouses1.Compare( spouses2 ) )
		{
			v_rowid.push_back( rowid );
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();


	str.Format( L"Azonos házasságok törlése (%d. iteráció)...", iter );
	CProgressWnd wndP(NULL, str ); 
	wndP.GoModal();
	wndP.SetRange(0, v_rowid.size() );
	wndP.SetPos(0);
	wndP.SetStep(1);

//	str.Format( L"v_rowid size: %d", v_rowid.size() );
//	AfxMessageBox( str );

	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_rowid.size(); ++i )
	{
		m_command.Format( L"DELETE FROM marriages WHERE rowid='%s'", v_rowid.at(i) );
		if( !theApp.execute( m_command ) ) return;
		++m_deletedM;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( !theApp.execute( L"COMMIT" ) ) return;
	wndP.DestroyWindow();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameP::query( CString command )
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
BOOL CSameP::query1( CString command )
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
BOOL CSameP::query2( CString command )
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
BOOL CSameP::query3( CString command )
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
BOOL CSameP::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::listSameVector( CString name )
{
	fwprintf( fh2, L"%s<br>", name ); 
	

	CString name1;
/*
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		if( name == L"AndreánszkyKlára" )
			m_recordset->MoveTo( vSame.at(i).index - 1  );
		else
			m_recordset->MoveTo( vSame.at(i).index  );
		name1.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		name1 = vSame.at(i).name;
		fwprintf( fh2, L"%4d. %4d %10d %d %s %s %s %s<br>", i+1, vSame.at(i).index, vSame.at(i).rowid, vSame.at(i).source, name1, vSame.at(i).father, vSame.at(i).mother,vSame.at(i).spouses2 ); 
	}
*/
	for( UINT i = 0; i < vSame.size(); ++i )
	{
		name1 = vSame.at(i).name;
		fwprintf( fh2, L"%4d. %10s %d %s %s %s<br>", i, vSame.at(i).rowid, vSame.at(i).source, name1, vSame.at(i).father, vSame.at(i).mother ); 
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::listColumnHeader( int i )
{
	str.Format( L"%13s %6s %6s %6s %4s %1s %1s %s%s%-10s %s%s%-10s %s%s%-10s %s%s%-10s | %s",\
	L"cnt",\
	L"rowid",\
	L"line#",\
	L"tabl#",\
	L"S",\
	L"U",\
	L"G",\
	L"név-----------------------",\
	L"születés---",\
	L"halál",\
	L"apa-----------------------",\
	L"születés---",\
	L"halál",\
	L"anya----------------------",\
	L"születés---",\
	L"halál",\
	L"házastársak---------------",\
	L"születés---",\
	L"halál",\
	L"gyerekek"\
	);

	if( i == 1 )
		fwprintf( fh1, L"<font color='red'>%s</font><br>", str );
	else
		fwprintf( fh2, L"<font color='red'>%s</font><br>", str );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySourceS(const SAME &v1, const SAME &v2) 
{ 
    return( v1.source < v2.source ); 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByGroupS(const SAME &v1, const SAME &v2) 
{ 
	if( v1.group == v2.group )
		return( v1.source < v2.source );
	return( (v1.group < v2.group ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::htmlHeader()
{
	CString algorithm1;
//	CString algorithm2;
	CString algorithm3;

	CString title1;
	CString title2;
	CString title3;
	CString title4;
	CString title1d;

	CString list1 = L"Az itt felsorolt azonos nevû emberek közül kijelöli, hogy melyeket lehetne összevonni.<br>";
	CString list2 = L"Az itt felsorolt azonos nevû embereket különbözõ embereknek találta a program.<br>";

	CString explanation = L"\
Az 'S' oszlopban az ember szerepkódja van, az 'U' oszlopban lévõ szám pedig megmutatja, hogy hány embert vont össze az \
gyesítési eljárás.\
<br><br>\
Egy ember az alábbi szerepekben fordulhat elõ:<br><br>\
1 = leszármazott<br>\
2 = leszármazott házastársa<br>\
3 = leszármazott házastárásának apja<br>\
4 = leszármazott házastársának anyja<br>\
5 = leszármazott házastársának további házastársa<br>\
<br>\
Az alábbi adatokat használjuk azonosításra:<br><br>\
1. születési dátum<br>\
2. halálozási dátum<br>\
3. apa neve és születési/halálozási dátuma<br>\
4. anya neve és születési/halálozási dátuma<br>\
5. házastárs neve és születési/halálozási dátuma<br>\
<br>\
Ha több házastársa van egy embernek, akkor csak azt követeljük meg, hogy legyen köztük azonos nevû és születési/halálozási dátumú.<br>\
Ha a házastársnak kettõs keresztneve van, akkor csak az elsõt használja a nevek összehasonlításakor!<br>\
Csak akkor vizsgájuk egy azonosítási kritérium fennállását, ha az mindkét azonos nevû embernél meg van adva.<br>\
<br>";
CString ident;
ident.Format( L"Két azonos nevû embert akkor tekintünk azonosnak, ha a fenti azonosítási kritériumok közül minimum %d teljesül és nincs ellentmondó adatuk.", _azonos );


	if( !_fullname.IsEmpty() )
	{
		if( _unite )
		{
			title1.Format( L"'%s' NEVÛ ÉS ÖSSZEVONT EMBEREK", _fullname );
			title1d.Format( L"'%s'NEVÛ DE KÜLÖNBÖZÕ EMBEREK", _fullname );
		}
		else
		{
			title1.Format( L"'%s' NEVÛ ÉS ÖSSZEVONHATÓ EMBEREK", _fullname );
			title1d.Format( L"'%s' NEVÛ DE KÜLÖNBÖZÕ EMBEREK", _fullname );
		}
	}
	else
	{
		if( _unite )
		{
			title1	= L"AZONOS NEVÛ ÉS AZONOSNAK ÉRTÉKELT ÖSSZEVONT EMBEREK";
			title1d	= L"AZONOS NEVÛ DE KÜLÖNBÖZÕNEK ÉRTÉKELT EMBEREK LISTÁJA";
		}
		else
		{
			title1	= L"AZONOS NEVÛ ÉS ÖSSZEVONHATÓ EMBEREK LISTÁJA";
			title1d	= L"AZONOS NEVÛ DE KÜLÖNBÖZÕNEK ÉRTÉKELT EMBEREK LISTÁJA"; 
		}
	}
	title2 = L"AZ AZONOS NEVÛ DE NEM AZONOSNAK MINÕSÍTETT EMBEREKET FEHÉR HÁTTÉREL ÍRJ AKI A PROGRAM.";
	title3 = L"AZ AZONOS NEVÛ ÉS AZONOSNAK MINÕSÍTETT EMBEREKET AZONOS HÁTTÉR SZÍNNEL ÍRJA KI.";
	if( _unite )
	{
		title4 = L"EZEK KÖZÜL AZ ELSÕ A MEGTARTOTT EMBER, A TÖBBI A TÖRÖLT EMBER";
	}
	else
	{
		title4 = L"EZEK KÖZÜL AZ ELSÕ A MEGTARTANDÓ EMBER, A TÖBBI A TÖRLENDÕ EMBER";
	}

	fwprintf( fh2, L"<HEAD>\n" );
	fwprintf( fh2, L"<style>\n" );
	fwprintf( fh2, L"</style>\n" );
	fwprintf( fh2, L"</HEAD>\n" );
	fwprintf( fh2, L"<BODY>\n" );
	fwprintf( fh2, L"<center>%s</center><br><br><br>", title1d );
	fwprintf( fh2, L"<pre>" );


	fwprintf( fh1, L"<HEAD>\n" );
	fwprintf( fh1, L"<style>\n" );
	fwprintf( fh1, L"</style>\n" );
	fwprintf( fh1, L"</HEAD>\n" );
	fwprintf( fh1, L"<BODY>\n" );
	
	fwprintf( fh1, L"<center>%s</center>", title1 );
	fwprintf( fh1, L"<center>%s</center>", title2 );
	fwprintf( fh1, L"<center>%s</center>", title3 );
	fwprintf( fh1, L"<center>%s</center><br>", title4 );
	fwprintf( fh1, L"<pre>" );

		
	fwprintf( fh1, L"%-45s %s<br>", L"Program: dragon verzió:", VERSION );
	fwprintf( fh1, L"%-45s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
	fwprintf( fh1, L"%-45s %s<br>", L"Lista készült:", theApp.getPresentDateTime() );
	fwprintf( fh1, L"%-45s %d<br>",  L"Teljesítendõ kritériumok elõírt száma:", _azonos );
	fwprintf( fh1, L"%-45s %s<br><br>", L"A bemeneten elvégzett összevonások:", _contractions );
	fwprintf( fh1, list1 );
	fwprintf( fh1, explanation );
	fwprintf( fh1, L"%s<br><br>", ident );

	listColumnHeader(1);


	fwprintf( fh2, L"%-45s %s<br>", L"Program: Dragon verzió:", VERSION );
	fwprintf( fh2, L"%-45s %s<br>", L"Adatbázis:", theApp.m_databaseSpec );
	fwprintf( fh2, L"%-45s %s<br>", L"Lista készült:", theApp.getPresentDateTime() );
	fwprintf( fh2, L"%-45s %d<br>",  L"Teljesítendõ kritériumok elõírt száma:", _azonos );
	fwprintf( fh2, L"%-45s %s<br><br>", L"A bemeneten elvégzett összevonások:", _contractions );
	fwprintf( fh2, list2 );
	fwprintf( fh2, explanation );
	fwprintf( fh2, L"%s<br><br>", ident );
	listColumnHeader(2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::htmlFooter()
{
	CString inputP;
	CString outputP;

	CString allP;
	CString deletedP;
	CString unitedP;
	CString maradtP;

	CString inputM;
	CString outputM;
	CString deletedM;
	CString maradtM;
	

	fwprintf( fh1, L"%-14s %12s %12s<br>", L" ", L"emberek", L"házasságok" );

	inputP		= thousend( (unsigned _int64)m_inputP );
	allP		= thousend( (unsigned _int64)m_allP );
	deletedP	= thousend( (unsigned _int64)m_deletedP );
	unitedP		= thousend( (unsigned _int64)m_unitedP );
	maradtP		= thousend( (unsigned _int64)m_maradtP );
	outputP		= thousend( (unsigned _int64)(m_inputP - m_deletedP) );


	inputM		= thousend( (unsigned _int64)m_inputM );
	deletedM	= thousend( (unsigned _int64)m_deletedM );
	maradtM		= thousend( (unsigned _int64)m_maradtM );
	outputM		= thousend( (unsigned _int64)(m_inputM - m_deletedM) );


	if( _unite )
	{
		fwprintf( fh1, L"%-15s %12s %12s<br><br>", L"input", inputP, inputM );
		fwprintf( fh1, L"%-15s %12s<br>", L"azonos nevûek:", allP );
		fwprintf( fh1, L"%-15s %12s<br>", L"összevonva:", unitedP );
		fwprintf( fh1, L"%-15s %12s %12s<br>", L"törölve", deletedP, deletedM );
		fwprintf( fh1, L"%-15s %12s %12s<br><br>", L"maradt:", maradtP, maradtM );
		fwprintf( fh1, L"%-15s %12s %12s<br><br>", L"output", outputP, outputM );
	}
	else
	{
		fwprintf( fh1, L"%-15s %12s<br>", L"azonos nevûek:", allP );
		fwprintf( fh1, L"%-15s %12s<br>", L"összevonható:", unitedP );
		fwprintf( fh1, L"%-15s %12s %12s<br>", L"törölhetõ:", deletedP, deletedM );
		fwprintf( fh1, L"%-15s %12s %12s<br><br>", L"maradt:", maradtP, maradtM );
	}

	fwprintf( fh1, L"__________________________________________<br>" );

	fwprintf( fh1, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
	fwprintf( fh1, L"</BODY>\n" );

	if( !m_unitedP )
		fwprintf( fh1, L"Nem találtam összevonnható azonos nevû embereket!" );

	fwprintf( fh2, L"Eltelt idõ: %s<br><br>", theApp.get_time_elapsed() );
	fwprintf( fh2, L"</BODY>\n" );

	fclose( fh1 );
	fclose( fh2 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::getSpouses( CString rowid, CString sex_id )
{
	CString spouse_id;
	CString spouse;
	CString spouse2;
	CString order;
	CString names( L"" );
	CString names2(L"");
	SPOUSES spousesS;
	CString firstName;
	CString lastName;
	CString source;
	int		pos;

	CString birthDate;
	CString deathDate;

	if( sex_id == L"1" )
		m_command.Format( L"SELECT spouse2_id, orderHusband FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT spouse1_id, orderWife FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !theApp.query1( m_command ) ) return;



	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		spouse_id	= theApp.m_recordset1->GetFieldString( 0 );
		order		= theApp.m_recordset1->GetFieldString( 1 );

		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", s_fields, spouse_id );
		if( !theApp.query2( m_command )) return;
		
		firstName	= theApp.m_recordset2->GetFieldString( S_FIRST_NAME );
		lastName	= theApp.m_recordset2->GetFieldString( S_LAST_NAME );
		source		= theApp.m_recordset2->GetFieldString( S_SOURCE );
		birthDate	= theApp.m_recordset2->GetFieldString( S_BIRTH_DATE );
		deathDate	= theApp.m_recordset2->GetFieldString( S_DEATH_DATE );
		firstName	= getWord( firstName, 1, &pos );


		spouse2.Format( L"%s %s %s", source, lastName, firstName );
		spouse.Format( L"%s %s", lastName, firstName );

		spouse.Replace( '|', '\'' );
		
		spousesS.rowid		= rowid;
		spousesS.oder		= order;
		spousesS.id			= spouse_id;
		spousesS.name		= spouse;
		spousesS.name2		= spouse2;
		spousesS.birth_date	= birthDate;
		spousesS.death_date	= deathDate;

		v_spouses1.push_back( spousesS );
		v_spouses2.push_back( spousesS );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::getKids( CString rowid, CString sex_id )
{
	CString child;
	KIDS	kids;


	if( sex_id == L"1" )
		m_command.Format( L"SELECT %s FROM people WHERE father_id = '%s'", k_fields, rowid );
	else
		m_command.Format( L"SELECT %s FROM people WHERE mother_id = '%s'", k_fields, rowid );
	if( !query2( m_command ) ) return;
	

	for( UINT i = 0; i < m_recordset2->RecordsCount(); ++i, m_recordset2->MoveNext() )
	{
		kids.rowid		= rowid;
		child			=  m_recordset2->GetFieldString( K_FIRST_NAME );
		child.Replace( '|', '\'' );
		kids.name			= child;
		kids.id				= m_recordset2->GetFieldString( K_ROWID );
		kids.mother_index	= m_recordset2->GetFieldString( K_MOTHER_INDEX );
		kids.birth_date		= m_recordset2->GetFieldString( K_BIRTH_DATE );
		kids.death_date		= m_recordset2->GetFieldString( K_DEATH_DATE );
		v_kids1.push_back( kids );
		v_kids2.push_back( kids );
	}
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// E G Y E T L E N   E M B E R R E L   A Z O N O S   E M B E R E K E T   K E R E S  ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::findSamePeople( CString firstName, CString lastName, BOOL replace )
{
	CString fileName( L"azonosEmber" );
	CString fileSpec;


	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	fwprintf( fh1, L"<center>AZONOS EMBER</center><br>" );


	fwprintf( fh1, L"<pre>" );
	listColumnHeader( 1 );


	CString name;
	CString nameNext;

	CProgressWnd wndProgress(NULL,L"Azonos emberek keresése ..." ); 
	wndProgress.GoModal();
#ifndef _DEBUG
	wndProgress.SetText( L"Emberek beolvasása folyik..." );
#endif
	m_command = L"SELECT count() FROM marriages";
	if( !query( m_command ) ) return;
//	_cntMarriagesI = _wtoi( m_recordset->GetFieldString( 0 ) );

	m_command = L"SELECT rowid,* FROM people ORDER BY last_name, first_name, source";
	if( !query( m_command ) ) return;
//	_cntPeopleI = m_recordset->RecordsCount();

	wndProgress.SetRange(0, m_recordset->RecordsCount());
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

#ifndef _DEBUG
	wndProgress.SetText( L"Emberek azonosítása..." );
#endif

	// azonos nevû emberek indexeinek kigyûjtése a vSame vektorba
	name.Format( L"%s%s", lastName, firstName );
	collectSameName( name );

	v_Replace.clear();
	if( vSame.size() > 1 )	processSame();		// 
	if( replace )	unite( 1 );
	v_Replace.clear();

	fwprintf( fh1, L"</table>\n" );
	fwprintf( fh1, L"</BODY>\n" );

	fclose( fh1 );

	if( vSame.size() )
		theApp.showHtmlFile( fileSpec );
	else
		AfxMessageBox( L"Nem találtam azonos nevû embereket! " );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::collectSameName( CString name )
{
	SAME same;
	sameClear( &same );
	vSame.clear();
	CString name2;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		name2.Format( L"%s%s", m_recordset->GetFieldString( _LAST_NAME ), m_recordset->GetFieldString( _FIRST_NAME ) );
		if( !name2.Compare( name ) )
		{
//			same.index = i;
			same.source = _wtoi( m_recordset->GetFieldString( _SOURCE ) );
			vSame.push_back( same );
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CSameP::getChildren( CString rowid, CString sex_id, std::vector<KIDS>* vektor )
{
	CString child_id;
	CString child;
	CString names( L"" );
	KIDS	nameid;


	vektor->clear();
	if( sex_id == L"1" )
		m_command.Format( L"SELECT rowid, last_name, first_name FROM people WHERE father_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT rowid, last_name, first_name FROM people WHERE mother_id = '%s'", rowid );
	if( !theApp.query1( m_command ) ) return L"" ;
	
	for( UINT i = 0; i < theApp.m_recordset1->RecordsCount(); ++i, theApp.m_recordset1->MoveNext() )
	{
		child.Format( L"%s %s", theApp.m_recordset1->GetFieldString( 1 ), theApp.m_recordset1->GetFieldString( 2 ) );
		child.Replace( '|', '\'' );
		nameid.id		= theApp.m_recordset1->GetFieldString( 0 );
		nameid.name	= child;
		vektor->push_back( nameid );
	}
	std::sort( vektor->begin(), vektor->end(), sortByNameKids );

	names.Empty();
	for( UINT i = 0; i < vektor->size(); ++i )
	{
		names += vektor->at(i).name;
		names += L", ";
	}
	names = names.Left( names.GetLength() - 2 );   
	return names;
} 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::title( int contracted )
{
	CString caption;
	CString caption2 = L"Az azonos színnel jelölt emberek összevonhatóak.";
	CString mi(L"minden név" );

	switch( contracted )
	{
	case 0:
		mi = L"csak amik nem összevonhatóak";
		break;
	case 1:
		mi = L"csak amik összevonhatóak";
	}

	if( _fullname.IsEmpty() )
		caption.Format( L"Azonos nevû összevonható emberek azonosítása. %s", caption2  );
	else
		caption.Format( L"%s nevû összevonható emberek azonosítása. %s", _fullname, caption2 );

	str.Format( L"%s - %s", theApp.m_databaseName, caption );
	SetWindowTextW( str );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int iData;

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
		iData	= m_ListCtrl.GetItemData( nItem );
		if( iData  )
		{
			pLVCD->clrTextBk = m_rgb[iData];
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CSameP:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML_P ))
    {
		pPopup = Menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	S_LINENUMBER ) );
	theApp.listHtmlLine( lineNumber );
}
void CSameP::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 	S_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnHtmlShows()
{
	POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int			nItem;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");

	while( pos )
	{
		nItem = m_ListCtrl.GetNextSelectedItem( pos );
		vLines.push_back( m_ListCtrl.GetItemText( nItem, S_LINENUMBER ) );
		if( name.Compare( m_ListCtrl.GetItemText( nItem, S_NAME ) ) )
		{
			name = m_ListCtrl.GetItemText( nItem, S_NAME );
			++cnt;
		}
	

	}

	CHtmlLines dlg;

	if( cnt == 1 )
		dlg.child = name;
	else
		dlg.child = L"";

	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnHtmlPeoplefather()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber	= m_ListCtrl.GetItemText( nItem, 	S_LINENUMBER );
	CString lineNumberF	= m_ListCtrl.GetItemText( nItem, 	S_LINENUMBERF );
	if( lineNumberF.IsEmpty() )
	{
		AfxMessageBox( L"A kijelölt embernek nem ismerjük az apját!" );
		return;
	}


	std::vector<CString> vLines;

	vLines.push_back( lineNumberF );
	vLines.push_back( lineNumber );

	CHtmlLines dlg;
	dlg._what = 2;
	dlg.parents.Format( L"%s - %s",  m_ListCtrl.GetItemText( nItem,S_FATHER ), m_ListCtrl.GetItemText( nItem,S_MOTHER ) );
	dlg.child	= m_ListCtrl.GetItemText( nItem,S_NAME );
	dlg.vLines	= &vLines;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnAll()
{
	fillTable( 2 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnContracted()
{
	fillTable( 1 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::OnNotcontracted()
{
	fillTable( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameP::fillTable( int contracted )
{
	int	nItem = 0;
	int fileLength;
	int position;
	CStdioFile file( fileSpec, CFile::modeRead); 
	fileLength = (int)file.GetLength();


	CProgressWnd wndP(NULL, L"Emberek beolvasása a táblázatba" );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	CString			cLine;
	CStringArray	A;
	int				n;
	int		group;
	CString	sex_id;
	int		volt;
	int		first;
	CString	cntP;
	CString	cntG;
	CString	rowid;
	CString	lineNumber;
	CString tableNumber;
	CString source;
	CString generation;
	CString united;
	CString name;
	CString birth;
	CString death;
	CString father;
	CString mother;
	CString spouses;

	int		cnt = 1;
	int		sameName = 1;

	m_ListCtrl.DeleteAllItems();

	while(file.ReadString(cLine)) 
	{
		if( cLine.IsEmpty() ) continue;

		A.RemoveAll();
		n = wordList( &A, cLine, '\t', TRUE );

		if( n != T_COLUMNS )
		{
			str.Format( L"Oszlopok száma %d !=%d", n, T_COLUMNS );
			AfxMessageBox( str );
			return;
		}

		volt = _wtoi( A[T_VOLT] );
		if( contracted == 0 && volt != 0 ) continue;		// csak amiben nincs összevonás
		if( contracted == 1 && volt == 0 ) continue;		// csak amiben van összevonás

		first = _wtoi( A[T_FIRST] );
		if( !first  )
		{
			nItem = m_ListCtrl.InsertItem( nItem, L"" );
			++nItem;
			++sameName;
		}


		group		= _wtoi( A[T_GROUP] );

		nItem = m_ListCtrl.InsertItem( nItem, A[T_SEX_ID] );
		m_ListCtrl.SetItemText( nItem, S_VOLT, A[T_VOLT] );

		str.Format( L"%d", cnt );
		m_ListCtrl.SetItemText( nItem, S_CNT_PEOPLE, str );
		str.Format( L"%d", sameName );
		m_ListCtrl.SetItemText( nItem, S_CNT_SAMENAME, str );
		if( A[T_GROUP] != L"0" ) 
			m_ListCtrl.SetItemText( nItem, S_GROUP, A[T_GROUP] );
		m_ListCtrl.SetItemText( nItem, S_ROWID, A[T_ROWID] );
		m_ListCtrl.SetItemText( nItem, S_LINENUMBER, A[T_LINENUMBER] );
		m_ListCtrl.SetItemText( nItem, S_TABLENUMBER, A[T_TABLENUMBER]);
		m_ListCtrl.SetItemText( nItem, S_UNITED, A[T_UNITED] );
		m_ListCtrl.SetItemText( nItem, S_SOURCE, A[T_SOURCE] );
		m_ListCtrl.SetItemText( nItem, S_GENERATION, A[T_GENERATION] );
		m_ListCtrl.SetItemText( nItem, S_NAME, A[T_NAME] );
		m_ListCtrl.SetItemText( nItem, S_BIRTH, A[T_BIRTH_DATE] );
		m_ListCtrl.SetItemText( nItem, S_DEATH, A[_DEATH_DATE] );
		m_ListCtrl.SetItemText( nItem, S_FATHER, A[T_FATHER] );
		m_ListCtrl.SetItemText( nItem, S_FATHER_BIRTH, A[T_FATHER_BIRTH] );
		m_ListCtrl.SetItemText( nItem, S_FATHER_DEATH, A[T_FATHER_DEATH] );
		m_ListCtrl.SetItemText( nItem, S_MOTHER, A[T_MOTHER] );
		m_ListCtrl.SetItemText( nItem, S_MOTHER_BIRTH, A[T_MOTHER_BIRTH] );
		m_ListCtrl.SetItemText( nItem, S_MOTHER_DEATH, A[T_MOTHER_DEATH] );
		m_ListCtrl.SetItemText( nItem, S_SPOUSES,A[T_SPOUSES] );
		m_ListCtrl.SetItemText( nItem, S_KIDS,A[T_KIDS] );
		m_ListCtrl.SetItemText( nItem, S_LINENUMBERF,A[T_LINENUMBERF] );
		++cnt;

		m_ListCtrl.SetItemData( nItem, 0 );
		if( group )
		{
			int colorIndex = group % 10;
			CString grp;
			grp.Format( L"%d", colorIndex );
			m_ListCtrl.SetItemText( nItem, S_GROUP, grp );
			m_ListCtrl.SetItemData( nItem, colorIndex );
		}
		++nItem;
	
		position = (int)file.GetPosition();
		wndP.SetPos( position );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	for(int i = 0;i < m_ListCtrl.GetHeaderCtrl()->GetItemCount();++i)
    m_ListCtrl.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);

	file.Close();
	title( contracted );
}