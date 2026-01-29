#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct TxInput {
    string prev_tx;
    int index;
    string owner;

    bool operator<(const TxInput &other) const {
        if (prev_tx != other.prev_tx)
            return prev_tx < other.prev_tx;
        return index < other.index;
    }
};

struct TxOutput {
    double amount;
    string address;
};

struct Transaction {
    string tx_id;
    vector<TxInput> inputs;
    vector<TxOutput> outputs;

    void print() const {
        cout << "TXID: " << tx_id << "\n";
        for (const auto& in : inputs) {
            cout << "  Input: " << in.prev_tx << ":" << in.index << " (" << in.owner << ")\n";
        }
        for (const auto& out : outputs) {
            cout << "  Output: " << out.amount << " -> " << out.address << "\n";
        }
    }
};

#endif