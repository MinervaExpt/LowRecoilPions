// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME UnfoldUtils

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/UnfoldUtils/dict/UnfoldUtilsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_RooUnfoldResponse(void *p = 0);
   static void *newArray_RooUnfoldResponse(Long_t size, void *p);
   static void delete_RooUnfoldResponse(void *p);
   static void deleteArray_RooUnfoldResponse(void *p);
   static void destruct_RooUnfoldResponse(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldResponse*)
   {
      ::RooUnfoldResponse *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldResponse >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldResponse", ::RooUnfoldResponse::Class_Version(), "RooUnfold/RooUnfoldResponse.h", 28,
                  typeid(::RooUnfoldResponse), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldResponse::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldResponse) );
      instance.SetNew(&new_RooUnfoldResponse);
      instance.SetNewArray(&newArray_RooUnfoldResponse);
      instance.SetDelete(&delete_RooUnfoldResponse);
      instance.SetDeleteArray(&deleteArray_RooUnfoldResponse);
      instance.SetDestructor(&destruct_RooUnfoldResponse);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldResponse*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldResponse*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldResponse*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfold(void *p = 0);
   static void *newArray_RooUnfold(Long_t size, void *p);
   static void delete_RooUnfold(void *p);
   static void deleteArray_RooUnfold(void *p);
   static void destruct_RooUnfold(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfold*)
   {
      ::RooUnfold *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfold >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfold", ::RooUnfold::Class_Version(), "RooUnfold/RooUnfold.h", 23,
                  typeid(::RooUnfold), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfold::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfold) );
      instance.SetNew(&new_RooUnfold);
      instance.SetNewArray(&newArray_RooUnfold);
      instance.SetDelete(&delete_RooUnfold);
      instance.SetDeleteArray(&deleteArray_RooUnfold);
      instance.SetDestructor(&destruct_RooUnfold);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfold*)
   {
      return GenerateInitInstanceLocal((::RooUnfold*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfold*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldBayes(void *p = 0);
   static void *newArray_RooUnfoldBayes(Long_t size, void *p);
   static void delete_RooUnfoldBayes(void *p);
   static void deleteArray_RooUnfoldBayes(void *p);
   static void destruct_RooUnfoldBayes(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldBayes*)
   {
      ::RooUnfoldBayes *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldBayes >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldBayes", ::RooUnfoldBayes::Class_Version(), "RooUnfold/RooUnfoldBayes.h", 24,
                  typeid(::RooUnfoldBayes), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldBayes::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldBayes) );
      instance.SetNew(&new_RooUnfoldBayes);
      instance.SetNewArray(&newArray_RooUnfoldBayes);
      instance.SetDelete(&delete_RooUnfoldBayes);
      instance.SetDeleteArray(&deleteArray_RooUnfoldBayes);
      instance.SetDestructor(&destruct_RooUnfoldBayes);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldBayes*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldBayes*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldBayes*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldBinByBin(void *p = 0);
   static void *newArray_RooUnfoldBinByBin(Long_t size, void *p);
   static void delete_RooUnfoldBinByBin(void *p);
   static void deleteArray_RooUnfoldBinByBin(void *p);
   static void destruct_RooUnfoldBinByBin(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldBinByBin*)
   {
      ::RooUnfoldBinByBin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldBinByBin >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldBinByBin", ::RooUnfoldBinByBin::Class_Version(), "RooUnfold/RooUnfoldBinByBin.h", 23,
                  typeid(::RooUnfoldBinByBin), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldBinByBin::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldBinByBin) );
      instance.SetNew(&new_RooUnfoldBinByBin);
      instance.SetNewArray(&newArray_RooUnfoldBinByBin);
      instance.SetDelete(&delete_RooUnfoldBinByBin);
      instance.SetDeleteArray(&deleteArray_RooUnfoldBinByBin);
      instance.SetDestructor(&destruct_RooUnfoldBinByBin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldBinByBin*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldBinByBin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldBinByBin*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_RooUnfoldErrors(void *p);
   static void deleteArray_RooUnfoldErrors(void *p);
   static void destruct_RooUnfoldErrors(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldErrors*)
   {
      ::RooUnfoldErrors *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldErrors >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldErrors", ::RooUnfoldErrors::Class_Version(), "RooUnfold/RooUnfoldErrors.h", 21,
                  typeid(::RooUnfoldErrors), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldErrors::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldErrors) );
      instance.SetDelete(&delete_RooUnfoldErrors);
      instance.SetDeleteArray(&deleteArray_RooUnfoldErrors);
      instance.SetDestructor(&destruct_RooUnfoldErrors);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldErrors*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldErrors*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldErrors*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldInvert(void *p = 0);
   static void *newArray_RooUnfoldInvert(Long_t size, void *p);
   static void delete_RooUnfoldInvert(void *p);
   static void deleteArray_RooUnfoldInvert(void *p);
   static void destruct_RooUnfoldInvert(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldInvert*)
   {
      ::RooUnfoldInvert *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldInvert >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldInvert", ::RooUnfoldInvert::Class_Version(), "RooUnfold/RooUnfoldInvert.h", 25,
                  typeid(::RooUnfoldInvert), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldInvert::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldInvert) );
      instance.SetNew(&new_RooUnfoldInvert);
      instance.SetNewArray(&newArray_RooUnfoldInvert);
      instance.SetDelete(&delete_RooUnfoldInvert);
      instance.SetDeleteArray(&deleteArray_RooUnfoldInvert);
      instance.SetDestructor(&destruct_RooUnfoldInvert);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldInvert*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldInvert*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldInvert*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldParms(void *p = 0);
   static void *newArray_RooUnfoldParms(Long_t size, void *p);
   static void delete_RooUnfoldParms(void *p);
   static void deleteArray_RooUnfoldParms(void *p);
   static void destruct_RooUnfoldParms(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldParms*)
   {
      ::RooUnfoldParms *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldParms >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldParms", ::RooUnfoldParms::Class_Version(), "RooUnfold/RooUnfoldParms.h", 22,
                  typeid(::RooUnfoldParms), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldParms::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldParms) );
      instance.SetNew(&new_RooUnfoldParms);
      instance.SetNewArray(&newArray_RooUnfoldParms);
      instance.SetDelete(&delete_RooUnfoldParms);
      instance.SetDeleteArray(&deleteArray_RooUnfoldParms);
      instance.SetDestructor(&destruct_RooUnfoldParms);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldParms*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldParms*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldParms*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldSvd(void *p = 0);
   static void *newArray_RooUnfoldSvd(Long_t size, void *p);
   static void delete_RooUnfoldSvd(void *p);
   static void deleteArray_RooUnfoldSvd(void *p);
   static void destruct_RooUnfoldSvd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldSvd*)
   {
      ::RooUnfoldSvd *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldSvd >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldSvd", ::RooUnfoldSvd::Class_Version(), "RooUnfold/RooUnfoldSvd.h", 26,
                  typeid(::RooUnfoldSvd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldSvd::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldSvd) );
      instance.SetNew(&new_RooUnfoldSvd);
      instance.SetNewArray(&newArray_RooUnfoldSvd);
      instance.SetDelete(&delete_RooUnfoldSvd);
      instance.SetDeleteArray(&deleteArray_RooUnfoldSvd);
      instance.SetDestructor(&destruct_RooUnfoldSvd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldSvd*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldSvd*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldSvd*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUnfoldTUnfold(void *p = 0);
   static void *newArray_RooUnfoldTUnfold(Long_t size, void *p);
   static void delete_RooUnfoldTUnfold(void *p);
   static void deleteArray_RooUnfoldTUnfold(void *p);
   static void destruct_RooUnfoldTUnfold(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUnfoldTUnfold*)
   {
      ::RooUnfoldTUnfold *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUnfoldTUnfold >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUnfoldTUnfold", ::RooUnfoldTUnfold::Class_Version(), "RooUnfold/RooUnfoldTUnfold.h", 25,
                  typeid(::RooUnfoldTUnfold), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUnfoldTUnfold::Dictionary, isa_proxy, 4,
                  sizeof(::RooUnfoldTUnfold) );
      instance.SetNew(&new_RooUnfoldTUnfold);
      instance.SetNewArray(&newArray_RooUnfoldTUnfold);
      instance.SetDelete(&delete_RooUnfoldTUnfold);
      instance.SetDeleteArray(&deleteArray_RooUnfoldTUnfold);
      instance.SetDestructor(&destruct_RooUnfoldTUnfold);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUnfoldTUnfold*)
   {
      return GenerateInitInstanceLocal((::RooUnfoldTUnfold*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooUnfoldTUnfold*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *MinervaUnfoldcLcLMnvUnfold_Dictionary();
   static void MinervaUnfoldcLcLMnvUnfold_TClassManip(TClass*);
   static void *new_MinervaUnfoldcLcLMnvUnfold(void *p = 0);
   static void *newArray_MinervaUnfoldcLcLMnvUnfold(Long_t size, void *p);
   static void delete_MinervaUnfoldcLcLMnvUnfold(void *p);
   static void deleteArray_MinervaUnfoldcLcLMnvUnfold(void *p);
   static void destruct_MinervaUnfoldcLcLMnvUnfold(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MinervaUnfold::MnvUnfold*)
   {
      ::MinervaUnfold::MnvUnfold *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MinervaUnfold::MnvUnfold));
      static ::ROOT::TGenericClassInfo 
         instance("MinervaUnfold::MnvUnfold", "MinervaUnfold/MnvUnfold.h", 24,
                  typeid(::MinervaUnfold::MnvUnfold), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MinervaUnfoldcLcLMnvUnfold_Dictionary, isa_proxy, 4,
                  sizeof(::MinervaUnfold::MnvUnfold) );
      instance.SetNew(&new_MinervaUnfoldcLcLMnvUnfold);
      instance.SetNewArray(&newArray_MinervaUnfoldcLcLMnvUnfold);
      instance.SetDelete(&delete_MinervaUnfoldcLcLMnvUnfold);
      instance.SetDeleteArray(&deleteArray_MinervaUnfoldcLcLMnvUnfold);
      instance.SetDestructor(&destruct_MinervaUnfoldcLcLMnvUnfold);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MinervaUnfold::MnvUnfold*)
   {
      return GenerateInitInstanceLocal((::MinervaUnfold::MnvUnfold*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MinervaUnfold::MnvUnfold*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MinervaUnfoldcLcLMnvUnfold_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MinervaUnfold::MnvUnfold*)0x0)->GetClass();
      MinervaUnfoldcLcLMnvUnfold_TClassManip(theClass);
   return theClass;
   }

   static void MinervaUnfoldcLcLMnvUnfold_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MinervaUnfoldcLcLMnvResponse_Dictionary();
   static void MinervaUnfoldcLcLMnvResponse_TClassManip(TClass*);
   static void *new_MinervaUnfoldcLcLMnvResponse(void *p = 0);
   static void *newArray_MinervaUnfoldcLcLMnvResponse(Long_t size, void *p);
   static void delete_MinervaUnfoldcLcLMnvResponse(void *p);
   static void deleteArray_MinervaUnfoldcLcLMnvResponse(void *p);
   static void destruct_MinervaUnfoldcLcLMnvResponse(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MinervaUnfold::MnvResponse*)
   {
      ::MinervaUnfold::MnvResponse *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MinervaUnfold::MnvResponse));
      static ::ROOT::TGenericClassInfo 
         instance("MinervaUnfold::MnvResponse", "MinervaUnfold/MnvResponse.h", 25,
                  typeid(::MinervaUnfold::MnvResponse), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MinervaUnfoldcLcLMnvResponse_Dictionary, isa_proxy, 4,
                  sizeof(::MinervaUnfold::MnvResponse) );
      instance.SetNew(&new_MinervaUnfoldcLcLMnvResponse);
      instance.SetNewArray(&newArray_MinervaUnfoldcLcLMnvResponse);
      instance.SetDelete(&delete_MinervaUnfoldcLcLMnvResponse);
      instance.SetDeleteArray(&deleteArray_MinervaUnfoldcLcLMnvResponse);
      instance.SetDestructor(&destruct_MinervaUnfoldcLcLMnvResponse);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MinervaUnfold::MnvResponse*)
   {
      return GenerateInitInstanceLocal((::MinervaUnfold::MnvResponse*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MinervaUnfold::MnvResponse*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MinervaUnfoldcLcLMnvResponse_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MinervaUnfold::MnvResponse*)0x0)->GetClass();
      MinervaUnfoldcLcLMnvResponse_TClassManip(theClass);
   return theClass;
   }

   static void MinervaUnfoldcLcLMnvResponse_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldResponse::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldResponse::Class_Name()
{
   return "RooUnfoldResponse";
}

