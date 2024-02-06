#pragma once

#define VERSION L"10.171"

// 10.170 2024.02.06 Lesz�rmazotti sz�lak sorrendje cs�ken� hossz�s�g
//  
// 10.170 2024.02.01 GEDCOM beolvas�sn�l a kis kezd�bet�s el�nevek kezel�se. (Komor�czy NAME eset�n);

// 10.169 2024.02.01 Felmen�kn�l �s lesz�rmazottakn�l kis kezd�bet�s el�n�v opci�. 

// 10.168 2024.01.30 3 f�le CRLF
 
// 10.167 2024.01.29 GEDCOM beolvas�sban a NAME feldolgoz�s igaz�t�sa a Komor�czy.ged-hez
// 
// 10.166 2024.01.28 GEDCOM adatb�zis eset�n a lesz�rmazottak h�v�s�nak jav�t�sa az �j programra.
// 
// 10.165 2024.01.27 <li>,<ol>,<\ol> ut�n CRFL, lesz�rmazotti lista parem�terablak�nak �trendez�se

// 10.164 2024.01.20 Az interakt�v line�ris t�bl�ban a csal�dnevek kiemel�se

// 10.163 2024.01.17 Lesz�rmazotti �s felmen�i list�k a ...list/descendans_x ill. ...list/ascendants_x mapp�ban
// 
// 10.162 2024.01.16 Lesz�rmazotti list�ban a CRLF �s sz�k�z�k rendbet�tele

// 10.161 2024.01.15 Interakt�v line�ris lesz�rmazotti t�bla 
// 
// 10.160 2024.01.11 N�i lesz�ramzottak gyerekeinek csal�dnev�t is kezeli a beolvas�sn�l

// 10.159 2024.01.10 Line�ris l.t�bl�ban a gener�ci�s hossz. ;/1 megsz�ntet�se. Kisebb jav�t�sok

// 10.158 2024.01.09 Line�ris lesz�rmazotti lista sorrendj�nek jav�t�sa (gener�ci�k egyben vannak )
// 
// 10 .157 2024.01.08 Lesz�rm,azotti line�ris t�bl�ban testv�rek egym�s ut�n k�vetkeznek.

// 10.156 2024.01.04 Sz�k�z�k jav�t�sa a lesz�rmazotti list�n. Az �sszevon�s->K�rd�ses bejegyz�sek t�bl�ban sz�nez�s

// 10.155 2024.01.02 �sszevon�skor 4. t�bl�ban vannak ellentmond�s mentes, de nem egyes�thet� bejegyz�sek

// 10.154 2024.01.02 H�zast�rsak sz�let�si �s hal�loz�si  d�tum�nak jav�t�sa a beolvas�skor

// 10.153 2024.01.02 K�zi �sszevon�s jav�t�sa

// 10.152 2024.01.01 A f�nemesi c�m �sszevon�sa

// 10.151 2024.01.XX
// Ismeretlen keresztnevek vizsg�lat�nak, bevitel�nek �talak�t�sa.
// H�zast�rsdak felbont�s�nak �talak�t�sa

// 10.150 2023.12.26
// A k�tf�le �sszevon�s megsz�nt. Ha nincs egyforma adat, akkor k�z�s gyerek azonoss�g�val az �sszevon�st elv�gzi.

// 10.149 2023.12.21
// Lesz�rmazottakr�l 3 lista k�sz�l: SZLUHA, Line�ris, line�ris printable

// 10.148 2023.12.08
// Ribbon jav�tgat�sa


// 10.147 2023.12.17
// Ribbon m�dos�t�sa az adatb�zis forr�sa alapj�n
// 
// 10. 146 2023.12.15
// magyar nyelv� men�
// k�pm�retnek megfelel� ablak megnyit�sa


// 10. 145 2023.12.14
// Doc-View architekt�r�ra �tt�r�s
// Felmen�k rajz�nak ki�r�sa f�jlba.
// Felmen� nev�nek, sz�let�si �s hal�loz�si �v�nek megjelen�t�sa eg�r kattint�sra


