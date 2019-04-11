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
#include <jsmm/value>
#include <jsmm/object>
#include <jsmm/string>
#include <jsmm/functionapi>
#include <jsmm/api>
#include <jsmm/class>

#include <js/jsapi.h>
#include <js/jsfun.h>
#include <js/jsobj.h>
#include <js/jsutil.h>

extern "C" 
{
#include <js/jslock.h>
}

#define jsmm_value_TYPE_TO_JS(t) \
    static_cast<JSType>(mapTypeToJS[static_cast<Type>(t)])

#define jsmm_value_TYPE_FROM_JS(t) \
    static_cast<Type>(mapTypeFromJS[static_cast<JSType>(t)])

jsmm_namespace_BEGIN

Value undefined;
Value null((JSObject *)0);

Bool Value::isObject() const 
  { return JSVAL_IS_OBJECT(js_value); }

Bool Value::isNumber() const 
  { return JSVAL_IS_NUMBER(js_value); }

Bool Value::isInt() const
  { return JSVAL_IS_INT(js_value); }

Bool Value::isDouble() const 
  { return JSVAL_IS_DOUBLE(js_value); }

Bool Value::isString() const
  { return JSVAL_IS_STRING(js_value); }

Bool Value::isBool() const
  { return JSVAL_IS_BOOLEAN(js_value); }

Bool Value::isNull() const 
  { return JSVAL_IS_NULL(js_value); }

Bool Value::isUndefined() const 
  { return JSVAL_IS_VOID(js_value); }

Bool Value::isPrimitive() const
  { return JSVAL_IS_PRIMITIVE(js_value); }

Bool Value::isGCThing() const 
  { return JSVAL_IS_GCTHING(js_value); }

Bool Value::isFunction(ContextRef cx) const
{ return (!isPrimitive() && ObjectAPI(getObject()).getClass(cx) == Class::getFunctionClass()); }

Bool Value::isNumber(ContextRef cx) const
{ return (isInt() || isDouble() || (!isPrimitive() && ObjectAPI(getObject()).getClass(cx) == Class::getFunctionClass())); }
  

void       *Value::getGCThing() const { return JSVAL_TO_GCTHING(js_value); }
ObjectPtr   Value::getObject()  const { return JSVAL_TO_OBJECT(js_value); }
Double     &Value::getDouble()  const { return *JSVAL_TO_DOUBLE(js_value); }
StringPtr   Value::getString()  const { return JSVAL_TO_STRING(js_value); }
Int         Value::getInt()     const { return JSVAL_TO_INT(js_value); }
Bool        Value::getBoolean() const { return JSVAL_TO_BOOLEAN(js_value); }
Pointer     Value::getPrivate() const { if(js_value == JSVAL_VOID) return 0; return JSVAL_TO_PRIVATE(js_value); }
FunctionPtr Value::getFunction(ContextRef cx) const { return static_cast<JSFunction *>(ObjectAPI(ObjectPtr(JSVAL_TO_OBJECT(js_value))).getPrivate(cx)); }
  
// TODO: throw exceptions when JS_ValueTo*() returns false 

ObjectPtr Value::toObject(ContextRef cx) const 
{ 
  JSObject *obj = 0;
  JS_ValueToObject(cx, js_value, &obj); 
  return obj;
}

FunctionPtr Value::toFunction(ContextRef cx) const 
{ 
  return JS_ValueToFunction(cx, js_value);
}

FunctionPtr Value::toConstruct (ContextRef cx) const 
{ 
  return JS_ValueToConstructor(cx, js_value);
}

StringPtr Value::toString(ContextRef cx) const 
{ 
  return JS_ValueToString(cx, js_value);
}

Double Value::toNumber(ContextRef cx) const 
{ 
  jsdouble dbl; 
  JS_ValueToNumber(cx, js_value, &dbl); 
  return dbl;
}

//  Word Value::setJSValue   (Word      val) { return js_value = val; }
Word Value::setObject(ObjectPtr obj) 
  { return js_value = OBJECT_TO_JSVAL(obj.getPtr()); }

Word Value::setFunction(FunctionPtr fn)
  { if(fn) return setObject(FunctionAPI(fn).getObject()); else return js_value = 0; }

Word Value::setDouble(Double &d)
  { return js_value = DOUBLE_TO_JSVAL(&d); }

Word Value::setString(StringRef str) 
  { return js_value = STRING_TO_JSVAL(str.getPtr()); }

Word Value::setInt(Int i)   
  { return js_value = INT_TO_JSVAL(i); }

Word Value::setBoolean(Bool b)   
  { return js_value = BOOLEAN_TO_JSVAL(b); }

Word Value::setPrivate(Pointer prv) 
  { return js_value = (prv ? PRIVATE_TO_JSVAL(prv.getPtr()) : JSVAL_VOID); }

Word Value::setUndefined()
  { return js_value = JSVAL_VOID; }

Word Value::setNull()
  { return js_value = OBJECT_TO_JSVAL(0); }

/*
Value::operator int () const
{
  return JSVAL_TO_INT(js_value);
}*/

/** Sets a Bool value */
/*  Value::Value(Bool b)
{
  js_value = BOOLEAN_TO_JSVAL(b);
}

Value Value::operator = (Bool b)
{
  js_value = BOOLEAN_TO_JSVAL(b);
  return *this;
}

Value::operator Bool () const
{
  return JSVAL_TO_BOOLEAN(js_value);
}*/

Type Value::typeOf(ContextRef cx) const
{
  Type type = VoidType;

  if(JSVAL_IS_NUMBER(js_value))
    type = NumberType;
  else if(JSVAL_IS_STRING(js_value)) 
    type = StringType;
  else if(JSVAL_IS_BOOLEAN(js_value)) 
    type = BooleanType;
  else
    return static_cast<Type>(JS_TypeOfValue(cx, js_value));
  
  return type;
}

