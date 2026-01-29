#include "mempool.h"
#include "validation.h" 
#include <algorithm>
#include <iostream>
using namespace std;

double Mempool::calculate_fee(const Transaction& tx, const UTXOManager& utxoMgr) const {
    double input_sum = 0, output_sum = 0;
    for (const auto& in : tx.inputs) input_sum += utxoMgr.get_amount(in.prev_tx, in.index);
    for (const auto& out : tx.outputs) output_sum += out.amount;
    return input_sum - output_sum;
}

pair<bool, string> Mempool::add_transaction(const Transaction& tx, const UTXOManager& utxoMgr) {
    if (transactions.size() >= max_size) {
        evict_lowest_fee(utxoMgr);
    }

    ValidationResult res = validate_transaction(tx, utxoMgr, *this);
    if (!res.success) return {false, res.message};

    transactions.push_back(tx);
    for (const auto& in : tx.inputs) {
        spent_utxos.insert({in.prev_tx, in.index});
    }
    return {true, "Transaction added to mempool."};
}

void Mempool::remove_transaction(const string& tx_id) {
    auto it = find_if(transactions.begin(), transactions.end(),
                           [&](const Transaction& t) { return t.tx_id == tx_id; });
    if (it != transactions.end()) {
        for (const auto& in : it->inputs) spent_utxos.erase({in.prev_tx, in.index});
        transactions.erase(it);
    }
}

vector<Transaction> Mempool::get_top_transactions(int n, const UTXOManager& utxoMgr) const {
    vector<Transaction> sorted = transactions;
    sort(sorted.begin(), sorted.end(), [&](const Transaction& a, const Transaction& b) {
        return calculate_fee(a, utxoMgr) > calculate_fee(b, utxoMgr);
    });
    if (n > sorted.size()) n = sorted.size();
    return vector<Transaction>(sorted.begin(), sorted.begin() + n);
}

bool Mempool::is_spent(const string& tx_id, int index) const {
    return spent_utxos.count({tx_id, index}) > 0;
}

void Mempool::evict_lowest_fee(const UTXOManager& utxoMgr) {
    if (transactions.empty()) return;
    auto min = min_element(transactions.begin(), transactions.end(),
        [&](const Transaction& a, const Transaction& b) {
            return calculate_fee(a, utxoMgr) < calculate_fee(b, utxoMgr);
        });
    cout << "Evicting low fee tx: " << min->tx_id << "\n";
    remove_transaction(min->tx_id);
}

void Mempool::clear() {
    transactions.clear();
    spent_utxos.clear();
}

void Mempool::print_pool() const {
    cout << "Mempool (" << transactions.size() << " txs):\n";
    for(const auto& t : transactions) cout << " - " << t.tx_id << "\n";
}