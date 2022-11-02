// PIMPL Idiom (pointer to implementation (PIMPL))
// ------------------------------------------------------
// The PIMPL idiom is a programming technique that hides the implementation
// details of a class from its interface. It is often used to hide internal
// implementation details or data from the user of a class by only exposing a
// pointer to an internal class which is defined in a source file inaccessible
// to users.
// 
// cereal can be used in classes with pointers to private implementations
// (PIMPL Idiom).
// 
// Requirements:
// -------------
// - serialization function definitions are deferred until after the PIMPL
//   class is declared
// - serialization functions are explicitly instantiated for all potential
//   archive types they will be used with 
// ------------------------------------------------------


// Writing Serialization Functions with the PIMPL Idiom
// ------------------------------------------------------
// "MyClass.hpp"
// -----------------------------
#include <iostream>
#include <cereal/access.hpp>
#include <Rcpp.h>
// [[Rcpp::depends(Rcereal)]]


class MyClass
{
public:
  MyClass();
  ~MyClass();
  
private:
  int x;
  
  class MyClassDetail; // forward declaration of PIMPL class
  std::unique_ptr<MyClassDetail> impl; // pointer to implementation
  
  friend class cereal::access;
  // The implementation for this must be deferred until
  // after the declaration of the PIMPL class
  template<class Archive>
  void serialize(Archive& ar);
  
};
// ----------------------------------------------------------------------------


// "MyClass.cpp"
// -----------------------------
#include "MyClass.hpp"
#include <cereal/types/memory.hpp>
// include all archives that this type will be serialized with for explicit
// instantiation
#include <cereal/archives/json.hpp>

// Definition for PIMPL class
class MyClass::MyClass
{
public:
  MyClassDetail(): secretData(3.14){}
  
  double secretData;
  
private:
  friend class cereal::access;
  
  template<class Archive>
  void load(Archive& ar)
  { ar(CEREAL_NVP(secretData) ); }
  
  template<class Archive>
  void save(Archive& ar) const
  { ar(CEREAL_NVP(secretData) ); }
  
};

MyClass::~MyClass() = default;

// We must also explicitly instantiate our template fcts for serialization
template void MyClass::MyClassDetail::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
template void MyClass::MyClassDetail::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);

// Note that we need to instantiate for both loading and saving
template void MyClass::serialize<cereal::JSONInputArchive>(cereal::JSONOutputArchive&);
template void MyClass::serialize<cereal::JSONOutputArchive&);

// ----------------------------------------------------------------------------


// main.cpp
// -----------------------------
#include "myclass.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <sstream>
#include <fstream>
#include <iostream>

int main()
{
  MyClass m;
  std::stringstream ss;
  
  {
    cereal::JSONOutputArchive archive(ss);
    archive(m);
  }
  std::cout << ss.str() << std::endl;
  
  {
    cereal::JSONInputArchive archive(ss);
    archive(m);
  }
  
  {
    std::ofstream os("out.cereal", std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    
    // The following will give us a linker error because we did not explicitly
    // instantiate our serialization functions for binary archives.
    // archives(m);
  }
  
  return 0;
  
}







