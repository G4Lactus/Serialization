// Serialization Functions
// ----------------------------------------
// supported types:
// - single functions: serialize
// - split load/save pairs: load, save
// 
// 
// Types of Serialization functions
// ---------------------------------
// Serialization functions can either be internal or external.
// Functionality can either be in a single serialize function, or split into
// load and save functions. 
//
// Unlike Boost, there is no need to explicitly tell cereal that it needs to use
// the split load-save pair.
// cereal will pick whichever is present and give a compile time error if it
// cannot disambiguate a single serialization method.

// NOTE: 
// - save functions are const or take const reference, o.w., cereal throws
//   a static assertion if it detects a non const save function.
//
// - external serialization functions should be part of the same namespace 


// Let's check out how to construct the data types to be functionable with 
// cereal
// ----------------

// ----------------------------------------
// Internal serialize function
struct MyClass
{
  int x, y, z;
  
  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(x, y, z);
  }
  
};
// ----------------------------------------


// ----------------------------------------
// Internal split save/load functions
struct MyClass
{
  int x, y, z;
  
  template<class Archive>
  void save(Archive& archive) const
  {
    archive(x, y, z);
  }
  
  template<class Archive>
  void load(Archive& archive)
  {
    archive(x, y, z);
  }
};
// ----------------------------------------


// ----------------------------------------
// External serialize function
struct MyClass
{
  int x, y, z;
};

template<class Archive>
void serialize(Archive& archive, MyClass& m)
{
  archive(m.x, m.y, m.z);
}
// ----------------------------------------


// ----------------------------------------
// External split/load functions
struct MyClass
{
  int x, y, z;
};

template<class Archive>
void save(Archive& archive, MyClass const& m)
{
  archive(m.x, m.y, m.z);
}

template<class Archive>
void load(Archive& archive, MyClass& m)
{
  archive(m.x, m.y, m.z);
}
// ----------------------------------------

