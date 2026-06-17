module;
#include <cstdint>
#include <string>

export module OR_gate;
export import Gate;

export class OR_gate : public Gate {
private:
  const std::string type = "OR_gate";
  int inputs_number = 2;
  std::string OR_label = "OR gate";

public:
  OR_gate(uint32_t id) : Gate(id) {
    init_inputs(inputs_number);
    init_outputs();
    set_label(OR_label);

    inputs[0].offsetX = 0.0f;
    inputs[0].offsetY = 10.0f;
    inputs[1].offsetX = 0.0f;
    inputs[1].offsetY = 30.0f;
    outputs[0].offsetX = 40.0f;
    outputs[0].offsetY = 20.0f;
  }

  OR_gate(uint32_t id, int n) : Gate(id) {
    init_inputs(n);
    init_outputs();
    set_label(OR_label);
  }

  void compute() override {
    output = 0;
    for (auto &p : inputs) {
      if (p.value == 1)
        output = 1;
    }
    outputs[0].value = output;
  }

  std::string get_type() override { return type; }
};
