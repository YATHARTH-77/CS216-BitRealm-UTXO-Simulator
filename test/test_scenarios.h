#ifndef TEST_SCENARIOS_H
#define TEST_SCENARIOS_H

#include "../src/utxo/utxo_manager.h"
#include "../src/mempool/mempool.h"
using namespace std;

void run_tests(UTXOManager& mgr, Mempool& pool);

#endif