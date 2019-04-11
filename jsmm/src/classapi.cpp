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

#include <jsmm/classapi>
#include <jsmm/classexception>
#include <jsmm/classprivate>
#include <jsmm/object>

#include <js/jsapi.h>

#include <string.h>

/* Flag that is set when the JSClass was allocateStructd by js++ */
#define jsmm_class_IS_ALLOCATED (1 << (JSCLASS_HIGH_FLAGS_SHIFT + 15))

#define jsmm_class_NAME_ALLOC   (1 << (JSCLASS_HIGH_FLAGS_SHIFT + 14))

/* Flag that is set when the JSClass contains a vtable pointer at name[-4] */
//#define jsmm_class_HAS_VTABLE   (1 << (JSCLASS_HIGH_FLAGS_SHIFT + 13))

/* Pointer conversion macro */
#define jsmm_class_PTR(val) reinterpret_cast<JSClass *> \
          (JSVAL_TO_PRIVATE(reinterpret_cast<jsval>(val)))

jsmm_namespace_BEGIN
/*  Class *ClassAPI::getInstance() const
  {
    if(!(getPtr()->flags & jsmm_class_IS_ALLOCATED))
      return 0;
    
    return *reinterpret_cast<Class **>(&getExtendedPtr()[1]);
  }*/
 
  Class &ClassAPI::requestInstance() const
  {
    Class *inst = getInstance();
    
    if(!inst)
      throw ClassException(this, "The class <" + (std::string)getName() +
                           "> has no instance!");
    
    return *inst;
  }
 
  /**
   * Allocate and initialize a JSClass structure.
   */
  ClassPtr ClassAPI::create
    (const char *name, Bool hasPrivate, Bool isExtended)
  {
    /* Allocate space for JSClass structure and an 
       additional pointer back to the JS::Class object */
    JSClass *cls = reinterpret_cast<JSClass *>(new JSExtendedClass);
  
    if(cls == 0)
      throw ClassException(0, "Could not allocate Class structure.");
  
    cls->name = name;
    cls->flags =
      (hasPrivate ? JSCLASS_HAS_PRIVATE : 0) |
      (isExtended ? JSCLASS_IS_EXTENDED : 0) |
      jsmm_class_IS_ALLOCATED | JSCLASS_HAS_RESERVED_SLOTS(3);
    
    /* Mandatory non-null function pointer members. */
    cls->addProperty =
    cls->delProperty =
    cls->getProperty =
    cls->setProperty =
      reinterpret_cast<JSPropertyOp>(&Object::propertyStub);
  
    cls->enumerate =
      reinterpret_cast<JSEnumerateOp>(&Object::enumerateStub);
  
    cls->resolve =
      reinterpret_cast<JSResolveOp>(&Object::resolveStub);
  
    cls->convert =
      reinterpret_cast<JSConvertOp>(&Object::convertStub);
  
    cls->finalize =
      reinterpret_cast<JSFinalizeOp>(&Object::finalizeStub);
        
    /* Optionally non-null members start here. */
    cls->getObjectOps = 0;
    cls->checkAccess = 0;
    cls->call = 0;
    cls->construct = 0;
    cls->xdrObject = 0;
    cls->hasInstance = 0;
    cls->mark = 0;
    cls->reserveSlots = 0;
      
    /* Initialize the extended class */
    JSExtendedClass *excls = reinterpret_cast<JSExtendedClass *>(cls);
  
    excls->equality = 0;
    excls->outerObject = 0;
    excls->innerObject = 0;
    
    return cls;
  }
  
  ObjectPtr ClassAPI::init
    (ContextRef cx, ObjectRef obj, ObjectPtr parent_proto, 
     Native *constructor, Uint nargs, PropertySpec ps[], FunctionSpec fs[], 
     PropertySpec static_ps[], FunctionSpec static_fs[]) const
  {
    return JS_InitClass(cx, obj, parent_proto, getPtr(), 
                        reinterpret_cast<JSNative>(constructor), nargs,
                        reinterpret_cast<JSPropertySpec *>(ps),
                        reinterpret_cast<JSFunctionSpec *>(fs), 
                        reinterpret_cast<JSPropertySpec *>(static_ps),
                        reinterpret_cast<JSFunctionSpec *>(static_fs));
  }

  Bool ClassAPI::hasPrivate() const
  {
    return !!(getPtr()->flags & JSCLASS_HAS_PRIVATE);
  }

  Bool ClassAPI::isExtended() const
  {
    return !!(getPtr()->flags & JSCLASS_IS_EXTENDED);
  }

  void ClassAPI::ensureIsExtended() const
  {
    if(!isExtended())
      throw ClassException(this, "The class <" + (std::string)getName() + 
                           "> is not an extended class.");
  }
  
  /**
   * Returns current class name
   */
  const char *ClassAPI::getName() const
  {
    return getPtr()->name;
  }
  
  Class *ClassAPI::getInstance() const
  {
    if(getPtr()->flags & jsmm_class_NAME_ALLOC)
    {
      AllocClassPrivate *clsprv = 
        reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);

      return clsprv->getPrivate().inst;
    }
    
    return 0;
  }
  
  void *ClassAPI::getClassVTable() const
  {
    if(getPtr()->flags & jsmm_class_NAME_ALLOC)
    {
      AllocClassPrivate *clsprv = 
        reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);

      return clsprv->getPrivate().vclass;
    }

    return 0;
  }
  
  void *ClassAPI::getObjectVTable() const
  {
    if(getPtr()->flags & jsmm_class_NAME_ALLOC)
    {
      AllocClassPrivate *clsprv = 
        reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);

      return clsprv->getPrivate().vobject;
    }

    return 0;
  }

