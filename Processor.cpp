#include "Trade.h"
#include "Processor.h"


#include <ranges>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <format>

constexpr std::string_view CSV_DELIMITER{ "," };
constexpr size_t CHUNK_SIZE{ 10 };
constexpr size_t MAX_LINE_SIZE{ 128 };
constexpr int NUMERIC_BASE{ 10 };

trade_ptr_t Parse(std::string& trade_as_str)
{
	trade_ptr_t trade = std::make_unique<Trade>();
	size_t index{ 0 };
	for (const auto& word : std::views::split(trade_as_str, CSV_DELIMITER))
	{
		std::string_view member(word.begin(), word.end());
		switch (index)
		{
		case 0:
		{
			trade->timestamp = std::strtoull(member.data(), nullptr, NUMERIC_BASE);
			break;
		}
		case 1:
		{
			trade->symbol = member;
			break;
		}
		case 2:
		{
			trade->quantity = std::strtoull(member.data(), nullptr, NUMERIC_BASE);
			break;
		}
		case 3:
		{
			trade->price = std::strtoull(member.data(), nullptr, NUMERIC_BASE);
			break;
		}
		default:
			// Report not processed columns
			std::cout << "Unhandled types:" << member << std::endl;
			break;
		}
		++index;
	}

	if (trade->timestamp == 0 || trade->symbol.empty() ||
		trade->quantity == 0 || trade->price == 0)
	{
		throw std::invalid_argument("Failed to parse line: " + trade_as_str);
	}

	return trade;
}

trade_vec_ptr_t Next(std::ifstream& stream)
{
	unsigned int read_size{ 0 };
	std::string file_line;
	trade_vec_ptr_t trades_vec;
	for (size_t i = 0; i < CHUNK_SIZE || !stream.eof(); ++i)
	{
		do {
			char chunk[MAX_LINE_SIZE];
			stream.getline(chunk, MAX_LINE_SIZE);
			file_line.append(chunk);
		} while (stream.gcount() == MAX_LINE_SIZE - 1);

		if (file_line.empty())
			continue;

		trades_vec.emplace_back(Parse(file_line));
		file_line.clear();
	}
	return trades_vec;
}

void accumulate(const trade_vec_ptr_t& trade_vec, trade_acc_map_t& TradesResultMap)
{
	for (const auto& rec : trade_vec)
	{
		const auto trade = rec.get();
		auto& record = TradesResultMap[trade->symbol];

		//if there is one trade only the timestamp gap is 0
		if (record.total_amount != 0)
		{
			if (record.max_ts_gap < trade->timestamp - record.previous_ts)
				record.max_ts_gap = trade->timestamp - record.previous_ts;
		}
		record.previous_ts = trade->timestamp;

		record.total_volume += trade->quantity;
		record.total_amount += trade->price * trade->quantity;
		if (record.max_price < trade->price)
			record.max_price = trade->price;
	}
}

bool Process(trade_acc_map_t& TradesResultMap)
{
	std::ifstream stream;
	stream.open("test_input/short_input.csv");
	if (!stream.is_open())
	{
		std::cout << "Failed to open input the stream" << std::endl;
		return false;
	}

	size_t last_read_size{ 0 };
	try {
		do
		{
			trade_vec_ptr_t data_chunk;
			data_chunk = Next(stream);
			accumulate(data_chunk, TradesResultMap);
			last_read_size = data_chunk.size();
		} while (last_read_size == CHUNK_SIZE);
	}
	catch (std::exception& exc)
	{
		std::cout << "Error: " << exc.what() << std::endl;
		return false;
	}
	return true;
}

std::vector<std::string> Compile(trade_acc_map_t& TradesResultMap)
{
	std::vector<std::string> result;
	std::string line;
	for (auto it = TradesResultMap.begin(); it != TradesResultMap.end(); ++it)
	{
		const auto& [symbol, trade] = *it;
		line = std::format("{},{},{},{},{}",
			symbol,
			trade.max_ts_gap,
			trade.total_volume,
			trade.total_amount / trade.total_volume, //Weigthed Average Price
			trade.max_price);
		result.push_back(line);
	}
	return result;
}
