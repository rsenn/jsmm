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

#include <jsmm/functionobject>
#include <jsmm/objectexception>

jsmm_namespace_BEGIN

FunctionObject::FunctionObject()
{}

FunctionObject::FunctionObject(ContextRef cx, ObjectAPI obj, ObjectFlags flags) :
  Object(cx, obj, flags), Function(cx, obj.getFunction(cx))
{
  if(!isFunction(cx))
    throw ObjectException(this, "Object is not a function");
}

FunctionObject::FunctionObject(ContextRef cx, FunctionObjectAPI obj, ObjectFlags flags) :
  Object(cx, obj, flags),
  Function(cx, obj.getFunction(cx))
{}

FunctionObject::FunctionObject(ContextRef cx, FunctionAPI fn, ObjectFlags flags) :
  Object(cx, fn.getObject(), flags), Function(cx, fn)
{}

void *FunctionObject::getVTable()
{
  FunctionObject obj;
  return obj.Object::getVTablePtr();
}

jsmm_namespace_END
