// CSameCouples.cpp : implementation file
//



#include "stdafx.h"
#include "Dragon.h"
#include "SameCouples.h"
#include "afxdialogex.h"
#include "checkParam0.h"
#include "GetLastFirst.h"
#include <algorithm>
#include <vector> 
#include "ProgressWnd.h"
#include "utilities.h"
// lek�rdezett oszlopok a people t�bl�b�l
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
bool sortByC(const CONTRACT &v1, const CONTRACT &v2) 
{
	return( v1.rowidBy < v2.rowidBy );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cser�ljen
// return FALS cser�ljen
bool sortBySpouses(const COUPLES &v1, const COUPLES &v2) 
{ 
	if( v1.spouse1 < v2.spouse1 ) return true;
	if( v1.spouse1 == v2.spouse1 )
	{
		if( v1.spouse2 < v2.spouse2 ) return true;
		if( v1.spouse2 == v2.spouse2 ) 
		{
			return( v1.sourceS1 < v2.sourceS1 );
		}
		return false;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cser�ljen
// return FALS cser�ljen
bool sortByGroupStatus(const COUPLES &v1, const COUPLES &v2) 
{ 
	if( v1.group > v2.group )
		return false;
	if( v1.group == v2.group )
	{
		if( v1.status1 < v2.status1 )
			return true;
		else
			return false;
	}
	return true;
}
bool sortByGroup(const COUPLES &v1, const COUPLES &v2) 
{
	return( v1.group < v2.group );
}
bool sortByMarriage(const COUPLES &v1, const COUPLES &v2) 
{
	return( _wtoi( v1.rowidM ) < _wtoi( v2.rowidM ) );
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
Az oszlopok jelent�se:\n\n\
h_id     - a h�zass�g azonos�t�ja,\n\
h�zass�g - ah�zass�g d�tuma\n\
gr       - group, az azonos nev� embercsoprton bel�l azonosnak �r�klelt alcsoportok sorsz�ma.\n\n\
st       - status, az azonos�t�s eredm�nye: -1 azonos, azaz egyes�tett, majd t�r�lt, 0: v�ltozatlanul hagyott, 1: ez az egyes�tett bejegyz�s.\n\
line     - a GA html f�jlban a sor sz�ma, amelyben el�fordul.\n\
G        - gener�ci�, az ember gener�ci�s jele a GA f�jlban.\n\
S        - source, a n�v el�fordul�s�nak hierarchia sz�ma.\n\
U        - united, az ember u sz�m� bejegyz�s�nek �sszevon�sa.\n\
f�rj adatai:\n\
rowid    - a f�rj azonos�t�ja\n\
f�rj     - a f�rj neve.\n\
sz�let�s - sz�let�si d�tum.\n\
hal�l    - hal�loz�s d�tuma.\n\
apja     - a f�rj apj�nak neve\n\
anyja    - a f�rj anyj�nak neve.\n\n\
feles�g hasol� adatai.\n\n\
";

	m_info = L"\
Az al�bbi adatokat vizsg�ljuk az azonos nev� h�zasp�rok azonoss�g�nak eld�nt�s�hez:\r\n\
\r\n\
1. F�rj sz�let�si d�tuma\r\n\
2. F�rj hal�loz�si d�tuma\r\n\
3. F�rj apja neve\r\n\
4. F�rj anyja neve\r\n\
5. Feles�g sz�let�si d�tuma\r\n\
6. Feles�g hal�loz�si d�tuma\r\n\
7. Feles�g apja neve\r\n\
8. Feles�g anyja neve\r\n\
\r\n\
A kett�s keresztnevekb�l csak az els� haszn�ljuk az �sszehasoonl�t�shoz.\r\n\
\r\n\
Ha a vizsg�lt adatok k�z�tt ellentmond�s van, akkor nyilv�nval�an nem azonos szem�ly bejegyz�seir�l van sz�.\r\n\
Ha nincs ellentmond�s, akkor a megadott sz�m� adat l�tez�se �s egyez�se sz�ks�ges az azonoss�g meg�llap�t�s�hoz.\r\n\
\r\n\
Az adathib�k nyilv�n egyes�t�si hib�khoz vezetnek, val�j�ban k�l�nb�z� emberekhez tartoz� bejegyz�sket t�vesen \
egyes�t�nk, vagy azonos emberek bejegyz�seinek �sszevon�sa elmarad. Ez�rt az egyes�t�s el�tt minden hib�t \
ki kel jav�tani!.\r\n\
Erre szolg�lnak az \"Ellen�rz�se egyes�t�s el�tt\" funkci�k, de az egyes�t�s sor�n az azonos�t�s erdm�ny�t tartalmaz� \
f�jlok is felfedhetnek hib�kat.\r\n\
\r\n\
";

//El��rhatjuk, hogy az ellentmond�smentes adatok mellett h�ny adatp�r l�tez�s�t �s egyenl�s�g�t k�vetelj�k meg az \
//azonoss�g elfogad�s�hoz. Javasolt �rt�ke: 0.\r\n\

	m_columns.Format( L"\n<b>\
%6s %12s \
%2s \
%2s %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s \
%2s %8s %1s %1s %2s %8s %-30s %-13s %-13s %-30s %-30s</b>\n", \
L"h_id", L"h�zass�g",\
L"gr",\
L"st",L"line", L"G", L"S", L"U",\
L"rowid", L"f�rj", L"sz�let�s", L"hal�l", L"apja", L"anyja", \
L"st",L"line", L"G", L"S", L"U",\
L"rowid", L"feles�g", L"sz�let�s", L"hal�l", L"apja", L"anyja" \
);

	m_colors.Add( L"bisque" );
	m_colors.Add( L"aquamarine" );
	m_colors.Add( L"yellow" );
	m_colors.Add( L"deepskyblue" );
	m_colors.Add( L"greenyellow" );
	m_colors.Add( L"thistle" ); 
	m_colors.Add( L"hotpink" );
	m_colors.Add( L"lightyellow" );
	m_colors.Add( L"aqua" );
	m_colors.Add( L"moccasin" );

//Ha csak egy h�zasp�rt akatsz vizsg�lni, add meg a nev�ket

//	_husband	= L"Ajkay Istv�n";	// f�rj neve
//	_wife		= L"N N";			// feles�g neve
	m_contract	= false;		
	m_contract	= true;			// v�grejatsa-e az �sszevon�sokat	
	m_loopMax		= 3;			// max loop
	m_azonos	= 1;			// legal�bb ennyi egyez�s ellemtmond�s n�lk�l
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSameCouples::~CSameCouples()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_info);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSameCouples, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSameCouples::OnBnClickedOk)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSameCouples::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::OnBnClickedOk()
{
	CString drive;
	CString path;
	CString fname;
	CString ext;
	TCHAR* old;
	TCHAR* renamed;

	wndP.Create( NULL, L"Azonos nev� h�zasp�rok..." );
	wndP.GoModal();
	
	m_loop = 1;
	while( m_loop < m_loopMax )
	{

		theApp.setStartTime();

		splitFilespec( theApp.m_databaseSpec, &drive, &path,  &fname, &ext );
	
		str.Format( L"%s:%s\\%sM.%s", drive, path, fname, ext );
		CopyFile( theApp.m_databaseSpec, str, false );
		theApp.m_databaseSpec = str;

		str.Format( L"%s:%s\\%sP_blob.%s", drive, path, fname, ext );
		CopyFile( theApp.m_blobSpec, str, false );

		theApp.openDatabase();




		openUnited();
		openDifferent();

		vContract.clear();

		core( m_loop );


		theApp.setUserVersion( vContract.size() );


		if( vContract.size() )
			str.Format( L"%d ember �sszevonva.", vContract.size() );
		else
			str = L"Nincs �sszevonhat� ember!";

		fwprintf( fU, L"\n\n%s\n", str );
		fwprintf( fD, L"\n\n%s\n", str );
		fwprintf( fU, L"</pre>" );
		fwprintf( fD, L"</pre>" );
		fwprintf( fU, L"Eltelt id�: %s<br><br>", theApp.get_time_elapsed() );
		fwprintf( fD, L"Eltelt id�: %s<br><br>", theApp.get_time_elapsed() );

		fclose( fU );
		fclose( fD );


		if( vContract.size() )
		{
			theApp.showHtmlFile( unitedSpec );
			theApp.showHtmlFile( differentSpec );
		}
		else
		{
			theApp.showHtmlFile( differentSpec );
			break;
		}
		++m_loop;
	}
	wndP.DestroyWindow();		

#ifndef _DEBUG
	wndP.SetText( L"Az adatb�zis t�m�r�t�se..." );
#endif
	theApp.execute( L"VACUUM");

	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::core( int loop )
{
	CString first_name1;
	CString	lastName;
	CString firstName;
	COUPLES vcouples;
	CString spouse;
	CString mother_id;
	CString father_id;

	int		z;

	str.Format( L"Azonos nev� emberek h�zass�gainak vizsg�lata �s �sszevon�sa - %d. iter�ci�", loop );
	wndP.m_strTitle = str;

#ifndef _DEBUG
	str.Format( L"H�zass�gok lek�rdez�se az adatb�zisb�l..." );
	wndP.SetText( str );
#endif


	m_command.Format( L"SELECT rowid,* FROM marriages" );
	if( !query( m_command ) ) return;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vCouples.clear();
	for( int i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		vcouples.group		= 0;
		vcouples.status1	= 0;
		vcouples.status2	= 0;
		
		vcouples.rowidM		= m_recordset->GetFieldString( MARRIAGES_ROWID );
		vcouples.dateM		= m_recordset->GetFieldString( MARRIAGES_DATE );
		vcouples.rowidS1	= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		vcouples.rowidS2	= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
	
//f�rj
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS1 );
		if( !query1( m_command ) ) return;

		vcouples.sex_idS1		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS1			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS1	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS1		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS1		= m_recordset1->GetFieldString( P_UNITED );

		lastName				= m_recordset1->GetFieldString( P_LAST_NAME);
		firstName				= m_recordset1->GetFieldString( P_FIRST_NAME );
		str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		str.TrimRight();
		if( str.IsEmpty() ) goto cont;
		vcouples.spouse1		= str;

		if( str == L"Harmos Tam�s" )
			z = 1;


		vcouples.birthS1		= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathS1		= m_recordset1->GetFieldString( P_DEATH_DATE );
		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );
		vcouples.mother_idS1	= mother_id;
		father_id				= m_recordset1->GetFieldString( P_FATHER_ID );
		vcouples.father_idS1	= father_id;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName	= m_recordset1->GetFieldString( 0 );
			firstName	= m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.motherS1 = str;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", father_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName	= m_recordset1->GetFieldString( 0 );
			firstName	= m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.fatherS1 = str;

		vcouples.generationS1.Trim();

// feles�g
		m_command.Format( L"SELECT %s FROM people WHERE rowid='%s'", p_fields, vcouples.rowidS2 );
		if( !query1( m_command ) ) return;

		vcouples.sex_idS2		= m_recordset1->GetFieldString( P_SEX_ID );
		vcouples.lineS2			= m_recordset1->GetFieldString( P_LINE );
		vcouples.generationS2	= m_recordset1->GetFieldString( P_GENERATION );
		vcouples.sourceS2		= m_recordset1->GetFieldString( P_SOURCE );
		vcouples.unitedS2		= m_recordset1->GetFieldString( P_UNITED );

		lastName				= m_recordset1->GetFieldString( P_LAST_NAME);
		firstName				= m_recordset1->GetFieldString( P_FIRST_NAME );
		str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		str.TrimRight();
		if( str.IsEmpty() ) goto cont;
		vcouples.spouse2		= str;

		father_id				= m_recordset1->GetFieldString( P_FATHER_ID );
		vcouples.father_idS2	= father_id;
		vcouples.birthS2		= m_recordset1->GetFieldString( P_BIRTH_DATE );
		vcouples.deathS2		= m_recordset1->GetFieldString( P_DEATH_DATE );
		mother_id				= m_recordset1->GetFieldString( P_MOTHER_ID );
		vcouples.mother_idS2	= mother_id;

// ez nem j�		if( !_wife.IsEmpty() && spouse != _wife ) goto cont;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", mother_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName = m_recordset1->GetFieldString( 0 );
			firstName = m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
		}
		vcouples.motherS2 = str;

		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid='%s'", father_id );
		if( !query1( m_command ) ) return;
		str.Empty();
		if( m_recordset1->RecordsCount() )
		{
			lastName = m_recordset1->GetFieldString( 0 );
			firstName = m_recordset1->GetFieldString( 1 );
			str.Format( L"%s %s", lastName, sepFirstName( firstName ) );
			str.TrimRight();
			vcouples.fatherS2 = str;
		}
		vcouples.fatherS2 = str;
		
		vcouples.generationS2.Trim();
		vcouples.cnt = i;
		vCouples.push_back( vcouples );
cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	

#ifndef _DEBUG
	str.Format( L"Lek�rdezett adatok rendez�se..." );
	wndP.SetText( str );
#endif

	wndP.SetPos(0);
	std::sort( vCouples.begin(), vCouples.end(), sortBySpouses );   // h�zasp�rok n�v szerinti rendez�se

#ifndef _DEBUG
	str.Format( L"Azonos h�zast�rsak �s h�zass�gok �sszevon�sa..." );
	wndP.SetText( str );
#endif

	wndP.SetPos(0);

	CString spouse1;
	CString spouse2;
	CString name;
	CString namePrev(L"");
	bool	first = true;
	
	
	wndP.SetRange( 0, vCouples.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	vSame.clear();

	// leszedi a vSame vektorba az azaonos nev� h�zasp�rokat �s azt feldolgozza
	for( ic = 0; ic < vCouples.size(); ++ic )
	{
		spouse1 =  vCouples.at(ic).spouse1;
		spouse2 =  vCouples.at(ic).spouse2;

		if( !_husband.IsEmpty()  && _husband != spouse1 ) goto cont2;
		if( !_wife.IsEmpty() && _wife != spouse2 ) goto cont2;
		name.Format( L"%-40s %-40s", spouse1, spouse2 );
		if( name == namePrev )
		{
			if( first )			// a csoport els� h�zasp�rj�nak ment�se
			{
				vSame.push_back( vCouples.at(ic-1) );
				first = false;
			}
			vSame.push_back( vCouples.at(ic) );
		}
		else
		{
			if( vSame.size() )
			{
				processSame();
				vSame.clear();
			}
			first = true;
		}
		namePrev = name;

cont2:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( vSame.size() )
	{
		processSame();
		vSame.clear();
	}

	theApp.execute( L"BEGIN" );
	contractFull( loop );
	deleteMarriages( loop );
	theApp.execute( L"COMMIT" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Egy vSame-ben adott azonos nev� h�zast�rsak rekordjaiban be�ll�tja a group, statusS1 �s statusS2 �rt�keit.
// A group 0-n �rt�ket vehet fel, �s a vSame-en bel�li lehets�ges csoportokat jel�li. Egy-egy group �rt�k� h�zasp�rok azonosak.  
// 
void CSameCouples::processSame()
{

	UINT group = 1;
	UINT db = 0;
	int z;
	CString rowid1;
	CString rowid2;
	
	resetRef();		// ez kell, hogy a kor�bbi ref-et t�r�lje!!
	for( UINT i1 = 0; i1 < vSame.size(); ++i1 )
	{
		rowid1 = vSame.at(i1).rowidS1;
		
		if( db ) 
		{
			resetRef();
			++group;
		}
		db = 0;
		if( vSame.at(i1).group == 0 )		// az i1. h�zasp�rt m�g nem rendelt�k hozz� egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vSame.size(); ++i2 )
			{
				rowid2 = vSame.at(i2).rowidS1;
				if( i1 != i2 && vSame.at(i2).group == 0 )	// term�szetesen csak k�l�nb�z� h�zasp�rokat vizsg�l, amelyeket m�g nem redneltek hozz� egyik csooprthoz sem
				{
					if( identical( i1, i2 ) )
					{
						setRef( i1 );
						setRef( i2 );

						vSame.at( i1 ).group = group;
						vSame.at( i2 ).group = group;
						++db;
					}
				}
 			}
		}
	}



	// a legalacsonyabb szerep kiv�laszt�sa csoportonk�nt
	// status1 = 1, a legalacsonyabb szerepre, ami megtart�sra ker�l
	// status1 = -1, az �ssze st�bbi, ami t�rl�sre ker�l.

	m_numOfGroups = group - 1;
	UINT ix1;
	UINT ix2;
	int sourceMin1;
	int sourceMin2;
	int sourceS1;
	int sourceS2;


	std::sort( vSame.begin(), vSame.end(), sortByMarriage );  //cnt a vCouples rekord sorsz�ma!! Erre valami�rt sz�ks�g van!!!
	std::sort( vSame.begin(), vSame.end(), sortByGroup );		// ez nem kell tal�n, b�r?
	// csoport legalacsonyabb szerep� index�nek meghat�roz�sa
	for( UINT i = 1; i < m_numOfGroups + 1; ++i )
	{
		sourceMin1 = INT_MAX;
		sourceMin2 = INT_MAX;
		ix1 = 0;
		ix2 = 0;
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			str = vSame.at(j).spouse1;
			if( str == L"Amad� Istv�n" )
				z = 1;
			if( vSame.at(j).group == i )
			{
				sourceS1 = _wtoi( vSame.at(j).sourceS1 );
				if( sourceS1 < sourceMin1 )	ix1 = j;
				sourceMin1 = __min( sourceMin1, sourceS1 );
			
				sourceS2 = _wtoi( vSame.at(j).sourceS2 );
				if( sourceS2 < sourceMin2 )	ix2 = j;
				sourceMin2 = __min( sourceMin2, sourceS2 );
			}
		}
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == i )
			{
				if( j == ix1 ) vSame.at(j).status1 = 1;
				else vSame.at(j).status1 = -1;
				if( j == ix2 ) vSame.at(j).status2 = 1;
				else vSame.at(j).status2 = -1;
			}
		}
	}
	contract();
	if( m_contracted )
		listUnited();
	else
		listDiff();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::resetRef()
{

	r.birthS1.Empty();
	r.birthS2.Empty();
	r.cnt = 0;
	r.contracted = 0;
	r.dateM.Empty();
	r.deathS1.Empty();
	r.deathS2.Empty();
	r.fatherS1.Empty();
	r.fatherS2.Empty();
	r.father_idS1.Empty();
	r.father_idS2.Empty();
	r.generationS1.Empty();
	r.generationS2.Empty();
	r.group = 0;
	r.lineS1.Empty();
	r.lineS2.Empty();
	r.motherS1.Empty();
	r.motherS2.Empty();
	r.mother_idS1.Empty();
	r.mother_idS2.Empty();
	r.rowidM.Empty();
	r.rowidS1.Empty();
	r.rowidS2.Empty();
	r.sex_idS1.Empty();
	r.sex_idS2.Empty();
	r.spouse1.Empty();
	r.spouse2.Empty();
	r.status1 = 0;
	r.status2 = 0;
	r.unitedS1.Empty();
	r.unitedS2.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::setRef( int i )
{
	COUPLES a = vSame.at(i );

	r.rowidS1 = a.rowidS1;
	r.rowidS2 = a.rowidS2;

	if( r.birthS1.IsEmpty() && !a.birthS1.IsEmpty() )				r.birthS1 = a.birthS1;
	if( r.deathS1.IsEmpty() && !a.deathS1.IsEmpty() )				r.deathS1 = a.deathS1;
	if( r.fatherS1.IsEmpty() && !a.fatherS1.IsEmpty() )				r.fatherS1 = a.fatherS1;
	if( r.father_idS1.IsEmpty() && !a.father_idS1.IsEmpty() )				r.father_idS1 = a.father_idS1;
	if( r.motherS1.IsEmpty() && !a.motherS1.IsEmpty() )				r.motherS1 = a.motherS1;
	if( r.mother_idS1.IsEmpty() && !a.mother_idS1.IsEmpty() )		r.mother_idS1 = a.mother_idS1;

	if( r.birthS2.IsEmpty() && !a.birthS2.IsEmpty() )				r.birthS2 = a.birthS2;
	if( r.deathS2.IsEmpty() && !a.deathS2.IsEmpty() )				r.deathS2 = a.deathS2;
	if( r.fatherS2.IsEmpty() && !a.fatherS2.IsEmpty() )				
		r.fatherS2 = a.fatherS2;
	if( r.motherS2.IsEmpty() && !a.motherS2.IsEmpty() )				r.motherS2 = a.motherS2;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 �s i2 index� h�zassp�rok azonoss�g�t �llap�thja meg.
// return true: azonosak (nincs ellentmond�s az adatai k�z�tt!!!) 
// return false: k�l�nb�z�ek
bool CSameCouples::identical( UINT i1, UINT i2 )
{
	COUPLES a = vSame.at(i1);
	COUPLES b = vSame.at(i2);

	if( !a.generationS1.IsEmpty() && !b.generationS1.IsEmpty() ) return false;

	int	good = 0;
	int g;



	if( ( g = same( r.birthS1, a.birthS1, b.birthS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.deathS1, a.deathS1, b.deathS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.fatherS1, a.fatherS1, b.fatherS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.motherS1, a.motherS1, b.motherS1 ) ) == -1 ) return false;
	if( g == 1 ) ++good;


	if( ( g = same( r.birthS2, a.birthS2, b.birthS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.deathS2, a.deathS2, b.deathS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.fatherS2, a.fatherS2, b.fatherS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( r.motherS2, a.motherS2, b.motherS2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( m_azonos <= good ) return true;				// legal�bb _azonos egyez�s ellentmond�s n�lk�l 
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Azonos nev� �s sz�let�si, hal�loz�si d�tumokban nem ellentmond�sos h�zasp�rok h�zass��gainak egyes�t�se
// A spouse1-b�l �s a spouse2-b�l a legkisebb source-� embert tartja meg
void CSameCouples::contract()
{
	CString rowidBy;
	CString rowid;
	CString rowidM;
	CString sex_id;
	CString name;
	int		source;
	CString rowidMD;
	int		group;
	int		z;
	CONTRACT vcontract;

	if( vSame.at(0).spouse1 == L"Bark�czy L�szl�"  && vSame.at(0).spouse2 == L"Krucsay Kl�ra" )
		z = 1;

	m_contracted = false;
	// csoporton bel�l keresi a t�rlend� �s megtartand� sorokat

	for( UINT i = 0; i < m_numOfGroups; ++i )
	{
		group = i + 1;
		// megtartand� f�rj bejegyz�s
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group && vSame.at(j).status1 == 1 )
			{
				rowidBy = vSame.at(j).rowidS1;
				rowidM  = vSame.at(j).rowidM;
				source	= _wtoi( vSame.at(j).sourceS1 );
				break;
			}
		}

		if( rowidBy.IsEmpty() ) 
			return;

		// t�rlend� f�rj bejegyz�s
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group )
			{
				if( vSame.at(j).status1 == -1 )
				{
					rowidMD = vSame.at(j).rowidM;
					rowid	= vSame.at(j).rowidS1;
					sex_id	= vSame.at(j).sex_idS1;

					if( rowid != rowidBy )  // akkor fordulhat el�, ha egy kor�bban h�zass�gban rowid-t m�r fel�l�rt�k rowidBy-al
					{ 
						vcontract.rowid		= rowid;
						vcontract.rowidBy	= rowidBy;
						vcontract.sex_id	= sex_id;
						vContract.push_back( vcontract );
					}
					m_contracted = true;
				}
			}
		}

		// negtartand� feles�g bejegyz�s
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group && vSame.at(j).status2 == 1 )  // mi�rt nem l�p ki, ha megvan?
			{
				rowidBy = vSame.at(j).rowidS2;
				rowidM  = vSame.at(j).rowidM;
				break;
			}
 		}

		if( rowidBy.IsEmpty() ) 
			return;

		// t�rlend� feles�g bejegyz�s
		for( UINT j = 0; j < vSame.size(); ++j )
		{
			if( vSame.at(j).group == group )
			{
				if( vSame.at(j).status2 == -1 )
				{
					rowidMD = vSame.at(j).rowidM;
					rowid	= vSame.at(j).rowidS2;
					sex_id	= vSame.at(j).sex_idS2;
					source	= _wtoi( vSame.at(j).sourceS2 );

					if( rowid != rowidBy )
					{
						vcontract.rowid		= rowid;
						vcontract.rowidBy	= rowidBy;
						vcontract.sex_id	= sex_id;
						vContract.push_back( vcontract );
					}
					++vSame.at(0).contracted;
					m_contracted = true;
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::contractFull( int loop )
{
	if( !m_contract ) return;

	CString rowid;
	CString rowidBy;
	CString sex_id;

#ifndef _DEBUG
	str.Format( L"T�rlend�-megtartand� azonos�t�k korrig�l�sa ..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	std::sort( vContract.begin(), vContract.end(), sortByC );
	for( UINT i = 0; i < vContract.size(); ++i )
	{
		rowid	= vContract.at(i).rowid;
		rowidBy = vContract.at(i).rowidBy;
		for( UINT j = 0; j < vContract.size(); ++j )
		{
			if( vContract.at(j).rowidBy == rowid ) 
				vContract.at(j).rowidBy = rowidBy;
			else if( rowid  < vContract.at(j).rowidBy ) break;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}


#ifndef _DEBUG
	str.Format( L"T�rl�s...", loop );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	for( UINT i = 0; i < vContract.size(); ++i )
	{
		rowid	= vContract.at(i).rowid;
		rowidBy = vContract.at(i).rowidBy;
		sex_id	= vContract.at(i).sex_id;

		m_command.Format( L"DELETE FROM people WHERE rowid ='%s'", rowid );
		if( !theApp.execute( m_command ) ) return;

		if( sex_id == L"1" )
			m_command.Format( L"UPDATE people SET father_id = '%s' WHERE father_id='%s'", rowidBy, rowid );
		else
			m_command.Format( L"UPDATE people SET mother_id = '%s' WHERE mother_id='%s'", rowidBy, rowid );
		if( !theApp.execute( m_command ) ) return;

		if( sex_id == L"1" )
			m_command.Format( L"UPDATE marriages SET spouse1_id='%s' WHERE spouse1_id ='%s'", rowidBy, rowid );
		else
			m_command.Format( L"UPDATE marriages SET spouse2_id='%s' WHERE spouse2_id ='%s'", rowidBy, rowid );
		if( !theApp.execute( m_command ) ) return;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::deleteMarriages( int loop )
{

#ifndef _DEBUG
	str.Format( L"H�zass�gok ellen�rz�se �s t�rl�se..." );
	wndP.SetText( str );
#endif
	// A legkisebb szerepk�d� h�zass�got tartja meg ( ORDER BY ... source )
	m_command = L"SELECT rowid, spouse1_id, spouse2_id FROM marriages ORDER BY spouse1_id, spouse2_id, source ";
	if( !query( m_command ) ) return;

	CString rowid11;
	CString rowid21;
	CString rowid12;
	CString rowid22;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		rowid11 = m_recordset->GetFieldString( 1 );
		rowid21 = m_recordset->GetFieldString( 2 );
		m_recordset->MoveNext();
		rowid12 = m_recordset->GetFieldString( 1 );
		rowid22 = m_recordset->GetFieldString( 2 );
		if( rowid11 == rowid12 && rowid21 == rowid22 )
		{
			m_command.Format( L"DELETE FROM marriages WHERE rowid = '%s'", m_recordset->GetFieldString( 0 ) );
			if( !theApp.execute( m_command ) ) return;
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.SetPos(0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listDiff()
{
	bool first;
	for( UINT j = 0; j <= m_numOfGroups; ++j )
	{
		first = true;
		for( UINT i = 0; i < vSame.size();++i )
		{
			if( vSame.at(i).group == j )
			{
				if( first )
				{
					r = vSame.at(j);
					first = false;
				}
				printYellow( i );
			}
		}
	}
	fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ki�rja az i2 rekordot, az i1 rekordhoz k�pesti elt�r�seket s�rga h�tt�rrel
void CSameCouples::printYellow( UINT i )
{
	COUPLES b = vSame.at( i );

	str.Format( L"%6s %12s %2d %2d %8s ", b.rowidM, b.dateM, b.group, b.status1, b.lineS1 );
	fwprintf( fD, L"%s", str );

	if( !r.generationS1.IsEmpty() && !b.generationS1.IsEmpty() && r.generationS1 != b.generationS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", b.generationS1 );
	}
	else
		fwprintf( fD, L"%1s ", b.generationS1 );

	str.Format( L"%1s %2s %8s %-30s ", b.sourceS1, b.unitedS1, b.rowidS1, b.spouse1 );
	fwprintf( fD, L"%s", str );

// birth
	if( !r.birthS1.IsEmpty() && !b.birthS1.IsEmpty() && r.birthS1 != b.birthS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.birthS1 );
	}
	else
		fwprintf( fD, L"%-13s ", b.birthS1 );// deathDate1


// death
	if( !r.deathS1.IsEmpty() && !b.deathS1.IsEmpty() && r.deathS1 != b.deathS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.deathS1 );
	}
	else
		fwprintf( fD, L"%-13s ", b.deathS1 );

// father1
	if( !r.fatherS1.IsEmpty() && !b.fatherS1.IsEmpty() && r.fatherS1 != b.fatherS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.fatherS1 );
	}
	else
		fwprintf( fD, L"%-30s ", b.fatherS1 );

// mother1
	if( !r.motherS1.IsEmpty() && !b.motherS1.IsEmpty() && r.motherS1 != b.motherS1 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.motherS1 );
	}
	else
		fwprintf( fD, L"%-30s ", b.motherS1 );

	str.Format( L"%2d %8s ", b.status2, b.lineS2 );
	fwprintf( fD, L"%s", str );

// generation
	if( !r.generationS2.IsEmpty() && !b.generationS2.IsEmpty() && r.generationS2 != b.generationS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%1s</span> ", b.generationS2 );
	}
	else
		fwprintf( fD, L"%-1s ", b.generationS2 );

	str.Format( L"%1s %2s %8s %-30s ", b.sourceS2, b.unitedS2, b.rowidS2, b.spouse2 );
	fwprintf( fD, L"%s", str );

// birthS2
	if( !r.birthS2.IsEmpty() && !b.birthS2.IsEmpty() && r.birthS2 != b.birthS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.birthS2 );
	}
	else
		fwprintf( fD, L"%-13s ", b.birthS2 );


// deathDate2
	if( !r.deathS2.IsEmpty() && !b.deathS2.IsEmpty() && r.deathS2 != b.deathS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-13s</span> ", b.deathS2 );
	}
	else
		fwprintf( fD, L"%-13s ", b.deathS2 );

// father2
	if( !r.fatherS2.IsEmpty() && !b.fatherS2.IsEmpty() && r.fatherS2 != b.fatherS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.fatherS2 );
	}
	else
		fwprintf( fD, L"%-30s ", b.fatherS2 );

// mother2
	if( !r.motherS2.IsEmpty() && !b.motherS2.IsEmpty() && r.motherS2 != b.motherS2 )
	{
		fwprintf( fD, L"<span style=\"background:yellow\">%-30s</span> ", b.motherS2 );
	}
	else
		fwprintf( fD, L"%-30s ", b.motherS2 );

	fwprintf( fD, L"\n");

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::listUnited()
{
	int z;
	int index;
	CString color;
	COUPLES a;
	std::sort( vSame.begin(), vSame.end(), sortByGroupStatus );

	for( UINT i = 0; i < vSame.size();++i )
	{
		a = vSame.at(i);
//f�rj
		str.Format( L"\
%6s %12s %2d \
%2d %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s",\
a.rowidM, a.dateM, a.group, \
a.status1, a.lineS1, a.generationS1, a.sourceS1, a.unitedS1,\
a.rowidS1, a.spouse1, a.birthS1, a.deathS1, a.fatherS1, a.motherS1 );
		if( a.status1 != 0 && a.status1 != -1 )
		{
			index = a.group;
			if( index > 5 ) index = 0;
			if( index == 2 )
				z = 1;
			color = m_colors.GetAt( index );
			fwprintf( fU, L"<span style=\"background:%s\">%s</span> ", color, str );
		}
		else if( a.status1 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span> ", str );
		else
			fwprintf( fU, L"%s ", str );
//feles�g
		str.Format( L"\
%2d %8s %1s %1s %2s \
%8s %-30s %-13s %-13s %-30s %-30s",\
a.status2, a.lineS2, a.generationS2, a.sourceS2, a.unitedS2, \
a.rowidS2, a.spouse2, a.birthS2, a.deathS2, a.fatherS2, a.motherS2 );
		if( a.status2 != 0 && a.status2 != -1 )
		{
			index = a.group;
			if( index > 5 ) index = 0;
			color = m_colors.GetAt( index );
			fwprintf( fU, L"<span style=\"background:%s\">%s</span>", color, str );
		}
		else if( a.status2 == -1 )
			fwprintf( fU, L"<span style=\"background:LightGray\">%s</span>", str );
		else
			fwprintf( fU, L"%s ", str );

		fwprintf( fU, L"\n");
	}
	fwprintf( fU, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::openUnited()
{
	CString fileName;
	fileName.Format( L"couplesUnited_%d", m_loop );
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	createHead( L"AZONOS NEV� H�ZASP�ROK, AKIK AZONOSAK, EZ�RT �SSZEVONHAT�AK" ); 

	fwprintf( fU, m_head );
	fwprintf( fU, m_description );

	str = L"A sz�rke h�tter� bejegyz�seket egyes�tett�k a z�ld h�tter� bejegyz�ssel. A feh�r h�tter� bejegyz�sek v�ltozatlanok maradtak.\n\
Ha egy azonos nev� csoportban t�bb k�l�nb�z� egyes�t�s lehets�ges, akkor azok a 'gr'-group oszlopba tal�lhat� sz�mmal vannak megl�l�nb�ztetve.\n\n";
	fwprintf( fU, str );

	fwprintf( fU, m_columns );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::openDifferent()
{
	CString fileName;
	fileName.Format( L"couplesDifferent_%d", m_loop );
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	createHead( L"AZONOS NEV� H�ZASP�ROK, AKIK K�L�NB�ZNEK EGYM�ST�L" ); 
	fwprintf( fD, m_head );
	fwprintf( fD, m_description );

	str.Format( L"A s�rga h�tter� adat k�l�nb�zik a csoport els� sor�ban l�v� megfelel� adatt�l.\n\n" );
	fwprintf( fD, str );

	fwprintf( fD, m_columns );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSameCouples::createHead( CString title )
{
	m_head.Format( L"\
<HEAD>\n\
<style>\n\
</style>\n\
</HEAD>\n\
<BODY>\n\
<center>%s</center><br><br>\n\n\
<pre>\n\
%-21s %s<br>\
%-21s %s<br><br>\
%-21s %d<br><br>\
",
title,\
L"Adatb�zis:",\
theApp.m_databaseSpec,\
L"Lista k�sz�lt:",\
theApp.getPresentDateTime(),\
L"Egyez�sek min. sz�ma:",\
m_azonos\
);
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
