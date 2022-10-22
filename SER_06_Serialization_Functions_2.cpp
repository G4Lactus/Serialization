// Non-public serialization and Minimal split serialization
// ----------------------------------------------------------
// [[Rcpp::depends(Rcereal)]]
#include <iostream>
#include <cereal/access.hpp>
#include <Rcpp.h>


// Non-public serialization
// --------------------------
// Serialization functions can be private or protected.
// cereal will need access to them, and can be given access by befriending.
// include: friend cereal::access, defined in <cereal/access.hpp>

class MyCoolClass
{
private:
  int secretX;
  
  friend class cereal::access;
  
  template<class Archive>
  void serialize(Archive& ar)
  {
    ar(secretX);
  }
  
};


// Minimal split serialization
// ------------------------------
// cereal also supports load_minimal and save_minimal serialization functions
// with a slightly different interface.
// 
// Minimal serialization functions are designed to take a type and reduce it to
// a single primitive or string value.
// The primary reason to use minimal serialization is to simplify the output
// to a human readable archive (e.g., XML or JSON).
// 

// internal split minimal serialization
struct MyData1
{
  double d;
  
  template<class Archive>
  double save_minimal(Archive const& ) const
  {
    return d;
  }
  
  template<class Archive>
  void load_minimal(Archive const&, double const& value)
  {
    d = value;
  }
  
};


// external split minimal serialization
struct MyData2
{
  double d;
};

template<class Archive>
double save_minimal(Archive const&, MyData2 const& md)
{
  return md.d;
}

template<class Archive>
void load_minimal(Archive const&, MyData2& md, double const& value)
{
  md.d = value;
}


// There are few key differences in the interface of the minimal functions:
// ------------------------------
// 1. They accept their Archive template by constant reference
// 2. save_minimal returns the minimal representation, which can either be an
//    arithmetic type or an std::string
// 3. load_minimal is passed a constant to the return type of save_minimal.
//
// Minimal serialization can only emit a single value and are only useful when
// this approach makes sense.

// The following example demonstrates the difference between minimal and
// standard serialization.

#include <cereal/archives/json.hpp>

struct Minimal
{
  std::string myData;
  
  template<class Archive>
  std::string save_minimal(Archive const&) const
  {
    return myData;
  }
  
  template<class Archive>
  void load_minimal(Archive const&, std::string const& value)
  {
    myData = value;
  }
};


struct Normal
{
  std::string myData;
  
  template<class Archive>
  void serialize(Archive& ar)
  {
    ar(myData);
  }
};



// [[Rcpp::export]]
int main()
{
  Minimal m = {"minimal"};
  Normal n = {"normal"};
  
  // don't create file, but print out the structure on the console
  cereal::JSONOutputArchive ar(std::cout);
  ar(CEREAL_NVP(m), CEREAL_NVP(n));
  
  return 0;
}




