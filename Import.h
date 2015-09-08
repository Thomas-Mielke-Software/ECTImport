/*//////////////////////////////////////////////////////////////////////////////
// Name:        import.h
// Purpose:     Interface of CImport, the import itself
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined( _IMPORT_H_ )
#define _IMPORT_H_

#include "ImportParams.h"
#include "resource.h"
#include "CSVFile.h"
#include "dokumentctrl.h"
#include "buchungctrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImportError : public CObject {
  public:
    CImportError() { m_Msg = _T(""); m_LineNo = 0; }
    CImportError( CImportError* Value ) { m_Msg = Value->m_Msg; m_LineNo = Value->m_LineNo; }
    CImportError ( int LineNo, const CString& Msg ) { m_Msg = Msg; m_LineNo = LineNo; }
    ~CImportError() {}

    inline void SetMsg ( const CString& Value ) { m_Msg = Value; };
    inline const CString& GetMsg() const { return m_Msg; }

    inline void SetLineNo ( const int Value ) { m_LineNo = Value; }
    inline int GetLineNo () const { return m_LineNo; }

  private:
    CString m_Msg;
    int m_LineNo;
};


class CImportErrorList : public CObArray {
  public:
    ~CImportErrorList();
    
    int Add ( int LineNo, const CString& Msg );
    
    void RemoveAll ();
    void RemoveAt ( int nIndex, int nCount = 1 );

    CImportError* GetAt ( int nIndex ) const { return (CImportError*) CObArray::GetAt ( nIndex ); }


  protected:
    void FreeObjects ( int StartIndex, int Count );

  private:
};


class CImport : public  CObject {
  public:
	  CImport();
	  virtual ~CImport();
    
    void SetParams ( CImportParams* const Value );
    void AssignParams ( CImportParams* const Value );
    
    inline void SetDoc ( int const Value ) { m_pDoc = Value; }
	inline void SetDokumentCtrl ( CDokumentctrl *Value) { m_pDokumentCtrl = Value; }
	inline void SetBuchungCtrl ( CBuchungctrl *Value) { m_pBuchungCtrl = Value; }
	inline void SetDokumentID ( LONG Value) { m_pDokumentCtrl->SetID(Value); }
    
    inline const CImportErrorList& GetErrors() const { return m_Errors; }
    inline const int GetImportCount() const { return m_ImportCount; }
    inline BOOL GetUserInterrupt() const { return m_UserInterrupt; }

    BOOL Execute(CByteArray *pSelectionArray);

  protected:
	  void Init();
    static BOOL ConfirmUserInterrupt ( HWND Owner );
    BOOL ExchangeField ( CFieldConversion* Field, CImportBuchung* pBuchung, CStringArray* LinePart, void* Dest );

    CImportBuchung* CreateNewBuchung();
    void AppendBuchung ( CImportBuchung* const Buchung, CEAFieldConversion::EinnahmeAusgabe EA, CBruttoNettoFieldConversion::BruttoNetto BN );

  private:
    BOOL m_ParamsOwner;
    CImportParams* m_pParams;
    int m_pDoc;
	CDokumentctrl* m_pDokumentCtrl;
	CBuchungctrl* m_pBuchungCtrl;
    CImportErrorList m_Errors;
    int m_CurrentLine;
    int m_ImportCount;
    BOOL m_UserInterrupt;
};

#endif // !defined( _IMPORT_H_ )
