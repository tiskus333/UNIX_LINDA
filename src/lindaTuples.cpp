#include "lindaTuples.h"
#define MAX_TUPLE_SIZE 128
#define MAX_TUPLE_NUMBER 10
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
  if (tuple.size() == 0)
    throw std::invalid_argument("CANNOT WRITE EMPTY TUPLE!");
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

size_t LindaTuples::getTuplesAmount() { return tuples_.size(); }

bool LindaTuples::deserialize(const char *address) {
  tuples_.clear();
  std::variant<std::string, int, float> value;
  char single_tuple[MAX_TUPLE_SIZE];
  for (int i = 0; i < MAX_TUPLE_NUMBER; ++i) {
    memset(single_tuple, 0, sizeof(single_tuple));
    strcpy(single_tuple, address + i * MAX_TUPLE_SIZE);

    std::string str(single_tuple);

    // end if all there are less tha max tuples
    if (str.empty())
      return true;

    std::cout << "PRZECZYTANO: " << str << std::endl;
    Tuple desirialized_tuple;

    std::regex parse_types(":");
    std::smatch base_match;

    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    while ((pos = str.find(delimiter)) != std::string::npos) {
      token = str.substr(0, pos);
      str.erase(0, pos + delimiter.length());

      std::regex_search(token, base_match, parse_types);
      std::string type_s = base_match.prefix();
      std::string value_s = base_match.suffix();
      if (type_s == "0")
        value = value_s;
      else if (type_s == "1")
        value = stoi(value_s);
      else if (type_s == "2")
        value = stof(value_s);
      desirialized_tuple.push_back(value);
    }
    printTuple(desirialized_tuple);
    tuples_.push_back(desirialized_tuple);
  }
  return true;
}

bool LindaTuples::serialize(char *const data, size_t max_size) {
  int offset = 0;
  for (const auto &t : tuples_) {
    if (++offset > MAX_TUPLE_NUMBER)
      break;

    std::stringstream serialized_stream;
    for (const auto &v : t) {
      serialized_stream << v.index() << ':' << v << ';';
    }
    std::string serialized_tuple = serialized_stream.str();
    if (serialized_tuple.size() >= MAX_TUPLE_SIZE)
      throw(std::invalid_argument("MAX TUPLE SIZE IS EXCEEDED"));
    char tuple_char[MAX_TUPLE_SIZE];
    memset(tuple_char, 0, sizeof(tuple_char));
    strncpy(tuple_char, serialized_tuple.c_str(), serialized_tuple.size());
    std::cout << tuple_char << std::endl;
    strcpy(data + offset * MAX_TUPLE_SIZE, tuple_char);
  }
  return true;
}
