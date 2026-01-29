#ifndef MINING_H
#define MINING_H

#include <string>
#include "utxo_manager.h"
#include "mempool.h"
using namespace std;

void mine_block(const string& miner_address, Mempool& mempool, UTXOManager& utxo_manager, int num_txs = 5);

#endif