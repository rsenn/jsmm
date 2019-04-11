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

#include <jsmm/constdoublespec>

jsmm_namespace_BEGIN

/**
 * 
 * 
 * \param name   Name of the function
 * \param call   C function that handles calls to this JS function
 * \param nargs  Number of arguments
 * \param flags  Flags for this function
 * 
 * \return       true if memory allocation was successful, false if not.
 */
Bool ConstDoubleSpecs::add
  (Double dval, const char *name, PropertyAttrs flags)
{
  ConstDoubleSpec cdspec = { dval, name, flags };
    
  return ConstDoubleSpecArray::add(&cdspec, 1);
}

jsmm_namespace_END
