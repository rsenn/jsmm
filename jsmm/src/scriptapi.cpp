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

#include <jsmm/scriptapi>
#include <jsmm/contextapi>
#include <jsmm/value>

#include <js/jsapi.h>
#include <js/jsscript.h>

using namespace std;

jsmm_namespace_BEGIN

/**
 * Gets the optional Script-class object wrapper.
 */
const char *ScriptAPI::getName() const
{
  return getPtr()->filename;
}

/**
 * Gets the optional Script-class object wrapper.
 */
ObjectPtr ScriptAPI::getObject() const
{
  return getPtr()->object;
}

/**
 * Creates new script object.
 */
ObjectPtr ScriptAPI::newObject(ContextRef cx) const
{
  JS_NewScriptObject(cx, getPtr());
  return getObject();
}

/**
 * Executes a previously compiled script.
 *
 * \param cx    Context in which we execute the script
 * \param rval  Reference to return value
 * 
 * \return  true if script execution was successful
 */
Bool ScriptAPI::execute(ContextRef cx, Value &rval) const
{
  return execute(cx, ContextAPI(cx).requestGlobalObject(), rval);
}

/**
 * Executes a previously compiled script.
 *
 * \param cx    Context in which we execute the script
 * \param obj   Object with which the script is associated
 * \param rval  Reference to return value
 * 
 * \return  true if script execution was successful
 */
Bool ScriptAPI::execute(ContextRef cx, ObjectPtr obj, Value &rval) const
{
  return JS_ExecuteScript(cx, obj, getPtr(), rval.getValuePointer());
}  

/**
 * Compiles a script from the specified file.
 * 
 * \param cx        Context in which we create the script
 * \param filename  Filename of the script to compile
 */
ScriptPtr ScriptAPI::compileFile(ContextRef cx, const string &filename)
{
  return compileFile(cx, ContextAPI(cx).requestGlobalObject(), filename);
}

/**
 * Compiles a script from the specified file.
 * 
 * \param cx        Context in which we create the script
 * \param obj       Object with which the script is associated
 * \param filename  Filename of the script to compile
 */
ScriptPtr ScriptAPI::compileFile(ContextRef cx, ObjectRef obj,
                                  const string &filename)
{
  return JS_CompileFile(cx, obj, filename.c_str());
}

/**
 * Compiles a script from a byte buffer.
 * If there is a previously compiled script, it is destroyed.
 * 
 * \param cx      Context in which we create the script
 * \param buf     Byte buffer containing the script
 * \param len     Length of the buffer
 * \param name    Script name
 * \param lineno  Start counting lines from lineno (0)
 * 
 * \return  true if compilation was successful, false on error
 */
ScriptPtr ScriptAPI::compileBuffer(ContextRef cx, const char *buf,
                                    Uint len, const string &name, 
                                    Uint lineno)
{
  return compileBuffer(cx, ContextAPI(cx).requestGlobalObject(), buf, len, 
                       name, lineno);
}

/**
 * Compiles a script from a byte buffer.
 * 
 * \param cx      Context in which we create the script
 * \param obj     Object with which the script is associated
 * \param buf     Byte buffer containing the script
 * \param len     Length of the buffer
 * \param name    Script name
 * \param lineno  Start counting lines from lineno (0)
 */
ScriptPtr ScriptAPI::compileBuffer(ContextRef cx, ObjectPtr obj,
                                    const char *buf, Uint len, 
                                    const string &name, Uint lineno)
{
  return JS_CompileScript(cx, obj, buf, len, name.c_str(), lineno);
}

/**
 * Compiles a script from a string.
 * If there is a previously compiled script, it is destroyed.
 * 
 * \param cx      Context in which we create the script
 * \param script  String containing the script
 * \param name    Script name
 * \param lineno  Start counting lines from lineno (0)
 * 
 * \return  true if compilation was successful, false on error
 */
ScriptPtr ScriptAPI::compileString
  (ContextRef cx, const string &script, const string &name, Uint lineno)
{
  return compileBuffer(cx, script.c_str(), script.length(), name, lineno);
}

/**
 * Compiles a script from a string.
 * If there is a previously compiled script, it is destroyed.
 * 
 * \param cx      Context in which we create the script
 * \param obj     Object with which the script is associated
 * \param script  String containing the script
 * \param name    Script name
 * \param lineno  Start counting lines from lineno (0)
 * 
 * \return  true if compilation was successful, false on error
 */
ScriptPtr ScriptAPI::compileString
  (ContextRef cx, ObjectPtr obj, const string &script, const string &name,
   Uint lineno)
{
  return compileBuffer(cx, obj, script.c_str(), script.length(), name, 
                       lineno);
}

StringPtr ScriptAPI::decompile(ContextRef cx, const char *name, Uint indent) const
{
  return JS_DecompileScript(cx, getPtr(), name, indent);
} 

/**
 * Given a buffer, return false if the buffer might become a valid
 * JavaScript statement with the addition of more lines. Otherwise
 * return true. The intent is to support interactive compilation -
 * accumulate lines in a buffer until isCompilableUnit is true, then
 * pass it to the compiler.
 */
Bool ScriptAPI::isCompilableUnit(ContextRef cx, const char *bytes,
                                  Uint length)
{
  return JS_BufferIsCompilableUnit(cx, ContextAPI(cx).requestGlobalObject(),
                                   bytes, length);
}


void ScriptAPI::destroy(ContextRef cx)
{
  JS_DestroyScript(cx, getPtr());
  jsPtr = 0;
}

jsmm_namespace_END
