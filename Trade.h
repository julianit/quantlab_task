#pragma once

#include <map>
#include <memory>
#include<string> 
#include<vector> 


struct Trade
{
public:
	std::uint64_t timestamp{ 0 };
	std::string symbol;
	std::uint64_t quantity{ 0 };
	std::uint64_t price{ 0 };
};

using trade_ptr_t = std::unique_ptr <Trade>;
using trade_vec_ptr_t = std::vector<trade_ptr_t>;


struct TradeResult
{
	std::uint64_t max_ts_gap{ 0 };
	std::uint64_t max_price{ 0 };
	std::uint64_t total_volume{ 0 };
	std::uint64_t total_amount{ 0 };
	std::uint64_t previous_ts{ 0 };
};

using trade_acc_map_t = std::map<std::string, TradeResult>;