//Link to Boost
#define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE basic_test
#include <string>
#include "lindaTuples.h"
//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>
#
template <typename T0, typename... Ts>
std::ostream &operator<<(std::ostream &s, std::variant<T0, Ts...> const &v)
{
    std::visit([&](auto &&arg) { s << arg; }, v);
    return s;
}

void printTuple(const Tuple &tuple)
{
    std::cout << "Tuple = {";
    for (auto &i : tuple)
    {
        std::cout << i << ',';
    }
    std::cout << "\b}\n";
}

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_SUITE(RegexTupleTesting_IncorrectCases)

BOOST_AUTO_TEST_CASE(TestEmptyRegex)
{
    BOOST_CHECK_THROW(LindaRegex testRegex(""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestAllTypesWithOnlyType)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("int"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("string"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestAllTypesWithOnlyTypeAndColon)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("int:"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("string:"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestAllTypesWithNothingAfterSampleOperator)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("string:<"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestIntForNonsenseConditions)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<a"), std::invalid_argument); //wrong comparison
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<>"), std::invalid_argument); //nonsense operator
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<1a"), std::invalid_argument); //check if regex parses whole argument
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<-"), std::invalid_argument); //lone unary operator
}

BOOST_AUTO_TEST_CASE(TestFloatForNonsenseConditions){
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<a"),std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<>"),std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<1a"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<-"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestFloatForProhibitedEqualityOperator){
    BOOST_CHECK_THROW(LindaRegex testRegex("float:==2"),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestIntComparisonToFloat){
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<2.5"),std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(RegexTupleTesting_CorrectCases)

BOOST_AUTO_TEST_CASE(TestStringForNonsenseConditions){
    //since there is nothing in Linda standard which would forbid the user from sending strings like "<>==", the regex should handle such cases normally
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("string:<a"));
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("string:<>"));
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("string:<1a")); 
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("string:<-")); 
}

BOOST_AUTO_TEST_CASE(TestAllTypesWithNoConditions)
{
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegexInt("int:*"));
    LindaRegex testRegexInt("int:*");
    BOOST_CHECK(testRegexInt.operator_ == ANY);

    BOOST_REQUIRE_NO_THROW(LindaRegex testRegexFloat("float:*"));
    LindaRegex testRegexFloat("float:*");
    BOOST_CHECK(testRegexFloat.operator_ == ANY);

    BOOST_REQUIRE_NO_THROW(LindaRegex testRegexString("string:*"));
    LindaRegex testRegexString("string:*");
    BOOST_CHECK(testRegexString.operator_ == ANY);
}

BOOST_AUTO_TEST_CASE(TestFloatComparisonToInt){
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("float:<=2"));
    LindaRegex testRegex("float:<=2");
    BOOST_CHECK(testRegex.operator_==LE);
    BOOST_CHECK(std::get<float>(testRegex.value_)==2);
}

BOOST_AUTO_TEST_CASE(TestIntComparisonsAndVariables){
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("int:<2"));
    LindaRegex testRegex("int:==2");
    BOOST_CHECK(testRegex.operator_==EQ);
    BOOST_CHECK(std::get<int>(testRegex.value_)==2);
}

BOOST_AUTO_TEST_CASE(TestFloatComparisonsAndVariables){
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("float:<1.5"));
    LindaRegex testRegex("float:<1.5");
    BOOST_CHECK(testRegex.operator_==LT);
    BOOST_CHECK(std::get<float>(testRegex.value_)==1.5f);
}

BOOST_AUTO_TEST_CASE(TestStringComparisonsAndVariables){
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("string:>test"));
    LindaRegex testRegex("string:>test");
    BOOST_CHECK(testRegex.operator_==GT);
    BOOST_CHECK(std::get<std::string>(testRegex.value_)=="test");
}

BOOST_AUTO_TEST_SUITE_END()