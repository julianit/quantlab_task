#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>


#include "Trade.h"
#include "Processor.h"

#include <string>
#include <vector>
#include <ranges>

std::vector<std::string> expected_result = {
	{"aaa,5787,40,1161,1222"},
	{"aab,6103,69,810,907"},
	{"aac,3081,41,559,638"}
};

BOOST_AUTO_TEST_CASE(ShortFileTestCase)
{
	BOOST_TEST_MESSAGE("Test Short input file");

	trade_acc_map_t TradesResultMap;
	BOOST_TEST(Process(TradesResultMap, "test_input/short_input.csv"));
	
	std::vector<std::string> result = Compile(TradesResultMap);
	BOOST_REQUIRE(result.size() == expected_result.size());
	
	for ( size_t i = 0; i< result.size(); ++i)
	{
		BOOST_REQUIRE(result[i].compare(expected_result[i]) == 0);
	}
	BOOST_TEST_MESSAGE("Test Short input file: Successful");
}

BOOST_AUTO_TEST_CASE(LongFileTestCase)
{
	BOOST_TEST_MESSAGE("Test Long input file");

	trade_acc_map_t TradesResultMap;
	BOOST_TEST(Process(TradesResultMap, "test_input/input.csv"));

	std::vector<std::string> result = Compile(TradesResultMap);
	const size_t EXPECTED_RESULT_LINES = 343;
	BOOST_REQUIRE(result.size() == EXPECTED_RESULT_LINES);

	BOOST_TEST_MESSAGE("Test Long input file: successful");
}