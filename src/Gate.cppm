module;
#include <cstdint>
#include <iostream>

export module Gate;
export import Component;

export class Gate : public Component {
protected:
  int n_of_inputs = 2;
  bool output = 0;

  Gate(uint32_t id) : Component(id) {}

  void init_inputs(int n) {
    for (int i = 0; i < n; ++i) {
      inputs.emplace_back(Pin(i, 0));
    }
  }

  void init_outputs() { outputs.emplace_back(Pin(0, output)); }

  void set_n_of_inputs(int number) { n_of_inputs = number; }

public:
  void print_gate() {
    std::cout << "\n" << get_label() << std::endl;
    for (auto &p : inputs) {
      std::cout << "x_" << p.pin_number << " - " << p.value << std::endl;
    }
    std::cout << "\noutput - " << outputs[0].value << std::endl;
  }
};
