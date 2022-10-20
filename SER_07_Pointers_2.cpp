// Deferring (cereal::defer) pointer serialization
// -------------------------------------------------
// Cereal performs serialization by performing a recursive depth-first traversal
// through the serialization functions it processes.
// In the case of pointers, it will explore the pointer, serializes its
// contents, and then proceed.
// If that pointer then contains another pointer, the recursion continues.

#include <iostream>
#include <fstream>

// [[Rcpp::depends(Rcereal)]]
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>

#include <Rcpp.h>


struct MyNode
{
  int node_id;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(node_id);
  }
};


struct MyEdge
{
  std::shared_ptr<MyNode> connection;
  int some_value;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::defer(connection), some_value);
  }

};


struct MyGraphStructure
{
  int some_random_data;
  std::vector<MyNode> nodes;
  std::vector<MyEdge> edges;

  template<class Archive>
  void serialize(Archive& archive)
  {
    // because of the deferment, ensure all nodes are serialized before any
    // connection pointers to those nodes in any edge are serialized.
    archive(some_random_data, edges, nodes);

    // we have to explicitly inform the archive when it is safe to serialize
    // the deferred data - this should only be called once
    archive.serializeDeferments();
  }

};


void simulate_deferring_smart_ptrs() {

  { // serialize
    Rcpp::Rcout << "Serialize begin!" << std::endl;
    std::ofstream os("Backend/myGraph.bin", std::ios::binary);
    cereal::BinaryOutputArchive ar(os);
    MyGraphStructure myGraph1;
    ar(myGraph1);
    Rcpp::Rcout << "Serialize finish!" << std::endl;
  }

  { // de-serialize
    Rcpp::Rcout << "Deserialize begin!" << std::endl;
    std::ifstream is("Backend/myGraph.bin");
    cereal::BinaryInputArchive ar(is);

    MyGraphStructure mg;
    ar(mg);
    Rcpp::Rcout << mg.some_random_data << std::endl; // some garbage value
    Rcpp::Rcout << "Deserialize begin!" << std::endl;
  }

  return;
}

// [[Rcpp::export]]
int main() {
  simulate_deferring_smart_ptrs();
  return 0;
}