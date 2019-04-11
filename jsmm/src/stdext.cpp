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

#include <jsmm/stdext>

jsmm_namespace_BEGIN

static const char HexChars[16] =
{
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

std::string operator+(std::string s, void *p)
{
  s += p;
  return s;
}

std::string operator+=(std::string &s, void *p)
{
  long pval = reinterpret_cast<long>(p);
  
  s += '0';
  
  if(p)
  {
    s += 'x';
    s += HexChars[(pval & 0xf0000000) >> 28];
    s += HexChars[(pval & 0x0f000000) >> 24];
    s += HexChars[(pval & 0x00f00000) >> 20];
    s += HexChars[(pval & 0x000f0000) >> 16];
    s += HexChars[(pval & 0x0000f000) >> 12];
    s += HexChars[(pval & 0x00000f00) >> 8];
    s += HexChars[(pval & 0x000000f0) >> 4];
    s += HexChars[(pval & 0x0000000f) >> 0];
  }

  return s;
}

jsmm_namespace_END