const char *Value::getTypeName(ContextRef cx, Type type) 
{
  return JS_GetTypeName(cx, static_cast<JSType>(type));
}

/**
 * Converts the value into the specified type.
 * 
 * \param cx    JS execution context.
 * \param type  Desired type.
 * \param vref  Reference to value which receives the result.
 * \return      true if conversion was successful.
 */
Bool Value::convert(ContextRef cx, Type type, Value &vref)
{
  return JS_ConvertValue
    (cx, js_value, static_cast<JSType>(type), &vref.js_value);
}

/**
 * Converts the value into the specified type or 'undefined' if failed.
 *
 * \param cx    JS execution context.
 * \param type  Desired type.
 * \return      New value or 'undefined'.
 */
Value Value::convert(ContextRef cx, Type type)
{
  Value rval;

  if(!convert(cx, type, rval))
    rval.setUndefined();
  
  return rval;
}

Bool Value::createDouble(ContextRef cx, Double d) 
{
  jsdouble *dbl = JS_NewDouble(cx, d);
  
  if(dbl)
  {
    js_value = DOUBLE_TO_JSVAL(dbl);
    return true;
  }
  
  return false;
}

Bool Value::createString(ContextRef cx, const char *s)
{
  return createString(cx, s, strlen(s));
}

Bool Value::createString(ContextRef cx, const std::string &s)
{
  return createString(cx, s.c_str(), s.length());
}

Bool Value::createString(ContextRef cx, const char *s, Size length)
{
  Byte *bytes = static_cast<Byte *>(::malloc(length + 1));

  if(!bytes)
    return false;

  ::memcpy(bytes, s, length);
  bytes[length] = '\0';

  JSString *str = JS_NewString(cx, static_cast<char *>(bytes), length);
  
  if(str)
  {
    js_value = STRING_TO_JSVAL(str);
    return true;
  }
  
  return false;
}

/*  Value Value::newDouble(ContextRef cx, Double d)
{
  jsdouble *dbl = JS_NewDouble(cx, d);
 
  if(dbl)
    return Value(DOUBLE_TO_JSVAL(dbl));
  
  return Value(JSVAL_VOID);
}*/

/**
 * Get NaN value.
 * 
 * \param cx   Script execution context.
 * \return     Value contaning NaN.
 */
Value Value::getNaN(ContextRef cx)
{
  return GetNaNValue(cx);
}

/**
 * Get negative infinity value.
 * 
 * \param cx   Script execution context.
 * \return     Value contaning negative infinity.
 */
Value Value::getNegativeInfinity(ContextRef cx)
{
  return GetNegativeInfinityValue(cx);
}

/**
 * Get positive infinity value.
 * 
 * \param cx   Script execution context.
 * \return     Value contaning positive infinity.
 */
Value Value::getPositiveInfinity(ContextRef cx)
{
  return GetPositiveInfinityValue(cx);
}

/**
 * Get empty string value.
 * 
 * \param cx   Script execution context.
 * \return     Value contaning empty string.
 */
Value Value::getEmptyString(ContextRef cx)
{
  return GetEmptyStringValue(cx);
}

Value Value::getUndefined()
{
  return undefined;
}

Value Value::getNull()
{
  return null;
}

/**
 * Convert a value to a number, then to an Int32, according to the ECMA rules
 * for ToInt32.
 * 
 * \param cx   Script execution context.
 * \param ip   Pointer to the integer variable that receives the result.
 * \return     true if successful, false if not.
 */
Int32 Value::toECMAInt32(ContextRef cx) const
{
  Int32 i;
  JS_ValueToECMAInt32(cx, js_value, 
                      reinterpret_cast<int32 *>(&i));
  return i;
}

/**
 * Convert a value to a number, then to an Uint32, according to the ECMA 
 * rules for ToUint32.
 * 
 * \param cx   Script execution context.
 * \param ip   Pointer to the integer variable that receives the result.
 * \return     true if successful, false if not.
 */
Uint32 Value::toECMAUint32(ContextRef cx) const
{
  Uint32 i;
  JS_ValueToECMAUint32(cx, js_value, 
                       reinterpret_cast<uint32 *>(&i));
  return i;
}

/**
 * Convert a value to a number, then to an int32 if it fits by rounding to
 * nearest; but failing with an error report if the double is out of range
 * or unordered.
 * 
 * \param cx   Script execution context.
 * \param ip   Pointer to the integer variable that receives the result.
 * \return     true if successful, false if not.
 */
Int32 Value::toInt32(ContextRef cx) const
{
  Int32 i;
  JS_ValueToInt32(cx, js_value, 
                  reinterpret_cast<int32 *>(&i));
  return i;
}

/**
 * ECMA ToUint16, for mapping a Value to a Unicode point.
 * 
 * \param cx   Script execution context.
 * \param ip   Pointer to the integer variable that receives the result.
 * \return     true if successful, false if not.
 */
Uint16 Value::toUint16(ContextRef cx) const
{
  Uint16 i;
  JS_ValueToUint16(cx, js_value, &i);
  return i;
}

/**
 * Mapping a Value to a Boolean.
 * 
 * \param cx   Script execution context.
 * \param bp   Pointer to the Boolean variable that receives the result.
 * \return     true if successful, false if not.
 */
Bool Value::toBoolean(ContextRef cx) const
{
  Bool b;
  JS_ValueToBoolean(cx, js_value, reinterpret_cast<JSBool *>(&b));
  return b;
}

jsmm_namespace_END
