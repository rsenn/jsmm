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

#include <jsmm/runtime>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

/**
 * Default constructor: Creates new JSRuntime object.
 *
 * \param maxbytes  Maximum number of allocated bytes
 *                  before garbage collection is run
 */
Runtime::Runtime(Uint maxbytes) : 
  RuntimeAPI(create(maxbytes))
{
  if(getPtr())
    setInstance(this);
}

/**
 * Destroy the JSRuntime only if it was created by this instance.
 */
Runtime::~Runtime()
{
  if(getInstance() == this)
    JS_DestroyRuntime(getPtr());
}

/**
 * Set the JSRuntime private pointer to a pointer to 
 * a JS::Runtime instance.
 * 
 * \param rt  Pointer to a JS::Runtime instance or 0.
 */
void Runtime::setInstance(Runtime *rt)
{
  JS_SetRuntimePrivate(getPtr(), rt);
}

jsmm_namespace_END