/*  void ClassAPI::freeName() const
  {
    if(jsPtr && (getPtr()->flags & jsmm_class_NAME_ALLOC))
      delete &getPtr()->name[0 - sizeof(void *) * 3];
    
    getPtr()->name = 0;
  }*/
  
  /**
   * Gets the number of reserved slots in objects of this class.
   */
  uint8_t ClassAPI::numReservedSlots() const
  {
    return JSCLASS_RESERVED_SLOTS(getPtr()) - 3;
  }

/*  void *ClassAPI::getVTable() const
  {
    if(getPtr()->flags & jsmm_class_HAS_VTABLE)
    {
      void *const *vtabptr = reinterpret_cast<void *const *>(getPtr()->name);
      return vtabptr[-1];
    }
  
    return 0;
  }*/

  /**
   * Initializes built-in class.
   */
/*  void ClassAPI::initBuiltin(Uint8 nslots, void *vtable)
  {
    getPtr()->flags = 
      (getPtr()->flags & 
       (~(JSCLASS_RESERVED_SLOTS_MASK << 
          JSCLASS_RESERVED_SLOTS_SHIFT))) |
      JSCLASS_HAS_RESERVED_SLOTS(JSCLASS_RESERVED_SLOTS(getPtr()) + nslots);
    
    if(vtable)
      setObjectVTable(vtable);
  }*/
  
  /** 
   * Get the pointer to the internal JSExtendedClass structure.
   * 
   * \return  Pointer to JSExtendedClass structure, 
   *          or 0 if the class is an not extended one.
   */
  JSExtendedClass *ClassAPI::getExtendedPtr() const
  {
    JSClass *jscls = jsmm_class_PTR(getPtr());
    
//    if(jscls->flags & JSCLASS_IS_EXTENDED)
      return reinterpret_cast<JSExtendedClass *>(jscls);
    
//    return 0;
  }
  
  /**
   * Sets the number of reserved slots in objects of this class.
   */
  void ClassAPI::setReservedSlots
    (Uint8 nslots) const
  {
    /* Clear reserved slots number */
    getPtr()->flags &=
      ~(JSCLASS_RESERVED_SLOTS_MASK << JSCLASS_RESERVED_SLOTS_SHIFT);
    
    /* Set reserved slots number */
    getPtr()->flags |= JSCLASS_HAS_RESERVED_SLOTS(nslots + 3);
  }

  /**
   * Sets the callback that gets called when 
   * adding a property on objects of this class.
   * 
   * \param op  The property operation callback.
   */
  void ClassAPI::setAddPropertyOp
    (PropertyOp &op) const
  {
    getPtr()->addProperty = reinterpret_cast<JSPropertyOp>(&op);
  }

  /**
   * Sets the callback that gets called when 
   * deleting a property on objects of this class.
   * 
   * \param op  The property operation callback.
   */
  void ClassAPI::setDelPropertyOp
    (PropertyOp &op) const
  {
    getPtr()->delProperty = reinterpret_cast<JSPropertyOp>(&op);
  }

  /**
   * Sets the callback that gets called when 
   * getting a property on objects of this class.
   * 
   * \param op  The property operation callback.
   */
  void ClassAPI::setGetPropertyOp
    (PropertyOp &op) const
  {
    getPtr()->getProperty = reinterpret_cast<JSPropertyOp>(&op);
  }
    
  /**
   * Sets the callback that gets called when 
   * setting a property on objects of this class.
   * 
   * \param op  The property operation callback.
   */
  void ClassAPI::setSetPropertyOp
    (PropertyOp &op) const
  {
    getPtr()->setProperty = reinterpret_cast<JSPropertyOp>(&op);
  }
  
  /**
   * Sets the callback that gets called when 
   * enumerating objects of this class.
   * 
   * \param op  The enumeration callback.
   */
  void ClassAPI::setEnumerateOp
    (EnumerateOp &op) const
  {
    getPtr()->enumerate = reinterpret_cast<JSEnumerateOp>(&op);
  }
  
  /**
   * Sets the callback that gets called when 
   * resolving objects of this class.
   * 
   * \param op  The resolve callback.
   */
  void ClassAPI::setResolveOp
    (ResolveOp &op) const
  {
    getPtr()->resolve = reinterpret_cast<JSResolveOp>(&op);
  }
  
  /**
   * Sets the callback that gets called when 
   * converting objects of this class.
   * 
   * \param op  The convert callback.
   */
  void ClassAPI::setConvertOp
    (ConvertOp &op) const
  {
    getPtr()->convert = reinterpret_cast<JSConvertOp>(&op);
  }
  
  /**
   * Sets the callback that gets called when 
   * finalizing objects of this class.
   * 
   * \param op  The finalize callback.
   */
  void ClassAPI::setFinalizeOp
    (FinalizeOp &op) const
  {
    getPtr()->finalize = reinterpret_cast<JSFinalizeOp>(&op);
  }

  void ClassAPI::setGetObjectOps
    (GetObjectOps *op) const
  {
    getPtr()->getObjectOps = reinterpret_cast<JSGetObjectOps>(op);
  }
  
  void ClassAPI::setCheckAccessOp
    (CheckAccessOp *op) const
  {
    getPtr()->checkAccess = reinterpret_cast<JSCheckAccessOp>(op);
  }
  
  void ClassAPI::setCallOp
    (Native *op) const
  {
    getPtr()->call = reinterpret_cast<JSNative>(op);
  }
  
  void ClassAPI::setConstructOp(Native *op) const
  {
    getPtr()->construct = reinterpret_cast<JSNative>(op);
  }
  
  void ClassAPI::setXDRObjectOp
    (XDRObjectOp *op) const
  {
    getPtr()->xdrObject = reinterpret_cast<JSXDRObjectOp>(op);
  }
  
  void ClassAPI::setHasInstanceOp
    (HasInstanceOp *op) const
  {
    getPtr()->hasInstance = reinterpret_cast<JSHasInstanceOp>(op);
  }
  
  void ClassAPI::setMarkOp
    (MarkOp *op) const
  {
    getPtr()->mark = reinterpret_cast<JSMarkOp>(op);
  }
  
  void ClassAPI::setReserveSlotsOp
    (ReserveSlotsOp *op) const
  {
    getPtr()->reserveSlots = reinterpret_cast<JSReserveSlotsOp>(op);
  }
      
  void ClassAPI::setEqualityOp
    (EqualityOp *op) const
  {
    ensureIsExtended();

    getExtendedPtr()->equality = reinterpret_cast<JSEqualityOp>(op);
  }
    
  void ClassAPI::setOuterObjectOp
    (ObjectOp *op) const
  {
    ensureIsExtended();
    
    getExtendedPtr()->outerObject = reinterpret_cast<JSObjectOp>(op);
  }
  
  void ClassAPI::setInnerObjectOp
    (ObjectOp *op) const
  {
    ensureIsExtended();
    
    getExtendedPtr()->innerObject = reinterpret_cast<JSObjectOp>(op);
  }  
}
