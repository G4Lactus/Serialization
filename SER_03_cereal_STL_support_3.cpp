// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(Rcereal)]]
#include <vector>
#include <fstream>
#include <algorithm>
// -----------------------------------
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
// -----------------------------------
#include <RcppArmadillo.h>


// ----------------------------------------------------------------------------
typedef std::vector<double> stdvec;
typedef std::vector< std::vector<double> > stdvecvec;

stdvecvec mat_row_to_std_vec(arma::mat &A) {
  stdvecvec V(A.n_rows);
  for (size_t i = 0; i < A.n_rows; ++i) {
    V[i] = arma::conv_to<stdvec>::from(A.row(i));
  };
  return V;
}

stdvecvec mat_col_to_std_vec(arma::mat& A) {
  stdvecvec V(A.n_cols);
  for (std::size_t i = 0; i < A.n_cols; ++i) {
    V[i] = arma::conv_to<stdvec>::from(A.col(i));
  }
  return V;
}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class Serialize_ArmaMatrix
{
private:
  std::size_t nrows;
  std::size_t ncols;
  stdvecvec matrix_data;

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
  Serialize_ArmaMatrix(){};
  Serialize_ArmaMatrix(arma::mat x)
    : nrows{x.n_rows}, ncols{x.n_cols},
      matrix_data{mat_col_to_std_vec(x)}
      {}
  ~Serialize_ArmaMatrix() = default;

  std::vector<std::size_t> get_dims() const {
    std::vector<std::size_t> dims;
    dims.push_back(nrows);
    dims.push_back(ncols);
    return dims;
  }

  stdvecvec get_matrix_data() const {
    return matrix_data;
  }
  
  arma::mat reconstruct_armaMatrix() {
    arma::mat mat(nrows, ncols);
    stdvecvec vals{matrix_data};
    std::vector<std::vector<double>>::iterator it;
    std::size_t col_counter{0};
    for (it = vals.begin(); it != vals.end(); ++it)
    {
      mat.col(col_counter++) = arma::conv_to<arma::mat>::from(*it);
    }
    
    return mat;
  }

};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
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
  
  std::vector<std::size_t> get_dims() const {
    std::vector<std::size_t> dims;
    dims.push_back(nrows);
    dims.push_back(ncols);
    return dims;
  }
  
  std::vector<double> get_matrix_data() const {
    return matrix_data;
  }
  
  Rcpp::NumericMatrix reconstruct_NumericMatrix() {
    Rcpp::NumericMatrix mat(nrows, ncols);
    std::vector<double> vals{matrix_data};
    std::copy(vals.begin(), vals.end(), mat.begin());
    
    return mat;
  }

};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class Serialize_ArmaMatrix_List
{
private:

  std::list<Serialize_ArmaMatrix> lst_sAMs;

  template<class Archive>
  void serialize(Archive& ar) {

  }

public:
  Serialize_ArmaMatrix_List(){};
  Serialize_ArmaMatrix_List(std::list<arma::mat> list_AMs){
    std::list<arma::mat>::iterator it;
    for (it = list_AMs.begin(); it != list_AMs.end(); ++it) {
      lst_sAMs.emplace_back(Serialize_ArmaMatrix{*it});
    }
  };
  ~Serialize_ArmaMatrix_List() = default;


  std::list<arma::mat> reconstruction_ArmaMatrix_list() {
    std::list<arma::mat> list_AMs;
    std::list<Serialize_ArmaMatrix>::iterator it;
    for (it = lst_sAMs.begin(); it != lst_sAMs.end(); ++it) {
      Serialize_ArmaMatrix s_AM = *it;
      list_AMs.emplace_back(s_AM.reconstruct_armaMatrix());
    }

    return list_AMs;
  }

};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class Fan_of_linear_Agebra
{
private:
  int i1, i2;
  Rcpp::NumericMatrix m;
  arma::mat am;
  std::list<arma::mat> lst_am;

  friend class cereal::access;
  
  template<class Archive>
  void save(Archive& archive) const
  {
    Serialize_NumericMatrix sNM{m};
    Serialize_ArmaMatrix sAM{am};

    archive(
      CEREAL_NVP(i1),
      CEREAL_NVP(i2)
    );
    
    archive(
      CEREAL_NVP(sNM),
      CEREAL_NVP(sAM)
    );
    
    std::size_t counter{0};
    std::for_each(lst_am.begin(), lst_am.end(), [&archive](arma::mat amat) {
      archive(Serialize_ArmaMatrix{amat});
    });

  }
  

  template<class Archive>
  void load(Archive& archive)
  {
    Serialize_NumericMatrix sNM;
    Serialize_ArmaMatrix sAM;
    Serialize_ArmaMatrix_List s_lst_am;
    
    archive(
      CEREAL_NVP(i1),
      CEREAL_NVP(i2)
    );
    
    archive(
      CEREAL_NVP(sNM),
      CEREAL_NVP(sAM)
    );
    m = sNM.reconstruct_NumericMatrix();
    am = sAM.reconstruct_armaMatrix();
    


  }
  
  
public:
  Fan_of_linear_Agebra() = default;
  Fan_of_linear_Agebra(int i1, int i2, Rcpp::NumericMatrix m)
    : i1{i1}, i2{i2}, m{m} {
    };
  Fan_of_linear_Agebra(int i1, int i2, Rcpp::NumericMatrix m, arma::mat am, std::list<arma::mat> lst_am)
    : i1{i1}, i2{i2}, m{m}, am{am}, lst_am{lst_am} {
    };
  ~Fan_of_linear_Agebra() = default;
  
  int get_int1() const {
    return i1;
  }
  
  int get_int2() const {
    return i2;
  }
  
  Rcpp::NumericMatrix get_NumericMatrix() const {
    return m;
  }
  
  arma::mat get_ArmaMat() const {
    return am;
  }
  
  std::list<arma::mat> get_ArmaMatrix_List() const {
    return lst_am;
  }
  
  Rcpp::NumericMatrix reconstruct_numericMatrix(Serialize_NumericMatrix sNM) {
    std::vector<std::size_t> dims = sNM.get_dims();
    std::size_t nrows = dims[0];
    std::size_t ncols = dims[1];
    Rcpp::NumericMatrix mat(nrows, ncols);
    std::vector<double> vals{sNM.get_matrix_data()};
    std::copy(vals.begin(), vals.end(), mat.begin());

    return mat;
  }
  
  arma::mat reconstruct_armaMatrix(Serialize_ArmaMatrix sAM) {
    std::vector<std::size_t> dims = sAM.get_dims();
    std::size_t nrows = dims[0];
    std::size_t ncols = dims[1];
    arma::mat mat(nrows, ncols);
    stdvecvec vals{sAM.get_matrix_data()};
    
    std::vector<std::vector<double>>::iterator it;
    std::size_t col_counter{0};
    for (it = vals.begin(); it != vals.end(); ++it)
    {
      mat.col(col_counter++) = arma::conv_to<arma::mat>::from(*it);
    }

    return mat;
  }

};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
void serialize() {
  std::ofstream os("Backend/fan_of_lin_algebra.bin", std::ios::binary);
  cereal::BinaryOutputArchive oarchive(os);
  
  Rcpp::NumericMatrix mat(5, 5);
  Rcpp::NumericVector vals = Rcpp::rnorm(mat.ncol()*mat.nrow(), 0, 1);
  std::copy(vals.begin(), vals.end(), mat.begin());
  
  arma::mat am(4, 3, arma::fill::randu);
  arma::mat bm(3, 4, arma::fill::randu);
  arma::mat cm(4, 6, arma::fill::randu);
  arma::mat dm(9, 9, arma::fill::randu);
  
  std::list<arma::mat> lst_ams;
  lst_ams.push_back(am);
  lst_ams.push_back(bm);
  lst_ams.push_back(cm);
  lst_ams.push_back(dm);


  Fan_of_linear_Agebra fola(4, 42, mat, am, lst_ams);
  bool verbose{true};
  if (verbose) {
    Rcpp::Rcout << fola.get_NumericMatrix() << std::endl;
    fola.get_ArmaMat().print();
    Rcpp::Rcout << std::endl;
    
    std::for_each(lst_ams.begin(), lst_ams.end(), [](arma::mat& amat) {
      amat.print();
      Rcpp::Rcout << std::endl;
    });
  }

  oarchive(cereal::make_nvp("Fan of linear algebra: ", fola));
  Rcpp::Rcout << std::endl;
  return;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void deserialize() {
  std::ifstream is("Backend/fan_of_lin_algebra.bin", std::ios::binary);
  cereal::BinaryInputArchive iarchive(is);
  Fan_of_linear_Agebra fola{};
  iarchive(fola);
  
  bool verbose{true};
  if (verbose) {
    Rcpp::Rcout << fola.get_NumericMatrix() << std::endl;
    fola.get_ArmaMat().print();
    Rcpp::Rcout << std::endl;
  }
  return;

}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// [[Rcpp::export]]
int main() {
  serialize();
  deserialize();
  
  return 0;
}
// ----------------------------------------------------------------------------