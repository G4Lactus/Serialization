// Pointers
// ---------------------------------
// cereal supports serializing smart pointers but not raw pointers.
// Pointer support: <cereal/types/memory.hpp>
// Pointers to polymorphic types are also supported but detailed elsewhere.
// 
// Cereal needs access to either a default constructor or a specialization of
// cereal::LoadAndConstruct for loading smart pointers.
//
// All smart pointers are supported, i.e.: 
// std::unique_ptr, std::shared_ptr, std::weak_ptr
// cereal takes care that the data are only serialized once.
// 
// For polymorphic objs cereal will also properly handle pointers to the 
// polymorphic objects (e.g., std::unique_ptr<Base> p(new Derived() ).


// Types with no default constructor
// ----------------------------------
// If you want to serialize a pointer to a type that does not have a default
// constructor, or a type that does not allow cereal access to its default
// constructor, you will either need to provide a member static function
// load_and_construct or provide a specialization of cereal::LoadAndConstruct
// for the type.
// If you choose to use the external version, specializing 
// cereal::LoadAndConstruct, cereal will call the static method of this struct,
// load_and_construct.

#include <iostream>
// [[Rcpp::depends(Rcereal)]]
#include <cereal/access.hpp>
#include <Rcpp.h>

struct MyType
{
  MyType(int x);
  int myX;
  
  // define a serialize or save/load pair as you normally would
  template <class Archive>
  void serialize(Archive& ar)
  {
    ar(myX);
  }
  
  // NOTE: at this point we could define load_and_construct internally
  // NOTE: version parameter is optional
  template<class Archive>
  static void load_and_construct(Archive& ar, cereal::construct<MyType>& construct,
                                 std::uint32_t const version)
  {
    int x;
    ar(x);
    construct(x); 
  }
};


// External specialization for LoadAndConstruct
namespace cereal 
{

  template <> struct LoadAndConstruct<MyType>
  {
    // load_construct will be passed the archive that you will be loading from as
    // well as a special construct object that can be used to involve the
    // constructor for your class.
    // 
    // More advanced functionality is available using construct, such as accessing
    // class members, which is detailed in the doxygen docs.
    
    template<class Archive>
    static void load_and_construct(Archive& ar, cereal::construct<MyType>& construct)
    {
      int x;
      ar{x};
      construct(x);
    }    
  };
}


// Deferring pointer serialization
// -----------------------------------
// Cereal performs serialization by performing a recursive depth-first traversal
// through the serialization functions it processes.
// In the case of pointers, it will explore the pointer, serializes its
// contents, and then proceed.
// If that pointer then contains another pointer, the recursion continues.
// 
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

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
  std::vector<MyEdge> edges;
  std::vector<MyNodes> nodes;
  
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

