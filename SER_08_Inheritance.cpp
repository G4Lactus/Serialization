// Inheritance
// --------------------------
// cereal fully supports serializing class inheritance and serialization.


// Normal Inheritance
// -----------------------
// [[Rcpp::depends(Rcereal)]]
#include <cereal/types/base_class.hpp>
#include <Rcpp.h>

struct Base
{
  int x;
  
  template<class Archive>
  void serialize(Archive& ar) { ar(x); }
  
};


struct Derived : public Base
{
  int y;
  template<class Archive>
  void serialize(Archive & ar)
  {
    // We pass this cast to the base type for each base type we need to 
    // serialize. Do this insteead of calling serialize functions directly.
    ar(cereal::base_class<Base>(this), y);
  }
  
};


// Virtual Inheritance
// -----------------------
// Only one copy of each base class will be contained within a derived object.
// cereal provides a method for tracking virtual base classes, preventing 
// multiple copies from being serialized.
// 
// If cereal::base_class were used in a situation where the same base class
// appeared in more than one traversal of the class hierarchy, it could
// potentially result in duplicate information being serialized.
// 
// USE: cereal::virtual_base_class, to track the instantiation of base objs
// for a derived class, ensuring that only one copy of each base class is
// serialized.
// -------------
#include <cereal/types/base_class.hpp>

struct BaseA
{
  int x;
  // Note no serialize member here
  // -> trying to call serialize from a derived class wouldn't work
};


template<class Archive>
void serialize(Archive& ar, BaseA& b) {ar(b.x);}

struct Left : virtual BaseA
{
  int l;
  template<class Archive>
  void serialize(Archive& ar) {ar(cereal::virtual_base_class<BaseA>(this), l);}
};


struct Right : virtual BaseA
{
  int r;
  template<class Archive>
  void serialize(Archive& ar) {ar(cereal::virtual_base_class<BaseA>(this), r);}
};


struct DerivedA : virtual Left, virtual Right
{
  int y;
  template<class Archive>
  void serialize(Archive& ar) {ar(cereal::virtual_base_class<Left>(this),
                                  cereal::virtual_base_class<Right>(this),
                                  y);}
};

// With virtual inheritance and cereal::virtual_base_class, x, l, r, and y
// will serialized exactly once.
// If we had not used virtual_base_class, two copies of the base class may have
// been serialized resulting in duplicate x entries.