// 10.144 
// Felmen�k rajz�ban a potok �sszek�t�se men�b�l.
// A felemen�k list�j�ban a lemaradt le�r�s visszahelyez�se.
// A ritk�tott felmen�i lista grafikus k�pe is elk�sz�lt.
// Bal eg�rgomb dupla kattint�sa egy pontre megjelen�ti a felmen� nev�t


// 10.143 2023.12.01
// A felmen�k grafikus k�perny�je. M�g lehetne sok mouse funlci�t megcsin�lni.

// 10.142 2023.11.27
// A felmen�k CListCtrl t�bl�zatai j�nnek l�tre, onnan lehet a htm f�jlokat k�rni. 
// Felmen�k line�ris html t�bl�j�ban a t�bbsz�ri h�zass�gok feh�r/feket�vel kiemelve.
// Lesz�rmazott gyerekek sorrendje v�laszthat� 



// 10.141   2023.11.xx
// A line�ris felmen�i t�bl�ba "anya" oszlop berak�sa az azonos apa �s k�l�nb�z� anya jelz�s�re.
// A lesz�rmazotti �s felmen�i t�bl�zat m�ret�nek �s a fejl�c m�ret�nek igaz�t�sa az aktu�lis screen felbont�s�hoz
// A t�bl�zatol lesz�rmazotti t�bl�j�t is elk�sz�ti a tabul�lt lista mellett.
// Az "Inform�ci�k->A programv�ltoz�sokr�l" alatt a v�ltoz�sokat mutatja ameg.
// A lesz�rmazotti sz�lak hossz�s�g szerinti rendez�se 1 gener�ci� m�lys�gig.


// 10.140	Nem ismert h�zast�rs "n/a" a felmen�i list�ban
//			V�laszthat�, hogy a priamisban gener�ci�n bel�l monoton sorsz�moz�s vagy Kekule sz�m legyen
//			Felmen�i lista els� elem�nek sorsz�ma 0 helyett 1.
//			Lesz�rmazottak line�ris t�bl�zata is l�trehozza 
// 10.139 Line�ris felmen�i t�bl�zat nyomtathat� verzi�ja

