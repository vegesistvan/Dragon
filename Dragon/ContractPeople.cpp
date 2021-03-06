// ContractPeopleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "ContractedPeople.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "ProgressWnd.h"
#include "ContractPeople.h"
#include <algorithm>

// CContractPeople dialog

// lek�rdezett mez�k a people t�bl�b�l
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
// CListCtrl t�bl�zat oszlopai
enum
{
	S_CNT = 0,
	S_LOOP, 
	S_GROUP,
	S_MATCH,
	S_GROUP2,
	S_STATUS,
	S_RGBCOLOR,
	S_LINE,
	S_UNITED,
	S_GENERATION,
	S_SOURCE,
	S_ROWID,
	S_NAME,
	S_ROWIDF,
	S_FATHER,
	S_ROWIDM,	
	S_MOTHER,
 	S_ROWIDS,
	S_SPOUSES,
	S_LINEF,
	COLUMNSCOUNT,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sortBySourceX(const SAMENAMES &v1, const SAMENAMES &v2) 
{
	return( v1.source < v2.source );
}
bool sortByX(const CONTRACT &v1, const CONTRACT &v2) 
{
	return( v1.rowidBy < v2.rowidBy );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return TRUE ne cser�ljen
// return FALS cser�ljen
bool sortByGroupStatusX(const SAMENAMES &v1, const SAMENAMES &v2) 
{ 
	if( v1.group > v2.group )
		return false;
	if( v1.group == v2.group )
	{
		if( v1.status > v2.status )
			return true;
		else
			return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CContractPeople, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractPeople::CContractPeople(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractPeople::IDD, pParent)
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

	m_rgb[0] = WHITE;					// egyed�l�ll�
	m_rgb[1] = YELLOW; 
	m_rgb[2] = LIGHTGREEN; 
	m_rgb[3] = RGB( 129, 190, 250 );
	m_rgb[4] = LIGHTRED;
	m_rgb[5] = RGB( 173, 255,  47 );
	m_rgb[6] = RGB( 216, 191, 216 );
	m_rgb[7] = RGB( 255, 105, 180 );
	m_rgb[8] = RGB(  0,  255, 255);
	m_rgb[9] = RGB( 246, 252, 192 );
	m_rgb[10]= RGB( 111, 188, 243 );
	m_rgb[11]= RGB( 216, 191, 216 );
	m_rgb[12]= RGB( 198, 198, 205 );	// sz�rke, t�rlend�




	sWHITE.Format( L"%u", RGB(255,255,255) );

	m_name = L"";
//	m_name = L"Bitt� Gy�rgy";				// ha csak egy embert akarunk vizsg�lni, itt megadhatjuk a nev�t

	m_azonos	= 2;						// az azonos adatp�rok el��rt sz�ma
	nItem		= 0;
	m_loopMax   = 4;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractPeople::~CContractPeople()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, comboCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractPeople, CDialogEx)
	ON_BN_CLICKED(IDOK, &CContractPeople::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CContractPeople::OnBnClickedCancel)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractPeople::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString info;
	info = L"Azoknak a bejegyz�seknek az �sszevon�s�t lehet elv�gezni, amelyekben azon t�l, hogy a nevek azonosak, \
egyik szem�lyes adatukban sem k�l�nb�znek, s�t az al�bb felsorolt adatok k�z�l legal�bb az el��rt sz�m� adat \
mindk�t bejegyz�sben ismert �s egyforma, ami val�sz�n�s�ti, hogy a bejegyz�sek azonos emberre vonatkoznak.\
\r\n\r\n\
Sajnos ezek a krit�riumok sem garant�lj�k 100%-osan, hogy val�ban azonos szem�lyhez tartoznak a bejegyz�sek.\
\r\n\r\n\
Ha mindk�t bejegyz�s rangja 1, akkor azokat nem vonjuk �ssze, hiszen ugyanaz az ember nem fordulhat el� k�tszer \
lesz�rmazottk�nt.\
\r\n\r\n\
A megk�vetelt adatazonoss�gok default sz�ma 2, ez aj�nlott. Csak kis�rleti c�lb�l van meg annak lehet�s�ge, \
hogy ezen v�ltoztassuk.\
\r\n\r\n\
Az adat-azonoss�g sz�m�nak n�vel�se sz�k�ti az �sszevont bejegyz�sek k�r�t, de cs�kkenti a megalapozatlan �sszevon�sokat.\
";

	GetDlgItem( IDC_INFO )->SetWindowTextW( info );

	comboCtrl.AddString( L"1" );
	comboCtrl.AddString( L"2" );
	comboCtrl.AddString( L"3" );
	comboCtrl.AddString( L"4" );
	comboCtrl.SetCurSel( 1 );

	contractPeople();
	CDialogEx::OnOK();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::OnBnClickedOk()
{
//	m_azonos = comboCtrl.GetCurSel() + 1;
	CDialogEx::OnOK();
	contractPeople();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContractPeople::contractPeople()
{
	m_fileSpecTextU.Format( L"%s\\%s_UNITED%d.txt", theApp.m_databasePath, theApp.m_baseName, m_azonos );
	if( !openFileSpec( &textU, m_fileSpecTextU, L"w+" ) ) return NULL;
	
	m_fileSpecTextD.Format( L"%s\\%s_DIFFERENT%d.txt", theApp.m_databasePath, theApp.m_baseName, m_azonos );
	if( !openFileSpec( &textD, m_fileSpecTextD, L"w+" ) ) return NULL;
	
	fwprintf( textU, L"m_azonos = %d\n", m_azonos );
	fwprintf( textD, L"m_azonos = %d\n", m_azonos );

	UINT userVersion;
	CString drive;
	CString path;
	CString fname;
	CString ext;
//	TCHAR* old;
//	TCHAR* renamed;

	m_loop = 1;		// ciklus sz�ml�l�
	wndP.Create( NULL, L"" );
	wndP.GoModal();
//	tableLines.clear();
	while( m_loop <= m_loopMax )
	{
		str.Format( L"Azonos emberek bejegyz�seinek �sszevon�sa. (%d. ciklus)", m_loop );
		wndP.m_strTitle = str;

		// az aktu�li sadatb�zis f�jlok m�sol�sa *P.db f�jlba
		splitFilespec( theApp.m_databaseSpec, &drive, &path,  &fname, &ext );
		if( m_loop > 1 )fname = fname.Left( fname.GetLength() - 1);
		str.Format( L"%s:%s\\%sP%d.%s", drive, path, fname, m_azonos, ext );
		CopyFile( theApp.m_databaseSpec, str, false );
		theApp.m_databaseSpec = str;
		str.Format( L"%s:%s\\%sP%d_blob.%s", drive, path, fname, m_azonos, ext );
		CopyFile( theApp.m_blobSpec, str, false );
		theApp.openDatabase();

		userVersion = m_loop << 16;   // userVersion = loop|m_azonos;
		userVersion |= m_azonos;
		theApp.setUserVersion( userVersion );

		// az �j adatb�zis nev�nek ki�r�sa a dlg ablak c�m�ben!
		GetParent()->SendMessage(WM_MAIN_TITLE, (WPARAM)NULL, (LPARAM)NULL);

		vContract.clear();			// az �sszevont emberek 
		if( !core() ) return false;
	
		theApp.insertIntoFiles( m_fileSpecTextU, UNITED_FILE );
		theApp.insertIntoFiles( m_fileSpecTextD, DIFFERENT_FILE );

		if( !vContract.size() ) break;		// nincs �sszevont ember, v�ge a programnak
		++m_loop;
	}
	wndP.DestroyWindow();
	fclose( textU );
	fclose( textD );

#ifndef _DEBUG
	str.Format( L"(5/5) Adatb�zis t�m�r�t�se" ); 
	wndP.SetText( str );
#endif
	theApp.execute( L"VACUUM");
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Azonos nev� emebereket keres �s azok adatait beteszi a vPeople vektorba.
// Ha �sszegy�ltek az azonos nev� emberek, akkor megh�vja a processPeople met�dust,
// amely meg�llap�tja az azonoss�got vagy k�l�nb�z�s�get.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContractPeople::core()
{
	CString namePrev;
	CString name;
	CString lastName;
	CString firstName;
	bool	first = true;;
	int		pos;

#ifndef _DEBUG

	str.Format( L"(1/5) Azonos nev� emberek bejegyz�seinek �sszegy�jt�se..." );
	wndP.SetText( str );
#endif
			
	m_command.Format( L"SELECT %s FROM people ORDER BY last_name, first_name, source", p_fields );
	if( !query( m_command ) ) return false;

	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	CString nameR;
	vPeople.clear();
	vSpouses.clear();
	for( UINT i = 0; i < m_recordset->RecordsCount()-1; ++i )
	{
		lastName  = m_recordset->GetFieldString( P_LAST_NAME );
		if( lastName.IsEmpty() ) goto cont;

		firstName = m_recordset->GetFieldString( P_FIRST_NAME );
		name.Format( L"%s %s", lastName, sepFirstName( firstName ) );
		if( name.TrimRight().IsEmpty() ) goto cont;
			
		if( !m_name.IsEmpty() )
		{
			nameR = name.Left( m_name.GetLength() );	
			if( nameR != m_name ) goto cont;
		}

		if( name == namePrev )
		{
			if( first )
			{
				putPeople( name, i-1 );
				first = false;
			}
			if( !putPeople( name, i ) ) return false;;
		}
		else if( vPeople.size() )
		{
			processPeople();
			vPeople.clear();
			vSpouses.clear();
			first = true;
		}
		namePrev = name;
cont:	m_recordset->MoveNext();
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if( vPeople.size() )
		processPeople();

	theApp.execute( L"BEGIN" );
	contractFull();			// a vContracxtban �sszegy�jt�tt megtartand�-t�rlend� adatok alapj�n t�r�l
	deleteMarriages();
	theApp.execute( L"COMMIT" );
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos nev� emberek adatainak �sszegy�jt�se a vPeople vetorba
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContractPeople::putPeople( CString name, UINT i )
{
	m_recordset->MoveTo( i );

	SAMENAMES vpeople;
	SAMENAMES vspouses;
	CString rowid = m_recordset->GetFieldString( P_ROWID );
	CString spouses;
	CString nameBD;
	CString spouse_id;
	CString father_id;
	CString birth;
	CString death;
	CString wedding;
	CString lfname;

	int z;
	if( rowid == L"136437" )
		z = 1;

	vpeople.name		= name;
	vpeople.group		= 0;
	vpeople.group2		= 0;
	vpeople.status		= 0;
	vpeople.match		= 0;
	vpeople.rowid		= rowid;
	vpeople.line		= m_recordset->GetFieldString( P_LINE );
	vpeople.generation	= m_recordset->GetFieldString( P_GENERATION );
	vpeople.generation.Trim();
	vpeople.source		= m_recordset->GetFieldString( P_SOURCE );
	vpeople.united		= m_recordset->GetFieldString( P_UNITED );
	vpeople.sex_id		= m_recordset->GetFieldString( P_SEX_ID );
	vpeople.birth		= m_recordset->GetFieldString( P_BIRTH_DATE );
	vpeople.death		= m_recordset->GetFieldString( P_DEATH_DATE );

	father_id			= m_recordset->GetFieldString( P_FATHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date, lineNumber FROM people WHERE rowid = '%s'", father_id );
	if( !query1( m_command ) ) return false;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.father	= name.TrimRight();

	if( father_id == L"0" ) father_id.Empty();
	vpeople.rowidF	= father_id;
	vpeople.birthF	= m_recordset1->GetFieldString( 2 );
	vpeople.deathF	= m_recordset1->GetFieldString( 3 );
	vpeople.lineF	= m_recordset1->GetFieldString( 4 );

	CString mother_id = m_recordset->GetFieldString( P_MOTHER_ID );
	m_command.Format( L"SELECT last_name, first_name, birth_date, death_date, father_id FROM people WHERE rowid = '%s'", mother_id );
	if( !query1( m_command ) ) return false;

	name.Empty();
	if( m_recordset1->RecordsCount() )
		name.Format( L"%s %s", m_recordset1->GetFieldString( 0 ), sepFirstName( m_recordset1->GetFieldString( 1 ) ) );
	vpeople.mother	= name.TrimRight();
	if( mother_id == L"0" ) mother_id.Empty();
	vpeople.rowidM	= mother_id;
	vpeople.birthM	= m_recordset1->GetFieldString( 2 );
	vpeople.deathM	= m_recordset1->GetFieldString( 3 );
	
	if( vpeople.sex_id == L"1" )
		m_command.Format( L"SELECT spouse2_id, date FROM marriages WHERE spouse1_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT spouse1_id, date FROM marriages WHERE spouse2_id = '%s'", rowid );
	if( !query1( m_command ) ) return false;

	for( UINT i = 0; i < m_recordset1->RecordsCount(); ++i )
	{
		wedding		= m_recordset1->GetFieldString( 1 );
		spouse_id	= m_recordset1->GetFieldString( 0 );
		m_command.Format( L"SELECT last_name, first_name, birth_date, death_date FROM people WHERE rowid = '%s'", spouse_id );
		if( !query2( m_command ) ) return false;
		birth = m_recordset2->GetFieldString( 2 );
		death = m_recordset2->GetFieldString( 3 );
		lfname.Format( L"%s %s", m_recordset2->GetFieldString(0),	sepFirstName( m_recordset2->GetFieldString(1) ) );
		nameBD = getNameBD( lfname, birth, death, wedding );

		spouses += nameBD.Trim();
		spouses += L", ";

		vspouses.rowid	= rowid;		// a f�rj azonos�t�ja!!!
		vspouses.name	= lfname;
		vspouses.nameBD	= nameBD;

		vspouses.wedding	= wedding;
		vspouses.birth		= m_recordset2->GetFieldString(2);
		vspouses.death		= m_recordset2->GetFieldString(3);
		vSpouses.push_back( vspouses );
		m_recordset1->MoveNext();
	}
	if( m_recordset1->RecordsCount() )
		spouses = spouses.Left( spouses.GetLength() - 2); 
	vpeople.spouses = spouses;
	if( spouse_id == L"0" ) spouse_id.Empty();
	vpeople.rowidS	= spouse_id;

	vPeople.push_back( vpeople );
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A vPeople vetorban vannak az azonos nev� emberek adataikkal egy�tt.
// A bejegyz�seket csoportokba osztja, olyk�ppen, hogy egy csoportba teszi az azonos emberhez tartoz� bejegyz�seket.
// A csoportokat megsz�mozza a "group" v�ltoz�ban. Ha egy ember egy csoporthoz sem tartozik, akkor a 'group' �rt�ke 0.
// Egy csoportba tartoz� bejegyz�sek k�z�l a legmagasabb priorit�s�t tartja meg, a t�bbit t�rli.
// A megtartand� bejegyz�st 'csoprtf�"-nek nevezz�k.
// Minden bejegyz�snek van egy 'status' szava, amely -1, ha t�rlend� �s 1 ha megtartand� bejegyz�s.
// Term�szetesen a t�rlend� bejegyz�sre hivatkoz� 'father_id, 'mother_id' mez�ket is fel�l�rja a megtartott bejegyz�ssel.
// Egy bejejegyz�s 'match" szav�ban lesz benne, hogy h�ny adata egyezik a megtartand� bejegyz�ssel.
//
// A probl�ma lehet, ha egy bejegyz�s egy-k�t adat alapj�n egyezik a csoportf� adataival, egy m�sik pedig m�s adatok alapj�n
// egyezik a csoportf�vel, de egym�ssal ellentmond�sban vannak. Ez adathib�k, adat hinyoss�gok miatt is el�fordulhat. 
// Ez�rt egy csoport egyez� adatait gy�jtj�k egy SAMENAMES r struktur�ban, �s az �jabb megtartand� bejegyz�s adatit 
// ebben a struktur�ban gy�jt�tt adatokkal hasonl�tjuk �ssze. 
// Ezzel biztos�thatjuk, hogy a csoportban nem lesz ellentmond�s, de nincs r� garancia, hogy val�ban a helyes �rt�kek \\
// gy�lnek az 'r' struktur�ban. 
// M�sik t�pus� �sszevon�si hiba, ha azonos emeberek bejegyz�seit nem vonjuk �ssze, amit szint�n adathiba vagy adathi�ny
// okozhat.
// A hib�s �sszevon�sokat az �sszevon�st k�vet� ellen�rz�si funkci�k valamelyik�val felt�rhatjuk, �s ha ennek 
// adathiba volt az oka, azt jav�thatjuk. 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::processPeople()
{
	UINT group = 1;
	UINT db = 0;
	int i;
	int z;

	int mi2;
	int g;
	CString gen1;
	CString gen2;
	int source1;
	int source2;
	CString line1;
	CString line2;
	CString spouse1;
	CString spouse2;
	CString rowid1;
	CString rowid2;

	m_contracted = false;
	resetRef();
	std::sort( vPeople.begin(), vPeople.end(), sortBySourceX );

/*
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		str.Format( L"%2d %8s %s %2s %-30s %-30s", i, vPeople.at(i).rowid, vPeople.at(i).generation, vPeople.at(i).source, vPeople.at(i).name, vPeople.at(i).spouses );
		fwprintf( fU, L"%s\n", str );
	}
	fwprintf( fU, L"\n\n" );
*/
	for( UINT i1 = 0; i1 < vPeople.size(); ++i1 )
	{
		rowid1 = vPeople.at(i1).rowid;
		if( rowid1 == L"87409" )
			z = 1;
		if( db )
		{
//			printRef( group ); 
 			resetRef();  // ha �j csoport kezd�dik, akkor �res rf-ek kellenek 
			++group;
		}
		db = 0;

		if( vPeople.at(i1).group == 0 )		// az i1. h�zasp�rt m�g nem rendelt�k hozz� egyik csoporthoz sem
		{
			for( UINT i2 = 0; i2 < vPeople.size(); ++i2 )
			{
				rowid2 = vPeople.at(i2).rowid;
				if( i1 != i2 )
				{
					gen1 = vPeople.at( i1 ).generation;
					gen2 = vPeople.at( i2 ).generation;

					if( !gen1.IsEmpty() && !gen2.IsEmpty() ) continue;
 					if( identical( i1, i2 ) )
					{  
						if( group == 2 )
							z = 2;

						line1 = vPeople.at(i1).line;
						line2 = vPeople.at(i2).line;
						
						source1 = _wtoi( vPeople.at(i1).source ); 
						source2 = _wtoi( vPeople.at(i2).source ); 

						if( (line1 == line2) && (source1 == source2) || gen1.IsEmpty() || gen2.IsEmpty() )
						{

							
							// Ha a bejegyz�s m�r tartozott valakihez, de az egyez�s most jobb, akkor elveszi t�le!!

							mi2 = vPeople.at(i2).match;
							g = vPeople.at(i2).group;

							if( vPeople.at(i2).group )
							{
								if( vPeople.at(i2).match < m_match )
								{
									vPeople.at(i2).group2	= vPeople.at(i2).group;		// �tveszi a group-b�l
								}
								else
								{
									continue;
								}
							}
							vPeople.at( i1 ).group = group;
							vPeople.at( i2 ).group = group;
/*
// i1 marad, i2 t�rlve lesz
// itt vizsgl�dni kellene, hogy melyiket cser�lj�k le!! Ha n�t vonunk �ssze, akkor nem j� a 1 hierarchi�t megtartani, mert 
// annak nincsenek gyerekei!!! Alapesetben H(i1) <= H(i2)
// Ha i1 n�, akko meg kell n�zni H(i1)-et �s H(i2)-t �s ha h(I1)==1 �s H(i2) == 2 akkor i1 �s i2 felcser�lend�, azaz H(i2)-t tarsuk meg!!!
							if( vPeople.at(i1).sex_id == WOMAN )
							{
								if( vPeople.at(i1).source == 1 && vPeople.at(i2).source == 2 )
								{
									i  = i1;
									i1 = i2;
									i2 = i;
								}
							}
*/
if( (i1 == 95921 && i2 == 141743 ) || (i1 == 141743 && i2 == 95921 ) )
   z = 1;

							vPeople.at( i1 ).status = 1;	// megtartani	
							vPeople.at( i2 ).status = -1;   // t�r�lni
							vPeople.at( i2 ).match	= m_match;
							pushVContract( i1, i2 );

							setRef( i1 );	// ha i1 �s i2 azonos, akkor a ref-be m�soljuk �t a megadott adatokat
							setRef( i2 );


							++db;
						}
					}
				}
			}
		}
	}
	group = checkEmptyCouples( group );
	checkEmptyCouples2( group );
	listPeople();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellen�rzi a status == 1, source==3 t�pus� bejegyz�seket, amelyekben semmi m�s adat nincs, hogy van-e egyetlen 
// azonos nev� status == 1, source == 1 bejegyz�s. Ha van, akkor ezeket �sszevonja.
int CContractPeople::checkEmptyCouples( int group )
{
	UINT i;
	UINT j;
	UINT k;
	int cnt;
	SAMENAMES a;
	SAMENAMES b;
	CString spouses;


	for( i = 0; i < vPeople.size(); ++i )
	{
		if( vPeople.at(i).spouses.IsEmpty() ) continue;

		a = vPeople.at(i);
		if( a.source == L"1" && a.status != -1 )  // NEM T�RLEND�, DE H�T EGYIK SEM AZ??
		{
			for( j = 0; j < vPeople.size(); ++j )
			{
				if( vPeople.at(j).spouses.IsEmpty() ) continue;
				b = vPeople.at(j);
				// m�g sehova sme tartoz� 3. �s 4. rang� �res bejegyz�sek ( merthogy ezeket m�rt �tn�zte �s nem volt tal�lat )
//				if( ( b.source == L"3" || b.source == L"4" ) && b.status == 0 && 
				if( b.source != L"1" && b.status == 0 && \
							b.birth.IsEmpty() && b.death.IsEmpty() && \
							b.father.IsEmpty() && b.birthF.IsEmpty() && b.deathF.IsEmpty() && \
							b.mother.IsEmpty() && b.birthM.IsEmpty() && b.deathM.IsEmpty() )
				{
					spouses = getTwoWords( b.spouses );
					// az 1. rang� bejegyz�s h�zast�rsai k�z�tt megtal�lhat� a 3./4. rang� bejegyz�se h�zast�rsa
					if( (a.spouses.Find( spouses )) != -1  )
					{
						if( ( cnt = howMany( b.spouses ) ) > 1 )	// ha azono sn�ven t�bb 1. rang� bejegyz�s van, akkor nem egyes�t
							break;							// tov�bb, mert nem lehet tudni, hogy melyikhez tartozik
						a.status = 1;						// megtartand� meger�s�t�se
						if( a.group == 0 )					// ha m�g nem tartozott csoportba, akkor hozz�rendeli a group-hoz
						{
							a.group = group;
							vPeople.at(i).status = 1;		// az 1. rang� bejegyz�s is csoport lesz
							vPeople.at(i).group = a.group;
							++vPeople.at(i).match;
							++group;
						}

						vPeople.at(j).status	= -1;		// a 3./4. rang� bejegyz�s t�rlend�
						vPeople.at(j).group		= a.group;
						vPeople.at(j).match		= 1;
						pushVContract( i, j );
					}
				}
			}
		}
	}
	return group;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellen�rzi a status == 1, source==3 t�pus� bejegyz�seket, amelyekben semmi m�s adat nincs, hogy van-e egyetlen 
int CContractPeople::checkEmptyCouples2( int group )
{
	UINT i;
	UINT j;
	UINT k;
	int cnt;
	SAMENAMES a;
	SAMENAMES b;
	CString spouses;


	for( i = 0; i < vPeople.size(); ++i )
	{
		if( vPeople.at(i).spouses.IsEmpty() ) continue;

		a = vPeople.at(i);
		if( a.source == L"2" && a.status != -1 )  // NEM T�RLEND�, DE H�T EGYIK SEM AZ??
		{
			for( j = 0; j < vPeople.size(); ++j )
			{
				if( vPeople.at(j).spouses.IsEmpty() ) continue;
				b = vPeople.at(j);
				// m�g sehova sme tartoz� 3. �s 4. rang� �res bejegyz�sek ( merthogy ezeket m�rt �tn�zte �s nem volt tal�lat )
//				if( ( b.source == L"3" || b.source == L"4" ) && b.status == 0 && 
				if( b.source == L"3" && b.status == 0 && \
							b.birth.IsEmpty() && b.death.IsEmpty() && \
							b.father.IsEmpty() && b.birthF.IsEmpty() && b.deathF.IsEmpty() && \
							b.mother.IsEmpty() && b.birthM.IsEmpty() && b.deathM.IsEmpty() )
				{
					spouses = getTwoWords( b.spouses );
					// az 1. rang� bejegyz�s h�zast�rsai k�z�tt megtal�lhat� a 3./4. rang� bejegyz�se h�zast�rsa
					if( (a.spouses.Find( spouses )) != -1  )
					{
//						if( ( cnt = howMany( b.spouses ) ) > 1 )	// ha azono sn�ven t�bb 1. rang� bejegyz�s van, akkor nem egyes�t
//							break;							// tov�bb, mert nem lehet tudni, hogy melyikhez tartozik
						a.status = 1;						// megtartand� meger�s�t�se
						if( a.group == 0 )					// ha m�g nem tartozott csoportba, akkor hozz�rendeli a group-hoz
						{
							a.group = group;
							vPeople.at(i).status = 1;		// a 2. rang� bejegyz�s is csoport lesz
							vPeople.at(i).group = a.group;
							++vPeople.at(i).match;
							++group;
						}

						vPeople.at(j).status	= -1;		// a 3./4. rang� bejegyz�s t�rlend�
						vPeople.at(j).group		= a.group;
						vPeople.at(j).match		= 1;
						pushVContract( i, j );
					}
				}
			}
		}
	}
	return group;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CContractPeople::howMany( CString spouse )
{
	UINT cnt = 0;
	CString spouses;
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		spouses = vPeople.at(i).spouses;
		if( vPeople.at(i).source == L"1" &&  (spouses.Find( spouse ) ) != -1 )
			++cnt;
	}
	return cnt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az i1 �s i2 index� bejegyz�seket vizsg�lja, amelyekben a nevek azonosak.
// Tekintehet� azonos ember bejegyz�seinek?
// A t�bbi atributum is ellentmond�s mentes?
// return true: azonosak (nincs ellentmond�s az adatai k�z�tt!!!) 
// return false: k�l�nb�z�ek
int CContractPeople::identical( UINT i1, UINT i2 )
{
	SAMENAMES	a = vPeople.at(i1);
	SAMENAMES	b = vPeople.at(i2);
	int			g;
	int			mF = 0;
	int			mM = 0;

	if( a.source == L"1" && b.source == L"1" )  return false;

	CString rowid1 = a.rowid;
	CString rowid2 = b.rowid;


	if( rowid1 == L"275899" && rowid2 == L"280579" )
		g = 1;
	m_match = 0;
	if( ( g = same( r.birth, a.birth, b.birth ) ) == -1 ) return false;
	if( g == 1 ) ++m_match;

	if( ( g = same( r.death, a.death, b.death ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 

	if( ( mF = same( r.father, a.father, b.father ) ) == -1 ) return false;		// egyed�l az apa nev�nek azonoss�ga nem lesz elegend�!!!
	if( mF == 1 )
	{
		++m_match;   
		if( ( g = same( r.birthF, a.birthF, b.birthF ) ) == -1 ) return false;
		if( g == 1 ) ++m_match; 

		if( ( g = same( r.deathF, a.deathF, b.deathF ) ) == -1 ) return false;
		if( g == 1 ) ++m_match; 
	}

	if( ( mM = same( r.mother, a.mother, b.mother ) ) == -1 ) return false;
	if( mM == 1 )
	{
		++m_match; 
		if( ( g = same( r.birthM, a.birthM, b.birthM ) ) == -1 ) return false;
		if( g == 1 ) ++m_match; 

		if( ( g = same( r.deathM, a.deathM, b.deathM ) ) == -1 ) return false;
		if( g == 1 ) ++m_match; 
	}
	
	if( ( g = sameSpouses( a.source, a.rowid, b.rowid ) ) == -1 ) return false;
	if( g == 1 ) ++m_match; 


	if( m_azonos <= m_match ) return true;			// legal�bb _azonos sz�m� egyez�s ellentmond�s n�lk�l 

	return false;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos nev� rowid1 �s rowid 2 emberk h�zast�rsai a vSpouses vektorban vannak, amely az �sszes azonos nev�
// ember �sszes h�zast�rs�t tartalmazza. A vSpouse.rowid jelzi, hogy melyik f�rjhez tatozik egy feles�g.
// Ha a rowid1 �s rowid2 h�zast�rsai k�z�tt van azonos nev�, akkor ellen�rzi azok sz�let�si �s hal�loz�si d�tum�nak 
// egyez�s�t is, ha meg vannak adva.
// a h�zast�rsak a f�rjek rowid-ja szerint vannak rendezve

// return -1		a f�rjek egyez�s�re kiz�r� ok van:
//					mindkett�nek van h�zast�rsa, de nincs k�z�tt�k azonos nev�, vagy van, de az azonos nev�ek 
//					sz�let�si vagy hal�loz�si d�tumai k�z�tt ellentmond�s van.

// return 1			a f�rjek egyez�s�t meger�s�ti:
//					vannak azonos nev� h�zat�rsak, akik sz�let�si �s hal�loz�si d�tuma k�z�tt nincs ellentmond�s

// return 0			a f�rjek egyez�s�re nincs kiz�r� ok:
//					egyiknek vagy mindkett�nek sincs h�zast�rsa


int CContractPeople::sameSpouses( CString source, CString rowid1, CString rowid2 )
{
	int i;
	int j;
	int z;

	CString name1;
	CString name2;

	CString wedding1;
	CString wedding2;

	CString birth1;
	CString birth2;

	CString death1;
	CString death2;

	CString dummy;

	int retB;
	int retD;
	int retW;
	int cnt1 = 0;
	int cnt2 = 0;
	m_spouseName = false;
	for( i = 0; i < vSpouses.size(); ++i )
	{
		if( vSpouses.at(i).rowid == rowid1 )
		{
			++cnt1;
			for( j = 0; j < vSpouses.size(); ++j )
			{
				if( vSpouses.at(j).rowid == rowid2 )
				{
					++cnt2;
					name1 = vSpouses.at(i).name;
					if( name1 == L"Farkas Erzs�bet" )
						z = 1;
					name2 = vSpouses.at(j).name;
					if( name1 == name2 )
					{
						birth1	= vSpouses.at(i).birth;
						birth2	= vSpouses.at(j).birth;
						if( ( retB = same( dummy, birth1, birth2 ) ) == -1 ) return -1; //continue;  // ellentmond�s
						if( retB ) ++m_match;
						
						death1	= vSpouses.at(i).death;
						death2	= vSpouses.at(j).death;
						if( ( retD = same( dummy, death1, death2 ) ) == -1 ) return -1; //continue;	// ellentmond�s
						if( retD ) ++m_match;
// ezt ki lehetne hagyni, �s akkor az 1. rend� bejegyz�s esk�v�i d�tuma maradna �rv�nyes
						wedding1 = vSpouses.at(i).wedding;
						wedding2 = vSpouses.at(j).wedding;
						if( ( retW = same( dummy, wedding1, wedding2 ) ) == -1 ) return -1;  //continue;  // ellentmond�s
						if( retW ) ++m_match;

						return 1;
					}
				}
			}
		}
	}
	if( !cnt1 || !cnt2 || (j == vSpouses.size()) ) return 0;			// egyik�knek vagy mindkett�j�knek nincs h�zast�rsa vagy a n�v azonsos, de nincs meger�s�t�s
	return -1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A vContract-ban gy�jti a t�rlend�, megartanad� azonos�t�kat
void CContractPeople::pushVContract( UINT iBy, UINT iDel )
{
	CString rowid	= vPeople.at( iDel ).rowid;
	CString rowidBy	= vPeople.at(iBy).rowid;
	CString sex_id	= vPeople.at(iDel).sex_id;
	int		sourceBy	= _wtoi( vPeople.at(iBy).source );
	int		sourceDel	= _wtoi( vPeople.at(iDel).source );
	CONTRACT vcontract;



	vcontract.rowid		= rowid;
	vcontract.rowidBy	= rowidBy;
	vcontract.sex_id	= sex_id;
	vcontract.sourceBy	= sourceBy;
	vcontract.sourceDel	= sourceDel;
	vContract.push_back( vcontract );

	m_contracted = true;		// csak a csoportra jelzi, hogy volt �sszevon�s, az annak megfelle� f�jlba kell list�zni
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::contractFull()
{
	CString rowid;
	CString rowidBy;
	CString sex_id;
	int		sourceBy;
	int		sourceDel;


	int united;	
	int spouse;
	int spouseparent;
	int spousespouse;


#ifndef _DEBUG
	str.Format( L"(2/5) T�rlend�-megtartand� azonos�t�k korrig�l�sa..." );
	wndP.SetText( str );
#endif
	wndP.SetRange( 0, vContract.size() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );


	// A t�rlend� rowid-ik l�tez�s�t vizsg�lja a megtartand� rowid-k k�z�tt-
	// Ha tal�l ilyet, akkor azt a megtartand� rowid-t lecser�li az �t helyettes�t� rowid-val.
	std::sort( vContract.begin(), vContract.end(), sortByX );
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
	str.Format( L"(3/5) Emberek f�l�sleges bejegyz�seinek t�rl�se..." ); 
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
		sourceDel	= vContract.at(i).sourceDel;

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



		
// az egyes�t�sek sz�m�t 1-el n�veli
	m_command.Format( L"SELECT united, spouse, spouseparent, spousespouse FROM people WHERE rowid='%s'", rowidBy );
	if( !query1( m_command ) ) return;
	united			= _wtoi( m_recordset1->GetFieldString( 0 ) );
	spouse			= _wtoi( m_recordset1->GetFieldString( 1 ) );
	spouseparent	= _wtoi( m_recordset1->GetFieldString( 2 ) );
	spousespouse	= _wtoi( m_recordset1->GetFieldString( 3 ) );

	
	++united;

	switch( sourceDel )
	{
	case 2:
		++spouse;
		break;
	case 3:
		++spouseparent;
		break;
	case 4:
		++spousespouse;
		break;
	}

	m_command.Format( L"UPDATE people SET united='%d', spouse='%d', spouseparent='%d', spousespouse='%d' WHERE rowid ='%s'",united, spouse, spouseparent, spousespouse, rowidBy );
	if( !theApp.execute( m_command ) ) return;

	
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az azonos (==rowid) h�zasp�rokat tartalmaz� h�zass�gokat t�rli.
// Nem a nevek, hanem a rowid-k azonoss�g�t vizsg�lja!!
void CContractPeople::deleteMarriages()
{

#ifndef _DEBUG
	str.Format( L"(4/5 H�zass�gok ellen�rz�se �s a f�l�slegesek t�rl�se..." );
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
void CContractPeople::listPeople()
{

	int status;
	int	group;
	int colorIndex;
	int	rgbColor;

	CString nameBD;
	CString nameBDF;
	CString nameBDM;
	CString nameBDS;

	int index;
	int z;
	SAMENAMES x;
	CString ident;
	std::sort( vPeople.begin(), vPeople.end(), sortByGroupStatusX );

// list into text file
	for( UINT i = 0; i < vPeople.size(); ++i )
	{
		x = vPeople.at(i);
		
		switch( x.status )
		{
		case 0:
			colorIndex = 0;					// egyedi, nem csin�lunk vele semmit - feh�r
			break;
		case 1:
			colorIndex = x.group % 12;		// megtartanad� -v�ltakoz� sz�nes
			break;
		case -1:
			colorIndex = 12;					// t�rlend� - sz�rke
			break;
		}

		rgbColor = m_rgb[colorIndex];

		nameBD = getNameBD( x.name, x.birth, x.death, L"" );
		nameBDF	= getNameBD( x.father, x.birthF, x.deathF, L"" );
		nameBDM	= getNameBD( x.mother, x.birthM, x.deathM, L"" );
		str.Format( L"\
%d\t%d\t%d\t%d\t%d\t%d\t%d\t\
%s\t%s\t%s\t%s\t\
%s\t%s\t\
%s\t%s\t\
%s\t%s\t\
%s\t%s\t%s\t\n",\
colorIndex, m_loop, x.group, x.match, x.group2, x.status, rgbColor,\
x.line, x.united, x.generation, x.source,\
x.rowid,nameBD,\
x.rowidF,nameBDF,\
x.rowidM,nameBDM,\
x.rowidS, x.spouses, x.lineF\
);
		if( m_contracted )
			fwprintf( textU, str );
		else
			fwprintf( textD, str );
	}
	if( m_contracted )
		emptyLine( textU, m_loop );
	else
		emptyLine( textD, m_loop );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::emptyLine( FILE* fl, int loop )
{
	UINT i;
	fwprintf( fl, L"\t%d\t", loop );
	for( i = 2; i < S_RGBCOLOR; i++ )	// egy �res sor az azonos nev� emberek ut�n
		fwprintf( fl, L"\t" );
	fwprintf( fl, sWHITE );						// RGB color white
	for( ; i < COLUMNSCOUNT; i++ )  // eg y�res sor az azonos nev� emberek ut�n
		fwprintf( fl, L"\t" );
	fwprintf( fl, L"\n" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CContractPeople::printRef( int group )
{
		str.Format( L"\
%2d %2d %2s %9s %2s %1s %1s \
%-39s %12s %12s \
%8s %-30s %12s %12s \
%8s %-30s %12s %12s \
%8s %s",\
m_loop, group, L" ", L" ", L" ", r.generation, L" ",
L" ", r.birth, r.death,
L" ", r.father, r.birthF, r.deathF,
L" ", r.mother, r.birthM, r.deathM,
L" ", r.spouses
);
	fwprintf( fU, L"<span style=\"background:DeepPink\">%s</span>\n", str );

	fwprintf( textU, str );
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractPeople::resetRef()
{
	r.group = 0;
	r.birth.Empty();
	r.death.Empty();
	r.father.Empty();
	r.mother.Empty();
	r.birthF.Empty();
	r.birthM.Empty();
	r.deathF.Empty();
	r.deathM.Empty();
	r.spouses.Empty();
	r.generation.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ha vPeople.at(i) azonos egy m�sik emberrel, akkor a kett�b�l �sszeszedj�k a megadott �rt�keket
void CContractPeople::setRef( int i )
{
	SAMENAMES a;
	a = vPeople.at(i);

	if( r.birth.IsEmpty() && !a.birth.IsEmpty() )				
		r.birth = a.birth;
	if( r.birthF.IsEmpty() && !a.birthF.IsEmpty() )				
		r.birthF = a.birthF;
	if( r.birthM.IsEmpty() && !a.birthM.IsEmpty() )				
		r.birthM = a.birthM;
	if( r.death.IsEmpty() && !a.death.IsEmpty() )				
		r.death = a.death;
	if( r.deathF.IsEmpty() && !a.deathF.IsEmpty() )				
		r.deathF = a.deathF;
	if( r.deathM.IsEmpty() && !a.deathM.IsEmpty() )				
		r.deathM = a.deathM;
	if( r.father.IsEmpty() && !a.father.IsEmpty() )				
		r.father = a.father;
	if( r.mother.IsEmpty() && !a.mother.IsEmpty() )				
		r.mother = a.mother;
	if( r.spouses.IsEmpty() && !a.spouses.IsEmpty() )			
		r.spouses = a.spouses;

	if( !r.spouses.IsEmpty() && r.spouses.GetLength() < a.spouses.GetLength() ) 
		r.spouses = a.spouses;  // ha t�bb h�zast�rs van megadava



	if( r.generation.IsEmpty() && !a.generation.IsEmpty() )		
		r.generation = a.generation;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractPeople::query( CString command )
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
BOOL CContractPeople::query1( CString command )
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
BOOL CContractPeople::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}


