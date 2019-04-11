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

#include <jsmm/constructorapi>
#include <jsmm/constructorobjectapi>
#include <jsmm/constructorexception>

jsmm_namespace_BEGIN

ConstructorAPI::ConstructorAPI
  (FunctionPtr ptr) : FunctionAPI(ptr)
{}

ConstructorAPI::ConstructorAPI
  (FunctionRef ref) : FunctionAPI(ref)
{}

FunctionPtr ConstructorAPI::create
  (ContextRef cx, ObjectRef obj, ObjectPtr parent_proto, ClassAPI cls, 
   Native &ctor, Uint8 nargs, PropertySpec ps[], FunctionSpec fs[],
   PropertySpec static_ps[], FunctionSpec static_fs[]) 
{
  FunctionObjectAPI ctorobj(cx, ConstructorObjectAPI::create
    (cx, obj, parent_proto, cls, ctor, nargs, ps, fs, static_ps, static_fs));
  
  return ctorobj.getFunction(cx);
}

Constructor *ConstructorAPI::getInstance
  (ContextRef cx) const
{
  Value inst;
  ObjectAPI obj = getObject();
  
  obj.getReservedSlot(cx, 0, inst);
  
  return static_cast<Constructor *>
    (static_cast<void *>(inst.getPrivate()));
}

PrototypeObjectAPI ConstructorAPI::getPrototype
  (ContextRef cx) const
{
  Value pval;
  ObjectAPI ctorobj = getObject();
  
  if(ctorobj.lookupProperty(cx, "prototype", pval))
  {
    if(pval.isObject())
      return PrototypeObjectAPI(pval.getObject());
  }

  throw ConstructorException(this, "Could not get prototype object.");
}

jsmm_namespace_END
