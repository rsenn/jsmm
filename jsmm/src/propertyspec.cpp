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

#include <jsmm/propertyspec>

jsmm_namespace_BEGIN

/**
 * Adds a new property spec to the array.
 * 
 * \param name    Name of the property.
 * \param tinyid  Id of the property.
 * \param flags   Flags for this property.
 * \param getter  Getter callback.
 * \param setter  Setter callback.
 * 
 * \return        true if memory allocation was successful, false if not.
 */
Bool PropertySpecs::add(const char *name, Int8 tinyid, 
                        PropertyOp &getter, PropertyOp &setter, Int flags)
{
  PropertySpec prspec = { name, tinyid, flags, &getter, &setter };
    
  return PropertySpecArray::add(&prspec, 1);
}

jsmm_namespace_END
