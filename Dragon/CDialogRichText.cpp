// CDialogRichText.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CDialogRichText.h"


// CDialogRichText dialog

IMPLEMENT_DYNAMIC(CDialogRichText, CDialogEx)

CDialogRichText::CDialogRichText(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDialogRichText, pParent)
{
	AfxInitRichEdit2();
}

CDialogRichText::~CDialogRichText()
{
}

void CDialogRichText::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RICHEDIT, m_editCtrl);
}


BEGIN_MESSAGE_MAP(CDialogRichText, CDialogEx)
//    ON_EN_SETFOCUS(IDC_RICHEDIT, &CDialogRichText::OnSetfocusRichEdit)
END_MESSAGE_MAP()


// CDialogRichText message handlers


BOOL CDialogRichText::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
    CString text1 = L"This is the first line";
    CString text2 = L"This is the second line";
    CString text3 = L"This is the third line";
    CString text4 = L"This is the fourth line";

    InsertText(text1, RED, true, false);
    InsertText(text2, GREEN, true, false);
    InsertText(text3, BLUE, true, false);
    InsertText(text4, BLACK, true, false);
*/

    CString info = L"\
\r\n\
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
\r\n\
Két azonos nevû ember bejegyzéseinek összevonhatóságához az alábbi rokonság nevét, születési és halálozási dátumát vizsgáljuk\r\n\
\r\n\
bejegyzés\t\tnév\tszületés\thalál\r\n\
apa\t\t\tnév\tszületés\thalál\r\n\
anya\t\t\tnév\tszületés\thalál\r\n\
házastárs\t\tnév\tszületés\thalál\t\tapja: név\tszületés\thalál\t\tanyja: név\tszületés\thalál\r\n\
\r\n\
Két bejegyzés fenti adatpárjainak összehasonlítása az alábbi eredményre vezethet:\r\n\
\r\n\
1. azonos:\tmindkét adat meg van adva, és azok azonosak.\r\n\
2. nem értékelhetõ:\tegyik vagy egy adat sincs megadva.\r\n\
3. ellentmondó:\tmindkét adat meg van adva, és azok nem egyeznek.\r\n\
\r\n\
Hibátlan adatok esetén egy ember különbözõ bejegyzéseiben és annak rokonságában semmilyen ellentmondó adat nincs, a bejegyzések \
azonosítása nagyon egyszerû feladat.\r\n\
\r\n\
Az adatokban azonban hibák és hiányosságok lehetnek, amik az azonosítást megnehezítik. Az összevonás során kétféle hibát követhetünk el, \
a hibák miatt nem ismerjük fel, hogy két bejegyzés ugyanannak az embernek a bejegyzése, vagy ugyancsak a hibák miatt két bejegyzést \
összevonunk, pedig nem azonos emberhez tartoznak. Mindkét hiba tragikus következményekkel járhat a rokoni kapcsolatokra. \
Ezért az összevonás elõtt igyekezzük a hiányosságokat pótolni, a hibákat kijavítani. \
Ehhez segítséget nyújtanak az 'Ellenõrzések összevonás elõtt' menü funkciói. \
Maga az összevonás is feltár hibákat, amiket azonnal megjelenít egy listában. Az itt azonosítható hibákat is javítsuk ki.\
Az azonosításban felhasznált adatok hibái elkerülhettelenül hibás összevonásokat fognak eredményezni!! Ezt kivédeni nem lehet!\r\n\
\r\n\
Azt gondolhatnánk,hogy ha két bejegyzésben a nevek, születési ill. halálozási dátumok ismertek és azonosak, akkor semmi baj nem lehet \
ha összevonjuk a két bejegyzést, hiszen nagyon nagy valószínûséggel azok azonos emberhez tartoznak. Ez igaz is, de ha az apa, anya vagy \
a házastársakban adathibák vannak, hamis rokoni kapcsolatok jöhetnek létre. Csöbörbõl vödörbe. Nem lehet tudni, hogy mi a helyes döntés.\r\n\
\r\n\
A felhasználó három összevonási algoritmus közül választhat, a laza, közepes és a szigorú ellnepõrzés közül.\r\n\
\r\n\
1. Laza ellenõrzéskor ha két bejegyzésben a név, születési és halálozási dátum megegyezik, azokat minden további vizsgálat nélkül azonos ember \
bejegyzéseinek értékeljük és összevonjuk õket. Ezzel elkerülhetjük, hogy a fel nem használt adatokban elõforduló hibák az összevonást megakadályozzák.\r\n\
\r\n\
2. Közepes ellenõrzésnél még ilyenkor is tovább vizsgáljuk a szülõket. Ha a szülõk között vannak értékelhetõ adatok és nincs ellentmondás közöttük, \
akkor azonosnak elfogadjuk a bejegyzéseket.\r\n\
\r\n\
3. Szigorú vizsgálatnál még ekkor is tovább megyünk és a házastársakat is összehasonlítjuk. Több házastárs esetén nem követeljük meg, hogy mindkét \
bejegyzéshez tartozó házaspár-pároknak meglegyen az értékelhetõ és azonos adata, csak annyit, hogy legyen egy nem ellentmondó adatokat tartalmazó \
azonos nevû házaspár. Ezzel az engedménnyel lehetõvé tesszük, hogy a bemeneti GA.htm fájlban hiányosan megadott házastársú bejegyzéseket is \
összevonhassunk.\r\n\
\r\n\
A laza és közepes ellenõrzéssel végzett összevonás nagyon veszélyes lehet, mert a nem ellenõrzött rokonságot is megszerzi az összevont bejegyzés \
ami ferobbanthatja a leszármazotti és felmenõi listákat. Így tud egy ember extra házastársakat szerezni, akár azonos nevû, megismételt házastársat, \
akiknek valamilyen adatuk nem egyezik, ezért nem lettek összevonva.\r\n\
\r\n\
A szigorú ellenõrzésnek is megvannak a hátrányai, elõfordulhat, hogy nem egyesítjük az azonos emberhez tartozó bejegyzéseket. \
Ennek ellenére a szigorú ellenõrzést javaslom. A többi inkább arra való, hogy belelássunk az összevonás rejtelmeibe.\r\n\
Az esetleg elmaradt összevonást utólag is el végezni: az emberek listáján kijelölt 2 bejegyzést a legördülõ menüben találhat összevonás mûvelettel \
egyesíthetünk.\n\
Nem gyõzõm hangsúlyozni, hogy hibás adatokkal nem lehet hibátlan összevonást végezni. Bármilyen nagy munka, a hibák kijavításának \
kell megelõznie az összevonást!\r\n\
\r\n\
Az azonosnak értékelt bejegyzések közül a legmagasabb prioritású bejegyzést megtartjuk, a törlendõkbõl pedig átvesszük azokat az adatokat, \
amelyek a megtartott bejegyzésben nem voltak megadva, majd töröljük ezeket. A törölt bejegyzésekre történõ hivatkozásokat \
(a gyerekek szülõkre ill.a házastársak egymásra történõ hivatkozásait) átirányítjuk a megtartott bejegyzésre.\r\n\
\r\n\
Leszármazottakat semmiképpen nem egyesítünk, mert õk csak egyszer fordulhatnak elõ. \
A kettõs keresztnevekbõl csak az elsõ használjuk az összehasonlításhoz.\n\
A dátumok összehasonlításánál megengedjük, hogy a pusztán évszámmal ill. teljes dátummal megadott adatpárokat azonosnak értékelje, ha az évszám azonos.\n\
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
    POINT p;
    p.x = 0;
    p.y = 0;

    CHARFORMAT cf = { 0 };
    m_editCtrl.GetSelectionCharFormat(cf);
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
    cf.dwEffects = CFE_BOLD;
    cf.crTextColor = LIGHTRED;
    cf.yHeight = 300;

