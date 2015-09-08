/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgimportstatus.h
// Purpose:     Interfaceof CDlgImportStatus, displays error-messages when import
//              (partly) fails
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined( _DLGIMPORTSTATUS_H_ )
#define _DLGIMPORTSTATUS_H_

#include "resource.h"
#include "import.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgImportStatus : public CDialog
{
public:
	CDlgImportStatus(CWnd* pParent = NULL);

	//{{AFX_DATA(CDlgImportStatus)
	enum { IDD = IDD_IMPORTSTATUS };
	CStatic	m_ErrorIcon;
	CListCtrl	m_MessagesList;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CDlgImportStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL
  
public:
	void UpdateColumnWidths();
  void SetMessages ( const CImportErrorList& Value );

protected:

	//{{AFX_MSG(CDlgImportStatus)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  CImportErrorList m_Messages;
};


#endif /* _DLGIMPORTSTATUS_H_ */

/*
#pragma once


// CDlgImportStatus dialog

class CDlgImportStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgImportStatus)

public:
	CDlgImportStatus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImportStatus();

// Dialog Data
	enum { IDD = IDD_IMPORTSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
*/