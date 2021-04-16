#ifndef LINDA_TUPLES_H
#define LINDA_TUPLES_H

#include "lindaRegex.h"
#include "utils.h"
#include <list>
#include <optional>
#include <sstream>

using RegexTuple = std::vector<LindaRegex>;
using Tuple = std::vector<std::variant<std::string, int, float>>;
class LindaTuples {

private:
  std::list<Tuple> tuples_;

  /**
   * @brief
   *
   * @param str
   * @return std::optional<std::list<Tuple>::iterator>
   */
  std::optional<std::list<Tuple>::iterator> match(const RegexTuple &str);

public:
  /**
   * @brief Get the Tuples Amount object
   *
   * @return size_t
   */
  size_t getTuplesAmount();
  /**
   * @brief - writes serialized tuple list to tuplespace
   *
   * @param tuple
   * @return true
   * @return false
   */
  bool output(const Tuple &tuple);
  /**
   * @brief
   *
   * @param tuple
   * @return Tuple
   * @throw
   *
   */
  Tuple input(const RegexTuple &tuple);

  /**
   * @brief
   *
   * @param tuple
   * @return Tuple
   * @throw
   */
  Tuple read(const RegexTuple &tuple);

public:
  /**
   * @brief - saves serialized version of tuple list
   *
   * @param data
   * @param max_size
   * @return true
   * @return false - failed
   */
  bool serialize(char *const data, size_t max_size);

  /**
   * @brief - reads (initializes or updates) list of tuples
   *
   * @param data
   * @return true
   * @throw false - failed
   */
  bool deserialize(const char *const data);
};
#endif
