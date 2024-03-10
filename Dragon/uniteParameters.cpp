// UnitePar.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "UniteParameters.h"
#include "displayInfo.h"

IMPLEMENT_DYNAMIC(CUniteParameters, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteParameters::CUniteParameters(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNITE_PARAMETERS, pParent)
	, m_name(_T(""))
	, m_rowid1(_T(""))
	, m_rowid2(_T(""))
	, m_snameEnough( true )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUniteParameters::~CUniteParameters()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_nameCtrl);
	DDX_Control(pDX, IDC_WARNING, colorWarning);
	DDX_Text(pDX, IDC_ROWID1, m_rowid1);
	DDX_Text(pDX, IDC_ROWID2, m_rowid2);
	DDX_Control(pDX, IDC_COMBO_CYCLE, m_comboCycleCtrl);
	DDX_Control(pDX, IDC_COMBO_ALGO, ComboAlgoCtrl);
	DDX_Check(pDX, IDC_CHECK_SPOUSE, m_snameEnough);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CUniteParameters, CDialogEx)
	ON_COMMAND(ID_INFO, &CUniteParameters::OnInfo)
	ON_BN_CLICKED(IDC_CLEAR, &CUniteParameters::OnClickedClear)
	ON_BN_CLICKED(IDC_CHECK_SPOUSE, &CUniteParameters::OnClickedCheckSpouse)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParameters::OnInfo()
{
	CString info = L"\
\r\n\
MILYEN BEJEGYZÉSEK VANNAK A GA.HTM FÁJLBAN ÉS MI AZ ÖSSZEVONÁS CÉLJA?\r\n\
\r\n\r\n\
A GA.htm fájlban egy embernek több bejegyzése is lehetséges, elõfordulhat leszármazottként, leszármazott házastársakénk, a leszármazott házastársának \
szülõjeként, leszármazott házastársának további házastársaként, de az nincs jelölve, hogy ezek ugyannnak az embernek különbözõ elõfordulásai. \
A különbözõ szerepben elõforduló bejegyzések különbözõ részletességûek és megbízhatóságúak. A házastárs szülõje és a házastárs további házastársa \
általában csak egy név, minden további adat nélkül. Tehát az azonosításuk csak a rokoni kapcsolaton keresztül lehetséges. Egy ember különbözõ \
bejegyzéseit prioritási sorrendben képzeljük el, ami a fenti sorrenddel egyezik meg. Ennek a késõbbiekben lesz jelentõsége.\r\n\
\r\n\
Kívánatos, hogy ezek a bejegyzéseket összevonjuk, vagyis minden embernek csak egy bejegyzése legyen, egy helyen lehessen az adatok karbatartását \
biztonságosan végezni. A különbözõ rokoni kapcsolatokat pedig ezen bejegyzésre történõ hivatkozásokkal fejezzük ki. \
Az összevonás nem csak azért fontos, hogy rend legyen. Az összevonás másik fontos szerepe a rokoni kapcsolatok teljes felépítése.\r\n\
\r\n\
A GA.htm fájlban nincs minden rokoni kapcsolat explicit módon megadva. Egy nõi leszármazott gyerekei általában nincsenek feltüntetve. A gyerekek a férj \
egy másik lista 'leszármazotti' elõfordulása után vannak felsorolva. Az apa és anya két elõfordulásának egyesítésével lesznek a gyerekek az egyesített \
apa és anya gyerekei.\r\n\
A kis családi, szülõ-gyerek kapcsolatok csak a leszármazottak között van explicit módon kifejezve, \
a házastársak közötti testvéri kapcsolatokat csak az azonos emberek bejegyzéseinek összevonása által tudjuk felépíteni.\r\n\
\r\n\
A GA.htm fájban gyakran hiányos adatok vannak, pl. egy férfi leszármazottnak nincs megadva az egyik felesége. \
Ha ez a nõ szerepel egy másik bejegyzésben is, ahol meg van adva az a férje is ahol õ nem szerepel házastársként, akkor a férj az összevonással \
megkapja a hiányzó feleségét is. \r\n\
\r\n\
A bejegyzések összevonásával az összevont bejegyzés átveszi a törölt bejegyzésekhez tartozó rokonságot is, a gyerekeket, házastársakat. \
Az összevonás tehát egy nagyon kritikus mûvelet, amellyel rokoni kapcsolatokat lehet helyreállítani, hibás összevonással pedig felborítani.\r\n\
\r\n\r\n\
AZ ÖSSZEVONÁS FELTÉTELEI\r\n\
\r\n\r\n\
Két azonos nevû ember bejegyzéseinek összevonhatóságához az alábbi rokonság nevét, születési és halálozási dátumát vizsgáljuk\r\n\
\r\n\
bejegyzés\tnév\tszületés\thalál\r\n\
apa\t\tnév\t\születés\thalál\r\n\
anya\t\tnév\tszületés\thalál\r\n\
házastárs\t\tnév\tszületés\thalál\t\tapja: név\tszületés\thalál\t\tanyja: név\tszületés\thalál\r\n\
\r\n\
Két bejegyzés fenti adatpárjainak összehasonlítása az alábbi eredményre vezethet:\r\n\
\r\n\
1. azonos:\tmindkét adat meg van adva, és azok azonosak.\r\n\
2. nem értékelhetõ:\tegyik vagy egy adat sincs megadva.\r\n\
3. ellentmondó:\tmindkét adat meg van adva, és azok nem egyeznek.\r\n\
\r\n\
Hibátlan adatok esetén egy ember különbözõ bejegyzéseiben és annak rokonságában semmilyen ellentmondó nem lehet, a bejegyzések \
azonosítása nagyon egyszerû feladat.\r\n\
\r\n\
Az adatokban azonban hibák és hiányosságok lehetnek, amik az azonosítást megnehezítik. Az összevonás során kétféle hibát követhetünk el, \
a hibák miatt nem ismerjük fel, hogy két bejegyzés ugyanannak az embernek a bejegyzése és nem vonjuk össze azokat, vagy két bejegyzést \
összevonunk hibás adatok miatt, pedig nem azonos emberhez tartoznak. Mindkét hiba tragikus következményekkel járhat a rokoni kapcsolatokra. \
Ezért az összevonás elõtt igyekezzük a hiányosságokat pótolni, a hibákat kijavítani. \
Ehhez segítséget nyújtanak az 'Ellenõrzések összevonás elõtt' menü funkciói. \
Maga az összevonás is feltár hibákat, amiket azonnal megjelenít egy listában. Az itt azonosítható hibákat is javítsuk ki.\
Az azonosításban felhasznált adatok hibái elkerülhettelenül hibás összevonásokat fognak eredményezni!! Ezt kivédeni nem lehet!\r\n\
\r\n\
Azt gondolhatnánk,hogy ha két bejegyzésben a nevek, születési ill. halálozási dátumok ismertek és azonosak, akkor semmi baj nem lehet \
ha összevonjuk a két bejegyzést, hiszen nagyon nagy valószínûséggel azok azonos emberhez tartoznak. Ez igaz is, de ha az apa, anya vagy \
a házastársakban adathibák vannak, hamis rokoni kapcsolatok jöhetnek létre. Csöbörbõl vödörbe. Nem lehet tudni, hogy mi a helyes döntés.\r\n\
\r\n\
Két bejegyzés összevonhatóságának szükséges feltétele a fenti adatok ellentmondás mentességee. Ha bármilyen ellentmondást találunk, \
akkor a két bejegyzést nem vonjuk össze.\n\
Az összevonáshoz - az ellentmondás mentességen kívül - még szükséges, hogy a rokonság neveinek azonosságán túl legalább egy dátumban \
azonos is legyen a két bejegyzés. A házastársaknál csak az azonos nevû házastársakat vizsgáljuk, nem követeljük meg, hogy több \
házastárs esetén mindkét bejegyzéshez minden házastárs kapcsolódjon. \
Ezzel az engedménnyel lehetõvé tesszük, hogy a bemeneti GA.htm fájlban hiányzó vagy hiányosan megadott házastársú bejegyzéseket is \
összevonhassunk.\r\n\
\r\n\
Minden gondosság dacára elõfordulhat hiba az összevonások során. Leginkább az, hogy azonos ember bejegyzésit nem vonjuk össze. \
Az esetleg elmaradt összevonást utólag is el végezni: az emberek listáján kijelölt 2 bejegyzést a legördülõ menüben találhat összevonás \
mûvelettel egyesíthetünk.\n\
Nem gyõzõm hangsúlyozni, hogy hibás adatokkal nem lehet hibátlan összevonást végezni. Bármilyen nagy munka, a hibák kijavításának \
kell megelõznie az összevonást!\r\n\
\r\n\r\n\
AZ EGYESÍTÉSI ELJÁRÁS\r\n\
\r\n\r\n\
Az azonosnak értékelt bejegyzések közül a legmagasabb prioritású bejegyzést megtartjuk, a törlendõkbõl pedig átvesszük azokat az adatokat, \
amelyek a megtartott bejegyzésben nem voltak megadva, majd töröljük ezeket. A törölt bejegyzésekre történõ hivatkozásokat \
(a gyerekek szülõkre ill.a házastársak egymásra történõ hivatkozásait) átirányítjuk a megtartott bejegyzésre.\r\n\
\r\n\
Leszármazottakat semmiképpen nem egyesítünk, mert õk csak egyszer fordulhatnak elõ. \
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz. \n\
A dátumok összehasonlításánál megengedjük, hogy a pusztán évszámmal ill. teljes dátummal megadott adatpárokat azonosnak értékelje, \
ha az évszám azonos.\n\
Ha két bejegyzést összevonunk, akkor apjaikat és anyjaikat is összevonjuk, hisz nyilvánvalóan õk is azonosak.\r\n\
\r\n\
A program ABC sorrendben végigfut az azonos nevû bejegyzések vizsgálatán, összevonásán, majd elõlrõl kezdi ezt a ciklust. \
Ezt azért csinálja, mert elõfordulhat, hogy 2 azonos nevû bejegyzés összevonása azért nem történik meg az elsõ ciklusban, \
mert az azonossá nyilvánításhoz szüksége lenne azonos nevû házastársa, de az adathiány miatt nincs. \
Azonban még az elsõ ciklusban összevonhat azonos nevû bejegyzést, és a megtartott bejegyzés hozza magával \
a törölt bejegyzések házastársait is. Ezek az esetek többnyire adat hiányra vagy adat hibára vezethetõk vissza és javítani kell. \
A második ciklusban összevont bejegyzéseket meg kell vizsgálni, miért nem történt  meg az egyesítés az 1. ciklusban.\r\n\
\r\n\
A paraméterek között megadhatunk egy nevet, akiknek a vizsgálatát és az azonosak egyesítését írjuk elõ. Ez kisérletezére \
szolgáló funkció.\r\n\
";
	CDisplayInfo dlg;
	dlg.m_text = &info;
	dlg.m_title = L"Egy ember különbözõ bejegyzéseinek vizsgálata, összevonása";
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CUniteParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_name = theApp.GetProfileString(L"dragon", L"m_name", L"");
	m_rowid1 = theApp.GetProfileString(L"dragon", L"m_rowid1", L"");
	m_rowid2 = theApp.GetProfileString(L"dragon", L"m_rowid2", L"");
	m_cycle = theApp.GetProfileInt(L"dragon", L"m_cycle", 2 );
	m_nameCtrl.SetWindowTextW(m_name);
	colorWarning.SetTextColor(LIGHTRED);

	m_comboCycleCtrl.AddString(L"1");
	m_comboCycleCtrl.AddString(L"2");
	m_comboCycleCtrl.AddString(L"3");
	m_comboCycleCtrl.AddString(L"4");
	m_comboCycleCtrl.SetCurSel(m_cycle);


	ComboAlgoCtrl.AddString(L"Lassú: azonos nevû bejegyzések ciklusában történik a beolvasás");
	ComboAlgoCtrl.AddString(L"Gyors: azonos nevû bejegyzések ");
	ComboAlgoCtrl.SetCurSel(1);
	UpdateData(TOSCREEN);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParameters::OnOK()
{
	UpdateData(FROMSCREEN);
	m_nameCtrl.GetWindowTextW( m_name );
	m_cycle = m_comboCycleCtrl.GetCurSel();
	m_algorithm = ComboAlgoCtrl.GetCurSel();

	theApp.WriteProfileString(L"dragon", L"m_name", m_name);
	theApp.WriteProfileString(L"dragon", L"m_rowid1", m_rowid1);
	theApp.WriteProfileString(L"dragon", L"m_rowid2", m_rowid2);
	theApp.WriteProfileInt(L"dragon", L"m_cycle", m_cycle);
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParameters::OnClickedClear()
{
	m_nameCtrl.SetWindowTextW(L"");
	m_comboCycleCtrl.SetCurSel(2);
	m_rowid1.Empty();
	m_rowid2.Empty();
	UpdateData(TOSCREEN);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUniteParameters::OnClickedCheckSpouse()
{
	m_snameEnough != m_snameEnough;
}
