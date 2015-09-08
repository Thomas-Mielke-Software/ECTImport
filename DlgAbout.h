/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgabout.h
// Purpose:     Interface of CDlgAbout
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined( _DLGABOUT_H_ )
#define _DLGABOUT_H_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgAbout : public CDialog {
  public:
  	CDlgAbout(CWnd* pParent = NULL);   

	  //{{AFX_DATA(CDlgAbout)
	  enum { IDD = IDD_ABOUTBOX_ECTIMPORTX };
	  CEdit	m_EMail;
	  CStatic	m_AboutIcon;
	  CStatic	m_Version;
	//}}AFX_DATA


  	//{{AFX_VIRTUAL(CDlgAbout)
	  protected:
    	virtual void DoDataExchange(CDataExchange* pDX);
  	//}}AFX_VIRTUAL

  protected:
	  //{{AFX_MSG(CDlgAbout)
  	virtual BOOL OnInitDialog();
	  afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
  	DECLARE_MESSAGE_MAP()
  
  private:
};


#endif // _DLGABOUT_H_
