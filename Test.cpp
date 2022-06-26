#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>


#include "Accumulators.h"
#include "Outputs.h"
#include "Processor.h"
#include "StreamReaders.h"
#include "StreamParsers.h"

const size_t CHUNK_SIZE_ARG{ 5 };
const size_t MAX_LINE_SIZE_TEST{ 128 };

int generate_output(std::string inFile, std::string outFile)
{
	StreamParserTrade tradeParser;
	StreamReaderFile stream(inFile, CHUNK_SIZE_ARG, tradeParser);
	TradeAccumulator acc;
	Processor processor;
	if (processor.Run(stream, acc))
	{
		OutputTradesFile output;
		return output.FileCsv(outFile, acc) > 0;
	}
	return false;
}


#include <string>
#include <vector>
#include <ranges>

std::vector<std::string> expected_result = {
	{"aaa,5787,40,1161,1222"},
	{"aab,6103,69,810,907"},
	{"aac,3081,41,559,638"}
};

class  FileWrapper
{
public:
	bool open(std::string filePath)
	{
		m_istream.open(filePath);
		return m_istream.is_open();
	}

	~FileWrapper()
	{
		if (m_istream.is_open())
			m_istream.close();
	}

	std::ifstream m_istream;
};

BOOST_AUTO_TEST_CASE(ShortFileTestCase)
{
	BOOST_TEST_MESSAGE("Test Short input file");

	const std::string outputFile = "test_output/output_short_test.csv";
	BOOST_TEST(generate_output("test_input/short_input.csv", outputFile));

	FileWrapper resultFile;
	std::string fileLine;
	std::vector<std::string> result;
	
	BOOST_TEST(resultFile.open(outputFile) );
		
	while (!resultFile.m_istream.eof())
	{
		do
		{
			char chunk[MAX_LINE_SIZE_TEST];
			resultFile.m_istream.getline(chunk, MAX_LINE_SIZE_TEST - 1);
			fileLine.append(chunk);
		} while (resultFile.m_istream.gcount() == MAX_LINE_SIZE_TEST - 1);
		result.push_back(fileLine);
		fileLine.clear();
	}

	resultFile.m_istream.close();

	BOOST_REQUIRE(result.size() == expected_result.size());

	for (size_t i = 0; i < result.size(); ++i)
	{
		BOOST_REQUIRE(result[i].compare(expected_result[i]) == 0);
	}

	BOOST_TEST_MESSAGE("Test Short input file: Successful");
}


BOOST_AUTO_TEST_CASE(LongFileTestCase)
{
	BOOST_TEST_MESSAGE("Test Long input file");

	const std::string outputFile = "test_output/output_long_test.csv";
	BOOST_TEST(generate_output("test_input/input.csv", outputFile));

	FileWrapper resultFile;
	std::string fileLine;
	std::vector<std::string> result;

	BOOST_TEST(resultFile.open(outputFile));

	while (!resultFile.m_istream.eof())
	{
		do
		{
			char chunk[MAX_LINE_SIZE_TEST];
			resultFile.m_istream.getline(chunk, MAX_LINE_SIZE_TEST - 1);
			fileLine.append(chunk);
		} while (resultFile.m_istream.gcount() == MAX_LINE_SIZE_TEST - 1);
		result.push_back(fileLine);
		fileLine.clear();
	}

	resultFile.m_istream.close();

	const size_t EXPECTED_RESULT_LINES = 343;
	BOOST_REQUIRE(result.size() == EXPECTED_RESULT_LINES);

	BOOST_TEST_MESSAGE("Test Long input file: successful");
}
