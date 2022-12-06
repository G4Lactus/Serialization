// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(Rcereal)]]
#include <iostream>
#include <fstream>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/access.hpp>
#include <RcppArmadillo.h>


namespace arma {

  template<class Archive>
  void save(Archive & ar, const arma::sp_mat &t){
    ar(t.n_rows, t.n_cols, t.n_nonzero);
    
    for (auto it = t.begin(); it != t.end(); ++it) {
      ar(it.row(), it.col(), *it);
    }
  }

  template<class Archive>
  void load(Archive & ar, arma::sp_mat &t) {
    arma::uword r, c, nz;
    ar(r, c, nz);
    
    t.zeros(r, c);
    double v{};
    while (nz--) {
      ar(--r, --c, v);
      t(r, c) = v;
    }
  }
}



// [[Rcpp::export]]
int main() {
  
  { // Serialize
    arma::mat C(3, 3, arma::fill::randu);
    C(0, 0) = 0;
    C(1, 1) = 0; // example so that a few of the components are u
    C(1, 2) = 0;
    C(2, 0) = 0;
    C(2, 1) = 0;
    arma::sp_mat const spA = arma::sp_mat(C);
    assert(spA.n_nonzero == 4);
    spA.print("spA: ");
    
    std::ofstream os("Backend/Serialize_sparseArma.bin", std::ios::binary);
    cereal::PortableBinaryOutputArchive oarchive(os);
    oarchive(spA);
  }
  
  // .... put put put ... 
  
  { // Deserialize
    arma::sp_mat spB;
    
    std::ifstream is("Backend/Serialize_sparseArma.bin", std::ios::binary);
    cereal::PortableBinaryInputArchive iarchive(is);
    iarchive(spB);
    
    spB.print();
  }
  
  return 0;
}