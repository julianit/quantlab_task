#pragma once

#include "Trade.h"

#include <map>

class Accumulator
{
public:
	Accumulator() = default;
	virtual ~Accumulator() = default;

	virtual void Accumulate(const data_vec_t& data) = 0;
};

class TradeAccumulator : public Accumulator
{
public:
	~TradeAccumulator() override { m_data.clear(); }

	struct TradeRaw
	{
		std::uint64_t max_ts_gap{ 0 };
		std::uint64_t max_price{ 0 };
		std::uint64_t total_volume{ 0 };
		std::uint64_t total_amount{ 0 };
		std::uint64_t previous_ts{ 0 };
	};
	using trade_acc_map_t = std::map<std::string, TradeRaw>;

	void Accumulate(const data_vec_t& data) override;
	[[nodiscard]] trade_acc_map_t GetData() const { return m_data;	};

private:
	trade_acc_map_t m_data;
};

