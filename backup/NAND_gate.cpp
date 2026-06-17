#include "NAND_gate.h"
#include <cstdint>

NAND_gate::NAND_gate(uint32_t id) : Gate(id) {
  init_inputs(inputs_number);
  init_outputs();
  set_label(NAND_label);

  outputs[0].offsetX = 50.0f;
  outputs[0].offsetY = 20.0f;

  inputs[0].offsetX = 0.0f;
  inputs[0].offsetY = 10.0f;

  inputs[1].offsetX = 0.0f;
  inputs[1].offsetY = 30.0f;
}

NAND_gate::NAND_gate(uint32_t id, int n) : Gate(id) {
  init_inputs(n);
  init_outputs();
  set_label(NAND_label);
}

void NAND_gate::compute() {
  output = 0;

  for (auto p : inputs) {
    if (p.value == 0) {
      output = 1;
    }
    outputs[0].value = output;
  }
}

std::string NAND_gate::get_type() { return type; }
