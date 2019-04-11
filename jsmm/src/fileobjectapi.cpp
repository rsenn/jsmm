#include <jsmm/fileobjectapi>
#include <jsmm/objectexception>
#include <jsmm/api>

#include <js/jsapi.h>

extern "C"
{
#include <js/jsfile.h>
}

extern "C" JSObject *
  js_NewFileObjectFromFILE(JSContext *cx, FILE *nativehandle, char *filename,
                           int32 mode, JSBool open, JSBool randomAccess);    

jsmm_namespace_BEGIN

FileObjectAPI::FileObjectAPI(ContextRef cx, ObjectAPI obj) :
  ObjectAPI(obj)
{
  if(!obj.isFileObject(cx))
    throw ObjectException(this, "Not a file object.");
}

ObjectPtr FileObjectAPI::create
  (ContextRef cx, const char *filename)
{
  return js_NewFileObject(cx, const_cast<char *>(filename));
}

ObjectPtr FileObjectAPI::create
  (ContextRef cx, FILE *native, const char *filename, 
   FileObjectModes mode, Bool open, Bool randomAccess)
{
  return js_NewFileObjectFromFILE
    (cx, native, const_cast<char *>(filename), mode, open, randomAccess);
}

File &FileObjectAPI::getPrivate
  (ContextRef cx) const
{
  return *static_cast<File *>(ObjectAPI::getPrivate(cx));
}

jsmm_namespace_END
