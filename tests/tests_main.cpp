//Link to Boost
 #define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
 #define BOOST_TEST_MODULE basic_test
//VERY IMPORTANT - include this last
#include "lindaTuples.h"
#include <boost/test/unit_test.hpp>

template <typename T0, typename... Ts>
std::ostream &operator<<(std::ostream &s, std::variant<T0, Ts...> const &v) {
  std::visit([&](auto &&arg) { s << arg; }, v);
  return s;
}

void printTuple(const Tuple &tuple) {
  std::cout << "Tuple = {";
  for (auto &i : tuple) {
    std::cout << i << ',';
  }
  std::cout << "\b}\n";
}

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE( Test1 )
{
    LindaRegex lr1("int:<2");
    LindaRegex lr2("float:<=-2.1");
    LindaRegex lr3("string:>=kupa");
    RegexTuple rt1{lr1, lr2, lr3};
    Tuple t1{1, 2.0f, "lupa"};
    Tuple t2{1, -3.0f, "lupa"};
    LindaTuples tuple_data;
    tuple_data.output(t1);
    tuple_data.output(t2);
    auto t = tuple_data.read(rt1);
    printTuple(t);
    t = tuple_data.read({{"int:==+1"}, {"float:*"}, {"string:*"}});
    printTuple(t);
}
