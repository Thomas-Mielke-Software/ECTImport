// DlgProgress.h : Header-Datei
// CG: Diese Datei wurde von der Komponente "Dialogfeld Fortschritt" hinzugefügt

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress Dialogfeld

#ifndef __DLGPROGRESS_H__
#define __DLGPROGRESS_H__

class CDlgProgress : public CDialog
{
public:
    CDlgProgress ( UINT nCaptionID = 0 );
    ~CDlgProgress();

    BOOL Create(CWnd *pParent=NULL);

    // Schaltfläche "Abbrechen" wird geprüft
    BOOL CheckCancelButton();

    // Anpassen des Dialogfelds Fortschritt
    bool SetStatus ( UINT MessageID );
    bool SetStatus ( LPCTSTR lpszMessage );
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
        
    //{{AFX_DATA(CDlgProgress)
    enum { IDD = CG_IDD_PROGRESS };
    CProgressCtrl	m_Progress;
    //}}AFX_DATA

    //{{AFX_VIRTUAL(CDlgProgress)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

protected:
	UINT m_nCaptionID;
  int m_nLower;
  int m_nUpper;
  int m_nStep;
  
  BOOL m_bCancel;
  BOOL m_bParentDisabled;

  void ReEnableParent();

  virtual void OnCancel();
  virtual void OnOK() {}; 
  void PumpMessages();

  //{{AFX_MSG(CDlgProgress)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif // __DLGPROGRESS_H__

/*
#pragma once


// CDlgProgress dialog

class CDlgProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProgress();

// Dialog Data
	enum { IDD = CG_IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
*/