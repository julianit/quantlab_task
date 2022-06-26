#pragma once

#include <fstream>
#include <string>
#include <vector>

class Accumulator;
class TradeAccumulator;

class OutputTradesFile
{
public:
	~OutputTradesFile();
	uint64_t FileCsv(std::string filepath, Accumulator& acc);

private:
	uint64_t FormatCsvFile(std::string filepath, Accumulator& acc);
	std::ofstream m_ofile;
	
};
