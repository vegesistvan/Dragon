// SamePeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SamePeople.h"
#include "afxdialogex.h"
#include <algorithm>
#include "Param1.h"
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
bool sortBySource(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.source < v2.source );
}
bool sortByMother(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.mother < v2.mother );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cseréljen
// return FALS cseréljen
bool sortByGroupStatus(const SAMENAMES &v1, const SAMENAMES &v2) 
{ 
	if( v1.group > v2.group )
		return false;
	if( v1.group == v2.group )
	{
		if( v1.status < v2.status )
			return true;
		else
			return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ha referencia és par2 is meg van adva, akkor ahhoz hasonlítja par2-t.
// Ha nincs, de par1 és par2 is adva van, akkor ezeket hasonlítja.
// par1 vagy par2 üres, akkor return 0;
//  1 : mindkettõ meg van adva és egyezik;
//  0 : csak az egyik vagy egy sincs van megadva, így nincs ellentmondás
//  -1 : mindkettõ meg van adva és nem egyezik
int same( CString ref, CString par1, CString par2 )
{
	if( !ref.IsEmpty() && !par2.IsEmpty() )
	{
		if( par1 == par2 )
			return 1;
		else
			return -1;
	}
	if( !par1.IsEmpty() && !par2.IsEmpty() )
	{
		if( par1 == par2 )
			return 1;
		else
			return -1;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSamePeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSamePeople::CSamePeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSamePeople::IDD, pParent)
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
Az oszlopok jelentése:\n\n\
gr     - group, az azonos nevû embercsoprton belül azonosnak éréklelt alcsoportok sorszáma.\n\
st     - status, az azonosítás eredménye: -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\n\
line   - a bejegyzés sorszáma a GA html fájlban.\n\
u      - united, az ember u számú bejegyzés összevonása.\n\
m      - married, X: házasember, üres: nem házas.\n\
G      - generáció, az ember generációs jele a GA fájlban.\n\
S      - az enber elõfordulása ( 1-2-3-4 )\n\
rowid  - a bejegyzés azonosítója\n\
name   - az ember neve\n\
birth  - születési dátum\n\
death  - halálozás dátuma\n\
father - apa neve\n\
mother - anya neve.\n\n\
";

	m_columns.Format( L"\
<b>\
%90s %s %s\n\
%2s %2s %9s %2s %1s %1s %1s \
%8s \
%-30s %12s %12s \
%-30s %12s %12s \
%-30s %12s %12s\n\
</b>\n",\
L" ",\
L"___________________ father______________________________",\
L"____________________mother______________________________",\
L"gr", L"st", L"line", L"u", L"m", L"G", L"S",\
L"rowid",\
L"name", L"birth", L"death",\
L"father", L"birth", L"death",\
L"mother", L"birth", L"death"\
);

//	m_name = L"Balás Anna";   // ha csak egy embert akarunk vizsgálni, itt megadhatjuk a nevét
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSamePeople::~CSamePeople()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSamePeople, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSamePeople::OnInitDialog()
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

	CString info;
	info = L"\
Az alábbi adatokat vizsgáljuk az azonos nevû emberek azonosságának eldöntéséhez:\r\n\
\r\n\
1. születési dátuma\r\n\
2. halálozási dátuma\r\n\
3. Férj apja neve\r\n\
4. Férj anyja neve\r\n\
\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasoonlításhoz.\r\n\r\n\
Ha a vizsgált adatok között ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor a megadott számú adat egyezése szükséges az azonosság megállapításához.\
\r\n\r\n\
";


	CParam1 dlg;
	dlg.m_caption	= L"Azonos nevû emberek összevonása";
	dlg.m_info		= info;
	dlg.m_text		= L"Az azonossághoz szükséges egyezõ adatok száma:";
	dlg._default	= 1;
	if( dlg.DoModal() == IDCANCEL )
	{
		OnCancel();
		return false;
	}
	_azonos	= dlg._azonos;


	openDifferent();
	openUnited();

	m_deleted = 0;
	collectPeople();

	m_command.Format( L"PRAGMA user_version='2'" );
	theApp.execute( m_command );

	if( m_deleted )
	{
		if( m_deleted > 100 )
		{
#ifndef _DEBUG
			str.Format( L"Adatbázis tömörítése..." );
			wndP.SetText( str );
#endif
	//		theApp.execute( L"VACUUM");
		}
		wndP.DestroyWindow();
		str.Format( L"\n\n%d bejegyzés került összevonásra.\n\n", m_deleted );
		fwprintf( fU, str );
		fclose( fU );
		theApp.showHtmlFile( unitedSpec );
	}
	else
	{
		wndP.DestroyWindow();
		fwprintf( fD, L"nincs összevonható bejegyzés.\n\n" );
		fclose( fU );
	}
	fclose( fD );
	theApp.showHtmlFile( differentSpec );
	OnCancel();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::collectPeople()
{
	CString namePrev;
	CString name;
	CString lastName;
	CString firstName;
	bool	first = true;;
	int		pos;
	
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
		lastName  = m_recordset->GetFieldString( P_LAST_NAME );
		if( lastName.IsEmpty() ) goto cont;

		firstName = m_recordset->GetFieldString( P_FIRST_NAME );

		name.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		if( name.TrimRight().IsEmpty() ) goto cont;
		if( !m_name.IsEmpty() && name != m_name ) goto cont;

		
		if( name == namePrev )
		{
			if( first )
			{
				putPeople( name, i-1 );
				first = false;
			}
			putPeople( name, i );
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
	theApp.execute( L"COMMIT" );

	if( vPeople.size() )
		processPeople();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::putPeople( CString name, UINT i )
{
	m_recordset->MoveTo( i );

	SAMENAMES vpeople;
	CString rowid = m_recordset->GetFieldString( P_ROWID );
	CString spouses;
	CString fullname;
	CString spouse_id;


	int z;
	if( rowid == L"136437" )
		z = 1;

	vpeople.name		= name;
	vpeople.group		= 0;
	vpeople.status		= 0;
	vpeople.rowid		= rowid;
	vpeople.line		= m_recordset->GetFieldString( P_LINE );
	vpeople.generation	= m_recordset->GetFieldString( P_GENERATION );
	vpeople.generation.Trim();
	vpeople.source		= m_recordset->GetFieldString( P_SOURCE );
	vpeople.united		= m_recordset->GetFieldString( P_UNITED );
	vpeople.sex_id		= m_recordset->GetFieldString( P_SEX_ID );
	vpeople.birth		= m_recordset->GetFieldString( P_BIRTH_DATE );
	vpeople.death		= m_recordset->GetFieldString( P_DEATH_DATE );


	CString father_id = m_recordset->GetFieldString( P_FATHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", father_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.father = name.TrimRight();
	vpeople.birthF = m_recordset1->GetFieldString( 2 );
	vpeople.deathF = m_recordset1->GetFieldString( 3 );

	CString mother_id = m_recordset->GetFieldString( P_MOTHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", mother_id );
	if( !query1( m_command ) ) return;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.mother = name.TrimRight();
	vpeople.birthM = m_recordset1->GetFieldString( 2 );
	vpeople.deathM = m_recordset1->GetFieldString( 3 );

	if( vpeople.sex_id == L"1" )
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query1( m_command ) ) return;
	int married = _wtoi( m_recordset1->GetFieldString( 0 ) );
	vpeople.married = married;


	if( vpeople.sex_id == L"1" )
		m_command.Format( L"SELECT spouse2_id FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT spouse1_id FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query1( m_command ) ) return;

	
	for( UINT i = 0; i < m_recordset1->RecordsCount(); ++i )
	{
		spouse_id = m_recordset1->GetFieldString( 0 );
		m_command.Format( L"SELECT last_name, first_name FROM people WHERE rowid = '%s'", spouse_id );
		if( !query2( m_command ) ) return;
			fullname.Format( L"%s %s", m_recordset2->GetFieldString(0), sepFirstName( m_recordset2->GetFieldString(1) ) );
		spouses += fullname.Trim();
		spouses += L", ";
		m_recordset1->MoveNext();
	}
	if( !spouses.IsEmpty() )
		spouses = spouses.Left( spouses.GetLength() - 2); 
	vpeople.spouses = spouses;
	vPeople.push_back( vpeople );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::processPeople()
{
	UINT group = 1;
	UINT db = 0;
	int z;
	CString gen1;
	CString gen2;
	int source1;
	int source2;
	CString line1;
	CString line2;
	CString spouse1;
	CString spouse2;
	if( vPeople.at(0).name == L"Battyhány Zsigmond" )
		z = 1;
	m_contracted = 0;
	resetRef();
	std::sort( vPeople.begin(), vPeople.end(), sortBySource );
	for( UINT i1 = 0; i1 < vPeople.size(); ++i1 )
	{
		if( db )
		{
			++group;
			resetRef();  // ha új csoport kezdõdik, akkor üres rf-ek kellenek 

		}
		db = 0;
		if( vPeople.at(i1).group == 0 )		// az i1. házaspárt még nem rendelték hozzá egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vPeople.size(); ++i2 )
			{
				if( i1 == 1 && i2 == 3 )
					z = 2;
				if( i1 != i2 && vPeople.at(i2).group == 0 )	// természetesen csak különbözõ házaspárokat vizsgál, amelyeket még nem redneltek hozzá egyik csooprthoz sem
				{
					spouse1 = vPeople.at(i1).spouses;
					spouse2 = vPeople.at(i2).spouses;
					if( spouse1 == L"Kubinyi Erzsébet" && spouse2 == spouse1 )
						z = 1;
					gen1 = vPeople.at( i1 ).generation;
					gen2 = vPeople.at( i2 ).generation;
					if( !gen1.IsEmpty() && !gen2.IsEmpty() ) continue;
					if( identical( i1, i2 ) )
					{
						setRef( i1 );	
						setRef( i2 );

						gen1 = vPeople.at( i1 ).generation;
						gen2 = vPeople.at( i2 ).generation;


						line1 = vPeople.at(i1).line;
						line2 = vPeople.at(i2).line;
						
						source1 = _wtoi( vPeople.at(i1).source ); 
						source2 = _wtoi( vPeople.at(i2).source ); 

						if( (line1 == line2) && (source1 == source2) || gen1.IsEmpty() || gen2.IsEmpty() )
						{
							vPeople.at( i1 ).group = group;
							vPeople.at( i2 ).group = group;
							vPeople.at( i1 ).status = 1;
							vPeople.at( i2 ).status = -1;
							contract( i1, i2 );
							++db;
						}
					}
				}
			}
		}
	}
	listPeople();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 és i2 indexû házasspárok azonosságát állapíthja meg.
// return true: azonosak (nincs ellentmondás az adatai között!!!) 
// return false: különbözõek
int CSamePeople::identical( UINT i1, UINT i2 )
{
	getData( i1 );
	getData2( i2 );

	// Ha mindkét ember leszármazott, azok nem lehetnek azonosak !!!
	if( _source == L"1" && _source2 == L"1" )  return false;

	int z;

	if( vPeople.at(i1).name == L"Baloghy Antónia" )
		z = 1;

	if( (vPeople.at(i1).rowid == L"154617" && vPeople.at(i2).rowid == L"154617") || (vPeople.at(i2).rowid == L"154617" && vPeople.at(i1).rowid == L"154617")  )
		z = 1;
	
	int g;
	int	good = 0;
	
	if( _mother == L"Meiczer Éva" && _mother2 == L"Meiczer Éva" )
		z = 2;
	if( ( g = same( m_birthR, _birth, _birth2 ) ) == -1 ) return false;
	if( g == 1 ) ++good;

	if( ( g = same( m_deathR, _death, _death2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_fatherR, _father, _father2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_motherR, _mother, _mother2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_birthFR, _birthF, _birthF2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_deathFR, _deathF, _deathF2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_birthMR, _birthM, _birthM2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_deathMR, _deathM, _deathM2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	if( ( g = same( m_spousesR, _spouses, _spouses2 ) ) == -1 ) return false;
	if( g == 1 ) ++good; 

	
	if( _azonos <= good ) return true;			// legalább _azonos számú egyezés ellentmondás nélkül 
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az iBy indexû emberrel helyettesíti az iDel indexû embert
void CSamePeople::contract( UINT iBy, UINT iDel )
{
	CString rowid	= vPeople.at( iDel ).rowid;
	CString rowidBy	= vPeople.at(iBy).rowid;
	CString sex_id	= vPeople.at(iDel).sex_id;
	int source		= _wtoi( vPeople.at(iBy).source );

	int z;
	if( rowid == L"25586" )
		z = 1;

	theApp.replaceBy( rowid, rowidBy, sex_id, source );

	m_contracted = 1;
	++m_deleted;
/*
// az alábbiak fölöslegesek, mert a replaceBy a házasság rekordban megváltoztatta spouse1_id/spouse2_id-t!!
	if( sex_id == L"1" )
		m_command.Format( L"DELETE FROM marriages WHERE spouse1_id ='%s'", rowid );
	else
		m_command.Format( L"DELETE FROM marriages WHERE spouse2_id ='%s'", rowid );
	if( !theApp.execute( m_command ) ) return;
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::listPeople()
{
	std::sort( vPeople.begin(), vPeople.end(), sortByGroupStatus );
	CString married;
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		married.Empty();
		getData(i);
		if( _married ) married = L"X";

str.Format( L"\
%2d %2d %9s %2s %1s %1s %1s \
%8s \
%-30s %12s %12s \
%-30s %12s %12s \
%-30s %12s %12s \
%s",\
_group, _status, _line, _united, married, _gen, _source,\
_rowid,\
_name, _birth, _death,\
_father, _birthF, _deathF,\
_mother, _birthM, _deathM,
_spouses\
); 

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
/*
str.Format( L"\
%33s \
%-30s %12s %12s \
%-30s %12s %12s \
%-30s %12s %12s \
%s",\
L" ",\
_name, m_birthR, m_deathR,\
m_fatherR, m_birthFR, m_deathFR,\
m_motherR, m_birthMR, m_deathMR,
m_spousesR\
);
	if( m_contracted )
		fwprintf( fU, L"<span style=\"background:cyan\">%s</span>\n", str );
*/

	if( m_contracted )
		fwprintf( fU, L"\n" );
	else
		fwprintf( fD, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::resetRef()
{
	m_birthR.Empty();
	m_deathR.Empty();
	m_fatherR.Empty();
	m_motherR.Empty();
	m_birthFR.Empty();
	m_birthMR.Empty();
	m_deathFR.Empty();
	m_deathMR.Empty();
	m_spousesR.Empty();
	m_generationR.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::setRef( int i )
{
	if( m_birthR.IsEmpty() && !vPeople.at(i).birth.IsEmpty() )
		m_birthR = vPeople.at(i).birth;
	if( m_birthFR.IsEmpty() && !vPeople.at(i).birthF.IsEmpty() )
		m_birthFR = vPeople.at(i).birthF;
	if( m_birthMR.IsEmpty() && !vPeople.at(i).birthM.IsEmpty() )
		m_birthMR = vPeople.at(i).birthM;
	if( m_deathR.IsEmpty() && !vPeople.at(i).death.IsEmpty() )
		m_deathR = vPeople.at(i).death;
	if( m_deathFR.IsEmpty() && !vPeople.at(i).deathF.IsEmpty() )
		m_deathFR = vPeople.at(i).deathF;
	if( m_deathMR.IsEmpty() && !vPeople.at(i).deathM.IsEmpty() )
		m_deathMR = vPeople.at(i).deathM;
	if( m_fatherR.IsEmpty() && !vPeople.at(i).father.IsEmpty() )
		m_fatherR = vPeople.at(i).father;
	if( m_motherR.IsEmpty() && !vPeople.at(i).mother.IsEmpty() )
		m_motherR = vPeople.at(i).mother;
	if( m_spousesR.IsEmpty() && !vPeople.at(i).spouses.IsEmpty() )
		m_spousesR = vPeople.at(i).spouses;
	if( m_generationR.IsEmpty() && !vPeople.at(i).generation.IsEmpty() )
		m_generationR = vPeople.at(i).generation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::getData( UINT i )
{
	_group		= vPeople.at(i).group;
	_status		= vPeople.at(i).status;
	_line		= vPeople.at(i).line;
	_united		= vPeople.at(i).united;
	_gen		= vPeople.at(i).generation.Trim();
	_source		= vPeople.at(i).source;
	_rowid		= vPeople.at(i).rowid;
	_name		= vPeople.at(i).name;
	_birth		= vPeople.at(i).birth;
	_death		= vPeople.at(i).death;
	_father		= vPeople.at(i).father;
	_mother		= vPeople.at(i).mother;
	_married	= vPeople.at(i).married;
	_birthF		= vPeople.at(i).birthF;
	_deathF		= vPeople.at(i).deathF;
	_birthM		= vPeople.at(i).birthM;
	_deathM		= vPeople.at(i).deathM;
	_spouses	= vPeople.at(i).spouses;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::getData2( UINT i )
{
	_group2		= vPeople.at(i).group;
	_status2	= vPeople.at(i).status;
	_line2		= vPeople.at(i).line;
	_united2	= vPeople.at(i).united;
	_gen2		= vPeople.at(i).generation.Trim();
	_source2		= vPeople.at(i).source;
	_rowid2		= vPeople.at(i).rowid;
	_name2		= vPeople.at(i).name;
	_birth2		= vPeople.at(i).birth;
	_death2		= vPeople.at(i).death;
	_father2	= vPeople.at(i).father;
	_mother2	= vPeople.at(i).mother;
	_married2	= vPeople.at(i).married;
	_birthF2	= vPeople.at(i).birthF;
	_deathF2	= vPeople.at(i).deathF;
	_birthM2	= vPeople.at(i).birthM;
	_deathM2	= vPeople.at(i).deathM;
	_spouses2	= vPeople.at(i).spouses;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::openUnited()
{
	CString fileName = L"peopleUnited";
	unitedSpec = theApp.openHtmlFile( &fU, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK AZONOSAK IS, EZÉRT BEJEGYZÉSEIK ÖSSZEVONHATÓAK" );
	fwprintf( fU, m_head );
	fwprintf( fU, m_description );

	str = L"A szürke hátterû bejegyzéseket egyesítettük a zöld hátterû bejegyzéssel.\n\
Ha egy azonos nevû csoportban több különbözõ egyesítés lehetséges, akkor azok a 'g'-group oszlopba található számmal vannak meglülönböztetve.\n\n";
	fwprintf( fU, str );

	fwprintf( fU, m_columns );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::openDifferent()
{
	CString fileName = L"peopleDifferent";
	differentSpec = theApp.openHtmlFile( &fD, fileName, L"w+" );

	createHead( L"AZONOS NEVÛ EMBEREK, AKIK KÜLÖNBÖZNEK EGYMÁSTÓ" );
	fwprintf( fD, m_head );
	fwprintf( fD, m_description );
	fwprintf( fD, m_columns );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeople::createHead( CString title )
{
	m_head.Format( L"\
<HEAD>\n\
<style>\n\
</style>\n\
</HEAD>\n\
<BODY>\n\
<center>%s</center><br><br>\n\n\
<pre>\n\
%-20s %s (%s)<br>\
%-20s %s<br><br>\
%s %d %s<br><br>\
",
title,\
L"Adatbázis:",\
theApp.m_databaseSpec,\
theApp.m_user_version,\
L"Lista készült:",\
theApp.getPresentDateTime(),\
L"Az azonosság megállapításához legalább ",\
_azonos,\
L"számú egyezés volt elõírva."\
);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSamePeople::query( CString command )
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
BOOL CSamePeople::query1( CString command )
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
BOOL CSamePeople::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
