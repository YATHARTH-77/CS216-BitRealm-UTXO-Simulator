#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <vector>
#include <string>
#include <set>
#include "../transaction/transaction.h"
#include "../utxo/utxo_manager.h"
using namespace std;

class Mempool {
private:
    vector<Transaction> transactions;
    set<pair<string, int>> spent_utxos;
    int max_size;

    void evict_lowest_fee(const UTXOManager& utxoMgr);
    double calculate_fee(const Transaction& tx, const UTXOManager& utxoMgr) const;

public:
    Mempool(int size = 50) : max_size(size) {}

    pair<bool, string> add_transaction(const Transaction& tx, const UTXOManager& utxoMgr);
    void remove_transaction(const string& tx_id);
    vector<Transaction> get_top_transactions(int n, const UTXOManager& utxoMgr) const;
    bool is_spent(const string& tx_id, int index) const;
    void clear();
    void print_pool() const;
    int size() const { return transactions.size(); }
};

#endif