module;
#include <cstdint>
#include <string>

export module Button;
export import Component;

export class Button : public Component {
private:
  const std::string type = "Button";
  int outputs_number = 1;
  bool state = 0;
  std::string Button_label = "Button";

public:
  Button(uint32_t id) : Component(id) {
    set_label(Button_label);
    outputs.emplace_back(Pin(0, 0));
    outputs[0].offsetX = 20.0f;
    outputs[0].offsetY = 10.0f;
  }

  void compute() override { outputs[0].value = state; }
  std::string get_type() override { return type; }
  void switch_state() { state = !state; }
  void set_state(bool value) { state = value; }
};
