#pragma once

#include "StreamParsers.h"
#include "Trade.h"

#include <fstream>
#include <sstream>

class StreamParser;

class StreamReader
{
public:
	StreamReader(size_t chunk, StreamParser& parser);
	virtual ~StreamReader() { Close(); };

	virtual bool Open() { return false; }
	virtual void Close() {};
	virtual [[nodiscard]] data_vec_t Next() = 0;
	[[nodiscard]] size_t GetChunk()const { return m_chunk; }

protected:
	StreamParser& m_parser;
	size_t m_chunk;
};

class StreamReaderFile : public StreamReader
{
public:
	StreamReaderFile(std::string filePath, size_t chunk, StreamParser& parser);
	bool Open() override;
	void Close() override
	{
		if (m_stream.is_open())
			m_stream.close();
	}
	[[nodiscard]] data_vec_t Next() override;

protected:
	std::ifstream m_stream;
	std::string m_filePath;
};
