// [[Rcpp::depends(Rcereal)]]
#include <vector>
#include <fstream>
// -----------------------------------
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
// -----------------------------------
#include <Rcpp.h>


class Serialize_ArmaMatrix
{
  
};


class Serialize_NumericMatrix
{
private:
  int nrows;
  int ncols;
  std::vector<double> matrix_data;
  
  friend class cereal::access;
  
  template<class Archive>
  void serialize(Archive& ar) {
    ar(
      CEREAL_NVP(nrows),
      CEREAL_NVP(ncols),
      CEREAL_NVP(matrix_data)
    );
  }
  

public:
  Serialize_NumericMatrix(){};
  Serialize_NumericMatrix(Rcpp::NumericMatrix x)
    : nrows{x.nrow()}, ncols{x.ncol()}, 
      matrix_data{Rcpp::as<std::vector<double>>(x)}
    {}
  ~Serialize_NumericMatrix() = default;
  
  Rcpp::NumericVector numMatrix() {
    Rcpp::NumericVector d = Rcpp::wrap(matrix_data);
    d.attr("dim") = Rcpp::Dimension(nrows, ncols);
    return d;
  }
  
  int get_nrows() const {
    return nrows;
  }
  
  int get_ncols() const {
    return ncols;
  }
  
  std::vector<std::size_t> get_dims() const {
    std::vector<std::size_t> dims;
    dims.push_back(get_nrows());
    dims.push_back(get_ncols());
    return dims;
  }
  
  std::vector<double> get_matrix_data() const {
    return matrix_data;
  }

};


class Fan_of_linear_Agebra
{
private:
  int i1, i2;
  Serialize_NumericMatrix sNM;
  Rcpp::NumericMatrix m;

  friend class cereal::access;
  
  template<class Archive>
  void serialize(Archive& archive) {
    archive(
      CEREAL_NVP(i1),
      CEREAL_NVP(i2)
    );
    
    archive(
      CEREAL_NVP(sNM)
    );

  }
  
public:
  Fan_of_linear_Agebra() = default;
  Fan_of_linear_Agebra(int i1, int i2, Rcpp::NumericMatrix m)
    : i1{i1}, i2{i2}, sNM{m} {
    };
  ~Fan_of_linear_Agebra() = default;
  
  int get_int1() const {
    return i1;
  }
  
  int get_int2() const {
    return i2;
  }
  
  Rcpp::NumericMatrix get_numericMatrix() {
    m = Rcpp::NumericMatrix(sNM.get_nrows(), sNM.get_ncols());
    m.
    return m;
  }
  
};


void serialize() {
  std::ofstream os("Backend/fan_of_lin_algebra.bin", std::ios::binary);
  cereal::BinaryOutputArchive oarchive(os);
  Rcpp::NumericMatrix mat(5, 5);
  std::fill(mat.begin(), mat.end(), 42);
  Fan_of_linear_Agebra fola(4, 42, mat);
  Rcpp::Rcout << fola.get_numericMatrix() << std::endl;
  
  oarchive(cereal::make_nvp("Fan of linear algebra: ", fola));
  
  return;
}


void deserialize() {
  std::ifstream is("Backend/fan_of_lin_algebra.bin");
  cereal::BinaryInputArchive iarchive(is);
  Fan_of_linear_Agebra fola{};
  iarchive(fola);
  Rcpp::Rcout << fola.get_numericMatrix() << std::endl;
  
  return;
}


// [[Rcpp::export]]
int main() {
  serialize();
  deserialize();
  
  return 0;
}