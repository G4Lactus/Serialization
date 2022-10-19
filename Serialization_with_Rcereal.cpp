// Serialization with Rcereal
// https://cran.r-project.org/web/packages/Rcereal/readme/README.html
// 
// Rcereal grants access to cereal header files, a C++11 serialization library.
// cereal takes (std::container) arbitrary data types and reversibly turns them
// into different representations, such as binary encodings, XML, or JSON.
// 
// This package can be used via the LinkingTo: field in the DESCRIPTION field
// of an R package and the Rcpp::depends in the Rcpp-attributes.
// 
// To compile the cpp file, the user must enable the support of c++11 before
// using Rcpp::sourceCpp.
// Sys.setenv(PKG_CXXFLAGS="-std=c++11")
// Rcpp::sourceCpp("<the path to the cpp file>")
// ------------------------------

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
// [[Rcpp::depends(Rcereal)]]
#include <Rcpp.h>
#include <cereal/archives/binary.hpp>


struct MyClass
{
  int x, y, z;
  double d1, d2;
  std::string s1, s2;
  
  // this method lets cereal know which data members to serialize
  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(x, y, z, d1, d2, s1, s2);
  }
  
};



// [[Rcpp::export]]
Rcpp::RawVector serialize_MyClass(int x = 1, int y = 2, int z = 3)
{
  MyClass my_instance;
  my_instance.x = x;
  my_instance.y = y;
  my_instance.z = z;
  my_instance.d1 = 42.0;
  my_instance.d2 = 42.42;
  my_instance.s1 = "Hans";
  my_instance.s2 = "Wurst";
  
  std::stringstream ss;
  {
    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(my_instance);    
  }
  ss.seekg(0, ss.end);
  Rcpp::RawVector retval(ss.tellg());
  ss.seekg(0, ss.beg);
  ss.read(reinterpret_cast<char*>(&retval[0]), retval.size());
  
  return retval;
}


// [[Rcpp::export]]
void deserialize_MyClass(Rcpp::RawVector src)
{
  std::stringstream ss;
  ss.write(reinterpret_cast<char*>(&src[0]), src.size());
  ss.seekg(0, ss.beg);
  
  MyClass my_instance;
  {
    cereal::BinaryInputArchive iarchive(ss);
    iarchive(my_instance);
  }
  
  Rcpp::Rcout << my_instance.x  << "\n"
              << my_instance.y  << "\n"
              << my_instance.z  << "\n"
              << my_instance.d1 << "\n"
              << my_instance.d2 << "\n"
              << my_instance.s1 << "\n"
              << my_instance.s2 << std::endl;
}
