#pragma once

#include "inputGEDCOM_TAGS.h"


class CGedcomFile : public CWnd
{
	DECLARE_DYNAMIC(CGedcomFile)
	DECLARE_MESSAGE_MAP()
public:
	CGedcomFile();
	virtual ~CGedcomFile();

	void CGedcomFile::listFile();
	void CGedcomFile::listFileTab();
	void CGedcomFile::listHEAD();
	void CGedcomFile::listZero();
	void CGedcomFile::checkTags();
	void CGedcomFile::listChain();
	void CGedcomFile::filterIrregular();
protected:
	
	GEDLINE lxtv;

	std::vector<GEDLINE> v_lxtv;
	CStdioFile gedFile;

	CString gedPathName;
//	CString gedFolderPath;
//	CString gedFileTitle;
//	CString gedFileName;

	CString CGedcomFile::getGedPathName();
	bool CGedcomFile::getGedcomPathName();
	void CGedcomFile::collectTag(CString tag, std::vector<TAGCNT>* v);
	void CGedcomFile::list_vlxtv();

public:
};


