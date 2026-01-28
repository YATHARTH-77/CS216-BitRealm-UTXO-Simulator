#include <iostream>
#include <ctime>
#include "utxo_manager.h"
#include "mempool.h"
#include "mining.h"
#include "test_scenarios.h"

// Helper to generate IDs
std::string get_unique_id() {
    static int counter = 0;
    return "tx_" + std::to_string(std::time(nullptr)) + "_" + std::to_string(counter++);
}

int main() {
    UTXOManager utxoManager;
    Mempool mempool;

    // Initialize Genesis Block (Section 5.1)
    utxoManager.add_utxo("genesis", 0, 50.0, "Alice");
    utxoManager.add_utxo("genesis", 1, 30.0, "Bob");
    utxoManager.add_utxo("genesis", 2, 20.0, "Charlie");
    utxoManager.add_utxo("genesis", 3, 10.0, "David");
    utxoManager.add_utxo("genesis", 4, 5.0, "Eve");

    while (true) {
        std::cout << "\n=== Bitcoin Transaction Simulator ===\n";
        std::cout << "1. Create new transaction\n";
        std::cout << "2. View UTXO set\n";
        std::cout << "3. View mempool\n";
        std::cout << "4. Mine block\n";
        std::cout << "5. Run test scenarios\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string sender, recipient;
            double amount;
            std::cout << "Enter sender: "; std::cin >> sender;
            std::cout << "Enter recipient: "; std::cin >> recipient;
            std::cout << "Enter amount: "; std::cin >> amount;

            // Simple Auto-Input Selection (Greedy)
            std::vector<UTXO> funds = utxoManager.get_utxos_for_owner(sender);
            std::vector<TxInput> inputs;
            double in_sum = 0;
            
            for (const auto& u : funds) {
                // Check if already used in mempool
                if (mempool.is_spent(u.tx_id, u.index)) continue;

                inputs.push_back({u.tx_id, u.index, sender});
                in_sum += u.amount;
                if (in_sum >= amount) break;
            }

            if (in_sum < amount) {
                std::cout << "Error: Insufficient funds or funds pending in mempool.\n";
                continue;
            }

            Transaction tx;
            tx.tx_id = get_unique_id();
            tx.inputs = inputs;
            tx.outputs.push_back({amount, recipient});
            
            // Change Logic
            double fee = 0.001; // Fixed fee for demo
            double change = in_sum - amount - fee;
            if (change > 0) tx.outputs.push_back({change, sender});

            auto res = mempool.add_transaction(tx, utxoManager);
            std::cout << "Result: " << res.second << " (ID: " << tx.tx_id << ")\n";

        } else if (choice == 2) {
            utxoManager.print_all();
        } else if (choice == 3) {
            mempool.print_pool();
        } else if (choice == 4) {
            std::string miner;
            std::cout << "Enter miner name: "; std::cin >> miner;
            mine_block(miner, mempool, utxoManager);
        } else if (choice == 5) {
            run_tests(utxoManager, mempool);
        } else if (choice == 6) {
            break;
        }
    }
    return 0;
}