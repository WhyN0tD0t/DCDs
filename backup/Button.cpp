#include "Button.h"
#include <cstdint>

Button::Button(uint32_t id) : Component(id) {
  set_label(Button_label);
  outputs.emplace_back(Pin(0, 0));

  outputs[0].offsetX = 20.0f;
  outputs[0].offsetY = 10.0f;
}

void Button::compute() { outputs[0].value = state; }

void Button::set_state(bool value) { state = value; }

void Button::switch_state() {
  if (state == 0)
    state = 1;
  else
    state = 0;
}

std::string Button::get_type() { return type; }
