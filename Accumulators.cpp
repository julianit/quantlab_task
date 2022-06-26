#include "Accumulators.h"

void TradeAccumulator::Accumulate(const data_vec_t& data)
{
	for (const auto& rec : data)
	{
		const Trade* trade = static_cast<Trade*>(rec.get());
		auto& record = m_data[trade->symbol];

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