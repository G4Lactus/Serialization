// Polymorphism
// ----------------------------------
// cereal supports serializing smart pointers to polymorphic base classes and
// will automatically deduce the derived types at runtime.
// 
// If you want to serialize data through pointers:
// 1. Include <cereal/types/polymorphic.hpp>
// 2. Include all of the archives you want to be able to use with your class
// 3. Read the documentation below to understand your decision for step 4
// 4. Use the CEREAL_REGISTER_TYPE(Your_Class_Name) macro in either the header
//    file in which the type is declared or the source file in which it is
//    defined.
// 5. You may also need to use 
//    CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, DerivedClass)
//

// It is not necessary to register base classes, but cereal must have a
// serialization path from derived to base type.

// ---------------------------------------------------
// Registering from a header file
// -----------------------------------
#include <memory>
#include <cereal/types/polymorphic.hpp>
// [[Rcpp::depends(Rcereal)]]
#include <Rcpp.h>


// A pure virtual base class
struct BaseClass
{
  virtual void sayType() = 0;
};

// A class derived from BaseClass
struct DerivedClassOne: public BaseClass
{
  void sayType();
  int x;
  
  template<class Archive>
  void serialize(Archive& ar) { ar(x); }

};


// Another class derived from BaseClass
struct EmbarrassingDerivedClass: public BaseClass
{
  void sayType();
  float y;
  
  template<class Archive>
  void serialize(Archive& ar) { ar(y); }
  
};


// Include any archive you plan on using with your type before your it.
// Note that this could be done in any other location so long as it was prior to
// this file being included.
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

// register DerivedClassOne
CEREAL_REGISTER_TYPE(DerivedClassOne);

// register EmbarassingDerivedClass with a less embarrasing name
CEREAL_REGISTER_TYPE_WITH_NAME(EmbarrassingDerivedClass, "DerivedClassTwo");

// Note that there is no need to register the base class, only derived classes.
// However, since we did not use cereal::base_class, we need to clarify
// the relationship
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, DerivedClassOne);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, EmbarrassingDerivedClass);
// ---------------------------------------------------


// ---------------------------------------------------
// myclasses.cpp (extra file)
#include <iostream>
#include "myclasses.hpp"

void DerivedClassOne::sayType()
{
  std::cout << "DerivedClassOne" << std::endl;
}

void EmbarrassingDerivedClass::sayType()
{
  std::cout << "EmbarrassingDerivedClass. Wait.. Imean DerivedClass Two!" << std::endl;
}
// ---------------------------------------------------



// ---------------------------------------------------
// main.cpp
// any archives include prior to "myclasses.hpp"
#include "myclasses.hpp"
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <iostream>
#include <fstream>

int main()
{
  {
    std::ofstream os("Backend/polymorphism_test.xml")
    cereal::XMLOutputArchive oarchive(os);
    
    // Create instances of the derived classes, but only keep base class 
    // pointers:
    std::shared_ptr<BaseClass> ptr1 = std::make_shared<DerivedClassOne>();
    std::shared_ptr<BaseClass> ptr2 = std::make_shared<EmbarrassingDerivedClass>();
    oarchive(ptr1, ptr2);
  }
  
  {
    std::ifstream is("polymorphism_test.xml")
    cereal::XMLInputArchive iarchive(is);
    
    // De-serialized the data as base class pointers, and watch as they are
    // re-instantiated as derived classes
    std::shared_ptr<BaseClass> ptr1;
    std::shared_ptr<BaseClass> ptr2;
    iarchive(ptr1, ptr2);
    
    // output
    ptr1->sayType();
    ptr2->sayType();
  }
  
  return 0;
}



// ---------------------------------------------------
