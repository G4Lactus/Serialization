// Archive Specialization
// ---------------------------------
// cereal comes with support for the exhibit specific behaviors with specific
// archives.
// This is done by replacing at least one generic template parameter with the
// type you wish to specialize for.
//
// Sometimes you may need to have custom functionality for a type that cereal
// provides the serialization for.
// Function overloading can be used to override the cereal implementation
// with a custom one. This will work if you include teh cereal support for a
// type - assuming that the compiler doesn't find any ambiguity in your
// overload.


// [[Rcpp::depends(Rcereal)]]
#include <Rcpp.h>
#include <cereal/archives/xml.hpp>

// Specializing the archive
// --------------------------
namespace cereal
{
  // Overload the std::complex serialization code for a specific archive
  template<class T>
  void save(cereal::XMLOutputArchive& ar, std::complex<T> const& comp)
  {/*your code*/}
  
  template<class T>
  void load(cereal::XMLInputArchive& ar, std::complex<T>& comp)
  {/*your code*/}
}

// Specializing the type
// ---------------------------
// The type for all archives or a group of archives can be specialized by 
// restricting the serialized type instead of restricting the archive.

// In the following we serialize std::map<std::string, std::string> for text
// archives such that it roughly matches the output of an SQL "WHERE" in which
// all expressions are ANDed.

// [[Rcpp::depends(Rcereal)]]
#include <Rcpp.h>

// Specializing the archive
// ---------------------------
namespace cereal
{
  // Saving for std::map<std::string, std::string> for text based archives.
  // Note, that this shows off some internal traits such as EnableIf,
  // which will only allow this template to be instantiated if its predicates.
  // are true.
  template<class Archive, class C, class A, traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae> inline
  void save(Archive& ar, std::map<std::string, std::string, C, A>const& map) {
    for (const auto& i: map)
    {
      ar(cereal::make_nvp(i.first, i.second));
    }
  }
    
}


// Specializing the type
// ----------------------------



