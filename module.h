/*//////////////////////////////////////////////////////////////////////////////
// Name:        module.h
// Purpose:     some globals
// Author:      Ruediger Herrmann
// Copyright:   (c) Ruediger Herrmann
//////////////////////////////////////////////////////////////////////////////*/

#if !defined ( _MODULE_H_ )
#define _MODULE_H_

//
#define CSVIMPORT_FILE_VERSION 1


//
#define HELP_FILENAME _T("ecteimport.hlp")


// key names for EasyCash&Tax-Ini-File
#define INI_KEYNAME_NAME                             _T("Name")
#define INI_KEYNAME_SEPARATIR                        _T("Separator")
#define INI_KEYNAME_FILENAME                         _T("Filename")
#define INI_KEYNAME_GAWKSCRIPT						 _T("GawkScript")
#define INI_KEYNAME_HEADERCOUNT                      _T("HeaderCount")
#define INI_KEYNAME_COLUMNNAME                       _T("ColumnName")
#define INI_KEYNAME_COLUMNWIDTH                      _T("ColumnWidth")

#define INI_KEYNAME_SOURCEFIELDID                    _T("SourceFieldID")  /* Prefix */

#define INI_KEYNAME_CURRENCYFORMAT                   _T("CurrencyFormat")

#define INI_KEYNAME_DATEFORMAT                       _T("DateFormat")
#define INI_KEYNAME_DATEDELIMITER                    _T("DateDelimiter")

#define INI_KEYNAME_CONVERSIONRULE_INCOMEEXPENSE     _T("ConversionRule")
#define INI_KEYNAME_FORMATEINNAHME                   _T("FormatEinnahme")
#define INI_KEYNAME_FORMATAUSGABE                    _T("FormatAusgabe")

#define INI_KEYNAME_CONVERSIONRULE_BRUTTONETTO       _T("ConversionRuleBruttoNetto")
#define INI_KEYNAME_FORMATBRUTTO                     _T("FormatBrutto")
#define INI_KEYNAME_FORMATNETTO                      _T("FormatNetto")


// menu item name - appears in extensions menu in EasyCash&Tax
static TCHAR MenuItemName[25]; 


//
//extern "C" AFX_EXTENSION_MODULE ECTEImportDLL;
// dont'use this any more, want to link statically

#endif /* !defined ( _MODULE_H_ ) */