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

#include <jsmm/forward>
#include <jsmm/stackframe>

#include <js/jsinterp.h>

jsmm_namespace_BEGIN

/**
 * Gets the (lazily created) call object from the stack frame.
 */
ObjectPtr StackFrame::getCallObject() const
{
  return getPtr()->callobj;
}

/**
 * Gets the (lazily created) arguments object from the stack frame.
 */
ObjectPtr StackFrame::getArgsObject() const
{
  return getPtr()->argsobj;
}

/**
 * Gets the vars object from the stack frame.
 */
ObjectPtr StackFrame::getVarObject() const
{
  return getPtr()->varobj;
}

/**
 * Gets the currently executed script from the stack frame.
 */
ScriptPtr StackFrame::getScript() const
{
  return getPtr()->script;
}

/**
 * Gets the currently called function from the stack frame.
 */
FunctionPtr StackFrame::getFunction() const
{
  return getPtr()->fun;
}

/**
 * Gets the 'this' object from the stack frame.
 */
ObjectPtr StackFrame::getThisObject() const
{
  return getPtr()->thisp;
}

/**
 * Gets the argument count from the stack frame.
 */
Uint StackFrame::getArgumentCount() const
{
  return getPtr()->argc;
}

/**
 * Gets the argument vector from the stack frame.
 */
const Value *StackFrame::getArgumentVector() const
{
  return reinterpret_cast<Value *>(getPtr()->argv);
}

/**
 * Gets the local variable count from the stack frame.
 */
Uint StackFrame::numVariables() const
{
  return getPtr()->nvars;
}

/**
 * Gets the local variables from the stack frame.
 */
const Value *StackFrame::getVariables() const
{
  return reinterpret_cast<const Value *>(getPtr()->vars);
}

/**
 * Gets the previous stack frame.
 */
StackFramePtr StackFrame::getPrevious() const
{
  return getPtr()->down;
}

jsmm_namespace_END
