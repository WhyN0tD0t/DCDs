#ifndef LOGIC_H
#define LOGIC_H

#include "AND_gate.h"
#include "Component.h"
#include "NOT_gate.h"
#include "OR_gate.h"

#include "Wire.h"

#include <cstdint>
#include <memory.h>
#include <memory>
#include <sys/types.h>
#include <vector>

class Logic {
private:
  uint32_t nextComponentID = 1;

  std::vector<std::unique_ptr<Component>> components;
  std::vector<std::unique_ptr<Wire>> wires;

public:
  uint32_t addNOTGate();

  // MAYBE I should use default constructor and then in the small menu
  // you could change the number of inputs (ImGui)

  uint32_t addANDGate();

  uint32_t addORGate();

  // Helper to find a component by ID when making connections
  Component *getComponentByID(uint32_t id);
  Wire *getWireByID(uint32_t id);

  uint32_t addWire();

  uint32_t getLastUsedID();
  const std::vector<std::unique_ptr<Wire>> &getWires() const { return wires; }
};

#endif
