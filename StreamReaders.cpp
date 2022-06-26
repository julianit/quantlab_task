#include "StreamReaders.h"

#include <iostream>

const unsigned int MAX_LINE_SIZE = 128;

StreamReader::StreamReader(size_t chunk, StreamParser& parser)
	: m_parser(parser), m_chunk(chunk)
{
}


StreamReaderFile::StreamReaderFile(std::string filePath, size_t chunk, StreamParser& parser)
	: StreamReader(chunk, parser),
	m_filePath(filePath)
{
}

bool StreamReaderFile::StreamReaderFile::Open()
{
	m_stream.open(m_filePath);
	if (!m_stream.is_open())
	{
		std::cout << "Failed to open input the stream" << std::endl;
		return false;
	}
	return true;
}

data_vec_t StreamReaderFile::StreamReaderFile::Next()
{
	data_vec_t result;
	std::string fileLine;
	for (unsigned int i = 0; i < m_chunk && !m_stream.eof(); ++i)
	{
		do
		{
			char chunk[MAX_LINE_SIZE];
			m_stream.getline(chunk, MAX_LINE_SIZE);
			fileLine.append(chunk);
		} while (m_stream.gcount() == MAX_LINE_SIZE - 1);

		if (fileLine.empty())
			continue;
		result.emplace_back(m_parser.Parse(fileLine));
		fileLine.clear();
	}
	return result;
}