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

#include <jsmm/script>

jsmm_namespace_BEGIN

/**
 * Script file constructor.
 * 
 * \param cx        Context in which we create the script.
 * \param filename  Filename of the script to compile.
 */
Script::Script(ContextRef cx, const std::string &filename) :
  ScriptAPI(ScriptAPI::compileFile(cx, filename))
{}

/**
 * Script string constructor.
 * 
 * \param cx      Context in which we create the script
 * \param script  Script buffer
 * \param name    Script name
 * \param lineno  Start counting lines from lineno (0)
 */
Script::Script(ContextRef cx, const std::string &script,
               const std::string &name, Uint lineno) :
  ScriptAPI(ScriptAPI::compileString(cx, script, name, lineno))
{}

jsmm_namespace_END
