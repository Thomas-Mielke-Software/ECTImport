/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgwewimportdescr.h
// Purpose:     Interface of CDlgNewImportDescr, asks user for name of new import
//              description
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_NEWIMPORTDESCR_H__91DA9871_E7D7_4A65_BD83_F769C94F1991__INCLUDED_)
#define AFX_NEWIMPORTDESCR_H__91DA9871_E7D7_4A65_BD83_F769C94F1991__INCLUDED_

#include "resource.h"
#include "ImportParams.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgNewImportDescr : public CDialog {
  public:
	  CDlgNewImportDescr ( CWnd* pParent = NULL );

    inline void SetCurrentImportParams ( CImportParams* Value ) { m_CurrentImportParams = Value; };
    inline void SetImportDescrList ( CImportParamsList* const Value ) { m_ImportDescrList = Value; };
    
    inline BOOL GetCopyExisting() const { return m_CopyExisting; }

  	//{{AFX_DATA(CDlgNewImportDescr)
	enum { IDD = IDD_NEWIMPORTDESCR };
	CButton	m_CopyExistingCheckBox;
  	CString	m_Name;
	BOOL	m_CopyExisting;
	//}}AFX_DATA


  	//{{AFX_VIRTUAL(CDlgNewImportDescr)
	  public:
  	protected:
	  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  	//}}AFX_VIRTUAL

  protected:

	  //{{AFX_MSG(CDlgNewImportDescr)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	  DECLARE_MESSAGE_MAP()

  private:
    CImportParamsList* m_ImportDescrList;
    CImportParams* m_CurrentImportParams;
	  void BuildDefaultName();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_NEWIMPORTDESCR_H__91DA9871_E7D7_4A65_BD83_F769C94F1991__INCLUDED_

/*
#pragma once


// CDlgNewImportDescr dialog

class CDlgNewImportDescr : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewImportDescr)

public:
	CDlgNewImportDescr(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNewImportDescr();

// Dialog Data
	enum { IDD = IDD_NEWIMPORTDESCR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
*/