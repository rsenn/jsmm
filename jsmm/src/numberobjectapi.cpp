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

#include <jsmm/numberobjectapi>
#include <jsmm/objectexception>
#include <jsmm/api>
#include <jsmm/object>
#include <jsmm/class>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

NumberObjectAPI::NumberObjectAPI(ObjectRef obj) :
  ObjectAPI(obj)
{
}

NumberObjectAPI::NumberObjectAPI(ContextRef cx, ObjectRef obj) :
  ObjectAPI(obj)
{
  if(!isNumberObject(cx))
    throw ObjectException(this, "Not an number object.");
}

ObjectPtr NumberObjectAPI::create
  (ContextRef cx, Double num)
{
  Object obj(cx, Class::getNumberClass(), 0, 0);
  obj.setPrivate(cx, Value::newDouble(cx, num));
  
  return obj;
}

Bool NumberObjectAPI::setNumber
  (ContextRef cx, Double num)
{
  setPrivate(cx, Value::newDouble(cx, num));
  return true;
}

jsmm_namespace_END
