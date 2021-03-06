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

#include <jsmm/prototypeobject>
#include <jsmm/objectexception>

jsmm_namespace_BEGIN

PrototypeObject::PrototypeObject()
{}

PrototypeObject::PrototypeObject
  (ContextRef cx, PrototypeObjectAPI obj, ObjectFlags flags) : 
  Object(cx, obj, flags)
{}

PrototypeObject::PrototypeObject
  (ContextRef cx, ObjectRef parent, ObjectPtr parent_proto, ClassAPI cls,
   PropertySpec ps[], FunctionSpec fs[], ObjectFlags flags) :
  Object(cx, PrototypeObjectAPI::create
         (cx, parent, parent_proto, cls, ps, fs), flags)
{}

PrototypeObject::PrototypeObject
  (ContextRef cx, ObjectRef parent, ObjectPtr parent_proto, ClassAPI cls, 
   Prototype &proto, ObjectFlags flags) :
  Object(cx, PrototypeObjectAPI::create
         (cx, parent, parent_proto, cls, 
          proto.getProperties(), 
          proto.getFunctions()), flags)
{}

void *PrototypeObject::getVTable()
{
  PrototypeObject obj;
  return obj.Object::getVTablePtr();
}

jsmm_namespace_END
