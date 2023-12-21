#include "unitedEntries.h"
#include "displayInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnInfoUnited()
{
	CString info = L"\r\n\
AZ EGYESÍTÉSI ELJÁRÁS EREDMÉNYEI\r\n\
\r\n\
Az adatokban lévõ hibák és hiányosságok hamis összevonásokat eredményezhetnek és szükséges összevonások kimaradhatnak az egyesítésbõl.\
Ezért hibás leszármazotti és felmenõi listák keletkezhetnek. Ezért fontos, hogy áttekinsük mi történt az összevonás során, a felismerhetõ \
hibákat javítsuk. Ezt segítik az itt megjelentetett listák, amelyeken betekinthetünk az egyesítési folyamatba.\r\n\
\r\n\
A bejegyzések egyesítési eljárásának eredményei 3 text fájlban keletkeznek, amelyek specifikációja az adatbázisban van megõrizve.\r\n\
A 3 fájlt egymásután beolvassa, azok tartalmát listázza. Az ablak 3 fülének egyikével választható ki a megnézni kívánt lista, amely \
az alábbiak egyike lehet.\r\n\
\r\n\
1. Az azonos nevû bejegyzések, amelyek között vannak egyesítettek\r\n\
2. Az azonos nevû bejegyzések, amelyek között nincsenek egyesítettek\r\n\
3. Azonos nevû bejegyzések, amelyek között vannak azonos és ellentmondó adatpárok\r\n\
\r\n\
A fülek kiválasztásával a fõmenû és a legördülõ menü funkciók is változnak.\r\n\
\r\n\
Mindhárom táblának ugyanolyan oszlopai vannak:\r\n\
Mindhárom táblában kereshetünk az 'ember' vagy az 'anya' oszlopban. A keresés oszlopát a bal felsõ srokban választhatjuk ki.\n\r\
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
Mindhárom táblánál a bal egérgombbal egy sorra kattintva, egy felugró ablakban megmutatja a bejegyzés családját.\r\n\
\r\n\
AZONOS NEVÛ BEJEGYZÉSEK, AMELYEK KÖZÖTT VANNAK EGYESÍTETTEK\r\n\
\r\n\
Az azonosítás eredményét a sorok háttér színezése is kiemeli:\r\n\
\r\n\
fehér\tegyik bejegyzéssel sem azonos\r\n\
színes\taz ezt követõ szürke hátterû bejegyzésekkel azonosnak értékelt és megtartott bejegyzés,\r\n\
szürke\ttörlve lettek.\r\n\
\r\n\
Az egyesített bejegyzések listáján csak azok az egyesítések találhatóak, amelyek az 'ember'-ek azonosításának eredményeként kerültek egyesítésre. \
Tehát nincsenek itt felsorolva azok a szülõk, akik az 'ember'-ek azonossága alapján lettek egyesítve, akik nyilvánvalóan azonosak, \
hiszen azonos emeberek szülei,\r\n\
A program 2 iterációt végez. A táblázatot a 'Szûrés menüponttal lehet szûrni: az 1. vagy a 2. iterációra, ill. szûrés nélkülire.\r\n\
Mint tudjuk, az egyesítési eljárás az megtratott bejegyzésbe átemeli a törlendõ bejegyzésekbõl azokat az adatokat, amelyek hosszabbak a törlendõ bejegyzésben. \
Ezt a listán az '^^^' karaktersorozattal jelzi a megfelelõ mezõben. Ezeket a megjelölt mezõket kereshetjük az 'Átvett adatok' menüpontra történõ \
ismételt kattintással.\r\n\
\r\n\
Egy sorra kattintva a legördülõ menübõl az alábbiak között választhatunk:\r\n\
- Bejegyzés a htm fájlban\r\n\
- 2 kijelölt bejegyzés a html fájlban\r\n\
- 2 kijelölt bejegyzés rokoságának összehasonlítása\r\n\
- 2 kijelölt bejegyzés összevonása\r\n\
- A kijelölt név szerinti szûrés\r\n\
\r\n\
Ez utóbbi funkciót úgy ajánlott használni, hogy elõszõr szürjük a listát a 2. iterációra, majd kijüljük a kívánt nevet és hívjuk \
a 'Kijelölt név szerinti szûrés'-t a legördülõ menübõl. Így látni fogjuk, hogy a kijelölt nevet az 1. és a 2. iterációban is, tehát \
láthatjuk, hogy mi történt az összevonásokkal a két iterációban.\r\n\
\r\n\
AZONOS NEVÛ BEJEGYZÉSEK, AMELYEK KÖZÖTT NINCSENEK EGYESÍTÉSEK\r\n\
\r\n\
Itt a legördülõ menüben értelemszerûen csak az az elsõ két menüpont található:\r\n\
- Bejegyzés megmutatás a htm fájlban a NotePad++-ban\r\n\
- 2 kijelölt bejegyzés rokoságának összehasonlítása\r\n\
\r\n\
AZONOS NEVÛ BEJEGYZÉSEK, AMELYEK KÖZÖTT VANNAK AZONOS ÉS ELLENTMONDÓ ADATPÁROK\r\n\
\r\n\
Itt párosával vannak felsorolva az ilyen bejegyzések, sárgával jelölve az eltérõ adatokat. Ezek a bejegyzések tehát nem lettek \
összewvonva, da nagyon valószínû, hogy ugyanannak az embernek a bejegyzései, csak hibás adatok miatt nem lehett összevonni õket.\
A hibákat ki kell javítani a html fájlban, és újra be kell olvasni majd összevonni az adatokat.\r\n\
\r\n\
A a legördülõ menüben található funkciók:\r\n\
- Bejegyzés a htm fájlban\r\n\
- 2 kijelölt bejegyzés rokonságának összehasonlítása\r\n\
\r\n\
Ezek a funkciók segítséget nyújtanak a htm fájl javításában.\r\n\
\r\n\
";
	CDisplayInfo dlg;
	dlg.m_text = &info;
	dlg.m_title = L"Bejegyzések egyesítési eljárásának erdményeirõl";
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnInfoUnion()
{
	CString info = L"\
\r\n\
MILYEN BEJEGYZÉSEK VANNAK A GA.HTM FÁJLBAN ÉS MI AZ ÖSSZEVONÁS CÉLJA?\r\n\
\r\n\r\n\
A GA.htm fájl beolvasásakor minden emberrõl, aki elõfordul a fájlban, egy bejegyzés készül, amely tartalmazza az ember nevét és minden, \
a fájlban megadott információt.\r\n\
A GA.htm fájlban egy ember elõfordulhat leszármazottként, leszármazott házastársakénk, a leszármazott házastársának \
szülõjeként, leszármazott házastársának további házastársaként. Mindegyikrõl egy bejegyzés készül. Egy embernek tehát több bejegyzése is \
lehet. A különbözõ szerepben elõforduló bejegyzések különbözõ részletességûek, megbízhatóságúak és különbözõ rokoni kapcsolatai vannak \
meghatározva. A házastárs szülõje és a házastárs további házastársa általában csak egy név, minden további adat nélkül. \
Tehát az õ azonosításuk csak a rokoni kapcsolaton keresztül lehetséges, ami viszont szintén nagyon szegényes, hiszen a szülõknek \
csak egy gyereke, a további házastársnak pedig egyik házastársa van meghatározva. \
Az õ aznosításuk a legproblematikusabb. \
Egy ember különbözõ bejegyzéseit prioritási sorrendben képzeljük el, a várhatóan legtöbb adattal és rokonsággal rendelkezõ leszármazott \
bejegyzése a legmagasabb, 1-es prioritású, 2-es a leszármazott házastársa, 3-as a leszármazott házastársának szülei, 4-es a leszármazott \
házastársának további házastársa. Ennek a késõbbiekben lesz jelentõsége.\r\n\
\r\n\
Kívánatos, hogy az egy emberhez tartozó bejegyzéseket összevonjuk, vagyis minden embernek csak egy bejegyzése legyen, egy helyen lehessen \
az adatok karbatartását biztonságosan végezni. A különbözõ rokoni kapcsolatokat pedig ezen bejegyzésre történõ hivatkozásokkal fejezzük ki. \
Az összevonás nem csak azért fontos, hogy rend legyen. Az összevonás másik fontos szerepe a rokoni kapcsolatok teljes felépítése.\r\n\
\r\n\
A GA.htm fájlban nincs minden rokoni kapcsolat explicit módon megadva. Egy nõi leszármazott gyerekei általában nincsenek feltüntetve. \
A gyerekek a férj egy másik lista 'leszármazotti' elõfordulása után vannak felsorolva. Az apa és anya két elõfordulásának egyesítésével \
lesznek a gyerekek az egyesített apa és anya gyerekei.\r\n\
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
a hibák miatt nem ismerjük fel, hogy két bejegyzés ugyanannak az embernek a bejegyzése és nem vonjuk össze azokat, vagy hibás adatokra építve \
két bejegyzést összevonunk, pedig nem azonos emberhez tartoznak. Mindkét hiba tragikus következményekkel járhat a rokoni kapcsolatokra. \
Ezért az összevonás elõtt igyekezzük a hiányosságokat pótolni, a hibákat kijavítani. \
Ehhez segítséget nyújtanak az 'Ellenõrzések összevonás elõtt' menü funkciói. \
Maga az összevonás is feltár hibákat, amiket azonnal megjelenít egy listában. Az itt azonosítható hibákat is javítsuk ki.\
Az azonosításban felhasznált adatok hibái elkerülhettelenül hibás összevonásokat fognak eredményezni!! Ezt kivédeni nem lehet!\r\n\
\r\n\
Azt gondolhatnánk,hogy ha két bejegyzésben a nevek, születési ill. halálozási dátumok ismertek és azonosak, akkor semmi baj nem lehet, \
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
	CDisplayInfo dlgU;
	dlgU.m_text = &info;
	dlgU.m_title = L"Egy ember különbözõ bejegyzéseinek vizsgálata, összevonása";
	dlgU.DoModal();

}
