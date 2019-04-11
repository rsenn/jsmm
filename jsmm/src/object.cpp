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

#include <jsmm/object>
#include <jsmm/contextapi>

#include <js/jsapi.h>
#include <js/jsobj.h>
#include <js/jsutil.h>

jsmm_namespace_BEGIN

typedef void DestructOp(Object *obj);

Object::Object() : 
  ObjectAPI(0)
{}

/*  Object::Object(JSObject *obj) : 
  ObjectAPI(obj)
{}*/

Object::Object(ContextRef cx, ObjectRef ref, ObjectFlags flg) :
  ObjectAPI(ref),
  flags(flg)
{
  setInstance(cx, this);
}

/**
 * Creates a new object within the specified context and with the global 
 * object as parent.
 *
 * \param cx   Context from which to derive runtime information.
 * \return        true on success, false on error.
 */
/*  Object::Object(ContextAPI cx) :
  ObjectAPI(create(cx, cx.requestGlobalObject()))
{
  if(getPtr())
    setInstance(cx, this);
}*/

/**
 * Instantiates a new object and invokes its constructor.
 * 
 * Instantiates a new object based on a specified class, prototype, and
 * parent object, and then invokes its constructor function.
 * 
 * \param cx      Context associated with the runtime in which to establish 
 *                the new object.
 * \param cls     An existing Class to use for internal methods, such as 
 *                finalize. 
 * \param proto   An optional pointer to the prototype Object with which to 
 *                associate the new object.
 *
 * Set proto to 0 to force to assign a prototype object for you. In this
 * case, JS_NewObject attempts to assign the new object the prototype object
 * belonging to the Class, if one is defined there. Otherwise, it creates an
 * empty Object stub for the prototype.
 * 
 * \param parent  An optional pointer to an existing Object to which to set
 *                the new Object's parent Object property. You can set 
 *                parent to 0 if you do not want to set the parent property.
 */
Object::Object(ContextRef cx, ClassPtr cls, ObjectPtr proto,
               ObjectPtr parent) : 
  ObjectAPI(create(cx, cls, proto, parent))
{
  if(getPtr())
    setInstance(cx, this);
}

/**
 * Instantiates a new object and invokes its constructor with the provided 
 * arguments.
 * 
 * Instantiates a new object based on a specified class, prototype, and
 * parent object, and then invokes its constructor function with the providede
 * arguments.
 * 
 * \param cx      Context associated with the runtime in which to establish 
 *                the new object.
 * \param cls     An existing Class to use for internal methods, such as 
 *                finalize. 
 * \param proto   An optional pointer to the prototype Object with which to 
 *                associate the new object.
 *
 * Set proto to 0 to force to assign a prototype object for you. In this
 * case, JS_NewObject attempts to assign the new object the prototype object
 * belonging to the Class, if one is defined there. Otherwise, it creates an
 * empty Object stub for the prototype.
 * 
 * \param parent  An optional pointer to an existing Object to which to set
 *                the new Object's parent Object property. You can set 
 *                parent to 0 if you do not want to set the parent property.
 *
 * \param argc    Number of arguments you are passing to the constructor.
 * \param argv    Pointer to the array of argument values to pass to the 
 *                constructor.
 */
Object::Object(ContextRef cx, ClassPtr cls, ObjectPtr proto,
               ObjectPtr parent, Uint argc, Value argv[]) :
  ObjectAPI(constructWithArguments(cx, cls, proto, parent, argc, argv))
{
  if(getPtr())
    setInstance(cx, this);
}

Object &Object::defineObject
  (ContextRef cx, const char *name, ClassPtr cls, ObjectPtr proto, 
   Int attrs) const
{
  ObjectAPI obj = 
    ObjectAPI::defineObject(cx, name, cls, proto, attrs);

  return obj.requestInstance(cx);
}

Object &Object::defineObject
  (ContextRef cx, const char *name, ClassPtr cls, Int attrs) const
{
  ObjectAPI obj = ObjectAPI::defineObject(cx, name, cls, attrs);

  return obj.requestInstance(cx);
}

Object &Object::defineObject
  (ContextRef cx, const char *name, Int attrs) const
{
  ObjectAPI obj = ObjectAPI::defineObject(cx, name, attrs);

  return obj.requestInstance(cx);
}

Object &Object::getPrototype
  (ContextRef cx) const
{
  ObjectAPI obj = ObjectAPI::getPrototype(cx);
  
  return obj.requestInstance(cx);
}

Object &Object::getParent
  (ContextRef cx) const
{
  ObjectAPI obj = ObjectAPI::getParent(cx);
  
  return obj.requestInstance(cx);
}

/**
 * Set the last getPtr() reserved slot to a pointer to 
 * the JS::Object instance.
 * 
 * \param cx   Context from which to derive runtime information.
 * \param obj  Pointer to a JS::Object instance or 0.
 */
void Object::setInstance
  (ContextRef cx, Object *obj)
{
  setReservedSlot(cx, numReservedSlots(cx), Value::newPrivate(obj));
}

Bool Object::propertyStub
  (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
{
  return true;
}

Bool Object::enumerateStub
  (ContextRef cx, ObjectAPI obj)
{
return true;
}

Bool Object::resolveStub
  (ContextRef cx, ObjectAPI obj, Value id)
{
  return true;
}

Bool Object::convertStub
  (ContextRef cx, ObjectAPI obj, Type type, Value &vref)
{
  return true;
}

Bool Object::finalizeStub
  (ContextRef cx, ObjectAPI obj)
{
  return true;
}

/**
 * JS::Object structure:
 * 
 * 0x00: Pointer to vtable
 * 0x04: Pointer to JSObject
 */
void *Object::getVTable()
{
  Object obj;
  return obj.getVTablePtr();
}

jsmm_namespace_END
