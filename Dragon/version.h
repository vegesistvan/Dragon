#pragma once

#define VERSION L"10.171"

// 10.170 2024.02.06 Leszármazotti szálak sorrendje csökenõ hosszúság
//  
// 10.170 2024.02.01 GEDCOM beolvasásnál a kis kezdõbetûs elõnevek kezelése. (Komoróczy NAME esetén);

// 10.169 2024.02.01 Felmenõknél és leszármazottaknál kis kezdõbetûs elõnév opció. 

// 10.168 2024.01.30 3 féle CRLF
 
// 10.167 2024.01.29 GEDCOM beolvasásban a NAME feldolgozás igazítása a Komoróczy.ged-hez
// 
// 10.166 2024.01.28 GEDCOM adatbázis esetén a leszármazottak hívásának javítása az új programra.
// 
// 10.165 2024.01.27 <li>,<ol>,<\ol> után CRFL, leszármazotti lista pareméterablakának átrendezése

// 10.164 2024.01.20 Az interaktív lineáris táblában a családnevek kiemelése

// 10.163 2024.01.17 Leszármazotti és felmenõi listák a ...list/descendans_x ill. ...list/ascendants_x mappában
// 
// 10.162 2024.01.16 Leszármazotti listában a CRLF és szóközök rendbetétele

// 10.161 2024.01.15 Interaktív lineáris leszármazotti tábla 
// 
// 10.160 2024.01.11 Nõi leszáramzottak gyerekeinek családnevét is kezeli a beolvasásnál

// 10.159 2024.01.10 Lineáris l.táblában a generációs hossz. ;/1 megszüntetése. Kisebb javítások

// 10.158 2024.01.09 Lineáris leszármazotti lista sorrendjének javítása (generációk egyben vannak )
// 
// 10 .157 2024.01.08 Leszárm,azotti lineáris táblában testvérek egymás után következnek.

// 10.156 2024.01.04 Szóközök javítása a leszármazotti listán. Az összevonás->Kérdéses bejegyzések táblában színezés

// 10.155 2024.01.02 Összevonáskor 4. táblában vannak ellentmondás mentes, de nem egyesíthetõ bejegyzések

// 10.154 2024.01.02 Házastársak születési és halálozási  dátumának javítása a beolvasáskor

// 10.153 2024.01.02 Kézi összevonás javítása

// 10.152 2024.01.01 A fõnemesi cím összevonása

// 10.151 2024.01.XX
// Ismeretlen keresztnevek vizsgálatának, bevitelének átalakítása.
// Házastársdak felbontásának átalakítása

// 10.150 2023.12.26
// A kétféle összevonás megszünt. Ha nincs egyforma adat, akkor közös gyerek azonosságával az összevonást elvégzi.

// 10.149 2023.12.21
// Leszármazottakról 3 lista készül: SZLUHA, Lineáris, lineáris printable

// 10.148 2023.12.08
// Ribbon javítgatása


// 10.147 2023.12.17
// Ribbon módosítása az adatbázis forrása alapján
// 
// 10. 146 2023.12.15
// magyar nyelvû menü
// képméretnek megfelelõ ablak megnyitása


// 10. 145 2023.12.14
// Doc-View architektúrára áttérés
// Felmenõk rajzának kiírása fájlba.
// Felmenõ nevének, születési és halálozási évének megjelenítésa egér kattintásra


// 10.144 
// Felmenõk rajzában a potok összekötése menübõl.
// A felemenõk listájában a lemaradt leírás visszahelyezése.
// A ritkított felmenõi lista grafikus képe is elkészült.
// Bal egérgomb dupla kattintása egy pontre megjeleníti a felmenõ nevét


// 10.143 2023.12.01
// A felmenõk grafikus képernyõje. Még lehetne sok mouse funlciót megcsinálni.

