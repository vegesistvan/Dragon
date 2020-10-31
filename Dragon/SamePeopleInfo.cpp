// SamePeopleInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "SamePeopleInfo.h"
#include "afxdialogex.h"


// CSamePeopleInfo dialog

IMPLEMENT_DYNAMIC(CSamePeopleInfo, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSamePeopleInfo::CSamePeopleInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSamePeopleInfo::IDD, pParent)
	, m_info(_T(""))
{
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
\r\n\
Ha két bejegyzés fenti adatokpárjai között akár egy ellentmondás van, akkor nyilvánvalóan nem azonos személy bejegyzéseirõl van szó.\r\n\
Ha nincs ellentmondás, akkor 1 adatpár létezése és egyezése elegendõ az azonosság megállapításához.\r\n\
Leszármazottakat akkor sem egyesítünk, ha mindenben megfelelel az összevonási kritériumoknak, mert leszármazottként \n\
csak egyszer szerepelhet egy ember.\r\n\
Több házastárs esetén az azonosításhoz elegendõ, ha egy azonos nevû házastárs szerepel mindkét bejegyzésben.\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz.\r\n\
\r\n\
Az adathibák nyilván egyesítési hibákhoz vezetnek, valójában különbözõ emberekhez tartozó bejegyzésket tévesen \
egyesítünk, vagy azonos emberek bejegyzéseinek összevonása elmarad. Ezért az egyesítés elõtt minden hibát \
ki kell javítani!.\r\n\
A hibákra az \"Ellenõrzése egyesítés elõtt\" funkciók világítanak rá. Az egyesítés során az azonosítás erdményét tartalmazó \
táblázat, fájlok is felfedhetnek hibákat.\r\n\
\r\n\
Az oszlopok jelentése:\r\n\
\r\n\
gr\tgroup, az azonos nevû embercsoporton belül azonosnak éréklelt alcsoportok sorszáma.\r\n\
mt\tmatch, az azonosnak talált adatpárok száma\r\n\
gp\tgroupP, korábban a gp csoporthoz tartozott, de elvette, mert több azonosság van ezzel a bejegyzéssel\r\n\
st\tstatus, -1 azonos, azaz egyesített, majd törölt, 0: változatlanul hagyott, 1: ez az egyesített bejegyzés.\r\n\
line\ta bejegyzés sorszáma a GA html fájlban.\r\n\
u\tunited, az ember u számú bejegyzés összevonása.\r\n\
G\tgeneráció, az ember generációs jele a GA fájlban.\r\n\
S\taz ember elõfordulása a GA.html fájlban ( 1-2-3-4 )\r\n\
\r\n\
rowid\ta bejegyzés azonosítója\r\n\
név\taz ember neve\r\n\
születés\tszületési dátum\r\n\
halál\thalálozás dátuma\r\n\
\r\n\
rowid\taz apa bejegyzésének azonosítója\r\n\
név\taz apa neve\r\n\
születés\tszületési dátuma\r\n\
halál\thalálozás dátuma\r\n\
\r\n\
rowid\taz anya bejegyzésének azonosítója\r\n\
név\taz anya neve\r\n\
születés\tszületési dátuma\r\n\
halál\thalálozás dátuma\r\n\
\r\n\
rowid\taz elsõ házastárs azonosítója\r\n\
házastársak felsorolása\r\n\
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
CSamePeopleInfo::~CSamePeopleInfo()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSamePeopleInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, EditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSamePeopleInfo, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSamePeopleInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_info );
	GetDlgItem(IDC_EDIT)->ModifyStyle(0, WS_DISABLED);
	return FALSE; 
}

