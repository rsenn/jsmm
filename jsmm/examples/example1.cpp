#include <jsmm/basefunction>
#include <jsmm/string>
#include <jsmm/class>
#include <jsmm/script>
#include <jsmm/object>

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

using namespace JS;

/* Define a new function class inherited from BaseFunction */
class AlertFunction : public BaseFunction
{
public:
  /* Initialize the alert function by calling the BaseFunction
     constructor which registers a new function with a dispatcher
     that calls the 'native' method of our inherited class */
  AlertFunction(ContextRef cx, const char *name = 0, ObjectPtr obj = 0) :
    BaseFunction(cx, 1, DefaultFunction, obj, name)
  {
    setVTable(cx);
  }
  
protected:
  /* This method is called when the function gets called */
  Bool native(ContextRef cx, ObjectPtr obj,
              Uint argc, Value argv[], Value &rval)
  {
    /* Convert the argument to string */
    String str = argv[0].toString(cx);

    /* Display a message box with the alert message */
#ifdef WIN32
    MessageBox(0, str.getBytes(), "JS++ Alert", 0);
#else
    std::cerr << "Alert: " << str.getBytes() << std::endl;
#endif

    return true;
  }  
};

int main()
{
  try
  {
    Context cx;
    Value rval;

    std::cout << Class::getArgumentsClass().getName() << " reserved slots: " 
              << (void *)(Uint)Class::getArgumentsClass().numReservedSlots() << std::endl;

    Object &global = cx.requestGlobalObject();

    /* Create an instance of our alert function located in the global scope */
    AlertFunction fn(cx);
    global.setProperty(cx, "alert", fn);
    
    std::cout << "installed alert() function" << std::endl;
    
    /* Execute the example script */
    Script script(cx, "example1.js");
    
    std::cout << "loaded the script example1.js" << std::endl;
    
    if(!script.execute(cx, rval))
    {
      std::cerr << "Failed executing script" << std::endl;
      return 1;
    }
    
    /* Output the return value */
    std::cout << "The script " << script.getName()
              << " returned: " << StringAPI(rval.toString(cx)).getBytes()
              << std::endl;
  }
  catch(const std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  
  return 0;
}