// 10.138 Line�ris felmen�i t�bl�zatban a fejl�c meg�ll�t�sa
// 
// 10.137 Line�ris felmen�i list�ban a fejl�c meg�ll�t�sa
// 
// 10.136 Felmen�i list�ban a rokoni kapcsolatok jav�t�sa. T�bl�zatos megjelen�t�s
// 
// 10-135 Felmen�i lista t�bl�zatban. Referenci�k jav�t�sa. Standard h�zast�rs indexek nincsenek ki�rva. Testv�rek s�rga h�tt�rrel.
//  
// 10-134 Felmen�k lineris list�ja �n�ll� m�velet. Mindkett�t egszerre k�sz�ti. Felmen�k genr�ci�j�nak kiterjeszt�se a kisbet�kre.
// 10-134A Jav�tgat�s
// 10.133 A line�ris felmen�i lista befejez�se
// 10.132 1 anyaindex el�tt ;, ut�tag ut�n opcion�lis CRLF
// 10.131 Ut�tag �s anyaindexek el�tti ; jelz�s elt�vol�tva. CRLF /1 el�tt A felmen�i lista k�sz�t�se beh�z�s n�lk�l. CRLF /1 el�tt
// 10.130 Anya index �s az ut�tag el�tt ; karakter opcion�lis ki�r�sa a lesz�rmazotti list�ban
// 10.129 Lesz�rmazotti �s felmen�i list�ban el� lehet �rni a csal�dn�v bold �r�sm�dj�t
// 10.128 Z�r�jelben �rt keresztn�v elfogad�sa keresztn�vnek.
// 10.127 A le�r�sban l�v� f�nemsi c�m meghat�roz�s�nak m�sodik jav�t�sa
// 10.126 A le�r�sban l�v� f�nemsi c�m meghat�roz�s�nak jav�t�sa
// 10.125 GEDCOMbemenetn�l a NAME-ben megadott n�v sz�tzszed�nek jav�t�sa
// 10.124 GEDCOM bement�l a NAME-ben megadott nevet sz�tszedi, ha a SURN nincs megadva
// 10.123 Felmen�i list�ban az "N N" ut�n csak 1 CRLF
// 10.122 Biuld d�tum jav�t�sa (Biuld->Rebiuld Solution)
// 10.121 Mindent bold-dal �rt ki a lsz�rmazotti list�ban. Jav�tva.
// 10.120 Kezeliazokat a h�zass�gokat, ahol nincs megadva a h�zast�rs, csak a hely/d�tum. A h�zast�rs neve "N N" lesz.
// 10.119 Ism�tl�d� felmen�k sz�nez�s�nek lehet�s�ge, csak az "gr�f", "gr�f," stb. f�nemesi c�mek �r�kl�dnek
// 10.118 �szzevon�sn�l el��rhatjuk, hogy �sszevonjuk-e a csak a h�zast�rs nev�ben egyez� bejegyz�seket.
// 9.117 �sszevon�sn�l a h�zast�rs n�vazonoss�ga nem elegend� 
// 9.116 Ism�tl�d� felmen�k korl�toz�sa �s sz�nez�se
// 9.115 Lesz�rmazott le�r�sa el�tt hi�nyz� sz�k�z jav�t�sa
// 9.114 Hi�nyz� sz�k�z�k jav�t�sa a lesz�rmazotti list�n
// 9.113 'properties' t�bla l�trehoz�sa, felt�lt�s �s egyes�t�s program verzi�j�nak meg�rz�se
// 8.0.112  Puszt�n a h�zast�rs nev�nek azonoss�ga is el�g az �sszevon�shoz
// 8..0.111 N�v ut�ni vessz�s le�r�sok kezel�se
// 8.0.110 N�v ut�n megadott b�r�,gr�f,herceg konvert�l�sa br, gr, hg-re.
// 8.0.109 Kiemelt csal�dnevekkel a t�bla-fejl�c is z�r�jelben list�zva van
// 8.0.108 Ha a folytat�s t�bla els� lesz�rmazottj�nak anyaindexe nem 1 volt, akkor hib�s volt. Jav�tva.
// 8.0.107 Kiemelt csal�dnevek m�dos�t�sa n�vv�ltoztat�skor 
// 8.0.106 Kiemelt csal�dnevek m�dos�t�s�nak jav�t�sa
// 8.0.105 Kiemelt csal�dnevek m�dos�t�sa
// 8.0.104 �sszevon�s jav�t�sa nem v�gesistvan felhaszn�l�k eset�n
// H�zast�rsak nem�nek ellen�rz�se �s jav�t�sa
//         GAHTLM eset�n nem nyitja meg a blob f�jlt.
//			CreateSiblings jav�t�sa ( m_mother_id->m_motherIdP
// 8.0.103  CString sex_id v�ltoztat�s stringre
// 
// 8.0.102 A n�i lesz�ramzottak gyerekeinek apa-index�nek megold�sa
// 
// 7.0.101 Adatb�zis strukt�ra v�ltoz�s! Az apa-anya indexekn bevezet�se. Beolvas�s ut�n megnyit�si hiba kijav�t�sa.
// 
// 6.0.100 �sf�ban az ut�tag ki�rr�sa. Lesz�rmazott h�zast�rsa apj�nak f�nemesic�me jav�tva.
// 
// 6.0.99 �sszevon�s sor�n a f�nesi c�m a megtartott bejegyz�s f�nemesi c�me lesz.
// 
// 6.0.98 F�nemesi c�mek csak a lesz�rmazozottakt�l �r�kl�dik, t�ba fejl�cben l�v� c�m figyelmen k�v�l hagyva
// 
// 6.0.97 GEDCOM �s GAHTML f�jlok vizsg�lat�nak �talak�t�sa: a db-hez kapcsolt f�jllal vagy ha nincs akkor a megk�rdezett f�jllal dolgozik
// 
// 
// 6.0.96 �sfa az el�rt gener�ci�ig. Az �sszevon�s azonnal indul, nem k�rdez semmit. Info az �sszevont adatokat tartalmaz� men�ben.
// 
// 6.0.95 �sszevon�shoz nem kell param�tereket megadni

