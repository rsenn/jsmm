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
#include <jsmm/objectexception>
#include <jsmm/class>
#include <jsmm/value>
#include <jsmm/prototype>
#include <jsmm/api>

#include <js/jsapi.h>
#include <js/jsobj.h>
#include <js/jsutil.h>

jsmm_namespace_BEGIN
  
/**
 * Creates a new JSObject.
 *
 * \param cx      Context from which to derive runtime information.
 * \param parent  The parent object in hierarchy.
 * \return        true on success, false on error.
 */
ObjectPtr ObjectAPI::create
  (ContextRef cx, ObjectPtr parent)
{
  return create(cx, Class::getDefaultClass(), parent);
}

/**
 * Creates a new JSObject
 *
 * \param cx      Context from which to derive runtime information.
 * \param cls     Class to derive object from.
 * \param parent  The parent object in hierarchy.
 * \return        true on success, false on error.
 */
ObjectPtr ObjectAPI::create
  (ContextRef cx, ClassPtr cls, ObjectPtr parent)
{
  return create(cx, cls, 0, parent);
}

/**
 * Creates a new JSObject
 *
 * \param cx      Context from which to derive runtime information.
 * \param cls     Class to derive object from.
 * \param proto   The prototype object to set.
 * \param parent  The parent object in hierarchy.
 * \return        true on success, false on error.
 */
