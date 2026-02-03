#include "test_scenarios.h"
#include "../src/mining/mining.h"
#include <iostream>
using namespace std;

void run_tests(UTXOManager& mgr, Mempool& pool) {
    cout << "\n=== Running Mandatory Test Scenarios ===\n";

    mgr = UTXOManager();
    pool.clear();

    // Initial UTXOs
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");
    mgr.add_utxo("genesis", 2, 20.0, "Alice");
    mgr.add_utxo("genesis", 3, 10.0, "David");
    mgr.add_utxo("genesis", 4, 5.0, "Eve");

    /* 
       TEST 1: Basic Valid Transaction
       Alice -> Bob (10 BTC) with change + fee
     */
    cout << "[Test 1] Basic Valid Transaction... ";
    Transaction t1;
    t1.tx_id = "t1";
    t1.inputs = {{"genesis", 0, "Alice"}};
    t1.outputs = {{10.0, "Bob"}, {39.999, "Alice"}};
    auto r1 = pool.add_transaction(t1, mgr);
    cout << (r1.first ? "PASS" : "FAIL") << "\n";

    /* 
       TEST 2: Multiple Inputs
       Alice spends 50 + 20 = 70 BTC
     */
    cout << "[Test 2] Multiple Inputs Aggregation... ";
    Transaction t2;
    t2.tx_id = "t2";
    t2.inputs = {
        {"genesis", 0, "Alice"},
        {"genesis", 2, "Alice"}
    };
    t2.outputs = {{60.0, "Bob"}, {9.999, "Alice"}};
    auto r2 = pool.add_transaction(t2, mgr);
    cout << (r2.first ? "PASS" : "FAIL") << "\n";

    pool.clear(); // isolate tests

    /* 
       TEST 3: Double Spend in SAME Transaction
     */
    cout << "[Test 3] Double Spend (Same TX)... ";
    Transaction t3;
    t3.tx_id = "t3";
    t3.inputs = {
        {"genesis", 0, "Alice"},
        {"genesis", 0, "Alice"}
    };
    t3.outputs = {{10.0, "Bob"}};
    auto r3 = pool.add_transaction(t3, mgr);
    cout << (!r3.first ? "PASS (Rejected)" : "FAIL") << "\n";

    /* 
       TEST 4: Double Spend in MEMPOOL
     */
    cout << "[Test 4] Double Spend (Mempool Conflict)... ";
    Transaction t4a, t4b;
    t4a.tx_id = "t4a";
    t4a.inputs = {{"genesis", 0, "Alice"}};
    t4a.outputs = {{10.0, "Bob"}, {39.999, "Alice"}};

    t4b.tx_id = "t4b";
    t4b.inputs = {{"genesis", 0, "Alice"}};
    t4b.outputs = {{10.0, "Charlie"}};

    auto r4a = pool.add_transaction(t4a, mgr);
    auto r4b = pool.add_transaction(t4b, mgr);

    cout << (r4a.first && !r4b.first ? "PASS" : "FAIL") << "\n";

    pool.clear();

    /* 
       TEST 5: Insufficient Funds
     */
    cout << "[Test 5] Insufficient Funds... ";
    Transaction t5;
    t5.tx_id = "t5";
    t5.inputs = {{"genesis", 1, "Bob"}};
    t5.outputs = {{35.0, "Alice"}};
    auto r5 = pool.add_transaction(t5, mgr);
    cout << (!r5.first ? "PASS (Rejected)" : "FAIL") << "\n";

    /* 
       TEST 6: Negative Output Amount
     */
    cout << "[Test 6] Negative Output Amount... ";
    Transaction t6;
    t6.tx_id = "t6";
    t6.inputs = {{"genesis", 1, "Bob"}};
    t6.outputs = {{-5.0, "Alice"}};
    auto r6 = pool.add_transaction(t6, mgr);
    cout << (!r6.first ? "PASS (Rejected)" : "FAIL") << "\n";

    /* 
       TEST 7: Zero Fee Transaction
       Inputs == Outputs
     */
    cout << "[Test 7] Zero Fee Transaction... ";
    Transaction t7;
    t7.tx_id = "t7";
    t7.inputs = {{"genesis", 1, "Bob"}};
    t7.outputs = {{30.0, "Alice"}};
    auto r7 = pool.add_transaction(t7, mgr);
    cout << (r7.first ? "PASS" : "FAIL") << "\n";

    pool.clear();

    /* 
       TEST 8: Race Attack (First-Seen Rule)
     */
    cout << "[Test 8] Race Attack Simulation... ";
    Transaction lowFee, highFee;

    lowFee.tx_id = "lowFee";
    lowFee.inputs = {{"genesis", 0, "Alice"}};
    lowFee.outputs = {{49.999, "Bob"}};

    highFee.tx_id = "highFee";
    highFee.inputs = {{"genesis", 0, "Alice"}};
    highFee.outputs = {{40.0, "Charlie"}};

    auto r8a = pool.add_transaction(lowFee, mgr);
    auto r8b = pool.add_transaction(highFee, mgr);

    cout << (r8a.first && !r8b.first ? "PASS" : "FAIL") << "\n";

    pool.clear();

    /* 
       TEST 9: Complete Mining Flow
     */
    cout << "[Test 9] Mining Flow... ";
    Transaction t9;
    t9.tx_id = "t9";
    t9.inputs = {{"genesis", 0, "Alice"}};
    t9.outputs = {{10.0, "Bob"}, {39.999, "Alice"}};
    pool.add_transaction(t9, mgr);

    mine_block("Miner1", pool, mgr);

    bool utxoSpent = !mgr.exists("genesis", 0);
    bool minerPaid = mgr.get_balance("Miner1") > 0;

    cout << (utxoSpent && minerPaid ? "PASS" : "FAIL") << "\n";

    /* 
       TEST 10: Unconfirmed Chain Spend
       Bob tries to spend unconfirmed output
     */
    cout << "[Test 10] Unconfirmed Chain Spend... ";
    pool.clear();
    mgr = UTXOManager();
    mgr.add_utxo("genesis", 0, 50.0, "Alice");

    Transaction parent, child;
    parent.tx_id = "parent";
    parent.inputs = {{"genesis", 0, "Alice"}};
    parent.outputs = {{10.0, "Bob"}, {39.999, "Alice"}};

    pool.add_transaction(parent, mgr);

    child.tx_id = "child";
    child.inputs = {{"parent", 0, "Bob"}};
    child.outputs = {{10.0, "Charlie"}};

    auto r10 = pool.add_transaction(child, mgr);
    cout << (!r10.first ? "PASS (Rejected)" : "FAIL") << "\n";

    pool.clear();
    cout << "=== All Tests Completed ===\n\n";

    mgr = UTXOManager();
    pool.clear();
    mgr.add_utxo("genesis", 0, 50.0, "Alice");
    mgr.add_utxo("genesis", 1, 30.0, "Bob");
    mgr.add_utxo("genesis", 2, 20.0, "Charlie");
    mgr.add_utxo("genesis", 3, 10.0, "David");
    mgr.add_utxo("genesis", 4, 5.0, "Eve");
    cout << "[INFO] Genesis state restored.\n\n";
}
