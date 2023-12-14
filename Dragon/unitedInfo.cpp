#include "unitedEntries.h"
#include "displayInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnInfoUnited()
{
	CString info = L"\r\n\
AZ EGYES�T�SI ELJ�R�S EREDM�NYEI\r\n\
\r\n\
Az adatokban l�v� hib�k �s hi�nyoss�gok hamis �sszevon�sokat eredm�nyezhetnek �s sz�ks�ges �sszevon�sok kimaradhatnak az egyes�t�sb�l.\
Ez�rt hib�s lesz�rmazotti �s felmen�i list�k keletkezhetnek. Ez�rt fontos, hogy �ttekins�k mi t�rt�nt az �sszevon�s sor�n, a felismerhet� \
hib�kat jav�tsuk. Ezt seg�tik az itt megjelentetett list�k, amelyeken betekinthet�nk az egyes�t�si folyamatba.\r\n\
\r\n\
A bejegyz�sek egyes�t�si elj�r�s�nak eredm�nyei 3 text f�jlban keletkeznek, amelyek specifik�ci�ja az adatb�zisban van meg�rizve.\r\n\
A 3 f�jlt egym�sut�n beolvassa, azok tartalm�t list�zza. Az ablak 3 f�l�nek egyik�vel v�laszthat� ki a megn�zni k�v�nt lista, amely \
az al�bbiak egyike lehet.\r\n\
\r\n\
1. Az azonos nev� bejegyz�sek, amelyek k�z�tt vannak egyes�tettek\r\n\
2. Az azonos nev� bejegyz�sek, amelyek k�z�tt nincsenek egyes�tettek\r\n\
3. Azonos nev� bejegyz�sek, amelyek k�z�tt vannak azonos �s ellentmond� adatp�rok\r\n\
\r\n\
A f�lek kiv�laszt�s�val a f�men� �s a leg�rd�l� men� funkci�k is v�ltoznak.\r\n\
\r\n\
Mindh�rom t�bl�nak ugyanolyan oszlopai vannak:\r\n\
Mindh�rom t�bl�ban kereshet�nk az 'ember' vagy az 'anya' oszlopban. A keres�s oszlop�t a bal fels� srokban v�laszthatjuk ki.\n\r\
\r\n\
Az oszlopok jelent�se:\r\n\
\r\n\
loop\taz �sszevon�s iter�ci�j�nak sorsz�ma\r\n\
gr\tcsoport azonos�t�, amibe az �sszevont emberek tartoznak\r\n\
st\tst�tusz: 1-megtartott bejegyz�s, 0-t�r�lt bejegyz�s\r\n\
u\tunited, az ember u sz�m� bejegyz�s �sszevon�sa.\r\n\
s\taz ember el�fordul�sa a GA.html f�jlban(1 - 2 - 3 - 4)\r\n\
\r\n\
rowid\ta bejegyz�s azonos�t�ja, neve, *sz�let�se, +hal�loz�sa\r\n\
rowid\taz apa bejegyz�s�nek azonos�t�ja, neve, *sz�let�se, +hal�loz�sa\r\n\
rowid\taz anya bejegyz�s�nek azonos�t�ja, neve, *sz�let�se, +hal�loz�sq)\r\n\
h�zast�rsak felsorol�sa azonos�t� sz�mukkal, sz�let�si �s hal�loz�si d�tumukkal\r\n\
\r\n\
Mindh�rom t�bl�n�l a bal eg�rgombbal egy sorra kattintva, egy felugr� ablakban megmutatja a bejegyz�s csal�dj�t.\r\n\
\r\n\
AZONOS NEV� BEJEGYZ�SEK, AMELYEK K�Z�TT VANNAK EGYES�TETTEK\r\n\
\r\n\
Az azonos�t�s eredm�ny�t a sorok h�tt�r sz�nez�se is kiemeli:\r\n\
\r\n\
feh�r\tegyik bejegyz�ssel sem azonos\r\n\
sz�nes\taz ezt k�vet� sz�rke h�tter� bejegyz�sekkel azonosnak �rt�kelt �s megtartott bejegyz�s,\r\n\
sz�rke\tt�rlve lettek.\r\n\
\r\n\
Az egyes�tett bejegyz�sek list�j�n csak azok az egyes�t�sek tal�lhat�ak, amelyek az 'ember'-ek azonos�t�s�nak eredm�nyek�nt ker�ltek egyes�t�sre. \
Teh�t nincsenek itt felsorolva azok a sz�l�k, akik az 'ember'-ek azonoss�ga alapj�n lettek egyes�tve, akik nyilv�nval�an azonosak, \
hiszen azonos emeberek sz�lei,\r\n\
A program 2 iter�ci�t v�gez. A t�bl�zatot a 'Sz�r�s men�ponttal lehet sz�rni: az 1. vagy a 2. iter�ci�ra, ill. sz�r�s n�lk�lire.\r\n\
Mint tudjuk, az egyes�t�si elj�r�s az megtratott bejegyz�sbe �temeli a t�rlend� bejegyz�sekb�l azokat az adatokat, amelyek hosszabbak a t�rlend� bejegyz�sben. \
Ezt a list�n az '^^^' karaktersorozattal jelzi a megfelel� mez�ben. Ezeket a megjel�lt mez�ket kereshetj�k az '�tvett adatok' men�pontra t�rt�n� \
ism�telt kattint�ssal.\r\n\
\r\n\
Egy sorra kattintva a leg�rd�l� men�b�l az al�bbiak k�z�tt v�laszthatunk:\r\n\
- Bejegyz�s a htm f�jlban\r\n\
- 2 kijel�lt bejegyz�s a html f�jlban\r\n\
- 2 kijel�lt bejegyz�s rokos�g�nak �sszehasonl�t�sa\r\n\
- 2 kijel�lt bejegyz�s �sszevon�sa\r\n\
- A kijel�lt n�v szerinti sz�r�s\r\n\
\r\n\
Ez ut�bbi funkci�t �gy aj�nlott haszn�lni, hogy el�sz�r sz�rj�k a list�t a 2. iter�ci�ra, majd kij�lj�k a k�v�nt nevet �s h�vjuk \
a 'Kijel�lt n�v szerinti sz�r�s'-t a leg�rd�l� men�b�l. �gy l�tni fogjuk, hogy a kijel�lt nevet az 1. �s a 2. iter�ci�ban is, teh�t \
l�thatjuk, hogy mi t�rt�nt az �sszevon�sokkal a k�t iter�ci�ban.\r\n\
\r\n\
AZONOS NEV� BEJEGYZ�SEK, AMELYEK K�Z�TT NINCSENEK EGYES�T�SEK\r\n\
\r\n\
Itt a leg�rd�l� men�ben �rtelemszer�en csak az az els� k�t men�pont tal�lhat�:\r\n\
- Bejegyz�s megmutat�s a htm f�jlban a NotePad++-ban\r\n\
- 2 kijel�lt bejegyz�s rokos�g�nak �sszehasonl�t�sa\r\n\
\r\n\
AZONOS NEV� BEJEGYZ�SEK, AMELYEK K�Z�TT VANNAK AZONOS �S ELLENTMOND� ADATP�ROK\r\n\
\r\n\
Itt p�ros�val vannak felsorolva az ilyen bejegyz�sek, s�rg�val jel�lve az elt�r� adatokat. Ezek a bejegyz�sek teh�t nem lettek \
�sszewvonva, da nagyon val�sz�n�, hogy ugyanannak az embernek a bejegyz�sei, csak hib�s adatok miatt nem lehett �sszevonni �ket.\
A hib�kat ki kell jav�tani a html f�jlban, �s �jra be kell olvasni majd �sszevonni az adatokat.\r\n\
\r\n\
A a leg�rd�l� men�ben tal�lhat� funkci�k:\r\n\
- Bejegyz�s a htm f�jlban\r\n\
- 2 kijel�lt bejegyz�s rokons�g�nak �sszehasonl�t�sa\r\n\
\r\n\
Ezek a funkci�k seg�ts�get ny�jtanak a htm f�jl jav�t�s�ban.\r\n\
\r\n\
";
	CDisplayInfo dlg;
	dlg.m_text = &info;
	dlg.m_title = L"Bejegyz�sek egyes�t�si elj�r�s�nak erdm�nyeir�l";
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitedEntries::OnInfoUnion()
{
	CString info = L"\
\r\n\
MILYEN BEJEGYZ�SEK VANNAK A GA.HTM F�JLBAN �S MI AZ �SSZEVON�S C�LJA?\r\n\
\r\n\r\n\
A GA.htm f�jl beolvas�sakor minden emberr�l, aki el�fordul a f�jlban, egy bejegyz�s k�sz�l, amely tartalmazza az ember nev�t �s minden, \
a f�jlban megadott inform�ci�t.\r\n\
A GA.htm f�jlban egy ember el�fordulhat lesz�rmazottk�nt, lesz�rmazott h�zast�rsak�nk, a lesz�rmazott h�zast�rs�nak \
sz�l�jek�nt, lesz�rmazott h�zast�rs�nak tov�bbi h�zast�rsak�nt. Mindegyikr�l egy bejegyz�s k�sz�l. Egy embernek teh�t t�bb bejegyz�se is \
lehet. A k�l�nb�z� szerepben el�fordul� bejegyz�sek k�l�nb�z� r�szletess�g�ek, megb�zhat�s�g�ak �s k�l�nb�z� rokoni kapcsolatai vannak \
meghat�rozva. A h�zast�rs sz�l�je �s a h�zast�rs tov�bbi h�zast�rsa �ltal�ban csak egy n�v, minden tov�bbi adat n�lk�l. \
Teh�t az � azonos�t�suk csak a rokoni kapcsolaton kereszt�l lehets�ges, ami viszont szint�n nagyon szeg�nyes, hiszen a sz�l�knek \
csak egy gyereke, a tov�bbi h�zast�rsnak pedig egyik h�zast�rsa van meghat�rozva. \
Az � aznos�t�suk a legproblematikusabb. \
Egy ember k�l�nb�z� bejegyz�seit priorit�si sorrendben k�pzelj�k el, a v�rhat�an legt�bb adattal �s rokons�ggal rendelkez� lesz�rmazott \
bejegyz�se a legmagasabb, 1-es priorit�s�, 2-es a lesz�rmazott h�zast�rsa, 3-as a lesz�rmazott h�zast�rs�nak sz�lei, 4-es a lesz�rmazott \
h�zast�rs�nak tov�bbi h�zast�rsa. Ennek a k�s�bbiekben lesz jelent�s�ge.\r\n\
\r\n\
K�v�natos, hogy az egy emberhez tartoz� bejegyz�seket �sszevonjuk, vagyis minden embernek csak egy bejegyz�se legyen, egy helyen lehessen \
az adatok karbatart�s�t biztons�gosan v�gezni. A k�l�nb�z� rokoni kapcsolatokat pedig ezen bejegyz�sre t�rt�n� hivatkoz�sokkal fejezz�k ki. \
Az �sszevon�s nem csak az�rt fontos, hogy rend legyen. Az �sszevon�s m�sik fontos szerepe a rokoni kapcsolatok teljes fel�p�t�se.\r\n\
\r\n\
A GA.htm f�jlban nincs minden rokoni kapcsolat explicit m�don megadva. Egy n�i lesz�rmazott gyerekei �ltal�ban nincsenek felt�ntetve. \
A gyerekek a f�rj egy m�sik lista 'lesz�rmazotti' el�fordul�sa ut�n vannak felsorolva. Az apa �s anya k�t el�fordul�s�nak egyes�t�s�vel \
lesznek a gyerekek az egyes�tett apa �s anya gyerekei.\r\n\
A kis csal�di, sz�l�-gyerek kapcsolatok csak a lesz�rmazottak k�z�tt van explicit m�don kifejezve, \
a h�zast�rsak k�z�tti testv�ri kapcsolatokat csak az azonos emberek bejegyz�seinek �sszevon�sa �ltal tudjuk fel�p�teni.\r\n\
\r\n\
A GA.htm f�jban gyakran hi�nyos adatok vannak, pl. egy f�rfi lesz�rmazottnak nincs megadva az egyik feles�ge. \
Ha ez a n� szerepel egy m�sik bejegyz�sben is, ahol meg van adva az a f�rje is ahol � nem szerepel h�zast�rsk�nt, akkor a f�rj az �sszevon�ssal \
megkapja a hi�nyz� feles�g�t is. \r\n\
\r\n\
A bejegyz�sek �sszevon�s�val az �sszevont bejegyz�s �tveszi a t�r�lt bejegyz�sekhez tartoz� rokons�got is, a gyerekeket, h�zast�rsakat. \
Az �sszevon�s teh�t egy nagyon kritikus m�velet, amellyel rokoni kapcsolatokat lehet helyre�ll�tani, hib�s �sszevon�ssal pedig felbor�tani.\r\n\
\r\n\r\n\
AZ �SSZEVON�S FELT�TELEI\r\n\
\r\n\r\n\
K�t azonos nev� ember bejegyz�seinek �sszevonhat�s�g�hoz az al�bbi rokons�g nev�t, sz�let�si �s hal�loz�si d�tum�t vizsg�ljuk\r\n\
\r\n\
bejegyz�s\tn�v\tsz�let�s\thal�l\r\n\
apa\t\tn�v\t\sz�let�s\thal�l\r\n\
anya\t\tn�v\tsz�let�s\thal�l\r\n\
h�zast�rs\t\tn�v\tsz�let�s\thal�l\t\tapja: n�v\tsz�let�s\thal�l\t\tanyja: n�v\tsz�let�s\thal�l\r\n\
\r\n\
K�t bejegyz�s fenti adatp�rjainak �sszehasonl�t�sa az al�bbi eredm�nyre vezethet:\r\n\
\r\n\
1. azonos:\tmindk�t adat meg van adva, �s azok azonosak.\r\n\
2. nem �rt�kelhet�:\tegyik vagy egy adat sincs megadva.\r\n\
3. ellentmond�:\tmindk�t adat meg van adva, �s azok nem egyeznek.\r\n\
\r\n\
Hib�tlan adatok eset�n egy ember k�l�nb�z� bejegyz�seiben �s annak rokons�g�ban semmilyen ellentmond� nem lehet, a bejegyz�sek \
azonos�t�sa nagyon egyszer� feladat.\r\n\
\r\n\
Az adatokban azonban hib�k �s hi�nyoss�gok lehetnek, amik az azonos�t�st megnehez�tik. Az �sszevon�s sor�n k�tf�le hib�t k�vethet�nk el, \
a hib�k miatt nem ismerj�k fel, hogy k�t bejegyz�s ugyanannak az embernek a bejegyz�se �s nem vonjuk �ssze azokat, vagy hib�s adatokra �p�tve \
k�t bejegyz�st �sszevonunk, pedig nem azonos emberhez tartoznak. Mindk�t hiba tragikus k�vetkezm�nyekkel j�rhat a rokoni kapcsolatokra. \
Ez�rt az �sszevon�s el�tt igyekezz�k a hi�nyoss�gokat p�tolni, a hib�kat kijav�tani. \
Ehhez seg�ts�get ny�jtanak az 'Ellen�rz�sek �sszevon�s el�tt' men� funkci�i. \
Maga az �sszevon�s is felt�r hib�kat, amiket azonnal megjelen�t egy list�ban. Az itt azonos�that� hib�kat is jav�tsuk ki.\
Az azonos�t�sban felhaszn�lt adatok hib�i elker�lhettelen�l hib�s �sszevon�sokat fognak eredm�nyezni!! Ezt kiv�deni nem lehet!\r\n\
\r\n\
Azt gondolhatn�nk,hogy ha k�t bejegyz�sben a nevek, sz�let�si ill. hal�loz�si d�tumok ismertek �s azonosak, akkor semmi baj nem lehet, \
ha �sszevonjuk a k�t bejegyz�st, hiszen nagyon nagy val�sz�n�s�ggel azok azonos emberhez tartoznak. Ez igaz is, de ha az apa, anya vagy \
a h�zast�rsakban adathib�k vannak, hamis rokoni kapcsolatok j�hetnek l�tre. Cs�b�rb�l v�d�rbe. Nem lehet tudni, hogy mi a helyes d�nt�s.\r\n\
\r\n\
K�t bejegyz�s �sszevonhat�s�g�nak sz�ks�ges felt�tele a fenti adatok ellentmond�s mentess�gee. Ha b�rmilyen ellentmond�st tal�lunk, \
akkor a k�t bejegyz�st nem vonjuk �ssze.\n\
Az �sszevon�shoz - az ellentmond�s mentess�gen k�v�l - m�g sz�ks�ges, hogy a rokons�g neveinek azonoss�g�n t�l legal�bb egy d�tumban \
azonos is legyen a k�t bejegyz�s. A h�zast�rsakn�l csak az azonos nev� h�zast�rsakat vizsg�ljuk, nem k�vetelj�k meg, hogy t�bb \
h�zast�rs eset�n mindk�t bejegyz�shez minden h�zast�rs kapcsol�djon. \
Ezzel az engedm�nnyel lehet�v� tessz�k, hogy a bemeneti GA.htm f�jlban hi�nyz� vagy hi�nyosan megadott h�zast�rs� bejegyz�seket is \
�sszevonhassunk.\r\n\
\r\n\
Minden gondoss�g dac�ra el�fordulhat hiba az �sszevon�sok sor�n. Legink�bb az, hogy azonos ember bejegyz�sit nem vonjuk �ssze. \
Az esetleg elmaradt �sszevon�st ut�lag is el v�gezni: az emberek list�j�n kijel�lt 2 bejegyz�st a leg�rd�l� men�ben tal�lhat �sszevon�s \
m�velettel egyes�thet�nk.\n\
Nem gy�z�m hangs�lyozni, hogy hib�s adatokkal nem lehet hib�tlan �sszevon�st v�gezni. B�rmilyen nagy munka, a hib�k kijav�t�s�nak \
kell megel�znie az �sszevon�st!\r\n\
\r\n\r\n\
AZ EGYES�T�SI ELJ�R�S\r\n\
\r\n\r\n\
Az azonosnak �rt�kelt bejegyz�sek k�z�l a legmagasabb priorit�s� bejegyz�st megtartjuk, a t�rlend�kb�l pedig �tvessz�k azokat az adatokat, \
amelyek a megtartott bejegyz�sben nem voltak megadva, majd t�r�lj�k ezeket. A t�r�lt bejegyz�sekre t�rt�n� hivatkoz�sokat \
(a gyerekek sz�l�kre ill.a h�zast�rsak egym�sra t�rt�n� hivatkoz�sait) �tir�ny�tjuk a megtartott bejegyz�sre.\r\n\
\r\n\
Lesz�rmazottakat semmik�ppen nem egyes�t�nk, mert �k csak egyszer fordulhatnak el�. \
A kett�s keresztnevekb�l csak az els� haszn�ljuk az �sszehasonl�t�shoz. \n\
A d�tumok �sszehasonl�t�s�n�l megengedj�k, hogy a puszt�n �vsz�mmal ill. teljes d�tummal megadott adatp�rokat azonosnak �rt�kelje, \
ha az �vsz�m azonos.\n\
Ha k�t bejegyz�st �sszevonunk, akkor apjaikat �s anyjaikat is �sszevonjuk, hisz nyilv�nval�an �k is azonosak.\r\n\
\r\n\
A program ABC sorrendben v�gigfut az azonos nev� bejegyz�sek vizsg�lat�n, �sszevon�s�n, majd el�lr�l kezdi ezt a ciklust. \
Ezt az�rt csin�lja, mert el�fordulhat, hogy 2 azonos nev� bejegyz�s �sszevon�sa az�rt nem t�rt�nik meg az els� ciklusban, \
mert az azonoss� nyilv�n�t�shoz sz�ks�ge lenne azonos nev� h�zast�rsa, de az adathi�ny miatt nincs. \
Azonban m�g az els� ciklusban �sszevonhat azonos nev� bejegyz�st, �s a megtartott bejegyz�s hozza mag�val \
a t�r�lt bejegyz�sek h�zast�rsait is. Ezek az esetek t�bbnyire adat hi�nyra vagy adat hib�ra vezethet�k vissza �s jav�tani kell. \
A m�sodik ciklusban �sszevont bejegyz�seket meg kell vizsg�lni, mi�rt nem t�rt�nt  meg az egyes�t�s az 1. ciklusban.\r\n\
\r\n\
A param�terek k�z�tt megadhatunk egy nevet, akiknek a vizsg�lat�t �s az azonosak egyes�t�s�t �rjuk el�. Ez kis�rletez�re \
szolg�l� funkci�.\r\n\
";
	CDisplayInfo dlgU;
	dlgU.m_text = &info;
	dlgU.m_title = L"Egy ember k�l�nb�z� bejegyz�seinek vizsg�lata, �sszevon�sa";
	dlgU.DoModal();

}
