#ifndef MINING_H
#define MINING_H

#include <string>
#include "utxo_manager.h"
#include "mempool.h"

void mine_block(const std::string& miner_address, Mempool& mempool, UTXOManager& utxo_manager, int num_txs = 5);

#endif