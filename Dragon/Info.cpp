
#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
#include "afxdialogex.h"

void CFaDlg::OnInfo()
{
	CString info;
	info = L"\r\n\
A 'fa' egy családfa program, amaly az emberek adatait és kapcsolatait sqlite adatbázisban tárolja. \
Az adatokból leszármazotti listákat készíthetünk ga.html (Gothai Almanach formátum Szluha-Késmárky \
specifikációja szerint ) formátumban.\r\n\r\n\
Az ablak legfelsõ sorában mutatja a program a megnyitott adatbázis fájl specifikációját valamint zárójelben az \
adatbázis típusát (lásd alább).\r\n\
A program az utoljára használt adatabázissal indul. Ha más adatbázis akarunk, akkor a 'Adatbázis->Megnyitás' funkcióval \
nyithatunk meg más vagy akár új adatbázist is.\r\n\
Az 'Információk->Az adatbázisról' funkcióval a megnyitott adatbázisról kaphatunk további információkat.\r\n\r\n\
Az adatokat bevihetjük:\r\n\r\n\
1. billentyûzetrõl\r\n\
2. ga.html fájlból\r\n\
3. GEDCOM fájlból\r\n\r\n\
A fenti adatbeviteli lehetõségeknek megfelelõen az adatbázis 3 típusáról beszélünk: MANUAL, GAHTML, és GEDCOM \
típusúakról. Ezeken kívül van még az EMPTY típus, amely az üres adatbázist jelenti. Az adatbázis típusok megkülönböztetését \
az indokolja, hogy más kisérõ információk vannak nyilvántartva a különbözõ esetekben. \r\n\
Ha ga.html fájlból vittük be az adatokat, akkor nyilván van tartva az adatok elhelyezkedése a \
ga.html fájlban, hogy egy adat melyik fájlból való, hanyadik sorban, melyik táblában volt és melyik családhoz tartozik. \
GEDCOM fájlból származó adatokat csak a bementi fájllal és táblával kapcsoljuk össze. Kézi adatbevitelnél semmilyen \
azonosítót nem tudunk az adatokhoz rendelni.\r\n\r\n\
A program nyilvántartja az adatbázis típusát, és ettõl föggõen különbözõ funkciókat ajánl fel az adatok kezelésére, \
a program menüje mindig az adott típusú adatbázisnál alkalmazható funkciókat tartalmazza.\r\n\r\n\
Az 'Bemenet' menüpont alatt az adatbázis típusától függõ adatbeviteli lehetõségeket kínál fel.\r\n\r\n\
A 'Táblák' menüpont alatt az adatbázis típusától függõen az alábbi táblázatok jeleníthetõek meg:\r\n\r\n\
GAHTML:\r\n\
1. Emberek\r\n\
2. Házasságok\r\n\
3. Bemeneti fájlok\r\n\
4. Bemeneti táblák\r\n\r\n\
GEDCOM:\r\n\
1. Emberek\r\n\
2. Házasságok\r\n\
3. Bemeneti fájlok\r\n\
4. Bemeneti táblák\r\n\r\n\
MANUAL:\r\n\
1. Emberek\r\n\
2. Házasságok\r\n\r\n\
Az 'Emberek' táblázat üresen indul, mert ha sokan vannak az adatbázisban, akkor elég sokáig tartana a táblázat felépítése. \
A táblázat megnyitása után a 'Szûrés' menüpont alatt válaszhatjuk ki, hogy milyen embereket akarunk listázni a táblázatban. \r\n\
A táblázat bejegyzéseit szerkeszthetjük is: a kurzorral a kérdéses sorra állva a jobb egérgomb kattintásával legördülõ \
menübõl választhatunk az Update, Insert és Delete funkciók közül. A bal egérgomb kétszeri kattintásával vagy egyszerûen az Enter billentyû \
leütésével is megjelenik az Update funkció, amikor is a kérdéses sor adatait látjuk és szerkeszthetjük egy táblázatban.\r\n\r\n\
A 'Házasságok' és az esetlegesen további táblázatok nem szerkeszthetõek.\r\n\r\n\
Az 'Emberek' táblázatban a kijelölt emberek leszármazottait a 'Leszármazottak' menüpontból kérhetjük. GAHTML típusú adatbázis \
esetén elágazásokkal vagy elágazások nélkül is kérhetjük a html listát.\
A 'Táblák->Bemeneti táblázatok' táblázatában a kijelölt táblák leszármazotti listáját kérhatjük elágazásokkal vagy anélkül.\r\n\r\n\
Az 'Adatbázis ellenõrzése' menüpont alatt különbözõ ellenõrzéseket végezhetünk az adtabázisban tárolt adatokon.\r\n\
A 'Dátum problémák' az egyedi emberek valamint a kapcsolatban lévõ emberek ( szülõk, gyerekek, házastársa ) születési, \
halálozási, házasságkötési dátumai közötti ellentmondásokat, extrém eltéréseit listázza.\
Az 'Azonos emberek'\
";

	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );
//	GetDlgItem( IDC_BUTTON )->SetFocus();

}// Info.cpp : implementation file
//

#include "stdafx.h"
#include "Info.h"
#include "afxdialogex.h"


// CInfo dialog

IMPLEMENT_DYNAMIC(CInfo, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInfo::CInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfo::IDD, pParent)
	, m_message(_T(""))
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInfo::~CInfo()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_message);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CInfo, CDialogEx)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW( m_caption );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_message ); 
	return TRUE;
}
