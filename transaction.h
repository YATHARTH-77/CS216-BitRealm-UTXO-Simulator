#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>

struct TxInput {
    std::string prev_tx;
    int index;
    std::string owner; // Simulates signature

    // Needed for using TxInput as a key in std::set/map
    bool operator<(const TxInput &other) const {
        if (prev_tx != other.prev_tx)
            return prev_tx < other.prev_tx;
        return index < other.index;
    }
};

struct TxOutput {
    double amount;
    std::string address;
};

struct Transaction {
    std::string tx_id;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;

    // Helper to calculate fee (Needs UTXO context, so logic is external)
    void print() const {
        std::cout << "TXID: " << tx_id << "\n";
        for (const auto& in : inputs) {
            std::cout << "  Input: " << in.prev_tx << ":" << in.index << " (" << in.owner << ")\n";
        }
        for (const auto& out : outputs) {
            std::cout << "  Output: " << out.amount << " -> " << out.address << "\n";
        }
    }
};

#endif