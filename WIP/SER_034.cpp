// [[Rcpp::depends(Rcereal)]]

#include <cereal/access.hpp>
#include <vector>
#include <cereal/types/vector.hpp>

#include <sstream>
#include <cereal/archives/binary.hpp>

#include <Rcpp.h>

class SerializeNumericMatrix
{
private:
  int nrows;
  int ncols;
  std::vector<double> data;
  
  template<class Archive>
  void serialization(Archive& ar) {
    ar(data, nrows, ncols);
  }
  
public:
  SerializeNumericMatrix(){};
  SerializeNumericMatrix(Rcpp::NumericMatrix x) {
    std::vector<double> y = Rcpp::as<std::vector<double>>(x);
    data = y;
    nrows = x.nrow();
    ncols = x.ncol();
  }
  
  Rcpp::NumericVector numMatrix() {
    Rcpp::NumericVector d = Rcpp::wrap(data);
    d.attr("dim") = Rcpp::Dimension(nrows, ncols);
    return d;
  }
  
  Rcpp::NumericMatrix 
  
};