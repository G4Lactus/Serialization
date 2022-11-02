// [[Rcpp::depends(Rcereal)]]
#include <fstream>
#include <memory>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Rcpp.h>

class Base
{
public:
  Base(){};
  
  int base_mem{};
  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(base_mem);
  }
};


class Derived: public Base
{
public:
  Derived(){};
  int der_mem{};
  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::base_class<Base>(this), der_mem);
  }
};


// [[Rcpp::export]]
int main() 
{
  {
    Base bs{};
    bs.base_mem = 42;
    std::shared_ptr<Derived> shr_ptr_dr = std::make_shared<Derived>();
    shr_ptr_dr.get()->der_mem = 43;
    shr_ptr_dr.get()->base_mem = 45;
    
    std::ofstream os("Backend/Output.bin", std::ios::binary);
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(bs, shr_ptr_dr);
  }
  
  {
    std::ifstream is("Backend/Output.bin", std::ios::binary);
    cereal::BinaryInputArchive iarchive(is);
    Base bs{};
    std::shared_ptr<Derived> shr_ptr_dr;
    iarchive(bs, shr_ptr_dr);
    
    Rcpp::Rcout << bs.base_mem << std::endl;
    Rcpp::Rcout << shr_ptr_dr.get()->der_mem << std::endl;
    Rcpp::Rcout << shr_ptr_dr.get()->base_mem << std::endl;
    
    
  }
}