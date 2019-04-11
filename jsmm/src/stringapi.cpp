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

#include <cstdlib>
#include <cstring>

#include <jsmm/stringapi>

#include <js/jsapi.h>

jsmm_namespace_BEGIN

/** 
 * Get byte buffer containing string (non-unicode).
 * 
 * \return  Pointer to a null-terminated char buffer.
 */
Byte *StringAPI::getBytes() const
{
  return JS_GetStringBytes(getPtr());
}

/**
 * Get unicode buffer containing string.
 * 
 * \return  Pointer to a null-terminated widechar buffer.
 */
Char *StringAPI::getChars() const
{
  return JS_GetStringChars(getPtr());
}

/**
 * Get the length of the string.
 * 
 * \return  Unsigned integer containing the length of the buffer.
 */
Uint StringAPI::getLength() const
{
  return JS_GetStringLength(getPtr());
}

StringPtr StringAPI::create
  (ContextRef cx, const Byte *bytes, Uint length)
{
  Byte *str = static_cast<Byte *>(::malloc(length + 1));
  strcpy(str, bytes);
  return JS_NewString(cx, str, length);
}

StringPtr StringAPI::create
  (ContextRef cx, const std::string &str)
{
  return create(cx, str.c_str(), str.length());
}

Int StringAPI::compare
  (StringRef str1, StringRef str2)
{
  return JS_CompareStrings(str1, str2);
}

jsmm_namespace_END
