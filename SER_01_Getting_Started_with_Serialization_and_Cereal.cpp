// SER_01 Getting started with serialization in cereal
// Example: Serialize some simple data
// ----------------------------------------
#include <iostream>
#include <sstream>

// [[Rcpp::depends(Rcereal)]]
#include <cereal/archives/binary.hpp>
#include <Rcpp.h>

// some complex data class
class MyData
{
public:
  MyData(){};
  
  int x, y, z;
  
  // let cereal know which data members to serialize
  template<typename Archive>
  void serialize(Archive& archive)
  {
    archive(x, y, z);
  }
  
};


// [[Rcpp::export]]
int main()
{

  std::stringstream ss; // any stream class can be used
  
  // serialization
  // -------------------
  {
    cereal::BinaryOutputArchive oarchive(ss); // write data to archive
    
    // we create three instances of our class "MyData"
    MyData m1, m2, m3;
    m1.x = 40;
    m1.y = 41;
    m1.z = 42;
    
    m2.x = 40;
    m2.y = 41;
    m2.z = 42;
    
    m3.x = 40;
    m3.y = 41;
    m3.z = 42;
    
    // write the data to the archive
    oarchive(m1, m2, m3);
    
  } // archive goes out of scope, ensuring all contents are flushed

  
  // Some archives in cereal can only safely finish flushing their contents upon
  // their destruction. Make sure, especially for output serialization, that
  // your archive is automatically destroyed when you are finished with it.


  // de-serialization
  // -------------------
  {
    cereal::BinaryInputArchive iarchive(ss); // create an input
    
    MyData m1, m2, m3;
    
    iarchive(m1, m2, m3); // read the data from archive
    
    Rcpp::Rcout << "Class obj m1: x, y, z" << std::endl;
    Rcpp::Rcout << m1.x << std::endl;
    Rcpp::Rcout << m1.y << std::endl;
    Rcpp::Rcout << m1.z << std::endl;
    Rcpp::Rcout << std::endl;
    
    Rcpp::Rcout << "Class obj m2: x, y, z" << std::endl;
    Rcpp::Rcout << m2.x << std::endl;
    Rcpp::Rcout << m2.y << std::endl;
    Rcpp::Rcout << m2.z << std::endl;
    Rcpp::Rcout << std::endl;    
    
    Rcpp::Rcout << "Class obj m3: x, y, z" << std::endl;
    Rcpp::Rcout << m3.x << std::endl;
    Rcpp::Rcout << m3.y << std::endl;
    Rcpp::Rcout << m3.z << std::endl;
    Rcpp::Rcout << std::endl;
        
  }
  
  
  return 0;
}