// 6.0.94 K�t bejegyz�s �sszevon�s�hoz a rokons�g neveinek azonioss�g�n k�v�l m�s d�tum adat azonoss�g�ra is sz�ks�g van
//  
// 6.0.93 �sszevon�si param�etr: csup�n h�zast�rs n�vazonoss�ga alapj�n legyen-e �sszevon�s
// 
// 6.0.92 Ism�tl�d� lesz�rmazottak k�z�l az els� megtart�sa jav�tva. 

// 6.0.91 <ol> a lesz�rmazotti sorban, �sszevon�s le�r�s�nak kib�v�t�se

// 6.0.90 Az �sszevont, nem �sszevonhat�, hib�s bejegyz�sek az ablak 3 f�l�vel v�lthat�

// 6.0.89 Az �sszevon�si ellen�rz�sek 3 fokozata. Kib�v�tett Info az �sszevon�sr�l.
// 
// 6.0.88 Info az �sszevon�s param�terei ablakban, ami az �sszevon�s �s a probl�m�k le�r�sa.
// 
// 6.0.87 Laza �s szigor� �sszevon�ai opci�
// 
// 6.0.86 �sszevon�s le�r�s�nak jav�t�sa. 
 
// 6.0.85 GA adatb�zishoz nem tartozik blob f�jl."b�r� �s herceg" t�pus� t�blafej�c kezel�se

// 6.0.84 R�gi �sszek�t�sek �s lesz�rmazottak lev�tele a men�r�l �s m�s apr�s�gok
// 
// 6.0.83 T�bla fejl�cben adott f�nemesi c�m �r�kl�d�se. �sfa list�n a gener�ci� ki�r�sa. 

// 6.0.82 �sszevon�s m�dis�t�sa: azonos nev� h�zast�rs nem el�g az egyes�t�shez
// 
// 6.0.81 �j lesz�rmazotti lista jav�t�sa �s t�bl�k �j lesz�rmazotti list�ja
// 
// 6.0.80 Notepad++ install�l�s�t elind�tja, ha nincs notepad.exe
// 6.0.79 Notepad++ install�l�s�t elind�tja, ha nincs notepad.exe
// 
// 6.0.78 Noteped++ hi�ny�t jelti, de megy tov�bb.
// 
// 6.0.77 Notepad-et a registry-b�l vweszi ki
// 
// 6.0.76 NN-el val� kit�lt�s jav�t�sa a femenl�i list�ban
// 
// 6.0.75 NN helyettes�t�s jav�t�sa. M�g vannak hi�nyoss�gok
// 
// 6.0.74 Ismeretlen felmen�ket NN-el helyettes�ti a list�n
// 
// 6.0.73 �j lesz�rmazotti lista v�laszthat� Ism�tl�d� lesz�ramazottak list�z�s�nak �j opci�i.

// �j lesz�rmazotti lista, egyel�re kis�rlet. 
// El�sz�r elk�sz�ti a lesz�rmazottak rowid vektor�t, �s ut�na kist�zza �ket.
// 
// 6.0.71 Hib�s adatb�zis kijel�l�s megnyit�s�nak jav�t�sa
// 
// 6.0.70 htm beolvas�sa ut�n az adatb�zis spec jav�t�sa
// 
// 6.0.69  Lesz�rmazotti lista programj�nak csinos�t�sa
// 
// 6.0.67  Sz�l�k �sszevon�s�nak jav�t�sa
// 
// 6.0.66  Az �sszevonhat� bejegyz�sekell egy�tt a sz�l�k is �sszevon�sra ker�lnek.
// 6.0.65	Felmen�k k�sz�t�se a CRelatives ablakb�l 
//			kb kezel�se a sz�let�sai �s hal�loz�si d�tumokban. 
// 
// 6.0.64 N; csal�dn�v el�tt soha nincsc el�n�v a felmen�k list�j�n
// 
// 6.0.63 N; csal�dn�v el�tt sohga nincs el�n�v a lesz�rmazotti list�ban.
// 
// 6.0.62 El�n�v ki�r�sa a csal�dn�vvel egy�tt a lesz�rmazotti sorban
// 
// 6.0.61 Felemen�i lista elk�sz�lt

