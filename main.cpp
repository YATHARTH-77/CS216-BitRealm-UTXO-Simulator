#include <iostream>
#include <ctime>
#include "./utxo/utxo_manager.cpp"
#include "./validation/validation.cpp"
#include "./mempool/mempool.cpp"
#include "./mining/mining.cpp"
#include "./test/test_scenarios.cpp"
using namespace std;

string get_unique_id() {
    static int counter = 0;
    return "tx_" + to_string(time(nullptr)) + "_" + to_string(counter++);
}

int main() {
    UTXOManager utxoManager;
    Mempool mempool;

    utxoManager.add_utxo("genesis", 0, 50.0, "Alice");
    utxoManager.add_utxo("genesis", 1, 30.0, "Bob");
    utxoManager.add_utxo("genesis", 2, 20.0, "Charlie");
    utxoManager.add_utxo("genesis", 3, 10.0, "David");
    utxoManager.add_utxo("genesis", 4, 5.0, "Eve");

    while (true) {
        cout << "\n=== Bitcoin Transaction Simulator ===\n";
        cout << "1. Create new transaction\n";
        cout << "2. View UTXO set\n";
        cout << "3. View mempool\n";
        cout << "4. Mine block\n";
        cout << "5. Run test scenarios\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string sender, recipient;
            double amount;
            cout << "Enter sender: "; cin >> sender;
            cout << "Enter recipient: "; cin >> recipient;
            cout << "Enter amount: "; cin >> amount;

            vector<UTXO> funds = utxoManager.get_utxos_for_owner(sender);
            vector<TxInput> inputs;
            double in_sum = 0;
            
            for (const auto& u : funds) {
                if (mempool.is_spent(u.tx_id, u.index)) continue;
                inputs.push_back({u.tx_id, u.index, sender});
                in_sum += u.amount;
                if (in_sum >= amount) break;
            }

            if (in_sum < amount) {
                cout << "Error: Insufficient funds or funds pending in mempool.\n";
                continue;
            }

            Transaction tx;
            tx.tx_id = get_unique_id();
            tx.inputs = inputs;
            tx.outputs.push_back({amount, recipient});
            
            double fee = 0.001;
            double change = in_sum - amount - fee;
            if (change > 0) tx.outputs.push_back({change, sender});

            auto res = mempool.add_transaction(tx, utxoManager);
            cout << "Result: " << res.second << " (ID: " << tx.tx_id << ")\n";

        } else if (choice == 2) {
            utxoManager.print_all();
        } else if (choice == 3) {
            mempool.print_pool();
        } else if (choice == 4) {
            string miner;
            cout << "Enter miner name: "; cin >> miner;
            mine_block(miner, mempool, utxoManager);
        } else if (choice == 5) {
            run_tests(utxoManager, mempool);
        } else if (choice == 6) {
            break;
        }
    }
    return 0;
}