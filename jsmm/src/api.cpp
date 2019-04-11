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

#include <jsmm/api>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

  Int64 Now
    ()
  {
    int64 r = JS_Now();
    return *reinterpret_cast<Int64 *>(&r);
  }
  
  Value GetNaNValue
    (ContextPtr cx)
  {
    jsval v = JS_GetNaNValue(cx);
    return *reinterpret_cast<Value *>(&v);
  }
  
  Value GetNegativeInfinityValue
    (ContextPtr cx)
  {
    jsval v = JS_GetNegativeInfinityValue(cx);
    return *reinterpret_cast<Value *>(&v);
  }
  
  Value GetPositiveInfinityValue
    (ContextPtr cx)
  {
    jsval v = JS_GetPositiveInfinityValue(cx);
    return *reinterpret_cast<Value *>(&v);  
  }
  
  Value GetEmptyStringValue
    (ContextPtr cx)
  {
    jsval v = JS_GetEmptyStringValue(cx);
    return *reinterpret_cast<Value *>(&v);  
  }
  
  Bool ConvertArguments
    (ContextPtr cx, Uint argc, Value argv[], const Byte *format, ...)
  {
    Bool ret;
    va_list ap;
    va_start(ap, format);

    ret = JS_ConvertArgumentsVA
      (cx, argc, reinterpret_cast<jsval *>(argv), format, ap);
    
    va_end(ap);    
    return ret;
  }
  
  Bool ConvertArgumentsVA
    (ContextPtr cx, Uint argc, Value argv[], const Byte *format, va_list ap)
  {
    return JS_ConvertArgumentsVA
      (cx, argc, reinterpret_cast<jsval *>(argv), format, ap);
  }
  
  Value *PushArguments
    (ContextPtr cx, void *&mark, const Byte *format, ...)
  {
    Value *ret;
    va_list ap;
    va_start(ap, format);

    ret = reinterpret_cast<Value *>
      (JS_PushArgumentsVA(cx, &mark, format, ap));
    
    va_end(ap);
    return ret;    
  }
  
  Value *PushArgumentsVA
    (ContextPtr cx, void *&mark, const Byte *format, va_list ap)
  {
    return reinterpret_cast<Value *>
      (JS_PushArgumentsVA(cx, &mark, format, ap));
  }
  
  void PopArguments
    (ContextPtr cx, void *mark)
  {
    return JS_PopArguments(cx, mark);
  }
  
  Bool AddArgumentFormatter
    (ContextPtr cx, const Byte *format, ArgumentFormatter &formatter)
  {
    return JS_AddArgumentFormatter
      (cx, format, reinterpret_cast<JSArgumentFormatter>(&formatter));
  }
  
  void RemoveArgumentFormatter
    (ContextPtr cx, const Byte *format)
  {
    return JS_RemoveArgumentFormatter(cx, format);
  }
  
  Bool ConvertValue
    (ContextPtr cx, Value v, Type type, Value &vref)
  {
    return JS_ConvertValue
      (cx, v, static_cast<JSType>(type), reinterpret_cast<jsval *>(&vref));
  }
  
  Bool ValueToObject
    (ContextPtr cx, Value v, ObjectPtr &obj)
  {
    return JS_ValueToObject(cx, v, obj);
  }
  
  FunctionPtr ValueToFunction
    (ContextPtr cx, Value v)
  {
    return JS_ValueToFunction(cx, v);
  }
  
  FunctionPtr ValueToConstructor
    (ContextPtr cx, Value v)
  {
    return JS_ValueToConstructor(cx, v);
  }
  
  StringPtr ValueToString
    (ContextPtr cx, Value v)
  {
    return JS_ValueToString(cx, v);
  }
  
  Bool ValueToNumber
    (ContextPtr cx, Value v, Double &d)
  {
    return JS_ValueToNumber(cx, v, &d);
  }
  
  Bool ValueToECMAInt32
    (ContextPtr cx, Value v, Int32 &i)
  {
    return JS_ValueToECMAInt32(cx, v, reinterpret_cast<int32 *>(&i));
  }
  
  Bool ValueToECMAUint32
    (ContextPtr cx, Value v, Uint32 &i)
  {
    return JS_ValueToECMAUint32(cx, v, reinterpret_cast<uint32 *>(&i));
  }
  
  Bool ValueToInt32
    (ContextPtr cx, Value v, Int32 &i)
  {
    return JS_ValueToInt32(cx, v, reinterpret_cast<int32 *>(&i));
  }
  
  Bool ValueToUint16
    (ContextPtr cx, Value v, Uint16 &i)
  {
    return JS_ValueToUint16(cx, v, &i);
  }
  
  Bool ValueToBool
    (ContextPtr cx, Value v, Bool &b)
  {
    return JS_ValueToBoolean(cx, v, &b);
  }
  
  Type TypeOfValue
    (ContextPtr cx, Value v)
  {
    return static_cast<Type>(JS_TypeOfValue(cx, v));
  }
  
  const Byte *GetTypeName
    (ContextPtr cx, Type type)
  {
    return JS_GetTypeName(cx, static_cast<JSType>(type));
  }
  
  RuntimePtr NewRuntime
    (Uint32 maxbytes)
  {
    return JS_NewRuntime(maxbytes);
  }
  
  void DestroyRuntime
    (RuntimePtr rt)
  {
    JS_DestroyRuntime(rt);
  }
  
  void ShutDown
    ()
  {
    JS_ShutDown();
  }
  
  void *GetRuntimePrivate
    (RuntimePtr rt)
  {
    return JS_GetRuntimePrivate(rt);
  }
  
  void SetRuntimePrivate
    (RuntimePtr rt, void *data)
  {
    JS_SetRuntimePrivate(rt, data);
  }
  
  void BeginRequest
    (ContextPtr cx)
  {
#ifdef JS_THREADSAFE
    JS_BeginRequest(cx);
#endif /* JS_THREADSAFE */
  }
  
  void EndRequest
    (ContextPtr cx)
  {
#ifdef JS_THREADSAFE
    JS_EndRequest(cx);
#endif /* JS_THREADSAFE */
  }
  
  void YieldRequest
    (ContextPtr cx)
  {
#ifdef JS_THREADSAFE
    JS_YieldRequest(cx);
#endif /* JS_THREADSAFE */
  }
  
  RefCount SuspendRequest
    (ContextPtr cx)
  {
#ifdef JS_THREADSAFE
    return JS_SuspendRequest(cx);
#else
    return 0;
#endif /* JS_THREADSAFE */
  }
  
  void ResumeRequest
    (ContextPtr cx, RefCount saveDepth)
  {
#ifdef JS_THREADSAFE
    JS_ResumeRequest(cx, saveDepth);
#endif /* JS_THREADSAFE */
  }
  
  void Lock
    (RuntimePtr rt)
  {
    JS_Lock(rt);
  }
  
  void Unlock
    (RuntimePtr rt)
  {
    JS_Unlock(rt);
  }
  
  ContextPtr NewContext
    (RuntimePtr rt, Size stackChunkSize)
  {
    return JS_NewContext(rt, stackChunkSize);
  }
  
  void DestroyContext
    (ContextPtr cx)
  {
    JS_DestroyContext(cx);
  }
  
  void DestroyContextNoGC
    (ContextPtr cx)
  {
    JS_DestroyContextNoGC(cx);
  }
  
  void DestroyContextMaybeGC
    (ContextPtr cx)
  {
    JS_DestroyContextMaybeGC(cx);
  }
  
  void *GetContextPrivate
    (ContextPtr cx)
  {
    return JS_GetContextPrivate(cx);
  }
  
  void SetContextPrivate
    (ContextPtr cx, void *data)
  {
    JS_SetContextPrivate(cx, data);
  }
  
  RuntimePtr GetRuntime
    (ContextPtr cx)
  {
    return JS_GetRuntime(cx);
  }
  
  ContextPtr ContextIterator
    (RuntimePtr rt, ContextPtr &iter)
  {
    return JS_ContextIterator(rt, iter);
  }
  
  Version GetVersion
    (ContextPtr cx)
  {
    return static_cast<Version>(JS_GetVersion(cx));
  }
  
  Version SetVersion
    (ContextPtr cx, Version version)
  {
    return static_cast<Version>(JS_SetVersion(cx, static_cast<JSVersion>(version)));
  }
  
  const Byte *VersionToString
    (Version version)
  {
    return JS_VersionToString(static_cast<JSVersion>(version));
  }
  
  Version StringToVersion
    (const Byte *string)
  {
    return static_cast<Version>(JS_StringToVersion(string));
  }
  
  Uint32 GetOptions
    (ContextPtr cx)
  {
    return JS_GetOptions(cx);
  }
  
  Uint32 SetOptions
    (ContextPtr cx, Uint32 options)
  {
    return JS_SetOptions(cx, options);
  }
  
  Uint32 ToggleOptions
    (ContextPtr cx, Uint32 options)
  {
    return JS_ToggleOptions(cx, options);
  }
  
  const Byte *GetImplementationVersion
    ()
  {
    return JS_GetImplementationVersion();
  }
  
  ObjectPtr GetGlobalObject
    (ContextPtr cx)
  {
    return JS_GetGlobalObject(cx);
  }
  
  void SetGlobalObject
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_SetGlobalObject(cx, obj);
  }
  
  Bool InitStandardClasses
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_InitStandardClasses(cx, obj);
  }
  
  Bool ResolveStandardClass
    (ContextPtr cx, ObjectPtr obj, Value id, Bool &resolved)
  {
    return JS_ResolveStandardClass(cx, obj, id, &resolved);
  }
  
  Bool EnumerateStandardClasses
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_EnumerateStandardClasses(cx, obj);
  }
  
  IdArray &EnumerateResolvedStandardClasses
    (ContextPtr cx, ObjectPtr obj, IdArray &ida)
  {
    return *reinterpret_cast<IdArray *>
      (JS_EnumerateResolvedStandardClasses
       (cx, obj, reinterpret_cast<JSIdArray *>(&ida)));
  }
  
  ObjectPtr GetScopeChain
    (ContextPtr cx)
  {
    return JS_GetScopeChain(cx);
  }
  
  void *malloc
    (ContextPtr cx, Size nbytes)
  {
    return JS_malloc(cx, nbytes);
  }
  
  void *realloc
    (ContextPtr cx, void *p, Size nbytes)
  {
    return JS_realloc(cx, p, nbytes);
  }
  
  void free
    (ContextPtr cx, void *p)
  {
    return JS_free(cx, p);
  }
  
  Byte *strdup
    (ContextPtr cx, const Byte *s)
  {
    return JS_strdup(cx, s);
  }
  
  Double *NewDouble
    (ContextPtr cx, Double d)
  {
    return JS_NewDouble(cx, d);
  }
  
  Bool NewDoubleValue
    (ContextPtr cx, Double d, Value &rval)
  {
    return JS_NewDoubleValue(cx, d, reinterpret_cast<jsval *>(&rval));
  }
  
  Bool NewNumberValue
    (ContextPtr cx, Double d, Value &rval)
  {
    return JS_NewNumberValue(cx, d, reinterpret_cast<jsval *>(&rval));
  }
  
  Bool AddRoot
    (ContextPtr cx, void *rp)
  {
    return JS_AddRoot(cx, rp);
  }
  
  Bool AddNamedRoot
    (ContextPtr cx, void *rp, const Byte *name)
  {
    return JS_AddNamedRoot(cx, rp, name);
  }
  
  Bool AddNamedRootRT
    (RuntimePtr rt, void *rp, const Byte *name)
  {
    return JS_AddNamedRootRT(rt, rp, name);
  }
  
  Bool RemoveRoot
    (ContextPtr cx, void *rp)
  {
    return JS_RemoveRoot(cx, rp);
  }
  
  Bool RemoveRootRT
    (RuntimePtr rt, void *rp)
  {
    return JS_RemoveRootRT(rt, rp);
  }
  
  void ClearNewbornRoots
    (ContextPtr cx)
  {
    JS_ClearNewbornRoots(cx);
  }
  
  Bool EnterLocalRootScope
    (ContextPtr cx)
  {
    return JS_EnterLocalRootScope(cx);
  }
  
  void LeaveLocalRootScope
    (ContextPtr cx)
  {
    return JS_LeaveLocalRootScope(cx);
  }
  
  void ForgetLocalRoot
    (ContextPtr cx, void *thing)
  {
    return JS_ForgetLocalRoot(cx, thing);
  }
  
  void DumpNamedRoots
    (RuntimePtr rt, void (*dump)(const Byte *name, void *rp, void *data), void *data)
  {
  }
  
  Uint32 MapGCRoots
    (RuntimePtr rt, GCRootMapFun &map, void *data)
  {
    return JS_MapGCRoots(rt, reinterpret_cast<JSGCRootMapFun>(&map), data);
  }
  
  Bool LockGCThing
    (ContextPtr cx, void *thing)
  {
    return JS_LockGCThing(cx, thing);
  }
  
  Bool LockGCThingRT
    (RuntimePtr rt, void *thing)
  {
    return JS_LockGCThingRT(rt, thing);
  }
  
  Bool UnlockGCThing
    (ContextPtr cx, void *thing)
  {
    return JS_UnlockGCThing(cx, thing);
  }
  
  Bool UnlockGCThingRT
    (RuntimePtr rt, void *thing)
  {
    return JS_UnlockGCThingRT(rt, thing);
  }
  
  void MarkGCThing
    (ContextPtr cx, void *thing, const Byte *name, void *arg)
  {
    JS_MarkGCThing(cx, thing, name, arg);
  }
  
  void GC
    (ContextPtr cx)
  {
    JS_GC(cx);
  }
  
  void MaybeGC
    (ContextPtr cx)
  {
    JS_MaybeGC(cx);
  }
  
  GCCallback *SetGCCallback
    (ContextPtr cx, GCCallback *cb)
  {
    return reinterpret_cast<GCCallback *>
      (JS_SetGCCallback(cx, reinterpret_cast<JSGCCallback>(cb)));
  }
  
  GCCallback *SetGCCallbackRT
    (RuntimePtr rt, GCCallback *cb)
  {
    return reinterpret_cast<GCCallback *>
      (JS_SetGCCallbackRT(rt, reinterpret_cast<JSGCCallback>(cb)));    
  }
  
  Bool IsAboutToBeFinalized
    (ContextPtr cx, void *thing)
  {
    return JS_IsAboutToBeFinalized(cx, thing);
  }
  
