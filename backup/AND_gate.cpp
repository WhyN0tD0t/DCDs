#include "AND_gate.h"
#include <cstdint>

AND_gate::AND_gate(uint32_t id) : Gate(id) {
  init_inputs(inputs_number);
  init_outputs();
  set_label(AND_label);
}

AND_gate::AND_gate(uint32_t id, int n) : Gate(id) {
  init_inputs(n);
  init_outputs();
  set_label(AND_label);
}

void AND_gate::compute() {
  output = 1;

  for (auto p : inputs) {
    if (p.value == 0) {
      output = 0;
    }
    outputs[0].value = output;
  }
}

std::string AND_gate::get_type() { return type; }
