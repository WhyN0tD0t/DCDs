#ifndef WIRE_H
#define WIRE_H

#include "Component.h"
#include <cstdint>

class Wire {
  bool value;
  Component *wire_begin;
  int index_pin_begin;
  Component *wire_end;
  int index_pin_end;

protected:
  uint32_t id;

public:
  void connect(Component *out_component, int output_pin,
               Component *in_component,
               int input_pin); // connects two components and its pins

  void compute(); // propagates signals
  // handshake variables for gui
  uint32_t sourceComponentID;
  uint32_t sourcePinIndex;
  uint32_t targetComponentID;
  uint32_t targetPinIndex;

  Wire(uint32_t assigned_id) : id(assigned_id) {}
  uint32_t getID() const { return id; }

  struct position {
    float x;
    float y;
  };

  position startPos;
  position endPos;
};

#endif
