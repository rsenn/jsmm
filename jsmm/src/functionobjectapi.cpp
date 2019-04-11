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

#include <js/jsapi.h>

jsmm_namespace_BEGIN

FunctionObjectAPI::FunctionObjectAPI(ContextRef cx, ObjectRef ref) : 
  ObjectAPI(ref)
{    
  if(!isFunction(cx))
    throw ObjectException(this, "Not a function");
}


FunctionObjectAPI::FunctionObjectAPI(FunctionAPI fn) :
  ObjectAPI(fn.getObject())
{}

/**
 * Gets a reference to the function the object contains.
 * 
 * \param cx   Context from which to derive runtime information.
 * \return     FunctionRef reference to the JSFunction.
 */
FunctionAPI FunctionObjectAPI::getFunction(ContextRef cx) const
{
  return FunctionPtr(static_cast<JSFunction *>(ObjectAPI::getPrivate(cx)));
}

/**
 * Gets the function instance the object contains.
 * 
 * \param cx   Context from which to derive runtime information.
 * \return     A reference to the Function instance.
 */
/*  Function *FunctionObjectAPI::getFunctionInstance(ContextRef cx) const
{
  Value ret;

  getReservedSlot(cx, 0, ret);
  
  return static_cast<Function *>
    (static_cast<void *>(ret.getPrivate()));
}*/

/*  void *FunctionObjectAPI::getVTable()
{
  FunctionObject obj(JSVAL_TO_OBJECT(-1));
  return obj.getVTablePtr();
}*/

jsmm_namespace_END
