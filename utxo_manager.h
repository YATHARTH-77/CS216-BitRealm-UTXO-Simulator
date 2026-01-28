#ifndef UTXO_MANAGER_H
#define UTXO_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "transaction.h"

struct UTXO {
    std::string tx_id;
    int index;
    double amount;
    std::string owner;
};

class UTXOManager {
private:
    // Key: {tx_id, index}
    std::map<std::pair<std::string, int>, UTXO> utxo_set;

public:
    UTXOManager() = default;

    void add_utxo(const std::string& tx_id, int index, double amount, const std::string& owner);
    void remove_utxo(const std::string& tx_id, int index);
    bool exists(const std::string& tx_id, int index) const;
    
    // Getters for Validation
    double get_amount(const std::string& tx_id, int index) const;
    std::string get_owner(const std::string& tx_id, int index) const;
    
    double get_balance(const std::string& owner) const;
    std::vector<UTXO> get_utxos_for_owner(const std::string& owner) const;
    void print_all() const;
};

#endif