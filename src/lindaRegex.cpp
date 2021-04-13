#include "lindaRegex.h"
#include <regex>
// regex to check correct format
// ((int:)(((<|<=|>|>=|==)([+-]?\d+))|\*))|((float:)(((<|<=|>|>=|==)([+-]?\d+\.?\d*))|\*))|((string:)(((<|<=|>|>=|==)(.+))|\*))

void LindaRegex::parse(const std::string &pattern) {
  std::regex correct_format_rgx(
      "((int:)(((<|<=|>|>=|==)([+-]?\\d+))|\\*))|((float:)(((<|<=|>|>=)([+-]?"
      "\\d+\\.?\\d*))|\\*))|((string:)(((<|<=|>|>=|==)(.+))|\\*))");
  std::regex op_rgx("<=|>=|==|<|>");
  std::smatch base_match;
  if (!std::regex_match(pattern, base_match, correct_format_rgx))
    throw("INCORRECT PATTERN!");

  std::regex_search(pattern, base_match, op_rgx);
  std::string type_s = base_match.prefix();
  std::string operand = base_match[base_match.size() - 1];
  std::string value_s = base_match.suffix();
  parseOperand(operand);
  std::cout << type_s << "," << operand << "," << value_s << std::endl;
  if (type_s == "string:") {
    type_ = 0;
    value_ = value_s;
  } else if (type_s == "int:") {
    type_ = 1;
    value_ = stoi(value_s);
  } else if (type_s == "float:") {
    type_ = 2;
    value_ = stof(value_s);
  } else
    throw("INCORECT TYPE");
}
void LindaRegex::parseOperand(const std::string &pattern) {
  for (const auto &o : RegexOperatorsMap) {
    auto [key, value] = o;
    if (pattern == value) {
      operator_ = key;
    }
  }
}
LindaRegex::LindaRegex(const std::string &pattern) { parse(pattern); }