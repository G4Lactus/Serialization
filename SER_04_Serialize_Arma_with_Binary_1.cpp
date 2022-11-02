// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(Rcereal)]]
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/list.hpp>
#include <cereal/access.hpp>
#include <RcppArmadillo.h>

namespace arma {

template<class Archive, class eT>
typename std::enable_if<cereal::traits::is_output_serializable<cereal::BinaryData<eT>, Archive>::value, void>::type
  save( Archive & ar, const arma::Mat<eT>& m ) {
    arma::uword n_rows = m.n_rows;
    arma::uword n_cols = m.n_cols;
    ar( n_rows );
    ar( n_cols );
    ar( cereal::binary_data(
        reinterpret_cast< void * const >( const_cast< eT* >( m.memptr() ) ),
        static_cast< std::size_t >( n_rows * n_cols * sizeof( eT ) ) ) );
  }

template<class Archive, class eT>
typename std::enable_if<cereal::traits::is_input_serializable<cereal::BinaryData<eT>, Archive>::value, void>::type
  load( Archive & ar, arma::Mat<eT>& m ) {
    arma::uword n_rows;
    arma::uword n_cols;
    ar( n_rows );
    ar( n_cols );
    
    m.resize( n_rows, n_cols );
    
    ar( cereal::binary_data(
        reinterpret_cast< void * const >( const_cast< eT* >( m.memptr() ) ),
        static_cast< std::size_t >( n_rows * n_cols * sizeof( eT ) ) ) );
  }

}

// [[Rcpp::export]]
int main() {
  {
    arma::mat amat = arma::randn(5, 10);
    std::ofstream os("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(amat);    
  }
  
  {
    arma::mat bmat;
    std::ifstream is("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::BinaryInputArchive iarchive(is);
    iarchive(bmat);
    bmat.print();
  }

  

  return 0;
}