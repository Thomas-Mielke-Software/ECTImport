/*//////////////////////////////////////////////////////////////////////////////
// Name:        dlgimportdescr.h
// Purpose:     Interface of CDlgImportDescr
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined( _DLGIMPORTDESCR_H_ )
#define _DLGIMPORTDESCR_H_

#include "resource.h"
#include "CSVFile.h"
#include "ImportParams.h"
#include "ResizableDialog.h"
#include "ImportUIBase.h"
#include "dokumentctrl.h"
#include "buchungctrl.h"
#include "EinstellungCtrl.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDlgImportDescr : public CImportUIBase {
public:
	CDlgImportDescr ( CWnd* pParent = NULL );
  virtual ~CDlgImportDescr();
  
  void OnSysCommand ( UINT nID, LPARAM lParam );

  virtual LRESULT WindowProc ( UINT message, WPARAM wParam, LPARAM lParam );

	//{{AFX_DATA(CDlgImportDescr)
	enum { IDD = IDD_IMPORTDESCR };
	CEdit	m_SeparatorChar;
	CComboBox	m_FieldAfARestwertFormat;
	CComboBox	m_FieldAfADegSatzFormat;
	CComboBox	m_FieldAfARestwert;
	CComboBox	m_FieldAfADegSatz;
	CComboBox	m_FieldBelegnummer;
	CEdit	m_FieldBNFormatNetto;
	CComboBox	m_FieldBetragFormat;
	CEdit	m_FieldBNFormatBrutto;
	CComboBox	m_FieldBruttoNetto;
	CComboBox	m_FieldBetrag;
	CComboBox	m_FieldAfADauer;
	CComboBox	m_FieldAfAAktJahr;
	CComboBox	m_FieldSteuersatzFormat;
	CComboBox	m_FieldDatumFormat;
	CEdit	m_FieldDatumDelimiter;
	CEdit	m_FieldEAFormatE;
	CEdit	m_FieldEAFormatA;
	CSpinButtonCtrl	m_NumHeaderLinesSpin;
	CComboBox	m_FieldSteuersatz;
	CComboBox	m_FieldKonto;
	CComboBox	m_FieldDatum;
	CComboBox	m_FieldEA;
	CComboBox	m_FieldBeschreibung;
	CButton	m_ViewFileContent;
	CComboBox	m_ImportDescr;
	CButton	m_SelFilename;
	CEdit	m_NumHeaderLines;
	CButton	m_NewImportDescr;
	CEdit	m_Filename;
	CEdit   m_GawkScript;
	CListCtrl	m_FileContent;
	CButton	m_DelImportDescr;
	//}}AFX_DATA

public:
	int m_Result;

	//{{AFX_VIRTUAL(CDlgImportDescr)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	void UpdateEAFieldComboBox ( BOOL SaveAndValidate );
  void UpdateBNFieldComboBox ( BOOL SaveAndValidate );
	void UpdateFieldComboBox ( CComboBox* ComboBox, CFieldConversion* Field, BOOL SaveAndValidate );
	void UpdateControlState();
	BOOL UpdateFileContent ();
  void UpdateFieldComboBoxes ( BOOL SaveAndValidate );
  void ClearFileContent ();
	BOOL GetColumnTitles ( CCSVFile* File );

	//{{AFX_MSG(CDlgImportDescr)
	afx_msg void OnSelFilename();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelImportDescr();
	afx_msg void OnNewImportDescr();
	afx_msg void OnSelChangeImportDescr();
	afx_msg void OnViewFileContent();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CDokumentctrl m_DokumentCtrl;
	CBuchungctrl m_BuchungCtrl;
	CDokumentctrl m_pDokumentCtrl;
	CBuchungctrl m_pBuchungCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnLvnItemchangedFilecontent(NMHDR *pNMHDR, LRESULT *pResult);
	CEinstellung m_EinstellungCtrl;
	afx_msg void OnCbnSelchangeFieldEa();
	afx_msg void OnBnClickedInfo();
	void UpdateFileContentColumnWidths(void);
	afx_msg void OnHdnEndtrackFilecontent(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_FieldAfAGenauigkeit;
	CComboBox m_FieldBetrieb;
	CComboBox m_FieldBestandskonto;
	afx_msg void OnCbnSelchangeFieldBestandskonto();
	afx_msg void OnBnClickedSelfilename2();
	afx_msg void OnBnClickedSelgawkscript();
	afx_msg void OnBnClickedEditgawkscript();
	afx_msg void OnBnClickedExegawkscript();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // _DLGIMPORTDESCR_H_