// 6.0.60 CAscendants funkci�inak befekjez�se
// 
// 6.0.59 �j felmen�i listra k�sz�t� program (CAscendants)
// 
// 6.0.58 Azonos�that� adatok n�lk�li egyes�t�sek kihagy�sa (ez eddig bizonyos esetkebn megengedett volt)

// 6.0.57 CListCtrlColoredHeader oszt�ly szines fejl�c nevekhez
// 
// 6.0.56 H�zass�g-t�bla �talak�t�sa. Azonos sorsz�m� h�zast�rsak t�bl�j�nak jav�t�sa leg�rd�l� men�b�l

// 6.0.55 T�bb azonos nev� h�zast�rs jelz�se a lesz�rmazotti list�ban. 
// Exception handling kikaocsol�sa a C++-yCode Generation->Enable c++ Exception handling: NO ( el�tte: /EHsc )

// 6.0.54 /ol �j sorban, ut�n�v lev�g�sa sz�mt�l

// 6.0.53 H�zast�rsak beolvas�s�nak jav�t�sa2

// 6.0.52 H�zast�rsak beolvas�s�nak jav�t�sa

// 6.0.51 H�zast�rsak beolvas�s�nak m�dos�t�sa

// 6.0.50 Z�r�jelek list�j�nak sz�r�se
// 
// 6.0.49 Z�r�jelek list�z�s�nak jav�t�sa, m�dos�t�sa

// 6.0.48 Sorsz�mozott hercegek kezel�se

// 6.0.47	isalpha helyett iswalpha az isTitolo-ban. Felmen�k jav�t�sa

// 6.0.46 Mindig r�vid�tett f�nemesi c�mek a csal�dnevek mellett 
// 
// 6.0.45 L�nygyerekek is �r�klik apjuk f�nemesi c�m�t
// 
// 6.0.44 F�nemsi c�mek �r�kl�d�s�nek jav�t�sa
// 
// 6.0.43 Le�r�sban l�v� f�nemesi c�mek jav�t�sa. El�nevek meghat�roz�s�nak jav�t�sa

// 6.0.42 3. �s 4 rang� bejegyz�seket akkor is �sszevon a t�bbiekkel, ha nincs ellentmond�s k�z�tt�k, de h�zast�rsaik k�z�tt van azonos nev�
// 6.0.41 %%% Antall (Antal), Kisjen�i t�pus� t�blafejl�cb�l az el�n�v kiv�te
// 6.0.40 2022.VI.16 t�pus� d�tumok elfogad�sa, konvert�l�sa 2022.06.16-ra.
// 
// 6.0.39 Meghat�rozhattalan nevek list�ja, el�n�v minden t�bla fejl�cben megadhat�. Beolvas�s jav�t�sa
// 
// 6.0.38 El�nevek azonos�t�s�nak jav�t�sa
// 
// 6.0.37 Beolvas�s �tdolgoz�sa
// 
// 6.0.36 Rang az ismeretlen kersztnevek list�j�ban. N�v felbont�s �jra�r�sa (inputGA_person)

// 6.0.35 Ismeretlen kersztnevek t�bl�zat�ban: a htm.sor �br�zol�sa, leg�rd�l� men� v�ltoztat�sa
// 
// 6.0.34 Esk�v�i adatok azonos�t�s�nak �talak�t�sa
// 
// 6.0.32 Ny�ry Eszter dupl�z�d�s�nak megsz�ntet�se
// 
// 6.0.31 H�zass�gk�t�s helye �s ideje meghat�roz�s�nak jav�t�sa
 
// 6.0.30 �sszevon�sn�l az azonoss�g meg�llap�t�s�n�l az apa,nagyapa,h�zast�rs apja nev�nek csak az els� keresztneve sz�m�t. 

