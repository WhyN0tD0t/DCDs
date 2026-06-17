#ifndef JUNCTION_H
#define JUNCTION_H

#include "Component.h"
#include <cstdint>

class Junction : public Component {
  std::string type = "Junction";
  int n_of_outputs = 1;

public:
  Junction(uint32_t id) : Component(id) {

    // one input
    inputs.push_back(Pin(0, 0));
    // one outputs
    outputs.emplace_back(Pin(0, 0));

    inputs[0].offsetX = 0.0f;
    inputs[0].offsetY = 0.0f;

    outputs[0].offsetX = 0.0f;
    outputs[0].offsetY = 0.0f;
  }

  void compute() override {
    bool incomingSignal = inputs[0].value;

    // Copy to all available output pins
    for (size_t i = 0; i < outputs.size(); i++) {
      outputs[i].value = incomingSignal;
    }
  }

  std::string get_type() override { return type; }
};

#endif
