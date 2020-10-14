#pragma once
#include "listctrlex.h"

/*
A rowid = 69338 ember eltûnt az adatbázisból az azonos nevû házaspárok összevonása után!!!
Az elsõ blokkban õ a törölt, a 4. blokkban õ a megtartott.De már töröltük, így nemlézetõ ember lett!
Az elsõ blokban a megtartott a 266478R ember, a 4.blokkban pedig a 69338RT lenne, de az elsõ blokkban az törlésre került.
Az adatbázisban a 69338R helyettesítjük 266473-al. Ezt a helyettesítést a vSame vektorban is meg kelle tenni!!!! 
Vagyis a 4. blokkban 69338-at átírni 266478-ra uyanazzal a source-al, ha az övé magassabb hierarchiájú, hogy nehogy törlésre kerüljön

// eltûnt: 69338

 1 -1  28433    43064   2  1    69338 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1    43064 L 1  1    69335 Nyáry Antónia                                              Nyáry Zsigmond                 Majláth Klára                  
 1  1 110687   163636 C 1  1   266473 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   163636   2  1   266476 Nyáry Antónia                                              Nyáry Zsigmond                 Majláth Klára                  
 1 -1 113270   167486   2  1   272511 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   167486   5  1   272512 Nyáry Antónia                                                                                                            

 1 -1  49944    74622   3  1   120853 Fischer József                                                                                                           -1    74622   4  1   120854 Orbán Alojzia                                                                                                            
 1  1 110698   163645 E 1  1   266498 Fischer József                 1781.09.03    1865          Fischer József                 Dessewffy Anna                  1   163645   2  1   266501 Orbán Alojzia                  1798.06.23                  Orbán Pál                      Dessewffy Klára                

 1  1 110691   163636 C 1  1   266473 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1   163636   2  1   266482 Perényi Anna                   1725                        Perényi László                 Andrássy Klára                 
 1 -1 113271   167486   2  1   272511 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   167486   5  1   272513 Perényi Anna                                                                                                             

 1 -1   9733    14999   3  1    24044 Fischer József                                                                                                           -1    14999   4  1    24045 Perényi Erzsébet                                                                                                         
 1  1  28435    43064   2  1    69338 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1    43064   5  1    69339 Perényi Erzsébet                                                                                                         
 1 -1 100253   148205   3  1   241820 Fischer József                                                                                                           -1   148205   4  1   241821 Perényi Erzsébet                                                                                                         
 1 -1 100350   148282   2  1   242016 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1   148282 O 1  1   242013 Perényi Erzsébet               1725          1779.06.07    Perényi László                 Andrássy Klára                 


 // eltûnt: 46256

 1 -1  18799    29038   2  1    46257 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                 1    29038 P 1  1    46254 Bossányi Zsófia                1715.07.12    1750.04.10    Bossányi Elek                  Motesiczky Teréz               
 1 -1  24009    36610   3  1    58734 Sigray Károly                                                                                                            -1    36610   4  1    58735 Bossányi Zsófia                                                                                                          
 1 -1  67859   102725   2  1   165131 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   102725   5  1   165132 Bossányi Zsófia                                                                                                          
 1  1  83585   126316 F 1  1   203453 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   126316   2  1   203456 Bossányi Zsófia                1715.07.12    1750.04.10    Bossányi Elek                  Motesiczky Teréz               
 1 -1  86709   130773   2  1   210918 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   130773   5  1   210919 Bossányi Zsófia                                                                                                          

 1 -1  18801    29038   2  1    46257 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1    29038   5  1    46258 Cziráky Kata                                                                                                             
 1 -1  67857   102725   2  1   165131 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                 1   102725 O 1  1   165128 Cziráky Kata                                               Cziráky József                 Barkóczy Borbála               
 1  1  83587   126316 F 1  1   203453 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   126316   2  1   203459 Cziráky Kata                                               Cziráky József                 Barkóczy Borbála               
 1 -1  86710   130773   2  1   210918 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   130773   5  1   210920 Cziráky Kata                                                                                                             

 1 -1  67860   102725   2  1   165131 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   102725   5  1   165133 Szvetics Mária Zsófia                                                                                                    
 1  1  83589   126316 F 1  1   203453 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1   126316   2  1   203462 Szvetics Mária Zsófia          1744.03.15    1784.06.17    Szvetics Jakab                 Lada Mária                     
 1 -1  86707   130773   2  1   210918 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                 1   130773 F 1  1   210915 Szvetics Mária Zsófia          1744.03.15    1784.06.17    Szvetics Jakab                 Lada Mária                     

 1 -1  16930    26373   3  1    41780 Sigray Károly                                                                                                             1    26373   4  1    41781 Szvetics Zsófia                                                                                                          
 1  1  18802    29038   2  1    46257 Sigray Károly                  1716.09.07    1800.07.05    Sigray József                  Gössinger Magda                -1    29038   5  1    46259 Szvetics Zsófia                                                                                                          

// eltûnt 142844

 1 -1  11565    17430   5  1    28341 Akay György                                                                                                               1    17430   2  1    28340 Baán Kata                                                                                                                
 1  1  38645    58764 D 1  1    94270 Akay György                                                Akay Pál                       Kubinyi Mária                  -1    58764   2  1    94274 Baán Kata                                                                                                                
 1 -1  49726    74395   3  1   120380 Akay György                                                                                                              -1    74395   4  1   120381 Baán Kata                                                                                                                
 1 -1  59178    87652   2  1   142844 Akay György                                                Akay Pál                       Kubinyi Mária                  -1    87652   5  1   142845 Baán Kata                                                                                                                
 1 -1  92360   138180   3  1   223985 Akay György                                                                                                              -1   138180   4  1   223986 Baán Kata                                                                                                                

 1 -1  25170    38545   3  1    61645 Akay György                                                                                                              -1    38545   4  1    61646 Justh Kata                                                                                                               
 1  1  59176    87652   2  1   142844 Akay György                                                Akay Pál                       Kubinyi Mária                   1    87652 H 1  1   142841 Justh Kata                                                 Justh András                   Okolicsányi Kata               

 1  1  38643    58764 D 1  1    94270 Akay György                                                Akay Pál                       Kubinyi Mária                   1    58764   2  1    94273 Justh Zsuzsa                                               Justh András                   Okolicsányi Kata               
 1 -1  60812    90227   3  1   146913 Akay György                                                                                                              -1    90227   4  1   146914 Justh Zsuzsa                                                                                                             

 // eltûnt: 81141
 1  1  30583    45896   2  1    74216 Skultéty Jakab                 1690          1751.11.09                                                                   1    45896 O 1  1    74214 Rudnyánszky Kata               1703.01.27    1757.11.09    Rudnyánszky Zsigmond           Egresdy Borbála                
 1 -1  33513    50009   2  1    81141 Skultéty Jakab                 1690          1751.11.09                                                                  -1    50009   5  1    81142 Rudnyánszky Kata                                                                                                         

 1  1  33512    50009   2  1    81141 Skultéty Jakab                 1690          1751.11.09                                                                   1    50009 E 1  1    81140 Tapolcsányi Teréz              1695.06.09    1725.10.21    Tapolcsányi Tamás              Sinkovics Anna                 
 1 -1  46120    70092   3  1   112383 Skultéty Jakab                                                                                                           -1    70092   4  1   112384 Tapolcsányi Teréz                                                                                                        

 //eltûnt: 242073

 0  0 120975   178883 I 1  1   291115 Balassa Gábor                                1702          Balassa András                 Szunyogh Ilona                  0   178883   2  1   291118 Lengyel Erzsébet                             1684.07.05    Lengyel János                  Ghyczy Zsuzsa.                 
 1  1  75730   114869   2  1   184482 Balassa Gábor                                1700          Balassa András                 Bakos Mária                     1   114869 F 1  1   184476 Lengyel Erzsébet                             1684.07.05    Lengyel János                  Ghyczy Zsuzsa                  
 1 -1 100375   148309   2  1   242073 Balassa Gábor                                1700          Balassa András                 Bakos Mária                    -1   148309   5  1   242074 Lengyel Erzsébet                                                                                                         
 1 -1 114925   170035   5  1   276650 Balassa Gábor                                1700                                                                        -1   170035   2  1   276649 Lengyel Erzsébet                             1684.07.05    Lengyel János                  Ghyczy Zsuzsa                  

 1 -1 100244   148198   3  1   241800 Balassa Gábor                                                                                                            -1   148198   4  1   241801 Perényi Anna                                                                                                             
 1  1 100373   148309   2  1   242073 Balassa Gábor                                1700          Balassa András                 Bakos Mária                     1   148309 M 1  1   242070 Perényi Anna                                               Perényi Ferenc                 Kornis Krisztina               

 1 -1  15673    24777   3  1    38907 Balassa Gábor                                                                                                            -1    24777   4  1    38908 Perényi Magdolna                                                                                                         
 1 -1  15705    24806   3  1    38972 Balassa Gábor                                                                                                            -1    24806   4  1    38973 Perényi Magdolna                                                                                                         
 1 -1  16703    26135   3  1    41292 Balassa Gábor                                                                                                            -1    26135   4  1    41293 Perényi Magdolna                                                                                                         
 1 -1  84995   128437   3  1   206887 Balassa Gábor                                                                                                            -1   128437   4  1   206888 Perényi Magdolna                                                                                                         
 1 -1 103854   153159   3  1   250110 Balassa Gábor                                                                                                            -1   153159   4  1   250111 Perényi Magdolna                                                                                                         
 1  1 120977   178883 I 1  1   291115 Balassa Gábor                                1702          Balassa András                 Szunyogh Ilona                  1   178883   2  1   291121 Perényi Magdolna                                           Perényi Ferenc                 Kornis Krisztina               

 // eltzûnt: 69338
 1 -1  28433    43064   2  1    69338 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1    43064 L 1  1    69335 Nyáry Antónia                                              Nyáry Zsigmond                 Majláth Klára                  
 1  1 110687   163636 C 1  1   266473 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   163636   2  1   266476 Nyáry Antónia                                              Nyáry Zsigmond                 Majláth Klára                  
 1 -1 113270   167486   2  1   272511 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   167486   5  1   272512 Nyáry Antónia                                                                                                            

 1 -1  49944    74622   3  1   120853 Fischer József                                                                                                           -1    74622   4  1   120854 Orbán Alojzia                                                                                                            
 1  1 110698   163645 E 1  1   266498 Fischer József                 1781.09.03    1865          Fischer József                 Dessewffy Anna                  1   163645   2  1   266501 Orbán Alojzia                  1798.06.23                  Orbán Pál                      Dessewffy Klára                

 1  1 110691   163636 C 1  1   266473 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1   163636   2  1   266482 Perényi Anna                   1725                        Perényi László                 Andrássy Klára                 
 1 -1 113271   167486   2  1   272511 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1   167486   5  1   272513 Perényi Anna                                                                                                             

 1 -1   9733    14999   3  1    24044 Fischer József                                                                                                           -1    14999   4  1    24045 Perényi Erzsébet                                                                                                         
 1  1  28435    43064   2  1    69338 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet           -1    43064   5  1    69339 Perényi Erzsébet                                                                                                         
 1 -1 100253   148205   3  1   241820 Fischer József                                                                                                           -1   148205   4  1   241821 Perényi Erzsébet                                                                                                         
 1 -1 100350   148282   2  1   242016 Fischer József                 1715          1781.03.31    Fischer Mihály                 Klobusiczky Erzsébet            1   148282 O 1  1   242013 Perényi Erzsébet               1725          1779.06.07    Perényi László                 Andrássy Klára                 

*/
/*
// nem létezik: 2221
 1 -1            1437   2  1     2221 Prusinszky Zsófia                                                                                                        
 1 -1  10348    15741   2  1    25449 Beniczky Imre                  1784.07.18    1822.09.23    Beniczky Márton                Detrich Mária                   1    15741 H 1  1    25446 Prusinszky Zsófia              1795                        Prusinszky László              Okolicsányi Judit              
 1 -1  12901    19684   5  1    31749 Beniczky Imre                                                                                                            -1    19684   2  1    31748 Prusinszky Zsófia              1795                        Prusinszky László              Okolicsányi Judit              


           1     1437   2  1     2221 Prusinszky Zsófia                                                                                                        
 1  1  10139    15455   3  1    24939 Vitális Jónás                                                                                                            -1    15455   4  1    24940 Prusinszky Zsófia                                                                                                        
 1 -1  10161    15488   3  1    24992 Vitális Jónás                                                                                                            -1    15488   4  1    24993 Prusinszky Zsófia                                                                                                        


 */

