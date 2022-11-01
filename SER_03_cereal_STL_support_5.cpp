// Serialization
// ----------------------------------------------
// https://www.youtube.com/watch?v=1zmujmkk95c

//[[Rcpp::depends(Rcereal)]]
#include <iostream>
#include <fstream>
#include <string>

#include <cereal/archives/binary.hpp>
#include <cereal/access.hpp>
// STL support
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
    
    void serialize_EmployeeData(std::string path_to_backend, std::string file_name) {
      std::ofstream os(path_to_backend + file_name, std::ios::binary);
      cereal::BinaryOutputArchive oarchive(os);
      oarchive(cereal::make_nvp(file_name, *this));
    }
    
    void deserialize_EmployeeData(std::string path_to_backend, std::string file_name) {
      std::ifstream is(path_to_backend + file_name);
      cereal::BinaryInputArchive iarchive(is);
      iarchive(*this);
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
  cereal::BinaryOutputArchive archive(os);
  
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
  std::ifstream is("Backend/employee.bin");
  cereal::BinaryInputArchive archive(is);
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
void simulate_serialization() {
  EmployeeData emp1("Chandan", 34, "Microsoft");
  
  Rcpp::Rcout << "Employee:\n" << "name: " << emp1.get_name()
              << ", age: " << emp1.get_age()
              << ", company: " << emp1.get_company()
              << std::endl;
  
  
  emp1.serialize_EmployeeData("Backend/", "employee_Chandan.bin");
}


// [[Rcpp::export]]
void simulate_deserialization() {
  EmployeeData emp1;
  emp1.deserialize_EmployeeData("Backend/", "employee_Chandan.bin");
  
  Rcpp::Rcout << "Employee:\n" << "name: " << emp1.get_name()
              << ", age: " << emp1.get_age()
              << ", company: " << emp1.get_company()
              << std::endl;
}


// [[Rcpp::export]]
int main()
{
  perform_Serialization();
  perform_Deserialization();
  
  return 0;
}


RCPP_MODULE(EmployeeData_mod) {
  
  Rcpp::class_<EmployeeData>("EmployeeData")
    // Constructors
    .constructor("No-args constructor.")
    .constructor<std::string, int, std::string>("Standard constructor for employee data.")
    
    // public methods
    .method("showData", &EmployeeData::showData, "Show employee data.")
    .method("get_name", &EmployeeData::get_name, "Return employee name.")
    .method("get_company", &EmployeeData::get_company, "Return company name.")
    .method("get_age", &EmployeeData::get_age, "Return employee age.")
    .method("serialize_EmployeeData", &EmployeeData::serialize_EmployeeData, "Serialize class instance and save on disks.")
    .method("deserialize_EmployeeData", &EmployeeData::deserialize_EmployeeData, "Deserialize class instance and restore from disk.")
    
  ;

}

/*** R
# Demo
# ----------------


# First, serialize and deserialize at the C++ backend
main()

# Second, serialize and deserialize with class methods
simulate_serialization()
simulate_deserialization()

# Third, do it from R
emp <- new(EmployeeData, "Chandan", 34, "R")
emp$showData()

# serialize class instance
emp$serialize_EmployeeData("Backend/", "employee_Chandan_from_R.bin")
rm(emp)
gc()

emp <- new(EmployeeData)
emp$deserialize_EmployeeData("Backend/", "employee_Chandan_from_R.bin")
emp$showData()
*/