// 6.0.29 �sszevon�sn�l a h�zast�rs anyj�nak jav�t�sa
// 
// 6.0.28 H�zast�rsak gyan�s sz�leinek ellen�rz�se, list�z�sa
// 
// 6.0.27 Esk�v�k lev�laszt�s�nak m�dos�t�sa, Ragadv�nynevek a lesz�rmazotti list�ban
// 
// 6.0.26 _(-vel kezd�d� n�v v�ltozatok lev�g�sa �s other-mez�be helyez�se
// 
// 6.0.25 Nevek felbont�s�nak jav�t�sa, X_(Y) csal�dnevek megtart�sa
 
// 6.0.24 A nevek �j felbont�s: inputGA_ProcessNameSubstr
// 
// 6.0.23 Nevek felbon�s�nal jav�t�sa
//  
// 6.0.22 A lesz�rmazotti list�kban kiemelt csal�dnevek m�dos�t�sa. A lista param�terek dialog ablakj�nak �trendez�se.
// 
// 6.0.21 table haeder jav�t�sa lesz�rmazotti list�ban
// 
// 6.0.20 Beolvas�s �talak�t�sa. �j inoutGA_processNameSubstr 
// 6.0.19 beolvas�skor a nevek el�tagjainak jav�t�sa
// 6.0.18 Lesz�rmazottak list�j�ban a sz�l�k "gr-gr" kijav�t�sa
// 6.0.17 Lesz�rmazotti lista param�tereinek �t�r�sa


// 6.0.16 h�zast�rs apja f�nemesi c�m n�lk�l, �s a f�nemesa any 2x ki�r�sa is jav�tva.
// 
// 6.0.15	A folyt sorok �sszekapcsol�sa beolvas�skor t�rt�nik!!
//			Az ismereten keresztnevek leg�rd�l� men�j�b�l h�vhat� a n�v-nyilv�ntart�s


// 5.0.5 '% name' jav�t�sa, ne �rja ki minden sor v�g�n. Lesz�rmazott h�zast�rsainak jav�t�sa: lesz�rmazott �s h�zast�rsai rowid-je egyes�vel n�vekszik
// 5.0.3 PAT pr�ba
// 
// 5.0.2 Tisztogat�s

// 5.0.1 Lesz�rmazotti list�k h�v�s�nak �tdolgoz�sa 
// 
// �j people struktura, v_generation �s vParentsIndex �sszevon�sa, ciklikus A-Z,a_z gener�ci� jel�l�s a lesz�rmazotti list�ban.

// 4.6.6 �j parentIndex sz�m�t�s
// inputGEDCOM 

// GEDCOM f�jl kisbet�vel kezd�d� neveinek nagybet�s�t�se

// 4.6.2 F�nemesi rang opcion�lis ki�r�sa a felmen�k list�j�n
// 
// 4.6.1 Lesz�rmazotti param�ter a gyerekek sorrendj�nek list�z�sa ( beviteli ill. sz�let�si sorrend)
 
// 4.5.14 GEDCOM f�jlb�l beolvasott adatokban a tableNumber = 1;

// 4.5.13 GEDCOM f�jl kiv�laszt�s�mak jav�t�sa
// 
// 4.5.12 Felemen�k addit�v beolvas�sa

// 4.5.6.10 Kozmetika �s egyenes�gi apa �s anyai felemn�k a t�bbivel egy�tt

// 4.5.6.9  Tisztogat�s
// 
// 4.5.6.8 H�zast�rsak �s sz�leik �s h�zast�rsai f�nemesi rangj�nak beolvas�sa �s m�s apr�s�gok

// 4.5.6.7 Jav�t�s
// 
// 4.5.6.6 Felmen�k list�z�s�nak hibajav�t�sa
// 
// 4.5.6.5 vGKR-ben a neml�zet� felmen�k is bejegyz�sre ker�lnek.
// 
// 4.5.6.4 �j Kekule piramis

// 4.5.6.3 �j felmen�i lista elk�sz�lt

// 4.5.6.2 �j felmen�i lista, egyel�re kis�relti �s nem teljes
// 
// 4.5.6.1 Leszu�rmazotti lista be�ll�tott param�tereit meg�rzi. Gen�ci�k sz�m�nak korl�toz�sa
// 
// 4.5.5.1 "Csal�dn�v csupa nagybet�" kiemelve: minden csal�dn�vre vonatkozik
//  
// 4.5.5.0 Lesz�rmazotti param�ter ablak m�dos�t�sa
// 
// 4.5.4.9 H�zast�rsak anyja �s tov�bbi h�zast�rsai is 
// 4.5.4.8 Sz�nes group box �s felirat a lesz�rmazotti param�ter ablakban

