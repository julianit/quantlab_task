// quantlab_task.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Trade.h"
#include "Processor.h"

int main()
{
	trade_acc_map_t TradesResultMap;
	return Process(TradesResultMap) ? 0 : 1;
}