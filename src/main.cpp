#include "lindaTuples.h"
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

int main() {
  try {
    LindaRegex lr1("int:<2");
    LindaRegex lr2("float:<=-2.1");
    LindaRegex lr3("string:>=kupa");
    RegexTuple t1{lr1, lr2, lr3};
    Tuple t2{1, -3.0f, "lupa"};
    LindaTuples tuple_data;
    tuple_data.output(t2);
    auto t = tuple_data.read(t1);
    printTuple(t);

  } catch (const char *ex) {
    std::cout << ex << std::endl;
  }
}