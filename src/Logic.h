#ifndef LOGIC_H
#define LOGIC_H

#include <cstdint>
#include <memory.h>
#include <memory>
#include <sys/types.h>
#include <vector>

import AND_gate;
import Bulb;
import Button;
import Component;
import NAND_gate;
import NOT_gate;
import OR_gate;
import NOR_gate;

// #include "Wire.h"
import Junction;
import LogicalConnection;
class Logic {
private:
  uint32_t nextComponentID = 1;

  std::vector<std::unique_ptr<Component>> components;
  std::vector<std::unique_ptr<LogicalConnection>> connections;

public:
  uint32_t addNOTGate();

  // MAYBE I should use default constructor and then in the small menu
  // you could change the number of inputs (ImGui)

  uint32_t addANDGate();

  uint32_t addNANDGate();

  uint32_t addORGate();

  uint32_t addNORGate();

  uint32_t addBulb();

  uint32_t addButton();

  // Helper to find a component by ID when making connections
  Component *getComponentByID(uint32_t id);
  // Wire *getWireByID(uint32_t id);

  uint32_t addLogicalConnection(Component *source, int sourcePin,
                                Component *target, int targetPin);

  const std::vector<std::unique_ptr<LogicalConnection>> &
  getConnections() const {
    return connections;
  }

  uint32_t addJunction();

  uint32_t getLastUsedID();

  // const std::vector<std::unique_ptr<Wire>> &getWires() const { return
  // wires;
  // }
  const std::vector<std::unique_ptr<Component>> &getComponents() const {
    return components;
  }

  void removeComponentByID(uint32_t id);

  void cleanUpJunc();

  void computeCircuit();

  void clearAll();
  void setNextComponentID(uint32_t max_id);
  Component *spawnComponentByType(const std::string &type, uint32_t id);
};

#endif
