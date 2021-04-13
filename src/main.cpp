#include "lindaTuples.h"

int main() {
  try {
    LindaRegex lr1("int:<+2");
    LindaRegex lr2("float:<=-2.1");
    LindaRegex lr3("string:<kupa");
    RegexTuple t1{lr1, lr2, lr3};
    Tuple t2{2, 3, "abc"};

  } catch (const char *ex) {
    std::cout << ex << std::endl;
  }
}