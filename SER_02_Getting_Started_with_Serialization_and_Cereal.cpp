// SER_02 Getting started: name value pairs
// 
// -----------------------------------
#include <fstream>

// [[Rcpp::depends(Rcereal)]]
#include <cereal/archives/xml.hpp>
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
  
  {
    std::ofstream os("Backend/data.xml");
    cereal::XMLOutputArchive archive(os);
    
    MyData m1;
    m1.x = 40;
    m1.y = 41;
    m1.z = 42;
    int someInt{0};
    double d{42.42};
    
    archive(
      CEREAL_NVP(m1), // Names, the output the same as the variable name
      someInt,        // No NVP - cereal will automatically generate a enumerate name 
      cereal::make_nvp("this_name_is_way_better", d) // specify a name of your choosing
    );
  }


  {
      std::ifstream is("Backend/data.xml");
      cereal::XMLInputArchive archive(is);
      
      MyData m1;
      int someInt;
      double d;
      
      archive(m1, someInt, d); // NVPs not strictly necessary when loading
                               // but could be used (even out of order)
  }
  
  
}

