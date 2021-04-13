#include "lindaTuples.h"

std::optional<std::list<Tuple>::iterator>
LindaTuples::match(const RegexTuple &tuple) {
  bool found = true;
  for (auto it = tuples_.begin(); it != tuples_.end(); ++it) {
    if (!((*it).size() == tuple.size()))
      continue;
    found = true;
    for (size_t i = 0; i < (*it).size(); ++i) {
      if ((*it)[i].index() != tuple[i].value_.index()) {
        found = false;
        break;
      }
      switch (tuple[i].operator_) {
      case EQ:
        if (!((*it)[i] == tuple[i].value_))
          found = false;
        break;
      case GE:
        if (!((*it)[i] >= tuple[i].value_))
          found = false;
        break;
      case GT:
        if (!((*it)[i] > tuple[i].value_))
          found = false;
        break;
      case LE:
        if (!((*it)[i] <= tuple[i].value_))
          found = false;
        break;
      case LT:
        if (!((*it)[i] < tuple[i].value_))
          found = false;
        break;
      case ANY:
        break;
      }
      if (!found)
        break;
    }
    if (found)
      return it;
  }
  return {};
}

bool LindaTuples::output(const Tuple &tuple) {
  tuples_.push_back(tuple);
  return true;
}
Tuple LindaTuples::input(const RegexTuple &tuple) {
  auto iter = match(tuple);
  if (iter.has_value()) {
    Tuple ret_tuple = *iter.value();
    tuples_.erase(iter.value());
    return ret_tuple;
  } else
    return Tuple{"waiting"};
}
Tuple LindaTuples::read(const RegexTuple &tuple) {
  auto ret = match(tuple);
  if (ret.has_value())
    return *ret.value();
  else
    return Tuple{"waiting"};
}