#pragma once


// CHtmlFile

class CHtmlFile : public CWnd
{
	DECLARE_DYNAMIC(CHtmlFile)
	DECLARE_MESSAGE_MAP()

public:

	CHtmlFile();
	virtual ~CHtmlFile();

	CString htmlPathName;

	void CHtmlFile::GaEditor();
	void CHtmlFile::GaOpen();

	void CHtmlFile::folytLines();
	void CHtmlFile::descendantFile();
	void CHtmlFile::descendantTable();
	void CHtmlFile::descendantLine();


	void CHtmlFile::marriagesFile();
	void CHtmlFile::marriagesTable();
	void CHtmlFile::marriagesLine();
	void CHtmlFile::ssFile();
	void CHtmlFile::ssTable();
	void CHtmlFile::ssLine();

	void CHtmlFile::roundBrackets();
	void CHtmlFile::squareBrackets();
	void CHtmlFile::bracesBrackets();
protected:
	CString htmlFileName;
	CString	htmlFolderPath;
	CString htmlFileTitle;
	
	bool CHtmlFile::getGAHtmlPathName();
};


