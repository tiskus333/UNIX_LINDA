#include "lindaTuples.h"

std::deque<Tuple>::iterator LindaTuples::match(const RegexTuple &str) {
  return tuples_.begin();
}
void LindaTuples::parseInput() {}

bool LindaTuples::output(const Tuple &tuple) {
  tuples_.push_back(tuple);
  return true;
}
Tuple LindaTuples::input(const RegexTuple &tuple) { return Tuple{"testowy"}; }
Tuple LindaTuples::read(const RegexTuple &tuple) { return Tuple{"testowy"}; }