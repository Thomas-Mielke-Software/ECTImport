/*//////////////////////////////////////////////////////////////////////////////
// Name:        importuibase.h
// Purpose:     Interface of CImportUIBase, common base class for CDlgImportDescr 
//              and CDlgImportWizard
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined ( _IMPORTUIBASE_H_ )
#define _IMPORTUIBASE_H_

#include "importparams.h"

#if _MSC_VER > 1000
#pragma once
#endif 


class CImportUIBase : public CResizableDialog
{
public:
  CImportUIBase( UINT nIDTemplate, CWnd* pParentWnd = NULL );
  virtual ~CImportUIBase();

  void SetDoc ( int Value );

  inline CImportParams* const GetParams() const { return m_CurrentImportParams; }

  inline void SetImportParamsList ( CImportParamsList* const Value ) { m_ImportParamsList = Value; }
  inline CImportParamsList* GetImportParamsList () const { return m_ImportParamsList; };

  BOOL OnHelpInfo ( HELPINFO* pHelpInfo );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	void EnableDialogItem ( int ID, BOOL Enable );

  int m_pDoc;	// document handle
  CImportParams* m_CurrentImportParams;
  CImportParamsList* m_ImportParamsList;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#endif // _IMPORTUIBASE_H_
