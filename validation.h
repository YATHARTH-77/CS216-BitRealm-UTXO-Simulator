#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include "transaction.h"
#include "utxo_manager.h"
using namespace std;

class Mempool; 

struct ValidationResult {
     bool success;
     string message;
     double fees;
};

ValidationResult validate_transaction(
    const Transaction &tx,
    const UTXOManager &utxo_manager,
    const Mempool &mempool
);

#endif