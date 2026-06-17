#include "NOT_gate.h"
#include <cstdint>

NOT_gate::NOT_gate(uint32_t id) : Gate(id) {
  init_inputs(inputs_number);
  init_outputs();
  set_label(NOT_label);

  inputs[0].offsetX = 0.0f;
  inputs[0].offsetY = 10.0f;

  outputs[0].offsetX = 30.0f;
  outputs[0].offsetY = 10.0f;
}

void NOT_gate::compute() {
  auto p = inputs[0];
  if (p.value == 1)
    output = 0;
  if (p.value == 0)
    output = 1;

  outputs[0].value = output;
}

std::string NOT_gate::get_type() { return type; }
