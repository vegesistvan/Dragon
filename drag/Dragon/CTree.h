#pragma once
#include "afxdialogex.h"


// CTree dialog

class CTree : public CDialogEx
{
	DECLARE_DYNAMIC(CTree)

public:
	CTree(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTree();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TREE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
