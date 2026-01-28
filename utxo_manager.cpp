#include "utxo_manager.h"

void UTXOManager::add_utxo(const std::string& tx_id, int index, double amount, const std::string& owner) {
    std::pair<std::string, int> key = {tx_id, index};
    utxo_set[key] = {tx_id, index, amount, owner};
}

void UTXOManager::remove_utxo(const std::string& tx_id, int index) {
    std::pair<std::string, int> key = {tx_id, index};
    utxo_set.erase(key);
}

bool UTXOManager::exists(const std::string& tx_id, int index) const {
    std::pair<std::string, int> key = {tx_id, index};
    return utxo_set.find(key) != utxo_set.end();
}

double UTXOManager::get_amount(const std::string& tx_id, int index) const {
    std::pair<std::string, int> key = {tx_id, index};
    if (utxo_set.count(key)) return utxo_set.at(key).amount;
    return 0.0;
}

std::string UTXOManager::get_owner(const std::string& tx_id, int index) const {
    std::pair<std::string, int> key = {tx_id, index};
    if (utxo_set.count(key)) return utxo_set.at(key).owner;
    return "";
}

double UTXOManager::get_balance(const std::string& owner) const {
    double balance = 0.0;
    for (const auto& pair : utxo_set) {
        if (pair.second.owner == owner) {
            balance += pair.second.amount;
        }
    }
    return balance;
}

std::vector<UTXO> UTXOManager::get_utxos_for_owner(const std::string& owner) const {
    std::vector<UTXO> result;
    for (const auto& pair : utxo_set) {
        if (pair.second.owner == owner) {
            result.push_back(pair.second);
        }
    }
    return result;
}

void UTXOManager::print_all() const {
    std::cout << "\n--- All UTXOs ---\n";
    for (const auto& pair : utxo_set) {
        const auto& u = pair.second;
        std::cout << "(" << u.tx_id << ", " << u.index << ") : " 
                  << u.amount << " BTC [" << u.owner << "]\n";
    }
    std::cout << "-----------------\n";
}