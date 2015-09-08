#pragma once

// ECTImportXPropPage.h : Declaration of the CECTImportXPropPage property page class.


// CECTImportXPropPage : See ECTImportXPropPage.cpp for implementation.

class CECTImportXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CECTImportXPropPage)
	DECLARE_OLECREATE_EX(CECTImportXPropPage)

// Constructor
public:
	CECTImportXPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ECTIMPORTX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

