#include "lindaTuples.h"

std::optional<std::list<Tuple>::iterator>

LindaTuples::match(const RegexTuple &tuple)
{
  bool found = true;
  for (auto it = tuples_.begin(); it != tuples_.end(); ++it)
  {
    if (!((*it).size() == tuple.size()))
      continue;
    found = true;
    for (size_t i = 0; i < (*it).size(); ++i)
    {
      if ((*it)[i].index() != tuple[i].value_.index())
      {
        found = false;
        break;
      }
      switch (tuple[i].operator_)
      {
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

bool LindaTuples::output(const Tuple &tuple)
{
  tuples_.push_back(tuple);
  return true;
}

Tuple LindaTuples::input(const RegexTuple &tuple)
{
  auto iter = match(tuple);
  if (iter.has_value())
  {
    Tuple ret_tuple = *iter.value();
    tuples_.erase(iter.value());
    return ret_tuple;
  }
  else
    return Tuple{"waiting"};
}

Tuple LindaTuples::read(const RegexTuple &tuple)
{
  auto ret = match(tuple);
  if (ret.has_value())
    return *ret.value();
  else
    return Tuple{"waiting"};
}

size_t LindaTuples::getTuplesAmount()
{
  return tuples_.size();
}

bool LindaTuples::deserialize(const char *address)
{
  return true;
}

bool LindaTuples::serialize(const char *address, size_t max_size)
{
  std::stringstream serialized_stream;
  for (const auto &t : tuples_)
  {
    serialized_stream << t.size() << ':';
    for (const auto &v : t)
    {
      serialized_stream << v.index() << ':' << v;
    }
    std::string serialized_tuple = serialized_stream.str();
    if (serialized_tuple.size() >= 512)
      throw(std::invalid_argument("MAX TUPLE SIZE IS EXCEEDED"));

    // write to shared memory
  }
  return true;
}
