#include "test_scenarios.h"
#include "mining.h"
#include <iostream>
using namespace std;

void run_tests(UTXOManager& mgr, Mempool& pool) {
    cout << "\n=== Running Mandatory Test Scenarios ===\n";

    // Setup Fresh State for Tests
    mgr = UTXOManager(); 
    pool.clear();
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");

    // Test 1: Basic Valid Tx
    cout << "[Test 1] Valid Tx: Alice -> Bob (10 BTC)... ";
    Transaction t1; t1.tx_id = "t1";
    t1.inputs = {{"genesis", 0, "Alice"}};
    t1.outputs = {{10.0, "Bob"}, {39.999, "Alice"}}; // Fee 0.001
    auto res1 = pool.add_transaction(t1, mgr);
    cout << (res1.first ? "PASS" : "FAIL") << "\n";

    // Test 3: Double Spend in Mempool
    cout << "[Test 3] Double Spend (Mempool): Alice -> Charlie using same UTXO... ";
    Transaction t2; t2.tx_id = "t2_bad";
    t2.inputs = {{"genesis", 0, "Alice"}}; // Spent in t1!
    t2.outputs = {{50.0, "Charlie"}};
    auto res2 = pool.add_transaction(t2, mgr);
    cout << (!res2.first ? "PASS (Rejected)" : "FAIL (Accepted!)") << " Msg: " << res2.second << "\n";

    // Test 6: Negative Amount
    cout << "[Test 6] Negative Output... ";
    Transaction t3; t3.tx_id = "t3_neg";
    t3.inputs = {{"genesis", 1, "Bob"}};
    t3.outputs = {{-5.0, "Alice"}};
    auto res3 = pool.add_transaction(t3, mgr);
    cout << (!res3.first ? "PASS (Rejected)" : "FAIL") << "\n";

    // Test 5: Insufficient Funds
    cout << "[Test 5] Insufficient Funds (Bob sends 35, has 30)... ";
    Transaction t4; t4.tx_id = "t4_poor";
    t4.inputs = {{"genesis", 1, "Bob"}};
    t4.outputs = {{35.0, "Alice"}};
    auto res4 = pool.add_transaction(t4, mgr);
    cout << (!res4.first ? "PASS (Rejected)" : "FAIL") << "\n";

    cout << "=== Tests Complete ===\n\n";
    
    // Reset for main menu
    pool.clear();
}