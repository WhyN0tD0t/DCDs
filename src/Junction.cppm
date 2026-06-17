module;
#include <cstdint>
#include <string>

export module Junction;
export import Component;

export class Junction : public Component {
private:
  std::string type = "Junction";
  int n_of_outputs = 1;

public:
  Junction(uint32_t id) : Component(id) {
    inputs.push_back(Pin(0, 0));
    outputs.emplace_back(Pin(0, 0));

    inputs[0].offsetX = 0.0f;
    inputs[0].offsetY = 0.0f;
    outputs[0].offsetX = 0.0f;
    outputs[0].offsetY = 0.0f;

    set_hitbox(x - 5, y - 5, x + 5, y + 5);
  }

  void compute() override {
    bool incomingSignal = inputs[0].value;
    for (size_t i = 0; i < outputs.size(); i++) {
      outputs[i].value = incomingSignal;
    }
  }

  std::string get_type() override { return type; }
};
