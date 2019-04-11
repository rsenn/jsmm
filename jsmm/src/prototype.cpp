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

#include <jsmm/prototype>

jsmm_namespace_BEGIN

/** 
 * Clear the property spec array.
 */
void Prototype::clearProperties()
{
  ps.clear();
}

/** 
 * Get a pointer to the property spec array.
 * 
 * \return  An array of PropertySpec structures, 
 *          terminated with a zeroed struct.
 */
PropertySpecs &Prototype::getProperties()
{
  return ps;
}

/** 
 * Add one or more properties to the prototype specification.
 * 
 * \param props  Property spec array.
 * \param n      Size of property spec array.
 * 
 * \return       true if memory allocation was successful, false if not.
 */
Bool Prototype::addProperties(PropertySpec *props, Uint n)
{
  return ps.PropertySpecArray::add(props, n);
}

/**
 * Add a new property to the prototype specification.
 * 
 * \param name    Name of the property.
 * \param tinyid  Id of the property.
 * \param flags   Flags for this property.
 * \param getter  Getter callback.
 * \param setter  Setter callback.
 * 
 * \return        true if memory allocation was successful, false if not.
 */
Bool Prototype::addProperty(const char *name, Uint8 tinyid,
                            PropertyOp &getter, PropertyOp &setter,
                            Int attrs)
{
  return ps.add(name, tinyid, getter, setter, attrs);
}

/** 
 * Clear the function spec array.
 */
void Prototype::clearFunctions()
{
  fs.clear();
}

/** 
 * Get a pointer to the function spec array.
 * 
 * \return  An array of FunctionSpec structures, 
 *          terminated with a zeroed struct.
 */
FunctionSpecs &Prototype::getFunctions()
{
  return fs;
}

/** 
 * Add functions to the prototype specification.
 * 
 * \param funcs  Function spec array.
 * \param n      Size of function spec array.
 * 
 * \return       true if memory allocation was successful, false if not.
 */
Bool Prototype::addFunctions(FunctionSpec *funcs, Uint n)
{
  return fs.FunctionSpecArray::add(funcs, n);
}

/**
 * Adds a new function spec to the prototype specification.
 * 
 * \param name   Name of the function.
 * \param call   C function that handles calls to this JS function.
 * \param nargs  Number of arguments.
 * \param flags  Flags for this function.
 * 
 * \return       true if memory allocation was successful, false if not.
 */
Bool Prototype::addFunction(const char *name, Native &call, Uint nargs,
                            Int flags)
{
  return fs.add(name, call, nargs, flags);
}

jsmm_namespace_END
