#include "Logic.h"
#include <cstdint>
#include <memory>

uint32_t Logic::addNOTGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<NOT_gate>(id));
  return id;
}

// MAYBE I should use default constructor and then in the small menu
// you could change the number of inputs (ImGui)

uint32_t Logic::addANDGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<AND_gate>(id));
  return id;
}

uint32_t Logic::addORGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<OR_gate>(id));
  return id;
}

// Helper to find a component by ID when making connections
Component *Logic::getComponentByID(uint32_t id) {
  for (const auto &comp : components) {
    if (comp->getID() == id) {
      return comp.get();
    }
  }
  return nullptr;
}

Wire *Logic::getWireByID(uint32_t id) {
  for (const auto &wire : wires) {
    if (wire->getID() == id) {
      return wire.get();
    }
  }
  return nullptr;
}

uint32_t Logic::addWire() {
  uint32_t id = nextComponentID++;
  wires.emplace_back(std::make_unique<Wire>(id));
  return id;
}

uint32_t Logic::getLastUsedID() { return nextComponentID - 1; }
