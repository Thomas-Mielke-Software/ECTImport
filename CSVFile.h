/*//////////////////////////////////////////////////////////////////////////////
// Name:        cvsfile.h
// Purpose:     Interface of CCSVFile
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_CSVFILE_H__F34C42CF_690D_4132_9867_8FAD0BAC637C__INCLUDED_)
#define AFX_CSVFILE_H__F34C42CF_690D_4132_9867_8FAD0BAC637C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCSVFile : public CStdioFile {
  public:
	  CCSVFile();
  	virtual ~CCSVFile();
  
    void ParseString ( CString& Line, CStringArray& Items, const CString& ExternalSeparatorChar = _T(";") );

  private:
};

#endif // !defined(AFX_CSVFILE_H__F34C42CF_690D_4132_9867_8FAD0BAC637C__INCLUDED_)
