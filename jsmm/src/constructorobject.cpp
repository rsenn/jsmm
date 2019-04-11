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

#include <jsmm/constructorobject>
#include <jsmm/prototypeobject>

jsmm_namespace_BEGIN

ConstructorObject::ConstructorObject
  ()
{}

ConstructorObject::ConstructorObject
  (ContextRef cx, ObjectAPI obj, ObjectPtr parent_proto, ClassAPI cls, 
   Native &ctor, Uint8 nargs, PropertySpec ps[], FunctionSpec fs[],
   PropertySpec static_ps[], FunctionSpec static_fs[],
   ObjectFlags flags) :
  FunctionObject
    (cx, ConstructorObjectAPI::create
     (cx, obj, parent_proto, cls, ctor, nargs, 
      ps, fs, static_ps, static_fs), flags),
  prototype(cx, ConstructorObjectAPI(cx, *this).getPrototype(cx), flags)
{}

ConstructorObject::ConstructorObject
  (ContextRef cx, ObjectAPI obj, ObjectPtr parent_proto, ClassAPI cls,
   Native &ctor, Uint8 nargs, Prototype &proto, Prototype &static_proto,
   ObjectFlags flags) :
  FunctionObject
     (cx, ConstructorObjectAPI::create
      (cx, obj, parent_proto, cls, ctor, nargs,
       proto.getProperties(), proto.getFunctions(),
       static_proto.getProperties(), static_proto.getFunctions()), flags),
  prototype(cx, ConstructorObjectAPI(cx, *this).getPrototype(cx), flags)
{}

ConstructorObject::ConstructorObject
  (ContextRef cx, ObjectAPI obj, ObjectPtr parent_proto, ClassAPI cls,
   Native &ctor, Uint8 nargs, Prototype &proto, ObjectFlags flags) :
  FunctionObject
     (cx, ConstructorObjectAPI::create
      (cx, obj, parent_proto, cls, ctor, nargs,
       proto.getProperties(), proto.getFunctions(), 0, 0), flags),
  prototype(cx, ConstructorObjectAPI(cx, *this).getPrototype(cx), flags)
{}

ConstructorObject::~ConstructorObject
  ()
{
/*    if(proto && (proto->getFlags() & DeleteObject) == 0)
    delete proto;*/
}

PrototypeObject &ConstructorObject::getPrototype
  ()
{
  return prototype;
}

void *ConstructorObject::getVTable
  ()
{
  ConstructorObject obj;
  return obj.Object::getVTablePtr();
}

jsmm_namespace_END
