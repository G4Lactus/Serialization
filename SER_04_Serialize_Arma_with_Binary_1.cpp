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
    arma::mat amat1 = arma::randn(5, 10);
    arma::mat amat2 = arma::randn(4, 5);
    arma::mat amat3 = arma::randn(3, 9);
    std::list<arma::mat> lst_amat;
    lst_amat.push_back(amat1);
    lst_amat.push_back(amat2);
    lst_amat.push_back(amat3);
    
    std::ofstream os("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(amat1, lst_amat);    
  }
  
  {
    arma::mat bmat;
    std::list<arma::mat> lst_bmat;
    std::ifstream is("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::BinaryInputArchive iarchive(is);
    iarchive(bmat, lst_bmat);
    bmat.print();
    Rcpp::Rcout << std::endl;
        
    std::list<arma::mat>::iterator it;
    for (it = lst_bmat.begin(); it != lst_bmat.end(); ++it) {
      (*it).print();
      Rcpp::Rcout << std::endl;
    }
  }

  

  return 0;
}