# From now on we focus on (de-) serialization of objects
# --------------------------------------------------------------------
# We use cereal which is a C++11 library for serialization of objects
# https://uscilab.github.io/cereal/
# 
# It is a header only library and takes arbitrary data type and reversibly
# turns them into different representations, such as compact binary encodings,
# XML, or JSON.
# It was designed as fast, lightweight, and easy to extend, also it has no
# external dependencies.
# 
# Features are full support of polymorphism and inheritance. However, it is
# a lightweight and performs not the same level of object tracking as Boost.
# As a consequence raw pointers and references are not supported, but smart 
# pointers (std::shared_ptr and std::unique_ptr).
# 
# cereal uses features from C++11 and requires a compliant compiler. It also
# supports g++ 4.7.3, clang++ 3.3, ad MSVC2013 and newer.
# 
# cereal offers more flexible ways of writing serialization functions such as
# moving them outside of class definitions or splitting them into separate load
# and save functions.
# 
# You can read all about that in the serialization functions section of the
# documentation.
# cereal can also support class versioning, private serialization methods, and
# even classes that don’t support default construction.
# 
# You can serialize primitive data types and nearly every type in the standard
# library without needing to write anything yourself.
#
# Available archives:
# -------------------
# Currently (2022) cereal supports three basic archive types:
# - binary
# - XML
# - JSON
# 
# To use them in C++ code include the following:
# ----------------------------------------------
# #include <cereal/archives/binary.hpp>
# #include <cereal/archives/portable_binary.hpp>
# #include <cereal/archives/xml.hpp>
# #include <cereal/archives/json.hpp>
# -------------------------------------------------------
library(Rcpp)
sourceCpp("SER_01_Getting_Started_with_Serialization_and_Cereal.cpp")

# create backend folder
path_to_backend <- paste0(getwd(), "/Backend/") 
if (!dir.exists(path_to_backend)) {
  dir.create(path_to_backend)
}

# invoke main() to create invoke serialization and deserialization of a class.
main()

