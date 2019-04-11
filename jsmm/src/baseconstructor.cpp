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

#include <jsmm/baseconstructor>
#include <jsmm/constructorapi>
#include <jsmm/stackframe>
#include <jsmm/constructorexception>

jsmm_namespace_BEGIN

/** 
 * Dispatches calls to JavaScript constructors and calls the 'native' method
 * of the associated function class which inherited from BaseConstructor.
 */
static Bool dispatchConstruct(ContextRef cx, ObjectPtr obj, Uint argc,
                              Value argv[], Value &rval)
{
  StackFrame fp = ContextAPI(cx).getStackFrame();
  ConstructorAPI ctorimpl = fp.getFunction();
  Constructor *ctor;

  void *fakeCtor[2];
  
  if(ctorimpl.getFlags() & InstanceFunction)
  {
    ctor = ctorimpl.getInstance(cx);
    
    if(!ctor)
      throw ConstructorException(&ctorimpl, "Has no instance.");
  }
  else
  {
    fakeCtor[0] = ctorimpl.getVTable(cx);
    fakeCtor[1] = ctorimpl.getPtr();
   
    if(!fakeCtor[0])
      throw ConstructorException(&ctorimpl, "Has no VTable set.");
    
    ctor = reinterpret_cast<Constructor *>(fakeCtor);
  }
  
  BaseConstructor *basector = dynamic_cast<BaseConstructor *>(ctor);
  
  if(!basector)
    throw ConstructorException(ctor, "Is not a BaseConstructor.");
  
  return basector->native(cx, obj, argc, argv, rval);
}

BaseConstructor::BaseConstructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, ClassAPI cls,
   Uint8 nargs, PropertySpec ps[], FunctionSpec fs[],
   PropertySpec static_ps[], FunctionSpec static_fs[]) :
  Constructor(cx, obj, parent_proto, cls, dispatchConstruct, nargs, 
              ps, fs, static_ps, static_fs)
{}

BaseConstructor::BaseConstructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, ClassAPI cls,
   Uint8 nargs, Prototype &proto) :
  Constructor(cx, obj, parent_proto, cls, dispatchConstruct, nargs, proto)
{}

BaseConstructor::BaseConstructor
  (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, ClassAPI cls,
   Uint8 nargs, Prototype &proto, Prototype &static_proto) :
  Constructor(cx, obj, parent_proto, cls, dispatchConstruct, nargs, proto, static_proto)
{}

jsmm_namespace_END
