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

#include <jsmm/arrayobject>
#include <jsmm/arrayobjectapi>
#include <jsmm/string>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

ArrayObject::ArrayObject(ContextRef cx, Int length, Value vector[]) :
  Object(cx, ArrayObjectAPI::create(cx, length, vector))
{}

ArrayObject::ArrayObject(ContextRef cx, const std::string &str, 
                         const Byte *sep) :
  Object(cx, ArrayObjectAPI::create(cx, 0))
{
  const Byte *s = str.c_str();
  const Byte *tok;
  Uint l;
  Int i = 0;
  Uint slen = strlen(sep);
  
  while((tok = strstr(s, sep)))
  {
    l = tok - s;
    String jstr(cx, s, l);
    
    setElement(cx, i++, jstr);
    s += l + slen;
  }
  
  l = strlen(s);
  
  if(l)
  {
    String jstr(cx, s, l);
    setElement(cx, i, jstr);
  }
}

Bool ArrayObject::getLength
  (ContextRef cx, Uint &length) const
{
  return JS_GetArrayLength
    (cx, getPtr(), reinterpret_cast<jsuint *>(&length));
}

Bool ArrayObject::setLength
  (ContextRef cx, Uint length)
{
  return JS_SetArrayLength(cx, getPtr(), length);
}

Bool ArrayObject::defineElement
  (ContextRef cx, Int index, Value value, 
   PropertyOp &getter, PropertyOp &setter, Uint attrs)
{
  return JS_DefineElement(cx, getPtr(), index, value.getJSValue(),
                          reinterpret_cast<JSPropertyOp>(&getter),
                          reinterpret_cast<JSPropertyOp>(&setter), attrs);
}

Bool ArrayObject::aliasElement
  (ContextRef cx, const char *name, Int alias)
{
  return JS_AliasElement(cx, getPtr(), name, alias);
}

Bool ArrayObject::hasElement
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

Bool ArrayObject::lookupElement
  (ContextRef cx, Int index, Value &v) const
{
  return JS_LookupElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObject::getElement
  (ContextRef cx, Int index, Value &v) const
{
  return JS_GetElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObject::setElement
  (ContextRef cx, Int index, Value v)
{
  return JS_SetElement(cx, getPtr(), index, v.getValuePointer());
}

Bool ArrayObject::deleteElement
  (ContextRef cx, Int index)
{
  return JS_DeleteElement(cx, getPtr(), index);
}

Bool ArrayObject::deleteElement
  (ContextRef cx, Int index, Value &rval)
{
  return JS_DeleteElement2(cx, getPtr(), index, rval.getValuePointer());
}

/**
 * JS::ArrayObject structure:
 * 
 * 0x00: Pointer to vtable
 * 0x04: Pointer to JSObject
 */
void *ArrayObject::getVTable() 
{
  ArrayObject obj;
  return obj.getVTablePtr();
}

jsmm_namespace_END
