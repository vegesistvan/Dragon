#pragma once
#define VERSION L"1.3.3"
// Mindig A-tól kezdi a leszármazotti lista generáció kódját

//#define VERSION L"1.3.32"
// Meurgey de Tupigny számozási rendszer választható a leszármazotti listáknál

// Azonos nevû emberek összevonása	
// #define VERSION L"1.2.0"
// Azonso nevû házastársak új azonosítása

//#define VERSION L"1.1.5"
// Azonos házastársak összevonása

// #define VERSION L"1.1.4"
// Leányági leszármazottak listázásánál automatikusan "összeköt", azaz más táblákon is keres

// #define L"1.1.3"
// ANSI és UTF8 kódolású bemeneti fájl

// #define VERSION L"1.1.2"
//	ANSI kódolású html fál

// #define VERSION L"1.1.1"
// A táblafejléc végén lévõ esetleges html formattálást ( <..> ) levágja 


//#define VERSION L"1.0.9"
// a fejlécben sorba ányazott <> kimutatása
// GA fáj beolvasáskor nincs választási lehetõség a felbontás módszerében


//#define VERSION L"1.0.8"
//	Felmenõi listák átalakítása, javítása
//#define VERSION L"1.1.7.1"

// other_names a people táblában
// Javítások.

// Egy ember azonos nevû házastársainak ellenõrzõ listája

// Az azonosítási listán a gyerekek rajta vannak, de nem követelmény az azonosságuk.
// A kettõs keresztnevekben megengedett az _ és - jel

//#define VERSION L"1.1.2.0"
//Az azonos nevû emberek azonosításban a gyerekeket is bevettem

//#define VERSION L"1.1.1.19"
//Házasságok felbontásánél a death és birth substringek  empty-re inicuüóializálása
// Az összevonás csak 1 iterációt csinál. Többször kell meghívni és az adatbázis user_version-jébe teszi az iteráció sorszámát.
// Az azonos emberek összevomnásakor 2 html fájlt készít, egyet az összevonható, egyet a nem összevonható neveknek.
// A SameNameAnd.. listákban a házastársak listáját szétszedi, és csak akkor sárgít, ha nincs közös házastárs.


//#define VERSION L"1.1.1.18"
//[xxxxx család] és [törzs xxxxx] összekötése.


//#define VERSION L"1.1.1.17"
//A leszármazotti listákhoz be lehet állítani, hogy összekösse-e az elágazásokat és a nõk leszármazottait is listázza-e.


//#define VERSION L"1.1.1.16"
// SameNameAnd listákban az apa ás anya hierarchiája is jelölve van

//define VERSION L"1.1.1.15"
//Samename listákban a házastársak hierarchiáje jelölve.


// Gyerekek a CRelations és CSameNameAndListCtrl osztályokban


//#define VERSION L"1.1.13"
// CIdentification bõvítése az azonos emberek összevonásával

//#define VERSION L"1.1.12"
// a sameNameAndd.. listákon a házastársak ABC sorrendben vannak

//#define VERSION L"1.1.11"
// %newname javítása


//#define VERSION L"1.1.10"
// CListCtrl celláinak szinezése a CSameNameAndListCtrl osztályban
// CRokonok osztály létrehozása

//#define VERSION L"1.1.9"
// A házastárs további házastársainak hibáját javítottam.

//#define VERSION L"1.1.8"
// A Z-order kezelése 

//#define VERSION L"1.1.7"
// A tree.bmp-et teljes terjedelméveb ábrázoljuk és program a tree.bmp méretével indul


//#define VERSION L"1.1.6"
// sameNameAnd txt fájlban linNumber és name


//#define VERSION L"1.1.5"
// Tree kép az induló képernyõn és máshol. Apró javítgatások a beolvasásban.

//#define VERSION L"1.1.4"
// A ga.line ablakot szerkeszteni lehet, és updatelni a ga.html fájlt


//#define VERSION L"1.1.3"
// A családnevek módosulásainak kezelése ( % újcsaládnáv )