// 10.142 2023.11.27
// A felmenõk CListCtrl táblázatai jönnek létre, onnan lehet a htm fájlokat kérni. 
// Felmenõk lineáris html táblájában a többszöri házasságok fehér/feketével kiemelve.
// Leszármazott gyerekek sorrendje választható 



// 10.141   2023.11.xx
// A lineáris felmenõi táblába "anya" oszlop berakása az azonos apa és különbözõ anya jelzésére.
// A leszármazotti és felmenõi táblázat méretének és a fejléc méretének igazítása az aktuális screen felbontásához
// A táblázatol leszármazotti tábláját is elkészíti a tabulált lista mellett.
// Az "Információk->A programváltozásokról" alatt a változásokat mutatja ameg.
// A leszármazotti szálak hosszúság szerinti rendezése 1 generáció mélységig.


// 10.140	Nem ismert házastárs "n/a" a felmenõi listában
//			Választható, hogy a priamisban generáción belül monoton sorszámozás vagy Kekule szám legyen
//			Felmenõi lista elsõ elemének sorszáma 0 helyett 1.
//			Leszármazottak lineáris táblázata is létrehozza 
// 10.139 Lineáris felmenõi táblázat nyomtatható verziója

// 10.138 Lineáris felmenõi táblázatban a fejléc megállítása
// 
// 10.137 Lineáris felmenõi listában a fejléc megállítása
// 
// 10.136 Felmenõi listában a rokoni kapcsolatok javítása. Táblázatos megjelenítés
// 
// 10-135 Felmenõi lista táblázatban. Referenciák javítása. Standard házastárs indexek nincsenek kiírva. Testvérek sárga háttérrel.
//  
// 10-134 Felmenõk lineris listája önálló mûvelet. Mindkettõt egszerre készíti. Felmenõk genrációjának kiterjesztése a kisbetûkre.
// 10-134A Javítgatás
// 10.133 A lineáris felmenõi lista befejezése
// 10.132 1 anyaindex elõtt ;, utótag után opcionális CRLF
// 10.131 Utótag és anyaindexek elõtti ; jelzés eltávolítva. CRLF /1 elõtt A felmenõi lista készítése behúzás nélkül. CRLF /1 elõtt
// 10.130 Anya index és az utótag elõtt ; karakter opcionális kiírása a leszármazotti listában
// 10.129 Leszármazotti és felmenõi listában elõ lehet írni a családnév bold írásmódját
// 10.128 Zárójelben írt keresztnév elfogadása keresztnévnek.
// 10.127 A leírásban lévõ fõnemsi cím meghatározásának második javítása
// 10.126 A leírásban lévõ fõnemsi cím meghatározásának javítása
// 10.125 GEDCOMbemenetnél a NAME-ben megadott név szétzszedének javítása
// 10.124 GEDCOM bementél a NAME-ben megadott nevet szétszedi, ha a SURN nincs megadva
// 10.123 Felmenõi listában az "N N" után csak 1 CRLF
// 10.122 Biuld dátum javítása (Biuld->Rebiuld Solution)
// 10.121 Mindent bold-dal írt ki a lszármazotti listában. Javítva.
// 10.120 Kezeliazokat a házasságokat, ahol nincs megadva a házastárs, csak a hely/dátum. A házastárs neve "N N" lesz.
// 10.119 Ismétlõdõ felmenõk színezésének lehetõsége, csak az "gróf", "gróf," stb. fõnemesi címek öröklõdnek
// 10.118 Öszzevonásnál elõírhatjuk, hogy összevonjuk-e a csak a házastárs nevében egyezõ bejegyzéseket.
// 9.117 Összevonásnál a házastárs návazonossága nem elegendõ 
// 9.116 Ismétlõdõ felmenõk korlátozása és színezése
// 9.115 Leszármazott leírása elõtt hiányzó szóköz javítása
// 9.114 Hiányzó szóközök javítása a leszármazotti listán
// 9.113 'properties' tábla létrehozása, feltöltés és egyesítés program verziójának megõrzése
// 8.0.112  Pusztán a házastárs nevének azonossága is elég az összevonáshoz
// 8..0.111 Név utáni vesszõs leírások kezelése
// 8.0.110 Név után megadott báró,gróf,herceg konvertálása br, gr, hg-re.
// 8.0.109 Kiemelt családnevekkel a tábla-fejléc is zárójelben listázva van
// 8.0.108 Ha a folytatás tábla elsõ leszármazottjának anyaindexe nem 1 volt, akkor hibás volt. Javítva.
// 8.0.107 Kiemelt családnevek módosítása névváltoztatáskor 
// 8.0.106 Kiemelt családnevek módosításának javítása
// 8.0.105 Kiemelt családnevek módosítása
// 8.0.104 Összevonás javítása nem végesistvan felhasználók esetén
// Házastársak nemének ellenõrzése és javítása
//         GAHTLM esetén nem nyitja meg a blob fájlt.
//			CreateSiblings javítása ( m_mother_id->m_motherIdP
// 8.0.103  CString sex_id változtatás stringre
// 
// 8.0.102 A nõi leszáramzottak gyerekeinek apa-indexének megoldása
// 
// 7.0.101 Adatbázis struktúra változás! Az apa-anya indexekn bevezetése. Beolvasás után megnyitási hiba kijavítása.
// 
// 6.0.100 Õsfában az utótag kiírrása. Leszármazott házastársa apjának fõnemesicíme javítva.
// 
// 6.0.99 Összevonás során a fõnesi cím a megtartott bejegyzés fõnemesi címe lesz.
// 
// 6.0.98 Fõnemesi címek csak a leszármazozottaktól öröklõdik, tába fejlécben lévõ cím figyelmen kívül hagyva
// 
// 6.0.97 GEDCOM és GAHTML fájlok vizsgálatának átalakítása: a db-hez kapcsolt fájllal vagy ha nincs akkor a megkérdezett fájllal dolgozik
// 
// 
// 6.0.96 Õsfa az elírt generációig. Az összevonás azonnal indul, nem kérdez semmit. Info az összevont adatokat tartalmazó menüben.
// 
// 6.0.95 Összevonáshoz nem kell paramétereket megadni

