#ifndef UTXO_MANAGER_H
#define UTXO_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "transaction.h"
using namespace std;

struct UTXO {
    string tx_id;
    int index;
    double amount;
    string owner;
};

class UTXOManager {
private:
    map<pair<string, int>, UTXO> utxo_set;

public:
    UTXOManager() = default;

    void add_utxo(const string& tx_id, int index, double amount, const string& owner);
    void remove_utxo(const string& tx_id, int index);
    bool exists(const string& tx_id, int index) const;
    double get_amount(const string& tx_id, int index) const;
    string get_owner(const string& tx_id, int index) const;
    double get_balance(const string& owner) const;
    vector<UTXO> get_utxos_for_owner(const string& owner) const;
    void print_all() const;
};

#endif