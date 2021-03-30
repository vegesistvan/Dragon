// SamePeopleInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "ContractInfo.h"
#include "afxdialogex.h"


// CContractInfo dialog

IMPLEMENT_DYNAMIC(CContractInfo, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractInfo::CContractInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CContractInfo::IDD, pParent)
	, m_info(_T(""))
{

	m_title = L"Azonos nevû emberek bejegyzéseinek vizsgálata és összevonása";
	m_info = L"\
Az alábbi adatokat vizsgáljuk az azonos nevû emberek azonosságának eldöntéséhez:\r\n\
\r\n\
 1. születési dátuma\r\n\
 2. halálozási dátuma\r\n\
 3. apa neve\r\n\
 4. apa születési dátuma\r\n\
 5. apa halálozási dátuma\r\n\
 6. anya neve\r\n\
 7. anya születési dátuma\r\n\
 8. anya halálozási dátuma\r\n\
 9. házastárs neve\r\n\
10. házastárs születési dátuma\r\n\
11. házastárs halálozási dátuma\r\n\
12. esküvõ dátuma\r\n\
\r\n\
Ha két bejegyzés fenti adatokpárjai között akár egy ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor 2 adatpár létezését és egyezéseét követeljük meg az azonosság megállapításához.\r\n\
Leszármazottakat akkor sem egyesítünk, ha mindenben megfelelel az összevonási kritériumoknak, mert leszármazottként \n\
csak egyszer szerepelhet egy ember.\r\n\
Több házastárs esetén az azonosításhoz elegendõ, ha egy azonos nevû házastárs szerepel mindkét bejegyzésben.\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz.\r\n\
\r\n\
Ha van olyan 2., 3. vagy 4. rendû bejegyzés, amelyben csak a név van megadva, és az megegyezik egy 1. rendû bejegyzés \
nevével valamint házastársaik neve is azonos, továbbá az ilyen 1. rendû bejegyzésbõl csak egy létezik, akkor azokat a \
bejegyzéseket összevonjuk, annak ellenére, hogy nincs legalább 2 azonos adata. Hiszen nem is lehet, mert a néven kívül nincs más adat megadva.\r\n\
\r\n\
Az adathibák nyilván egyesítési hibákhoz vezetnek, valójában különbözõ emberekhez tartozó bejegyzésket tévesen \
egyesítünk, vagy azonos emberek bejegyzéseinek összevonása elmarad. Ezért az egyesítés elõtt minden hibát \
ki kell javítani!.\r\n\
A hibákra az \"Ellenõrzése egyesítés elõtt\" funkciók világítanak rá. Az egyesítés során az azonosítás erdményét tartalmazó \
táblázat, fájlok is felfedhetnek hibákat.\r\n\
\r\n\
Az oszlopok jelentése:\r\n\
\r\n\
line\ta bejegyzés sorszáma a GA html fájlban.\r\n\
U\tunited, az ember u számú bejegyzés összevonása.\r\n\
G\tgeneráció, az ember generációs jele a GA fájlban.\r\n\
S\taz ember elõfordulása a GA.html fájlban ( 1-2-3-4 )\r\n\
\r\n\
rowid\ta bejegyzés azonosítója\r\n\
név\taz ember neve (*születés +halálozás)\r\n\
rowid\taz apa bejegyzésének azonosítója\r\n\
apa neve (*születés +halálozás)\r\n\
rowid\taz anya bejegyzésének azonosítója\r\n\
anya neve (*születés +halálozás)\r\n\
\r\n\
rowid\t az elsõ házastárs azonosítója\r\n\
házastársak felsorolása születési és halálozási dátumukkal valamint az esküvõ dátumával\r\n\
\r\n\
Az azonosítás eredményét a sorok háttér színezése is kiemeli:\r\n\
\r\n\
fehér\tegyik bejegyzéssel sem azonos\r\n\
színes\taz ezt követõ szürke hátterû bejegyzésekkel azonos, azokkal össze lett vonva\r\n\
\r\n\
A jobb egérgombbal a táblázat egy sorára kattintva egy legördülõ menübõl választhatunk hasznos funkciókat a GA-html fájl vizsgálatához, javításához.\r\n\
Az összehasonlítás, összevonás eredményét két html fájlban is elõállítja a program, amiket a html menüponttal nézhetünk meg.\
";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CContractInfo::~CContractInfo()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContractInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, EditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CContractInfo, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CContractInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_title );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_info );
	GetDlgItem(IDC_EDIT)->ModifyStyle(0, WS_DISABLED);
	return FALSE; 
}

