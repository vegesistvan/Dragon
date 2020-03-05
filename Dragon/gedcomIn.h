#pragma once


typedef struct
{
	int		level;
	CString xref;
	CString tag;
	CString value;
}GEDLINE;


typedef struct
{
	TCHAR	*name;
	TCHAR	*type;
//	void   (*foo)( FILE* fl, CString);
	void   (*foo)( int level, CString xref, CString tag, CString value );
}GEDTAGS;


typedef struct
{
	CString refI;
	CString rowid;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString sex;
	CString birth_place;
	CString birth_date;
	CString death_place;
	CString death_date;
	CString comment;
	CString occupation;
	CString	father_id;
	CString mother_id;
	CString source;
	int		mother_index;		// apja hanyadik feleségének gyermeke, ami kiírásra kerül a html-ben
	int		mother_index2;		// apja hanyadik feleségének gyermeke
	int		orderFather;		// apja hagyadik gyermeke
	int		orderMother;		// anyja hanyadik gyermeke
	int		numOfSpouses;
	CString https;
}INDI;

/*
typedef struct
{
	CString refI;
	CString link;
}PHOTOS;
*/

typedef struct
{
	int		cnt;
	CString refF;
	CString refH;
	CString refW;
	CString place;
	CString date;
	int		marriageH;
	int		marriageW;
	int		marriageHAll;
	int		marriageWAll;
	int		numOfChildren;
}FAM; 

typedef struct
{
	int		cnt;
	CString refF;
	CString	refH;
	CString refW;
	CString refC;
	int		numRefI;
	int		mother_index;
	int		mother_index2;
	int		orderFather;
	int		orderMother;
}CHIL;

typedef struct
{
	CString refI;	// INDI rekord xref-je
	CString refF;	// INDI rekord FAMC-re
}INDIFAM;

bool sortByCnt(const FAM &v1, const FAM &v2);
bool multiSort_FAM_refH(const FAM &v1, const FAM &v2);
bool multiSort_FAM_refW(const FAM &v1, const FAM &v2);
bool multiSort_refH(const CHIL &v1, const CHIL &v2);
bool multiSort_refW(const CHIL &v1, const CHIL &v2);
bool sortChil_cnt(const CHIL &v1, const CHIL &v2);


bool sort_refF(const INDIFAM &v1, const INDIFAM &v2);
bool extract( CString cLine, GEDLINE* lxtv );

class CGedcomIn : public CWnd
{
	DECLARE_DYNAMIC(CGedcomIn)

public:
	CGedcomIn();
	virtual ~CGedcomIn();
	void CGedcomIn::listPeople();
	void CGedcomIn::listFamiliesByName();
	void CGedcomIn::checkTags();
	void CGedcomIn::listFAM();
	void CGedcomIn::listINDI();
	void CGedcomIn::listHEAD();
	void CGedcomIn::zeroLevel();
	bool CGedcomIn::gedcomInput();
	void CGedcomIn::tagTable();
	
	void listVindi();

	
	CStdioFile file_ged;
protected:
	DECLARE_MESSAGE_MAP()
	CString str;
	CString m_command;
	
	ULONGLONG fpos;

	std::vector<INDI> v_indi;
	std::vector<FAM> v_fam;
	std::vector<CHIL> v_chil;
	std::vector<INDIFAM> v_indifam;
	std::vector<PHOTOS> vPhotos;
	
	std::vector<GEDLINE> v_lxtv;

	
	CString m_blobFolder;
	CString _refI;
	CString _refF;
	CString _refH;
	CString _refW;
	CString _refC;
	int		_ix;


	int CGedcomIn::gedCHAR( CString fileSpec );
	void CGedcomIn::fillVectors();
	void CGedcomIn::fill_v_indi();
	void CGedcomIn::fill_v_fam();
	void CGedcomIn::sync_fam_indi();

	
	void CGedcomIn::recordINDI( GEDLINE* gl );
	void CGedcomIn::recordFAM( GEDLINE* gl, int cnt );

