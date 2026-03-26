#include "Component.h"


void Component::set_label(std::string label) {
    component_label = label;
}

std::string Component::get_label() {
    return component_label;
}

bool Component::get_output_from(int pin_number) {
    return outputs[pin_number].value;
}

void Component::set_pin_value(int n, bool value) {
    inputs[n] = Pin(n, value);
}

