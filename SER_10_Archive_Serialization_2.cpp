// Specializing User Defined Types
// ------------------------------------
// No requirement that the serialization code resides inside the cereal
// namespace. Since there is no provided implementation to conflict with, you
// have more control over the generality of your implementations.

// Useful Type Traits:
// ---------------------
// There are several useful type traits defined in <cereal/details/traits.hpp>
// which are useful when specializing serialization functions.
// See doxygen documentation.

// EnableIf and DisableIf
// In template metaprogramming, techniques such as std::enable_if can be used to
// selectively disable function overloads.
// cereal provides some similar functionality in a slightly more aesthetically
// pleasing manner with EnableIf and DisableIf. These will enable or disable
// a function overload if all of their variadic bool parameters, when ANDed
// together, are true. Since they perform an AND operation of their arguments,
// OR operations or other more complicated boolean expressions should be done
// manually.
// EnableIf and DisableIf are added as extra template parameter to the function,
// with the default value of cereal::traits::sfinae.


// is_same_archive
// operates in a similar manner to std::is_same except that it will automatically
// strip various wrappers and CV qualifiers. Its use is recommneded over
// std::is_same because cereal will often slightly adjust archive parameters
// so that they become const or are wrapped in some traits class.


// is_text_archive
// checks to see if an archive has been tagged with the cereal::traits::TextArchive
// tag. JSON and XML archives that ship with cereal both have this tag.

// strip_minimal
// Useful when working with custom save_minimal or load_minimal. cereal may
// wrap types when performing compile time checks to ensure the validity of such
// functions, which may cause some custom metaprogramming to fail unexpectedly.
// Using the type provided by strip_minimal, instead of the raw template
// parameter, will help prevent this.



// [[Rcpp::depends(Rcereal)]]
#include <cereal/cereal.hpp>
#include <Rcpp.h>

struct Hello
{
  int x;
  
  // Enabled for text archives (e.g. XML, JSON)
  template <class Archive,
            cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>
    = cereal::traits::sfinae>
    std::string save_minimal( Archive & ) const
    { 
      return std::to_string( x ) + "hello";
    }
  
  // Enabled for text archives (e.g. XML, JSON)
  template <class Archive,
            cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>
    = cereal::traits::sfinae>
    void load_minimal( Archive const &, std::string const & str )
    {
      x = std::stoi( str.substr(0, 1) );
    }
  
  
  // Enabled for binary archives (e.g. binary, portable binary)
  template <class Archive,
            cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value>
    = cereal::traits::sfinae>
    int save_minimal( Archive & ) const
    { 
      return x; 
    }
  
  // Enabled for binary archives (e.g. binary, portable binary)
  template <class Archive,
            cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value>
    = cereal::traits::sfinae>
    void load_minimal( Archive const &, int const & xx )
    {
      x = xx;
    }
};


