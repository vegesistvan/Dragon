// CheckMarriageOrder.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "CheckMarriageOrder.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "checkParam0.h"
#include "GetLastFirst.h"
#include "utilities.h"
#include <algorithm>
#include "html_Lines.h"
// CCheckMarriageOrder dialog

enum
{
	P_ROWID = 0,
	P_FIRST_NAME,
	P_LAST_NAME,
	P_SEX_ID,
	P_LINENUMBER,
	P_TABLENUMBER,
	P_GENERATION,
	P_SOURCE,
	P_BIRTH_DATE,
	P_DEATH_DATE,
	P_FATHER_ID,
	P_MOTHER_ID,
	P_UNITED,
};

// ListCtrl oszlopok
enum
{
	L_LINENUMBER = 0,
	L_ORDER, 
	L_DATE,
	L_SOURCE,
	L_ROWID,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_ROWIDF,
	L_FATHER,
	L_BIRTHF,
	L_DEATHF,
	L_ROWIDM,
	L_MOTHER,
	L_BIRTHM,
	L_DEATHM,
};
bool sortByNameX(const SPOUSE1 &v1, const SPOUSE1 &v2);
IMPLEMENT_DYNAMIC(CCheckMarriageOrder, CDialogEx)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMarriageOrder::CCheckMarriageOrder(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckMarriageOrder::IDD, pParent)
{
	m_explanation = L"Azokat az embereket listázzuk, akiknek több házastársa van. A lista elsõsorban arra szolgál, \
hogy ellenõrizzük a házasságok sorszámát.\
<br><br>\
Az alábbi oszlopok vannak a listában:\
<pre>\
<br>\
line#	a leszármazott/házastárs ssorszáma aGA.html fájlban.<br>\
O	a házasság sorszáma<br>\
esküvõ	az esküvõ dátuma<br>\
S	a házatársak hierachia sorszáma. ( 1-5 )<br>\
</pre>\
a további oszlopok magától érthetõek.<br>\
";

	p_fields = L"\
rowid,\
first_name,\
last_name,\
sex_id,\
lineNumber,\
tableNumber,\
generation,\
source,\
birth_date,\
death_date,\
father_id,\
mother_id,\
united\
";

	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckMarriageOrder::~CCheckMarriageOrder()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckMarriageOrder, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckMarriageOrder::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCheckMarriageOrder::OnDblclkList)
	ON_EN_KILLFOCUS(IDC_SEARCH, &CCheckMarriageOrder::OnKillfocusSearch)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMarriageOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_INFO,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();
