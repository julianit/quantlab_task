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

BOOST_AUTO_TEST_CASE(SimpleTestCase)
{
	trade_acc_map_t TradesResultMap;
	BOOST_TEST(Process(TradesResultMap));
	
	std::vector<std::string> result = Compile(TradesResultMap);
	BOOST_REQUIRE(result.size() == expected_result.size());
	
	for ( size_t i = 0; i< result.size(); ++i)
	{
		BOOST_REQUIRE(result[i].compare(expected_result[i]) == 0);
	}

}