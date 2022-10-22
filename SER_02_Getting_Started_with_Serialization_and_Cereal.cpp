// SER_02 Getting started: name value pairs and different file types backed on
// disk
// -----------------------------------
// [[Rcpp::depends(Rcereal)]]
#include <fstream> // the chosen stream operator is more less irrelevant

// archive types offered by cereal
// ----------------------------------
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
// ----------------------------------
#include <Rcpp.h>


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
  
  { // serialize towards files backed on disk
    std::ofstream os1("Backend/data_SER02.bin", std::ios::binary);
    std::ofstream os2("Backend/data_SER02.xml");
    std::ofstream os3("Backend/data_SER02.json");
    
    cereal::BinaryOutputArchive oarchive1(os1);
    cereal::XMLOutputArchive oarchive2(os2);
    cereal::JSONOutputArchive oarchive3(os3);

    // some data we want to store on file
    MyData m1;
    m1.x = 40;
    m1.y = 41;
    m1.z = 42;
    int someInt{0};
    double d{42.42};
    
    oarchive1(
      CEREAL_NVP(m1), // Names, the output the same as the variable name
      someInt,        // No NVP - cereal will automatically generate a enumerate name 
      cereal::make_nvp("this_name_is_way_better", d) // specify a name of your choosing
    );
    
    oarchive2(
      CEREAL_NVP(m1), // Names, the output the same as the variable name
      someInt,        // No NVP - cereal will automatically generate a enumerate name 
      cereal::make_nvp("this_name_is_way_better", d) // specify a name of your choosing
    );
    
    oarchive3(
      CEREAL_NVP(m1), // Names, the output the same as the variable name
      someInt,        // No NVP - cereal will automatically generate a enumerate name 
      cereal::make_nvp("this_name_is_way_better", d) // specify a name of your choosing
    );
  }

  // in this block no data are availabe, they reside on disk
  
  
  { // deserialize from disk
    std::ifstream is1("Backend/data_SER02.bin");
    std::ifstream is2("Backend/data_SER02.xml"); 
    std::ifstream is3("Backend/data_SER02.json");    

    cereal::BinaryInputArchive iarchive1(is1);
    cereal::XMLInputArchive iarchive2(is2);
    cereal::JSONInputArchive iarchive3(is3);

    MyData m1, m2, m3;
    int someInt1, someInt2, someInt3;
    double d1, d2, d3;
      
      
    // NVPs are not strictly necessary when loading but could be used
    // (even out of order)      
    iarchive1(m1, someInt1, d1);
    iarchive2(m2, someInt2, d2);
    iarchive3(m3, someInt3, d3);
    // deserialization finished
    
    // data are back, work with them :)
    Rcpp::Rcout << "source bin: "<< "x: " << m1.x << ", y: " << m1.y << " z: " << m1.z << std::endl;
    Rcpp::Rcout << "source xml: "<< "x: " << m2.x << ", y: " << m2.y << " z: " << m2.z << std::endl;
    Rcpp::Rcout << "source json: "<< "x: " << m3.x << ", y: " << m3.y << " z: " << m3.z << std::endl;
    
    Rcpp::Rcout << "source bin: "<< "someInt: " << someInt1 << std::endl;
    Rcpp::Rcout << "source xml: "<< "someInt: " << someInt2 << std::endl;
    Rcpp::Rcout << "source json: "<< "someInt: " << someInt3 << std::endl;
    
    Rcpp::Rcout << "source bin: "<< "d: " << d1 << std::endl;
    Rcpp::Rcout << "source xml: "<< "d: " << d2 << std::endl;
    Rcpp::Rcout << "source json: "<< "d: " << d3 << std::endl;
    
    // continue working with your class
    m1.x += 42;
    m1.z -= 7;
    m2.y += m3.x;
    m3.y *= d2;
    Rcpp::Rcout << m3.y << std::endl;
  }
  
  return 0;
}

