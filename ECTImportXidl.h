

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Nov 26 15:49:16 2015
 */
/* Compiler settings for ECTImportX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ECTImportXidl_h__
#define __ECTImportXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DECTImportX_FWD_DEFINED__
#define ___DECTImportX_FWD_DEFINED__
typedef interface _DECTImportX _DECTImportX;
#endif 	/* ___DECTImportX_FWD_DEFINED__ */


#ifndef ___DECTImportXEvents_FWD_DEFINED__
#define ___DECTImportXEvents_FWD_DEFINED__
typedef interface _DECTImportXEvents _DECTImportXEvents;
#endif 	/* ___DECTImportXEvents_FWD_DEFINED__ */


#ifndef __ECTImportX_FWD_DEFINED__
#define __ECTImportX_FWD_DEFINED__

#ifdef __cplusplus
typedef class ECTImportX ECTImportX;
#else
typedef struct ECTImportX ECTImportX;
#endif /* __cplusplus */

#endif 	/* __ECTImportX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ECTImportXLib_LIBRARY_DEFINED__
#define __ECTImportXLib_LIBRARY_DEFINED__

/* library ECTImportXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_ECTImportXLib;

#ifndef ___DECTImportX_DISPINTERFACE_DEFINED__
#define ___DECTImportX_DISPINTERFACE_DEFINED__

/* dispinterface _DECTImportX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DECTImportX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C0940CD6-FE5D-4F94-A8D9-8BC0B30198BC")
    _DECTImportX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DECTImportXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DECTImportX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DECTImportX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DECTImportX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DECTImportX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DECTImportX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DECTImportX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DECTImportX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DECTImportXVtbl;

    interface _DECTImportX
    {
        CONST_VTBL struct _DECTImportXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DECTImportX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DECTImportX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DECTImportX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DECTImportX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DECTImportX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DECTImportX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DECTImportX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DECTImportX_DISPINTERFACE_DEFINED__ */


#ifndef ___DECTImportXEvents_DISPINTERFACE_DEFINED__
#define ___DECTImportXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DECTImportXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DECTImportXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1380B78A-5545-4E30-8955-A7FD6057AB4B")
    _DECTImportXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DECTImportXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DECTImportXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DECTImportXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DECTImportXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DECTImportXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DECTImportXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DECTImportXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DECTImportXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DECTImportXEventsVtbl;

    interface _DECTImportXEvents
    {
        CONST_VTBL struct _DECTImportXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DECTImportXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DECTImportXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DECTImportXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DECTImportXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DECTImportXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DECTImportXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DECTImportXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DECTImportXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ECTImportX;

#ifdef __cplusplus

class DECLSPEC_UUID("163097F8-09D6-4437-B1DC-16DCAB67BC4D")
ECTImportX;
#endif
#endif /* __ECTImportXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


