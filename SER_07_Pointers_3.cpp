#include <iostream>
#include <memory>
#include <string>


// [[Rcpp::depends(Rcereal)]]
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#include <Rcpp.h>


struct A {
  
  explicit A(int x_) : x(x_) {}
  virtual ~A() = default;
  
  int x;
  
  template<typename Archive>
  void serialize(Archive& archive)
  {
    archive(x, hidden);
  }
  
  template <class Archive>
  static void load_and_construct( Archive & ar, cereal::construct<A> & construct )
  {
    int x;
    ar(x);
    construct(x);
    // Issue #1
  }
  
private:
  
  bool hidden = false;
  
};


struct B : A {
  
  explicit B(int x, std::string y_) : A(x), y(y_) {}
  
  std::string y;
  
  template<typename Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::base_class<A>(this), y);
  }
  
  template <class Archive>
  static void load_and_construct( Archive & ar, cereal::construct<B> & construct )
  {
    // Issue #2
    int x;
    std::string y;
    ar(x, y);
    construct(x, y);
  }
  
};

CEREAL_REGISTER_TYPE(B);

void simulate_procedure()
{
  std::shared_ptr<B> b = std::make_shared<B>(10, "1");
  
  std::stringstream ss;
  {
    cereal::JSONOutputArchive oarchive(ss);
    oarchive(b);
  }
  Rcpp::Rcout << ss.str() << std::endl;
  std::shared_ptr<B> b2;
  {
    cereal::JSONInputArchive iarchive(ss);
    iarchive(b2);
  }  
}


// [[Rcpp::export]]
int main() {
  
  simulate_procedure();
  Rcpp::Rcout << std::endl;
  
  return 0;
}