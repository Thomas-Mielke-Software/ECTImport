/*//////////////////////////////////////////////////////////////////////////////
// Name:        importparams.h
// Purpose:     Interface of CImportParams, describes how the import should akt,
//              including several dependant classes
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined ( _IMPORTPARAMS_H_ )
#define _IMPORTPARAMS_H_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif /* _MSC_VER > 1000 */


class CFieldConversion : public CObject {
  protected:
    DECLARE_SERIAL ( CFieldConversion );
  
  public:
    CFieldConversion();
    virtual void Serialize ( CArchive& ar );
    
    CFieldConversion& operator = ( CFieldConversion& RightSideArgument );

    inline const CString& GetDataTypeName() const { return m_DataTypeName; }

    inline void SetName ( const CString& Value ) { m_Name = Value; }
    inline const CString& GetName() const { return m_Name; }

    inline void SetFestwert ( const CString& Value ) { m_Festwert = Value; }
    inline const CString& GetFestwert() const { return m_Festwert; }

    inline void SetSourceFieldID ( int Value ) { m_SourceFieldID = Value; }
    inline int GetSourceFieldID () const { return m_SourceFieldID; }  // is -1 when no source field assigned
    
    inline void SetBuchung ( CImportBuchung* Value ) { m_pBuchung = Value; }

    inline void SetLine ( CStringArray* Value ) { m_Line = Value; }
    inline const CStringArray& GetLine() const { return *m_Line; }

    inline const CString& GetErrorMsg() const { return m_ErrorMsg; }

    virtual BOOL Exchange ( void* Dest );

    virtual BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    virtual BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );

    BOOL CheckSourceFieldID();

    CString m_DataTypeName;

  private:
    int m_SourceFieldID;
    CString m_Name;
    CImportBuchung* m_pBuchung;
    CStringArray* m_Line;
    CString m_ErrorMsg;
	CString m_Festwert;
};


class CIntFieldConversion : public CFieldConversion {
  protected:
    DECLARE_SERIAL ( CIntFieldConversion );
  public:
    enum BlankHandling
    {
      bhIgnoreBlank = 0,
      bhBlankIsZero = 1,
      bhBlankIsError = 2
    };
    
    CIntFieldConversion();
    virtual void Serialize ( CArchive& ar );

    inline void SetBlankHandling ( const BlankHandling Value ) { m_BlankHandling = Value; }
    
    virtual BOOL Exchange ( void* Dest );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
    virtual void AssignFrom ( CIntFieldConversion* const Source );

  private:
    BlankHandling m_BlankHandling;
};


class CDateFieldConversion : public CFieldConversion {
  protected:
    DECLARE_SERIAL ( CDateFieldConversion );
  public:
    enum DateFormat {
      dfDMY = 0,
      dfMDY = 1,
      dfYMD = 2,
      dfDMYY = 3,
      dfMDYY = 4,
      dfYYMD = 5
    };

    CDateFieldConversion();
    virtual void Serialize ( CArchive& ar );

    inline void SetFormat ( const DateFormat Value ) { m_Format = Value; }
    inline DateFormat GetFormat () const { return m_Format; };

    inline void SetDelimiter ( const CString& Value ) { m_Delimiter = Value; }
    inline const CString& GetDelimiter () const { return m_Delimiter; }

    virtual BOOL Exchange ( void* Dest );

    virtual BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    virtual BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
    virtual void AssignFrom ( CDateFieldConversion* const Source );
  
  private:
    DateFormat m_Format;
    CString m_Delimiter;
};


class CTextFieldConversion : public CFieldConversion {

    DECLARE_SERIAL ( CTextFieldConversion );
  public:
    CTextFieldConversion();
    void Serialize ( CArchive& ar );

    virtual BOOL Exchange ( void* Dest );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
};


class CCurrencyFieldConversion : public CFieldConversion {
  protected:
    DECLARE_SERIAL ( CCurrencyFieldConversion );
  public:
    enum CurrencyFormat {
      cfGerman = 0,
      cfEnglish = 1,
      cf100 = 2
    };
    
    CCurrencyFieldConversion();
    virtual void Serialize ( CArchive& ar );

    inline void SetFormat ( const CurrencyFormat& Value ) { m_Format = Value; }
    inline CurrencyFormat GetFormat () const { return m_Format; };

    virtual BOOL Exchange ( void* Dest );

    virtual BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    virtual BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
    virtual void AssignFrom ( CCurrencyFieldConversion* const Source );

