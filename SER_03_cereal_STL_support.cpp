// SER_03: Standard Library Support
// -------------------------------------------------------
// cereal supports almost all containers and classes found in C++ STL.
// Therefore, include the proper header file, s.a., 
// #include <cereal/types/xxxx.hpp>
// where x stands for containers, s.a., vector
// 
// If you don't include the appropriate type, you will receive compile time
// errors about cereal being unable to find an appropriate serialization
// function.
// ----------------------------------
#include <iostream>

// [[Rcpp::depends(Rcereal)]]
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

// for doing the actual serialization into JSON format
#include <cereal/archives/json.hpp>
#include <Rcpp.h>

class STL_User_Class
{
public:
  STL_User_Class() = default;
  
  void fillData()
  {
    // fill map with key value pairs
    data = {
      {"real", { {1.0f, 0},
                 {2.2f, 0},
                 {3.3f, 0} }},
      {"imaginary", { {0, -1.0f},
                      {0, -2.9932f},
                      {0, -3.5f} }}
    };

  }
  
private:
  // our data type
  std::map<std::string, std::vector<std::complex<float>>> data;
  
  // friend
  friend class cereal::access;
  
  // let cereal know what to archive
  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(CEREAL_NVP(data));
  }
  
};



// [[Rcpp::export]]
int main()
{
  // create container for serial data
  cereal::JSONOutputArchive output(std::cout); // stream to console, no file
                                               // is created

  
  // create class instance to create data
  STL_User_Class stl_user_class;
  stl_user_class.fillData();
  
  // fill container with data
  output(cereal::make_nvp("best data ever", stl_user_class));
  
}


/*** R
main()

*/