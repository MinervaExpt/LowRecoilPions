// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MATmIMINERvA

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
#include "/minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/MAT-MINERvA/dict/MAT-MINERvADict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *PlotUtilscLcLweightRPA_Dictionary();
   static void PlotUtilscLcLweightRPA_TClassManip(TClass*);
   static void delete_PlotUtilscLcLweightRPA(void *p);
   static void deleteArray_PlotUtilscLcLweightRPA(void *p);
   static void destruct_PlotUtilscLcLweightRPA(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::weightRPA*)
   {
      ::PlotUtils::weightRPA *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::weightRPA));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::weightRPA", "weightRPA.h", 46,
                  typeid(::PlotUtils::weightRPA), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLweightRPA_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::weightRPA) );
      instance.SetDelete(&delete_PlotUtilscLcLweightRPA);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLweightRPA);
      instance.SetDestructor(&destruct_PlotUtilscLcLweightRPA);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::weightRPA*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::weightRPA*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::weightRPA*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLweightRPA_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::weightRPA*)0x0)->GetClass();
      PlotUtilscLcLweightRPA_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLweightRPA_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLweight_2p2h_Dictionary();
   static void PlotUtilscLcLweight_2p2h_TClassManip(TClass*);
   static void delete_PlotUtilscLcLweight_2p2h(void *p);
   static void deleteArray_PlotUtilscLcLweight_2p2h(void *p);
   static void destruct_PlotUtilscLcLweight_2p2h(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::weight_2p2h*)
   {
      ::PlotUtils::weight_2p2h *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::weight_2p2h));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::weight_2p2h", "weight_2p2h.h", 20,
                  typeid(::PlotUtils::weight_2p2h), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLweight_2p2h_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::weight_2p2h) );
      instance.SetDelete(&delete_PlotUtilscLcLweight_2p2h);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLweight_2p2h);
      instance.SetDestructor(&destruct_PlotUtilscLcLweight_2p2h);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::weight_2p2h*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::weight_2p2h*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::weight_2p2h*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLweight_2p2h_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::weight_2p2h*)0x0)->GetClass();
      PlotUtilscLcLweight_2p2h_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLweight_2p2h_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLweightLowQ2Pi_Dictionary();
   static void PlotUtilscLcLweightLowQ2Pi_TClassManip(TClass*);
   static void *new_PlotUtilscLcLweightLowQ2Pi(void *p = 0);
   static void *newArray_PlotUtilscLcLweightLowQ2Pi(Long_t size, void *p);
   static void delete_PlotUtilscLcLweightLowQ2Pi(void *p);
   static void deleteArray_PlotUtilscLcLweightLowQ2Pi(void *p);
   static void destruct_PlotUtilscLcLweightLowQ2Pi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::weightLowQ2Pi*)
   {
      ::PlotUtils::weightLowQ2Pi *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::weightLowQ2Pi));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::weightLowQ2Pi", "weightLowQ2Pi.h", 18,
                  typeid(::PlotUtils::weightLowQ2Pi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLweightLowQ2Pi_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::weightLowQ2Pi) );
      instance.SetNew(&new_PlotUtilscLcLweightLowQ2Pi);
      instance.SetNewArray(&newArray_PlotUtilscLcLweightLowQ2Pi);
      instance.SetDelete(&delete_PlotUtilscLcLweightLowQ2Pi);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLweightLowQ2Pi);
      instance.SetDestructor(&destruct_PlotUtilscLcLweightLowQ2Pi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::weightLowQ2Pi*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::weightLowQ2Pi*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::weightLowQ2Pi*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLweightLowQ2Pi_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::weightLowQ2Pi*)0x0)->GetClass();
      PlotUtilscLcLweightLowQ2Pi_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLweightLowQ2Pi_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLweightDIS_Dictionary();
   static void PlotUtilscLcLweightDIS_TClassManip(TClass*);
   static void delete_PlotUtilscLcLweightDIS(void *p);
   static void deleteArray_PlotUtilscLcLweightDIS(void *p);
   static void destruct_PlotUtilscLcLweightDIS(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::weightDIS*)
   {
      ::PlotUtils::weightDIS *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::weightDIS));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::weightDIS", "weightDIS.h", 27,
                  typeid(::PlotUtils::weightDIS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLweightDIS_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::weightDIS) );
      instance.SetDelete(&delete_PlotUtilscLcLweightDIS);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLweightDIS);
      instance.SetDestructor(&destruct_PlotUtilscLcLweightDIS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::weightDIS*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::weightDIS*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::weightDIS*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLweightDIS_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::weightDIS*)0x0)->GetClass();
      PlotUtilscLcLweightDIS_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLweightDIS_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLweightZExp_Dictionary();
   static void PlotUtilscLcLweightZExp_TClassManip(TClass*);
   static void delete_PlotUtilscLcLweightZExp(void *p);
   static void deleteArray_PlotUtilscLcLweightZExp(void *p);
   static void destruct_PlotUtilscLcLweightZExp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::weightZExp*)
   {
      ::PlotUtils::weightZExp *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::weightZExp));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::weightZExp", "weightZExp.h", 28,
                  typeid(::PlotUtils::weightZExp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLweightZExp_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::weightZExp) );
      instance.SetDelete(&delete_PlotUtilscLcLweightZExp);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLweightZExp);
      instance.SetDestructor(&destruct_PlotUtilscLcLweightZExp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::weightZExp*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::weightZExp*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::weightZExp*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLweightZExp_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::weightZExp*)0x0)->GetClass();
      PlotUtilscLcLweightZExp_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLweightZExp_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLTargetUtils_Dictionary();
   static void PlotUtilscLcLTargetUtils_TClassManip(TClass*);
   static void *new_PlotUtilscLcLTargetUtils(void *p = 0);
   static void *newArray_PlotUtilscLcLTargetUtils(Long_t size, void *p);
   static void delete_PlotUtilscLcLTargetUtils(void *p);
   static void deleteArray_PlotUtilscLcLTargetUtils(void *p);
   static void destruct_PlotUtilscLcLTargetUtils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::TargetUtils*)
   {
      ::PlotUtils::TargetUtils *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::TargetUtils));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::TargetUtils", "utilities/TargetUtils.h", 379,
                  typeid(::PlotUtils::TargetUtils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLTargetUtils_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::TargetUtils) );
      instance.SetNew(&new_PlotUtilscLcLTargetUtils);
      instance.SetNewArray(&newArray_PlotUtilscLcLTargetUtils);
      instance.SetDelete(&delete_PlotUtilscLcLTargetUtils);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLTargetUtils);
      instance.SetDestructor(&destruct_PlotUtilscLcLTargetUtils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::TargetUtils*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::TargetUtils*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::TargetUtils*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLTargetUtils_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::TargetUtils*)0x0)->GetClass();
      PlotUtilscLcLTargetUtils_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLTargetUtils_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLMinervaUniverse_Dictionary();
   static void PlotUtilscLcLMinervaUniverse_TClassManip(TClass*);
   static void *new_PlotUtilscLcLMinervaUniverse(void *p = 0);
   static void *newArray_PlotUtilscLcLMinervaUniverse(Long_t size, void *p);
   static void delete_PlotUtilscLcLMinervaUniverse(void *p);
   static void deleteArray_PlotUtilscLcLMinervaUniverse(void *p);
   static void destruct_PlotUtilscLcLMinervaUniverse(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::MinervaUniverse*)
   {
      ::PlotUtils::MinervaUniverse *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::MinervaUniverse));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::MinervaUniverse", "MinervaUniverse.h", 19,
                  typeid(::PlotUtils::MinervaUniverse), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLMinervaUniverse_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::MinervaUniverse) );
      instance.SetNew(&new_PlotUtilscLcLMinervaUniverse);
      instance.SetNewArray(&newArray_PlotUtilscLcLMinervaUniverse);
      instance.SetDelete(&delete_PlotUtilscLcLMinervaUniverse);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLMinervaUniverse);
      instance.SetDestructor(&destruct_PlotUtilscLcLMinervaUniverse);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::MinervaUniverse*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::MinervaUniverse*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::MinervaUniverse*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLMinervaUniverse_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::MinervaUniverse*)0x0)->GetClass();
      PlotUtilscLcLMinervaUniverse_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLMinervaUniverse_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlotUtilscLcLPOTCounter_Dictionary();
   static void PlotUtilscLcLPOTCounter_TClassManip(TClass*);
   static void *new_PlotUtilscLcLPOTCounter(void *p = 0);
   static void *newArray_PlotUtilscLcLPOTCounter(Long_t size, void *p);
   static void delete_PlotUtilscLcLPOTCounter(void *p);
   static void deleteArray_PlotUtilscLcLPOTCounter(void *p);
   static void destruct_PlotUtilscLcLPOTCounter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlotUtils::POTCounter*)
   {
      ::PlotUtils::POTCounter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlotUtils::POTCounter));
      static ::ROOT::TGenericClassInfo 
         instance("PlotUtils::POTCounter", "POTCounter.h", 38,
                  typeid(::PlotUtils::POTCounter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PlotUtilscLcLPOTCounter_Dictionary, isa_proxy, 4,
                  sizeof(::PlotUtils::POTCounter) );
      instance.SetNew(&new_PlotUtilscLcLPOTCounter);
      instance.SetNewArray(&newArray_PlotUtilscLcLPOTCounter);
      instance.SetDelete(&delete_PlotUtilscLcLPOTCounter);
      instance.SetDeleteArray(&deleteArray_PlotUtilscLcLPOTCounter);
      instance.SetDestructor(&destruct_PlotUtilscLcLPOTCounter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlotUtils::POTCounter*)
   {
      return GenerateInitInstanceLocal((::PlotUtils::POTCounter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PlotUtils::POTCounter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlotUtilscLcLPOTCounter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlotUtils::POTCounter*)0x0)->GetClass();
      PlotUtilscLcLPOTCounter_TClassManip(theClass);
   return theClass;
   }

   static void PlotUtilscLcLPOTCounter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLweightRPA(void *p) {
      delete ((::PlotUtils::weightRPA*)p);
   }
   static void deleteArray_PlotUtilscLcLweightRPA(void *p) {
      delete [] ((::PlotUtils::weightRPA*)p);
   }
   static void destruct_PlotUtilscLcLweightRPA(void *p) {
      typedef ::PlotUtils::weightRPA current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::weightRPA

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLweight_2p2h(void *p) {
      delete ((::PlotUtils::weight_2p2h*)p);
   }
   static void deleteArray_PlotUtilscLcLweight_2p2h(void *p) {
      delete [] ((::PlotUtils::weight_2p2h*)p);
   }
   static void destruct_PlotUtilscLcLweight_2p2h(void *p) {
      typedef ::PlotUtils::weight_2p2h current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::weight_2p2h

namespace ROOT {
   // Wrappers around operator new
   static void *new_PlotUtilscLcLweightLowQ2Pi(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::weightLowQ2Pi : new ::PlotUtils::weightLowQ2Pi;
   }
   static void *newArray_PlotUtilscLcLweightLowQ2Pi(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::weightLowQ2Pi[nElements] : new ::PlotUtils::weightLowQ2Pi[nElements];
   }
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLweightLowQ2Pi(void *p) {
      delete ((::PlotUtils::weightLowQ2Pi*)p);
   }
   static void deleteArray_PlotUtilscLcLweightLowQ2Pi(void *p) {
      delete [] ((::PlotUtils::weightLowQ2Pi*)p);
   }
   static void destruct_PlotUtilscLcLweightLowQ2Pi(void *p) {
      typedef ::PlotUtils::weightLowQ2Pi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::weightLowQ2Pi

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLweightDIS(void *p) {
      delete ((::PlotUtils::weightDIS*)p);
   }
   static void deleteArray_PlotUtilscLcLweightDIS(void *p) {
      delete [] ((::PlotUtils::weightDIS*)p);
   }
   static void destruct_PlotUtilscLcLweightDIS(void *p) {
      typedef ::PlotUtils::weightDIS current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::weightDIS

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLweightZExp(void *p) {
      delete ((::PlotUtils::weightZExp*)p);
   }
   static void deleteArray_PlotUtilscLcLweightZExp(void *p) {
      delete [] ((::PlotUtils::weightZExp*)p);
   }
   static void destruct_PlotUtilscLcLweightZExp(void *p) {
      typedef ::PlotUtils::weightZExp current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::weightZExp

namespace ROOT {
   // Wrappers around operator new
   static void *new_PlotUtilscLcLTargetUtils(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::TargetUtils : new ::PlotUtils::TargetUtils;
   }
   static void *newArray_PlotUtilscLcLTargetUtils(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::TargetUtils[nElements] : new ::PlotUtils::TargetUtils[nElements];
   }
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLTargetUtils(void *p) {
      delete ((::PlotUtils::TargetUtils*)p);
   }
   static void deleteArray_PlotUtilscLcLTargetUtils(void *p) {
      delete [] ((::PlotUtils::TargetUtils*)p);
   }
   static void destruct_PlotUtilscLcLTargetUtils(void *p) {
      typedef ::PlotUtils::TargetUtils current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::TargetUtils

namespace ROOT {
   // Wrappers around operator new
   static void *new_PlotUtilscLcLMinervaUniverse(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::MinervaUniverse : new ::PlotUtils::MinervaUniverse;
   }
   static void *newArray_PlotUtilscLcLMinervaUniverse(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::MinervaUniverse[nElements] : new ::PlotUtils::MinervaUniverse[nElements];
   }
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLMinervaUniverse(void *p) {
      delete ((::PlotUtils::MinervaUniverse*)p);
   }
   static void deleteArray_PlotUtilscLcLMinervaUniverse(void *p) {
      delete [] ((::PlotUtils::MinervaUniverse*)p);
   }
   static void destruct_PlotUtilscLcLMinervaUniverse(void *p) {
      typedef ::PlotUtils::MinervaUniverse current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::MinervaUniverse

namespace ROOT {
   // Wrappers around operator new
   static void *new_PlotUtilscLcLPOTCounter(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::POTCounter : new ::PlotUtils::POTCounter;
   }
   static void *newArray_PlotUtilscLcLPOTCounter(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PlotUtils::POTCounter[nElements] : new ::PlotUtils::POTCounter[nElements];
   }
   // Wrapper around operator delete
   static void delete_PlotUtilscLcLPOTCounter(void *p) {
      delete ((::PlotUtils::POTCounter*)p);
   }
   static void deleteArray_PlotUtilscLcLPOTCounter(void *p) {
      delete [] ((::PlotUtils::POTCounter*)p);
   }
   static void destruct_PlotUtilscLcLPOTCounter(void *p) {
      typedef ::PlotUtils::POTCounter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlotUtils::POTCounter

namespace {
  void TriggerDictionaryInitialization_MATmIMINERvA_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "MATmIMINERvA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$weightRPA.h")))  weightRPA;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$weight_2p2h.h")))  weight_2p2h;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$weightLowQ2Pi.h")))  weightLowQ2Pi;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$weightDIS.h")))  weightDIS;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$weightZExp.h")))  weightZExp;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$utilities/TargetUtils.h")))  __attribute__((annotate("$clingAutoload$MinervaUniverse.h")))  TargetUtils;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$MinervaUniverse.h")))  MinervaUniverse;}
namespace PlotUtils{class __attribute__((annotate("$clingAutoload$POTCounter.h")))  POTCounter;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "MATmIMINERvA dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef __GCC_XML__
  #define __GCC_XML__ 1
#endif
#ifndef NCINTEX
  #define NCINTEX 1
#endif
#ifndef FORM
  #define FORM 1
#endif
#ifndef MNVROOT6
  #define MNVROOT6 1
#endif
#ifndef PLOTUTILS_STANDALONE
  #define PLOTUTILS_STANDALONE 1
#endif
#ifndef BUILD_SHARED_LIBS
  #define BUILD_SHARED_LIBS OFF
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef MATMINERVADICT_H 
#define MATMINERVADICT_H 1

// Include files for PlotUtils dictionary.

/** @file PlotUtilsDict.h
 *  
 *
 *  @author Jeremy Wolcott <jwolcott@fnal.gov>
 *  @date   2012-11-25
 */
// ============================================================================
// PlotUtils
// ============================================================================

// here we need to include all the header files
// for the classes we want to make dictionaries of

#include <vector>

// PlotUtils weight classes
#include "weightRPA.h"
#include "weight_2p2h.h"
#include "weightLowQ2Pi.h"
#include "weightDIS.h"
#include "weightZExp.h"

//PlotUtils systematic universes classes (new sys framework)
#include "MinervaUniverse.h"
#include "GenieSystematics.h"
#include "GeantHadronSystematics.h"
#include "MnvTuneSystematics.h"
#include "MinosEfficiencySystematics.h"
#include "MuonSystematics.h"
#include "MuonResolutionSystematics.h"
#include "MichelSystematics.h"
#include "AngleSystematics.h"
#include "ResponseSystematics.h"

//Utils
#include "TargetUtils.h"
#include "POTCounter.h"


//TODO: Do I need this?
//#include "ErrorHandler.h"

// this garbage is necessary so that gccxml is able to create dictionaries for these custom containers
// (since it otherwise doesn't know which specific version of these templated classes to instantiate)
// see: http://root.cern.ch/root/roottalk/roottalk10/0035.html
// somehow std::map<>s seem to be instantiated somewhere else, so explicit instantiation is not necessary?
#ifdef __GCCXML__

// Use extern keyword because these functions are instantiated in FluxReweighter.cxx already

#endif
#endif // MATMINERVADICT_H


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PlotUtils::MinervaUniverse", payloadCode, "@",
"PlotUtils::POTCounter", payloadCode, "@",
"PlotUtils::TargetUtils", payloadCode, "@",
"PlotUtils::weightDIS", payloadCode, "@",
"PlotUtils::weightLowQ2Pi", payloadCode, "@",
"PlotUtils::weightRPA", payloadCode, "@",
"PlotUtils::weightZExp", payloadCode, "@",
"PlotUtils::weight_2p2h", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MAT-MINERvA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MATmIMINERvA_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MATmIMINERvA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MATmIMINERvA() {
  TriggerDictionaryInitialization_MATmIMINERvA_Impl();
}