/*	
		_info = L"\
Azokat az embereket listázzuk, akiknek több azonos nevû házastársa van. Egy embernek természetesen lehet két \
vagy akár több azonos nevû házastársa, azonban legtöbbször hibás adatokra hívja fel a figyelmet.\
\r\n\
Két vagy több azonos nevû házastárs személyes adatai közötti különbségeket sárga színnel emeli ki a program.\
\r\n\r\n\
Ha az azonos emberek összevonása elõtt készítjük el a listát, akkor csak olyankor fordulhat elõ több azonos nevû házaspár, \
ha azok egyetlen leszármazotti sorban vannak megadva házastársként.\
\r\n\
Az azonos emberek összevonása megsokszorozhatja az ilyen eseteket, ugyanis ha emberünk azonossá lett egy vagy több más azonos \
nevû emberrel, akkor ezek összevonásra kerültek, de azonos nevû házastársaik nem feltétlenül teljesítik az összevonási \
kritériumot, ezért õk bár azonos nevûek, mégis különbözõ emberként maradnak az adatbázisban.\
\r\n\r\n\
Az a második, harmadik, n. házastérs, amelynek sorában nincs az elsõvel különbséget jelzõ sárga adat, összevonásra került \
az elsõ házastárssal.\
";



	CCheckParam0 dlg;
	dlg._caption = L"Emberek sorszám nélküli házasságai";
	dlg._info = _info;
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
		caption = L"Emberek többszörös házasságai";
	}
	else
		caption.Format( L"%s nevû emberek többszörös házasságai", _fullname );
	SetWindowTextW( caption ),

*/
	str = L"piros az ember, fekete a házastársa.        O - házasság sorszáma  S - az ember hierarchia száma ";
	GetDlgItem( IDC_INFO )->SetWindowTextW( str );

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ORDER,		L"O",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_DATE,		L"esküvõ",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_SOURCE,		L"S",			LVCFMT_RIGHT,	 20,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_ROWIDF,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_FATHER,		L"apa",			LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHF,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHF,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ROWIDM,		L"rowid",		LVCFMT_RIGHT,	 80,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MOTHER,		L"anya",		LVCFMT_LEFT,	200,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_BIRTHM,		L"születés",	LVCFMT_RIGHT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATHM,		L"halál",		LVCFMT_RIGHT,	 80,-1,COL_TEXT);

	m_columnsCount	= m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	nItem = 0;
	m_cnt = 0;
	marriages();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCheckMarriageOrder::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::marriages()
{
	CString fileName;
	CString title(L"Emberek, akiknek több házastársuk volt" ); 

	
	wndP.Create( NULL, title );
	wndP.GoModal();
	
	fileName += L"peopleHaveUnorderedSpouses";
	fileSpec = theApp.openHtmlFile( &fh1, fileName, L"w+" );
	htmlHeader( title );

#ifndef _DEBUG
	str.Format( L"Férjek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif

	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse1_id" );
	if( !query( m_command ) ) return;

	collectHusband();
	
#ifndef _DEBUG
	str.Format( L"Feleségek lekérdezése az adatbázisból..." );
	wndP.SetText( str );
#endif
	m_command.Format( L"SELECT rowid,* FROM marriages ORDER BY spouse2_id" );
	if( !query( m_command ) )
		return;

	collectWife();

	fillSpouse1();
	fillSpouse2();

	std::sort( vSpouse1.begin(), vSpouse1.end(), sortByNameX );
//	listHtml();
	fillTable();


	wndP.DestroyWindow();
	
	if( !vSpouse1.size() )
	{
		str = L"Nem találtam olyan embert, akinek több házassága lenne!";
		AfxMessageBox( str );
		return;
	}


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::collectHusband()
{
	CString spouse1_id;
	CString spouse2_id;

	UINT i;
	bool	volt = false;
	
	SPOUSE1 husband;
	SPOUSE2 wifes;

#ifndef _DEBUG
	str.Format( L"Férfiak házasságai..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, m_recordset->RecordsCount()/2-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		spouse1_id = m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		m_recordset->MoveNext();

		if( spouse1_id == m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID ) )
		{
			if( !volt )
			{
				husband.rowid	= spouse1_id;
				vSpouse1.push_back( husband );
				volt = true;
			}
			m_recordset->MovePrevious();
			wifes.rowidS	= spouse1_id;
			wifes.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );

			wifes.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
			wifes.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
			wifes.order	= m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		
			vSpouse2.push_back( wifes );
			m_recordset->MoveNext();
		}
		else
		{
			if( volt )
			{
				m_recordset->MovePrevious();
				wifes.rowidS	= spouse1_id;
				wifes.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );

				wifes.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
				wifes.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
				wifes.order	= m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		
				vSpouse2.push_back( wifes );
				m_recordset->MoveNext();
			}
			volt = false;
		}

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::collectWife()
{
	CString spouse1_id;
	CString spouse2_id;

	UINT i;
	bool	volt = false;
	
	SPOUSE1 wife;
	SPOUSE2 husband;

#ifndef _DEBUG
	str.Format( L"Nõk házasságai..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, m_recordset->RecordsCount()/2-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		spouse2_id = m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		m_recordset->MoveNext();
		if( spouse2_id == m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID ) )
		{
			if( !volt )
			{
				wife.rowid	= spouse2_id;
				vSpouse1.push_back( wife );
				volt = true;
			}
			m_recordset->MovePrevious();
			husband.rowidS	= spouse2_id;
			husband.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );

			husband.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
			husband.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
			husband.order	= m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
		
			vSpouse2.push_back( husband );
			m_recordset->MoveNext();
		}
		else
		{
			if( volt )
			{
				m_recordset->MovePrevious();
				husband.rowidS	= spouse2_id;
				husband.rowid	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );

				husband.date	= m_recordset->GetFieldString( MARRIAGES_DATE );
				husband.place	= m_recordset->GetFieldString( MARRIAGES_PLACE );
				husband.order	= m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );
		
				vSpouse2.push_back( husband );
				m_recordset->MoveNext();
			}
			volt = false;
		}

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::fillSpouse1()
{
	UINT i;
	CString rowid;
	CString firstName;
	CString lastName;
	CString name;
	CString father_id;
	CString mother_id;

#ifndef _DEBUG
	str.Format( L"Emberek adatainak feltöltése..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( i = 0; i < vSpouse1.size(); ++i )
	{
		rowid = vSpouse1.at(i).rowid;
		m_command.Format( L"SELECT last_name,  first_name, birth_date, death_date, source, sex_id, lineNumber, father_id, mother_id FROM people WHERE rowid ='%s'", rowid );
		if( !query( m_command ) ) return;

		lastName		= m_recordset->GetFieldString( 0 );
		firstName		= m_recordset->GetFieldString( 1 );
		name.Format ( L"%s %s", lastName, firstName );
		vSpouse1.at(i).name = name;
		vSpouse1.at(i).birth	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).death	= m_recordset->GetFieldString( 3 );
		vSpouse1.at(i).source	= m_recordset->GetFieldString( 4 );
		vSpouse1.at(i).sex_id	= m_recordset->GetFieldString( 5 );
		vSpouse1.at(i).linenumber = m_recordset->GetFieldString( 6 );

		father_id				= m_recordset->GetFieldString( 7 );
		mother_id				= m_recordset->GetFieldString( 8 );


		vSpouse1.at(i).rowidF	= father_id;
		vSpouse1.at(i).rowidM	= mother_id;



		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", father_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse1.at(i).father	= name;
		vSpouse1.at(i).birthF	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).deathF	= m_recordset->GetFieldString( 3 );
		

		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", mother_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse1.at(i).mother	= name;
		vSpouse1.at(i).birthM	= m_recordset->GetFieldString( 2 );
		vSpouse1.at(i).deathM	= m_recordset->GetFieldString( 3 );

		if( vSpouse1.at(i).rowidF == L"0" )
			vSpouse1.at(i).rowidF = L" ";
		if( vSpouse1.at(i).rowidM == L"0" )
			vSpouse1.at(i).rowidM = L" ";

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::fillSpouse2()
{
	UINT i;
	CString rowid;
	CString firstName;
	CString lastName;
	CString name;
	CString father_id;
	CString mother_id;

#ifndef _DEBUG
	str.Format( L"Házastársak adatainak feltöltése..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, vSpouse2.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( i = 0; i < vSpouse2.size(); ++i )
	{
		rowid = vSpouse2.at(i).rowid;
		m_command.Format( L"SELECT last_name,  first_name, birth_date, death_date, source, sex_id, lineNumber, father_id, mother_id FROM people WHERE rowid ='%s'", rowid );
		if( !query( m_command ) ) return;

		lastName		= m_recordset->GetFieldString( 0 );
		firstName		= m_recordset->GetFieldString( 1 );
		name.Format ( L"%s %s", lastName, firstName );
		vSpouse2.at(i).name = name;
		vSpouse2.at(i).birth	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).death	= m_recordset->GetFieldString( 3 );
		vSpouse2.at(i).source	= m_recordset->GetFieldString( 4 );
		vSpouse2.at(i).sex_id	= m_recordset->GetFieldString( 5 );
		vSpouse2.at(i).linenumber = m_recordset->GetFieldString( 6 );

		father_id				= m_recordset->GetFieldString( 7 );
		mother_id				= m_recordset->GetFieldString( 8 );
		vSpouse2.at(i).rowidF	= father_id;
		vSpouse2.at(i).rowidM	= mother_id;


		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", father_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse2.at(i).father	= name;
		vSpouse2.at(i).birthF	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).deathF	= m_recordset->GetFieldString( 3 );


		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid ='%s'", mother_id );
		if( !query( m_command ) ) return;
		firstName = getFirstWord( m_recordset->GetFieldString( 1 ) );
		name.Format( L"%s %s", m_recordset->GetFieldString( 0 ), firstName );
		vSpouse2.at(i).mother	= name;
		vSpouse2.at(i).birthM	= m_recordset->GetFieldString( 2 );
		vSpouse2.at(i).deathM	= m_recordset->GetFieldString( 3 );

		if( vSpouse2.at(i).rowidF == L"0" )
			vSpouse2.at(i).rowidF = L" ";
		if( vSpouse2.at(i).rowidM == L"0" )
			vSpouse2.at(i).rowidM = L" ";

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::htmlHeader( CString title )
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
	
	str.Format( L"\n%10s %1s %12s %1s %8s %-25s %12s %12s %8s %-20s %12s %12s %8s %-20s %12s %12s<br>\n",\
L"line#", L"O", L"esküvõ",L"S",\
L"rowid", L"name", L"születés", L"halál",\
L"rowid", L"apja", L"születés", L"halál",\
L"rowid", L"anyja", L"születés", L"halál"\
);
	fwprintf( fh1, str );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::listHtml()
{
	CString header;
	CString people;
	CString father;
	CString mother;
	CString spouse1_id;
	CString spouse2_id;
#ifndef _DEBUG
	str.Format( L"Házasságok listázása..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	for( int i = 0; i < vSpouse1.size(); ++i )
	{
		spouse2_id = vSpouse1.at(i).rowid;
		header.Format( L"%10s %14s %1s", vSpouse1.at(i).linenumber, L" ", vSpouse1.at(i).source );
		people.Format( L"%8s %-25s %12s %12s", vSpouse1.at(i).rowid, vSpouse1.at(i).name, vSpouse1.at(i).birth, vSpouse1.at(i).death );
		father.Format( L"%8s %-20s %12s %12s", vSpouse1.at(i).rowidF, vSpouse1.at(i).father, vSpouse1.at(i).birthF, vSpouse1.at(i).deathF );
		mother.Format( L"%8s %-20s %12s %12s", vSpouse1.at(i).rowidM, vSpouse1.at(i).mother, vSpouse1.at(i).birthM, vSpouse1.at(i).deathM );
		fwprintf( fh1, L"<font color='red'>%s %s %s %s</font><br>", header, people, father, mother );
//		fwprintf( fh1, L"<span style=\"background:yellow\">%s %s %s %s</span>\n", header, people, father, mother );
		for( int j = 0 ; j< vSpouse2.size(); ++j )
		{
			spouse1_id = vSpouse2.at(j).rowidS;
			if( spouse2_id == spouse1_id )
			{
				spouse1_id = vSpouse2.at(j).rowid;
				header.Format( L"%10s %1s %12s %1s", vSpouse2.at(j).linenumber,  vSpouse2.at(j).order, vSpouse2.at(j).date, vSpouse2.at(j).source );
				people.Format( L"%8s %-25s %12s %12s", vSpouse2.at(j).rowid, vSpouse2.at(j).name, vSpouse2.at(j).birth, vSpouse2.at(j).death );
				father.Format( L"%8s %-20s %12s %12s", vSpouse2.at(j).rowidF, vSpouse2.at(j).father, vSpouse2.at(j).birthF, vSpouse2.at(j).deathF );
				mother.Format( L"%8s %-20s %12s %12s", vSpouse2.at(j).rowidM, vSpouse2.at(j).mother, vSpouse2.at(j).birthM, vSpouse2.at(j).deathM );
				fwprintf( fh1, L"%s %s %s %s\n", header, people, father, mother );
			}
		}
		fwprintf( fh1, L"\n" );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	fwprintf( fh1, L"</pre>" );
	fclose( fh1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::fillTable()
{
	CString rowid;
	CString rowidS;

#ifndef _DEBUG
	str.Format( L"Tábla feltöltése..." );
	wndP.SetText( str );
#endif

	wndP.SetRange( 0, vSpouse1.size()-1 );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	v_tableMarriages.clear();
	vColor.clear();
	for( int i = 0; i < vSpouse1.size(); ++i )
	{
		rowid = vSpouse1.at(i).rowid;
		push( vSpouse1.at(i).linenumber );
		push( L"" );							// order
		push( L"" );							// date
		push( vSpouse1.at(i).source );
		push( rowid );
		push( vSpouse1.at(i).name );
		push( vSpouse1.at(i).birth );
		push( vSpouse1.at(i).death );
		push( vSpouse1.at(i).rowidF );
		push( vSpouse1.at(i).father );
		push( vSpouse1.at(i).birthF );
		push( vSpouse1.at(i).deathF );
		push( vSpouse1.at(i).rowidM );
		push( vSpouse1.at(i).mother );
		push( vSpouse1.at(i).birthM );
		push( vSpouse1.at(i).deathM );
		vColor.push_back(1);
		for( int j = 0 ; j< vSpouse2.size(); ++j )
		{

			rowidS = vSpouse2.at(j).rowidS;
			if( rowidS == rowid )
			{
				push( vSpouse2.at(j).linenumber );
				push( vSpouse2.at(j).order );	
				push( vSpouse2.at(j).date );	
				push( vSpouse2.at(j).source );
				push( vSpouse2.at(j).rowid );
				push( vSpouse2.at(j).name );
				push( vSpouse2.at(j).birth );
				push( vSpouse2.at(j).death );
				push( vSpouse2.at(j).rowidF );
				push( vSpouse2.at(j).father );
				push( vSpouse2.at(j).birthF );
				push( vSpouse2.at(j).deathF );
				push( vSpouse2.at(j).rowidM );
				push( vSpouse2.at(j).mother );
				push( vSpouse2.at(j).birthM );
				push( vSpouse2.at(j).deathM );
				vColor.push_back(0);
			}
		}
		emptyRow();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.AttachDataset( &v_tableMarriages );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::push( CString str )
{
	int tLen=str.GetLength()+1;
	LPTSTR sT=new TCHAR[tLen];
	_tcscpy_s(sT,tLen,str.GetBuffer());
	v_tableMarriages.push_back(sT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::emptyRow()
{
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	push( L"" );
	vColor.push_back(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckMarriageOrder::query( CString command )
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
BOOL CCheckMarriageOrder::query1( CString command )
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
BOOL CCheckMarriageOrder::query2( CString command )
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
BOOL CCheckMarriageOrder::query3( CString command )
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
BOOL CCheckMarriageOrder::query4( CString command )
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
bool sortByNameX(const SPOUSE1 &v1, const SPOUSE1 &v2) 
{ 
    return( v1.name < v2.name ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;

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
		if( vColor.at(nItem) == 1 )
		{
			pLVCD->clrText = RGB( 255,0,0 );
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckMarriageOrder::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	if( m_ListCtrl.GetItemText( nItem, L_ORDER ) != L"" ) return;

	std::vector<CString> vLines;

	CString linenumber;
	CString linenumberPrev(L"");
	CString name = m_ListCtrl.GetItemText( nItem, L_NAME );

	while( (linenumber = m_ListCtrl.GetItemText( nItem, L_LINENUMBER ) ) != "" )
	{
		if( linenumber != linenumberPrev )
			vLines.push_back( linenumber );
		linenumberPrev = linenumber;
		++nItem;
	}

	CHtmlLines dlg;

	dlg.child = name;
	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
	*pResult = 0;
}


void CCheckMarriageOrder::OnKillfocusSearch()
{
	// TODO: Add your control notification handler code here
}
