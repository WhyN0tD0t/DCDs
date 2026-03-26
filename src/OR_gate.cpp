#include "OR_gate.h"
#include <cstdint>

OR_gate::OR_gate(uint32_t id) : Gate(id) {
  init_inputs(inputs_number);
  init_outputs();
  set_label(OR_label);
}

OR_gate::OR_gate(uint32_t id, int n) : Gate(id) {
  init_inputs(n);
  init_outputs();
  set_label(OR_label);
}

void OR_gate::compute() {
  output = 0;

  for (auto p : inputs) {
    if (p.value == 1) {
      output = 1;
    }
  }
  outputs[0].value = output;
}

std::string OR_gate::get_type() { return type; }
