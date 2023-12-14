#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "GEDCOM_typdef.h"
#include "uniteStructures.h"

#include <afxcoll.h>
#include <afxwin.h>



//#define WM_EDIT_INDIVIDUAL WM_USER + 0x1FFA//a Table_firstname-nek küld üzenetet, hogy újra kell listázni az adatbázist
//#define WM_FILL_TABLE WM_USER + 0x1FF9//a Table_firstname-nek küld üzenetet, hogy újra kell listázni az adatbázist

#define WM_MAIN_TITLE WM_USER + 0x1FF9           //a listctrl delete gombbal torteno esemenyerol kuld infot
#define WM_SYNCRONIZE_PEOPLE WM_USER + 0x1FFA 

CString date2date(CString date);
CString convertNameToLower(CString name);
//CString convertNameToUpper(CString name);
bool extract(CString cLine, GEDLINE* lxtv);
bool sortCBySource(const UNITE_ENTRIES& v1, const UNITE_ENTRIES& v2);
void splitPathName(CString pathName, CString* fileTitle, CString* fileName, CString* folderPath);