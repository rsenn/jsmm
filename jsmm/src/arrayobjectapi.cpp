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

#include <jsmm/arrayobjectapi>
#include <jsmm/objectexception>
#include <jsmm/api>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

ArrayObjectAPI::ArrayObjectAPI(ObjectAPI obj) :
  ObjectAPI(obj)
{
}

ArrayObjectAPI::ArrayObjectAPI(ContextRef cx, ObjectAPI obj) :
  ObjectAPI(obj)
{
  if(!obj.isArrayObject(cx))
    throw ObjectException(this, "Not an array object.");
}

ObjectPtr ArrayObjectAPI::create
  (ContextRef cx, Int length, Value vector[])
{
  return NewArrayObject(cx, length, vector);
}

Bool ArrayObjectAPI::getLength
  (ContextRef cx, Uint &length) const
{
  return JS_GetArrayLength
    (cx, getPtr(), reinterpret_cast<jsuint *>(&length));
}

Bool ArrayObjectAPI::setLength
  (ContextRef cx, Uint length)
{
  return JS_SetArrayLength(cx, getPtr(), length);
}

Bool ArrayObjectAPI::defineElement
  (ContextRef cx, Int index, Value value, 
   PropertyOp &getter, PropertyOp &setter, Uint attrs)
{
  return JS_DefineElement(cx, getPtr(), index, value.getJSValue(),
                          reinterpret_cast<JSPropertyOp>(&getter),
                          reinterpret_cast<JSPropertyOp>(&setter), attrs);
}

Bool ArrayObjectAPI::aliasElement
  (ContextRef cx, const char *name, Int alias)
{
  return JS_AliasElement(cx, getPtr(), name, alias);
}

Bool ArrayObjectAPI::hasElement
  (ContextRef cx, Int index, Bool &found) const
{
  JSBool b;
  
  if(JS_HasElement(cx, getPtr(), index, &b))
  {
    found = b;
    return true;
  }
  return false;
}

Bool ArrayObjectAPI::lookupElement
  (ContextRef cx, Int index, Value &v) const
{
  return JS_LookupElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObjectAPI::getElement
  (ContextRef cx, Int index, Value &v) const
{
  return JS_GetElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObjectAPI::setElement
  (ContextRef cx, Int index, Value v)
{
  return JS_SetElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObjectAPI::deleteElement
  (ContextRef cx, Int index)
{
  return JS_DeleteElement(cx, getPtr(), index);
}

Bool ArrayObjectAPI::deleteElement
  (ContextRef cx, Int index, Value &rval)
{
  return JS_DeleteElement2(cx, getPtr(), index, rval.getValuePointer());
}

jsmm_namespace_END
