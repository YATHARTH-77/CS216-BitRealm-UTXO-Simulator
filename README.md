# CS216 – Bitcoin Transaction & UTXO Simulator (C++)

## Team Information
**Team Name:** <Your BitRealm>  
**Members:**
1. Yatarth Maurya - 240001082
2. Adharsh Gopalakrishnan - 240002004
3. Aditya Rai - 240041002
4. Harshitkumar Singh - 240002027

---

## Overview
This project is a simplified simulation of Bitcoin’s transaction system using the **UTXO model**.
It demonstrates:
- UTXO management
- Transaction validation
- Mempool conflict prevention
- Mining and fee collection
- Double-spending and race attack prevention

The system is a **local, single-node simulation** with no networking or cryptography.

---

## Features Implemented

### 1. UTXO Manager
- Stores UTXOs as `(tx_id, index) → (amount, owner)`
- Supports add, remove, balance calculation, and ownership queries

### 2. Transaction Validation
Validation rules enforced:
- Inputs must exist
- No duplicate inputs in same transaction
- Sum(inputs) ≥ Sum(outputs)
- No negative outputs
- No mempool conflicts

### 3. Mempool
- Stores unconfirmed transactions
- Prevents double-spending using `spent_utxos`
- Fee-based eviction when full
- First-seen rule enforced

### 4. Mining Simulation
- Selects highest-fee transactions
- Updates UTXO set permanently
- Miner receives total transaction fees
- Clears mined transactions from mempool

---

## Design Decisions

### Zero-Fee Transactions
Allowed, consistent with Bitcoin rules.

### Race Attacks
Handled using **first-seen rule**: once a UTXO is marked spent in the mempool, all conflicting transactions are rejected.

### Unconfirmed Chain (Test 10)
Unconfirmed outputs **cannot be spent**.  
Child transactions referencing outputs of unmined transactions are rejected.
