#ifndef LINDA_TUPLES_H
#define LINDA_TUPLES_H

#include "lindaRegex.h"
#include <deque>

using RegexTuple = std::vector<LindaRegex>;
using Tuple = std::vector<std::variant<std::string, int, float>>;
class LindaTuples {

private:
  std::deque<Tuple>::iterator match(const RegexTuple &str);
  void parseInput();

public:
  std::deque<Tuple> tuples_;
  bool output(const Tuple &tuple);
  Tuple input(const RegexTuple &tuple);
  Tuple read(const RegexTuple &tuple);
};
#endif