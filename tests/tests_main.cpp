//Link to Boost
#define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE basic_test
#include <string>
#include <thread>
#include "lindaTuples.h"
#include "lindaSpace.h"
//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_SUITE(LindaRegexTesting_IncorrectCases)

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
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<a"), std::invalid_argument);  //wrong comparison
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<>"), std::invalid_argument);  //nonsense operator
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<1a"), std::invalid_argument); //check if regex parses whole argument
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<-"), std::invalid_argument);  //lone unary operator
}

BOOST_AUTO_TEST_CASE(TestFloatForNonsenseConditions)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<a"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<>"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<1a"), std::invalid_argument);
    BOOST_CHECK_THROW(LindaRegex testRegex("float:<-"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestFloatForProhibitedEqualityOperator)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("float:==2"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(TestIntComparisonToFloat)
{
    BOOST_CHECK_THROW(LindaRegex testRegex("int:<2.5"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(LindaRegexTesting_CorrectCases)

BOOST_AUTO_TEST_CASE(TestStringForNonsenseConditions)
{
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

BOOST_AUTO_TEST_CASE(TestFloatComparisonToInt)
{
    BOOST_CHECK_NO_THROW(LindaRegex testRegex("float:<=2"));
    LindaRegex testRegex("float:<=2");
    BOOST_CHECK(testRegex.operator_ == LE);
    BOOST_CHECK(std::get<float>(testRegex.value_) == 2);
}

BOOST_AUTO_TEST_CASE(TestIntComparisonsAndVariables)
{
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("int:<2"));
    LindaRegex testRegex("int:==2");
    BOOST_CHECK(testRegex.operator_ == EQ);
    BOOST_CHECK(std::get<int>(testRegex.value_) == 2);
}

BOOST_AUTO_TEST_CASE(TestFloatComparisonsAndVariables)
{
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("float:<1.5"));
    LindaRegex testRegex("float:<1.5");
    BOOST_CHECK(testRegex.operator_ == LT);
    BOOST_CHECK(std::get<float>(testRegex.value_) == 1.5f);
}

BOOST_AUTO_TEST_CASE(TestStringComparisonsAndVariables)
{
    BOOST_REQUIRE_NO_THROW(LindaRegex testRegex("string:>test"));
    LindaRegex testRegex("string:>test");
    BOOST_CHECK(testRegex.operator_ == GT);
    BOOST_CHECK(std::get<std::string>(testRegex.value_) == "test");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(LindaTuplesTesting)

BOOST_AUTO_TEST_CASE(TestOutputAndReadEmptyTupleWithEmptyRegex)
{
    LindaTuples tuples;
    Tuple testTuple{};
    BOOST_REQUIRE_THROW(tuples.output(testTuple),std::invalid_argument);
    RegexTuple testRegexTuple{};
    auto testRead = tuples.read(testRegexTuple);
    BOOST_CHECK(std::get<std::string>(testRead[0])=="waiting");
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)0);
}

BOOST_AUTO_TEST_CASE(TestOutputAndInputEmptyTupleWithEmptyRegex)
{
LindaTuples tuples;
    Tuple testTuple{};
    BOOST_REQUIRE_THROW(tuples.output(testTuple),std::invalid_argument);
    RegexTuple testRegexTuple{};
    auto testRead = tuples.input(testRegexTuple);
    BOOST_CHECK(std::get<std::string>(testRead[0])=="waiting");
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)0);
}

BOOST_AUTO_TEST_CASE(TestOutputAndReadTupleWithMatchingRegex)
{
    LindaTuples tuples;
    Tuple testTuple{1};
    tuples.output(testTuple);
    LindaRegex regex("int:*");
    RegexTuple testRegexTuple{regex};
    auto testRead = tuples.input(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<int>(testRead[0]) == 1);
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)0);
}

