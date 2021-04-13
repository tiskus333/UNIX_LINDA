#ifndef LINDA_TUPLES_H
#define LINDA_TUPLES_H

#include "lindaRegex.h"
#include <list>
#include <optional>

using RegexTuple = std::vector<LindaRegex>;
using Tuple = std::vector<std::variant<std::string, int, float>>;
class LindaTuples {

private:
  std::optional<std::list<Tuple>::iterator> match(const RegexTuple &str);
  void parseInput();

public:
  std::list<Tuple> tuples_;
  bool output(const Tuple &tuple);
  Tuple input(const RegexTuple &tuple);
  Tuple read(const RegexTuple &tuple);
};
#endif