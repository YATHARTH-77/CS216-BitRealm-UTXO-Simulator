#ifndef MINING_H
#define MINING_H

#include <string>
#include "../utxo/utxo_manager.h"
#include "../mempool/mempool.h"
using namespace std;

void mine_block(const string& miner_address, Mempool& mempool, UTXOManager& utxo_manager, int num_txs = 5);

#endif