BOOST_AUTO_TEST_CASE(TestOutputAndInputTupleWithMatchingRegex)
{
    LindaTuples tuples;
    Tuple testTuple{1};
    tuples.output(testTuple);
    LindaRegex regex("int:*");
    RegexTuple testRegexTuple{regex};
    auto testRead = tuples.read(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<int>(testRead[0]) == 1);
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)1);
}

BOOST_AUTO_TEST_CASE(TestOutputMultipleTuples)
{
    LindaTuples tuples;
    Tuple testTuple1{1};
    Tuple testTuple2{2};
    tuples.output(testTuple1);
    tuples.output(testTuple2);
    BOOST_CHECK(tuples.getTuplesAmount()==(size_t)2);
}

BOOST_AUTO_TEST_CASE(TestReadMultipleOutputsWithOnlyOneMatchingRegex){
    LindaTuples tuples;
    Tuple testTuple1{1};
    Tuple testTuple2{3};
    LindaRegex regex("int:==3");
    RegexTuple testRegexTuple{regex};
    tuples.output(testTuple1);
    tuples.output(testTuple2);
    auto testRead = tuples.read(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<int>(testRead[0]) == 3);
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)2);
}

BOOST_AUTO_TEST_CASE(TestInputMultipleOutputsWithMultipleOutputsMatchingRegex){
//Linda standard does not precisely state what to do when there are multiple tuples fitting the regex, so we just pick the one that entered the memory earliest
    LindaTuples tuples;
    Tuple testTuple1{1,"first"};
    Tuple testTuple2{1,"second"};
    LindaRegex regex1("int:==1");
    LindaRegex regex2("string:==first");
    RegexTuple testRegexTuple{regex1, regex2};
    tuples.output(testTuple1);
    tuples.output(testTuple2);
    auto testRead = tuples.input(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<int>(testRead[0]) == 1);
    BOOST_CHECK(std::get<std::string>(testRead[1]) == "first");
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)1);
}

BOOST_AUTO_TEST_CASE(TestOutputFittingRegexOnlyPartly_DifferentTypes){
    LindaTuples tuples;
    Tuple testTuple{1,"second"};
    LindaRegex regex1("int:==1");
    LindaRegex regex2("string:==first");
    RegexTuple testRegexTuple{regex1, regex2};
    tuples.output(testTuple);
    auto testRead = tuples.input(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<std::string>(testRead[0])=="waiting");
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)1);
}

BOOST_AUTO_TEST_CASE(TestOutputFittingRegexOnlyPartly_SameTypes){
    LindaTuples tuples;
    Tuple testTuple{1,3};
    LindaRegex regex1("int:==1");
    LindaRegex regex2("int:==2");
    RegexTuple testRegexTuple{regex1, regex2};
    tuples.output(testTuple);
    auto testRead = tuples.input(testRegexTuple);
    BOOST_REQUIRE(!testRead.empty());
    BOOST_CHECK(std::get<std::string>(testRead[0])=="waiting");
    BOOST_CHECK(tuples.getTuplesAmount() == (size_t)1);
}

BOOST_AUTO_TEST_CASE(TestWriteremove){

    LindaSpace ls(true);
    Tuple testTuple{1};
    LindaRegex regex1("int:==1");
    RegexTuple testRegexTuple{regex1};

    ls.write(testTuple);
    // ls.read(testRegexTuple);
}

// BOOST_AUTO_TEST_CASE(TestLindaSpace, *boost::unit_test::timeout(30))
// {
//     LindaSpace ls(true);
//     Tuple testTuple{1};
//     LindaRegex regex1("int:==1");
//     RegexTuple testRegexTuple{regex1};
//     std::thread t3(&LindaSpace::remove, &ls, testRegexTuple);
//     std::thread t2(&LindaSpace::write, &ls, testTuple);
//     sleep(2);
//     std::thread t1(&LindaSpace::read, &ls, testRegexTuple);
//     std::thread t4(&LindaSpace::write, &ls, testTuple);

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();
// }

BOOST_AUTO_TEST_SUITE_END()