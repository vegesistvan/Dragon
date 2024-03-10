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
MILYEN BEJEGYZ�SEK VANNAK A GA.HTM F�JLBAN �S MI AZ �SSZEVON�S C�LJA?\r\n\
\r\n\r\n\
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
a hib�k miatt nem ismerj�k fel, hogy k�t bejegyz�s ugyanannak az embernek a bejegyz�se �s nem vonjuk �ssze azokat, vagy k�t bejegyz�st \
�sszevonunk hib�s adatok miatt, pedig nem azonos emberhez tartoznak. Mindk�t hiba tragikus k�vetkezm�nyekkel j�rhat a rokoni kapcsolatokra. \
Ez�rt az �sszevon�s el�tt igyekezz�k a hi�nyoss�gokat p�tolni, a hib�kat kijav�tani. \
Ehhez seg�ts�get ny�jtanak az 'Ellen�rz�sek �sszevon�s el�tt' men� funkci�i. \
Maga az �sszevon�s is felt�r hib�kat, amiket azonnal megjelen�t egy list�ban. Az itt azonos�that� hib�kat is jav�tsuk ki.\
Az azonos�t�sban felhaszn�lt adatok hib�i elker�lhettelen�l hib�s �sszevon�sokat fognak eredm�nyezni!! Ezt kiv�deni nem lehet!\r\n\
\r\n\
Azt gondolhatn�nk,hogy ha k�t bejegyz�sben a nevek, sz�let�si ill. hal�loz�si d�tumok ismertek �s azonosak, akkor semmi baj nem lehet \
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
	CDisplayInfo dlg;
	dlg.m_text = &info;
	dlg.m_title = L"Egy ember k�l�nb�z� bejegyz�seinek vizsg�lata, �sszevon�sa";
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


	ComboAlgoCtrl.AddString(L"Lass�: azonos nev� bejegyz�sek ciklus�ban t�rt�nik a beolvas�s");
	ComboAlgoCtrl.AddString(L"Gyors: azonos nev� bejegyz�sek ");
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
