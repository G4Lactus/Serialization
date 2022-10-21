// Ensuring a path exists from a derived to base type
// -----------------------------------------------------
// cereal does not require that the base type itself is registered, but only
// derived classes. However, it needs to know how to properly convert between
// derived and base types.
// 
// Normally, cereal can determine this automatically if your derived class
// serializes the base class.
// ----------------------------
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <Rcpp.h>
// [[Rcpp::depends(Rcereal)]]

struct Base
{
  virtual void foo() = 0;
  int x;
  
  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(x);
  }
};

struct DerivedOne: Base
{
  void foo() {}
  double y;
  
  // cereal can automatically deduce that DerivedOnce is related to Base
  // requires that all possible base classes that are actually used to store a
  // pointer have a well defined path from the derived class
  template<class Archive>
  void serialize(Archive& ar) {
    ar(cereal::base_class<Base>(this), y);
  }
};

struct EmptyBase
{
  virtual void foo() = 0;
};

struct DerivedTwo: EmptyBase
{
  void foo() {}
  double y;
  
  template<class Archive>
  void serialize(Archive& ar)
  {
    void foo() {}
    double y;
    
    template<class Archive>
    void serialize(Archive& ar) { ar(y); }
  }
};

CEREAL_REGISTER_TYPE(DerivedOne);
CEREAL_REGISTER_POLYMORPHIC_RELATION(EmptyBase, DerivedTwo);



// Registering Archives: requirement to be used with polymorphic types. Use the
// CEREAL_REGISTER_ARCHIVE macro. Only relevant if you want to design a custom
// archive and wish for it to support polymorphism.
// --------------------------
// namespace mynamespace
// {
//   class MyNewOutputArchive: public OutputArchive<MyNewArchive>
//   {/*...*/};
// }
// CEREAL_REGISTER_ARCHIVE(mynamespace::MyNewOutputArchive);
// --------------------------


