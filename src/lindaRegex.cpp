#include "lindaRegex.h"
#include <regex>
#include <string>
#include <stdexcept>

void LindaRegex::parse(const std::string &pattern)
{
  std::regex correct_format_rgx(
      "((int:)(((<|<=|>|>=|==)([+-]?\\d+))|\\*))|((float:)(((<|<=|>|>=)([+-]?"
      "\\d+\\.?\\d*))|\\*))|((string:)(((<|<=|>|>=|==)(.+))|\\*))");
  std::regex op_rgx("<=|>=|==|<|>|\\*");
  std::smatch base_match;
  if (!std::regex_match(pattern, base_match, correct_format_rgx))
    throw std::invalid_argument("Incorrect pattern");

  std::regex_search(pattern, base_match, op_rgx);
  std::string type_s = base_match.prefix();
  std::string operand = base_match[base_match.size() - 1];
  std::string value_s = base_match.suffix();
  parseOperand(operand);

  if (operator_ != ANY)
  {
    if (type_s == "string:")
      value_ = value_s;
    else if (type_s == "int:")
      value_ = stoi(value_s);
    else if (type_s == "float:")
      value_ = stof(value_s);
  }
  else
  {
    if (type_s == "string:")
      value_ = "";
    else if (type_s == "int:")
      value_ = 0;
    else if (type_s == "float:")
      value_ = 0.0f;
  }
}

void LindaRegex::parseOperand(const std::string &pattern)
{
  for (const auto &o : RegexOperatorsMap)
  {
    auto [key, value] = o;
    if (pattern == value)
      operator_ = key;
  }
}

LindaRegex::LindaRegex(const std::string &pattern)
{
  try
  {
    parse(pattern);
  }
  catch (std::invalid_argument &e)
  {
    throw e;
  }
}