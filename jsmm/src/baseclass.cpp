// <jsmm> -*- C++ -*- 

// C++ wrapper for the SpiderMonkey JavaScript engine.
// Copyright (C) 2006  Roman Senn <r.senn@nexbyte.com>
//    
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

#include <jsmm/baseclass>
#include <jsmm/classexception>
#include <jsmm/api>

#include <js/jsapi.h>
#include <js/jsfun.h>
#include <js/jsobj.h>
#include <js/jsinterp.h>

#include "../config/config.h"

jsmm_namespace_BEGIN
  
#if JS_DEBUG
#define static
#endif /* JS_DEBUG */
  
static BaseClass *getBaseClass(ClassAPI clsimpl)
{
  Class &cls = clsimpl.requestInstance();
  
  return dynamic_cast<BaseClass *>(&cls);
}

static BaseClass &requestBaseClass(ClassAPI clsimpl)
{
  BaseClass *basecls = getBaseClass(clsimpl);
  
  if(!basecls)
    throw ClassException
    (&clsimpl, "The class <" + (std::string)clsimpl.getName() +
     "> is not a BaseClass!");
  
  return *basecls;      
}

static BaseClass &dispatchClassOps(ContextRef cx, ObjectAPI obj)
{
  ClassAPI clsimpl = obj.getClass(cx);
  return requestBaseClass(clsimpl);
}

static Bool dispatchAddPropertyOp
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
{
  return dispatchClassOps(cx, obj).addProperty(cx, obj, id, vref);
}

static Bool dispatchDelPropertyOp
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
{
  return dispatchClassOps(cx, obj).delProperty(cx, obj, id, vref);
}

static Bool dispatchGetPropertyOp
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
{
  return dispatchClassOps(cx, obj).getProperty(cx, obj, id, vref);
}

static Bool dispatchSetPropertyOp
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
{
  return dispatchClassOps(cx, obj).setProperty(cx, obj, id, vref);
}

static Bool dispatchEnumerateOp
  (ContextRef cx, ObjectAPI obj)
{
  return dispatchClassOps(cx, obj).enumerate(cx, obj);
}

static Bool dispatchResolveOp
  (ContextRef cx, ObjectAPI obj, Value id)
{
  return dispatchClassOps(cx, obj).resolve(cx, obj, id);
}

static Bool dispatchConvertOp
  (ContextRef cx, ObjectAPI obj, Type type, Value &vref)
{
  return dispatchClassOps(cx, obj).convert(cx, obj, type, vref);
}

static void dispatchFinalizeOp
  (ContextRef cx, ObjectAPI obj)
{
  return dispatchClassOps(cx, obj).finalize(cx, obj);
}

static ObjectOps *dispatchGetObjectOps
  (ContextRef cx, ClassRef cls)
{
  return requestBaseClass(cls).getObjectOps(cx, cls);
}

static Bool dispatchCheckAccessOp
  (ContextRef cx, ObjectAPI obj, Value id, AccessMode mode, Value &vref)
{
  return dispatchClassOps(cx, obj).checkAccess(cx, obj, id, mode, vref);
}