// 4.5.4.7 F�nemesi c�mek a le�r�sb�l a GA.html f�jl beolvas�sakor

// 4.5.4.6 h�zast�rsak f�nemesi c�m�t is ki�rja
// 
// 4.5.4.5 CInputGED oszt�ly l�trehoz�sa
// 
// 4.5.4.4 F�nemesi rangok ki�r�s�nak jav�t�sa
// 
// 4.5.4.3 Lesz�rmazottak el�nevei a t�bl�kon kereszt�l �r�kl�dnek
// 
// 4.5.4.2 Program �s az adatb�zis is nyilv�ntartja a default ga.html f�jlspecifik�ci�t.
// 
// h�zast�rsak el� �s ut�tagja norm�l bet�vel
// 
// 4.5.4.0 �szevon�s csak 1 ciklus, ellen�rz�si lehet�sg az �sszevon�s el�tt
// 
// 4.5.3.3 H�zast�rsak sz�ma �sszevon�s el�tt �s ut�n
// 
// 
// 4.5.3.2 H�zast�rs vizsg�lata, ha az azonoss�gi pontsz�m kevesebb, mint a megadott limit.
// 
// 4.5.3.1 Azonos nev�, de nem egyes�tett bejegyz�sek list�ja
// 
// 4.5.1.2 Kisebb jav�t�sok �s az el�n�v �tvitele 2-b�l 1-be.

// 4.5.1.1 Azonos nev�, valamely  adat�ban egyforma, de m�gsem �sszevont bejegyz�sek
// 
// 4.4.9.1 Azonos nev�, sz�let�si �s hal�loz�si d�tumui gyerekek. % name megold�sa
// 
// 4.4.8.2 Posterior norm�l bet�kkel, h�zast�rs ism�tl�d� h�zast�rsai pirossal
// 
// 4.4.8.1 H�zasp�rak ezonoss�g�nak jav�t�sa
// 
//4.4.8.0 �j �sszevon�s jav�t�sa
// 
// 4.4.7.0 �j �sszevon�si algoritmus
// 
// L"4.4.6.0" Sannta Cruz, Bolivia felmont�sa az esk�v�kben


// 4.2.3.1 El�nevek leszed�s�nek m�dos�t�sa a csal�d-t�bla fejl�cekb�l
// �sszevon�skor a h�zast�rsak sz�leinek n�v-azonoss�g�t is ellen�rzi

// 4.2.2.10 2021.11.02 M�gi egyes�ti a puszt�n azonos nev� h�sp�rral rendelkez� azonos nev� embereket.
// 
// 4.2.2.9 2021.10.31 Puszt�n azonos nev� h�zast�rs alapj�n nem egyes�ti az embereket.
// 
// 4.2.2.8 2021.10.31 Ism�tl�d� lesz�rmazottak kihagy�sa opci�
// 
// L"4.2.2.7" 2021.10.31 Kiemelz csal�dnevek mellett a t�bla azonos�t� is list�zva van.
//  
// L"4.2.2.6" 2021.10.30 Csaal�dn�v v�ltoz�s jav�t�sa
// 
// 2021.10.30 A lesz�rmazoztti list�n kiemlt csal�dnekek jav�t�sa

// 2021.10.29 Ism�tl�d� lesz�rmazottak jel�l�se (!)-el �s kiemel�se pirossal 
// 
// L"4.2.2.2"
// 2021.10.28 Lesz�rmazotti lista kiemelt csal�dnevek eset�n hib�san indent�lt, Jav�tva.
// 
// VERSION L"4.2.2.1"
// 2021.10.27 Lesz�rmazotti lista jobb marg�ja �ll�that�. Azonos emberek �sszev�s�ban a
//VERSION L"4.2.2.0"
// 2021.10.26 N�i lesz�rmazottak gyerekeinek csal�dneve az apa neve!!