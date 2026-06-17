module;
#include <cstdint>
#include <iostream>

export module LogicalConnection;
export import Component;

export class LogicalConnection {
private:
  uint32_t id;
  Component *source_component;
  Component *target_component;
  int source_pin;
  int target_pin;
  bool current_value;

public:
  LogicalConnection(uint32_t id)
      : id(id), source_component(nullptr), target_component(nullptr),
        source_pin(0), target_pin(0), current_value(false) {}

  uint32_t getSourceID() const {
    return source_component ? source_component->getID() : -1;
  }
  uint32_t getTargetID() const {
    return target_component ? target_component->getID() : -1;
  }
  int getSourcePin() const { return source_pin; }
  int getTargetPin() const { return target_pin; }

  uint32_t getID() const { return id; }

  void connect(Component *out_comp, int out_pin, Component *in_comp,
               int in_pin) {
    source_component = out_comp;
    target_component = in_comp;
    source_pin = out_pin;
    target_pin = in_pin;
  }

  void resetSource() {
    source_component = nullptr;
    source_pin = 0;

    // if there was the target, the signal is reseted (its in void)
    if (target_component) {
      target_component->set_pin_value(target_pin, false);
    }
    current_value = false;
  }

  void resetTarget() {
    target_component = nullptr;
    target_pin = 0;
  }

  void compute() {
    if (source_component && target_component) {
      current_value = source_component->get_output_from(source_pin);
      target_component->set_pin_value(target_pin, current_value);
    }
  }
};