static Bool dispatchCall
  (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
{
  return dispatchClassOps(cx, obj).call(cx, obj, argc, argv, rval);
}

static Bool dispatchConstruct
  (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
{
  ClassAPI cls = ObjectAPI(obj).getClass(cx);
  BaseClass *basecls = getBaseClass(cls);
  
  if(basecls)
    return basecls->construct(cx, obj, argc, argv, rval);
  
  return true;
}

/*  static Bool dispatchXDRObject
  (XDRState &xdr, ObjectPtr &objref)
{
  return dispatchClassOps(xdr.cx, objref).
}*/

static Bool dispatchHasInstanceOp
  (ContextRef cx, ObjectAPI obj, Value v, Bool &bref)
{
  return dispatchClassOps(cx, obj).hasInstance(cx, obj, v, bref);
}

static Uint dispatchMarkOp
  (ContextRef cx, ObjectAPI obj, void *arg)
{
  return dispatchClassOps(cx, obj).mark(cx, obj, arg);
}

static Uint dispatchReserveSlotsOp
  (ContextRef cx, ObjectAPI obj)
{
  return dispatchClassOps(cx, obj).reserveSlots(cx, obj);
}

static Bool dispatchEqualityOp
  (ContextRef cx, ObjectAPI obj, Value v, Bool &bref)
{
  return dispatchClassOps(cx, obj).equality(cx, obj, v, bref);
}

static ObjectPtr dispatchOuterObjectOp
  (ContextRef cx, ObjectAPI obj)
{
  if(!obj) 
  {
    puts("warning: NULL pointer in dispatchOuterObjectOp!");
    return obj;
  }
  
/*  printf("%p\n", cx.getPtr());
  printf("%p\n", obj.getPtr());
  printf("%p\n", *((void **)&cx));
  printf("%p\n", *((void **)&cx + 1));*/

  return dispatchClassOps(cx, obj).outerObject(cx, obj);
}

static ObjectPtr dispatchInnerObjectOp
  (ContextRef cx, ObjectAPI obj)
{
  return dispatchClassOps(cx, obj).innerObject(cx, obj);
}

BaseClass::BaseClass
  (const char *name, Bool hasPrivate) :
  Class(name, hasPrivate, true)
{
  setAddPropertyOp(dispatchAddPropertyOp);
  setDelPropertyOp(dispatchDelPropertyOp);
  setGetPropertyOp(dispatchGetPropertyOp);
  setSetPropertyOp(dispatchSetPropertyOp);
  setEnumerateOp(dispatchEnumerateOp);
  setResolveOp(dispatchResolveOp);
  setConvertOp(dispatchConvertOp);
  setFinalizeOp(dispatchFinalizeOp);
  
  setGetObjectOps(&dispatchGetObjectOps);
  setCheckAccessOp(&dispatchCheckAccessOp);
  setCallOp(&dispatchCall);
  setConstructOp(&dispatchConstruct);
//    setXDRObjectOp(&dispatchXDRObjectOp);
  setHasInstanceOp(&dispatchHasInstanceOp);
  setMarkOp(&dispatchMarkOp);
  setReserveSlotsOp(&dispatchReserveSlotsOp);
  
  setEqualityOp(&dispatchEqualityOp);
  setOuterObjectOp(&dispatchOuterObjectOp);
  setInnerObjectOp(&dispatchInnerObjectOp);
}

BaseClass::BaseClass
  (const char *name, void *vtable, Bool hasPrivate) :
  Class(name, vtable, hasPrivate, true)
{
  setAddPropertyOp(dispatchAddPropertyOp);
  setDelPropertyOp(dispatchDelPropertyOp);
  setGetPropertyOp(dispatchGetPropertyOp);
  setSetPropertyOp(dispatchSetPropertyOp);
  setEnumerateOp(dispatchEnumerateOp);
  setResolveOp(dispatchResolveOp);
  setConvertOp(dispatchConvertOp);
  setFinalizeOp(dispatchFinalizeOp);
}

Bool BaseClass::addProperty
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
{
  return PropertyStub(cx, obj, id, vref);
}

Bool BaseClass::delProperty
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
{
  return PropertyStub(cx, obj, id, vref);
}

Bool BaseClass::getProperty
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
{
  return PropertyStub(cx, obj, id, vref);
}

Bool BaseClass::setProperty
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
{
  return PropertyStub(cx, obj, id, vref);
}

Bool BaseClass::enumerate
  (ContextRef cx, ObjectAPI obj) const
{
  return EnumerateStub(cx, obj);
}

Bool BaseClass::resolve
  (ContextRef cx, ObjectAPI obj, Value id) const
{
  return ResolveStub(cx, obj, id);
}

Bool BaseClass::convert
  (ContextRef cx, ObjectAPI obj, Type type, Value &vref) const
{
  return ConvertStub(cx, obj, type, vref);
}

void BaseClass::finalize
  (ContextRef cx, ObjectAPI obj) const
{
  FinalizeStub(cx, obj);
}  

ObjectOps *BaseClass::getObjectOps
  (ContextRef cx, ClassRef cls) const
{
  return reinterpret_cast<ObjectOps *>(JS_GetDefaultObjectOps());
}

Bool BaseClass::checkAccess
  (ContextRef cx, ObjectAPI obj, Value id, AccessMode mode, Value &vref)
  const
{
  return true;
}

Bool BaseClass::call
  (ContextRef cx, ObjectAPI obj, Uint argc, Value argv[], Value &rval)
  const
{
  JS_ReportIsNotFunction(cx, argv[-2], 0);
  return false;
}

Bool BaseClass::construct
  (ContextRef cx, ObjectAPI obj, Uint argc, Value argv[], Value &rval)
  const
{
  JS_ReportIsNotFunction(cx, argv[-2], JSV2F_CONSTRUCT);
  return false;
}

/*  Bool BaseClass::xdrObject
  (XDRState &xdr, ObjectPtr &objref) const
{
  return true;
}*/

Bool BaseClass::hasInstance
  (ContextRef cx, ObjectAPI obj, Value v, Bool &bref) const
{
  return false;
}

Uint BaseClass::mark
  (ContextRef cx, ObjectAPI obj, void *arg) const
{
  return true;
}

Uint BaseClass::reserveSlots
  (ContextRef cx, ObjectAPI obj) const
{
  return 0;
}

Bool BaseClass::equality
  (ContextRef cx, ObjectAPI obj, Value v, Bool &bref) const
{
  return (v.isObject() && v.getObject() == obj);
}

ObjectPtr BaseClass::outerObject
  (ContextRef cx, ObjectAPI obj) const
{
  return 0;
}

ObjectPtr BaseClass::innerObject
  (ContextRef cx, ObjectAPI obj) const
{
  return 0;
}

jsmm_namespace_END
