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

#include <jsmm/contextapi>
#include <jsmm/objectapi>
#include <jsmm/class>
#include <jsmm/value>
#include <jsmm/api>

#include <iostream>
#include <js/jscntxt.h>

jsmm_namespace_BEGIN

/**
 * Creates an internal getPtr() structure
 * 
 * \param rt         Create context within the specified runtime.
 * \param stackChunkSize  Stack chunk allocation size.
 * \return           true on success, false on error
 */
ContextPtr ContextAPI::create(RuntimeRef rt, Uint stackChunkSize)
{
  return NewContext(rt, stackChunkSize);
}

/**
 * Get a pointer to the JS::Context object which created 
 * the JSContext structure.
 * 
 * \return  Pointer to the JS::Context instance or 0.
 */
Context *ContextAPI::getInstance() const
{
  return static_cast<Context *>(GetContextPrivate(*this));
}
  
/**
 * Gets the runtime associated with this context.
 * 
 * \return  A reference to a 'Runtime' object
 */
Runtime &ContextAPI::getRuntime() const
{
  return *static_cast<Runtime *>(GetRuntimePrivate(GetRuntime(*this)));
}

/**
 * Gets the global object of this context.
 * 
 * \return  A pointer to an 'Object' object or NULL
 */
ObjectPtr ContextAPI::getGlobalObject() const
{
  return GetGlobalObject(*this);
}

/**
 * Gets the global object of this context.
 * If it does not exist, then it is created and the
 * standard classes are initialized on it.
 * 
 * \return  A pointer to an 'Object' object
 */
ObjectRef ContextAPI::requestGlobalObject()
{
  /* If it doesn't exist, then create it */
  if(!GetGlobalObject(*this))
  {
    ObjectAPI obj = ObjectAPI::create
      (*this, Class::getDefaultClass(), 0, 0);

    obj.initStandardClasses(*this);
  }

  return GetGlobalObject(*this);
}

/**
 * Sets the global object for this context
 */
void ContextAPI::setGlobalObject(ObjectPtr obj) const
{
  SetGlobalObject(*this, obj);
}

/**
 * Sets the error reporter
 */
void ContextAPI::setErrorReporter(ErrorReporter *report) const
{
  SetErrorReporter(*this, report);
}

/**
 * Performs garbage collection 
 */
void ContextAPI::garbageCollect() const
{
  GC(*this);
}

/**
 * Maybe performs garbage collection 
 */
void ContextAPI::maybeGarbageCollect() const
{
  MaybeGC(*this);
}

Bool ContextAPI::lockGCThing(void *thing) const
{
  return LockGCThing(*this, thing);
}

Bool ContextAPI::unlockGCThing(void *thing) const
{
  return UnlockGCThing(*this, thing);
}

/**
 * Registers an object pointer as a member of the garbage collector's
 * root set, to protect the object the root points at from garbage 
 * collection.
 * 
 * \param obj  The object to root.
 * \return     true if successful, false otherwise.
 */
Bool ContextAPI::addRoot(ObjectRef &obj) const
{
  return AddRoot(*this, &obj);
}

/**
 * Registers a pointer to an object, aWord with a name string for debugging
 * and diagnostics, as a member of the garbage collector's root set, to 
 * protect the object the root points at from garbage collection.
 * 
 * \param obj   The object to root.
 * \param name  The name of the root.
 * \return      true if successful, false otherwise.
 */
Bool ContextAPI::addNamedRoot(ObjectRef &obj, const char *name) const
{
  return AddNamedRoot(*this, &obj, name);
}

// Bool ContextAPI::addRoot(StringPtr &obj, const char *name);
// Bool ContextAPI::addDouble(DoublePtr &obj, const char *name);

/**
 * Registers a value pointer as a member of the garbage collector's
 * root set, to protect the value the root points at from garbage 
 * collection.
 * 
 * \param val  The value to root.
 * \return     true if successful, false otherwise.
 */
/*  Bool ContextAPI::addRoot(Value &val)
{
  if(!val.isGCThing())
    return false;

  return AddRoot(*this, &val);
}*/

