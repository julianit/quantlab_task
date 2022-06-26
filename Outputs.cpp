#include "Outputs.h"

#include "Accumulators.h"
#include "Processor.h"

#include <iostream>
#include <iterator>
#include <format>

OutputTradesFile::~OutputTradesFile()
{
	if (m_ofile.is_open())
		m_ofile.close();
}


uint64_t OutputTradesFile::FileCsv(std::string filepath, Accumulator& acc)
{
	return FormatCsvFile(filepath, acc);
}


uint64_t OutputTradesFile::FormatCsvFile(std::string filepath, Accumulator& acc)
{
	const TradeAccumulator* const tradeAcc = dynamic_cast<TradeAccumulator*>(&acc);
	const auto data_v = tradeAcc->GetData();
	std::vector<std::string> report;

	m_ofile.open(filepath);
	if (!m_ofile.is_open())
	{
		std::cout << "Failed to crate output file: " << filepath << std::endl;
		return 0;
	}

	std::string line;
	for (auto it = data_v.begin(); it != data_v.end(); ++it)
	{
		const auto& [symbol, row] = *it;
		line = std::format("{},{},{},{},{}{}",
			symbol,
			row.max_ts_gap,
			row.total_volume,
			row.total_amount / row.total_volume, //Weigthed Average Price
			row.max_price,
			std::ranges::next(it) == data_v.end() ? "" : "\n");
		m_ofile.write(line.c_str(), line.size());
	}
	m_ofile.close();
	return data_v.size();
}