// 6.0.94 Két bejegyzés összevonásához a rokonság neveinek azoniosságán kívõl más dátum adat azonosságára is szükség van
//  
// 6.0.93 Összevonási paraméetr: csupén házastárs névazonossága alapján legyen-e összevonás
// 
// 6.0.92 Ismétlõdõ leszármazottak közül az elsõ megtartása javítva. 

// 6.0.91 <ol> a leszármazotti sorban, összevonás leírásának kibõvítése

// 6.0.90 Az összevont, nem összevonható, hibás bejegyzések az ablak 3 fülével váltható

// 6.0.89 Az összevonási ellenõrzések 3 fokozata. Kibõvített Info az összevonásról.
// 
// 6.0.88 Info az összevonás paraméterei ablakban, ami az összevonás és a problémák leírása.
// 
// 6.0.87 Laza és szigorú összevonáai opció
// 
// 6.0.86 Összevonás leírásának javítása. 
 
// 6.0.85 GA adatbázishoz nem tartozik blob fájl."báró és herceg" típusú táblafejéc kezelése

// 6.0.84 Régi összekötések és leszármazottak levétele a menürõl és más apróságok
// 
// 6.0.83 Tábla fejlécben adott fõnemesi cím öröklõdése. Õsfa listán a generáció kiírása. 

