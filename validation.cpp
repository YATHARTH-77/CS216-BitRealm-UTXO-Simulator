#include "validation.h"
#include "mempool.h" 
#include <set>
using namespace std;

ValidationResult validate_transaction(const Transaction &tx, const UTXOManager &utxo_manager, const Mempool &mempool) {
     set<pair<string, int>> seen_inputs;
     double input_sum = 0.0;
     double output_sum = 0.0;

     for (const auto &in : tx.inputs) {
          pair<string, int> utxo_ref = {in.prev_tx, in.index};
          
          if (seen_inputs.count(utxo_ref)) return {false, "Double spending within transaction", 0.0};
          seen_inputs.insert(utxo_ref);

          if (!utxo_manager.exists(in.prev_tx, in.index)) return {false, "Input UTXO does not exist", 0.0};
          if (utxo_manager.get_owner(in.prev_tx, in.index) != in.owner) return {false, "Invalid owner (Signature mismatch)", 0.0};
          if (mempool.is_spent(in.prev_tx, in.index)) return {false, "UTXO is already spent in mempool", 0.0};

          input_sum += utxo_manager.get_amount(in.prev_tx, in.index);
     }

     for (const auto &out : tx.outputs) {
          if (out.amount < 0.0) return {false, "Negative output amount", 0.0};
          output_sum += out.amount;
     }

     if (input_sum < output_sum) return {false, "Insufficient funds", 0.0};

     return {true, "Transaction valid", input_sum - output_sum};
}