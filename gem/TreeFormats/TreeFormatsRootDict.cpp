// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TreeFormatsRootDict

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
#include "pdpcevent.h"
#include "GemEvent.h"
#include "PDPCFarichHitData.h"
#include "PDPCFarichRecoEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_PdpcHit(void *p = 0);
   static void *newArray_PdpcHit(Long_t size, void *p);
   static void delete_PdpcHit(void *p);
   static void deleteArray_PdpcHit(void *p);
   static void destruct_PdpcHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PdpcHit*)
   {
      ::PdpcHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PdpcHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PdpcHit", ::PdpcHit::Class_Version(), "pdpcevent.h", 10,
                  typeid(::PdpcHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PdpcHit::Dictionary, isa_proxy, 4,
                  sizeof(::PdpcHit) );
      instance.SetNew(&new_PdpcHit);
      instance.SetNewArray(&newArray_PdpcHit);
      instance.SetDelete(&delete_PdpcHit);
      instance.SetDeleteArray(&deleteArray_PdpcHit);
      instance.SetDestructor(&destruct_PdpcHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PdpcHit*)
   {
      return GenerateInitInstanceLocal((::PdpcHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PdpcHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GemCluster(void *p = 0);
   static void *newArray_GemCluster(Long_t size, void *p);
   static void delete_GemCluster(void *p);
   static void deleteArray_GemCluster(void *p);
   static void destruct_GemCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GemCluster*)
   {
      ::GemCluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GemCluster >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GemCluster", ::GemCluster::Class_Version(), "GemEvent.h", 28,
                  typeid(::GemCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GemCluster::Dictionary, isa_proxy, 4,
                  sizeof(::GemCluster) );
      instance.SetNew(&new_GemCluster);
      instance.SetNewArray(&newArray_GemCluster);
      instance.SetDelete(&delete_GemCluster);
      instance.SetDeleteArray(&deleteArray_GemCluster);
      instance.SetDestructor(&destruct_GemCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GemCluster*)
   {
      return GenerateInitInstanceLocal((::GemCluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GemCluster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PDPCFarichHitData(void *p = 0);
   static void *newArray_PDPCFarichHitData(Long_t size, void *p);
   static void delete_PDPCFarichHitData(void *p);
   static void deleteArray_PDPCFarichHitData(void *p);
   static void destruct_PDPCFarichHitData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PDPCFarichHitData*)
   {
      ::PDPCFarichHitData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PDPCFarichHitData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PDPCFarichHitData", ::PDPCFarichHitData::Class_Version(), "PDPCFarichHitData.h", 6,
                  typeid(::PDPCFarichHitData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PDPCFarichHitData::Dictionary, isa_proxy, 4,
                  sizeof(::PDPCFarichHitData) );
      instance.SetNew(&new_PDPCFarichHitData);
      instance.SetNewArray(&newArray_PDPCFarichHitData);
      instance.SetDelete(&delete_PDPCFarichHitData);
      instance.SetDeleteArray(&deleteArray_PDPCFarichHitData);
      instance.SetDestructor(&destruct_PDPCFarichHitData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PDPCFarichHitData*)
   {
      return GenerateInitInstanceLocal((::PDPCFarichHitData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PDPCFarichHitData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PDPCFarichRecoHit(void *p = 0);
   static void *newArray_PDPCFarichRecoHit(Long_t size, void *p);
   static void delete_PDPCFarichRecoHit(void *p);
   static void deleteArray_PDPCFarichRecoHit(void *p);
   static void destruct_PDPCFarichRecoHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PDPCFarichRecoHit*)
   {
      ::PDPCFarichRecoHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PDPCFarichRecoHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PDPCFarichRecoHit", ::PDPCFarichRecoHit::Class_Version(), "PDPCFarichRecoEvent.h", 8,
                  typeid(::PDPCFarichRecoHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PDPCFarichRecoHit::Dictionary, isa_proxy, 4,
                  sizeof(::PDPCFarichRecoHit) );
      instance.SetNew(&new_PDPCFarichRecoHit);
      instance.SetNewArray(&newArray_PDPCFarichRecoHit);
      instance.SetDelete(&delete_PDPCFarichRecoHit);
      instance.SetDeleteArray(&deleteArray_PDPCFarichRecoHit);
      instance.SetDestructor(&destruct_PDPCFarichRecoHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PDPCFarichRecoHit*)
   {
      return GenerateInitInstanceLocal((::PDPCFarichRecoHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PDPCFarichRecoHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PDPCFarichRecoEvent(void *p = 0);
   static void *newArray_PDPCFarichRecoEvent(Long_t size, void *p);
   static void delete_PDPCFarichRecoEvent(void *p);
   static void deleteArray_PDPCFarichRecoEvent(void *p);
   static void destruct_PDPCFarichRecoEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PDPCFarichRecoEvent*)
   {
      ::PDPCFarichRecoEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PDPCFarichRecoEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PDPCFarichRecoEvent", ::PDPCFarichRecoEvent::Class_Version(), "PDPCFarichRecoEvent.h", 137,
                  typeid(::PDPCFarichRecoEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PDPCFarichRecoEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PDPCFarichRecoEvent) );
      instance.SetNew(&new_PDPCFarichRecoEvent);
      instance.SetNewArray(&newArray_PDPCFarichRecoEvent);
      instance.SetDelete(&delete_PDPCFarichRecoEvent);
      instance.SetDeleteArray(&deleteArray_PDPCFarichRecoEvent);
      instance.SetDestructor(&destruct_PDPCFarichRecoEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PDPCFarichRecoEvent*)
   {
      return GenerateInitInstanceLocal((::PDPCFarichRecoEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PDPCFarichRecoEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PdpcHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PdpcHit::Class_Name()
{
   return "PdpcHit";
}

//______________________________________________________________________________
const char *PdpcHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdpcHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PdpcHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PdpcHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PdpcHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdpcHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PdpcHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PdpcHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GemCluster::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GemCluster::Class_Name()
{
   return "GemCluster";
}

//______________________________________________________________________________
const char *GemCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GemCluster*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GemCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GemCluster*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GemCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GemCluster*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GemCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GemCluster*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PDPCFarichHitData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PDPCFarichHitData::Class_Name()
{
   return "PDPCFarichHitData";
}

//______________________________________________________________________________
const char *PDPCFarichHitData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichHitData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PDPCFarichHitData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichHitData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PDPCFarichHitData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichHitData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PDPCFarichHitData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichHitData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PDPCFarichRecoHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PDPCFarichRecoHit::Class_Name()
{
   return "PDPCFarichRecoHit";
}

//______________________________________________________________________________
const char *PDPCFarichRecoHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PDPCFarichRecoHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PDPCFarichRecoHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PDPCFarichRecoHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PDPCFarichRecoEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PDPCFarichRecoEvent::Class_Name()
{
   return "PDPCFarichRecoEvent";
}

//______________________________________________________________________________
const char *PDPCFarichRecoEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PDPCFarichRecoEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PDPCFarichRecoEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PDPCFarichRecoEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PDPCFarichRecoEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PdpcHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class PdpcHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PdpcHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(PdpcHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PdpcHit(void *p) {
      return  p ? new(p) ::PdpcHit : new ::PdpcHit;
   }
   static void *newArray_PdpcHit(Long_t nElements, void *p) {
      return p ? new(p) ::PdpcHit[nElements] : new ::PdpcHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_PdpcHit(void *p) {
      delete ((::PdpcHit*)p);
   }
   static void deleteArray_PdpcHit(void *p) {
      delete [] ((::PdpcHit*)p);
   }
   static void destruct_PdpcHit(void *p) {
      typedef ::PdpcHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PdpcHit

//______________________________________________________________________________
void GemCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class GemCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(GemCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(GemCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GemCluster(void *p) {
      return  p ? new(p) ::GemCluster : new ::GemCluster;
   }
   static void *newArray_GemCluster(Long_t nElements, void *p) {
      return p ? new(p) ::GemCluster[nElements] : new ::GemCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_GemCluster(void *p) {
      delete ((::GemCluster*)p);
   }
   static void deleteArray_GemCluster(void *p) {
      delete [] ((::GemCluster*)p);
   }
   static void destruct_GemCluster(void *p) {
      typedef ::GemCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GemCluster

//______________________________________________________________________________
void PDPCFarichHitData::Streamer(TBuffer &R__b)
{
   // Stream an object of class PDPCFarichHitData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PDPCFarichHitData::Class(),this);
   } else {
      R__b.WriteClassBuffer(PDPCFarichHitData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PDPCFarichHitData(void *p) {
      return  p ? new(p) ::PDPCFarichHitData : new ::PDPCFarichHitData;
   }
   static void *newArray_PDPCFarichHitData(Long_t nElements, void *p) {
      return p ? new(p) ::PDPCFarichHitData[nElements] : new ::PDPCFarichHitData[nElements];
   }
   // Wrapper around operator delete
   static void delete_PDPCFarichHitData(void *p) {
      delete ((::PDPCFarichHitData*)p);
   }
   static void deleteArray_PDPCFarichHitData(void *p) {
      delete [] ((::PDPCFarichHitData*)p);
   }
   static void destruct_PDPCFarichHitData(void *p) {
      typedef ::PDPCFarichHitData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PDPCFarichHitData

//______________________________________________________________________________
void PDPCFarichRecoHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class PDPCFarichRecoHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PDPCFarichRecoHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(PDPCFarichRecoHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PDPCFarichRecoHit(void *p) {
      return  p ? new(p) ::PDPCFarichRecoHit : new ::PDPCFarichRecoHit;
   }
   static void *newArray_PDPCFarichRecoHit(Long_t nElements, void *p) {
      return p ? new(p) ::PDPCFarichRecoHit[nElements] : new ::PDPCFarichRecoHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_PDPCFarichRecoHit(void *p) {
      delete ((::PDPCFarichRecoHit*)p);
   }
   static void deleteArray_PDPCFarichRecoHit(void *p) {
      delete [] ((::PDPCFarichRecoHit*)p);
   }
   static void destruct_PDPCFarichRecoHit(void *p) {
      typedef ::PDPCFarichRecoHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PDPCFarichRecoHit

//______________________________________________________________________________
void PDPCFarichRecoEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PDPCFarichRecoEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PDPCFarichRecoEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PDPCFarichRecoEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PDPCFarichRecoEvent(void *p) {
      return  p ? new(p) ::PDPCFarichRecoEvent : new ::PDPCFarichRecoEvent;
   }
   static void *newArray_PDPCFarichRecoEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PDPCFarichRecoEvent[nElements] : new ::PDPCFarichRecoEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PDPCFarichRecoEvent(void *p) {
      delete ((::PDPCFarichRecoEvent*)p);
   }
   static void deleteArray_PDPCFarichRecoEvent(void *p) {
      delete [] ((::PDPCFarichRecoEvent*)p);
   }
   static void destruct_PDPCFarichRecoEvent(void *p) {
      typedef ::PDPCFarichRecoEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PDPCFarichRecoEvent

namespace {
  void TriggerDictionaryInitialization_TreeFormatsRootDict_Impl() {
    static const char* headers[] = {
"pdpcevent.h",
"GemEvent.h",
"PDPCFarichHitData.h",
"PDPCFarichRecoEvent.h",
0
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/gcf/stark/home/ovtin/development/gem/TreeFormats/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TreeFormatsRootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$pdpcevent.h")))  PdpcHit;
class __attribute__((annotate("$clingAutoload$GemEvent.h")))  GemCluster;
class __attribute__((annotate("$clingAutoload$PDPCFarichHitData.h")))  PDPCFarichHitData;
class __attribute__((annotate("$clingAutoload$PDPCFarichRecoEvent.h")))  PDPCFarichRecoHit;
class __attribute__((annotate("$clingAutoload$PDPCFarichRecoEvent.h")))  PDPCFarichRecoEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TreeFormatsRootDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "pdpcevent.h"
#include "GemEvent.h"
#include "PDPCFarichHitData.h"
#include "PDPCFarichRecoEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"GemCluster", payloadCode, "@",
"PDPCFarichHitData", payloadCode, "@",
"PDPCFarichRecoEvent", payloadCode, "@",
"PDPCFarichRecoHit", payloadCode, "@",
"PdpcHit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TreeFormatsRootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TreeFormatsRootDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TreeFormatsRootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TreeFormatsRootDict() {
  TriggerDictionaryInitialization_TreeFormatsRootDict_Impl();
}