// 6.0.82 Összevonás módisítása: azonos nevû házastárs nem elég az egyesítéshez
// 
// 6.0.81 Új leszármazotti lista javítása és táblák új leszármazotti listája
// 
// 6.0.80 Notepad++ installálását elindítja, ha nincs notepad.exe
// 6.0.79 Notepad++ installálását elindítja, ha nincs notepad.exe
// 
// 6.0.78 Noteped++ hiányát jelti, de megy tovább.
// 
// 6.0.77 Notepad-et a registry-bõl vweszi ki
// 
// 6.0.76 NN-el való kitöltés javítása a femenlõi listában
// 
// 6.0.75 NN helyettesítés javítása. Még vannak hiányosságok
// 
// 6.0.74 Ismeretlen felmenõket NN-el helyettesíti a listán
// 
// 6.0.73 Új leszármazotti lista választható Ismétlõdõ leszáramazottak listázásának új opciói.

// Új leszármazotti lista, egyelõre kisérlet. 
// Elõször elkészíti a leszármazottak rowid vektorát, és utána kistázza õket.
// 
// 6.0.71 Hibás adatbázis kijelölés megnyitásának javítása
// 
// 6.0.70 htm beolvasása után az adatbázis spec javítása
// 
// 6.0.69  Leszármazotti lista programjának csinosítása
// 
// 6.0.67  Szülõk összevonásának javítása
// 
// 6.0.66  Az összevonható bejegyzésekell együtt a szülõk is összevonásra kerülnek.
// 6.0.65	Felmenõk készítése a CRelatives ablakból 
//			kb kezelése a születésai és halálozási dátumokban. 
// 
// 6.0.64 N; családnév elõtt soha nincsc elõnév a felmenõk listáján
// 
// 6.0.63 N; családnév elõtt sohga nincs elõnév a leszármazotti listában.
// 
// 6.0.62 Elõnév kiírása a családnévvel együtt a leszármazotti sorban
// 
// 6.0.61 Felemenõi lista elkészült

// 6.0.60 CAscendants funkcióinak befekjezése
// 
// 6.0.59 Új felmenõi listra készító program (CAscendants)
// 
// 6.0.58 Azonosítható adatok nélküli egyesítések kihagyása (ez eddig bizonyos esetkebn megengedett volt)

// 6.0.57 CListCtrlColoredHeader osztály szines fejléc nevekhez
// 
// 6.0.56 Házasság-tábla átalakítása. Azonos sorszámú házastársak táblájának javítása legördülõ menübõl

// 6.0.55 Több azonos nevû házastárs jelzése a leszármazotti listában. 
// Exception handling kikaocsolása a C++-yCode Generation->Enable c++ Exception handling: NO ( elõtte: /EHsc )

// 6.0.54 /ol új sorban, utónév levágása számtól

// 6.0.53 Házastársak beolvasásának javítása2

// 6.0.52 Házastársak beolvasásának javítása

// 6.0.51 Házastársak beolvasásának módosítása

// 6.0.50 Zárójelek listájának szûrése
// 
// 6.0.49 Zárójelek listázásának javítása, módosítása

// 6.0.48 Sorszámozott hercegek kezelése

// 6.0.47	isalpha helyett iswalpha az isTitolo-ban. Felmenõk javítása

// 6.0.46 Mindig rövidített fõnemesi címek a családnevek mellett 
// 
// 6.0.45 Lánygyerekek is öröklik apjuk fõnemesi címét
// 
// 6.0.44 Fõnemsi címek öröklõdésének javítása
// 
// 6.0.43 Leírásban lévõ fõnemesi címek javítása. Elõnevek meghatározásának javítása

// 6.0.42 3. és 4 rangú bejegyzéseket akkor is összevon a többiekkel, ha nincs ellentmondás közöttük, de házastársaik között van azonos nevû
// 6.0.41 %%% Antall (Antal), Kisjenõi típusú táblafejlécbõl az elõnév kivéte
// 6.0.40 2022.VI.16 típusú dátumok elfogadása, konvertálása 2022.06.16-ra.
// 
// 6.0.39 Meghatározhattalan nevek listája, elõnév minden tábla fejlécben megadható. Beolvasás javítása
// 
// 6.0.38 Elõnevek azonosításának javítása
// 
// 6.0.37 Beolvasás átdolgozása
// 
// 6.0.36 Rang az ismeretlen kersztnevek listájában. Név felbontás újraírása (inputGA_person)

