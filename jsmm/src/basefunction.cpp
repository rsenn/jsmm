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

#include <jsmm/basefunction>
#include <jsmm/functionobject>
#include <jsmm/context>
#include <jsmm/value>
#include <jsmm/stackframe>
#include <jsmm/functionexception>

#include <js/jsapi.h>
#include <js/jsfun.h>

#include <iostream>

jsmm_namespace_BEGIN

/** 
 * Dispatches calls to JavaScript functions and calls the 'native' method
 * of the associated function class which inherited from BaseFunction.
 */
static Bool dispatchCall(ContextRef cx, ObjectPtr obj, Uint argc,
                         Value argv[], Value &rval)
{
  StackFrame fp = ContextAPI(cx).getStackFrame();
  FunctionAPI fnimpl = fp.getFunction();
  Function *fn;

  void *fakeFn[2];
  
  if(fnimpl.getFlags() & InstanceFunction)
  {
    fn = fnimpl.getInstance(cx);
    
    if(!fn)
      throw FunctionException(&fnimpl, "Has no instance.");
  }
  else
  {
    fakeFn[0] = fnimpl.getVTable(cx);
    fakeFn[1] = fnimpl.getPtr();
   
    if(!fakeFn[0])
      throw FunctionException(&fnimpl, "Has no VTable set.");
    
    fn = reinterpret_cast<Function *>(fakeFn);
  }
  
  BaseFunction *basefn = dynamic_cast<BaseFunction *>(fn);
  
  if(!basefn)
    throw FunctionException(fn, "Is not a BaseFunction.");
  
  return basefn->native(cx, obj, argc, argv, rval);
}

BaseFunction::BaseFunction
  (ContextRef cx, Uint args, Int flags, ObjectPtr obj, const char *name) :
  Function(cx, dispatchCall, args, flags, obj, name)
{
//    setVTable(getVTablePtr());
}

jsmm_namespace_END