// CSameCouples dialog

class CSameCouples : public CDialogEx
{
	DECLARE_DYNAMIC(CSameCouples)
	DECLARE_EASYSIZE


public:
	CSameCouples(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameCouples();

// Dialog Data
	enum { IDD = IDD_SAME_COUPLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

	CString m_info;
	CString m_head;
	CString m_description;
	CString m_columns;

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	// házaspár, akiket vizsgálunk;
	CString	_husband;
	CString _wife;

	CString str;
	CString m_command;
	CString m_explanation;
	CString	_contractions;
	CString unitedSpec;
	CString differentSpec;

	FILE* fU;
	FILE* fD;
	int nItem;
	int	m_cnt;
	int m_deleted;

	UINT	ic;			// vCouples indexe

	CString p_fields;
	CString _tag;

	CString m_rowid;
	CString m_sex_id;

	UINT	m_numOfGroups;
	int		m_contracted;
	
//////////////////////////////////////////////////////////	
	CString rowidM_1;
	CString dateM_1;
	

	UINT	group_1;
	int		statusS1_1;
	CString sex_idS1_1;
	CString rowidS1_1;
	CString generationS1_1;
	CString	sourceS1_1;
	CString unitedS1_1;
	CString lineS1_1;
	CString spouse1_1;
	CString birthS1_1;
	CString deathS1_1;
	CString motherS1_1;
	CString fatherS1_1;

	int		statusS2_1;
	CString rowidS2_1;
	CString sex_idS2_1;
	CString generationS2_1;
	CString	sourceS2_1;
	CString unitedS2_1;
	CString lineS2_1;
	CString spouse2_1;
	CString birthS2_1;
	CString deathS2_1;
	CString motherS2_1;
	CString fatherS2_1;

	UINT	group_2;
	CString rowidM_2;
	CString dateM_2;
	int		statusS1_2;
	CString sex_idS1_2;
	CString rowidS1_2;
	CString generationS1_2;
	CString	sourceS1_2;
	CString unitedS1_2;
	CString lineS1_2;
	CString spouse1_2;
	CString birthS1_2;
	CString deathS1_2;
	CString motherS1_2;
	CString fatherS1_2;

	int		statusS2_2;
	CString rowidS2_2;
	CString sex_idS2_2;
	CString generationS2_2;
	CString	sourceS2_2;
	CString unitedS2_2;
	CString lineS2_2;
	CString spouse2_2;
	CString birthS2_2;
	CString deathS2_2;
	CString motherS2_2;
	CString fatherS2_2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CString birth1Ref;
	CString death1Ref;
	CString mother1Ref;
	CString birth2Ref;
	CString death2Ref;
	CString mother2Ref;

	CString m_rowidMother;
	int		_azonos;


	CListCtrlEx m_ListCtrl;


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	std::vector<COUPLES> vCouples;
	std::vector<COUPLES> vSame;
	std::vector<COUPLES> vWife;
	std::vector<CString> vDeleted;

	void openUnited();
	void openDifferent();

	void createHead( CString title );
//	void createColumns();
	void sameSpouses();
	void getSameCouples();
	void listCtrlLine( UINT i );
	void setData( UINT i1, UINT i2 );

	void referenceValues();
	void processSame();
	void listDiff();
	void listUnited();

	UINT getNumOfGroups();
	void contract();
	void printYellow( UINT i, int nItem );

	bool identical( UINT i1, UINT i2 );
	int		birth1();
	int		death1();
	int		mother1( );
	int		father1();

	int		birth2();
	int		death2();
	int		mother2( );
	int		father2();

	void ReplaceSpouse1( CString rowid, CString rowidBy, CString name, int source );
	void ReplaceSpouse2( CString rowid, CString rowidBy, CString name, int source );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
//	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnLineHusband();
//	afx_msg void OnLineWife();

};
