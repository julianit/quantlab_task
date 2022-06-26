#pragma once

#include "Trade.h"

class StreamParser
{
public:
	StreamParser() = default;
	virtual ~StreamParser() = default;

	virtual [[nodiscard]] data_ptr_t Parse(const std::string& line) = 0;
};

class StreamParserTrade : public StreamParser
{
public:
	[[nodiscard]] data_ptr_t Parse(const std::string& line) override;
};