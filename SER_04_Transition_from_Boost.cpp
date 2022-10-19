// Transition from Boost
// --------------------------------------------------
// Key differences:
// ----------------
// - cereal attempts to store as little metadata as possible when saving data.
//   Boost, by default, stores various metadata about the library version as
//   well as the types themselves
// 
// - cereal will not perform any checks to ensure that serialization was done
//   with the same version of cereal
//   
// - cereal is header only, nothing to link against
//
// - cereal supports almost everything in the standard library out of the box
// 
// - cereal by default supports data containers Boost does not support:
//  - <forward_list>
//  - <memory>
//  - <queue>
//  - <stack>
//  - <stack>
//  - <tuple>
//  - <unordered_set>
//  - <unordered_map>
//  
// - cereal cannot handle raw pointers, but smart ones
// 
// - cereal can be less verbose
// 
// - cereal uses static_assert to give meaningful errors when you make mistakes
// 
// - cereal has a different prefereed syntax for serialization:
//    Boost: &, <<, >>   to send things to archive
//    cereal: ()         archive(mydata1, mydata2)
// --------------------------------------------------
#include <fstream>
// [[Rcpp::depends(Rcereal)]]
#include <cereal/archives/binary.hpp>
#include <Rcpp.h>


// create some complex data class
class SomeData
{
public:
  SomeData() = default;
  int a;
  int b;
  
private:
  // set cereal access as friend
  friend class cereal::access;
  
  // cereal supports class versioning (optional)
  template <class Archive>
  void save( Archive& ar, std::uint32_t const version) const
  {
    ar(a, b); // operator() is the preferred way of interfacing the archive
  }
  
  template <class Archive>
  void load( Archive& ar, std::uint32_t const version )
  {
    ar(a, b);
  }
  
};
CEREAL_CLASS_VERSION(SomeData, 1);


// create a structure we wish to serialize
struct MyType
{
  int x;
  double y;
  SomeData s;
  
  template <class Archive>
  void serialize(Archive& ar, std::uint32_t const version)
  {
    ar(x, y); // take primitives
    ar(s);    // take complex data type
  }
};


// [[Rcpp::export]]
int main()
{
  // create container
  std::ofstream os("Backend/out.bin", std::ios::binary);
  // local scope to fill and flush out archive
  {
    cereal::BinaryOutputArchive ar(os);
    MyType m;
    ar( m );
  }
  
  return 0;
}

