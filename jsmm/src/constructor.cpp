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

#include <jsmm/constructor>
#include <jsmm/prototype>
#include <jsmm/constructorapi>

jsmm_namespace_BEGIN

Constructor::Constructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, ClassAPI cls,
   Native &ctor, Uint8 nargs, PropertySpec ps[], FunctionSpec fs[],
   PropertySpec static_ps[], FunctionSpec static_fs[]) :
  Function(cx, ConstructorAPI::create
           (cx, obj, parent_proto, cls, ctor, nargs, ps, fs, static_ps, static_fs)),
  prototype(cx, ConstructorAPI(*this).getPrototype(cx))
{}

Constructor::Constructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, 
   ClassAPI cls, Native &ctor, Uint8 nargs, Prototype &proto) :
  Function(cx, ConstructorAPI::create
           (cx, obj, parent_proto, cls, ctor, nargs, 
            proto.getProperties(), proto.getFunctions())),
  prototype(cx, ConstructorAPI(*this).getPrototype(cx))
{}

Constructor::Constructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, ClassAPI cls,
   Native &ctor, Uint8 nargs, Prototype &proto, Prototype &static_proto) :
  Function(cx, ConstructorAPI::create
           (cx, obj, parent_proto, cls, ctor, nargs, 
            proto.getProperties(), proto.getFunctions(), 
            static_proto.getProperties(), static_proto.getFunctions())),
  prototype(cx, ConstructorAPI(*this).getPrototype(cx))
{}

Constructor::~Constructor()
{
}

PrototypeObject &Constructor::getPrototype()
{
  return prototype;
}

jsmm_namespace_END
