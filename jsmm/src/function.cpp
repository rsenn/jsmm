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

#include <jsmm/function>
#include <jsmm/context>
#include <jsmm/object>
#include <jsmm/value>
#include <jsmm/stackframe>
#include <jsmm/string>

#include <js/jsapi.h>
#include <js/jsfun.h>

#include <iostream>

jsmm_namespace_BEGIN

Function::Function() :
  FunctionAPI()
//    jsContext(0)
{}

Function::Function(ContextRef cx, FunctionRef fn) : 
  FunctionAPI(fn)//,
//    jsContext(0)
{
  setInstance(cx, this);
}

/**
 * Creates a new JS function that wraps a native C function.
 * 
 * \param cx      Context in which to create function.
 * \param fn      The C function call.
 * \param args    Number of arguments.
 * \param flags   Function flags.
 * \param parent  Parent object of the function.
 * \param name    Name of the function.
 */
Function::Function(ContextRef cx, Native &fn, Uint args, FunctionFlags flags,
                   ObjectPtr obj, const char *name) :
  FunctionAPI(create(cx, fn, args, flags, obj, name))//,
//    jsContext(cx)
{
  setInstance(cx, this);
}

/*  FunctionObject Function::getObject(ContextRef cx)
{
  return FunctionObject(cx, *this);
}*/

/**
 * Define a function as a method of an object.
 * 
 * \param cx     Context in which to create function.
 * \param obj    Object that will receive the method.
 * \param name   Name of the function.
 * \param fn     The C function call.
 * \param args   Number of arguments.
 * \param flags  Function flags.
 */
/*  Function::Function(ContextPtr cx, ObjectPtr obj, const char *name, 
                   Native &fn, Uint args, Uint flags) :
  FunctionRef(0)
{
//    define(cx, obj, name, fn, args, flags);
}*/

/**
 * Destructor
 */
Function::~Function()
{
/*    if(jsContext)
    setInstance(jsContext, 0);*/
}

void Function::setInstance(ContextRef cx, Function *fn) const
{
  ObjectAPI obj = getObject();
  
  obj.setReservedSlot(cx, 0, Value::newPrivate(fn));
}

void Function::setVTable(ContextRef cx) const
{
  ObjectAPI obj = getObject();

  obj.setReservedSlot(cx, 1, Value::newPrivate(getVTablePtr()));
}

void *Function::getVTablePtr() const
{
  return *(void **)this;
}

/**
 * Define a function as a method of an object.
 * 
 * \param cx     Context in which to create function.
 * \param obj    Object that will receive the method.
 * \param name   Name of the function.
 * \param fn     The C function call.
 * \param args   Number of arguments.
 * \param flags  Function flags.
 * 
 * \return       true on success, false on error.
 */
/*  FunctionPtr Function::define(ContextPtr cx, ObjectPtr obj, const char *name,
                             Native &fn, Uint args, Int flags)
{
  return FunctionPtr
    (JS_DefineFunction(&cx.getJSContext(), &obj.getJSObject(),
                       name, reinterpret_cast<JSNative>(&fn),
                       args, flags));
}
*/

jsmm_namespace_END

