module;
#include <cstdint>
#include <string>

export module NOT_gate;
export import Gate;

export class NOT_gate : public Gate {
private:
  const std::string type = "NOT_gate";
  int inputs_number = 1;
  std::string NOT_label = "NOT gate";

public:
  NOT_gate(uint32_t id) : Gate(id) {
    init_inputs(inputs_number);
    init_outputs();
    set_label(NOT_label);

    inputs[0].offsetX = 0.0f;
    inputs[0].offsetY = 10.0f;
    outputs[0].offsetX = 30.0f;
    outputs[0].offsetY = 10.0f;
  }

  void compute() override {
    // Safe boolean inversion
    output = !inputs[0].value;
    outputs[0].value = output;
  }

  std::string get_type() override { return type; }
};
