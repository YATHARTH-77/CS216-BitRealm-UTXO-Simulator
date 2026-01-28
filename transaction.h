#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>

struct TxInput {
    std::string prev_tx;  // Previous Transaction ID
    int index;            // Output index from that previous tx
    std::string owner;    // Signature/Owner claiming this UTXO
};

struct TxOutput {
    double amount;
    std::string address;  // Recipient
};

struct Transaction {
    std::string tx_id;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;
    
    // Helper to print transaction details
    void print() const {
        std::cout << "  TXID: " << tx_id << "\n";
        std::cout << "    Inputs:\n";
        for(const auto& in : inputs) {
            std::cout << "      - Ref: " << in.prev_tx << ":" << in.index << " (Owner: " << in.owner << ")\n";
        }
        std::cout << "    Outputs:\n";
        for(const auto& out : outputs) {
            std::cout << "      - " << out.amount << " BTC -> " << out.address << "\n";
        }
    }
};

#endif