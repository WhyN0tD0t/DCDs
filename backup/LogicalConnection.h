#ifndef WIRE_H
#define WIRE_H

#include "Component.h"
#include <cstdint>

class LogicalConnection {
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

  uint32_t getID() const { return id; }
  void connect(Component *out_component, int output_pin,
               Component *in_component, int input_pin);
  void compute();
};

#endif
