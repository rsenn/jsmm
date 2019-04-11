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

#include <jsmm/engine>
#include <jsmm/runtime>
#include <jsmm/class>
#include <jsmm/arrayobject>
#include <jsmm/functionobject>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

/** 
 * Built-in classes
 * @{
 */
static Class ArgumentsClass(*JS_GetArgumentsClass());
static Class ArrayClass(*JS_GetArrayClass());
static Class BoolClass(*JS_GetBooleanClass());
static Class CallClass(*JS_GetCallClass());
static Class FunctionClass(*JS_GetFunctionClass());
static Class NumberClass(*JS_GetNumberClass());
static Class ObjectClass(*JS_GetObjectClass());
static Class RegExpClass(*JS_GetRegExpClass());
static Class ScriptClass(*JS_GetScriptClass());
static Class StringClass(*JS_GetStringClass());
static Class WithClass(*JS_GetWithClass());
/* @} */

/** Global engine instance */
static Engine SpiderMonkey;

/**
 * Constructor: Set default runtime to zero.
 */
Engine::Engine() : m_runtime(0)
{
  initBuiltinClasses();
}

/**
 * Destroys all associated runtimes and 
 * shuts down the SpiderMonkey engine.
 */
Engine::~Engine()
{
  /* If there was a default runtime, destroy it now */
  if(m_runtime)
    delete m_runtime;

  /* Finally shut down the SpiderMonkey engine */
  if(this == &SpiderMonkey)
    JS_ShutDown();
}

/**
 * Initializes built-in classes.
 */
void Engine::initBuiltinClasses()
{
  Class::getArgumentsClass().initBuiltin(3);
  Class::getArrayClass().initBuiltin(3, ArrayObject::getVTable());
  Class::getBoolClass().initBuiltin(3);
  Class::getCallClass().initBuiltin(3);

  // The Function class has already 2 reserved slots for XPconnect
  Class::getFunctionClass().initBuiltin(3, FunctionObject::getVTable());

  Class::getNumberClass().initBuiltin(3);
  Class::getObjectClass().initBuiltin(3, Object::getVTable());
  Class::getRegExpClass().initBuiltin(3);
  Class::getScriptClass().initBuiltin(3);
  Class::getStringClass().initBuiltin(3);
  Class::getWithClass().initBuiltin(3);

  Class::setDefaultClass(Class::getObjectClass());
}

/**
 * Creates/gets a default runtime
 */
Runtime &Engine::requestRuntime()
{
  if(!m_runtime)
    m_runtime = new Runtime();
    
  return *m_runtime;
}

/**
 * Returns the global engine class
 */
Engine &Engine::getEngine()
{
  return SpiderMonkey;
}

/**
 * Return the implementation version 
 */
std::string &Engine::getImplementationVersion()
{
  const char *version = JS_GetImplementationVersion();
  return *(new std::string(version ? version : ""));
}

/**
 * Abort the process in a non-graceful manner. This will cause a core file,
 * call to the debugger or other moral equivalent as well as causing the
 * entire process to stop.
 */
#if 0
void Engine::abort()
{
  JS_Abort();
}
#endif

/*@{*/
/**
 * Getters for the built-in classes 
 */  
Class &Class::getArgumentsClass()
{ return ArgumentsClass; }

Class &Class::getArrayClass()
{ return ArrayClass; }

Class &Class::getBoolClass()
{ return BoolClass; }

Class &Class::getCallClass()      
{ return CallClass; }

Class &Class::getFunctionClass()
{ return FunctionClass; }

Class &Class::getNumberClass()
{ return NumberClass; }

Class &Class::getObjectClass()
{ return ObjectClass; }

Class &Class::getRegExpClass()
{ return RegExpClass; }

Class &Class::getScriptClass()
{ return ScriptClass; }

Class &Class::getStringClass()
{ return StringClass; }

Class &Class::getWithClass()
{ return WithClass; }
/*@}*/

jsmm_namespace_END