ObjectPtr ObjectAPI::create
  (ContextRef cx, ClassPtr cls, ObjectPtr proto, ObjectPtr parent)
{
  return NewObject(cx, cls, proto, parent);
}


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
ObjectPtr ObjectAPI::construct
  (ContextRef cx, ClassPtr cls, ObjectPtr proto, ObjectPtr parent)
{
  return ConstructObject(cx, cls, proto, parent);
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
 * \param parent  An optional pointer to an existing Object to which to set
 *                the new Object's parent Object property. You can set 
 *                parent to 0 if you do not want to set the parent property.
 *
 * Set proto to 0 to force to assign a prototype object for you. In this
 * case, NewObject attempts to assign the new object the prototype object
 * belonging to the Class, if one is defined there. Otherwise, it creates an
 * empty Object stub for the prototype.
 * 
 * \param argc    Number of arguments you are passing to the constructor.
 * \param argv    Pointer to the array of argument values to pass to the 
 *                constructor.
 */
ObjectPtr ObjectAPI::constructWithArguments
  (ContextRef cx, ClassPtr cls, ObjectPtr proto, ObjectPtr parent,
   Uint argc, Value argv[])
{
  return ConstructObjectWithArguments(cx, cls, proto, parent, argc, argv);
}

/**
 * Instantiates an object that is a property of another object.
 * 
 * define() instantiates and names a new Object as a property of the Object.
 * name is the property name to assign to obj to hold the new object, and
 * attrs contains the property attributes to set for the newly created
 * property. The following table lists possible values you can pass in
 * attrs, either singly, or OR'd together: 
 * 
 * JSPROP_ENUMERATE: property is visible in for loops.
 * JSPROP_READONLY:  property is read-only.
 * JSPROP_PERMANENT: property cannot be deleted.
 * JSPROP_EXPORTED:  property can be exported outside its object.
 * JSPROP_GETTER:    property has a defined getter function.
 * JSPROP_SETTER:    property has a defined setter function.
 * JSPROP_SHARED:    property is shared.
 * JSPROP_INDEX:     property is actually an array element.
 * 
 * cls is a pointer to the base class to use when creating the new object,
 * and proto is a pointer to the prototype upon which to base the new object.
 * If you set proto to 0, JS sets the prototype object for you.
 * 
 * \param cx     Context from which to derive runtime information.
 * \param name   Name of the property that encapsulates the new object.
 * \param cls    An existing Class to use for internal methods, such as 
 *               finalize. 
 * \param proto  An optional pointer to the prototype Object with which to 
 *               associate the new object.
 * \param attrs  Property attributes for the new object.
 * 
 * \return       The newly created property object if successful.
 *               If the property already exists, or cannot be created, 
 *               DefineObject returns NULL. TODO: Exception?
 */
ObjectPtr ObjectAPI::defineObject
  (ContextRef cx, const char *name, ClassPtr cls, ObjectPtr proto, 
   Int attrs) const
{
  return DefineObject(cx, getPtr(), name, cls, proto, attrs);
}

ObjectPtr ObjectAPI::defineObject
  (ContextRef cx, const char *name, ClassPtr cls, Int attrs) const
{
  return DefineObject(cx, getPtr(), name, cls, 0, attrs);
}

ObjectPtr ObjectAPI::defineObject
  (ContextRef cx, const char *name, Int attrs) const
{
  return DefineObject(cx, getPtr(), name, Class::getDefaultClass(), 0, attrs);
}

ObjectPtr ObjectAPI::createArray
  (ContextRef cx, Int length, Value *vector) 
{
  return NewArrayObject(cx, length, vector);
}

/**
 * Initializes standard classes
 * 
 * \param cx   Script execution context.
 * \return     true if successful, false if not.
 */
Bool ObjectAPI::initStandardClasses
  (ContextRef cx) const
{
  return InitStandardClasses(cx, getPtr());
}

/**
 * Retrieves the Class associated with the Object.
 * 
 * Returns a pointer to the Class associated with the Object. The class is
 * an internal data structure that you can create for objects as needed.
 * Generally you do not expose a class in your applications, but use it 
 * behind the scenes.
 *
 * \param cx   Context from which to derive runtime information.
 * 
 * \return     The class of the object.
 */
ClassRef ObjectAPI::getClass
  (ContextRef cx) const
{
  return GetClass(cx, getPtr());
}

/**
 * Determines if the Object is an instance of the specified Class. 
 * 
 * The instanceOf method determines if the Object has a Class. If the
 * Object's internal Class pointer corresponds to cls, this function
 * returns true, indicating that the object is an instance of the class. 
 * Otherwise, instanceOf returns false.
 * If you pass a non-null argument vector, argv, to instanceOf, and obj
 * is not an instance of cls, this function may report a class mismatch
 * before returning. To do so, instanceOf tests whether or not there is a
 * function name associated with the argument vector, and if there is, 
 * reports the name in an error message using the JS_ReportError function.
 * 
 * \param cx    Context from which to derive runtime information.
 * \param cls   Class against which to test the object.
 * \param argv  Optional argument vector. 
 * 
 * \return      true when the object is an instance 
 *              of the specified class, otherwise false.
 */
Bool ObjectAPI::instanceOf
  (ContextRef cx, ClassPtr cls, Value argv[]) const
{
  return InstanceOf(cx, *this, cls, argv);
}

/** 
 * Checks whether value delegates behaviour to this object. 
 * Used by the instanceof operator.
 * 
 * \param cx    Context from which to derive runtime information.
 * \param v     The value to check.
 * \param bref  Reference to the Boolean receiving the result.
 * 
 * \return      true if successful, otherwise false.
 */
Bool ObjectAPI::hasInstance
  (ContextRef cx, Value v, Bool &bref) const
{
  return HasInstance(cx, getPtr(), v, bref);
}

Bool ObjectAPI::isDelegate
  (ContextRef cx, Value v, Bool &bref) const
{
  return JS_IsDelegate(cx, getPtr(), v.getJSValue(), &bref);
}

/**
 * Get private pointer.
 * 
 * \param cx   Context from which to derive runtime information.
 * \return     Private pointer, previously set with setPrivate().
 */
void *ObjectAPI::getPrivate
  (ContextRef cx) const
{
  ensureHasPrivate(cx);
  return JS_GetPrivate(cx, getPtr());
}

/**
 * Set private pointer.
 * 
 * \param cx   Context from which to derive runtime information.
 * \param prv  Private pointer, can be retrieved using getPrivate().
 */
void ObjectAPI::setPrivate
  (ContextRef cx, void *prv) const
{
  ensureHasPrivate(cx);
  JS_SetPrivate(cx, getPtr(), prv);
}

/**
 * Retrieves the private data associated with the Object if it is
 * an instance of the specified Class.
 * 
 * getInstancePrivate determines if the Object is an instance of the Class,
 * cls, and if it is, returns a pointer to the Object's private data.
 * If the Object's internal class pointer corresponds to cls, and you do
 * not also pass an optional argument vector, argv, this function attempts
 * to retrieve a pointer to the private data. Otherwise, it returns 0.
 * 
 * If you pass a non-null argument vector, argv, to getInstancePrivate, and
 * the Object is not an instance of cls, this function reports a class
 * mismatch before returning 0. In this case, getInstancePrivate tests
 * whether or not there is a function name associated with the argument
 * vector, and if there is, reports the name in an error message using 
 * the JS_ReportError function.
 * 
 * \param cx    Context from which to derive runtime information.
 * \param cls   Class against which to test the object.
 * \param argv  Optional argument vector. If you do not want to pass an 
 *              argument vector, pass 0 for this argument.
 * 
 * \return      Pointer to private data.
 */
void *ObjectAPI::getInstancePrivate
  (ContextRef cx, ClassPtr cls, Value argv[]) const
{
  return JS_GetInstancePrivate
    (cx, getPtr(), cls, 
     (argv ? argv->getValuePointer() : 0));
}

/** 
 * Retrieves the Object's prototype.
 * 
 * getPrototype retrieves the prototype Object for the Object. 
 * A prototype object provides properties shared by similar JS objects.
 * 
 * If an Object has a prototype, getPrototype returns a pointer to the
 * prototype. If the Object does not have a prototype, or the Object 
 * finalize function is active, getPrototype returns 0.
 * 
 * \param cx    Context from which to derive runtime information.
 * 
 * \return      The prototype of the Object.
 */
ObjectPtr ObjectAPI::getPrototype
  (ContextRef cx) const
{
  return ObjectPtr
    (JS_GetPrototype(cx, getPtr()));
}

/**
 * Sets the prototype for the Object.
 * 
 * setPrototype enables you to set the prototype Object the Object.
 * A prototype object provides properties that are shared by similar Object
 * instances. Ordinarily you set a prototype for an Object when you create
 * the object, but if you do not set a prototype at that time, you can later
 * call setPrototype to do so.
 * 
 * proto is a pointer to a second existing object upon which Object is based.
 * 
 * setPrototype is successful, it returns true. Otherwise, if it cannot 
 * create and fill a prototype slot for the Object, it returns false.
 * 
 * \param cx     Context from which to derive runtime information.
 * \param proto  The prototype.
 * 
 * \return       true if successful, false if not.
 */
Bool ObjectAPI::setPrototype
  (ContextRef cx, ObjectPtr proto) const
{
  return JS_SetPrototype(cx, getPtr(), proto);
}

/**
 * Retrieves the parent Object.
 * 
 * \param cx   Context from which to derive runtime information.
 * 
 * \return     The parent object.
 */
ObjectPtr ObjectAPI::getParent
  (ContextRef cx) const
{
  return ObjectPtr
    (JS_GetParent(cx, getPtr()));
}

/**
 * Sets the parent Object.
 * 
 * \param cx      Context from which to derive runtime information.
 * \param parent  The parent Object to set.
 * 
 * \return        true if successful, false if not.
 */
Bool ObjectAPI::setParent
  (ContextRef cx, ObjectPtr parent) const
{
  return JS_SetParent(cx, getPtr(), parent);
}

/**
 * Get a unique identifier for the Object, good for the lifetime of the
 * Object (even if it is moved by a copying GC).
 * Return false on failure (likely out of memory), and true with id
 * containing the unique id on success.
 * 
 * \param cx   Context from which to derive runtime information.
 * \param id   Reference to variable receiving the Id.
 * 
 * \return     true if successful, false if not.
 */
Bool ObjectAPI::getId
  (ContextRef cx, Id &id) const
{
  return JS_GetObjectId(cx, getPtr(), &id);
}   

/**
 * Seal the object.
 * 
 * \param cx    Context from which to derive runtime information.
 * \param deep  Indicates whether child objects are also sealed.
 * \return      true if successful, false if not.
 */
Bool ObjectAPI::sealObject
  (ContextRef cx, Bool deep) const
{
  return JS_SealObject
    (cx, getPtr(), deep);
}

/*  Bool ObjectAPI::definePrototype
  (ContextRef cx, Prototype &proto)
{
  if(!defineFunctions(cx, proto.getFunctions()))
    return false;
  
  if(!defineProperties(cx, proto.getProperties()))
    return false;
  
  return true;
}*/

Bool ObjectAPI::defineConstDoubles
  (ContextRef cx, ConstDoubleSpec cds[]) const
{
  return JS_DefineConstDoubles
    (cx, getPtr(), 
     reinterpret_cast<JSConstDoubleSpec *>(cds));
}

Bool ObjectAPI::defineProperties
  (ContextRef cx, PropertySpec ps[]) const
{
  return JS_DefineProperties
    (cx, getPtr(), reinterpret_cast<JSPropertySpec *>(ps));
}

Bool ObjectAPI::defineProperty
  (ContextRef cx, const char *name, Value value,
   PropertyOp &getter, PropertyOp &setter, Int attrs) const
{
  return JS_DefineProperty
    (cx, getPtr(), name, 
     value.getJSValue(),
     reinterpret_cast<JSPropertyOp>(&getter),
     reinterpret_cast<JSPropertyOp>(&setter),
     attrs);
}

Bool ObjectAPI::defineProperty(ContextRef cx, const Char *name,
                               Uint namelen, Value value, 
                               PropertyOp &getter, PropertyOp &setter, 
                               Int attrs) const
{
  return JS_DefineUCProperty
    (cx, getPtr(), name,
     namelen, value.getJSValue(), 
     reinterpret_cast<JSPropertyOp>(&getter),
     reinterpret_cast<JSPropertyOp>(&setter),
     attrs);
}

Bool ObjectAPI::getPropertyAttributes
  (ContextRef cx, const char *name, PropertyAttr &attrs, Bool &found) const
{
  JSBool b;
  Uint tmpattr = 0;
  if(JS_GetPropertyAttributes(cx, getPtr(), name,
                              &tmpattr, &b))
  {
    found = b;
    attrs = static_cast<PropertyAttr>(tmpattr);
    return true;
  }
  return false;
}

Bool ObjectAPI::getPropertyAttributes
  (ContextRef cx, const Char *name, Uint namelen, PropertyAttr &attrs, 
   Bool &found) const
{
  JSBool b;
  uintN tmpattr = 0;
  if(JS_GetUCPropertyAttributes(cx, getPtr(), name,
                                namelen, &tmpattr, &b))
  {
    found = b;
    attrs = static_cast<PropertyAttr>(tmpattr);
    return true;
  }
  return false;
}


Bool ObjectAPI::getPropertyAttrsGetterAndSetter
  (ContextRef cx, const char *name, PropertyAttr &attrs, Bool &found,
   PropertyOp *&getter, PropertyOp *&setter) const
{
  JSBool b;
  uintN tmpattr = 0;
  
  if(JS_GetPropertyAttrsGetterAndSetter
     (cx, getPtr(), name, &tmpattr, &b,
      reinterpret_cast<JSPropertyOp *>(&getter),
      reinterpret_cast<JSPropertyOp *>(&setter)))
  {
    found = b;
    attrs = static_cast<PropertyAttr>(tmpattr);
    return true;
  }
  return false;
}

Bool ObjectAPI::getPropertyAttrsGetterAndSetter
  (ContextRef cx, const Char *name, Uint namelen, PropertyAttr &attrs, 
   Bool &found, PropertyOp *&getter, PropertyOp *&setter) const
{
  JSBool b;
  Uint tmpattr = 0;
  if(JS_GetUCPropertyAttrsGetterAndSetter
     (cx, getPtr(), name, namelen, &tmpattr, &b,
      reinterpret_cast<JSPropertyOp *>(&getter),
      reinterpret_cast<JSPropertyOp *>(&setter)))
  {
    found = b;
    attrs = static_cast<PropertyAttr>(tmpattr);
    return true;
  }
  return false;
}

Bool ObjectAPI::setPropertyAttributes
  (ContextRef cx, const char *name, Int attrs, Bool &found) const
{
  JSBool b;
  
  if(JS_SetPropertyAttributes(cx, getPtr(), name,
                              attrs, &b))
  {
    found = b;
    return true;
  }
  return false;
}

Bool ObjectAPI::setPropertyAttributes
  (ContextRef cx, const Char *name, Uint namelen, Int attrs, Bool &found)
  const
{
  JSBool b;
  
  if(JS_SetUCPropertyAttributes(cx, getPtr(), name, 
                                namelen, attrs, &b))
  {
    found = b;
    return true;
  }
  return false;
}

Bool ObjectAPI::definePropertyWithTinyId
  (ContextRef cx, const char *name, Uint8 tinyid, Value value,
   PropertyOp &getter, PropertyOp &setter, Int attrs) const
{
  return JS_DefinePropertyWithTinyId
    (cx, getPtr(), name, tinyid, value.getJSValue(),
     reinterpret_cast<JSPropertyOp>(&getter),
     reinterpret_cast<JSPropertyOp>(&setter),
     attrs);
}

Bool ObjectAPI::definePropertyWithTinyId
  (ContextRef cx, const Char *name, Uint namelen, Uint8 tinyid, Value value,
   PropertyOp &getter, PropertyOp &setter, Int attrs) const
{
  return JS_DefineUCPropertyWithTinyId
    (cx, getPtr(), name, namelen, tinyid, 
     value.getJSValue(),
     reinterpret_cast<JSPropertyOp>(&getter),
     reinterpret_cast<JSPropertyOp>(&setter),
     attrs);
}


Bool ObjectAPI::aliasProperty
  (ContextRef cx, const char *name, const char *alias) const
{
  return JS_AliasProperty(cx, getPtr(), name, alias);
}


Bool ObjectAPI::hasProperty
  (ContextRef cx, const char *name, Bool &found) const
{
  JSBool b;
  
  if(JS_HasProperty(cx, getPtr(), name, &b))
  {
    found = b;
    return true;
  }
  return false;
}

Bool ObjectAPI::hasProperty
  (ContextRef cx, const Char *name, Uint namelen, Bool &found) const
{
  JSBool b;
  
  if(JS_HasUCProperty(cx, getPtr(), name, namelen, &b))
  {
    found = b;
    return true;
  }
  return false;
}

Bool ObjectAPI::lookupProperty
  (ContextRef cx, const char *name, Value &v) const
{
  return JS_LookupProperty(cx, getPtr(), name, 
                           v.getValuePointer());
}

Bool ObjectAPI::lookupProperty
  (ContextRef cx, const Char *name, Uint namelen, Value &v) const
{
  return JS_LookupUCProperty
    (cx, getPtr(), name, namelen, v.getValuePointer());
}

Bool ObjectAPI::lookupPropertyWithFlags
  (ContextRef cx, const char *name, PropertyAttr flags, Value &v) const
{
  return JS_LookupPropertyWithFlags
    (cx, getPtr(), name, flags, v.getValuePointer());
}

/*  Bool ObjectAPI::lookupPropertyWithFlags(ContextRef cx, const Char *name,
                                        Uint namelen, PropertyAttr flags, 
                                        Value &v)
{
  return JS_LookupUCPropertyWithFlags(cx, getPtr(), name, namelen, flags, v.getValuePointer());
}*/

Bool ObjectAPI::getProperty
  (ContextRef cx, const char *name, Value &v) const
{
  return JS_GetProperty
    (cx, getPtr(), name, v.getValuePointer());
}

Bool ObjectAPI::getProperty
  (ContextRef cx, const Char *name, Uint namelen, Value &v) const
{
  return JS_GetUCProperty
    (cx, getPtr(), name, namelen, v.getValuePointer());
}

Bool ObjectAPI::getMethod
  (ContextRef cx, const char *name, ObjectPtr &obj, Value &v) const
{
  return JS_GetMethod(cx, getPtr(), name, obj, v.getValuePointer());
}

Bool ObjectAPI::setProperty
  (ContextRef cx, const char *name, Value v) const
{
  return JS_SetProperty
    (cx, getPtr(), name, v.getValuePointer());
}

Bool ObjectAPI::setProperty
  (ContextRef cx, const Char *name, Uint namelen, Value v) const
{
  return JS_SetUCProperty
    (cx, getPtr(), name, namelen, v.getValuePointer());
}

Bool ObjectAPI::deleteProperty
  (ContextRef cx, const char *name) const
{
  return JS_DeleteProperty
    (cx, getPtr(), name);
}

/*  Bool ObjectAPI::deleteProperty(ContextRef cx, const Char *name, Uint namelen)
{
  return JS_DeleteUCProperty(cx, getPtr(), name, namelen);
}*/


Bool ObjectAPI::deleteProperty
  (ContextRef cx, const char *name, Value &rval) const
{
  return JS_DeleteProperty2
    (cx, getPtr(), name, rval.getValuePointer());
}

Bool ObjectAPI::deleteProperty
  (ContextRef cx, const Char *name, Uint namelen, Value &rval) const
{
  return JS_DeleteUCProperty2
    (cx, getPtr(), name, namelen, rval.getValuePointer());
}

Bool ObjectAPI::isFunction
  (ContextRef cx) const
{
  return JS_ObjectIsFunction
    (cx, getPtr());
}

FunctionPtr ObjectAPI::getFunction
  (ContextRef cx) const
{
  return static_cast<JSFunction *>(JS_GetPrivate(cx, getPtr()));
}

Bool ObjectAPI::defineFunctions
  (ContextRef cx, FunctionSpec fs[]) const
{
  return JS_DefineFunctions
    (cx, getPtr(), reinterpret_cast<JSFunctionSpec *>(fs));
}

FunctionPtr ObjectAPI::defineFunction
  (ContextRef cx, const char *name, Native &call, Uint nargs, Int attrs)
  const
{
  return FunctionPtr
    (JS_DefineFunction(cx, getPtr(), name, 
                       reinterpret_cast<JSNative>(&call), nargs, attrs));
}

FunctionPtr ObjectAPI::defineFunction
  (ContextRef cx, const Char *name, Uint namelen, Native &call, Uint nargs, 
   Int attrs) const
{
  return FunctionPtr
    (JS_DefineUCFunction(cx, getPtr(), name, namelen,
                         reinterpret_cast<JSNative>(&call), nargs, attrs));
}

ObjectPtr ObjectAPI::cloneFunction
  (ContextRef cx, ObjectPtr parent) const
{
  return JS_CloneFunctionObject(cx, getPtr(), parent);
}

FunctionPtr ObjectAPI::compileFunction
  (ContextRef cx, const char *name, Uint nargs, const char **argnames,
   const char *bytes, Uint length, const char *filename, Uint lineno) const
{
  return FunctionPtr
    (JS_CompileFunction(cx, getPtr(), name, nargs, 
                        argnames, bytes, length, filename, lineno));
}

FunctionPtr ObjectAPI::compileFunction
  (ContextRef cx, const char *name, Uint nargs, const char **argnames,
   const Char *chars, Uint length, const char *filename, Uint lineno) const
{
  return FunctionPtr
    (JS_CompileUCFunction(cx, getPtr(), name, nargs, 
                          argnames, chars, length, filename, lineno));
}

FunctionPtr ObjectAPI::compileFunctionForPrincipals
  (ContextRef cx, const char *name, Principals *principals,
   Uint nargs, const char **argnames, const char *bytes, 
   Uint length, const char *filename, Uint lineno) const
{
  return FunctionPtr
    (JS_CompileFunctionForPrincipals
     (cx, getPtr(), 
      reinterpret_cast<JSPrincipals *>(principals),
      name, nargs, argnames, bytes, length, filename, lineno));
}

FunctionPtr ObjectAPI::compileFunctionForPrincipals
  (ContextRef cx, const char *name, Principals *principals,
   Uint nargs, const char **argnames, const Char *chars,
   Uint length, const char *filename, Uint lineno) const
{
  return FunctionPtr
    (JS_CompileUCFunctionForPrincipals
     (cx, getPtr(), 
      reinterpret_cast<JSPrincipals *>(principals),
      name, nargs, argnames, chars, length, filename, lineno));
}

Bool ObjectAPI::isNumberObject
  (ContextRef cx) const
{
  return (getClass(cx) == Class::getNumberClass());
}

Bool ObjectAPI::isArrayObject
  (ContextRef cx) const
{
  return JS_IsArrayObject
    (cx, getPtr());
}

Bool ObjectAPI::isFileObject
  (ContextRef cx) const
{
  return !strcmp(JS_GET_CLASS(cx.getPtr(), getPtr())->name, "File");
}

Bool ObjectAPI::hasArrayLength
  (ContextRef cx, Uint &length) const
{
  return JS_HasArrayLength
    (cx, getPtr(), reinterpret_cast<jsuint *>(&length));
}

void ObjectAPI::clearScope
  (ContextRef cx) const
{
  JS_ClearScope(cx, getPtr());
}

IdArray &ObjectAPI::enumerate
  (ContextRef cx) const
{
  return *reinterpret_cast<IdArray *>
    (JS_Enumerate(cx, getPtr()));
}

ObjectPtr ObjectAPI::newPropertyIterator
  (ContextRef cx) const
{
  return ObjectPtr
    (JS_NewPropertyIterator(cx, getPtr()));
}

Bool ObjectAPI::nextProperty
  (ContextRef cx, Id &id) const
{
  return JS_NextProperty
    (cx, getPtr(), &id);
}

Bool ObjectAPI::checkAccess
  (ContextRef cx, Id id, AccessMode mode, Value &v, 
   PropertyAttrs &attrs) const
{
  uintN a;

  if(JS_CheckAccess(cx, getPtr(), id, 
                    static_cast<JSAccessMode>(mode), v.getValuePointer(), &a))
  {
    attrs = static_cast<PropertyAttrs>(a);
    return true;
  }
  return false;
}

/**
 * Gets the number of reserved slots.
 *
 * \param cx   Context from which to derive runtime information.
 * \return     Number of reserved slots for the object.
 */
Uint8 ObjectAPI::numReservedSlots
  (ContextRef cx) const
{
  ClassAPI cls = getClass(cx);
  return cls.numReservedSlots();
}
  
/**
 * Gets value from reserved slot.
 *
 * \param cx     Context from which to derive runtime information.
 * \param index  Reserved slot index.
 * \param val    Reference to value.
 *
 * \return       true if slot index was valid, false if not.
 */
Bool ObjectAPI::getReservedSlot
  (ContextRef cx, Uint8 index, Value &val) const
{
  return JS_GetReservedSlot
    (cx, getPtr(), index, val.getValuePointer());
}

/**
 * Sets the value of a reserved slot.
 *
 * \param cx     Context from which to derive runtime information.
 * \param index  Reserved slot index.
 * \param val    The value to set.
 *
 * \return       true if slot index was valid, false if not.
 */  
Bool ObjectAPI::setReservedSlot
  (ContextRef cx, Uint8 index, Value val) const
{
  return JS_SetReservedSlot(cx, getPtr(), 
                            index, val.getJSValue());
}

/**
 * Gets a pointer to the Object associated with the JSObject.
 * 
 * \param cx   Context from which to derive runtime information.
 */
Object *ObjectAPI::getInstance
  (ContextRef cx) const
{
  Value inst;

  if(!getReservedSlot(cx, numReservedSlots(cx), inst))
    return 0;

  return reinterpret_cast<Object *>(inst.getPrivate().getPtr());
}

/**
 * Requestes a pointer to the Object associated with the JSObject,
 * which means that a new Object will be instantiated and associated
 * with the JSObject if there is no previous instance.
 *
 * \param cx   Context from which to derive runtime information.
 */
Object &ObjectAPI::requestInstance
  (ContextRef cx) const
{
  Object *obj = getInstance(cx);

  if(!obj)
    obj = new Object(cx, *(ObjectRef *)this, DeleteObject);

  return *obj;
}

void ObjectAPI::ensureHasPrivate(ContextRef cx) const
{
  ClassAPI cls = getClass(cx);

  if(!cls.hasPrivate())
    throw ObjectException(this, "The class <" + (std::string)cls.getName() +
                          "> doesn't have the Private flag set.");
}

jsmm_namespace_END
