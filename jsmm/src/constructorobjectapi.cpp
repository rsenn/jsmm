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

#include <jsmm/constructorobjectapi>
#include <jsmm/objectexception>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

ConstructorObjectAPI::ConstructorObjectAPI
  (ContextRef cx, ObjectPtr ptr) : FunctionObjectAPI(cx, ptr)
{
  Value pval;
  
  if(!lookupProperty(cx, "prototype", pval) || pval.isUndefined())
    throw ObjectException(this, "Constructor has no 'prototype' property.");
}

ObjectPtr ConstructorObjectAPI::create
  (ContextRef cx, ObjectAPI obj, ObjectPtr parent_proto, ClassAPI cls,
   Native &ctor, Uint nargs, PropertySpec ps[], FunctionSpec fs[],
   PropertySpec static_ps[], FunctionSpec static_fs[])
{
  ObjectPtr proto = cls.init
    (cx, obj, parent_proto, &ctor, nargs, ps, fs, static_ps, static_fs);

  if(proto)
  {
    Value cval;
    
    if(obj.lookupProperty(cx, cls.getName(), cval) && cval.isObject())
      return cval.getObject();
  }    
  
  return 0;
}

PrototypeObjectAPI ConstructorObjectAPI::getPrototype
  (ContextRef cx) const
{
  Value pval;
  
  if(lookupProperty(cx, "prototype", pval))
  {
    if(pval.isObject())
      return PrototypeObjectAPI(pval.getObject());
  }
  
  throw ObjectException(this, "Could not get prototype object.");
}

jsmm_namespace_END
