#include <jsmm/basefunction>
#include <jsmm/baseclass>
#include <jsmm/baseconstructor>
#include <jsmm/string>
#include <jsmm/script>
#include <jsmm/object>

#include <iostream>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

namespace Example
{  
  /* An example class to bind */
  class Rectangle
  {
  public:  
    Rectangle() :
      xpos(0), ypos(0), width(0), height(0)
    {}
    
    Rectangle(double x, double y, double w, double h) :
      xpos(x), ypos(y), width(w), height(h)
    {}
  
    double getX() const { return xpos; }
    double getY() const { return ypos; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    
    void set(double x, double y, double w, double h)
    {
      xpos = x;
      ypos = y;
      width = w;
      height = h;
    };
    
    void setPos(double x, double y) 
    {
      xpos = x;
      ypos = y;
    }

    void setSize(double x, double y) 
    {
      xpos = x;
      ypos = y;
    }

    void setX(double x) { xpos = x; }
    void setY(double y) { ypos = y; }
    void setWidth(double w) { width = w; }
    void setHeight(double h) { height = h; }

    double calcPlane() const { return width * height; }
 
    void calcCenter(double &x, double &y) 
    { 
      x = xpos + width / 2; 
      y = ypos + height / 2; 
    }      
    
    void scale(double factor) 
    { 
      width *= factor; 
      height *= factor; 
    }

    void move(double x, double y) 
    {
      xpos += x;
      ypos += y;
    }

    void resize(double h, double v) 
    {
      width += h;
      height += v;
    }

  private:
    double xpos;
    double ypos;
    double width;
    double height;
  };
}

namespace JS
{
  /* Define a new function class inherited from BaseFunction */
  class AlertFunction : public BaseFunction
  {
  public:
    /* Initialize the alert function by calling the BaseFunction
       constructor which registers a new function with a dispatcher
       that calls the 'native' method of our inherited class */
    AlertFunction(ContextRef cx, ObjectPtr obj = 0, const char *name = 0) :
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
      MessageBoxA(0, str.getBytes(), "JS++ Alert", 0);
#else
    std::cerr << "Alert: " << str.getBytes() << std::endl;
#endif
      return true;
    }
  };

  class RectangleClass : public BaseClass
  {
  public:
    RectangleClass(const char *name) : BaseClass(name, true)
    {}
    
    Bool addPropertyOp
      (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
    {
      std::cerr << "Add property op" << std::endl;
      return true;
    }

    Bool delPropertyOp
      (ContextRef cx, ObjectAPI obj, Value id, Value &vref) const
    {
      std::cerr << "Delete property op" << std::endl;
      return true;
    }
  };
  
  /* Setting up the prototype for the class methods/properties */
  class RectanglePrototype : public Prototype
  {
  private:
    static Bool getter
      (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
    {
      double value;
      Example::Rectangle *rp = static_cast<Example::Rectangle *>
        (obj.getPrivate(cx));
      
      if(rp)
      {
        switch(id.getInt())
        {
          case 0: value = rp->getX(); break;
          case 1: value = rp->getY(); break;
          case 2: value = rp->getWidth(); break;
          case 3: value = rp->getHeight(); break;
          case 4: value = rp->getX() + rp->getWidth(); break;
          case 5: value = rp->getY() + rp->getHeight(); break;
          
          default: value = rp->calcPlane(); break;
        }
        
        vref.createDouble(cx, value);
      }
      return true;
    }

    static Bool setter
      (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
    {
      double value = vref.toNumber(cx);
      Example::Rectangle *rp = static_cast<Example::Rectangle *>
        (obj.getPrivate(cx));

      if(rp)
      {
        switch(id.getInt())
        {
          case 0: rp->setX(value); break;
          case 1: rp->setY(value); break;
          case 2: rp->setWidth(value); break;
          case 3: rp->setHeight(value); break;
          case 4: rp->setWidth(value - rp->getX()); break;
          case 5: rp->setHeight(value - rp->getY()); break;
          
          default: return false;
        }
      }
      return true;
    }
     
    static Bool setRectangle
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));
      
        if(rect)
          rect->set(argv[0].toNumber(cx),
                    argv[1].toNumber(cx),
                    argv[2].toNumber(cx),
                    argv[3].toNumber(cx));
      }
      return true;
    }

