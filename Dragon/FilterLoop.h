#pragma once
#include "listboxex.h"


// CFilterLoop dialog

class CFilterLoop : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterLoop)

public:
	CFilterLoop(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterLoop();

// Dialog Data
	enum { IDD = IDD_FILTERLOOP };


	CListBoxEx m_ListBox;
	int m_loop;

	std::vector<int>* vLoops;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	


	afx_msg void OnDblclkList();
	virtual BOOL OnInitDialog();
};
