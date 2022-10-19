// Explicit versioning and Inheritance
// ----------------------------------------
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
// [[Rcpp::depeneds(Rcereal)]]
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
    if (version > some_number) {
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


// iIf we don't associate a class with a version number and use a versioned
// serialize function, its version number will default to 0.