	UINT CGedcomIn::last1Bellow( int level );
	void CGedcomIn::listZeroRecord( CString tag );
	void CGedcomIn::list( CString fam, int cnt  );
	void CGedcomIn::getName( CString value, INDI* indi );
	CString CGedcomIn::getIndi( CString ref );
	int CGedcomIn::getIndexIndi( CString ref );
	int CGedcomIn::getIndexFam( CString ref );
	void CGedcomIn::insertHttps( CString rowid, CString fileSpec, CString url, int cnt );
	void CGedcomIn::insertFile( CString rowid, CString fileSpec, int primary );

	void CGedcomIn::insertPeople();
	void CGedcomIn::updateParents();
	void CGedcomIn::insertMarriages();
	CString CGedcomIn::getRowid( CString xref );
	CString CGedcomIn::findHref( CString refFam );
	void CGedcomIn::listMultiMarriages();
//	int CGedcomIn::GetInputCode( CString fileSpec );

	void (*foo)(CString );

public:
//	afx_msg void OnGedcomTagtable();
};


//#include "utilities.h"
CString date2date( CString date );



void indi( int level, CString xref, CString tag, CString value );
void abbr( int level, CString xref, CString tag, CString value );
void addr( int level, CString xref, CString tag, CString value );
void adr1( int level, CString xref, CString tag, CString value );
void adr2( int level, CString xref, CString tag, CString value );
void date( int level, CString xref, CString tag, CString value );
void birt( int level, CString xref, CString tag, CString value );
void deat( int level, CString xref, CString tag, CString value );
void surn( int level, CString xref, CString tag, CString value );
void givn( int level, CString xref, CString tag, CString value );
void cont( int level, CString xref, CString tag, CString value );

void name( int level, CString xref, CString tag, CString value );
void fams( int level, CString xref, CString tag, CString value );
void time( int level, CString xref, CString tag, CString value );
void titl( int level, CString xref, CString tag, CString value );
void form( int level, CString xref, CString tag, CString value );
void obje( int level, CString xref, CString tag, CString value );
void npfx( int level, CString xref, CString tag, CString value );
void sex( int level, CString xref, CString tag, CString value );
void file( int level, CString xref, CString tag, CString value );
void sour( int level, CString xref, CString tag, CString value );
void note(int level, CString xref, CString tag, CString value );
void plac( int level, CString xref, CString tag, CString value );
void famc( int level, CString xref, CString tag, CString value );
void chan( int level, CString xref, CString tag, CString value );
void head( int level, CString xref, CString tag, CString value );
void subm( int level, CString xref, CString tag, CString value );
void vers( int level, CString xref, CString tag, CString value );
void charx( int level, CString xref, CString tag, CString value );
void gedc( int level, CString xref, CString tag, CString value );

void fam( int level, CString xref, CString tag, CString value );
void husb( int level, CString xref, CString tag, CString value );
void wife( int level, CString xref, CString tag, CString value );
void chil( int level, CString xref, CString tag, CString value );
void marr( int level, CString xref, CString tag, CString value );

