#include "Processor.h"

#include "Accumulators.h"
#include "StreamReaders.h"

#include <iostream>

bool Processor::Run(StreamReader& stream, Accumulator& acc)
{
	if (!stream.Open())
	{
		std::cout << "Failed to open input the stream" << std::endl;
		return false;
	}
	const auto chunk = stream.GetChunk();
	size_t size = 0;
	try {
		do
		{
			data_vec_t data_chunk;
			data_chunk = stream.Next();
			acc.Accumulate(data_chunk);
			size = data_chunk.size();
		} while (size == chunk);
	}
	catch (std::exception& exc)
	{
		std::cout << "Error: " << exc.what() << std::endl;
		return false;
	}
	return true;

}