    static Bool setRectanglePos
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        if(rect)
          rect->setPos(argv[0].toNumber(cx),
                       argv[1].toNumber(cx));
      }
      return true;
    }

    static Bool setRectangleSize
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        if(rect)
          rect->setSize(argv[0].toNumber(cx),
                        argv[1].toNumber(cx));
      }
      return true;
    }
    
    static Bool scaleRectangle
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        if(rect)
          rect->scale(argv[0].toNumber(cx));
      }
      return true;
    }
    
    static Bool moveRectangle
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        if(rect)
          rect->move(argv[0].toNumber(cx),
                     argv[1].toNumber(cx));
      }
      return true;
    }
    
    static Bool resizeRectangle
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        if(rect)
          rect->resize(argv[0].toNumber(cx),
                       argv[1].toNumber(cx));
      }
      return true;
    }
    
    static Bool toSourceRectangle
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      if(obj)
      {
        std::string s = "(new ";
        std::ostringstream output(s);

        ObjectAPI thisobj = obj;
        Example::Rectangle *rect = 
          static_cast<Example::Rectangle *>(thisobj.getPrivate(cx));

        s += ClassAPI(thisobj.getClass(cx)).getName();
        s += "(";
        
        std::cerr << "rect: " << thisobj.getPrivate(cx) << std::endl;
          
        if(rect)
        {
/*          output << rect->getX() << ", ";
          output << rect->getY() << ", ";
          output << rect->getWidth() << ", ";
          output << rect->getHeight() << "))";*/
          output.flush();
        }

        std::cerr << s << std::endl;

        rval.setString(String(cx, s));
      }
      return true;
    }
    
  public:
    RectanglePrototype()
    {
      addProperty("x", 0, getter, setter, EnumerateProp);
      addProperty("y", 1, getter, setter, EnumerateProp);
      addProperty("w", 2, getter, setter, EnumerateProp);
      addProperty("h", 3, getter, setter, EnumerateProp);

      addProperty("x1", 0, getter, setter);
      addProperty("y1", 1, getter, setter);
      addProperty("x2", 4, getter, setter);
      addProperty("y2", 5, getter, setter);

      addProperty("plane", 4, getter, setter, ReadOnlyProp);

      addFunction("set",     setRectangle,     4, BoundMethod);
      addFunction("setPos",  setRectanglePos,  2, BoundMethod);
      addFunction("setSize", setRectangleSize, 2, BoundMethod);
      addFunction("scale",   scaleRectangle,   1, BoundMethod);
      addFunction("move",    moveRectangle,    2, BoundMethod);
      addFunction("resize",  resizeRectangle,  2, BoundMethod);

      addFunction("toSource", toSourceRectangle, 0, BoundMethod);
    }
  };
  
  static Bool getRectangleHCenter
    (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
  {
    double x, y;

    Example::Rectangle *rect =
      static_cast<Example::Rectangle *>(ObjectAPI(obj.getParent(cx)).getPrivate(cx));

    if(rect)
    {
      rect->calcCenter(x, y);
      vref.createDouble(cx, x);
    }
    return true;
  }
  
  static Bool getRectangleVCenter
    (ContextRef cx, ObjectAPI obj, Value id, Value &vref)
  {
    double x, y;
    
    Example::Rectangle *rect =
      static_cast<Example::Rectangle *>(ObjectAPI(obj.getParent(cx)).getPrivate(cx));

    if(rect)
    {
      rect->calcCenter(x, y);
      vref.createDouble(cx, y);
    }
    return true;
  }

  RectangleClass RectangleClass("Rectangle");

  class RectangleConstructor : public BaseConstructor
  {
  public:
    RectangleConstructor
      (ContextRef cx, ObjectRef obj,  ObjectPtr parent_proto, 
       ClassAPI cls, Prototype &proto) :
      BaseConstructor(cx, obj, parent_proto, cls, 4, proto)
    {
      setVTable(cx);
    }
                         
    Bool native
      (ContextRef cx, ObjectPtr obj, Uint argc, Value argv[], Value &rval)
    {
      ObjectAPI robj = obj;
      Example::Rectangle *rect;
    
      if(argv[0].isObject())
      {
        ObjectAPI arg = argv[0].getObject();
        
        if(arg.getClass(cx) == RectangleClass)
          rect = new Example::Rectangle
          (*reinterpret_cast<Example::Rectangle *>(arg.getPrivate(cx)));
        else
          throw ObjectException(&obj, "Object is not a valid Rectangle.");
      }
      else
      {
        rect = new Example::Rectangle
          (argv[0].toNumber(cx), argv[1].toNumber(cx),
           argv[2].toNumber(cx), argv[3].toNumber(cx));
      }
      
      ObjectAPI(obj).setPrivate(cx, static_cast<void *>(rect));
      
      return true;
    }
  };
}

using namespace JS;

class MyException : public Exception
{
public:
  MyException() : Exception((ExceptionType)0)
  {}
  
  virtual const char *what()
  {
    return "MyException";
  }
};

int main()
{
  try
  {
    throw MyException();
    
    Context cx;
    Object &global = cx.requestGlobalObject();
    AlertFunction fn(cx);
    Script script(cx, "example3.js");
    Value rval;
    
    global.setProperty(cx, "alert", fn);
    RectanglePrototype proto;  
    
    RectangleConstructor ctor(cx, global, 0, RectangleClass, proto);
    
    PrototypeObject &pobj = ctor.getPrototype();
    
    Object &centerobj = pobj.defineObject
      (cx, "center", EnumerateProp);
    
    centerobj.defineProperty
      (cx, "x", Value(0), getRectangleHCenter,
       Object::propertyStub, EnumerateProp|ReadOnlyProp);
    
    centerobj.defineProperty
      (cx, "y", Value(0), getRectangleVCenter,
       Object::propertyStub, EnumerateProp|ReadOnlyProp);

    /* Execute the example script */
    if(!script.execute(cx, rval))
    {
      std::cerr << "failed executing script" << std::endl;
      return 1;
    }
    
    cx.garbageCollect();
    
    /* Output the return value */
    std::cout << "the script " << script.getName()
              << " returned: " << String(rval.toString(cx)).getBytes()
              << std::endl;
  }
  catch(Exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}