const GEDTAGS tags[] =
{
	L"ABBR",	L"ABBREVIATION", 	&abbr,		// A short name of a title, description, or name.
	L"ADDR",	L"ADDRESS",			&addr,		// The contemporary place, usually required for postal purposes, of an individual, a submitter of information, a repository, a business, a school, or a company.
	L"ADR1",	L"ADDRESS1",		&adr1,		// The first line of an address. 
	L"ADR2",	L"ADDRESS2",		&adr2,		// The second line of an address. 
	L"ADOP",	L"ADOPTION",		NULL,		// Pertaining to creation of a child-parent relationship that does not exist biologically. 
	L"AFN",		L"AFN",				NULL,		// A unique permanent record file number of an individual record stored in Ancestral File. 
	L"AGE",		L"AGE",				NULL,		// The age of the individual at the time an event occurred, or the age listed in the document. 
	L"AGNC",	L"AGANCY",			NULL,		// The institution or individual having authority and/or responsibility to manage or govern. 
	L"ALIA",	L"ALIAS",			NULL,		// An indicator to link different record descriptions of a person who may be the same person. 
	L"ANCE",	L"ANCESTORS",		NULL,		// Pertaining to forbearers of an individual. 
	L"ANCI",	L"ANCES_INTEREST",	NULL,		// Indicates an interest in additional research for ancestors of this individual. (See also DESI.) 
	L"ANUL",	L"ANNULMENT",		NULL,		// Declaring a marriage void from the beginning (never existed).
	L"ASSO",	L"ASSOCIATES",		NULL,		// An indicator to link friends, neighbors, relatives, or associates of an individual. 
	L"AUTH",	L"AUTHOR",			NULL,		// The name of the individual who created or compiled information. 
	L"BAPL",	L"BAPTISM-LDS",		NULL,		// The event of baptism performed at age eight or later by priesthood authority of the LDS Church. (See also BAPM) 
	L"BAPM",	L"BAPTISM",			NULL,		// The event of baptism (not LDS), performed in infancy or later. (See also BAPL, and CHR.) 
	L"BARM",	L"BAR_MITZVAH",		NULL,		// The ceremonial event held when a Jewish boy reaches age 13. 
	L"BASM",	L"BAS_MITZVAH",		NULL,		// The ceremonial event held when a Jewish girl reaches age 13, also known as "Bat Mitzvah." 
	L"BIRT",	L"BIRTH",			&birt,		// The event of entering into life. 
	L"BLES",	L"BLESSING",		NULL,		// A religious event of bestowing divine care or intercession. Sometimes given in connection with a naming ceremony. 
	L"BLOB",	L"BINARY_OBJECT",	NULL,		// A grouping of data used as input to a multimedia system that processes binary data to represent images, sound, and video. 
	L"BURI",	L"BURIAL",			NULL,		// The event of the proper disposing of the mortal remains of a deceased person. 
	L"CALN",	L"CALL_NUMBER",		NULL,		// The number used by a repository to identify the specific items in its collections. 
	L"CAST",	L"CASTE",			NULL,		// The name of an individual's rank or status in society, based on racial or religious differences, or differences in wealth, inherited rank, profession, occupation, etc. 
	L"CAUS",	L"CAUSE",			NULL,		// A description of the cause of the associated event or fact, such as the cause of death. 
	L"CENS",	L"CENSUS",			NULL,		// The event of the periodic count of the population for a designated locality, such as a national or state Census. 
	L"CHAN",	L"CHANGE",			&chan,		// Indicates a change, correction, or modification. Typically used in connection with a DATE to specify when a change in information occurred. 
	L"CHAR",	L"CHARACTER",		&charx,		// An indicator of the character set used in writing this automated information. 
	L"CHIL",	L"CHILD",			&chil,		// The natural, adopted, or sealed (LDS) child of a father and a mother. 
	L"CHR",		L"CHRISTENING",		NULL,		// The religious event (not LDS) of baptizing and/or naming a child. 
	L"CHRA",	L"ADULT_CHRISTENING",	NULL,	// The religious event (not LDS) of baptizing and/or naming an adult person. 
	L"CITY",	L"CITY",			NULL,		// A lower level jurisdictional unit. Normally an incorporated municipal unit. 
	L"CONC",	L"CONCATENATION",	NULL,		// An indicator that additional data belongs to the superior value. The information from the CONC value is to be connected to the value of the superior preceding line without a space and without a carriage return and/or new line character. Values that are split for a CONC tag must always be split at a non-space. If the value is split on a space the space will be lost when concatenation takes place. This is because of the treatment that spaces get as a GEDCOM delimiter, many GEDCOM values are trimmed of trailing spaces and some systems look for the first non-space starting after the tag to determine the beginning of the value. 
	L"CONF",	L"CONFIRMATION",	NULL,		// The religious event (not LDS) of conferring the gift of the Holy Ghost and, among protestants, full church membership. 
	L"CONL",	L"CONFIRMATION_L",	NULL,		// The religious event by which a person receives membership in the LDS Church. 
	L"CONT",	L"CONTINUED",		&cont,		// An indicator that additional data belongs to the superior value. The information from the CONT value is to be connected to the value of the superior preceding line with a carriage return and/or new line character. Leading spaces could be important to the formatting of the resultant text. When importing values from CONT lines the reader should assume only one delimiter character following the CONT tag. Assume that the rest of the leading spaces are to be a part of the value. 
	L"COPR",	L"COPYRIGHT",		NULL,		// A statement that accompanies data to protect it from unlawful duplication and distribution. 
	L"CORP",	L"CORPORATE",		NULL,		// A name of an institution, agency, corporation, or company. 
	L"CREM",	L"CREMATION",		NULL,		// Disposal of the remains of a person's body by fire. 
	L"CTRY",	L"COUNTRY",			NULL,		// The name or code of the country. 
	L"DATA",	L"DATA",			NULL,		// Pertaining to stored automated information. 
	L"DATE",	L"DATE",			&date,		// The time of an event in a calendar format. 
	L"DEAT",	L"DEATH",			&deat,		// The event when mortal life terminates. 
	L"DESC",	L"DESCENDANTS",		NULL,		// Pertaining to offspring of an individual. 
	L"DESI",	L"DESCENDANT_INT",	NULL,		// Indicates an interest in research to identify additional descendants of this individual. (See also ANCI.) 
	L"DEST",	L"DESTINATION",		NULL,		// A system receiving data. 
	L"DIV",		L"DIVORCE",			NULL,		// An event of dissolving a marriage through civil action. 
	L"DIVF",	L"DIVORCE_FILED",	NULL,		// An event of filing for a divorce by a spouse. 
	L"DSCR",	L"PHY_DESCRIPTION",	NULL,		// The physical characteristics of a person, place, or thing. 
	L"EDUC",	L"EDUCATION",		NULL,		// Indicator of a level of education attained. 
	L"EMIG",	L"EMIGRATION",		NULL,		// An event of leaving one's homeland with the intent of residing elsewhere. 
	L"ENDL",	L"ENDOWMENT",		NULL,		// A religious event where an endowment ordinance for an individual was performed by priesthood authority in an LDS temple. 
	L"ENGA",	L"ENGAGEMENT",		NULL,		// An event of recording or announcing an agreement between two people to become married. 
	L"EVEN",	L"EVENT",			NULL,		// A noteworthy happening related to an individual, a group, or an organization. 
	L"FAM",		L"FAMILY",			&fam,		// Identifies a legal, common law, or other customary relationship of man and woman and their children, if any, or a family created by virtue of the birth of a child to its biological father and mother.
	L"FAMC",	L"FAMILY_CHILD",	&famc,		// Identifies the family in which an individual appears as a child. 
	L"FAMF",	L"FAMILY_FILE",		NULL,		// Pertaining to, or the name of, a family file. Names stored in a file that are assigned to a family for doing temple ordinance work. 
	L"FAMS",	L"FAMILY_SPOUSE",	&fams,		// Identifies the family in which an individual appears as a spouse. 
	L"FCOM",	L"FIRST_COMMUNION",	NULL,		// A religious rite, the first act of sharing in the Lord's supper as part of church worship. 
	L"FILE",	L"FILE",			&file,		// An information storage place that is ordered and arranged for preservation and reference. 
	L"FORM",	L"FORMAT",			&form,		// An assigned name given to a consistent format in which information can be conveyed. 
	L"GEDC",	L"GEDCOM",			&gedc,		// Information about the use of GEDCOM in a transmission. 
	L"GIVN",	L"GIVEN_NAME",		&givn,		// A given or earned name used for official identification of a person. 
	L"GRAD",	L"GRADUATION",		NULL,		// An event of awarding educational diplomas or degrees to individuals. 
	L"HEAD",	L"HEADER",			&head,		// Identifies information pertaining to an entire GEDCOM transmission. 
	L"HUSB",	L"HUSBAND",			&husb,		// An individual in the family role of a married man or father. 
	L"IDNO",	L"IDENT_NUMBER",	NULL,		// A number assigned to identify a person within some significant external system. 
	L"IMMI",	L"IMMIGRATION",		NULL,		// An event of entering into a new locality with the intent of residing there. 
	L"INDI",	L"INDIVIDUAL",		&indi,		// A person. 
	L"LANG",	L"LANGUAGE",		NULL,		// The name of the language used in a communication or transmission of information. 
	L"LEGA",	L"LEGATEE",			NULL,		// A role of an individual acting as a person receiving a bequest or legal devise. 
	L"MARB",	L"MARRIAGE_BANN",	NULL,		// An event of an official public notice given that two people intend to marry. 
	L"MARC",	L"MARR_CONTRACT",	NULL,		// An event of recording a formal agreement of marriage, including the prenuptial agreement in which marriage partners reach agreement about the property rights of one or both, securing property to their children. 
	L"MARL",	L"MARR_LICENSE",	NULL,		// An event of obtaining a legal license to marry. 
	L"MARR",	L"MARRIAGE",		&marr,		// A legal, common-law, or customary event of creating a family unit of a man and a woman as husband and wife. 
	L"MARS",	L"MARR_SETTLEMENT",	NULL,		// An event of creating an agreement between two people contemplating marriage, at which time they agree to release or modify property rights that would otherwise arise from the marriage. 
	L"MEDI",	L"MEDIA",			NULL,		// Identifies information about the media or having to do with the medium in which information is stored. 
	L"NAME",	L"NAME",			&name,		// A word or combination of words used to help identify an individual, title, or other item. More than one NAME line should be used for people who were known by multiple names. 
	L"NATI",	L"NATIONALITY",		NULL,		// The national heritage of an individual. 
	L"NATU",	L"NATURALIZATION",	NULL,		// The event of obtaining citizenship. 
	L"NCHI",	L"CHILDREN_COUNT",	NULL,		// The number of children that this person is known to be the parent of (all marriages) when subordinate to an individual, or that belong to this family when subordinate to a FAM_RECORD. 
	L"NICK",	L"NICKNAME",		NULL,		// A descriptive or familiar that is used instead of, or in addition to, one's proper name. 
	L"NMR",		L"MARRIAGE_COUNT",	NULL,		// The number of times this person has participated in a family as a spouse or parent. 
	L"NOTE",	L"NOTE",			&note,		// Additional information provided by the submitter for understanding the enclosing data. 
	L"NPFX",	L"NAME_PREFIX",		&npfx,		// Text which appears on a name line before the given and surname parts of a name. i.e. ( Lt. Cmndr. ) Joseph /Allen/ jr. In this example Lt. Cmndr. is considered as the name prefix portion. 
	L"NSFX",	L"NAME_SUFFIX",		NULL,		// Text which appears on a name line after or behind the given and surname parts of a name. i.e. Lt. Cmndr. Joseph /Allen/ ( jr. ) In this example jr. is considered as the name suffix portion. 
	L"OBJE",	L"OBJECT",			&obje,		// Pertaining to a grouping of attributes used in describing something. Usually referring to the data required to represent a multimedia object, such an audio recording, a photograph of a person, or an image of a document. 
	L"OCCU",	L"OCCUPATION",		NULL,		// The type of work or profession of an individual. 
	L"ORDI",	L"ORDINANCE",		NULL,		// Pertaining to a religious ordinance in general. 
	L"ORDN",	L"ORDINATION",		NULL,		// A religious event of receiving authority to act in religious matters. 
	L"PAGE",	L"PAGE",			NULL,		// A number or description to identify where information can be found in a referenced work. 
	L"PEDI",	L"PEDIGREE",		NULL,		// Information pertaining to an individual to parent lineage chart. 
	L"PHON",	L"PHONE",			NULL,		// A unique number assigned to access a specific telephone. 
	L"PLAC",	L"PLACE",			&plac,		// A jurisdictional name to identify the place or location of an event. 
	L"POST",	L"POSTAL_CODE",		NULL,		// A code used by a postal service to identify an area to facilitate mail handling. 
	L"PROB",	L"PROBATE",			NULL,		// An event of judicial determination of the validity of a will. May indicate several related court activities over several dates. 
	L"PROP",	L"PROPERTY",		NULL,		// Pertaining to possessions such as real estate or other property of interest. 
	L"PUBL",	L"PUBLICATION",		NULL,		// Refers to when and/or were a work was published or created. 
	L"QUAY",	L"QUALITY_OF_DATA",	NULL,		// An assessment of the certainty of the evidence to support the conclusion drawn from evidence. 
	L"REFN",	L"REFERENCE",		NULL,		// A description or number used to identify an item for filing, storage, or other reference purposes. 
	L"RELA",	L"RELATIONSHIP",	NULL,		// A relationship value between the indicated contexts. 
	L"RELI",	L"RELIGION",		NULL,		// A religious denomination to which a person is affiliated or for which a record applies. 
	L"REPO",	L"REPOSITORY",		NULL,		// An institution or person that has the specified item as part of their collection(s). 
	L"RESI",	L"RESIDENCE",		NULL,		// The act of dwelling at an address for a period of time. 
	L"RESN",	L"RESTRICTION",		NULL,		// A processing indicator signifying access to information has been denied or otherwise restricted. 
	L"RETI",	L"RETIREMENT",		NULL,		// An event of exiting an occupational relationship with an employer after a qualifying time period. 
	L"RFN",		L"REC_FILE_NUMBER",	NULL,		// A permanent number assigned to a record that uniquely identifies it within a known file. 
	L"RIN",		L"REC_ID_NUMBER",	NULL,		// A number assigned to a record by an originating automated system that can be used by a receiving system to report results pertaining to that record. 
	L"ROLE",	L"ROLE",			NULL,		// A name given to a role played by an individual in connection with an event. 
	L"SEX",		L"SEX",				&sex,		// Indicates the sex of an individual--male or female. 
	L"SLGC",	L"SEALING_CHILD",	NULL,		// A religious event pertaining to the sealing of a child to his or her parents in an LDS temple ceremony. 
	L"SLGS",	L"SEALING_SPOUSE",	NULL,		// A religious event pertaining to the sealing of a husband and wife in an LDS temple ceremony. 
	L"SOUR",	L"SOURCE",			&sour,		// The initial or original material from which information was obtained. 
	L"SPFX",	L"SURN_PREFIX",		NULL,		// A name piece used as a non-indexing pre-part of a surname. 
	L"SSN",		L"SOC_SEC_NUMBER",	NULL,		// A number assigned by the United States Social Security Administration. Used for tax identification purposes. 
	L"STAE",	L"STATE",			NULL,		// A geographical division of a larger jurisdictional area, such as a State within the United States of America. 
	L"STAT",	L"STATUS",			NULL,		// An assessment of the state or condition of something. 
	L"SUBM",	L"SUBMITTER",		&subm,		// An individual or organization who contributes genealogical data to a file or transfers it to someone else. 
	L"SUBN",	L"SUBMISSION",		NULL,		// Pertains to a collection of data issued for processing. 
	L"SURN",	L"SURNAME",			&surn,		// A family name passed on or used by members of a family. 
	L"TEMP",	L"TEMPLE",			NULL,		// The name or code that represents the name a temple of the LDS Church. 
	L"TEXT",	L"TEXT",			NULL,		// The exact wording found in an original source document. 
	L"TIME",	L"TIME",			&time,		// A time value in a 24-hour clock format, including hours, minutes, and optional seconds, separated by a colon (:). Fractions of seconds are shown in decimal notation. 
	L"TITL",	L"TITLE",			&titl,		// A description of a specific writing or other work, such as the title of a book when used in a source context, or a formal designation used by an individual in connection with positions of royalty or other social status, such as Grand Duke. 
	L"TRLR",	L"TRAILER",			NULL,		// At level 0, specifies the end of a GEDCOM transmission. 
	L"TYPE",	L"TYPE",			NULL,		// A further qualification to the meaning of the associated superior tag. The value does not have any computer processing reliability. It is more in the form of a short one or two word note that should be displayed any time the associated data is displayed. 
	L"VERS",	L"VERSION",			&vers,		// Indicates which version of a product, item, or publication is being used or referenced. 
	L"WIFE",	L"WIFE",			&wife,		// An individual in the role as a mother and/or married woman. 
	L"WILL",	L"WILL",			NULL,		// A legal document treated as an event, by which a person disposes of his or her estate, to take effect after death. The event date is the date the will was signed while the person was alive. (See also PROBate.)
};


