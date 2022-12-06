// Serialization
// ----------------------------------------------
// Inspiration taken from:
// https://www.youtube.com/watch?v=1zmujmkk95c

//[[Rcpp::depends(Rcereal)]]
#include <iostream>
#include <fstream>
#include <string>

#include <cereal/archives/portable_binary.hpp>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

#include <Rcpp.h>


class EmployeeData
{

  public:
    EmployeeData() = default;
    EmployeeData(std::string name, int age, std::string company)
      : name{name}, age{age}, company{company} {}
    ~EmployeeData() = default;
    
    void showData()
    {
      std::cout << name << ", " << age << ", " << company << std::endl;
    }
  
    std::string get_name() const {
      return name;
    }
    
    std::string get_company() const {
      return company;
    }
    
    int get_age() const {
      return age;
    }
    
  private:

    std::string name;
    int age;
    std::string company;

    friend class cereal::access;
        
    template<class Archive>
    void serialize(Archive& archive)
    {
      archive(
        CEREAL_NVP(name),
        CEREAL_NVP(age),
        CEREAL_NVP(company)
      );
    }

};

void perform_Serialization() {
  std::ofstream os("Backend/employee.bin", std::ios::binary);
  cereal::PortableBinaryOutputArchive archive(os);
  
  EmployeeData emp1("Chandan", 34, "Microsoft");
  EmployeeData emp2("Hans", 21, "Google");
  EmployeeData emp3("Juergen", 56, "SAP");
  archive(cereal::make_nvp("Microsoft employee: ", emp1),
          cereal::make_nvp("Google employeee: ", emp2),
          cereal::make_nvp("SAP employee: ", emp3)
        );
  return;
}

void perform_Deserialization() {
  std::ifstream is("Backend/employee.bin", std::ios::binary);
  cereal::PortableBinaryInputArchive archive(is);
  EmployeeData emp1{}, emp2{}, emp3{};
  archive(emp1, emp2, emp3);
  
  Rcpp::Rcout << "Employee:\n" << "name: " << emp1.get_name()
              << ", age: " << emp1.get_age()
              << ", company: " << emp1.get_company()
              << std::endl;
  
  Rcpp::Rcout << "Employee:\n" << "name: " << emp2.get_name()
              << ", age: " << emp2.get_age()
              << ", company: " << emp2.get_company()
              << std::endl;
  
  Rcpp::Rcout << "Employee:\n" << "name: " << emp3.get_name()
              << ", age: " << emp3.get_age()
              << ", company: " << emp3.get_company()
              << std::endl;
  
}

// [[Rcpp::export]]
int main()
{
  perform_Serialization();
  perform_Deserialization();
  
  return 0;
}



