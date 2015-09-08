/*//////////////////////////////////////////////////////////////////////////////
// Name:        importuibase.cpp
// Purpose:     Implementation of CImportUIBase
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "easycashdoc.h"
#include "resource.h"
#include "module.h"
#include "ResizableDialog.h"
#include "ImportUIBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CImportUIBase::CImportUIBase( UINT nIDTemplate, CWnd* pParentWnd )
  : CResizableDialog ( nIDTemplate, pParentWnd )
{
	m_CurrentImportParams = NULL;
}


CImportUIBase::~CImportUIBase()
{
}


void CImportUIBase::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}


void CImportUIBase::SetDoc ( int const Value )
{
  m_pDoc = Value;
}


void CImportUIBase::EnableDialogItem ( int ID, BOOL Enable )
{
  CWnd* Wnd = GetDlgItem ( ID );
  if ( Wnd )
    ::EnableWindow ( Wnd->m_hWnd, Enable );
}


BOOL CImportUIBase::OnHelpInfo ( HELPINFO* pHelpInfo )
{
  // call help system
  ::WinHelp ( m_hWnd, HELP_FILENAME, HELP_CONTEXT, GetWindowContextHelpId() );

	return CResizableDialog::OnHelpInfo ( pHelpInfo );
}
BEGIN_MESSAGE_MAP(CImportUIBase, CResizableDialog)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CImportUIBase::OnDestroy()
{
	RemoveAllAnchors();

	CResizableDialog::OnDestroy();
}

BOOL CImportUIBase::OnEraseBkgnd(CDC* pDC)
{
	// soll Flackern beim Resizen verhindern. Naja...

	return CResizableDialog::OnEraseBkgnd(pDC);
}
