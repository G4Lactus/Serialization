// [[Rcpp::depends(Rcereal)]]
#include <string>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>
#include <Rcpp.h>

struct ShaderProgram
{
  ShaderProgram(){};
  ShaderProgram(std::string program_name)
    : program_name{program_name}{};
  ~ShaderProgram() = default;
  
  std::string get_program_name() const {
    return program_name;
  }
  
private:
  std::string program_name{};
  
  friend class cereal::access;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(program_name);
  }
  
};


// [[Rcpp::export]]
int main() {
  
  {
    ShaderProgram sp("King Kong 8");
    
    std::ofstream os("Backend/Serialize_StringProgram.bin", std::ios::binary);
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(sp);
  }
  
  {
    ShaderProgram sp{};
    std::ifstream is("Backend/Serialize_StringProgram.bin", std::ios::binary);
    cereal::BinaryInputArchive iarchive(is);
    iarchive(sp);
    
    Rcpp::Rcout << sp.get_program_name() << std::endl;
  }

}


