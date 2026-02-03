# 🪙 CS216 – Bitcoin Transaction & UTXO Simulator

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

**A comprehensive C++ simulation of Bitcoin's transaction system using the UTXO (Unspent Transaction Output) model**

</div>

---

## 👥 Team Information

| Team Name | **BitRealm** |
|-----------|--------------|

### Team Members

| S.No. | Name | Roll Number |
|-------|------|-------------|
| 1 | Yatharth Maurya | 240001082 |
| 2 | Adharsh Gopalakrishnan | 240002004 |
| 3 | Aditya Rai | 240041002 |
| 4 | Harshitkumar Singh | 240002027 |

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Dependencies & Installation](#-dependencies--installation)
- [How to Run](#-how-to-run)
- [Project Architecture](#-project-architecture)
- [Features Implemented](#-features-implemented)
- [Design Decisions](#-design-decisions)
- [Test Scenarios](#-test-scenarios)
- [Sample Output](#-sample-output)

---

## 🎯 Overview

This project is a **local, single-node C++ simulation** of Bitcoin's transaction system using the **UTXO (Unspent Transaction Output) model**. It demonstrates how Bitcoin:

- ✅ Validates transactions
- ✅ Prevents double spending
- ✅ Manages a mempool of unconfirmed transactions
- ✅ Rewards miners through transaction fees

> **Note:** No networking, cryptography, or consensus protocols are implemented.

### Learning Objectives

- Understand the **UTXO model** and its role in Bitcoin
- Implement **Bitcoin-style transaction validation**
- Prevent **double spending** attacks
- Simulate the **transaction lifecycle** (creation → mempool → mining)
- Demonstrate **miner incentives and fee economics**
- Simulate **race attacks** using the first-seen rule

---

## 📦 Dependencies & Installation

### Prerequisites

| Requirement | Version | Purpose |
|-------------|---------|---------|
| **C++ Compiler** | C++11 or higher | Compilation (g++, clang++, or MSVC) |


### Supported Compilers

- **GCC** (g++) - Linux/macOS/Windows (MinGW)
- **Clang** (clang++) - macOS/Linux
- **MSVC** - Windows (Visual Studio)

### Installation Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/YATHARTH-77/CS216-BitRealm-UTXO-Simulator.git
   cd CS216-BitRealm-UTXO-Simulator
   ```

2. **Verify your compiler is installed**
   ```bash
   g++ --version
   # OR
   clang++ --version
   ```

---

## 🚀 How to Run

### Option 1: Using g++ (Recommended)

**On Linux/macOS:**
```bash
cd src
g++ -std=c++11 -I. main.cpp -o bitcoin_simulator
./bitcoin_simulator
```

**On Windows (Command Prompt/PowerShell):**
```bash
cd src
g++ -std=c++11 -I. main.cpp -o bitcoin_simulator.exe
.\bitcoin_simulator.exe
```



### Option 2: Using Visual Studio (Windows)

1. Open Visual Studio
2. Create a new C++ Console Application project
3. Add all source files from the repository
4. Build and Run (F5)

### Interactive Menu

Once the program is running, you'll see an interactive menu:

```
=== Bitcoin Transaction Simulator ===
1. Create new transaction
2. View UTXO set
3. View mempool
4. Mine block
5. Run test scenarios
6. Exit
Enter choice: 
```

---

##  Project Architecture

```
CS216-BitRealm-UTXO-Simulator/
├── src/
│   ├── main.cpp             # Entry point with interactive menu
│   ├── transaction/         # Transaction data structures
│   │   └── transaction.h    # TxInput, TxOutput, Transaction structs
│   ├── utxo/                # UTXO management module
│   │   ├── utxo_manager.h   # UTXOManager class declaration
│   │   └── utxo_manager.cpp # UTXOManager implementation
│   ├── validation/          # Transaction validation module
│   │   ├── validation.h     # Validation function declarations
│   │   └── validation.cpp   # Validation logic implementation
│   ├── mempool/             # Mempool management module
│   │   ├── mempool.h        # Mempool class declaration
│   │   └── mempool.cpp      # Mempool implementation
│   ├── mining/              # Mining simulation module
│   │   ├── mining.h         # Mining function declarations
│   │   └── mining.cpp       # Mining logic implementation
│   └── test/                # Test scenarios
│       ├── test_scenarios.h # Test function declarations
│       └── test_scenarios.cpp # 10 mandatory test cases
├── Sample_Output.txt        # Example program output
└── README.md                # This file
```

### Module Descriptions

| Module | Description |
|--------|-------------|
| **transaction/** | Defines core data structures (`TxInput`, `TxOutput`, `Transaction`) |
| **utxo/** | Manages the UTXO set with add/remove/query operations |
| **validation/** | Validates transactions against UTXO set and mempool |
| **mempool/** | Stores unconfirmed transactions, prevents double-spending |
| **mining/** | Simulates mining by selecting transactions and updating UTXO set |
| **test/** | Implements all 10 mandatory test scenarios |

---

## ⚙️ Features Implemented

### 1. UTXO Manager
- Stores UTXOs as `(tx_id, index) → (amount, owner)`
- Add and remove UTXOs dynamically
- Compute balance for any address
- Retrieve all UTXOs owned by a specific address

### 2. Transaction Structure & Validation
Each transaction consists of:
- **Inputs**: References to existing UTXOs being spent
- **Outputs**: Newly created UTXOs

**Validation Rules Enforced:**
| Rule | Description |
|------|-------------|
| UTXO Existence | All input UTXOs must exist in the UTXO set |
| No Duplicate Inputs | No duplicate inputs in the same transaction |
| Balance Check | Sum(inputs) ≥ Sum(outputs) |
| Non-Negative Outputs | No negative output amounts allowed |
| Mempool Conflict | No conflict with mempool-spent UTXOs |

### 3. Mempool Management
- Stores unconfirmed transactions in memory
- Prevents double spending using a `spent_utxos` set
- Enforces the **first-seen rule** for conflict resolution
- Fee-based eviction when mempool reaches capacity (default: 50 transactions)
- Zero-fee transactions are allowed

### 4. Mining Simulation
- Selects top transactions by fee (greedy selection)
- Updates the UTXO set permanently after mining
- Removes mined transactions from the mempool
- Creates **coinbase UTXO** for miner with total transaction fees

---

## 🎨 Design Decisions

### Zero-Fee Transactions
✅ **Allowed** — Consistent with Bitcoin's actual transaction rules where zero-fee transactions can be valid.

### Double Spending & Race Attacks
- **Prevention**: Mempool conflict detection using `spent_utxos` set
- **First-Seen Rule**: Once a UTXO is referenced by a transaction, any conflicting transaction is rejected
- This closely simulates Bitcoin's actual behavior against race attacks

### Unconfirmed Chain Spending (Test 10)
- Outputs of unconfirmed transactions **cannot be spent**
- Child transactions referencing unmined outputs are rejected
- This is a deliberate design choice (explicitly allowed by the assignment)

### Fee Calculation
```
Fee = Sum(Input Amounts) - Sum(Output Amounts)
```

---

## 🧪 Test Scenarios

All 10 mandatory test cases from the assignment are implemented:

| Test # | Scenario | Expected Result |
|--------|----------|-----------------|
| 1 | Basic valid transaction | ✅ PASS |
| 2 | Multiple input aggregation | ✅ PASS |
| 3 | Double-spend in same transaction | ❌ Rejected |
| 4 | Mempool double-spend prevention | ❌ Rejected |
| 5 | Insufficient funds | ❌ Rejected |
| 6 | Negative output amount | ❌ Rejected |
| 7 | Zero-fee transaction | ✅ PASS |
| 8 | Race attack simulation | ❌ Second TX Rejected |
| 9 | Complete mining flow | ✅ PASS |
| 10 | Unconfirmed chain handling | ❌ Rejected |

### Running Tests

From the interactive menu, select option **5** to run all test scenarios:
```
Enter choice: 5

=== Running Mandatory Test Scenarios ===
[Test 1] Basic Valid Transaction... PASS
[Test 2] Multiple Inputs Aggregation... PASS
[Test 3] Double Spend (Same TX)... PASS (Rejected)
...
=== All Tests Completed ===
```

---

## 📸 Sample Output

### Viewing Genesis UTXOs
```
--- All UTXOs ---
(genesis, 0) : 50 BTC [Alice]
(genesis, 1) : 30 BTC [Bob]
(genesis, 2) : 20 BTC [Charlie]
(genesis, 3) : 10 BTC [David]
(genesis, 4) : 5 BTC [Eve]
-----------------
```

### Creating a Transaction
```
Enter sender: Alice
Enter recipient: Bob
Enter amount: 15
Result: Transaction added to mempool. (ID: tx_1769966581_0)
```

### Mining a Block
```
Enter miner name: miner1
Block mined! Miner miner1 earned 0.002 BTC
```

### Updated UTXO Set After Mining
```
--- All UTXOs ---
(coinbase_0_miner1, 0) : 0.002 BTC [miner1]
(genesis, 1) : 30 BTC [Bob]
(genesis, 3) : 10 BTC [David]
(genesis, 4) : 5 BTC [Eve]
(tx_1769966581_0, 0) : 15 BTC [Bob]
(tx_1769966581_0, 1) : 34.999 BTC [Alice]
-----------------
```

> For complete sample output, see [`Sample_Output.txt`](./Sample_Output.txt)

---



<div align="center">

**Made with ❤️ by Team BitRealm**

*CS216 - Introduction to Blockchains | IIT Indore*

</div>
