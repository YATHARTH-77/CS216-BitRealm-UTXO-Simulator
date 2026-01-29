#include "mining.h"
#include <vector>
using namespace std;

void mine_block(const string& miner_address, Mempool& mempool, UTXOManager& utxo_manager, int num_txs) {
    vector<Transaction> selected = mempool.get_top_transactions(num_txs, utxo_manager);
    double total_fees = 0.0;

    for (const Transaction& tx : selected) {
        double input_sum = 0.0;
        double output_sum = 0.0;

        for (const auto& in : tx.inputs) input_sum += utxo_manager.get_amount(in.prev_tx, in.index);
        for (const auto& out : tx.outputs) output_sum += out.amount;
        
        double fee = input_sum - output_sum;
        if(fee > 0) total_fees += fee;

        for (const auto& in : tx.inputs) {
            utxo_manager.remove_utxo(in.prev_tx, in.index);
        }

        for (size_t i = 0; i < tx.outputs.size(); i++) {
            utxo_manager.add_utxo(tx.tx_id, (int)i, tx.outputs[i].amount, tx.outputs[i].address);
        }

        mempool.remove_transaction(tx.tx_id);
    }

    static int block_height = 0;
    string coinbase_id = "coinbase_" + to_string(block_height++) + "_" + miner_address;
    
    if (total_fees > 0) {
        utxo_manager.add_utxo(coinbase_id, 0, total_fees, miner_address);
        cout << "Block mined! Miner " << miner_address << " earned " << total_fees << " BTC\n";
    } else {
        cout << "Block mined (No fees collected).\n";
    }
}