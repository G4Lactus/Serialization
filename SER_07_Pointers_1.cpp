// Pointers
// ----------------------------------------------------------------------------
// cereal supports serializing smart pointers but not raw pointers.
// Pointer support: <cereal/types/memory.hpp>
// Pointers to polymorphic types are also supported.
// 
// Cereal needs access to either a default constructor or a specialization of
// cereal::LoadAndConstruct for loading smart pointers.
//
// All smart pointers are supported, i.e.:
//  std::unique_ptr, std::shared_ptr, std::weak_ptr
// cereal takes care that the data are only serialized once.
// 
// For polymorphic objs cereal will also properly handle pointers to the 
// polymorphic objects (e.g., std::unique_ptr<Base> p(new Derived() ).
// ----------------------------------------------------------------------------


// Types with no default constructor
// ----------------------------------
// If you want to serialize a smart pointer to a type that does not have a
// default constructor, or a type that does not allow cereal access to its
// default constructor, you will either need to provide either:
// - a member static function load_and_construct 
// or provide a specialization of
// - cereal::LoadAndConstruct
// for the type.
// 
// If you choose to use the external version, specializing 
// cereal::LoadAndConstruct, cereal will call the static method of this struct,
// load_and_construct.
// --------------------------------------
#include <iostream>
#include <fstream>
#include <memory>
// [[Rcpp::depends(Rcereal)]]
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <Rcpp.h>


// ----------------------------------------------------------------------------
struct MyType1
{

  MyType1() = delete;       // remove no-args ctor
  MyType1(int x): myX{x}{}; // class has no default ctor
  int myX;
  
  // define a serialize or save/load pair
  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(
      CEREAL_NVP(myX)
    );
  }
  
  // NOTE: at this point we could define load_and_construct internally
  // NOTE: version parameter is optional
  template<class Archive>
  static void load_and_construct(Archive& ar, 
                                 cereal::construct<MyType1>& construct)
                                 // std::uint32_t const version)
  {
    int x;
    ar(x);
    construct(x);
  }
};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Alternatively, we define an external specialization for LoadAndConstruct
struct MyType2
{

  MyType2() = delete; // remove no-args ctor
  MyType2(int x): myX{x}{}; // class has no default ctor
  int myX;

  // define a serialization or save/load pair
  template<class Archive>
  void serialize(Archive& ar)
  {
    ar(myX);
  }
};


namespace cereal
{
  template <> struct LoadAndConstruct<MyType2>
  {
    // load_and_construct will be passed the archive that you will be loading from
    // as well as a special construct object that can be used to involve the
    // constructor for your class.

    // More advanced functionality is available using construct, such as accessing
    // class members, which is detailed in the doxygen docs.
    template<class Archive>
    static void load_and_construct(Archive& ar, 
                                   cereal::construct<MyType2>& construct)
    {
      int x;
      ar(x);
      construct(x);
    }
  };
}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// demo
void simulate_types_without_default_ctor() {
  
  // MyType1
  // -------------------------------------------
  { // serialize
    Rcpp::Rcout << "Serialize begin!" << std::endl;
    std::ofstream os1("Backend/mytype1.bin", std::ios::binary);
    cereal::BinaryOutputArchive ar(os1);
    std::shared_ptr<MyType1> mty1 = std::make_shared<MyType1>(MyType1(5));
    Rcpp::Rcout << mty1->myX << std::endl;
    ar(mty1);
    Rcpp::Rcout << "Serialize finish!" << std::endl;
  }

  { // deserialize
    Rcpp::Rcout << "Deserialize begin!" << std::endl;
    std::ifstream is1("Backend/mytype1.bin", std::ios::binary);
    cereal::BinaryInputArchive ar(is1);
    std::shared_ptr<MyType1> myt1;
    ar(myt1);
    Rcpp::Rcout << myt1->myX << std::endl;
    Rcpp::Rcout << "Deserialize finish!" << std::endl;    
  }
  // -------------------------------------------  
  
  
  // MyType2
  // -------------------------------------------
  { // serialize
    Rcpp::Rcout << "Serialize begin!" << std::endl;
    std::ofstream os2("Backend/mytype2.bin", std::ios::binary);
    cereal::BinaryOutputArchive ar(os2);
    std::shared_ptr<MyType2> mty2 = std::make_shared<MyType2>(MyType2(42));
    Rcpp::Rcout << mty2->myX << std::endl;
    ar(mty2);
    Rcpp::Rcout << "Serialize finish!" << std::endl;    
  }
  
  { // deserialize
    Rcpp::Rcout << "Deserialize begin!" << std::endl;
    std::ifstream is2("Backend/mytype2.bin", std::ios::binary);
    cereal::BinaryInputArchive ar(is2);
    std::shared_ptr<MyType2> myt2;
    ar(myt2);
    Rcpp::Rcout << myt2->myX << std::endl;
    Rcpp::Rcout << "Deserialize finish!" << std::endl;
  }
  // -------------------------------------------
  return;
}


// [[Rcpp::export]]
int main() {

  simulate_types_without_default_ctor();
  Rcpp::Rcout << std::endl;

  return 0;
}