/**
 * Registers a pointer to a value, aWord with a name string for debugging
 * and diagnostics, as a member of the garbage collector's root set, to
 * protect the value the root points at from garbage collection.
 * 
 * \param val   The value to root.
 * \param name  The name of the root.
 * \return      true if successful, false otherwise.
 */
/*  Bool ContextAPI::addNamedRoot(Value &val, const char *name)
{
  if(!val.isGCThing())
    return false;

  return AddNamedRoot(*this, &val, name);
}*/

/**
 * Unregisters a pointer to an object so that it is no longer a member of
 * the garbage collector's root set. This enables the pointed-at object
 * to be garbage collected as needed.
 * 
 * \param obj  The object root to remove.
 * \return     true if successful, false otherwise.
 */
/*  Bool ContextAPI::removeRoot(ObjectRef &obj)
{
  return RemoveRoot(*this, &obj);
}*/

// Bool ContextAPI::removeRoot(StringPtr &obj);
// Bool ContextAPI::removeRoot(DoublePtr &obj);

/**
 * Unregisters a pointer to a value so that it is no longer a member of
 * the garbage collector's root set. This enables the pointed-at value
 * to be garbage collected as needed.
 * 
 * \param val  The value root to remove.
 * \return     true if successful, false otherwise.
 */
/*  Bool ContextAPI::removeRoot(Value &val)
{
  if(!val.isGCThing())
    return false;
  
  return RemoveRoot(*this, &val);
}*/

/**
 * The last GC thing of each type (object, string, double, external string
 * types) created on a given context is kept alive until another thing of the
 * same type is created, using a newborn root in the context.  These newborn
 * roots help native code protect newly-created GC-things from GC invocations
 * activated before those things can be rooted using local or global roots.
 *
 * However, the newborn roots can also entrain great gobs of garbage, so the
 * JS_GC entry point clears them for the context on which GC is being forced.
 * Embeddings may need to do likewise for all contexts.
 *
 * See the scoped local root API immediately below for a better way to manage
 * newborns in cases where native hooks (functions, getters, setters, etc.)
 * create many GC-things, potentially without connecting them to predefined
 * local roots such as *rval or argv[i] in an active native function.  Using
 * JS_EnterLocalRootScope disables updating of the context's per-gc-thing-
 * type newborn roots, until control flow unwinds and leaves the outermost
 * nesting local root scope.
 */
/*  void ContextAPI::clearNewbornRoots()
{
  ClearNewbornRoots(*this);
}*/

/*@{*/
/*
 * Scoped local root management allows native functions, getter/setters, etc.
 * to avoid worrying about the newborn root pigeon-holes, overloading local
 * roots allocated in argv and *rval, or ending up having to call JS_Add*Root
 * and JS_RemoveRoot to manage global roots temporarily.
 *
 * Instead, calling JS_EnterLocalRootScope and JS_LeaveLocalRootScope around
 * the body of the native hook causes the engine to allocate a local root for
 * each newborn created in between the two API calls, using a local root 
 * stack associated with cx.  For example:
 *
 *    JSBool
 *    my_GetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
 *    {
 *        JSBool ok;
 *
 *        if (!JS_EnterLocalRootScope(cx))
 *            return JS_FALSE;
 *        ok = my_GetPropertyBody(cx, obj, id, vp);
 *        JS_LeaveLocalRootScope(cx);
 *        return ok;
 *    }
 *
 * NB: JS_LeaveLocalRootScope must be called once for every prior successful
 * call to JS_EnterLocalRootScope.  If JS_EnterLocalRootScope fails, you must
 * not make the matching JS_LeaveLocalRootScope call.
 *
 * In case a native hook allocates many objects or other GC-things, but the
 * native protects some of those GC-things by storing them as property values
 * in an object that is itself protected, the hook can call JS_ForgetLocalRoot
 * to free the local root automatically pushed for the now-protected GC-thing.
 * 
 * JS_ForgetLocalRoot works on any GC-thing allocated in the current local
 * root scope, but it's more time-efficient when called on references to more
 * recently created GC-things.  Calling it successively on other than the most
 * recently allocated GC-thing will tend to average the time inefficiency, and
 * may risk O(n^2) growth rate, but in any event, you shouldn't allocate too
 * many local roots if you can root as you go (build a tree of objects from
 * the top down, forgetting each latest-allocated GC-thing immediately upon
 * linking it to its parent).
 */
