/*//////////////////////////////////////////////////////////////////////////////
// Name:        cvsfile.cpp
// Purpose:     Implementation of CCSVFile, a CFile descentand with CSV-parsing
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "CSVFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CCSVFile::CCSVFile() {
}


CCSVFile::~CCSVFile() {
}


void CCSVFile::ParseString ( CString& Line, CStringArray& Items, const CString& ExternalSeparatorChar ) {
  // clear
  Items.RemoveAll();

  // declare helper variables
  int i;
  BOOL InQuotation = FALSE;
  CString Part = _T("");
  CString SeparatorChar;

  // decode separator char
  if ( ExternalSeparatorChar.CompareNoCase ( _T("<Tab>") ) == 0 )
    SeparatorChar = _T("\t");
  else
    SeparatorChar = ExternalSeparatorChar;

  /*
  //
  for ( i = 0; i <= Line.GetLength() - 1; i++ ) {
    if ( Line[i] == QuotationMark && ( ( i <= Line.GetLength() - 1 && Line[i+1] != QuotationMark ) || i+1 > Line.GetLength() - 1 ) )
      InQuotation = !InQuotation;
    else 
      if ( !InQuotation && Line[i] == SeparatorChar ) {
        Items.Add ( Part );
        Part.Empty();
      }
      else
        Part += Line[i];
  }
  if ( Part != _T("") )
    Items.Add ( Part );
  */

  //
  i = 0;
  while ( i <= Line.GetLength() -1 ) 
  {
    if ( InQuotation ) 
    {
      if ( Line.GetAt ( i ) == _T("\"")[0] ) 
      {
        if ( i < Line.GetLength() - 1 && Line[i+1] == _T("\"")[0] ) 
        {
          Part += Line[i];
          i++;
        }
        else
          InQuotation = !InQuotation;
      }
      else 
        Part += Line[i];
    }
    else 
    {
      if ( Line[i] == SeparatorChar ) 
      {
        Items.Add ( Part );
        Part.Empty();
      }
      else
        if ( Line[i] == _T("\"")[0] )
          InQuotation = TRUE;
        else
          Part += Line[i];
    }
    i++;
  }
  if ( !Part.IsEmpty() )
    Items.Add ( Part );
}
