# CS216 – Bitcoin Transaction & UTXO Simulator (C++)

## Team Information
**Team Name:**  BitRealm

**Members:**
1. Yatarth Maurya - 240001082
2. Adharsh Gopalakrishnan - 240002004
3. Aditya Rai - 240041002
4. Harshitkumar Singh - 240002027

---

## Overview

This project is a **C++ simulation of Bitcoin’s transaction system** using the **UTXO (Unspent Transaction Output) model**.  
It demonstrates how Bitcoin validates transactions, prevents double spending, manages a mempool, and rewards miners through transaction fees.

The simulator is **local and single-node**.  
No networking, cryptography, or consensus protocols are implemented, as per assignment instructions.

---

## Learning Objectives

- Understand the **UTXO model**
- Implement **Bitcoin-style transaction validation**
- Prevent **double spending**
- Simulate the **transaction lifecycle** (creation → mempool → mining)
- Demonstrate **miner incentives and fee economics**
- Simulate **race attacks using the first-seen rule**

---

## Features Implemented

### 1. UTXO Manager
- Stores UTXOs as `(tx_id, index) → (amount, owner)`
- Add and remove UTXOs
- Compute balance for an address
- Retrieve all UTXOs owned by an address

---

### 2. Transaction Structure & Validation
Each transaction consists of:
- **Inputs**: references to existing UTXOs
- **Outputs**: newly created UTXOs

Validation rules enforced:
- All input UTXOs must exist
- No duplicate inputs in the same transaction
- Sum(inputs) ≥ Sum(outputs)
- No negative output amounts
- No conflict with mempool-spent UTXOs

---

### 3. Mempool Management
- Stores unconfirmed transactions
- Prevents double spending using a `spent_utxos` set
- Enforces the **first-seen rule**
- Fee-based eviction when mempool is full
- Zero-fee transactions are allowed

---

### 4. Mining Simulation
- Selects top transactions by fee
- Updates the UTXO set permanently
- Removes mined transactions from the mempool
- Miner receives total transaction fees via a coinbase UTXO

---

## Design Decisions

### Zero-Fee Transactions
Allowed, consistent with Bitcoin’s transaction rules.

### Double Spending & Race Attacks
- Prevented using mempool conflict detection.
- Once a UTXO is referenced by a transaction, any conflicting transaction is rejected.
- Demonstrates the **first-seen rule**.

### Unconfirmed Chain (Test 10)
- Outputs of unconfirmed transactions **cannot be spent**.
- Child transactions referencing unmined outputs are rejected.
- This design choice is explicitly allowed by the assignment.

---

## Mandatory Test Scenarios Covered

All mandatory test cases specified in the assignment PDF are implemented:

1. Basic valid transaction  
2. Multiple input aggregation  
3. Double-spend in same transaction  
4. Mempool double-spend prevention  
5. Insufficient funds  
6. Negative output amount  
7. Zero-fee transaction  
8. Race attack simulation  
9. Complete mining flow  
10. Unconfirmed chain handling  