Bool ContextAPI::enterLocalRootScope() const
{
  return EnterLocalRootScope(*this);
}

void ContextAPI::leaveLocalRootScope() const
{
  LeaveLocalRootScope(*this);
}

void ContextAPI::forgetLocalRoot(ObjectRef &obj) const
{
  ForgetLocalRoot(*this, &obj);
}

void ContextAPI::forgetLocalRoot(Value &val) const
{
  ForgetLocalRoot(*this, &val);
}

/*@}*/ 

/** 
 * Indicates to the JS engine that an application thread is entering
 * a critical section that calls the JS API freely but does not block.
 */
void ContextAPI::beginRequest() const
{
  BeginRequest(*this);
}

/**
 * Indicates to the JS engine that an application thread is leaving a 
 * critical section that calls the JS API freely but does not block.
 */
void ContextAPI::endRequest() const
{
  EndRequest(*this);
}

/** 
 * Yield to pending GC operations, regardless of request depth 
 */
void ContextAPI::yieldRequest() const
{
  YieldRequest(*this);
}
/** 
 * Indicates to the JS engine that an application thread executing 
 * JS API calls in a request can suspend its API-calling critical 
 * section and block.
 */
RefCount ContextAPI::suspendRequest() const
{
  return SuspendRequest(*this);
}

/**
 * Indicates to the JS engine that an application thread in a suspended
 * request wants to resume executing JS API calls from non-blocking 
 * code. Restarts a previously suspended thread.
 */
void ContextAPI::resumeRequest(RefCount saveDepth) const
{
  ResumeRequest(*this, saveDepth);
}

/**
 * Gets the current stack frame.
 */
StackFramePtr ContextAPI::getStackFrame() const
{
  return getPtr()->fp;
}

/**
 * Currently executing a script? 
 * 
 * \return  true if executing, false if not.
 */
Bool ContextAPI::isRunning() const
{
  return (getPtr()->fp != 0);
}

/**
 * Currently constructing a new object?
 * 
 * \return  true if constructing, false if not.
 */
Bool ContextAPI::isConstructing() const
{
  return IsConstructing(*this);
}

/**
 * Currently assigning a value to a variable?
 * 
 * \return  true if assigning, false if not.
 */
Bool ContextAPI::isAssigning() const
{
  return IsAssigning(*this);
}

/**
 * Allocate context-bound memory.
 * 
 * \param nbytes  Number of bytes to allocate.
 * \return        Pointer to allocated memory or 0.
 */
void *ContextAPI::malloc(Uint nbytes) const
{
  return JS::malloc(*this, nbytes);
}

/**
 * Re-allocate context-bound memory.
 * 
 * \param ptr     Pointer to allocated memory.
 * \param nbytes  Number of bytes to re-allocate.
 * \return        Pointer to allocated memory or 0.
 */
void *ContextAPI::realloc(void *ptr, Uint nbytes) const
{
  return JS::realloc(*this, ptr, nbytes);
}

/**
 * Free context-bound memory.
 * 
 * \param ptr  Pointer to allocated memory.
 */
void ContextAPI::free(void *ptr) const
{
JS::free(*this, ptr);
}

/**
 * Duplicate a string into context-bound memory.
 * 
 * \param str  The string to duplicate.
 * \return     Pointer to the duplicated string or 0.
 */
char *ContextAPI::strdup(const char *str) const
{
  return JS::strdup(*this, str);
}

RefCount ContextAPI::holdPrincipals(Principals &pcps) const
{
  return HoldPrincipals(*this, pcps);
}

RefCount ContextAPI::dropPrincipals(Principals &pcps) const
{
  return DropPrincipals(*this, pcps);
}

Bool ContextAPI::isExceptionPending() const
{
  return IsExceptionPending(*this);
}

Bool ContextAPI::getPendingException(Value &v) const
{
  return GetPendingException(*this, v);
}

void ContextAPI::setPendingException(Value &v) const
{
  SetPendingException(*this, v);
}

void ContextAPI::clearPendingException() const
{
  ClearPendingException(*this);
}

Bool ContextAPI::reportPendingException() const
{
  return ReportPendingException(*this);
}

jsmm_namespace_END
