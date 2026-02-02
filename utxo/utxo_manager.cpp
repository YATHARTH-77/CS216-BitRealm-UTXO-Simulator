#include "utxo_manager.h"
using namespace std;

void UTXOManager::add_utxo(const string& tx_id, int index, double amount, const string& owner) {
    pair<string, int> key = {tx_id, index};
    utxo_set[key] = {tx_id, index, amount, owner};
}

void UTXOManager::remove_utxo(const string& tx_id, int index) {
    pair<string, int> key = {tx_id, index};
    utxo_set.erase(key);
}

bool UTXOManager::exists(const string& tx_id, int index) const {
    pair<string, int> key = {tx_id, index};
    return utxo_set.find(key) != utxo_set.end();
}

double UTXOManager::get_amount(const string& tx_id, int index) const {
    pair<string, int> key = {tx_id, index};
    if (utxo_set.count(key)) return utxo_set.at(key).amount;
    return 0.0;
}

string UTXOManager::get_owner(const string& tx_id, int index) const {
    pair<string, int> key = {tx_id, index};
    if (utxo_set.count(key)) return utxo_set.at(key).owner;
    return "";
}

double UTXOManager::get_balance(const string& owner) const {
    double balance = 0.0;
    for (const auto& pair : utxo_set) {
        if (pair.second.owner == owner) {
            balance += pair.second.amount;
        }
    }
    return balance;
}

vector<UTXO> UTXOManager::get_utxos_for_owner(const string& owner) const {
    vector<UTXO> result;
    for (const auto& pair : utxo_set) {
        if (pair.second.owner == owner) {
            result.push_back(pair.second);
        }
    }
    return result;
}