//    int txtLen = m_editCtrl.GetTextLength();

    // Want the text limit to be at least the size of the new string.
//    if (m_editCtrl.GetLimitText() < txtLen)
//        m_editCtrl.LimitText( txtLen);
    m_editCtrl.SetWindowTextW(info);
    m_editCtrl.SetSel(0, -1);
    m_editCtrl.SetSelectionCharFormat(cf);
    m_editCtrl.Clear();
/*
  //  m_editCtrl.SetSel(txtLen, m_editCtrl.GetTextLength());
    m_editCtrl.SetSel(0, -1);
    m_editCtrl.SetSelectionCharFormat(cf);
    m_editCtrl.SetSel(0, 0);

    InsertText(info, BLACK, false, false);
    m_editCtrl.SetCaretPos(p);
    ShowWindow(SW_MAXIMIZE);
*/
	return TRUE;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDialogRichText::InsertText(CString text, COLORREF color, bool bold, bool italic)
{
    CHARFORMAT cf = { 0 };
    int txtLen = m_editCtrl.GetTextLength();

    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR | CFM_SIZE;
    cf.dwEffects = (bold ? CFE_BOLD : 0) | (italic ? CFE_ITALIC : 0);
    cf.crTextColor = color;
    cf.yHeight = 300;
    m_editCtrl.SetSel(txtLen, -1); // Set the cursor to the end of the text area and deselect everything.
    m_editCtrl.ReplaceSel((txtLen < 1 ? L"" : L"\n") + text);

    // Apply formating to the just inserted text.
    m_editCtrl.SetSel(txtLen, m_editCtrl.GetTextLength());
    m_editCtrl.SetSelectionCharFormat(cf);

}

//void CDialogRichText::OnSetfocusRichEdit()
//{
//    m_editCtrl.SetSel(0, 0);
//}