/*  void SetGCParameter
    (RuntimePtr rt, GCParamKey key, Uint32 value)
  {
    return JS_SetGCParameter(rt, static_cast<JSGCParamKey>(key), value);
  }*/

  Int AddExternalStringFinalizer
    (StringFinalizeOp &finalizer)
  {
    return JS_AddExternalStringFinalizer
      (reinterpret_cast<JSStringFinalizeOp>(&finalizer));
  }
  
  Int RemoveExternalStringFinalizer
    (StringFinalizeOp &finalizer)
  {
    return JS_RemoveExternalStringFinalizer
      (reinterpret_cast<JSStringFinalizeOp>(&finalizer));
  }
  
  StringPtr NewExternalString
    (ContextPtr cx, Char *chars, Size length, Int type)
  {
    return JS_NewExternalString(cx, chars, length, type);
  }
  
  Int GetExternalStringGCType
    (RuntimePtr rt, StringPtr str)
  {
    return JS_GetExternalStringGCType(rt, str);
  }
  
  void SetThreadStackLimit
    (ContextPtr cx, Uword limitAddr)
  {
    JS_SetThreadStackLimit(cx, limitAddr);
  }
  
  void DestroyIdArray
    (ContextPtr cx, IdArray &ida)
  {
    JS_DestroyIdArray(cx, reinterpret_cast<JSIdArray *>(&ida));
  }
  
  Bool ValueToId
    (ContextPtr cx, Value v, Id &idref)
  {
    return JS_ValueToId(cx, v, &idref);
  }
  
  Bool IdToValue
    (ContextPtr cx, Id id, Value &vref)
  {
    return JS_IdToValue(cx, id, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool PropertyStub
    (ContextPtr cx, ObjectPtr obj, Value id, Value &vref)
  {
    return JS_PropertyStub(cx, obj, id, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool EnumerateStub
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_EnumerateStub(cx, obj);
  }
  
  Bool ResolveStub
    (ContextPtr cx, ObjectPtr obj, Value id)
  {
    return JS_ResolveStub(cx, obj, id);
  }
  
  Bool ConvertStub
    (ContextPtr cx, ObjectPtr obj, Type type, Value &vref)
  {
    return JS_ConvertStub
      (cx, obj, static_cast<JSType>(type), reinterpret_cast<jsval *>(&vref));
  }
  
  void FinalizeStub
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_FinalizeStub(cx, obj);
  }
  
  ObjectPtr InitClass
    (ContextPtr cx, ObjectPtr obj, ObjectPtr parent_proto, ClassPtr clasp,
     Native &constructor, Uint nargs, PropertySpec *ps, FunctionSpec *fs, 
     PropertySpec *static_ps, FunctionSpec *static_fs)
  {
    return JS_InitClass
      (cx, obj, parent_proto, clasp, 
       reinterpret_cast<JSNative>(&constructor), nargs,
       reinterpret_cast<JSPropertySpec *>(ps),
       reinterpret_cast<JSFunctionSpec *>(fs),
       reinterpret_cast<JSPropertySpec *>(static_ps),
       reinterpret_cast<JSFunctionSpec *>(static_fs));
  }
  
  ClassPtr GetClass
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_GET_CLASS(cx, obj);
  }

  Bool InstanceOf
    (ContextPtr cx, ObjectPtr obj, ClassPtr clasp, Value argv[])
  {
    return JS_InstanceOf(cx, obj, clasp, reinterpret_cast<jsval *>(argv));
  }
  
  Bool HasInstance
    (ContextPtr cx, ObjectPtr obj, Value v, Bool &b)
  {
    return JS_HasInstance(cx, obj, v, &b);
  }

  void *GetPrivate
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_GetPrivate(cx, obj);
  }
  
  Bool SetPrivate
    (ContextPtr cx, ObjectPtr obj, void *data)
  {
    return JS_SetPrivate(cx, obj, data);
  }
  
  void *GetInstancePrivate
    (ContextPtr cx, ObjectPtr obj, ClassPtr clasp, Value argv[])
  {
    return JS_GetInstancePrivate(cx, obj, clasp, reinterpret_cast<jsval *>(argv));
  }
  
  ObjectPtr GetPrototype
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_GetPrototype(cx, obj);
  }
  
  Bool SetPrototype
    (ContextPtr cx, ObjectPtr obj, ObjectPtr proto)
  {
    return JS_SetPrototype(cx, obj, proto);
  }
  
  ObjectPtr GetParent
    (ContextPtr cx, ObjectPtr obj)
  {
    return JS_GetParent(cx, obj);
  }
  
  Bool SetParent
    (ContextPtr cx, ObjectPtr obj, ObjectPtr parent)
  {
    return JS_SetParent(cx, obj, parent);
  }
  
  ObjectPtr GetConstructor
    (ContextPtr cx, ObjectPtr proto)
  {
    return JS_GetConstructor(cx, proto);
  }
  
  Bool GetObjectId
    (ContextPtr cx, ObjectPtr obj, Id &idref)
  {
    return JS_GetObjectId(cx, obj, &idref);
  }
  
  ObjectPtr NewObject
    (ContextPtr cx, ClassPtr clasp, ObjectPtr proto, ObjectPtr parent)
  {
    return JS_NewObject(cx, clasp, proto, parent);
  }
  
  Bool SealObject(ContextPtr cx, ObjectPtr obj, Bool deep)
  {
    return JS_SealObject(cx, obj, deep);
  }
  
  ObjectPtr ConstructObject
    (ContextPtr cx, ClassPtr clasp, ObjectPtr proto, ObjectPtr parent)
  {
    return JS_ConstructObject(cx, clasp, proto, parent);
  }
  
  ObjectPtr ConstructObjectWithArguments
    (ContextPtr cx, ClassPtr clasp, ObjectPtr proto, ObjectPtr parent, Uint argc, 
     Value argv[])
  {
    return JS_ConstructObjectWithArguments
      (cx, clasp, proto, parent, argc, reinterpret_cast<jsval *>(argv));
  }
  
  ObjectPtr DefineObject
    (ContextPtr cx, ObjectPtr obj, const Byte *name, ClassPtr clasp, ObjectPtr proto, 
     Uint attrs)
  {
    return JS_DefineObject(cx, obj, name, clasp, proto, attrs);
  }
  
  Bool DefineConstDoubles
    (ContextPtr cx, ObjectPtr obj, ConstDoubleSpec cds[])
  {
    return JS_DefineConstDoubles(cx, obj, reinterpret_cast<JSConstDoubleSpec *>(cds));
  }
  
  Bool DefineProperties
    (ContextPtr cx, ObjectPtr obj, PropertySpec ps[])
  {
    return JS_DefineProperties(cx, obj, reinterpret_cast<JSPropertySpec *>(ps));
  }
  
  Bool DefineProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Value value, 
     PropertyOp *getter, PropertyOp *setter, Uint attrs)
  {
    return JS_DefineProperty
      (cx, obj, name, value,
       reinterpret_cast<JSPropertyOp>(getter), 
       reinterpret_cast<JSPropertyOp>(setter), attrs);
  }
  
  Bool GetPropertyAttributes
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Uint &attrs, Bool &found)
  {
    return JS_GetPropertyAttributes(cx, obj, name, &attrs, &found);
  }
  
  Bool GetPropertyAttrsGetterAndSetter
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Uint &attrs, Bool &found, 
     PropertyOp *&getterp, PropertyOp *&setterp)
  {
    return JS_GetPropertyAttrsGetterAndSetter
      (cx, obj, name, &attrs, &found,  
       reinterpret_cast<JSPropertyOp *>(&getterp), 
       reinterpret_cast<JSPropertyOp *>(&setterp));
                                              
  }
  
  Bool SetPropertyAttributes
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Uint attrs, Bool &found)
  {
    return JS_SetPropertyAttributes(cx, obj, name, attrs, &found);
  }
  
  Bool DefinePropertyWithTinyId
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Uint8 tinyid, Value value, 
     PropertyOp *getter, PropertyOp *setter, Uint attrs)
  {
    return JS_DefinePropertyWithTinyId
      (cx, obj, name, tinyid, value, 
       reinterpret_cast<JSPropertyOp>(getter), 
       reinterpret_cast<JSPropertyOp>(setter), attrs);
  }

  Bool AliasProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, const Byte *alias)
  {
    return JS_AliasProperty(cx, obj, name, alias);
  }
  
  Bool HasProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Bool &found)
  {
    return JS_HasProperty(cx, obj, name, &found);
  }
  
  Bool LookupProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Value &vref)
  {
    return JS_LookupProperty(cx, obj, name, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool LookupPropertyWithFlags
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Uint flags, Value &vref)
  {
    return JS_LookupPropertyWithFlags
      (cx, obj, name, flags, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool GetProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Value &vref)
  {
    return JS_GetProperty(cx, obj, name, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool GetMethod
    (ContextPtr cx, ObjectPtr obj, const Byte *name, ObjectPtr &objp, Value &vref)
  {
    return JS_GetMethod(cx, obj, name, objp, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool SetProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Value &vref)
  {
    return JS_SetProperty(cx, obj, name, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool DeleteProperty
    (ContextPtr cx, ObjectPtr obj, const Byte *name)
  {
    return JS_DeleteProperty(cx, obj, name);
  }
  
  Bool DeleteProperty2
    (ContextPtr cx, ObjectPtr obj, const Byte *name, Value &rval)
  {
    return JS_DeleteProperty2(cx, obj, name, reinterpret_cast<jsval *>(&rval));
  }
  
  Bool DefineUCProperty
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Value value, 
     PropertyOp *getter, PropertyOp *setter, Uint attrs)
  {
    return JS_DefineUCProperty
      (cx, obj, name, namelen, value, 
       reinterpret_cast<JSPropertyOp>(getter), 
       reinterpret_cast<JSPropertyOp>(setter), attrs);
  }
  
  Bool GetUCPropertyAttributes
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Uint &attrs, 
     Bool &found)
  {
    return JS_GetUCPropertyAttributes(cx, obj, name, namelen, &attrs, &found);
  }
  
  Bool GetUCPropertyAttrsGetterAndSetter
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Uint &attrs, 
     Bool &found, PropertyOp *&getterp, PropertyOp *&setterp)
  {
    return JS_GetUCPropertyAttrsGetterAndSetter
      (cx, obj, name, namelen, &attrs, &found, 
       reinterpret_cast<JSPropertyOp *>(&getterp), 
       reinterpret_cast<JSPropertyOp *>(&setterp));
  }
  
  Bool SetUCPropertyAttributes
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Uint attrs, 
     Bool &found)
  {
    return JS_SetUCPropertyAttributes(cx, obj, name, namelen, attrs, &found);
  }
  
  Bool DefineUCPropertyWithTinyId
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Int8 tinyid,
     Value value, PropertyOp *getter, PropertyOp *setter, Uint attrs)
  {
    return JS_DefineUCPropertyWithTinyId
      (cx, obj, name, namelen, tinyid, value, 
       reinterpret_cast<JSPropertyOp>(getter), 
       reinterpret_cast<JSPropertyOp>(setter), attrs);
  }
  
  Bool HasUCProperty
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Bool &b)
  {
    return JS_HasUCProperty(cx, obj, name, namelen, &b);
  }
  
  Bool LookupUCProperty
    (ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Value &vref)
  {
    return JS_LookupUCProperty
      (cx, obj, name, namelen, reinterpret_cast<jsval *>(&vref));
  }
  
  Bool GetUCProperty(ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Value &v);
  Bool SetUCProperty(ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Value &v);
  Bool DeleteUCProperty2(ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Value &rval);
  ObjectPtr NewArrayObject(ContextPtr cx, Int length, Value vector[])
  {
    return JS_NewArrayObject(cx, length, reinterpret_cast<jsval *>(vector));
  }

  Bool IsArrayObject(ContextPtr cx, ObjectPtr obj);
  Bool GetArrayLength(ContextPtr cx, ObjectPtr obj, Uint &length);
  Bool SetArrayLength(ContextPtr cx, ObjectPtr obj, Uint length);
  Bool HasArrayLength(ContextPtr cx, ObjectPtr obj, Uint &length);
  Bool DefineElement(ContextPtr cx, ObjectPtr obj, Int index, Value value, PropertyOp *getter, PropertyOp *setter, Uint attrs);
  Bool AliasElement(ContextPtr cx, ObjectPtr obj, const Byte *name, Int alias);
  Bool HasElement(ContextPtr cx, ObjectPtr obj, Int index, Bool *foundp);
  Bool LookupElement(ContextPtr cx, ObjectPtr obj, Int index, Value &v);
  Bool GetElement(ContextPtr cx, ObjectPtr obj, Int index, Value &v);
  Bool SetElement(ContextPtr cx, ObjectPtr obj, Int index, Value &v);
  Bool DeleteElement(ContextPtr cx, ObjectPtr obj, Int index);
  Bool DeleteElement2(ContextPtr cx, ObjectPtr obj, Int index, Value &rval);
  void ClearScope(ContextPtr cx, ObjectPtr obj);
  IdArray *Enumerate(ContextPtr cx, ObjectPtr obj);
  ObjectPtr NewPropertyIterator(ContextPtr cx, ObjectPtr obj);
  Bool NextProperty(ContextPtr cx, ObjectPtr iterobj, Id &idref);
  Bool CheckAccess(ContextPtr cx, ObjectPtr obj, Id id, AccessMode mode, Value &v, Uint &attrs);
  CheckAccessOp *SetCheckObjectAccessCallback(RuntimePtr rt, CheckAccessOp *acb);
  Bool GetReservedSlot(ContextPtr cx, ObjectPtr obj, Uint32 index, Value &v);
  Bool SetReservedSlot(ContextPtr cx, ObjectPtr obj, Uint32 index, Value v);
  RefCount HoldPrincipals(ContextPtr cx, Principals &principals)
  {
    return JSPRINCIPALS_HOLD
      (cx, reinterpret_cast<JSPrincipals *>(&principals));    
  }
  
  RefCount DropPrincipals(ContextPtr cx, Principals &principals)
  {
    return JSPRINCIPALS_DROP
      (cx, reinterpret_cast<JSPrincipals *>(&principals));    
  }
  
  PrincipalsTranscoder *SetPrincipalsTranscoder(RuntimePtr rt, PrincipalsTranscoder *px);
  ObjectPrincipalsFinder *SetObjectPrincipalsFinder(RuntimePtr rt, ObjectPrincipalsFinder *fop);
  FunctionPtr NewFunction(ContextPtr cx, Native &call, Uint nargs, Uint flags, ObjectPtr parent, const Byte *name);
  ObjectPtr GetFunctionObject(FunctionPtr fun);
  const Byte *GetFunctionName(FunctionPtr fun);
  StringPtr GetFunctionId(FunctionPtr fun);
  Uint GetFunctionFlags(FunctionPtr fun);
  Bool ObjectIsFunction(ContextPtr cx, ObjectPtr obj);
  Bool DefineFunctions(ContextPtr cx, ObjectPtr obj, FunctionSpec fs[]);
  FunctionPtr DefineFunction(ContextPtr cx, ObjectPtr obj, const Byte *name, Native &call, Uint nargs, Uint attrs);
  FunctionPtr DefineUCFunction(ContextPtr cx, ObjectPtr obj, const Char *name, Size namelen, Native &call, Uint nargs, Uint attrs);
  ObjectPtr CloneFunctionObject(ContextPtr cx, ObjectPtr funobj, ObjectPtr parent);
  Bool BufferIsCompilableUnit(ContextPtr cx, ObjectPtr obj, const Byte *bytes, Size length);
  ScriptPtr CompileScript(ContextPtr cx, ObjectPtr obj, const Byte *bytes, Size length, const Byte *filename, Uint lineno);
  ScriptPtr CompileScriptForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Byte *bytes, Size length, const Byte *filename, Uint lineno);
  ScriptPtr CompileUCScript(ContextPtr cx, ObjectPtr obj, const Char *chars, Size length, const Byte *filename, Uint lineno);
  ScriptPtr CompileUCScriptForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Char *chars, Size length, const Byte *filename, Uint lineno);
  ScriptPtr CompileFile(ContextPtr cx, ObjectPtr obj, const Byte *filename);
  ScriptPtr CompileFileHandle(ContextPtr cx, ObjectPtr obj, const Byte *filename, FILE *fh);
  ScriptPtr CompileFileHandleForPrincipals(ContextPtr cx, ObjectPtr obj, const Byte *filename, FILE *fh, Principals &principals);
  ObjectPtr NewScriptObject(ContextPtr cx, ScriptPtr script);
  ObjectPtr GetScriptObject(ScriptPtr script);
  void DestroyScript(ContextPtr cx, ScriptPtr script);
  FunctionPtr CompileFunction(ContextPtr cx, ObjectPtr obj, const Byte *name, Uint nargs, const Byte **argnames, const Byte *bytes, Size length, const Byte *filename, Uint lineno);
  FunctionPtr CompileFunctionForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Byte *name, Uint nargs, const Byte **argnames, const Byte *bytes, Size length, const Byte *filename, Uint lineno);
  FunctionPtr CompileUCFunction(ContextPtr cx, ObjectPtr obj, const Byte *name, Uint nargs, const Byte **argnames, const Char *chars, Size length, const Byte *filename, Uint lineno);
  FunctionPtr CompileUCFunctionForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Byte *name, Uint nargs, const Byte **argnames, const Char *chars, Size length, const Byte *filename, Uint lineno);
  StringPtr DecompileScript(ContextPtr cx, ScriptPtr script, const Byte *name, Uint indent);
  StringPtr DecompileFunction(ContextPtr cx, FunctionPtr fun, Uint indent);
  StringPtr DecompileFunctionBody(ContextPtr cx, FunctionPtr fun, Uint indent);
  Bool ExecuteScript(ContextPtr cx, ObjectPtr obj, ScriptPtr script, Value &rval);
  Bool ExecuteScriptPart(ContextPtr cx, ObjectPtr obj, ScriptPtr script, ExecPart part, Value &rval);
  Bool EvaluateScript(ContextPtr cx, ObjectPtr obj, const Byte *bytes, Uint length, const Byte *filename, Uint lineno, Value &rval);
  Bool EvaluateScriptForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Byte *bytes, Uint length, const Byte *filename, Uint lineno, Value &rval);
  Bool EvaluateUCScript(ContextPtr cx, ObjectPtr obj, const Char *chars, Uint length, const Byte *filename, Uint lineno, Value &rval);
  Bool EvaluateUCScriptForPrincipals(ContextPtr cx, ObjectPtr obj, Principals &principals, const Char *chars, Uint length, const Byte *filename, Uint lineno, Value &rval);
  Bool CallFunction(ContextPtr cx, ObjectPtr obj, FunctionPtr fun, Uint argc, Value argv[], Value &rval);
  Bool CallFunctionName(ContextPtr cx, ObjectPtr obj, const Byte *name, Uint argc, Value argv[], Value &rval);
  Bool CallFunctionValue(ContextPtr cx, ObjectPtr obj, Value fval, Uint argc, Value argv[], Value &rval);
  BranchCallback *SetBranchCallback(ContextPtr cx, BranchCallback *cb);

  Bool IsRunning(ContextPtr cx)
  {
    return JS_IsRunning(cx);
  }

  Bool IsConstructing(ContextPtr cx)
  {
    return JS_IsConstructing(cx);
  }

  Bool IsAssigning
    (ContextPtr cx)
  {
    return JS_IsAssigning(cx);
  }

  void SetCallReturnValue2(ContextPtr cx, Value v);
  StringPtr NewString(ContextPtr cx, Byte *bytes, Size length);
  StringPtr NewStringCopyN(ContextPtr cx, const Byte *s, Size n);
  StringPtr NewStringCopyZ(ContextPtr cx, const Byte *s);
  StringPtr InternString(ContextPtr cx, const Byte *s);
  StringPtr NewUCString(ContextPtr cx, Char *chars, Size length);
  StringPtr NewUCStringCopyN(ContextPtr cx, const Char *s, Size n);
  StringPtr NewUCStringCopyZ(ContextPtr cx, const Char *s);
  StringPtr InternUCStringN(ContextPtr cx, const Char *s, Size length);
  StringPtr InternUCString(ContextPtr cx, const Char *s);
  Byte *GetStringBytes(StringPtr str);
  Char *GetStringChars(StringPtr str);
  Size GetStringLength(StringPtr str);
  Int CompareStrings(StringPtr str1, StringPtr str2);
  StringPtr NewGrowableString(ContextPtr cx, Char *chars, Size length);
  StringPtr NewDependentString(ContextPtr cx, StringPtr str, Size start, Size length);
  StringPtr ConcatStrings(ContextPtr cx, StringPtr left, StringPtr right);
  const Char *UndependString(ContextPtr cx, StringPtr str);
  Bool MakeStringImmutable(ContextPtr cx, StringPtr str);
  void SetLocaleCallbacks(ContextPtr cx, LocaleCallbacks *callbacks[]);
  LocaleCallbacks **GetLocaleCallbacks(ContextPtr cx);
  void ReportError(ContextPtr cx, const Byte *format, ...);
  void ReportErrorNumber(ContextPtr cx, ErrorCallback *errorCallback, void *userRef, const Uint errorNumber, ...);
  void ReportErrorNumberUC(ContextPtr cx, ErrorCallback *errorCallback, void *userRef, const Uint errorNumber, ...);
  Bool ReportWarning(ContextPtr cx, const Byte *format, ...);
  Bool ReportErrorFlagsAndNumber(ContextPtr cx, Uint flags, ErrorCallback *errorCallback, void *userRef, const Uint errorNumber, ...);
  Bool ReportErrorFlagsAndNumberUC(ContextPtr cx, Uint flags, ErrorCallback *errorCallback, void *userRef, const Uint errorNumber, ...);
  void ReportOutOfMemory(ContextPtr cx);
  ErrorReporter *SetErrorReporter(ContextPtr cx, ErrorReporter *er)
  {
    return reinterpret_cast<ErrorReporter *>(JS_SetErrorReporter(cx, reinterpret_cast<JSErrorReporter>(er)));
  }

  ObjectPtr NewRegExpObject(ContextPtr cx, Byte *bytes, Size length, Uint flags);
  ObjectPtr NewUCRegExpObject(ContextPtr cx, Char *chars, Size length, Uint flags);
  void SetRegExpInput(ContextPtr cx, StringPtr input, Bool multiline);
  void ClearRegExpStatics(ContextPtr cx);
  void ClearRegExpRoots(ContextPtr cx);

  Bool IsExceptionPending(ContextPtr cx)
  {
    return JS_IsExceptionPending(cx);
  }

  Bool GetPendingException(ContextPtr cx, Value &vref)
  {
    return JS_GetPendingException(cx, vref);
  }

  void SetPendingException(ContextPtr cx, Value v)
  {
    JS_SetPendingException(cx, v);
  }

  void ClearPendingException(ContextPtr cx)
  {
    JS_ClearPendingException(cx);
  }

  Bool ReportPendingException(ContextPtr cx)
  {
    return JS_ReportPendingException(cx);
  }
  
  ExceptionState *SaveExceptionState(ContextPtr cx);
  void RestoreExceptionState(ContextPtr cx, ExceptionState &state);
  void DropExceptionState(ContextPtr cx, ExceptionState &state);
  ErrorReport *ErrorFromException(ContextPtr cx, Value v);
  Bool ThrowReportedError(ContextPtr cx, const Byte *message, ErrorReport &report);
  Word GetContextThread(ContextPtr cx);
  Word SetContextThread(ContextPtr cx);
  Word ClearContextThread(ContextPtr cx);

jsmm_namespace_END
