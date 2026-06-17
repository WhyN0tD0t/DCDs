#include "Bulb.h"
#include <cstdint>

Bulb::Bulb(uint32_t id) : Component(id) {

  set_label(Bulb_label);
  inputs.emplace_back(Pin(0, 0));
  outputs.emplace_back(Pin(0, 0));

  inputs[0].offsetX = -10.0f;
  inputs[0].offsetY = 0.0f;
}

void Bulb::compute() {
  output = inputs[0].value;
  outputs[0].value = output;
}

std::string Bulb::get_type() { return type; }