//______________________________________________________________________________
const char *RooUnfoldResponse::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldResponse*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldResponse::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldResponse*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldResponse::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldResponse*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldResponse::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldResponse*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfold::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfold::Class_Name()
{
   return "RooUnfold";
}

//______________________________________________________________________________
const char *RooUnfold::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfold*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfold::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfold*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfold::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfold*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfold::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfold*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldBayes::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldBayes::Class_Name()
{
   return "RooUnfoldBayes";
}

//______________________________________________________________________________
const char *RooUnfoldBayes::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBayes*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldBayes::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBayes*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldBayes::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBayes*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldBayes::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBayes*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldBinByBin::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldBinByBin::Class_Name()
{
   return "RooUnfoldBinByBin";
}

//______________________________________________________________________________
const char *RooUnfoldBinByBin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBinByBin*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldBinByBin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBinByBin*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldBinByBin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBinByBin*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldBinByBin::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldBinByBin*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldErrors::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldErrors::Class_Name()
{
   return "RooUnfoldErrors";
}

//______________________________________________________________________________
const char *RooUnfoldErrors::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldErrors*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldErrors::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldErrors*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldErrors::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldErrors*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldErrors::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldErrors*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldInvert::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldInvert::Class_Name()
{
   return "RooUnfoldInvert";
}

