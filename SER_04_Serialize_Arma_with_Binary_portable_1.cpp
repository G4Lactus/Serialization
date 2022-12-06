// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(Rcereal)]]
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/list.hpp>
#include <cereal/access.hpp>

#include <RcppArmadillo.h>


namespace arma {

  template<class Archive, class eT>
  typename std::enable_if<cereal::traits::is_output_serializable<cereal::BinaryData<eT>, Archive>::value, void>::type
    save(Archive& ar, const arma::Mat<eT>& m) {
      arma::uword n_rows = m.n_rows;
      arma::uword n_cols = m.n_cols;
      ar( n_rows );
      ar( n_cols );
      ar( cereal::binary_data(
            // static_cast<void* const>( const_cast<eT*>( m.memptr() ) ),
            const_cast<eT*>( m.memptr() ),
            static_cast<std::size_t>( n_rows * n_cols * sizeof(eT) ) 
          )
        );
      return;
  }

  template<class Archive, class eT>
  typename std::enable_if<cereal::traits::is_input_serializable<cereal::BinaryData<eT>, Archive>::value, void>::type
    load(Archive& ar, arma::Mat<eT>& m ) {
      arma::uword n_rows{};
      arma::uword n_cols{};
      ar( n_rows );
      ar( n_cols );
      m.resize( n_rows, n_cols );

      ar( cereal::binary_data(
            // static_cast<void* const>( const_cast<eT*>( m.memptr() ) ),
            const_cast<eT*>( m.memptr() ),            
            static_cast<std::size_t>( n_rows * n_cols * sizeof(eT) ) 
          )
        );
      return;
  }

  // template<class Archive>
  // void save(Archive & ar, const arma::mat &m, unsigned){
  //   arma::uword n_rows = m.n_rows;
  //   arma::uword n_cols = m.n_cols;
  //   ar( n_rows );
  //   ar( n_cols );
  // 
  //   return;    
  // }
  
  // template<class Archive>
  // void load(Archive & ar, arma::sp_mat &t, unsigned) {
  //   arma::uword r, c, nz;
  //   ar(r, c, nz);
  //   
  //   t.zeros(r, c);
  //   double v{};
  //   while (nz--) {
  //     ar(--r, --c, v);
  //     t(r, c) = v;
  //   }
  //   return;
  // }
  
  
  template<class Archive>
  void save(Archive & ar, const arma::sp_mat &t, unsigned){
    ar(t.n_rows, t.n_cols, t.n_nonzero);
    
    for (auto it = t.begin(); it != t.end(); ++it) {
      ar(it.row(), it.col(), *it);
    }
    return;    
  }
  
  template<class Archive>
  void load(Archive & ar, arma::sp_mat &t, unsigned) {
    arma::uword r, c, nz;
    ar(r, c, nz);

    t.zeros(r, c);
    double v{};
    while (nz--) {
      ar(--r, --c, v);
      t(r, c) = v;
    }
    return;
  }
}



// [[Rcpp::export]]
int main() {
  
  Rcpp::Rcout << ":)" << std::endl;
  
  { // Serialize
    arma::mat amat1 = arma::randn(5, 10);
    arma::mat amat2 = arma::randn(4, 5);
    arma::mat amat3 = arma::randn(3, 9);
    
    arma::vec avec1 = arma::randn(5, 1);
    
    arma::mat C(3, 3, arma::fill::randu);
    C(0, 0) = 0;
    C(1, 1) = 0; // example so that a few of the components are u
    C(1, 2) = 0;
    C(2, 0) = 0;
    C(2, 1) = 0;
    arma::sp_mat const spA = arma::sp_mat(C);
    assert(spA.n_nonzero == 4);
    spA.print("spA: ");

    std::list<arma::mat> lst_amat;
    lst_amat.push_back(amat1);
    lst_amat.push_back(amat2);
    lst_amat.push_back(amat3);
    
    Rcpp::Rcout << "Serialization begin." << std::endl;
    std::ofstream os("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::PortableBinaryOutputArchive oarchive(os);
    // cereal::BinaryOutputArchive oarchive(os);
    oarchive(amat1, lst_amat, avec1, spA);
    Rcpp::Rcout << "Serialization finished." << std::endl;
  }
  
  // .... put put put ... 
  
  { // Deserialize
    arma::mat bmat;
    std::list<arma::mat> lst_bmat;
    arma::vec bvec;
    arma::sp_mat sbmat1;

    Rcpp::Rcout << "Deserialization begin." << std::endl;
    std::ifstream is("Backend/Serialize_Arma.bin", std::ios::binary);
    cereal::PortableBinaryInputArchive iarchive(is);
    // cereal::BinaryInputArchive iarchive(is);
    iarchive(bmat, lst_bmat, bvec, sbmat1);
    Rcpp::Rcout << "Deserialization finished." << std::endl;    
    
    bmat.print();
    Rcpp::Rcout << std::endl;
    std::list<arma::mat>::iterator it;
    for (it = lst_bmat.begin(); it != lst_bmat.end(); ++it) {
      (*it).print();
      Rcpp::Rcout << std::endl;
    }
    bvec.print();
    Rcpp::Rcout << std::endl;
    sbmat1.print();
  }

  return 0;
}