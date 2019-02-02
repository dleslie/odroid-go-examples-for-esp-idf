#include <memory>
#include <type_traits>

namespace Examples {
  class Example {
  public:
    virtual const char *name();
    virtual void setup();
    virtual void loop();
    virtual void takedown();
    virtual ~Example();

    std::shared_ptr<Example> next;
    std::shared_ptr<Example> prev;
  };

  void register_example(std::shared_ptr<Example> &example);
  void next_example();
  
  template<typename T>
    std::shared_ptr<Example> make_example() {
    static_assert(std::is_base_of<Examples::Example, T>::value,
		  "Expected T to derive Examples::Example.");
  
    std::shared_ptr<Examples::Example> new_example = std::make_shared<T>();
    register_example(new_example);
    return new_example;
  }
  
  extern std::shared_ptr<Example> example_list;
  extern std::shared_ptr<Example> current;

  class HelloWorld : public Example { 
  public:			       
    virtual const char * name();	      
    virtual void setup();	      
  };

  class Buttons : public Example { 
  public:			       
    virtual const char * name();	      
    virtual void setup();	      
    virtual void loop();	      
  };

  class BreathingLED : public Example { 
  public:			       
    virtual const char * name();	      
    virtual void setup();	      
    virtual void loop();	      
    virtual void takedown();
  private:
    bool ascending;
    uint16_t pinVal;
  };

  class Battery : public Example { 
  public:			       
    virtual const char * name();	      
    virtual void setup();	      
    virtual void loop();	      
  };

  class SpeakerImpl;
  class Speaker : public Example { 
  public:			       
    virtual const char * name();	      
    virtual void setup();	      
    virtual void loop();	      
    virtual void takedown();
  private:
    std::shared_ptr<SpeakerImpl> impl;
  };

  class BluetoothImpl;
  class Bluetooth : public Example { 
  public:		       
    virtual const char * name();	      
    virtual void setup();	      
    virtual void loop();	      
    virtual void takedown();
  private:
    std::shared_ptr<BluetoothImpl> impl;
  };
}
