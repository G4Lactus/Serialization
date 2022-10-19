// Tread Safety
// ----------------------
// cereal in multithreaded environment:
// 1. ensure that archives by only accessed by on thread at a time
// 2. if you will be accessing separate archives simultaneously, ensure that
//    CEREAL_THREAD_SAFE is defined and non-zero before any cereal headers are
//    included, or modify its default value in
//    <cereal/macros.hpp>
//
// before including any cereal header file
#define CEREAL_THREAD_SAFE 1
// the macros forces cereal to lock on certain global objs during polymorphism
// and versioning serialization.
// NOTE: the macro must be defined before any cereal headers.
// Alternatively, modify the default value for the macro <cereal/macros.hpp>

// now include your cereal headers
// [[Rcpp::depends(Rcereal)]]
#include <cereal/cereal.hpp>
#include <Rcpp.h>

// code .. code .. code
