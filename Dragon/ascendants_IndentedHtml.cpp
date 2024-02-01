#include "pch.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "ascendants.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// P I R A M I D ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////
void CAscendants::OnClickedPyramid()
{
	int i;	// futó index


	_int64 K = m_editK;	// csúcs Kekule száma
	int I;				// csúcs indexe
	int G;				// csúcs genrációja
	CString title;
	m_indent = -1;

	UpdateData(FROMSCREEN);
	saveParameters();
	CDialog::OnCancel();

	// elõírt paraméterek ellenõrzése
	for (i = 0; i < vGKR.size(); ++i)
	{
		if (vGKR.at(i).k == K)
		{
			I = i;
			G = vGKR.at(i).g;
			break;
		}
	}
	if (i == vGKR.size())
	{
		str.Format(L"k=%I64d Kekule számú felmenõ nem ismert!", m_editK);
		AfxMessageBox(str);
		return;
	}

//	if (!m_monoton)
	{
		for (i = 0; i < vGKR.size(); ++i)
		{
			vGKR.at(i).gcnt = vGKR.at(i).k;
		}
	}

	int gMax = G + _wtoi(s_editG);	// maximális listázandó generáció
	gMax = G + _wtoi(s_editG) + 1;	// maximális listázandó generáció

	getPeople(vGKR.at(0).rowid);

	title.Format(L"%s felmenõinek %d generációs piramisa a %I64d Kekule számtól", m_name, m_editG, K);

	CProgressWnd wndP(NULL, title);
	wndP.GoModal();
	wndP.SetPos(0);
	wndP.SetStep(1);
	wndP.SetRange(0, vGKR.size());

	openHtml(title, title);

	m_GPrev = G - 1;
	m_cnt = 0;
	m_count = 0;

	listPeople
	(I);		// csúcs kiírása

	_int64	kMin = K;
	_int64	kMax = K;
	_int64	kLast = K;
	_int64	k;
	int		g;
	int gLast = G;
	int db = 1;
	gLast = G;		// elõzõ listázott felmenõ generációja	

	for (int i = I + 1; i < vGKR.size(); ++i)
	{
		g = vGKR.at(i).g;
		if (g > gMax) break;				// elérte e maximális listázandó generációt
		k = vGKR.at(i).k;					// a következõ felmenõ 
		if (kMin <= k && k <= kMax)			// a generáció kiirandó tartományában van
		{
			while (m_NN && kLast + 1 < k)	// ha a generáción belül luk van az ismeret felmenõkben, akkor azt NN-el tölti ki, ha kell
			{
				listNN(kLast + 1);
				++kLast;
			}
			listPeople(i);
			kLast = k;
		}
		else								// k kívül esik a tartományon, új generációba kell lépni
		{
			while (m_NN && kLast < kMax)	// az elhagyni szándékozott generációt kiegészíti NN-el kMax-ig, ha kell.
			{
				listNN(kLast + 1);
				++kLast;
			}
			if (g == gLast + 1)				// generáció váltás, ha nincs generáció váltás egyszerõen kihagyja	
			{
				kMin = kMin * 2;			// az új generáció tartománya
				kMax = kMin + db;
				db = db * 2 + 1;
				kLast = kMin - 1;

				while (m_NN && k > kLast + 1 && kLast < kMax)		// ha az új generáció nem az elsõ felmenõvel kezdõdik, akkor NN-el kezdeni amíg kell
				{
					listNN(kLast + 1);
					++kLast;
				}

				if (kMin <= k && k <= kMax)			// a generáció kiirandó tartományában van
				{
					listPeople(i);
					kLast = vGKR.at(i).k;
				}
			}

		}
		gLast = g;
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	if (m_NN)					// NN-el kell helyettesíteni a nem létezõ felmenõket
	{
		if (K == 1)
		{
			while (m_kLast < kMax)			// az utolsó, nem teljes  generáció befejezése
			{
				listNN(m_kLast + 1);
				++m_kLast;
			}
		}
		++g;

		k = m_kLast + 1;
		for (; g < gMax; ++g)				// az elírt, de nem ismert generációk listázása
		{
			kMin = kMin * 2;				// az új generáció tartománya
			kMax = kMin + db;
			db = db * 2 + 1;
			kLast = kMin - 1;
			k = kMin;
			while (kMin <= k && k <= kMax)
			{
				listNN(k);
				++k;
			}
		}
	}



	for (int j = 0; j < m_GPrev; ++j)  // behúzások törlése
	{
		fwprintf(fh, L"</ul>");
	}
	if (m_term)
	{
		str = L"<br>A felmenõk beolvasása megszakítva!!<br>";
		print0(str);
	}

	fclose(fh);

	if (m_log)
	{
		printvGKR();
		fclose(flvGKR);
		theApp.notepad(fs_vGKR, L"");
	}

	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek felmenõi %s-nek a %d Kekule számtól", m_name, K);
		AfxMessageBox(str);
	}
	else
	{
		if( m_show)
			ShellExecute(NULL, L"open", m_piramidFile, NULL, NULL, SW_SHOWNORMAL);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A  vGKR i. bejegyzését listázza
void CAscendants::listPeople(int i)
{
	CString people;
	CString color;
	CString ss;
	GKR gkr = vGKR.at(i);

	int ix;
	int j;
	int x;
	int g = gkr.g;
	_int64  cnt;
	_int64  gcnt = gkr.gcnt;
	_int64	k = gkr.k;
	TCHAR gen = TCHAR('A') + g;

	ss = indent(g, m_GPrev);

	m_GPrev = g;

	if (g > m_GPrev || m_GPrev == 0)	// jobbra tolás, új sor generációja nagyobb, (csak 1-esével nõhet a generáció)
	{
		++m_indent;
	}

	int abc = 26;

	int cycle = (m_indent - 1) / abc;
	int bias = (m_indent - 1) % abc;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;

	people = getPeopleString(i);
	people.Format(L"%s%I64d. %c&diams;%s", ss, gcnt, gen, (CString)people);


	if (m_checkColor == true)													// ismétlõdõk kékkel
	{
		ix = gkr.cnt - 1;
		if (ix >= 0)
		{
			j = ix % m_numOfColors;
			color = CK::colors[j].color;
			people.Format(L"<font color=\"blue\">%s</font>", (CString)people);
		}
	}
	/*
		else if (m_color == 2)												// csak az elsõ elõfordulás színes
		{

			//	ix = vGKR.at(i).repeated - 1;
			//	ix = vGKR.at(i).root - 1;
			//	if (ix >= 0)
			//	{
			//		j = ix % m_numOfColors;
			//		color = CK::colors[j].color;
			//		str.Format(L"<font color='%s'>%s</font>", color, (CString)str);
			//	}

			//	ix = vGKR.at(i).cnt - 1;
			//	if (ix >= 0)
			//	{
			//		j = ix % m_numOfColors;
			//		color = CK::colors[j].color;
			//		str.Format(L"<font color='%s'>%s-%d</font>", color, (CString)str, j);
			//	}

			//	if( vGKR.at(i).cnt == 1)
			//		str.Format(L"<font color='blue'> %s</font>",(CString)str);

			//	if( vGKR.at(i).cnt == 2)
			//	str.Format(L"<font color='purple'> %s</font>", (CString)str);
		}
	*/
	print0(people);
	++m_cnt;
	m_kLast = k;
	++m_count;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::listNN(_int64 k)
{
	CString ss;
	CString people;
	int		g = getGfromK(k);


	if (g > m_GPrev || m_GPrev == 0)	// jobbra tolás, új sor generációja nagyobb, (csak 1-esével nõhet a generáció)
	{
		++m_indent;
	}

	TCHAR gen;
	int abc = 26;

	int cycle = (m_indent - 1) / abc;
	int bias = (m_indent - 1) % abc;

	if (isEven(cycle))
		gen = TCHAR('A') + bias;
	else
		gen = TCHAR('a') + bias;

	//	TCHAR gen = TCHAR('A') + g;

	ss = indent(g, m_GPrev);
	m_GPrev = g;

	people = L"N N";
	str.Format(L"%s%I64d. %c&diams;%s", ss, k, gen, people);
	print0(str);
	++m_cnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::indent(int gen, int genPrev)
{
	CString ss;


	if (gen == genPrev)			// azonos generáció, azonos behúzás
		ss = L"<li>";
	else if (gen > genPrev)
	{
		ss = m_ulStyle;
		//	ss = L"<ul><li>";			// új generáció, beljebb nyomja, default style
		++m_cnt_ul;
		++m_indent;
	}
	/*
		else if (gen < genPrev)        // régi generáció, kijebb hozza a generáció-különbség-szeresen
		{
			for (int j = 0; j < (genPrev - gen); ++j)  // behúzások törlése
			{
				fwprintf(fh, L"</ul>");
			}
			ss = L"<li>";
		}
	*/
	return ss;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::getPeopleString(int i)
{
	CString people;
	CString spouse_id;
	CString rowid = vGKR.at(i).rowid;

	CString sp(L" ");
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid);
	if (!queryP(m_command)) return(L"");

	CString name;
	CString	str;
	CString str1;
	CString generation;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString posterior;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	int		parentIndex;
	CString comment;
	CString arm;
	CString peer;
	CString csalad;

	generation = m_recP->GetFieldString(DBP_GENERATION);
	title = m_recP->GetFieldString(DBP_TITLE);
	titolo = m_recP->GetFieldString(DBP_TITOLO);
	if (p_titololower)
		titolo = konvTitolo(titolo);

	last_name = m_recP->GetFieldString(DBP_LAST_NAME);
	first_name = m_recP->GetFieldString(DBP_FIRST_NAME);
	posterior = m_recP->GetFieldString(DBP_POSTERIOR);
	birth_place = m_recP->GetFieldString(DBP_BIRTH_PLACE);
	birth_date = m_recP->GetFieldString(DBP_BIRTH_DATE);
	death_place = m_recP->GetFieldString(DBP_DEATH_PLACE);
	death_date = m_recP->GetFieldString(DBP_DEATH_DATE);
	father_id = m_recP->GetFieldString(DBP_FATHER_ID);
	mother_id = m_recP->GetFieldString(DBP_MOTHER_ID);
	parentIndex = _wtoi(m_recP->GetFieldString(DBP_PARENTINDEX));
	comment = m_recP->GetFieldString(DBP_COMMENT);
	arm = m_recP->GetFieldString(DBP_ARM);
	csalad = m_recP->GetFieldString(DBP_CSALAD);
	peer = m_recP->GetFieldString(DBP_PEER);

	//	name.Format(L"%s %s", last_name, first_name);

	str.Empty();
	if (m_peerPrint && !peer.IsEmpty() )
	{
		str = peer;
		str += L" ";
	}
	if (m_bold) str += L"<b>";
	if (last_name != L"N;" && !titolo.IsEmpty()) { str += titolo; str += L" "; }

	if (m_firstBold)
	{
		str += L"<b>";
		str += last_name;
		if (m_firstBold) str += L"</b>";
	}
	else
	{
		str += last_name;
	}
	if (!last_name.IsEmpty()) { str += L" "; }


	if (!first_name.IsEmpty()) { str += first_name; str += L" "; }
	if (!posterior.IsEmpty()) str += posterior;
	str.Trim();
	if (m_bold) str += L"</b>";

	if (!birth_place.IsEmpty() || !birth_date.IsEmpty())
	{
		str += L" *";
		if (!birth_place.IsEmpty()) str += birth_place;
		if (!birth_date.IsEmpty())
		{
			if (!birth_place.IsEmpty()) str += sp;
			str += birth_date;
		}
	}

	if (!death_place.IsEmpty() || !death_date.IsEmpty())
	{
		str += L" +";
		if (!death_place.IsEmpty()) str += death_place;
		if (!death_date.IsEmpty())
		{
			if (!death_place.IsEmpty()) str += sp;
			str += death_date;
		}
	}

	if (!comment.IsEmpty()) { str += " "; str += comment; }

	if (i + 1 < vGKR.size())
	{
		if (vGKR.at(i + 1).k == vGKR.at(i).k + 1)
		{
			spouse_id = vGKR.at(i + 1).rowid;
			str += L" ";
			str += getMarriage(rowid, spouse_id);
			str.TrimRight();
		}
	}
	if (!arm.IsEmpty())
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	if (!csalad.IsEmpty())
	{
		str += L" [";
		str += csalad;
		str += L" család]";
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::konvTitolo(CString titolo)
{
	TCHAR kar;

	for (int i = 0; i < titolo.GetLength(); ++i)
	{
		if (iswupper(titolo[i]))
		{
			kar = towlower(titolo[i]);
			titolo.SetAt(i, kar);
		}
	}
	return titolo;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendants::getMarriage(CString rowid, CString rowidS)
{
	if (rowidS.IsEmpty()) return L"";

	CString marriage;
	CString place;
	CString date;

	m_command.Format(L"SELECT place, date FROM marriages WHERE husband_id ='%s' AND wife_id= '%s'", rowid, rowidS);
	if (!queryM(m_command)) return L"";

	if (!m_recM->RecordsCount()) return marriage;

	place = m_recM->GetFieldString(0);
	date = m_recM->GetFieldString(1);
	if (!place.IsEmpty() && !date.IsEmpty())
		marriage.Format(L"=%s %s", place, date);
	else if (!place.IsEmpty())
		marriage.Format(L"=%s", place);
	else if (!date.IsEmpty())
		marriage.Format(L"=%s", date);

	return marriage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kekule számból a generációt kiszámítani
UINT CAscendants::getGfromK(_int64 k)
{
	_int64 kMin;
	_int64 kMax;
	UINT g;
	for (g = 0; ; ++g)
	{
		kMin = pow(2, g);
		kMax = pow(2, g + 1) - 1;
		if (kMin <= k && k <= kMax)
			break;
	}
	return g;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// D I R E C T   A S C E N D A N T S ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedDirectFatherAscendants()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági apai felmenõi", m_rootName);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();

//	if (!m_monoton)
	{
		for (int i = 0; i < vGKR.size(); ++i)
		{
			vGKR.at(i).gcnt = vGKR.at(i).k;
		}
	}

	getPeople(vGKR.at(0).rowid);
	openHtml(title, title);
	m_indent = -1;
	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 2;
	for (int i = 0; i < vGKR.size(); ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
	{
		ShellExecute(NULL, L"open", m_piramidFile, NULL, NULL, SW_SHOWNORMAL);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedDirectMotherAscendants()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági anyai felmenõi", m_rootName);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();

//	if (!m_monoton)
	{
		for (int i = 0; i < vGKR.size(); ++i)
		{
			vGKR.at(i).gcnt = vGKR.at(i).k;
		}
	}

	getPeople(vGKR.at(0).rowid);
	openHtml(title, title);
	m_indent = -1;
	m_kMin = 1;
	m_kMax = 1;

	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 3;
	for (int i = 0; i < vGKR.size(); ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_piramidFile, NULL, NULL, SW_SHOWNORMAL);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedDirectCouplesF()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági apai felmenõ házastársak", m_name);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();
	getPeople(vGKR.at(0).rowid);
	openHtml(title, title);

//	if (!m_monoton)
	{
		for (int i = 0; i < vGKR.size(); ++i)
		{
			vGKR.at(i).gcnt = vGKR.at(i).k;
		}
	}

	m_indent = -1;
	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 2;
	for (int i = 1; i < vGKR.size() - 1; ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			listPeople(i);
			if (vGKR.at(i + 1).k == k + 1)  // feleség kiírása, ha van
			{
				listPeople(i + 1);
				++i;
			}
			else
			{
				if (m_NN)
					listNN(k + 1);
			}
			kF *= 2;;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_piramidFile, NULL, NULL, SW_SHOWNORMAL);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendants::OnClickedDirectCouplesM()
{
	UpdateData(FROMSCREEN);
	CString title;
	title.Format(L"%s egyenes ági anyai felmenõ házastársak", m_name);
	CProgressWnd wndP(NULL, title);
	wndP.GoModal();

//	if (!m_monoton)
	{
		for (int i = 0; i < vGKR.size(); ++i)
		{
			vGKR.at(i).gcnt = vGKR.at(i).k;
		}
	}


	getPeople(vGKR.at(0).rowid);
	openHtml(title, title);
	m_indent = -1;
	m_cnt = 0;
	m_GPrev = -1;
	listPeople(0);
	_int64 k;
	_int64 kF = 3;
	for (int i = 1; i < vGKR.size() - 1; ++i)					// A K0-tól indul
	{
		k = vGKR.at(i).k;
		if (k == kF)
		{
			//			listPeople1(i);
			listPeople(i);
			if (k != 3 && vGKR.at(i + 1).k == k + 1)  // feleség kiírása, ha van
			{
				listPeople(i + 1);
				++i;
			}
			else
			{
				if (m_NN)
					listNN(k + 1);
			}
			kF *= 2;
		}
	}
	wndP.DestroyWindow();
	fclose(fh);
	if (m_cnt == 1)
	{
		str.Format(L"Nincsenek %s", title);
		AfxMessageBox(str);
	}
	else
		ShellExecute(NULL, L"open", m_piramidFile, NULL, NULL, SW_SHOWNORMAL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendants::openHtml(CString filename, CString title)  // több helyrõl  hívják, ezért paraméter a filename és a title
{

	m_piramidFile.Format(L"%s\\%s_%s.html", m_ascendantsPath, filename, getTimeTag());
	if (!openFileSpec(&fh, m_piramidFile, L"w+")) return false;

	int cnt = 0;

	CString yesno;

	printOnly(L"<!DOCTYPE html>", cnt);
	printOnly(L"<html>", cnt);
	printOnly(L"<head>", cnt);
	printOnly(L"<meta charset=\"UTF-8\">", cnt);
	printOnly(L"<meta name=\"viewport\" content =\"width=device-width, initial-scale=1\">", cnt);
	printOnly(L"<style>", cnt);
	printOnly(L"bY{ background: yellow; }", cnt);
	printOnly(L"bG{ background: #eee;}", cnt);
	printOnly(L"bW{ background: #fff;}", cnt);
	printOnly(L"bN{ background: NavajoWhite;}", cnt);

	printOnly(L"body {", cnt);
	printOnly(L"margin: 0;", cnt);
	//		printOnly(L"font - family: Arial, Helvetica, sans - serif;");
	printOnly(L"}", cnt);

	printOnly(L".top - container {", cnt);
	printOnly(L"background-color: #f1f1f1;", cnt);
	printOnly(L"padding: 30px;", cnt);
	printOnly(L"text-align: center;", cnt);
	printOnly(L"}", cnt);

	printOnly(L".header {", cnt);
	//		printOnly(L"padding: 10px 16px;", cnt);
	printOnly(L"background: LightGray;", cnt);
	//		printOnly(L"background: #555;", cnt);
	printOnly(L"color: Black;", cnt);
	printOnly(L"}", cnt);

	printOnly(L".content {", cnt);
	printOnly(L"padding: 16px;", cnt);
	printOnly(L"}", cnt);

	printOnly(L".sticky {", cnt);
	printOnly(L"position: fixed;", cnt);
	printOnly(L"top : 0;", cnt);
	printOnly(L"width: 100%;", cnt);
	printOnly(L"}", cnt);

	printOnly(L".sticky + .content {", cnt);
	//		printS(L"padding-top: 102px;", cnt);
	printOnly(L"}", cnt);
	printOnly(L"</style>", cnt);
	printOnly(L"</head>", cnt);
	printOnly(L"<body>", cnt);

	printTopContainer(title, cnt);
	if (m_checkColor == true)
	{
		print0(L"Ismétlõdõ felmenõk kék színûek.<br><br>\n");
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
