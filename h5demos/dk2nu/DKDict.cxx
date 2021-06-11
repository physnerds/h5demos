// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DKDict
#define R__NO_DEPRECATION

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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "include/dkmeta.h"
#include "include/dk2nu.h"
#include "include/dflt.h"

// Header files passed via #pragma extra_include

namespace bsim {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *bsim_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("bsim", 0 /*version*/, "include/dkmeta.h", 35,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &bsim_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *bsim_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_bsimcLcLLocation(void *p = 0);
   static void *newArray_bsimcLcLLocation(Long_t size, void *p);
   static void delete_bsimcLcLLocation(void *p);
   static void deleteArray_bsimcLcLLocation(void *p);
   static void destruct_bsimcLcLLocation(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::Location*)
   {
      ::bsim::Location *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::Location >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::Location", ::bsim::Location::Class_Version(), "include/dkmeta.h", 54,
                  typeid(::bsim::Location), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::Location::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::Location) );
      instance.SetNew(&new_bsimcLcLLocation);
      instance.SetNewArray(&newArray_bsimcLcLLocation);
      instance.SetDelete(&delete_bsimcLcLLocation);
      instance.SetDeleteArray(&deleteArray_bsimcLcLLocation);
      instance.SetDestructor(&destruct_bsimcLcLLocation);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::Location*)
   {
      return GenerateInitInstanceLocal((::bsim::Location*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::Location*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLDkMeta(void *p = 0);
   static void *newArray_bsimcLcLDkMeta(Long_t size, void *p);
   static void delete_bsimcLcLDkMeta(void *p);
   static void deleteArray_bsimcLcLDkMeta(void *p);
   static void destruct_bsimcLcLDkMeta(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::DkMeta*)
   {
      ::bsim::DkMeta *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::DkMeta >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::DkMeta", ::bsim::DkMeta::Class_Version(), "include/dkmeta.h", 78,
                  typeid(::bsim::DkMeta), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::DkMeta::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::DkMeta) );
      instance.SetNew(&new_bsimcLcLDkMeta);
      instance.SetNewArray(&newArray_bsimcLcLDkMeta);
      instance.SetDelete(&delete_bsimcLcLDkMeta);
      instance.SetDeleteArray(&deleteArray_bsimcLcLDkMeta);
      instance.SetDestructor(&destruct_bsimcLcLDkMeta);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::DkMeta*)
   {
      return GenerateInitInstanceLocal((::bsim::DkMeta*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::DkMeta*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLNuRay(void *p = 0);
   static void *newArray_bsimcLcLNuRay(Long_t size, void *p);
   static void delete_bsimcLcLNuRay(void *p);
   static void deleteArray_bsimcLcLNuRay(void *p);
   static void destruct_bsimcLcLNuRay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::NuRay*)
   {
      ::bsim::NuRay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::NuRay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::NuRay", ::bsim::NuRay::Class_Version(), "include/dk2nu.h", 94,
                  typeid(::bsim::NuRay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::NuRay::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::NuRay) );
      instance.SetNew(&new_bsimcLcLNuRay);
      instance.SetNewArray(&newArray_bsimcLcLNuRay);
      instance.SetDelete(&delete_bsimcLcLNuRay);
      instance.SetDeleteArray(&deleteArray_bsimcLcLNuRay);
      instance.SetDestructor(&destruct_bsimcLcLNuRay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::NuRay*)
   {
      return GenerateInitInstanceLocal((::bsim::NuRay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::NuRay*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLDecay(void *p = 0);
   static void *newArray_bsimcLcLDecay(Long_t size, void *p);
   static void delete_bsimcLcLDecay(void *p);
   static void deleteArray_bsimcLcLDecay(void *p);
   static void destruct_bsimcLcLDecay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::Decay*)
   {
      ::bsim::Decay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::Decay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::Decay", ::bsim::Decay::Class_Version(), "include/dk2nu.h", 121,
                  typeid(::bsim::Decay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::Decay::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::Decay) );
      instance.SetNew(&new_bsimcLcLDecay);
      instance.SetNewArray(&newArray_bsimcLcLDecay);
      instance.SetDelete(&delete_bsimcLcLDecay);
      instance.SetDeleteArray(&deleteArray_bsimcLcLDecay);
      instance.SetDestructor(&destruct_bsimcLcLDecay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::Decay*)
   {
      return GenerateInitInstanceLocal((::bsim::Decay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::Decay*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLAncestor(void *p = 0);
   static void *newArray_bsimcLcLAncestor(Long_t size, void *p);
   static void delete_bsimcLcLAncestor(void *p);
   static void deleteArray_bsimcLcLAncestor(void *p);
   static void destruct_bsimcLcLAncestor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::Ancestor*)
   {
      ::bsim::Ancestor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::Ancestor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::Ancestor", ::bsim::Ancestor::Class_Version(), "include/dk2nu.h", 183,
                  typeid(::bsim::Ancestor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::Ancestor::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::Ancestor) );
      instance.SetNew(&new_bsimcLcLAncestor);
      instance.SetNewArray(&newArray_bsimcLcLAncestor);
      instance.SetDelete(&delete_bsimcLcLAncestor);
      instance.SetDeleteArray(&deleteArray_bsimcLcLAncestor);
      instance.SetDestructor(&destruct_bsimcLcLAncestor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::Ancestor*)
   {
      return GenerateInitInstanceLocal((::bsim::Ancestor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::Ancestor*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLTgtExit(void *p = 0);
   static void *newArray_bsimcLcLTgtExit(Long_t size, void *p);
   static void delete_bsimcLcLTgtExit(void *p);
   static void deleteArray_bsimcLcLTgtExit(void *p);
   static void destruct_bsimcLcLTgtExit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::TgtExit*)
   {
      ::bsim::TgtExit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::TgtExit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::TgtExit", ::bsim::TgtExit::Class_Version(), "include/dk2nu.h", 264,
                  typeid(::bsim::TgtExit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::TgtExit::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::TgtExit) );
      instance.SetNew(&new_bsimcLcLTgtExit);
      instance.SetNewArray(&newArray_bsimcLcLTgtExit);
      instance.SetDelete(&delete_bsimcLcLTgtExit);
      instance.SetDeleteArray(&deleteArray_bsimcLcLTgtExit);
      instance.SetDestructor(&destruct_bsimcLcLTgtExit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::TgtExit*)
   {
      return GenerateInitInstanceLocal((::bsim::TgtExit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::TgtExit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLTraj(void *p = 0);
   static void *newArray_bsimcLcLTraj(Long_t size, void *p);
   static void delete_bsimcLcLTraj(void *p);
   static void deleteArray_bsimcLcLTraj(void *p);
   static void destruct_bsimcLcLTraj(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::Traj*)
   {
      ::bsim::Traj *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::Traj >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::Traj", ::bsim::Traj::Class_Version(), "include/dk2nu.h", 293,
                  typeid(::bsim::Traj), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::Traj::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::Traj) );
      instance.SetNew(&new_bsimcLcLTraj);
      instance.SetNewArray(&newArray_bsimcLcLTraj);
      instance.SetDelete(&delete_bsimcLcLTraj);
      instance.SetDeleteArray(&deleteArray_bsimcLcLTraj);
      instance.SetDestructor(&destruct_bsimcLcLTraj);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::Traj*)
   {
      return GenerateInitInstanceLocal((::bsim::Traj*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::Traj*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_bsimcLcLDk2Nu(void *p = 0);
   static void *newArray_bsimcLcLDk2Nu(Long_t size, void *p);
   static void delete_bsimcLcLDk2Nu(void *p);
   static void deleteArray_bsimcLcLDk2Nu(void *p);
   static void destruct_bsimcLcLDk2Nu(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::bsim::Dk2Nu*)
   {
      ::bsim::Dk2Nu *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::bsim::Dk2Nu >(0);
      static ::ROOT::TGenericClassInfo 
         instance("bsim::Dk2Nu", ::bsim::Dk2Nu::Class_Version(), "include/dk2nu.h", 318,
                  typeid(::bsim::Dk2Nu), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::bsim::Dk2Nu::Dictionary, isa_proxy, 4,
                  sizeof(::bsim::Dk2Nu) );
      instance.SetNew(&new_bsimcLcLDk2Nu);
      instance.SetNewArray(&newArray_bsimcLcLDk2Nu);
      instance.SetDelete(&delete_bsimcLcLDk2Nu);
      instance.SetDeleteArray(&deleteArray_bsimcLcLDk2Nu);
      instance.SetDestructor(&destruct_bsimcLcLDk2Nu);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::bsim::Dk2Nu*)
   {
      return GenerateInitInstanceLocal((::bsim::Dk2Nu*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::bsim::Dk2Nu*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr Location::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Location::Class_Name()
{
   return "bsim::Location";
}

//______________________________________________________________________________
const char *Location::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Location*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Location::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Location*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Location::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Location*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Location::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Location*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr DkMeta::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DkMeta::Class_Name()
{
   return "bsim::DkMeta";
}

//______________________________________________________________________________
const char *DkMeta::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::DkMeta*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DkMeta::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::DkMeta*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DkMeta::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::DkMeta*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DkMeta::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::DkMeta*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr NuRay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NuRay::Class_Name()
{
   return "bsim::NuRay";
}

//______________________________________________________________________________
const char *NuRay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::NuRay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NuRay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::NuRay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NuRay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::NuRay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NuRay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::NuRay*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr Decay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Decay::Class_Name()
{
   return "bsim::Decay";
}

//______________________________________________________________________________
const char *Decay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Decay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Decay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Decay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Decay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Decay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Decay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Decay*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr Ancestor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Ancestor::Class_Name()
{
   return "bsim::Ancestor";
}

//______________________________________________________________________________
const char *Ancestor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Ancestor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Ancestor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Ancestor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ancestor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Ancestor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ancestor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Ancestor*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr TgtExit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TgtExit::Class_Name()
{
   return "bsim::TgtExit";
}

//______________________________________________________________________________
const char *TgtExit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::TgtExit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TgtExit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::TgtExit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TgtExit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::TgtExit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TgtExit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::TgtExit*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr Traj::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Traj::Class_Name()
{
   return "bsim::Traj";
}

//______________________________________________________________________________
const char *Traj::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Traj*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Traj::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Traj*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Traj::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Traj*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Traj::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Traj*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
atomic_TClass_ptr Dk2Nu::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Dk2Nu::Class_Name()
{
   return "bsim::Dk2Nu";
}

//______________________________________________________________________________
const char *Dk2Nu::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Dk2Nu*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Dk2Nu::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::bsim::Dk2Nu*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Dk2Nu::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Dk2Nu*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Dk2Nu::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::bsim::Dk2Nu*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace bsim
namespace bsim {
//______________________________________________________________________________
void Location::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::Location.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::Location::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::Location::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLLocation(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Location : new ::bsim::Location;
   }
   static void *newArray_bsimcLcLLocation(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Location[nElements] : new ::bsim::Location[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLLocation(void *p) {
      delete ((::bsim::Location*)p);
   }
   static void deleteArray_bsimcLcLLocation(void *p) {
      delete [] ((::bsim::Location*)p);
   }
   static void destruct_bsimcLcLLocation(void *p) {
      typedef ::bsim::Location current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::Location

namespace bsim {
//______________________________________________________________________________
void DkMeta::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::DkMeta.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::DkMeta::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::DkMeta::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLDkMeta(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::DkMeta : new ::bsim::DkMeta;
   }
   static void *newArray_bsimcLcLDkMeta(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::DkMeta[nElements] : new ::bsim::DkMeta[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLDkMeta(void *p) {
      delete ((::bsim::DkMeta*)p);
   }
   static void deleteArray_bsimcLcLDkMeta(void *p) {
      delete [] ((::bsim::DkMeta*)p);
   }
   static void destruct_bsimcLcLDkMeta(void *p) {
      typedef ::bsim::DkMeta current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::DkMeta

namespace bsim {
//______________________________________________________________________________
void NuRay::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::NuRay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::NuRay::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::NuRay::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLNuRay(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::NuRay : new ::bsim::NuRay;
   }
   static void *newArray_bsimcLcLNuRay(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::NuRay[nElements] : new ::bsim::NuRay[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLNuRay(void *p) {
      delete ((::bsim::NuRay*)p);
   }
   static void deleteArray_bsimcLcLNuRay(void *p) {
      delete [] ((::bsim::NuRay*)p);
   }
   static void destruct_bsimcLcLNuRay(void *p) {
      typedef ::bsim::NuRay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::NuRay

namespace bsim {
//______________________________________________________________________________
void Decay::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::Decay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::Decay::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::Decay::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLDecay(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Decay : new ::bsim::Decay;
   }
   static void *newArray_bsimcLcLDecay(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Decay[nElements] : new ::bsim::Decay[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLDecay(void *p) {
      delete ((::bsim::Decay*)p);
   }
   static void deleteArray_bsimcLcLDecay(void *p) {
      delete [] ((::bsim::Decay*)p);
   }
   static void destruct_bsimcLcLDecay(void *p) {
      typedef ::bsim::Decay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::Decay

namespace bsim {
//______________________________________________________________________________
void Ancestor::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::Ancestor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::Ancestor::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::Ancestor::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLAncestor(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Ancestor : new ::bsim::Ancestor;
   }
   static void *newArray_bsimcLcLAncestor(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Ancestor[nElements] : new ::bsim::Ancestor[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLAncestor(void *p) {
      delete ((::bsim::Ancestor*)p);
   }
   static void deleteArray_bsimcLcLAncestor(void *p) {
      delete [] ((::bsim::Ancestor*)p);
   }
   static void destruct_bsimcLcLAncestor(void *p) {
      typedef ::bsim::Ancestor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::Ancestor

namespace bsim {
//______________________________________________________________________________
void TgtExit::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::TgtExit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::TgtExit::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::TgtExit::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLTgtExit(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::TgtExit : new ::bsim::TgtExit;
   }
   static void *newArray_bsimcLcLTgtExit(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::TgtExit[nElements] : new ::bsim::TgtExit[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLTgtExit(void *p) {
      delete ((::bsim::TgtExit*)p);
   }
   static void deleteArray_bsimcLcLTgtExit(void *p) {
      delete [] ((::bsim::TgtExit*)p);
   }
   static void destruct_bsimcLcLTgtExit(void *p) {
      typedef ::bsim::TgtExit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::TgtExit

namespace bsim {
//______________________________________________________________________________
void Traj::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::Traj.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::Traj::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::Traj::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLTraj(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Traj : new ::bsim::Traj;
   }
   static void *newArray_bsimcLcLTraj(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Traj[nElements] : new ::bsim::Traj[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLTraj(void *p) {
      delete ((::bsim::Traj*)p);
   }
   static void deleteArray_bsimcLcLTraj(void *p) {
      delete [] ((::bsim::Traj*)p);
   }
   static void destruct_bsimcLcLTraj(void *p) {
      typedef ::bsim::Traj current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::Traj

namespace bsim {
//______________________________________________________________________________
void Dk2Nu::Streamer(TBuffer &R__b)
{
   // Stream an object of class bsim::Dk2Nu.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(bsim::Dk2Nu::Class(),this);
   } else {
      R__b.WriteClassBuffer(bsim::Dk2Nu::Class(),this);
   }
}

} // namespace bsim
namespace ROOT {
   // Wrappers around operator new
   static void *new_bsimcLcLDk2Nu(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Dk2Nu : new ::bsim::Dk2Nu;
   }
   static void *newArray_bsimcLcLDk2Nu(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::bsim::Dk2Nu[nElements] : new ::bsim::Dk2Nu[nElements];
   }
   // Wrapper around operator delete
   static void delete_bsimcLcLDk2Nu(void *p) {
      delete ((::bsim::Dk2Nu*)p);
   }
   static void deleteArray_bsimcLcLDk2Nu(void *p) {
      delete [] ((::bsim::Dk2Nu*)p);
   }
   static void destruct_bsimcLcLDk2Nu(void *p) {
      typedef ::bsim::Dk2Nu current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::bsim::Dk2Nu

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 447,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::__1::vector<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::allocator<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 447,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::__1::vector<int, std::__1::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 447,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::__1::vector<double, std::__1::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEbsimcLcLTrajgR_Dictionary();
   static void vectorlEbsimcLcLTrajgR_TClassManip(TClass*);
   static void *new_vectorlEbsimcLcLTrajgR(void *p = 0);
   static void *newArray_vectorlEbsimcLcLTrajgR(Long_t size, void *p);
   static void delete_vectorlEbsimcLcLTrajgR(void *p);
   static void deleteArray_vectorlEbsimcLcLTrajgR(void *p);
   static void destruct_vectorlEbsimcLcLTrajgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bsim::Traj>*)
   {
      vector<bsim::Traj> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bsim::Traj>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bsim::Traj>", -2, "vector", 447,
                  typeid(vector<bsim::Traj>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEbsimcLcLTrajgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<bsim::Traj>) );
      instance.SetNew(&new_vectorlEbsimcLcLTrajgR);
      instance.SetNewArray(&newArray_vectorlEbsimcLcLTrajgR);
      instance.SetDelete(&delete_vectorlEbsimcLcLTrajgR);
      instance.SetDeleteArray(&deleteArray_vectorlEbsimcLcLTrajgR);
      instance.SetDestructor(&destruct_vectorlEbsimcLcLTrajgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bsim::Traj> >()));

      ::ROOT::AddClassAlternate("vector<bsim::Traj>","std::__1::vector<bsim::Traj, std::__1::allocator<bsim::Traj> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bsim::Traj>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEbsimcLcLTrajgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bsim::Traj>*)0x0)->GetClass();
      vectorlEbsimcLcLTrajgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEbsimcLcLTrajgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEbsimcLcLTrajgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Traj> : new vector<bsim::Traj>;
   }
   static void *newArray_vectorlEbsimcLcLTrajgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Traj>[nElements] : new vector<bsim::Traj>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEbsimcLcLTrajgR(void *p) {
      delete ((vector<bsim::Traj>*)p);
   }
   static void deleteArray_vectorlEbsimcLcLTrajgR(void *p) {
      delete [] ((vector<bsim::Traj>*)p);
   }
   static void destruct_vectorlEbsimcLcLTrajgR(void *p) {
      typedef vector<bsim::Traj> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bsim::Traj>

namespace ROOT {
   static TClass *vectorlEbsimcLcLNuRaygR_Dictionary();
   static void vectorlEbsimcLcLNuRaygR_TClassManip(TClass*);
   static void *new_vectorlEbsimcLcLNuRaygR(void *p = 0);
   static void *newArray_vectorlEbsimcLcLNuRaygR(Long_t size, void *p);
   static void delete_vectorlEbsimcLcLNuRaygR(void *p);
   static void deleteArray_vectorlEbsimcLcLNuRaygR(void *p);
   static void destruct_vectorlEbsimcLcLNuRaygR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bsim::NuRay>*)
   {
      vector<bsim::NuRay> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bsim::NuRay>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bsim::NuRay>", -2, "vector", 447,
                  typeid(vector<bsim::NuRay>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEbsimcLcLNuRaygR_Dictionary, isa_proxy, 4,
                  sizeof(vector<bsim::NuRay>) );
      instance.SetNew(&new_vectorlEbsimcLcLNuRaygR);
      instance.SetNewArray(&newArray_vectorlEbsimcLcLNuRaygR);
      instance.SetDelete(&delete_vectorlEbsimcLcLNuRaygR);
      instance.SetDeleteArray(&deleteArray_vectorlEbsimcLcLNuRaygR);
      instance.SetDestructor(&destruct_vectorlEbsimcLcLNuRaygR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bsim::NuRay> >()));

      ::ROOT::AddClassAlternate("vector<bsim::NuRay>","std::__1::vector<bsim::NuRay, std::__1::allocator<bsim::NuRay> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bsim::NuRay>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEbsimcLcLNuRaygR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bsim::NuRay>*)0x0)->GetClass();
      vectorlEbsimcLcLNuRaygR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEbsimcLcLNuRaygR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEbsimcLcLNuRaygR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::NuRay> : new vector<bsim::NuRay>;
   }
   static void *newArray_vectorlEbsimcLcLNuRaygR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::NuRay>[nElements] : new vector<bsim::NuRay>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEbsimcLcLNuRaygR(void *p) {
      delete ((vector<bsim::NuRay>*)p);
   }
   static void deleteArray_vectorlEbsimcLcLNuRaygR(void *p) {
      delete [] ((vector<bsim::NuRay>*)p);
   }
   static void destruct_vectorlEbsimcLcLNuRaygR(void *p) {
      typedef vector<bsim::NuRay> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bsim::NuRay>

namespace ROOT {
   static TClass *vectorlEbsimcLcLLocationgR_Dictionary();
   static void vectorlEbsimcLcLLocationgR_TClassManip(TClass*);
   static void *new_vectorlEbsimcLcLLocationgR(void *p = 0);
   static void *newArray_vectorlEbsimcLcLLocationgR(Long_t size, void *p);
   static void delete_vectorlEbsimcLcLLocationgR(void *p);
   static void deleteArray_vectorlEbsimcLcLLocationgR(void *p);
   static void destruct_vectorlEbsimcLcLLocationgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bsim::Location>*)
   {
      vector<bsim::Location> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bsim::Location>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bsim::Location>", -2, "vector", 447,
                  typeid(vector<bsim::Location>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEbsimcLcLLocationgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<bsim::Location>) );
      instance.SetNew(&new_vectorlEbsimcLcLLocationgR);
      instance.SetNewArray(&newArray_vectorlEbsimcLcLLocationgR);
      instance.SetDelete(&delete_vectorlEbsimcLcLLocationgR);
      instance.SetDeleteArray(&deleteArray_vectorlEbsimcLcLLocationgR);
      instance.SetDestructor(&destruct_vectorlEbsimcLcLLocationgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bsim::Location> >()));

      ::ROOT::AddClassAlternate("vector<bsim::Location>","std::__1::vector<bsim::Location, std::__1::allocator<bsim::Location> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bsim::Location>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEbsimcLcLLocationgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bsim::Location>*)0x0)->GetClass();
      vectorlEbsimcLcLLocationgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEbsimcLcLLocationgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEbsimcLcLLocationgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Location> : new vector<bsim::Location>;
   }
   static void *newArray_vectorlEbsimcLcLLocationgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Location>[nElements] : new vector<bsim::Location>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEbsimcLcLLocationgR(void *p) {
      delete ((vector<bsim::Location>*)p);
   }
   static void deleteArray_vectorlEbsimcLcLLocationgR(void *p) {
      delete [] ((vector<bsim::Location>*)p);
   }
   static void destruct_vectorlEbsimcLcLLocationgR(void *p) {
      typedef vector<bsim::Location> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bsim::Location>

namespace ROOT {
   static TClass *vectorlEbsimcLcLAncestorgR_Dictionary();
   static void vectorlEbsimcLcLAncestorgR_TClassManip(TClass*);
   static void *new_vectorlEbsimcLcLAncestorgR(void *p = 0);
   static void *newArray_vectorlEbsimcLcLAncestorgR(Long_t size, void *p);
   static void delete_vectorlEbsimcLcLAncestorgR(void *p);
   static void deleteArray_vectorlEbsimcLcLAncestorgR(void *p);
   static void destruct_vectorlEbsimcLcLAncestorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bsim::Ancestor>*)
   {
      vector<bsim::Ancestor> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bsim::Ancestor>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bsim::Ancestor>", -2, "vector", 447,
                  typeid(vector<bsim::Ancestor>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEbsimcLcLAncestorgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<bsim::Ancestor>) );
      instance.SetNew(&new_vectorlEbsimcLcLAncestorgR);
      instance.SetNewArray(&newArray_vectorlEbsimcLcLAncestorgR);
      instance.SetDelete(&delete_vectorlEbsimcLcLAncestorgR);
      instance.SetDeleteArray(&deleteArray_vectorlEbsimcLcLAncestorgR);
      instance.SetDestructor(&destruct_vectorlEbsimcLcLAncestorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bsim::Ancestor> >()));

      ::ROOT::AddClassAlternate("vector<bsim::Ancestor>","std::__1::vector<bsim::Ancestor, std::__1::allocator<bsim::Ancestor> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bsim::Ancestor>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEbsimcLcLAncestorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bsim::Ancestor>*)0x0)->GetClass();
      vectorlEbsimcLcLAncestorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEbsimcLcLAncestorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEbsimcLcLAncestorgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Ancestor> : new vector<bsim::Ancestor>;
   }
   static void *newArray_vectorlEbsimcLcLAncestorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bsim::Ancestor>[nElements] : new vector<bsim::Ancestor>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEbsimcLcLAncestorgR(void *p) {
      delete ((vector<bsim::Ancestor>*)p);
   }
   static void deleteArray_vectorlEbsimcLcLAncestorgR(void *p) {
      delete [] ((vector<bsim::Ancestor>*)p);
   }
   static void destruct_vectorlEbsimcLcLAncestorgR(void *p) {
      typedef vector<bsim::Ancestor> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bsim::Ancestor>

namespace {
  void TriggerDictionaryInitialization_DKDict_Impl() {
    static const char* headers[] = {
"include/dkmeta.h",
"include/dk2nu.h",
"include/dflt.h",
0
    };
    static const char* includePaths[] = {
"/Users/bashyala/cern_root/new_root/root_install/include/",
"/Users/bashyala/Argonne_Stuff/general_h5demos/h5demos/dk2nu/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DKDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dkmeta.h")))  Location;}
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$iosfwd")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  NuRay;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  Ancestor;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  Traj;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dkmeta.h")))  DkMeta;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  Decay;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  TgtExit;}
namespace bsim{class __attribute__((annotate("$clingAutoload$include/dk2nu.h")))  Dk2Nu;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DKDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/dkmeta.h"
#include "include/dk2nu.h"
#include "include/dflt.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"bsim::Ancestor", payloadCode, "@",
"bsim::Decay", payloadCode, "@",
"bsim::Dk2Nu", payloadCode, "@",
"bsim::DkMeta", payloadCode, "@",
"bsim::Location", payloadCode, "@",
"bsim::NuRay", payloadCode, "@",
"bsim::TgtExit", payloadCode, "@",
"bsim::Traj", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DKDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DKDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DKDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DKDict() {
  TriggerDictionaryInitialization_DKDict_Impl();
}
