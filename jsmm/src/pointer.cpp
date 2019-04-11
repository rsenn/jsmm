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

#include <jsmm/pointer>

using namespace std;

jsmm_namespace_BEGIN

static const char *const PointerTypeNames[] =
{
  "void",
  "JSRuntime",
  "JSContext",
  "JSObject",
  "JSFunction",
  "JSScript",
  "JSClass"
};

std::string operator+(const char *s, Pointer &ptr)
{
  std::string str(s);

  str += (std::string)ptr;

  return str;
}
/** Get pointer type name for a specific PointerType. */
const char *Pointer::getTypeName(PointerType typeId)
{
  return PointerTypeNames[typeId];
}

/** Conversion of Pointers into strings. */
std::string Pointer::toString(PointerType typeId, bool ref) const
{
  std::string str = (ref ? "Reference" : "Pointer");
  
  if(typeId != voidPtr)
  {
    str = "Typed" + str;

    str += "<";
    str += getTypeName(typeId);
    str += ", ";
    str += getTypeName(typeId);
    str += "Ptr>(";
  }
  
  str += jsPtr;
  str += ")";

  return str; 
}

/** Allows concatenation of a string to a pointer. */
/*  std::string Pointer::operator+(const char *s) const

{
  std::string str;
  
  str += toString();
  str += s; 
  
  return s;
}*/

jsmm_namespace_END