  private:
    CurrencyFormat m_Format;
};


class CEAFieldConversion : public CFieldConversion {
  protected:
    DECLARE_SERIAL ( CEAFieldConversion );
  public:
    enum ConversionRule {
      cUseFormat = 0,
      cIncomeOnly = 1,
      cExpenseOnly = 2,
      cUseSign = 3
    };
    
    enum EinnahmeAusgabe {
      eaUnknown = 0,
      eaIncome = 1,
      eaExpense = 2
    };
    
    CEAFieldConversion();
    virtual void Serialize ( CArchive& ar );

    void SetConversionRule ( const ConversionRule& Value );
    inline const ConversionRule GetConversionRule () const { return m_ConversionRule; };

    inline void SetFormatIncome ( const CString& Value ) { m_FormatIncome = Value; }
    inline const CString& GetFormatIncome() const { return m_FormatIncome; };

    inline void SetFormatExpense ( const CString& Value ) { m_FormatExpense = Value; }
    inline const CString& GetFormatExpense() const { return m_FormatExpense; };

    virtual BOOL Exchange ( void* Dest );

    virtual BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    virtual BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
    virtual void AssignFrom ( CEAFieldConversion* const Source );

  private:
    ConversionRule m_ConversionRule;
    CString m_FormatIncome;
    CString m_FormatExpense;
};


class CBruttoNettoFieldConversion : public CFieldConversion {
  protected:
    DECLARE_SERIAL ( CBruttoNettoFieldConversion );
  public:
    enum ConversionRule {
      cUseFormat = 0,
      cBruttoOnly = 1,
      cNettoOnly = 2,
    };
    
    enum BruttoNetto {
      bnUnknown = 0,
      bnBrutto = 1,
      bnNetto = 2
    };
    
    CBruttoNettoFieldConversion();
    virtual void Serialize ( CArchive& ar );

    void SetConversionRule ( const ConversionRule& Value );
    inline const ConversionRule GetConversionRule () const { return m_ConversionRule; };

    inline void SetFormatBrutto ( const CString& Value ) { m_FormatBrutto = Value; }
    inline const CString& GetFormatBrutto() const { return m_FormatBrutto; };

    inline void SetFormatNetto ( const CString& Value ) { m_FormatNetto = Value; }
    inline const CString& GetFormatNetto() const { return m_FormatNetto; };

    virtual BOOL Exchange ( void* Dest );

    virtual BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    virtual BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    virtual void Init();

  protected:
    virtual void AssignFrom ( CFieldConversion* const Source );
    virtual void AssignFrom ( CBruttoNettoFieldConversion* const Source );

  private:
    ConversionRule m_ConversionRule;
    CString m_FormatBrutto;
    CString m_FormatNetto;
};


class CImportParams : public CObject {
  protected:
    DECLARE_SERIAL ( CImportParams );

  public:
    static CImportParams* CreateStandard();
    
    /*
    enum Fields {
      fldMin = 0,
      fldEinAus = 0,
      fldDatum = 1,
      fldKonto = 2,
      fldText = 3,
      fldBrutto = 4,
      fldNetto = 5,
      fldSteuer = 6,
      fldAfaNr = 7,
      fldAfaJahre = 8,
      fldMax = 8
    };
    */

	  CImportParams();
  	virtual ~CImportParams();

  	virtual void Serialize ( CArchive& ar );

    BOOL Validate ( CString* ErrorMsg = NULL );
    
    inline void SetName ( const CString& Value ) { m_Name = Value; }
    inline const CString& GetName () const { return m_Name; }

    inline void SetSeparatorChar ( const CString& Value ) { m_SeparatorChar = Value; }
    inline const CString& GetSeparatorChar () const { return m_SeparatorChar; }

    inline void SetFilename ( const CString Value ) { m_Filename = Value; }
    inline const CString& GetFilename () const { return m_Filename; }

    inline void SetGawkScript ( const CString Value ) { m_GawkScript = Value; }
    inline const CString& GetGawkScript () const { return m_GawkScript; }

    BOOL SetHeaderCount ( int Value );
    inline int GetHeaderCount() const { return m_HeaderCount; }

	inline void SetColumnWidth ( int Index, int Value ) { m_ColumnWidths.SetAtGrow(Index, Value); }
	inline int GetColumnWidth( int Index ) const { return Index >= m_ColumnWidths.GetSize() ? 0 : m_ColumnWidths[Index]; }
    
