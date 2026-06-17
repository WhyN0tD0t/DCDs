#include "Gate.h"

#include <iostream>

// protected

Gate::Gate(uint32_t id) : Component(id) {
  // Any other Gate-specific setup goes here
}
void Gate::init_inputs(int n) {
  for (int i = 0; i < n; ++i) {
    inputs.emplace_back(
        Pin(i, 0)); // all created pins are 0 by default for gates
  }
}

void Gate::init_outputs() { outputs.emplace_back(Pin(0, output)); }

void Gate::set_n_of_inputs(int number) { n_of_inputs = number; }

// public

void Gate::print_gate() {
  std::cout << "\n" << get_label() << std::endl;
  for (auto p : inputs) {
    std::cout << "x_" << p.pin_number << " - " << p.value << std::endl;
  }
  std::cout << "\noutput - " << outputs[0].value << std::endl;
}
