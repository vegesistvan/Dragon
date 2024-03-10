// SamePeopleInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "uniteInfo.h"
#include "afxdialogex.h"


// CUniteInfo dialog

IMPLEMENT_DYNAMIC(CUniteInfo, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteInfo::CUniteInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniteInfo::IDD, pParent)
	, m_info(_T(""))
{

	m_title = L"Azonos nevû emberek bejegyzéseinek vizsgálata és összevonása";
	m_info = L"\
Két azonos nevû ember bejegyzéseinek összevonhatóságához az alábbi rokonság nevét, születési és halálozási dátumát vizsgáljuk:\r\n\
\r\n\
1. maguk a két azonos nevû emberek\r\n\
2. az apákat\r\n\
3. az anyákat\r\n\
4. az apai nagyapákat\r\n\
5. az apai nagyanyákat\r\n\
6. az anyai nagyapákat\r\n\
7. az anyai nagyanyákat\r\n\
8. egy közös nevû házastársat\r\n\
9. a házastársak apját\r\n\
10.a házastársak anyját\r\n\
\r\n\
Ha két bejegyzés fenti adatpárjai között akár egy ellentmondás van, akkor azokat különbözõ emberek bejegyzéseinek tekintjük.\r\n\
\r\n\
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz.\r\n\
Leszármazottakat semmiképpen nem egyesítünk, mert õk csak egyszer fordulhatnak elõ.\r\n\
Több házastárs esetén az azonosításhoz elegendõ, ha egy azonos nevû házastárs ellentmondás menetesen szerepel mindkét bejegyzésben.\r\n\
\r\n\
Az adathibák nyilván egyesítési hibákhoz vezetnek, különbözõ emberekhez tartozó bejegyzésket tévesen egyesíthetünk\
vagy azonos emberek bejegyzéseinek összevonása elmarad. Ezért az egyesítés elõtt törkedni kell a hibák kijavítanására.\r\n\
A hibákra az \"Ellenõrzése egyesítés elõtt\" funkciók világítanak rá. Innne indítva, ezeket az egyesítési listákat úgy készíti el, hogy nem \
törli a törlendõ bejegyzéseket, csak szürke színnel jelzi õket. Így lehetõségünk van rá hogy megvizsgájuk, miért minõsíttte úgy a bejegyzéseket, \
ahogy minõsítette.\r\n\
\r\n\
Az oszlopok jelentése:\r\n\
\r\n\
loop\taz összevonás iterációjának sorszáma\r\n\
gr\tcsoport azonosító, amibe az összevont emberek tartoznak\r\n\
st\tstátusz: 1-megtartott bejegyzés, 0-törült bejegyzés\r\n\
u\tunited, az ember u számú bejegyzés összevonása.\r\n\
s\taz ember elõfordulása a GA.html fájlban(1 - 2 - 3 - 4)\r\n\
\r\n\
rowid\ta bejegyzés azonosítója, neve, *születése, +halálozása\r\n\
rowid\taz apa bejegyzésének azonosítója, neve, *születése, +halálozása\r\n\
rowid\taz anya bejegyzésének azonosítója, neve, *születése, +halálozásq)\r\n\
házastársak felsorolása azonosító számukkal, születési és halálozási dátumukkal\r\n\
\r\n\
A 'Listák' menüpontból három táblázat valamelyikét ábrázolhatjuk:\r\n\
\r\n\
1. Azonos nevû bejegyzések, amelyek között talált összevonhatókat.\r\n\
2. Név szerint csoportosított bejegyzések, amelyek között nem talált összevonhatókat.\r\t\
3. Gyanús bejegyzések, amelyek között az elírt száûmú adatpár egyezést megtalálta, de ellentmondásos adatpárok is vannak, \
ezért nem egyesíti azokat.\r\n\
\r\n\
Az azonosítás eredményét a sorok háttér színezése is kiemeli:\r\n\
\r\n\
fehér\tegyik bejegyzéssel sem azonos\r\n\
színes\taz ezt követõ szürke hátterû bejegyzésekkel azonos, azokkal össze lett vonva, a szürkék törlve lettek.\r\n\
\r\n\
A jobb egérgombbal a táblázat egy sorára kattintva egy legördülõ menübõl választhatunk hasznos funkciókat a GA-html fájl \
vizsgálatához, javításához.\r\n\
A bal egérombbal kétszer kattintva egy bejegyzése, a sorban megadott azonosítójú ember rokoni kapcsolatait mutatja meg, amiben az ismert \
módon navigálhatunk. Ha két sort jelölünk ki, és a bal egérgomb kattintással legördülõ menürõl az \"Egyesítési adatok\" funkciót választjuk, \
akkor a kijelölt bejegyzés fent felsorolt adtai, amiket az azonosság eldöntéséhetz használunk, egy ablakban megmutatja, jelezve a különbségeket.\
";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteInfo::~CUniteInfo()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, EditCtrl);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUniteInfo, CDialogEx)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW( m_title );
	GetDlgItem( IDC_EDIT )->SetWindowTextW( m_info );
	GetDlgItem(IDC_EDIT)->ModifyStyle(0, WS_DISABLED);
	return FALSE; 
}