// 6.0.35 Ismeretlen kersztnevek táblázatában: a htm.sor ábrázolása, legördülõ menü változtatása
// 
// 6.0.34 Esküvõi adatok azonosításának átalakítása
// 
// 6.0.32 Nyáry Eszter duplázódásának megszüntetáse
// 
// 6.0.31 Házasságkötés helye és ideje meghatározásának javítása
 
// 6.0.30 Összevonásnál az azonosság megállapításánél az apa,nagyapa,házastárs apja nevének csak az elsõ keresztneve számít. 

// 6.0.29 Összevonásnál a házastárs anyjának javítása
// 
// 6.0.28 Házastársak gyanús szüleinek ellenõrzése, listázása
// 
// 6.0.27 Esküvõk leválasztásának módosítása, Ragadványnevek a leszármazotti listában
// 
// 6.0.26 _(-vel kezdõdõ név változatok levágása és other-mezõbe helyezése
// 
// 6.0.25 Nevek felbontásának javítása, X_(Y) családnevek megtartása
 
// 6.0.24 A nevek új felbontás: inputGA_ProcessNameSubstr
// 
// 6.0.23 Nevek felbonásánal javítása
//  
// 6.0.22 A leszármazotti listákban kiemelt családnevek módosítása. A lista paraméterek dialog ablakjának átrendezése.
// 
// 6.0.21 table haeder javítása leszármazotti listában
// 
// 6.0.20 Beolvasás átalakítása. Új inoutGA_processNameSubstr 
// 6.0.19 beolvasáskor a nevek elõtagjainak javítása
// 6.0.18 Leszármazottak listájában a szülõk "gr-gr" kijavítása
// 6.0.17 Leszármazotti lista paramétereinek átírása


// 6.0.16 házastárs apja fõnemesi cím nélkül, és a fõnemesa any 2x kiírása is javítva.
// 
// 6.0.15	A folyt sorok összekapcsolása beolvasáskor történik!!
//			Az ismereten keresztnevek legördülõ menüjébõl hívható a név-nyilvántartás


// 5.0.5 '% name' javítása, ne írja ki minden sor végén. Leszármazott házastársainak javítása: leszármazott és házastársai rowid-je egyesével növekszik
// 5.0.3 PAT próba
// 
// 5.0.2 Tisztogatás

// 5.0.1 Leszármazotti listák hívásának átdolgozása 
// 
// Új people struktura, v_generation és vParentsIndex összevonása, ciklikus A-Z,a_z generáció jelölés a leszármazotti listában.

// 4.6.6 Új parentIndex számítás
// inputGEDCOM 

// GEDCOM fájl kisbetûvel kezdõdõ neveinek nagybetûsítése

// 4.6.2 Fõnemesi rang opcionális kiírása a felmenõk listáján
// 
// 4.6.1 Leszármazotti paraméter a gyerekek sorrendjánek listázása ( beviteli ill. születési sorrend)
 
// 4.5.14 GEDCOM fájlból beolvasott adatokban a tableNumber = 1;

// 4.5.13 GEDCOM fájl kiválasztásámak javítása
// 
// 4.5.12 Felemenõk additív beolvasása

// 4.5.6.10 Kozmetika és egyenesági apa és anyai felemnõk a többivel együtt

// 4.5.6.9  Tisztogatás
// 
// 4.5.6.8 Házastársak és szüleik és házastársai fõnemesi rangjának beolvasása és más apróságok

// 4.5.6.7 Javítás
// 
// 4.5.6.6 Felmenõk listázásának hibajavítása
// 
// 4.5.6.5 vGKR-ben a nemlézetõ felmenõk is bejegyzésre kerülnek.
// 
// 4.5.6.4 Új Kekule piramis

