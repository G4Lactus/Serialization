// Explicit versioning and Inheritance
// ----------------------------------------

// Explicit versioning
// ------------------------------------
// cereal supports explicit versioning for types, much like Boost class 
// versioning.
// In cereal it is optional, the parameter has always to be a 
// const std::uint32_t, typically named version.
// 
// An explicit version can be specified by CEREAL_CLASS_VERSION macro. It takes
// a type and a version number, and causes cereal to serialize this version
// information when it saves that type using a versioned serialization function.
// 
// If you use this macro, but do not use a versioned serialized function, no
// version information will be saved.
// If you use a versioned serialization function and do not specify the version
// number using the macro, cereal will default to giving a version of zero.
// 
// When performing loads, cereal will load versioning information if your
// serialization function is versioned.
// If you did not use a versioned serialization function to create the archive
// you are loading from, your data will be corrupted and your program will
// likely crash. The version number is supplied by the version parameter.
// 

#include <iostream>
// [[Rcpp::depends(Rcereal)]]
#include <cereal/cereal.hpp>
#include <Rcpp.h>

struct MyCoolClass
{
  // cereal will supply the version automatically when loading or saving
  // The version number comes from the CEREAL_CLASS_VERSION macro
  template<class Archive>
  void serialize(Archive& ar, std::uint32_t const version)
  {
    // You can choose different behaviors depending on the version
    // This is useful if you need to support older variants of your codebase
    // interacting with newer ones
    std::size_t some_version{42};
    if (version > some_version) {
      Rcpp::Rcout << "Your version is more recent!" << std::endl;
    } else {
      Rcpp::Rcout << "Your version is older or equal!"<< std::endl;
    }
  }

};


struct AnotherType {};;

// Versioning can be applied to any type of serialization function, but if
// used for a load it must be used for a save
template<class Archive>
void save(Archive& ar, AnotherType const& at, std::int32_t const version) {}


template<class Archive>
void load(Archive& ar, AnotherType& at, std::uint32_t const version) {}

// Associate some type with a version number
CEREAL_CLASS_VERSION(MyCoolClass, 42);

// If we don't associate a class with a version number and use a versioned
// serialize function, its version number will default to 0.



// Inheritance
// ------------------------------------
// Serialization functions, like any other function, will be inherited by
// derived classes.
#include <cereal/archives/binary.hpp>

struct MyBase
{
  template<class Archive>
  void serialize(Archive&) {}
  
};


struct MyDerived: MyBase
{
  template<class Archive>
  void load(Archive& ) const {}
  
  template<class Archive>
  void save(Archive& ) const {}
};


// [[Rcpp::export]]
int main()
{
  MyDerived d;
  cereal::BinaryOutputArchive ar(std::cout);
  ar(d);
  
  return 0;
}

// potentiall here a static assertion failure could arise during compile time,
// because cereal detects both: a serialize and save/load pair for MyDerived.
// MyDerived inhertis the public serialize from MyBase, thus giving it both a
// serialize and load-save pair, a disambiguate situation for cereal.


// Even if serialize was made private in the base class, cereal may still have
// access to it from the derived class because of a friend to cereal::access,
// resulting in the same error.
#include <cereal/access.hpp>
namespace cereal 
{
  template<class Archive>
  struct specialize<Archive, MyDerived, cereal::specialization::member_load_save>{};
  // cereal no longer has any ambiguity when serializing MyDerived
}

// CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(MyDerived, cereal::specialization::member_load_save)

// Default construction
// cereal requires access to a default constructor for types it serializes.
// If you don’t want to provide a default constructor but do want to serialize
// smart pointers to it, you can get around this restriction using a special
// overload, detailed in the pointers section of the documentation.

// Renaming serialization functions
// If your coding conventions conflict with the expected names for cereal
// serialization functions or if they conflict for any other reason, you can use
// the macros in <cereal/macros.hpp> to change these names.
// See the doxygen documentation for more information.