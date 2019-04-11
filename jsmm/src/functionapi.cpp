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

#include <jsmm/functionobjectapi>
#include <jsmm/value>

#include <js/jsapi.h>
#include <js/jsfun.h>

#include <iostream>

jsmm_namespace_BEGIN

FunctionAPI::FunctionAPI()
{}

FunctionAPI::FunctionAPI(FunctionPtr ptr) : FunctionRef(ptr)
{}

FunctionAPI::FunctionAPI(FunctionRef ref) : FunctionRef(ref)
{}

/**
 * Creates a new JS function that wraps a native C function.
 * 
 * \param cx      Context in which to create function.
 * \param fn      The C function call.
 * \param args    Number of arguments.
 * \param flags   Function flags.
 * \param parent  Parent object of the function.
 * \param name    Name of the function.
 * 
 * \return        true on success, false on error.
 */
FunctionPtr FunctionAPI::create
  (ContextRef cx, Native &fn, Uint args, FunctionFlags flags, 
   ObjectPtr parent, const char *name)
{
  JSFunction *fun = JS_NewFunction(cx, reinterpret_cast<JSNative>(&fn), 
                                   args, flags, parent, name);
  if(fun) fun->flags = flags;
  return fun;
}

/**
 * Gets the function name
 */
const char *FunctionAPI::getName() const
{
  return JS_GetFunctionName(getPtr());
}

FunctionFlags FunctionAPI::getFlags() const
{
  return JS_GetFunctionFlags(getPtr());
}

/**
 * Gets the function object
 */
ObjectPtr FunctionAPI::getObject() const
{
  return JS_GetFunctionObject(getPtr());
}

/*  const FunctionObjectAPI &FunctionAPI::getObject() const
{
  return *reinterpret_cast<FunctionObjectAPI *>
    (getPtr()->object);
}*/

Function *FunctionAPI::getInstance(ContextRef cx) const
{
  Value inst;
  ObjectAPI obj = getObject();

  obj.getReservedSlot(cx, 0, inst);

  return static_cast<Function *>
    (static_cast<void *>(inst.getPrivate()));
}

void *FunctionAPI::getVTable(ContextRef cx) const
{
  Value inst;
  ObjectAPI obj = getObject();

  obj.getReservedSlot(cx, 1, inst);

  return static_cast<void *>(inst.getPrivate());
}

/**
 * Clones the function object for that function.
 */
ObjectPtr FunctionAPI::cloneObject
  (ContextRef cx, ObjectPtr parent) const
{
  return JS_CloneFunctionObject(cx, getObject(), parent);
}

/**
 * Is it a native function?
 */
Bool FunctionAPI::isNative() const
{
  return !getPtr()->interpreted;
}

StringPtr FunctionAPI::decompile
  (ContextRef cx, Uint indent) const
{
  return JS_DecompileFunction(cx, getPtr(), indent);
}

StringPtr FunctionAPI::decompileBody
  (ContextRef cx, Uint indent) const
{
  return JS_DecompileFunctionBody(cx, getPtr(), indent);
}

/**
 * Call the function.
 * 
 * \param cx     Context in which to call function.
 * \param obj    Object passed to function as 'this'.
 * \param argc   Number of arguments.
 * \param argv   Argument vector.
 * \param rval   Pointer to return value.
 * 
 * \return       true on success, false on error.
 */
Bool FunctionAPI::call
  (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval) const
{
  return JS_CallFunction(cx, obj, getPtr(), argc,
                         argv ? argv->getValuePointer() : 0,
                         rval.getValuePointer());
}

/**
 * Call a function by name.
 * 
 * \param cx    Context in which to call function.
 * \param obj   Object passed to function as 'this'.
 * \param name  Function name.
 * \param argc  Number of arguments.
 * \param argv  Argument vector.
 * \param rval  Pointer to return value.
 * 
 * \return      true on success, false on error
 */
Bool FunctionAPI::call
  (ContextRef cx, ObjectPtr obj, const char *name, Uint argc,
   Value argv[], Value &rval)
{
  return JS_CallFunctionName(cx, obj, name, argc,
                             argv ? argv->getValuePointer() : 0,
                             rval.getValuePointer());
}

/**
 * Call a function by value.
 * 
 * \param cx    Context in which to call function.
 * \param obj   Object passed to function as 'this'.
 * \param val   Value containing the function.
 * \param argc  Number of arguments.
 * \param argv  Argument vector.
 * \param rval  Pointer to return value.
 * 
 * \return      true on success, false on error.
 */
Bool FunctionAPI::call
  (ContextRef cx, ObjectPtr obj, const Value &val, Uint argc, 
   Value argv[], Value &rval) 
{
  return JS_CallFunctionValue(cx, obj, val.getJSValue(), argc,
                              argv ? argv->getValuePointer() : 0,
                              rval.getValuePointer());
}

/*  Value FunctionAPI::toValue() const
{
  return OBJECT_TO_JSVAL(getObject().getPtr());*
}*/

jsmm_namespace_END