    // ??? *** !!! hötzen? quirk! tüddelpipp/
    inline CStringArray* const GetColumnNames () { return &m_ColumnNames; }

    inline CEAFieldConversion* const GetFieldEA () { return &m_FieldEA; }
    inline CDateFieldConversion* const GetFieldDatum () { return &m_FieldDatum; }
    inline CCurrencyFieldConversion* const GetFieldBetrag () { return &m_FieldBetrag; }
    inline CBruttoNettoFieldConversion* const GetFieldBruttoNetto () { return &m_FieldBruttoNetto; }
    inline CTextFieldConversion* const GetFieldBeschreibung () { return &m_FieldBeschreibung; }
    inline CTextFieldConversion* const GetFieldBelegnummer () { return &m_FieldBelegnummer; }
    inline CTextFieldConversion* const GetFieldKonto () { return &m_FieldKonto; }
    inline CCurrencyFieldConversion* const GetFieldSteuersatz () { return &m_FieldSteuersatz; }
    inline CIntFieldConversion* const GetFieldAfAAktJahr () { return &m_FieldAfAAktJahr; }
    inline CIntFieldConversion* const GetFieldAfADauer () { return &m_FieldAfADauer; }
    inline CCurrencyFieldConversion* const GetFieldAfADegSatz () { return &m_FieldAfADegSatz; }
    inline CCurrencyFieldConversion* const GetFieldAfARestwert () { return &m_FieldAfARestwert; }
    inline CIntFieldConversion* const GetFieldAfAGenauigkeit () { return &m_FieldAfAGenauigkeit; }
    inline CTextFieldConversion* const GetFieldBetrieb () { return &m_FieldBetrieb; }
    inline CTextFieldConversion* const GetFieldBestandskonto () { return &m_FieldBestandskonto; }

    CImportParams& operator = ( CImportParams& RightSideArgument );  

    BOOL LoadFromIniFile ( const CString &Filename, const CString& Sectionname );
    BOOL SaveToIniFile ( const CString &Filename, const CString& Sectionname );

    void Init();

  protected:

  private:
    CString m_Name;
    CString m_SeparatorChar;
    CString m_Filename;
	CString m_GawkScript;
    int m_HeaderCount;
    CStringArray m_ColumnNames;
	CArray<int,int&> m_ColumnWidths;
    CEAFieldConversion m_FieldEA;
    CDateFieldConversion m_FieldDatum;
    CCurrencyFieldConversion m_FieldBetrag;
    CBruttoNettoFieldConversion m_FieldBruttoNetto;
    CTextFieldConversion m_FieldBeschreibung;
    CTextFieldConversion m_FieldBelegnummer;
    CTextFieldConversion m_FieldKonto;
    CCurrencyFieldConversion m_FieldSteuersatz;
    CIntFieldConversion m_FieldAfAAktJahr;
    CIntFieldConversion m_FieldAfADauer;
    CCurrencyFieldConversion m_FieldAfADegSatz;
    CCurrencyFieldConversion m_FieldAfARestwert;
    CIntFieldConversion m_FieldAfAGenauigkeit;
    CTextFieldConversion m_FieldBetrieb;
    CTextFieldConversion m_FieldBestandskonto;
};



#define SEC_CSVIMPORT  _T("CSVIMPORT")
#define KEY_DEFAULT_IMPORT_PARAMS  _T("DefaultImportParams")


class CImportParamsList : public CObArray {
  protected:
    DECLARE_SERIAL ( CImportParamsList )

  public:
  	~CImportParamsList();
    
    virtual void Serialize ( CArchive& ar );

    inline CImportParams* const GetDefaultImportParams() const { return m_DefaultImportParams; }
    BOOL SetDefaultImportParams ( CImportParams* const Value );

  	inline BOOL IsEmpty() const { return GetSize() == 0 ? TRUE : FALSE; }
    
    void RemoveAll ();
    void RemoveAt ( int nIndex, int nCount = 1 );

    BOOL CImportParamsList::Contains ( CImportParams* Element ) const;
    int FindByName ( const CString& AName ) const;

    static inline void BuildSectionname ( CString* Sectionname, const int Number );

    BOOL LoadFromIniFile ();
    BOOL LoadFromIniFile ( const CString &Filename );
    BOOL SaveToIniFile ();
    BOOL SaveToIniFile ( const CString &Filename );

  protected:

  private:
    CImportParams* m_DefaultImportParams;
    void FreeObjects ( int StartIndex, int Count );
};


#endif /* _IMPORTPARAMS_H_ */
