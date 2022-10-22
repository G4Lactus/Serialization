// Serialization Archives
// -------------------------
// Support for 3 types:
// - binary
// - XML
// - JSON
// 
// Archives can be used by including <cereal/archives/xxxx.hpp> for the 
// particular type.
// They must be registered before any type of polymorphism.
//
// Archives decide how to output or interpret data that is being serialized.
// Cereal archives operate on either std::ostream or std::istream objs.
// These can be files, in-memory streams, or even things like standard in/out.
// 
// Archives are only guaranteed to have finished their contents when they are
// destroyed, so some archives (e.g., XML) will not output anything until
// their destruction.
// 
// cereal was not designed to be a robust long term storage solution - it is
// your responsibility to ensure version compatibility between saved and loaded
// cereal archives.
// 
// It is recommended that you use the same version of cereal for both loading
// and saving data.
// ----------------------


// Binary Data
// ----------------------
#include <cereal/archives/binary.hpp>
// produce compact bit level representation of data and is not human readable.
// It is a good choice, when computers are at both ends of writing/reading the
// serialization.
// The binary archive is also the fastest archive that comes with cereal.
// Binary archives will ignore name-value pairs and only serialize the values.

#include <cereal/archives/portable_binary.hpp>
// When using a binary archive and a file stream (std::fstream), remember to
// specify the binary flag (std::ios::binary) when constructing the stream.



// XML
// ----------------------
#include <cereal/archives/xml.hpp>
// XML is a human readable format and should not be used in situations where
// serialized data "size" is critical.
// Unlike the binary archive, which outputs its data incrementally as
// serialization functions are called, the XML archive builds a tree in memory
// and only outputs upon destruction of the archive.
//
// XML archives utilize name-value pairs to give human readable names to its
// output. If a name-value pair is not supplied, it will automatically generate
// an enumerated name.
// 
// XML archives do not need to output size metadata for variable sized
// containers since the number of children of a node can be queried when
// loading the data. This means that it is possible to add extra data manually
// to an XML archive before loading it:



// JSON
// ----------------------
#include <cereal/archives/json.hpp>
// Human readable format, that should not be used in situations where serialized
// data "size" is critical. They support name-value pairs.
// Note:
// - dynamically sized containers (e.g. std::vector) are serialized as JSON
//   arrays
//   
// - fixed sized containers (e.g. std::array) are serialized as JSON objects
// 
// This distinction is important as it allows you to hand insert data into
// variable sized containers in a JSON file by inserting elements into
// an array, whereas you cannot insert new data into objects. You can still hand
// edit values in objects, but you cannot append or deduct data from them.


// Adding More Archives
// ----------------------
// Special functions are called immediately before and after every type is
// serialized. They are called prologue and epilogue.
// 
// The default cases for these functions look like:
template<class Archive, class T>
void prologue(Archive&, T const &) {}

