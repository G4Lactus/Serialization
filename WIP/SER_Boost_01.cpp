// Serialization with boost
// ----------------------------------
// https://www.youtube.com/watch?v=1zmujmkk95c


#include <iostream>
#include <sstream>
#include <string>


#include <Rcpp.h>
// [[Rcpp::depends(BH)]]

// header files for binary boost serialization
// -------------------------------------------
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
// -------------------------------------------



class EmployeeData
{
private:
  
  friend class boost::serialization::access;
  std::string name;
  int age;
  std::string company;
  
  template<class Archive>
  void serialize(Archive& archive, const unsigned int version)
  {
    archive& name;
    archive& age;
    
    if (version > 0) {
      archive& company;
    }
  }
  
  
public:
  EmployeeData() = default;
  EmployeeData(std::string name, int age, std::string company)
    : name{name}, age{age}, company{company} {}
  ~EmployeeData() = default;
  
  void showData()
  {
    std::cout << name << ", " << age << ", " << company << std::endl;
  }

  void save(std::ostringstream& oss)
  {
    boost::archive::binary_oarchive oa(oss);
    oa&* (this);
  }
  
  void load(std::ostringstream& oss)
  {
    std::string str_data = oss.str();
    std::istringstream iss(str_data);
    boost::archive::binary_iarchive ia(iss);
    ia&* (this);
  }
};

// versioning
BOOST_CLASS_VERSION(EmployeeData, 1);


// [[Rcpp::export]]
int main() 
{
  EmployeeData emp("Chandan", 34, "Microsoft");
  
  std::ostringstream oss;
  
  // saving data in oss
  emp.save(oss);

  EmployeeData newEmp;
  
  // loading data in newEmp with binary archive
  newEmp.load(oss);
  newEmp.showData();
  
  return 0;
}

