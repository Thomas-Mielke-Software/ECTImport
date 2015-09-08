#pragma once

// ECTImportXCtrl.h : Declaration of the CECTImportXCtrl ActiveX Control class.


// CECTImportXCtrl : See ECTImportXCtrl.cpp for implementation.

class CECTImportXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CECTImportXCtrl)

// Constructor
public:
	CECTImportXCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CECTImportXCtrl();

	CImport* m_pImport;
	CImportParamsList *m_pImportParamsList;
	CDlgImportDescr *m_pImportDlg;
	CDlgAbout *m_pDlgAbout;
	int m_pDoc;

	DECLARE_OLECREATE_EX(CECTImportXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CECTImportXCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CECTImportXCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidInit = 1L
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	void Init(LONG dokID);
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

