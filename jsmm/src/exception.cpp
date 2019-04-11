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

#include <jsmm/exception>

using namespace std;

jsmm_namespace_BEGIN

static const char *const ExceptionTypes[] =
{
  "Unknown",
  "Reference",
  "Class",
  "Context",
  "Object",
  "Function",
  "Constructor",
  "Script"
};

/** Creates Exception and sets the type. */
Exception::Exception(ExceptionType t) /*throw()*/ :
  type(t),
  message(ExceptionTypes[type])
{
  message += " exception";
}

/** Get the Exception type name. */
const char *Exception::getTypeName() const /*throw()*/
{
  return ExceptionTypes[type];
}

jsmm_namespace_END