// 4.5.6.3 Új felmenõi lista elkészült

// 4.5.6.2 Új felmenõi lista, egyelõre kisérelti és nem teljes
// 
// 4.5.6.1 Leszuármazotti lista beállított paramétereit megõrzi. Genációk számának korlátozása
// 
// 4.5.5.1 "Családnév csupa nagybetû" kiemelve: minden családnévre vonatkozik
//  
// 4.5.5.0 Leszármazotti paraméter ablak módosítása
// 
// 4.5.4.9 Házastársak anyja és további házastársai is 
// 4.5.4.8 Színes group box és felirat a leszármazotti paraméter ablakban

// 4.5.4.7 Fõnemesi címek a leírásból a GA.html fájl beolvasásakor

// 4.5.4.6 házastársak fõnemesi címét is kiírja
// 
// 4.5.4.5 CInputGED osztály létrehozása
// 
// 4.5.4.4 Fõnemesi rangok kiírásának javítása
// 
// 4.5.4.3 Leszármazottak elõnevei a táblákon keresztül öröklõdnek
// 
// 4.5.4.2 Program és az adatbázis is nyilvántartja a default ga.html fájlspecifikációt.
// 
// házastársak elõ és utótagja normál betûvel
// 
// 4.5.4.0 Öszevonás csak 1 ciklus, ellenõrzási lehetõsg az összevonás elõtt
// 
// 4.5.3.3 Házastársak száma összevonás elõtt és után
// 
// 
// 4.5.3.2 Házastárs vizsgálata, ha az azonossági pontszám kevesebb, mint a megadott limit.
// 
// 4.5.3.1 Azonos nevû, de nem egyesített bejegyzések listája
// 
// 4.5.1.2 Kisebb javítások és az elõnév átvitele 2-bõl 1-be.

// 4.5.1.1 Azonos nevõ, valamely  adatában egyforma, de mégsem összevont bejegyzések
// 
// 4.4.9.1 Azonos nevû, születési és halálozási dátumui gyerekek. % name megoldása
// 
// 4.4.8.2 Posterior normál betûkkel, házastárs ismétlõdö házastársai pirossal
// 
// 4.4.8.1 Házaspárak ezonosságának javítása
// 
//4.4.8.0 Új összevonás javítása
// 
// 4.4.7.0 Új összevonási algoritmus
// 
// L"4.4.6.0" Sannta Cruz, Bolivia felmontása az esküvõkben


// 4.2.3.1 Elõnevek leszedésének módosítása a család-tábla fejlécekbõl
// Összevonáskor a házastársak szüleinek név-azonosságát is ellenõrzi

// 4.2.2.10 2021.11.02 Mégi egyesíti a pusztán azonos nevû háspárral rendelkezõ azonos nevû embereket.
// 
// 4.2.2.9 2021.10.31 Pusztán azonos nevû házastárs alapján nem egyesíti az embereket.
// 
// 4.2.2.8 2021.10.31 Ismétlõdõ leszármazottak kihagyása opció
// 
// L"4.2.2.7" 2021.10.31 Kiemelz családnevek mellett a tábla azonosító is listázva van.
//  
// L"4.2.2.6" 2021.10.30 Csaaládnév változás javítása
// 
// 2021.10.30 A leszármazoztti listán kiemlt családnekek javítása

// 2021.10.29 Ismétlõdõ leszármazottak jelölése (!)-el és kiemelése pirossal 
// 
// L"4.2.2.2"
// 2021.10.28 Leszármazotti lista kiemelt családnevek esetén hibásan indentált, Javítva.
// 
// VERSION L"4.2.2.1"
// 2021.10.27 Leszármazotti lista jobb margója állítható. Azonos emberek összevásában a
//VERSION L"4.2.2.0"
// 2021.10.26 Nõi leszármazottak gyerekeinek családneve az apa neve!!