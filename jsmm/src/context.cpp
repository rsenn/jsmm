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

#include <jsmm/context>
#include <jsmm/runtime>
#include <jsmm/engine>
#include <jsmm/errorreport>
#include <jsmm/contextexception>
#include <jsmm/stackframe>
#include <jsmm/object>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

/**
 * Create a new script execution context.
 *
 * This constructor will request the default runtime from JS::Engine.
 *
 * \param stackChunkSize  Stack chunk allocation size.
 */
Context::Context(Uint stackChunkSize) :
  ContextAPI(ContextAPI::create(Engine::getEngine().requestRuntime(), 
                                  stackChunkSize)),
  globalObject(0)
{
  setInstance(this);
  setErrorReporter(defaultErrorReporter);
}

/**
 * Create a new script execution context.
 *
 * \param rt              Create context within the specified runtime.
 * \param stackChunkSize  Stack chunk allocation size.
 */
Context::Context(RuntimeRef rt, Uint stackChunkSize) :
  ContextAPI(ContextAPI::create(rt, stackChunkSize)),
  globalObject(0)
{
  setInstance(this);
  setErrorReporter(defaultErrorReporter);
}

/**
 * Destroys the associated getPtr(), but only if it was created by
 * this instance.
 */
Context::~Context()
{
  if(getInstance() == this)
    JS_DestroyContext(getPtr());

  if(globalObject)
    delete globalObject;
}

Object *Context::getGlobalObject()
{
  /* There is already a global object instance - return it */
  if(globalObject)
    return globalObject;

  /* No instance, but perhaps there is a global object */
  ObjectPtr obj = ContextAPI::getGlobalObject();
  
  if(obj)
  {
    globalObject = new Object((ContextRef)*this, obj, DeleteObject);
    return globalObject;
  }
  
  return 0;
}

Object &Context::requestGlobalObject()
{
  ContextAPI::requestGlobalObject();
  return *getGlobalObject();
}

/**
 * Default error reporter
 */
void Context::defaultErrorReporter(ContextPtr cx, const char *msg,
                                   ErrorReport &err)
{    
  throw ContextException(cx, "Runtime error: " + (std::string)msg);
/*    std::cerr << msg << std::endl;
  std::cerr << (errorString ? errorString : "(null)") << std::endl;
  std::cerr << err.filename << ':' << err.lineNo << ": "
            << err.lineBuf << std::endl;*/
}

/** 
 * Frees the stack space allocated to the Context. 
 * Calling this method will also result in a GC being performed.
 */
/*  void Context::destroy()
{
  JS_DestroyContext(getPtr());
  getPtr() = 0;
}*/

/** 
 * Frees the stack space allocated to the Context.
 * Garbage collection will not be performed.
 */
/*  void Context::destroyNoGC()
{
  JS_DestroyContextNoGC(getPtr());
  getPtr() = 0;
}*/

/** 
 * Frees the stack space allocated to the Context.
 * Maybe garbage collection will be performed.
 */
/*  void Context::destroyMaybeGC()
{
  JS_DestroyContextMaybeGC(getPtr());
  getPtr() = 0;
}*/

/**
 * Set the getPtr() private pointer to a pointer to 
 * a JS::Context instance.
 * 
 * \param rt  Pointer to a JS::Context instance or 0.
 */
void Context::setInstance(Context *rt) const
{
  JS_SetContextPrivate(getPtr(), rt);
}

jsmm_namespace_END
