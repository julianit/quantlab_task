#pragma once

//bool Process(trade_acc_map_t& TradesResultMap, std::string filepath);
//std::vector<std::string> Compile(trade_acc_map_t& TradesResultMap);

class StreamReader;
class Accumulator;

class Processor
{
public:
	bool Run(StreamReader& stream, Accumulator& acc);
};