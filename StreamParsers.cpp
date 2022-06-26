#include "StreamParsers.h"
#include "Trade.h"

#include <iostream>
#include <ranges>
#include <stdexcept>


constexpr std::string_view CSV_DELIMITER{ "," };
constexpr int NUMERIC_BASE{ 10 };

data_ptr_t StreamParserTrade::Parse(const std::string& trade_as_str)
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
			std::cout << "Unhandled columns:" << member << std::endl;
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