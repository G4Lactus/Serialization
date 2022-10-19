#include <iostream>
#include <memory>
#include <Rcpp.h>

// Creating a std::shared_ptr and returning it to R
// https://stackoverflow.com/questions/26217102/creating-a-stdshared-ptr-object-and-returning-it-to-the-r-side-rcpp

// Rcpp::XPtr is a template smart pointer class, it's parameter is the pointer
// class, not the class it points to.
// So a Rcpp::XPtr<std::shared_ptr<TestClass>> would be a smart pointer to a
// std::shared_ptr<TestClass>*. Note the *, that's the important bit. 
// 
// The answer from @d3coy has pretty much all the information. Rcpp::XPtr is a
// template smart pointer class, it's parameter is the pointee class, not the
// class it points to. So a Rcpp::XPtr<std::shared_ptr<TestClass>> would be a
// smart pointer to a std::shared_ptr<TestClass>*. Note the *, that's the
// important bit.
// When a shared_ptr goes out of scope, if that's the last holder of the raw
// pointer, the raw pointer might get deleted. That's definitely not what you
// want. 
// Instead you can create a raw pointer with new and feed XPtr with that. This
// pointer will get deleted when the garbage colector collects the R object
// undelying the XPtr, which is what you usually want when you deal with
// external pointers.
// I know the current guidelines is to use make_unique and make_shared as much
// as possible instead of new but in this case you need new. The smartness comes
// from XPtr, if you mix it with shared_ptr they will get in the way of each
// other.
// 
// However: the point is that the R side indeed doesn't necessarily require the
// std::shared_ptr. However I'm interfacing with an existing C++ library.
// I need to instantiate a std::shared_ptr<TestClass> of the library (using R)
// and feed in back into the C++ algorithm which expects a
// std::shared_ptr<TestClass>.
// --------------------

class Test {
  public:
    int value;
    
    Test() = delete;
    Test(int value) : value{value} {}
    ~Test() = default;
    
};


// [[Rcpp::export]]
SEXP get_Test_obj() {
  std::shared_ptr<Test> s_ptr = std::make_shared<Test>(5);
  Rcpp::XPtr<Test> x_ptr(s_ptr.get(), true);
  return x_ptr;
}


// -----------------------------------------------------------------
class TestClass {
public:
  int value;
  TestClass(int initial_val): value(initial_val) {}
  
};

class TestClassContainer {
public:
  std::shared_ptr<TestClass> test_class_obj;
  TestClassContainer(): test_class_obj(std::make_shared<TestClass>()) {} 
};

//[[Rcpp::export]]
SEXP get_test_obj() {
  Rcpp::XPtr<TestClassContainer> ptr(new TestClassContainer(), true);
  return ptr;
};


//[[Rcpp::export]]
SEXP do_something_with_shared_ptr_testclass(SEXP test_container_obj) {
  Rcpp::XPtr<ResourceContainer> test_ptr(test_container_obj);
  ExternalLib::do_function(test_container_obj->test_class_obj);
};

/*** R
test_container <- get_test_obj()
do_something_with_shared_ptr_testclass(test_container)
*/