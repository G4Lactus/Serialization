#include <Rcpp.h>
// [[Rcpp::depends(BH)]]
#include <boost/integer/common_factor.hpp>

// [[Rcpp::export]]
int computeGCD(int a, int b) {
  return boost::integer::gcd(a, b);
}

// [[Rcpp::export]]
int computeLCM(int a, int b) {
  return boost::integer::lcm(a, b);
}

/*** R
computeGCD(6, 15)
computeLCM(96, 484)


*/