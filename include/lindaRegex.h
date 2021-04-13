#ifndef LINDA_REGEX_H
#define LINDA_REGEX_H

#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

enum RegexOperatorsEnum { ANY, EQ, LT, LE, GT, GE };
static const std::map<RegexOperatorsEnum, const char *> RegexOperatorsMap{
    {ANY, "*"}, {EQ, "=="}, {LT, "<"}, {LE, "<="}, {GT, ">"}, {GE, ">="}};
class LindaRegex {
public:
  int type_ = 0;
  RegexOperatorsEnum operator_ = ANY;
  std::variant<std::string, int, float> value_;
  LindaRegex(const std::string &pattern);
  void parse(const std::string &pattern);
  void parseOperand(const std::string &pattern);
};
#endif