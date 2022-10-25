// SER_03: Standard Library Support
// -------------------------------------------------------
// cereal supports almost all containers and classes found in C++ STL.
// Therefore, include the proper header file, s.a., 
// #include <cereal/types/xxxx.hpp>
// where x stands for containers, s.a., vector
// 
// If you don't include the appropriate type, you will receive compile time
// errors about cereal being unable to find an appropriate serialization
// function (static assertion failure).
// ------------------------------------

// [[Rcpp::depends(Rcereal)]]
#include <iostream>

// data types for cereal serialization we are going to simulate
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/atomic.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/forward_list.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/stack.hpp>
// ... check the remaining ones out


// doing serialization into JSON format
#include <cereal/archives/json.hpp>

#include <Rcpp.h>


class STL_User_Class
{
public:
  STL_User_Class() = default;
  ~STL_User_Class() = default;
  
  void fillDataContainer() {
    fillDataMap();
    fillArray();
    fillVector();
    fillDeque();
    fillList();
    fillForwardList();
    fillSet();
    fillMultiSet();
    fillMultiMap();
    fillUnorderedMap();
    fillStack();
    fillQueue();
    fillPriorityQueue();
    return;
  }

  void fillDataMap()
  {
    // fill map with key value pairs
    data_map = {
      {"real", { {1.0f, 0},
                 {2.2f, 0},
                 {3.3f, 0} }},
      {"imaginary", { {0, -1.0f},
                      {0, -2.9932f},
                      {0, -3.5f} }}
    };
    return;
  }

  std::vector<double> filledVector() {
    std::vector<double> vec;
    for (std::size_t l{0}; l < 10; ++l) {
      vec.emplace_back(R::norm_rand());
    }
    return vec;
  }

  void fillVector()
  {
    data_vector = filledVector();
    return;
  }

  void fillList()
  {
    for (std::size_t l{0}; l < 25; ++l) {
      data_list_of_vecs.emplace_back(filledVector());
    }
    return;
  }

  void fillDeque()
  {
    data_deque = {7, 15, 16, 8};
    data_deque.push_back(13);
    data_deque.push_front(25);
  }

  void fillArray()
  {
    data_array.fill(42);
    str_array = {"Larry", "Moe", "Curly", "Frank"};
  }

  void fillForwardList()
  {
    data_forward_list.assign({1, 2, 3});
    data_forward_list.assign(5, 42); // assing 5x 42
    data_forward_list.assign(data_forward_list.begin(), data_forward_list.end());
    data_forward_list.push_front(62);
    data_forward_list.pop_front();
  }

  void fillSet()
  {
    string_set.insert("first");
    string_set.insert("second");
    string_set.insert("third");
    string_set.insert("fourth");
    string_set.insert("first"); // duplicate, won't be added
  }

  void fillMultiSet()
  {
    data_multiset.insert(42);
    data_multiset.insert(32);
    data_multiset.insert(62);
    data_multiset.insert(22);
    data_multiset.insert(52);
  }

  void fillMultiMap()
  {
    data_multimap.insert(std::pair<int, int>(1, 42));
    data_multimap.insert(std::pair<int, double>(1, 42.42));
  }

  void fillUnorderedMap()
  {
    data_unorderd_map["Hans"] = 10;
    data_unorderd_map["Wurst"] = 42;
    data_unorderd_map["Nervt!"] = 0;
  }

  void fillStack()
  {
    data_stack.push(21);
    data_stack.push(22);
    data_stack.push(23);
    data_stack.push(24);
  }

  void fillQueue()
  {
    data_queue.emplace("Cat");
    data_queue.emplace("Dog");
    data_queue.emplace("Whale");
  }

  void fillPriorityQueue()
  {
    data_priority_queue.push(19);
    data_priority_queue.push(18);
    data_priority_queue.push(17);
  }

  // some public data
  // ------------------------------
  int public_int{4};
  double public_double{4.24};
  std::string public_string{"HI!"};
  bool public_bool{true};
  std::vector<double> public_vector = filledVector();

private:
  // our data types
  // ------------------------------
  int someInt{42};
  double someDouble{42.42};
  std::string someStr{"Hans Wurst"};  
  std::array<double, 25> data_array;
  std::array<std::string, 4> str_array;
  std::vector<double> data_vector;
  std::deque<int> data_deque;
  std::list<std::vector<double>> data_list_of_vecs;
  std::forward_list<double> data_forward_list;
  std::map<std::string, std::vector<std::complex<float>>> data_map;
  std::set<std::string> string_set;
  std::multimap<int, int> data_multimap;
  std::multiset<int, std::greater<int>> data_multiset;
  std::unordered_map<std::string, double> data_unorderd_map;
  std::stack<double> data_stack;
  std::queue<std::string> data_queue;
  std::priority_queue<int> data_priority_queue;
  // ------------------------------

  
  // friend
  // --------------------------------
  friend class cereal::access;
  
  
  // let cereal know what to archive
  // NOTE: YOU HAVE TO LIST THE VARIABLES IN THE ORDER OF OCCURRENCE, OTHERWISE
  //       YOU END IN AN INFINITY LOOP, RAM CORRUPTION AND FATAL ERROR
  // --------------------------------
  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      // --------------------------      
      CEREAL_NVP(public_int),
      CEREAL_NVP(public_double),
      CEREAL_NVP(public_string),
      CEREAL_NVP(public_bool),
      CEREAL_NVP(public_vector),
      // --------------------------
      CEREAL_NVP(someInt),
      CEREAL_NVP(someDouble),
      CEREAL_NVP(someStr),
      CEREAL_NVP(data_array),
      CEREAL_NVP(str_array),
      CEREAL_NVP(data_vector),
      CEREAL_NVP(data_deque),
      CEREAL_NVP(data_list_of_vecs),
      CEREAL_NVP(data_forward_list),
      CEREAL_NVP(data_map),
      CEREAL_NVP(string_set),
      CEREAL_NVP(data_multimap),
      CEREAL_NVP(data_multiset),
      CEREAL_NVP(data_unorderd_map),
      CEREAL_NVP(data_stack),
      CEREAL_NVP(data_queue),
      CEREAL_NVP(data_priority_queue)
      // --------------------------
    );
  }

};


void perform_Serialization() {
  // create container for serial but this time we stream the archived data to
  // the console, no file is created (demo purpose)
  cereal::JSONOutputArchive output(std::cout); 
  
  // create class instance to create data
  STL_User_Class stl_user_class{};
  stl_user_class.fillDataContainer();
  
  // fill container with data
  output(cereal::make_nvp("best data ever", stl_user_class));
  return;
}


// [[Rcpp::export]]
int main()
{
  perform_Serialization();
  
  return 0;
}

