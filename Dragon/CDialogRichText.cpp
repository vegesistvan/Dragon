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
A GA.htm f�jlban egy embernek t�bb bejegyz�se is lehets�ges, el�fordulhat lesz�rmazottk�nt, lesz�rmazott h�zast�rsak�nk, a lesz�rmazott h�zast�rs�nak \
sz�l�jek�nt, lesz�rmazott h�zast�rs�nak tov�bbi h�zast�rsak�nt, de az nincs jel�lve, hogy ezek ugyannnak az embernek k�l�nb�z� el�fordul�sai. \
A k�l�nb�z� szerepben el�fordul� bejegyz�sek k�l�nb�z� r�szletess�g�ek �s megb�zhat�s�g�ak. A h�zast�rs sz�l�je �s a h�zast�rs tov�bbi h�zast�rsa \
�ltal�ban csak egy n�v, minden tov�bbi adat n�lk�l. Teh�t az azonos�t�suk csak a rokoni kapcsolaton kereszt�l lehets�ges. Egy ember k�l�nb�z� \
bejegyz�seit priorit�si sorrendben k�pzelj�k el, ami a fenti sorrenddel egyezik meg. Ennek a k�s�bbiekben lesz jelent�s�ge.\r\n\
\r\n\
K�v�natos, hogy ezek a bejegyz�seket �sszevonjuk, vagyis minden embernek csak egy bejegyz�se legyen, egy helyen lehessen az adatok karbatart�s�t \
biztons�gosan v�gezni. A k�l�nb�z� rokoni kapcsolatokat pedig ezen bejegyz�sre t�rt�n� hivatkoz�sokkal fejezz�k ki. \
Az �sszevon�s nem csak az�rt fontos, hogy rend legyen. Az �sszevon�s m�sik fontos szerepe a rokoni kapcsolatok teljes fel�p�t�se.\r\n\
\r\n\
A GA.htm f�jlban nincs minden rokoni kapcsolat explicit m�don megadva. Egy n�i lesz�rmazott gyerekei �ltal�ban nincsenek felt�ntetve. A gyerekek a f�rj \
egy m�sik lista 'lesz�rmazotti' el�fordul�sa ut�n vannak felsorolva. Az apa �s anya k�t el�fordul�s�nak egyes�t�s�vel lesznek a gyerekek az egyes�tett \
apa �s anya gyerekei.\r\n\
A kis csal�di, sz�l�-gyerek kapcsolatok csak a lesz�rmazottak k�z�tt van explicit m�don kifejezve, \
a h�zast�rsak k�z�tti testv�ri kapcsolatokat csak az azonos emberek bejegyz�seinek �sszevon�sa �ltal tudjuk fel�p�teni.\r\n\
\r\n\
A GA.htm f�jban gyakran hi�nyos adatok vannak, pl. egy f�rfi lesz�rmazottnak nincs megadva az egyik feles�ge. \
Ha ez a n� szerepel egy m�sik bejegyz�sben is, ahol meg van adva az a f�rje is ahol � nem szerepel h�zast�rsk�nt, akkor a f�rj az �sszevon�ssal \
megkapja a hi�nyz� feles�g�t is. \r\n\
\r\n\
A bejegyz�sek �sszevon�s�val az �sszevont bejegyz�s �tveszi a t�r�lt bejegyz�sekhez tartoz� rokons�got is, a gyerekeket, h�zast�rsakat. \
Az �sszevon�s teh�t egy nagyon kritikus m�velet, amellyel rokoni kapcsolatokat lehet helyre�ll�tani, hib�s �sszevon�ssal pedig felbor�tani.\r\n\
\r\n\
K�t azonos nev� ember bejegyz�seinek �sszevonhat�s�g�hoz az al�bbi rokons�g nev�t, sz�let�si �s hal�loz�si d�tum�t vizsg�ljuk\r\n\
\r\n\
bejegyz�s\t\tn�v\tsz�let�s\thal�l\r\n\
apa\t\t\tn�v\tsz�let�s\thal�l\r\n\
anya\t\t\tn�v\tsz�let�s\thal�l\r\n\
h�zast�rs\t\tn�v\tsz�let�s\thal�l\t\tapja: n�v\tsz�let�s\thal�l\t\tanyja: n�v\tsz�let�s\thal�l\r\n\
\r\n\
K�t bejegyz�s fenti adatp�rjainak �sszehasonl�t�sa az al�bbi eredm�nyre vezethet:\r\n\
\r\n\
1. azonos:\tmindk�t adat meg van adva, �s azok azonosak.\r\n\
2. nem �rt�kelhet�:\tegyik vagy egy adat sincs megadva.\r\n\
3. ellentmond�:\tmindk�t adat meg van adva, �s azok nem egyeznek.\r\n\
\r\n\
Hib�tlan adatok eset�n egy ember k�l�nb�z� bejegyz�seiben �s annak rokons�g�ban semmilyen ellentmond� adat nincs, a bejegyz�sek \
azonos�t�sa nagyon egyszer� feladat.\r\n\
\r\n\
Az adatokban azonban hib�k �s hi�nyoss�gok lehetnek, amik az azonos�t�st megnehez�tik. Az �sszevon�s sor�n k�tf�le hib�t k�vethet�nk el, \
a hib�k miatt nem ismerj�k fel, hogy k�t bejegyz�s ugyanannak az embernek a bejegyz�se, vagy ugyancsak a hib�k miatt k�t bejegyz�st \
�sszevonunk, pedig nem azonos emberhez tartoznak. Mindk�t hiba tragikus k�vetkezm�nyekkel j�rhat a rokoni kapcsolatokra. \
Ez�rt az �sszevon�s el�tt igyekezz�k a hi�nyoss�gokat p�tolni, a hib�kat kijav�tani. \
Ehhez seg�ts�get ny�jtanak az 'Ellen�rz�sek �sszevon�s el�tt' men� funkci�i. \
Maga az �sszevon�s is felt�r hib�kat, amiket azonnal megjelen�t egy list�ban. Az itt azonos�that� hib�kat is jav�tsuk ki.\
Az azonos�t�sban felhaszn�lt adatok hib�i elker�lhettelen�l hib�s �sszevon�sokat fognak eredm�nyezni!! Ezt kiv�deni nem lehet!\r\n\
\r\n\
Azt gondolhatn�nk,hogy ha k�t bejegyz�sben a nevek, sz�let�si ill. hal�loz�si d�tumok ismertek �s azonosak, akkor semmi baj nem lehet \
ha �sszevonjuk a k�t bejegyz�st, hiszen nagyon nagy val�sz�n�s�ggel azok azonos emberhez tartoznak. Ez igaz is, de ha az apa, anya vagy \
a h�zast�rsakban adathib�k vannak, hamis rokoni kapcsolatok j�hetnek l�tre. Cs�b�rb�l v�d�rbe. Nem lehet tudni, hogy mi a helyes d�nt�s.\r\n\
\r\n\
A felhaszn�l� h�rom �sszevon�si algoritmus k�z�l v�laszthat, a laza, k�zepes �s a szigor� ellnep�rz�s k�z�l.\r\n\
\r\n\
1. Laza ellen�rz�skor ha k�t bejegyz�sben a n�v, sz�let�si �s hal�loz�si d�tum megegyezik, azokat minden tov�bbi vizsg�lat n�lk�l azonos ember \
bejegyz�seinek �rt�kelj�k �s �sszevonjuk �ket. Ezzel elker�lhetj�k, hogy a fel nem haszn�lt adatokban el�fordul� hib�k az �sszevon�st megakad�lyozz�k.\r\n\
\r\n\
2. K�zepes ellen�rz�sn�l m�g ilyenkor is tov�bb vizsg�ljuk a sz�l�ket. Ha a sz�l�k k�z�tt vannak �rt�kelhet� adatok �s nincs ellentmond�s k�z�tt�k, \
akkor azonosnak elfogadjuk a bejegyz�seket.\r\n\
\r\n\
3. Szigor� vizsg�latn�l m�g ekkor is tov�bb megy�nk �s a h�zast�rsakat is �sszehasonl�tjuk. T�bb h�zast�rs eset�n nem k�vetelj�k meg, hogy mindk�t \
bejegyz�shez tartoz� h�zasp�r-p�roknak meglegyen az �rt�kelhet� �s azonos adata, csak annyit, hogy legyen egy nem ellentmond� adatokat tartalmaz� \
azonos nev� h�zasp�r. Ezzel az engedm�nnyel lehet�v� tessz�k, hogy a bemeneti GA.htm f�jlban hi�nyosan megadott h�zast�rs� bejegyz�seket is \
�sszevonhassunk.\r\n\
\r\n\
A laza �s k�zepes ellen�rz�ssel v�gzett �sszevon�s nagyon vesz�lyes lehet, mert a nem ellen�rz�tt rokons�got is megszerzi az �sszevont bejegyz�s \
ami ferobbanthatja a lesz�rmazotti �s felmen�i list�kat. �gy tud egy ember extra h�zast�rsakat szerezni, ak�r azonos nev�, megism�telt h�zast�rsat, \
akiknek valamilyen adatuk nem egyezik, ez�rt nem lettek �sszevonva.\r\n\
\r\n\
A szigor� ellen�rz�snek is megvannak a h�tr�nyai, el�fordulhat, hogy nem egyes�tj�k az azonos emberhez tartoz� bejegyz�seket. \
Ennek ellen�re a szigor� ellen�rz�st javaslom. A t�bbi ink�bb arra val�, hogy belel�ssunk az �sszevon�s rejtelmeibe.\r\n\
Az esetleg elmaradt �sszevon�st ut�lag is el v�gezni: az emberek list�j�n kijel�lt 2 bejegyz�st a leg�rd�l� men�ben tal�lhat �sszevon�s m�velettel \
egyes�thet�nk.\n\
Nem gy�z�m hangs�lyozni, hogy hib�s adatokkal nem lehet hib�tlan �sszevon�st v�gezni. B�rmilyen nagy munka, a hib�k kijav�t�s�nak \
kell megel�znie az �sszevon�st!\r\n\
\r\n\
Az azonosnak �rt�kelt bejegyz�sek k�z�l a legmagasabb priorit�s� bejegyz�st megtartjuk, a t�rlend�kb�l pedig �tvessz�k azokat az adatokat, \
amelyek a megtartott bejegyz�sben nem voltak megadva, majd t�r�lj�k ezeket. A t�r�lt bejegyz�sekre t�rt�n� hivatkoz�sokat \
(a gyerekek sz�l�kre ill.a h�zast�rsak egym�sra t�rt�n� hivatkoz�sait) �tir�ny�tjuk a megtartott bejegyz�sre.\r\n\
\r\n\
Lesz�rmazottakat semmik�ppen nem egyes�t�nk, mert �k csak egyszer fordulhatnak el�. \
A kett�s keresztnevekb�l csak az els� haszn�ljuk az �sszehasonl�t�shoz.\n\
A d�tumok �sszehasonl�t�s�n�l megengedj�k, hogy a puszt�n �vsz�mmal ill. teljes d�tummal megadott adatp�rokat azonosnak �rt�kelje, ha az �vsz�m azonos.\n\
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
