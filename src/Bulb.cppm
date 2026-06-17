module;
#include <cstdint>
#include <string>

export module Bulb;
export import Component;

export class Bulb : public Component {
private:
  const std::string type = "Bulb";
  int inputs_number = 1;
  bool output = 0;
  std::string Bulb_label = "Bulb";

public:
  Bulb(uint32_t id) : Component(id) {
    set_label(Bulb_label);
    inputs.emplace_back(Pin(0, 0));
    outputs.emplace_back(Pin(0, 0));
    inputs[0].offsetX = -10.0f;
    inputs[0].offsetY = 0.0f;
  }

  void compute() override {
    output = inputs[0].value;
    outputs[0].value = output;
  }

  std::string get_type() override { return type; }
};
