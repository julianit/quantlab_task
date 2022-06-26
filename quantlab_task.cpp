// Yulian Iliev test for quantlab

#include "Accumulators.h"
#include "Outputs.h"
#include "Processor.h"
#include "StreamReaders.h"
#include "StreamParsers.h"

#include <iostream>

constexpr size_t CHUNK_SIZE_ARG{ 5 };

int main()
{
   std::cout << "Start processing file" << std::endl;

   StreamParserTrade tradeParser;
   StreamReaderFile stream( "test_input/short_input.csv", CHUNK_SIZE_ARG, tradeParser );
   TradeAccumulator acc;
   Processor processor;
   if (!processor.Run(stream, acc))
   {
      std::cout << "Processing file failed to" << std::endl;
      return 1;
   }

   OutputTradesFile output;
   const auto total_lines = output.FileCsv("test_output/output.csv", acc);
   std::cout << "Output file generated: " << total_lines << " lines." << std::endl;
   return 0;
}