//______________________________________________________________________________
const char *RooUnfoldInvert::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldInvert*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldInvert::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldInvert*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldInvert::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldInvert*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldInvert::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldInvert*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldParms::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldParms::Class_Name()
{
   return "RooUnfoldParms";
}

//______________________________________________________________________________
const char *RooUnfoldParms::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldParms*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldParms::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldParms*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldParms::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldParms*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldParms::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldParms*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldSvd::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldSvd::Class_Name()
{
   return "RooUnfoldSvd";
}

//______________________________________________________________________________
const char *RooUnfoldSvd::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldSvd*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldSvd::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldSvd*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldSvd::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldSvd*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldSvd::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldSvd*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUnfoldTUnfold::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUnfoldTUnfold::Class_Name()
{
   return "RooUnfoldTUnfold";
}

//______________________________________________________________________________
const char *RooUnfoldTUnfold::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldTUnfold*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUnfoldTUnfold::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldTUnfold*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUnfoldTUnfold::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldTUnfold*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUnfoldTUnfold::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUnfoldTUnfold*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RooUnfoldResponse::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldResponse.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldResponse::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldResponse::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldResponse(void *p) {
      return  p ? new(p) ::RooUnfoldResponse : new ::RooUnfoldResponse;
   }
   static void *newArray_RooUnfoldResponse(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldResponse[nElements] : new ::RooUnfoldResponse[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldResponse(void *p) {
      delete ((::RooUnfoldResponse*)p);
   }
   static void deleteArray_RooUnfoldResponse(void *p) {
      delete [] ((::RooUnfoldResponse*)p);
   }
   static void destruct_RooUnfoldResponse(void *p) {
      typedef ::RooUnfoldResponse current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldResponse

//______________________________________________________________________________
void RooUnfold::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfold.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfold::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfold::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfold(void *p) {
      return  p ? new(p) ::RooUnfold : new ::RooUnfold;
   }
   static void *newArray_RooUnfold(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfold[nElements] : new ::RooUnfold[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfold(void *p) {
      delete ((::RooUnfold*)p);
   }
   static void deleteArray_RooUnfold(void *p) {
      delete [] ((::RooUnfold*)p);
   }
   static void destruct_RooUnfold(void *p) {
      typedef ::RooUnfold current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfold

//______________________________________________________________________________
void RooUnfoldBayes::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldBayes.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldBayes::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldBayes::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldBayes(void *p) {
      return  p ? new(p) ::RooUnfoldBayes : new ::RooUnfoldBayes;
   }
   static void *newArray_RooUnfoldBayes(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldBayes[nElements] : new ::RooUnfoldBayes[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldBayes(void *p) {
      delete ((::RooUnfoldBayes*)p);
   }
   static void deleteArray_RooUnfoldBayes(void *p) {
      delete [] ((::RooUnfoldBayes*)p);
   }
   static void destruct_RooUnfoldBayes(void *p) {
      typedef ::RooUnfoldBayes current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldBayes

//______________________________________________________________________________
void RooUnfoldBinByBin::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldBinByBin.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldBinByBin::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldBinByBin::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldBinByBin(void *p) {
      return  p ? new(p) ::RooUnfoldBinByBin : new ::RooUnfoldBinByBin;
   }
   static void *newArray_RooUnfoldBinByBin(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldBinByBin[nElements] : new ::RooUnfoldBinByBin[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldBinByBin(void *p) {
      delete ((::RooUnfoldBinByBin*)p);
   }
   static void deleteArray_RooUnfoldBinByBin(void *p) {
      delete [] ((::RooUnfoldBinByBin*)p);
   }
   static void destruct_RooUnfoldBinByBin(void *p) {
      typedef ::RooUnfoldBinByBin current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldBinByBin

//______________________________________________________________________________
void RooUnfoldErrors::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldErrors.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldErrors::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldErrors::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RooUnfoldErrors(void *p) {
      delete ((::RooUnfoldErrors*)p);
   }
   static void deleteArray_RooUnfoldErrors(void *p) {
      delete [] ((::RooUnfoldErrors*)p);
   }
   static void destruct_RooUnfoldErrors(void *p) {
      typedef ::RooUnfoldErrors current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldErrors

//______________________________________________________________________________
void RooUnfoldInvert::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldInvert.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldInvert::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldInvert::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldInvert(void *p) {
      return  p ? new(p) ::RooUnfoldInvert : new ::RooUnfoldInvert;
   }
   static void *newArray_RooUnfoldInvert(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldInvert[nElements] : new ::RooUnfoldInvert[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldInvert(void *p) {
      delete ((::RooUnfoldInvert*)p);
   }
   static void deleteArray_RooUnfoldInvert(void *p) {
      delete [] ((::RooUnfoldInvert*)p);
   }
   static void destruct_RooUnfoldInvert(void *p) {
      typedef ::RooUnfoldInvert current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldInvert

//______________________________________________________________________________
void RooUnfoldParms::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldParms.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldParms::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldParms::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldParms(void *p) {
      return  p ? new(p) ::RooUnfoldParms : new ::RooUnfoldParms;
   }
   static void *newArray_RooUnfoldParms(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldParms[nElements] : new ::RooUnfoldParms[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldParms(void *p) {
      delete ((::RooUnfoldParms*)p);
   }
   static void deleteArray_RooUnfoldParms(void *p) {
      delete [] ((::RooUnfoldParms*)p);
   }
   static void destruct_RooUnfoldParms(void *p) {
      typedef ::RooUnfoldParms current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldParms

//______________________________________________________________________________
void RooUnfoldSvd::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldSvd.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldSvd::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldSvd::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldSvd(void *p) {
      return  p ? new(p) ::RooUnfoldSvd : new ::RooUnfoldSvd;
   }
   static void *newArray_RooUnfoldSvd(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldSvd[nElements] : new ::RooUnfoldSvd[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldSvd(void *p) {
      delete ((::RooUnfoldSvd*)p);
   }
   static void deleteArray_RooUnfoldSvd(void *p) {
      delete [] ((::RooUnfoldSvd*)p);
   }
   static void destruct_RooUnfoldSvd(void *p) {
      typedef ::RooUnfoldSvd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldSvd

//______________________________________________________________________________
void RooUnfoldTUnfold::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUnfoldTUnfold.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooUnfoldTUnfold::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooUnfoldTUnfold::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUnfoldTUnfold(void *p) {
      return  p ? new(p) ::RooUnfoldTUnfold : new ::RooUnfoldTUnfold;
   }
   static void *newArray_RooUnfoldTUnfold(Long_t nElements, void *p) {
      return p ? new(p) ::RooUnfoldTUnfold[nElements] : new ::RooUnfoldTUnfold[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUnfoldTUnfold(void *p) {
      delete ((::RooUnfoldTUnfold*)p);
   }
   static void deleteArray_RooUnfoldTUnfold(void *p) {
      delete [] ((::RooUnfoldTUnfold*)p);
   }
   static void destruct_RooUnfoldTUnfold(void *p) {
      typedef ::RooUnfoldTUnfold current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooUnfoldTUnfold

namespace ROOT {
   // Wrappers around operator new
   static void *new_MinervaUnfoldcLcLMnvUnfold(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MinervaUnfold::MnvUnfold : new ::MinervaUnfold::MnvUnfold;
   }
   static void *newArray_MinervaUnfoldcLcLMnvUnfold(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MinervaUnfold::MnvUnfold[nElements] : new ::MinervaUnfold::MnvUnfold[nElements];
   }
   // Wrapper around operator delete
   static void delete_MinervaUnfoldcLcLMnvUnfold(void *p) {
      delete ((::MinervaUnfold::MnvUnfold*)p);
   }
   static void deleteArray_MinervaUnfoldcLcLMnvUnfold(void *p) {
      delete [] ((::MinervaUnfold::MnvUnfold*)p);
   }
   static void destruct_MinervaUnfoldcLcLMnvUnfold(void *p) {
      typedef ::MinervaUnfold::MnvUnfold current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MinervaUnfold::MnvUnfold

namespace ROOT {
   // Wrappers around operator new
   static void *new_MinervaUnfoldcLcLMnvResponse(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MinervaUnfold::MnvResponse : new ::MinervaUnfold::MnvResponse;
   }
   static void *newArray_MinervaUnfoldcLcLMnvResponse(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MinervaUnfold::MnvResponse[nElements] : new ::MinervaUnfold::MnvResponse[nElements];
   }
   // Wrapper around operator delete
   static void delete_MinervaUnfoldcLcLMnvResponse(void *p) {
      delete ((::MinervaUnfold::MnvResponse*)p);
   }
   static void deleteArray_MinervaUnfoldcLcLMnvResponse(void *p) {
      delete [] ((::MinervaUnfold::MnvResponse*)p);
   }
   static void destruct_MinervaUnfoldcLcLMnvResponse(void *p) {
      typedef ::MinervaUnfold::MnvResponse current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MinervaUnfold::MnvResponse

namespace ROOT {
   static TClass *maplEstringcOstringgR_Dictionary();
   static void maplEstringcOstringgR_TClassManip(TClass*);
   static void *new_maplEstringcOstringgR(void *p = 0);
   static void *newArray_maplEstringcOstringgR(Long_t size, void *p);
   static void delete_maplEstringcOstringgR(void *p);
   static void deleteArray_maplEstringcOstringgR(void *p);
   static void destruct_maplEstringcOstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,string>*)
   {
      map<string,string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,string>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,string>", -2, "map", 96,
                  typeid(map<string,string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOstringgR_Dictionary, isa_proxy, 4,
                  sizeof(map<string,string>) );
      instance.SetNew(&new_maplEstringcOstringgR);
      instance.SetNewArray(&newArray_maplEstringcOstringgR);
      instance.SetDelete(&delete_maplEstringcOstringgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOstringgR);
      instance.SetDestructor(&destruct_maplEstringcOstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,string>*)0x0)->GetClass();
      maplEstringcOstringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,string> : new map<string,string>;
   }
   static void *newArray_maplEstringcOstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,string>[nElements] : new map<string,string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOstringgR(void *p) {
      delete ((map<string,string>*)p);
   }
   static void deleteArray_maplEstringcOstringgR(void *p) {
      delete [] ((map<string,string>*)p);
   }
   static void destruct_maplEstringcOstringgR(void *p) {
      typedef map<string,string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,string>

namespace {
  void TriggerDictionaryInitialization_UnfoldUtils_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "UnfoldUtils dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Respose Matrix)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfold.h")))  RooUnfoldResponse;
class __attribute__((annotate(R"ATTRDUMP(Unfolding base class: implementations in RooUnfoldBayes, RooUnfoldSvd, RooUnfoldBinByBin, RooUnfoldTUnfold, and RooUnfoldInvert)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfold.h")))  RooUnfold;
class __attribute__((annotate(R"ATTRDUMP(Bayesian Unfolding)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldBayes.h")))  RooUnfoldBayes;
class __attribute__((annotate(R"ATTRDUMP(Bin-by-bin unfolding)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldBinByBin.h")))  RooUnfoldBinByBin;
class __attribute__((annotate(R"ATTRDUMP(Show unfolding errors)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldErrors.h")))  RooUnfoldErrors;
class __attribute__((annotate(R"ATTRDUMP(Unregularised unfolding)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldInvert.h")))  RooUnfoldInvert;
class __attribute__((annotate(R"ATTRDUMP(Optimisation of unfolding regularisation parameter)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldParms.h")))  RooUnfoldParms;
class __attribute__((annotate(R"ATTRDUMP(SVD Unfolding (interface to TSVDUnfold))ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldSvd.h")))  RooUnfoldSvd;
class __attribute__((annotate(R"ATTRDUMP(Interface to TUnfold)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUnfold/RooUnfoldTUnfold.h")))  RooUnfoldTUnfold;
namespace MinervaUnfold{class __attribute__((annotate("$clingAutoload$MinervaUnfold/MnvUnfold.h")))  MnvUnfold;}
namespace std{template <class _CharT> struct __attribute__((annotate("$clingAutoload$bits/char_traits.h")))  __attribute__((annotate("$clingAutoload$string")))  char_traits;
}
namespace std{template <typename > class __attribute__((annotate("$clingAutoload$bits/memoryfwd.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace MinervaUnfold{class __attribute__((annotate("$clingAutoload$MinervaUnfold/MnvResponse.h")))  MnvResponse;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "UnfoldUtils dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef __GCC_XML__
  #define __GCC_XML__ 1
#endif
#ifndef MNVROOT6
  #define MNVROOT6 1
#endif
#ifndef FORM
  #define FORM 1
#endif
#ifndef BUILD_SHARED_LIBS
  #define BUILD_SHARED_LIBS OFF
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef UNFOLDUTILSDICT_H 
#define UNFOLDUTILSDICT_H 1

// Include files for UnfoldUtils dictionary.

/** @file UnfoldUtilsDict.h
 *  
 *
 *  @author Jeremy Wolcott <jwolcott@fnal.gov>
 *  @date   2014-06-04
 */
// ============================================================================
// UnfoldUtils
// ============================================================================

// here we need to include all the header files
// for the classes we want to make dictionaries of

#include <vector>

#include "RooUnfold/RooUnfold.h"
#include "RooUnfold/RooUnfoldBayes.h"
#include "RooUnfold/RooUnfoldBinByBin.h"
//#include "../RooUnfold/RooUnfoldDagostini.h"  # isn't built by default in RooUnfold anyhow
#include "RooUnfold/RooUnfoldErrors.h"
#include "RooUnfold/RooUnfoldInvert.h"
#include "RooUnfold/RooUnfoldParms.h"
#include "RooUnfold/RooUnfoldResponse.h"
#include "RooUnfold/RooUnfoldSvd.h"
#include "RooUnfold/RooUnfoldTUnfold.h"

#include "MinervaUnfold/MnvUnfold.h"
#include "MinervaUnfold/MnvResponse.h"

// this garbage is necessary so that gccxml is able to create dictionaries for these custom containers
// see: http://root.cern.ch/root/roottalk/roottalk10/0035.html
#ifdef __GCCXML__
template class std::map<std::string, std::string>;
template class std::map<std::string,int>;
#endif

#endif // UNFOLDUTILSDICT_H


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MinervaUnfold::MnvResponse", payloadCode, "@",
"MinervaUnfold::MnvUnfold", payloadCode, "@",
"RooUnfold", payloadCode, "@",
"RooUnfoldBayes", payloadCode, "@",
"RooUnfoldBinByBin", payloadCode, "@",
"RooUnfoldErrors", payloadCode, "@",
"RooUnfoldInvert", payloadCode, "@",
"RooUnfoldParms", payloadCode, "@",
"RooUnfoldResponse", payloadCode, "@",
"RooUnfoldSvd", payloadCode, "@",
"RooUnfoldTUnfold", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("UnfoldUtils",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_UnfoldUtils_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_UnfoldUtils_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_UnfoldUtils() {
  TriggerDictionaryInitialization_UnfoldUtils_Impl();
}
