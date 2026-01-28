#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <vector>
#include <string>
#include <set>
#include <map>
#include "transaction.h"
#include "utxo_manager.h"

class Mempool {
private:
    std::vector<Transaction> transactions;
    std::set<std::pair<std::string, int>> spent_utxos; // Tracks inputs used in mempool
    int max_size;

    void evict_lowest_fee(const UTXOManager& utxoMgr);
    double calculate_fee(const Transaction& tx, const UTXOManager& utxoMgr) const;

public:
    Mempool(int size = 50) : max_size(size) {}

    // Returns {success, message}
    std::pair<bool, std::string> add_transaction(const Transaction& tx, const UTXOManager& utxoMgr);
    
    void remove_transaction(const std::string& tx_id);
    std::vector<Transaction> get_top_transactions(int n, const UTXOManager& utxoMgr) const;
    
    bool is_spent(const std::string& tx_id, int index) const;
    void clear();
    void print_pool() const;
    int size() const { return transactions.size(); }
};

#endif