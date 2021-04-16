#ifndef LINDA_REGEX_H
#define LINDA_REGEX_H

#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum RegexOperatorsEnum { ANY, EQ, LT, LE, GT, GE };
static const std::map<RegexOperatorsEnum, const char *> RegexOperatorsMap{
    {ANY, "*"}, {EQ, "=="}, {LT, "<"}, {LE, "<="}, {GT, ">"}, {GE, ">="}};
class LindaRegex {
public:
  RegexOperatorsEnum operator_ = ANY;
  std::variant<std::string, int, float> value_ = "";
  /**
   * @brief Construct a new Linda Regex object
   *
   * @param pattern
   */
  LindaRegex(const std::string &pattern);
  /**
   * @brief parse
   *
   * @param pattern
   */
  void parse(const std::string &pattern);
  void parseOperand(const std::string &pattern);
};
#endif