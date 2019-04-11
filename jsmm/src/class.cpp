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

#include <jsmm/class>
#include <jsmm/object>
#include <jsmm/classprivate>

#include <js/jsapi.h>

#include <stdlib.h>
#include <string.h>

/* Flag that is set when the JSClass was allocateStructd by js++ */
#define jsmm_class_IS_ALLOCATED (1 << (JSCLASS_HIGH_FLAGS_SHIFT + 15))

#define jsmm_class_NAME_ALLOC   (1 << (JSCLASS_HIGH_FLAGS_SHIFT + 14))


/* Pointer conversion macro */
#define jsmm_class_PTR(val) reinterpret_cast<JSClass *> \
          (JSVAL_TO_PRIVATE(reinterpret_cast<jsval>(val)))

jsmm_namespace_BEGIN

/**
 * This is the class we derive all objects from by default,
 * because they all need JSCLASS_HAS_PRIVATE, so we can set
 * pointers to their JS::Object.
 */
//  static Class FallbackClass("Object");

static Class *DefaultClass;

Class::Class(ClassRef ref) : 
  ClassAPI(ref)
{
  setInstance(this);
}

/**
 * Construct Class object, allocates and 
 * initializes internal JSClass structure.
 * 
 * \param name  Name of the class
 */
Class::Class(const char *name, Bool hasPrivate, Bool isExtended) :
  ClassAPI(create(name, hasPrivate, isExtended))
{
  setInstance(this);
}

Class::Class(const char *name, void *vtable, 
             Bool hasPrivate, Bool isExtended) :
  ClassAPI(create(name, hasPrivate, isExtended))
{
  setInstance(this);
  setObjectVTable(vtable);
}

/**
 * Free the internal class structure 
 */
Class::~Class()
{
  if(getInstance() == this)
  {
    freeName();
    freeStruct();
  }
}

void Class::setClassVTable(void *vtable) const
{
  if(!(getPtr()->flags & jsmm_class_NAME_ALLOC))
    setName(getPtr()->name);

  AllocClassPrivate *clsprv = 
    reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);
  
  clsprv->getPrivate().vclass = vtable;
}

void Class::setObjectVTable(void *vtable) const
{
  if(!(getPtr()->flags & jsmm_class_NAME_ALLOC))
    setName(getPtr()->name);
  
  AllocClassPrivate *clsprv = 
    reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);
  
  clsprv->getPrivate().vobject = vtable;
}

void Class::setInstance(Class *inst) const
{
  if(!(getPtr()->flags & jsmm_class_NAME_ALLOC))
    setName(getPtr()->name);

  AllocClassPrivate *clsprv = 
    reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);
  
  clsprv->getPrivate().inst = inst;
}

void Class::setName(const char *name) const
{
  if((getPtr()->flags & jsmm_class_NAME_ALLOC) == 0)
  {
    AllocClassPrivate clsprv(strlen(name) + 1);

    strcpy(clsprv.getBase(), name);
    clsprv.getPrivate().vclass = getClassVTable();
    clsprv.getPrivate().vobject = getObjectVTable();
    clsprv.getPrivate().inst = getInstance();

    getPtr()->name = clsprv.moveBase();
    getPtr()->flags |= jsmm_class_NAME_ALLOC;
  }
  else
  {
    AllocClassPrivate *clsprv = 
      reinterpret_cast<AllocClassPrivate *>(&getPtr()->name);
    
    clsprv->allocate(strlen(name) + 1);
    strcpy(clsprv->getBase(), name);
  }
}

void Class::freeName() const
{
  /* If the name was allocated, then free it now. */
  if(jsPtr && (getPtr()->flags & jsmm_class_NAME_ALLOC))
  {
    AllocClassPrivate clsprv(const_cast<char *>(getPtr()->name));
  }

  getPtr()->name = 0;
}

/**
 * Destroys JSClass structure
 */
void Class::freeStruct()
{
  if(getPtr() && (getPtr()->flags & jsmm_class_IS_ALLOCATED))
  {
    delete getPtr();
    jsPtr = 0;
  }
}

/**
 * Initializes built-in class.
 */
void Class::initBuiltin(Uint8 nslots, void *vtable) const
{
  /* Increase reserved slots number by one */
  getPtr()->flags =
    (getPtr()->flags &
     (~(JSCLASS_RESERVED_SLOTS_MASK <<
        JSCLASS_RESERVED_SLOTS_SHIFT))) |
    JSCLASS_HAS_RESERVED_SLOTS(JSCLASS_RESERVED_SLOTS(getPtr()) + nslots);
  
  if(vtable)
    setObjectVTable(vtable);
}

/**
 * Attaches this object to the JSClass
 * structure by setting the pointer past
 * the structure.
 */
/*  void Class::setInstance(Class *cls)
{
  Class **ref = reinterpret_cast<Class **>(&getExtendedPtr()[1]);

  *ref = cls;
}*/

/**
 * Sets new class name
 */
//  void Class::setName(const char *name)
//  {
  /* Free previously allocated class name */
//    if(getPtr()->name)
//      free(const_cast<char *>(getPtr()->name));
  
  /* Set new class name */
//    getPtr()->name = strdup(name);
//  }

/**
 * Returns the default class
 */
Class &Class::getDefaultClass()
{
  return *DefaultClass;
}

void Class::setDefaultClass(Class &cls)
{
  DefaultClass = &cls;
}

jsmm_namespace_END
