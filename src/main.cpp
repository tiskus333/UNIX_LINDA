#include "lindaTuples.h"

int main() {
  try {
    char data[10 * 128];
    memset(data, 0, sizeof(data));
    LindaTuples l;
    l.output({0, 1.f, "kupa", -2});
    l.output({"dupa", -2, "papuga"});
    // l.output({});
    l.output({""});
    l.serialize(data, 0);
    l.deserialize(data);
    std::cout << std::endl << (int)l.getTuplesAmount() << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
