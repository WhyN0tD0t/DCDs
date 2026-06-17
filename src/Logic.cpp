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

uint32_t Logic::addNANDGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<NAND_gate>(id));
  return id;
}

uint32_t Logic::addORGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<OR_gate>(id));
  return id;
}

uint32_t Logic::addNORGate() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<NOR_gate>(id));
  return id;
}

uint32_t Logic::addBulb() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<Bulb>(id));
  return id;
}

uint32_t Logic::addButton() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<Button>(id));
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

uint32_t Logic::addJunction() {
  uint32_t id = nextComponentID++;
  components.emplace_back(std::make_unique<Junction>(id));
  return id;
}

uint32_t Logic::addLogicalConnection(Component *source, int sourcePin,
                                     Component *target, int targetPin) {
  uint32_t id = nextComponentID++;

  // 1. Create the new connection
  auto conn = std::make_unique<LogicalConnection>(id);

  // 2. Link the components logically
  conn->connect(source, sourcePin, target, targetPin);

  // 3. Store it in the list
  connections.push_back(std::move(conn));

  return id;
}

uint32_t Logic::getLastUsedID() { return nextComponentID - 1; }

void Logic::removeComponentByID(uint32_t id) {
  // Cleaning up wires and permanently removing dead connections
  for (auto it = connections.begin(); it != connections.end();) {
    if ((*it)->getSourceID() == id) {
      // The source component is being deleted
      (*it)->resetSource();
      // fully deleting it
      it = connections.erase(it);
    } else if ((*it)->getTargetID() == id) {
      // The target component is being deleted
      (*it)->resetTarget();

      it = connections.erase(it);
    } else {
      ++it;
    }
  }

  // Deleting the component itself
  for (auto it = components.begin(); it != components.end(); ++it) {
    if ((*it)->getID() == id) {
      components.erase(it);
      break;
    }
  }
}

void Logic::cleanUpJunc() {
  bool juncExists = false;
  while (juncExists) {

    for (auto it = components.begin(); it != components.end(); ++it) {
      if ((*it)->get_type() == "Junction") {
        uint32_t jID = (*it)->getID();

        // Counting how many wires are plugged into this junction
        int connectionCount = 0;
        for (auto &conn : connections) {
          if (conn->getSourceID() == jID || conn->getTargetID() == jID) {
            connectionCount++;
          }
        }

        // If it has absolutely zero connections it's being deleted
        if (connectionCount == 0) {
          components.erase(it);
          juncExists = true;
          break; // Breaking the for loop to prevent crashing
        }
      }
    }
  }
}

void Logic::computeCircuit() {
  // 2. Compute all connections
  // This physically moves the boolean values from the outputs to the next
  // inputs
  int max_propagation =
      50; // how many junction to junction propagations can be done in one frame

  for (int step = 0; step < max_propagation; ++step) {

    for (auto &comp : components) {
      comp->compute();
    }

    for (auto &conn : connections) {
      conn->compute();
    }
  }
}

// SAVING stuff below

void Logic::clearAll() {
  connections.clear();
  components.clear();
  nextComponentID = 1;
}

void Logic::setNextComponentID(uint32_t max_id) {
  nextComponentID = max_id + 1;
}

Component *Logic::spawnComponentByType(const std::string &type, uint32_t id) {
  if (type == "AND_gate")
    components.push_back(std::make_unique<AND_gate>(id));
  else if (type == "OR_gate")
    components.push_back(std::make_unique<OR_gate>(id));
  else if (type == "NOT_gate")
    components.push_back(std::make_unique<NOT_gate>(id));
  else if (type == "NAND_gate")
    components.push_back(std::make_unique<NAND_gate>(id));
  else if (type == "Button")
    components.push_back(std::make_unique<Button>(id));
  else if (type == "Bulb")
    components.push_back(std::make_unique<Bulb>(id));
  else if (type == "Junction")
    components.push_back(std::make_unique<Junction>(id));
  else if (type == "NOR_gate")
    components.push_back(std::make_unique<NOR_gate>(id));
  else
    return nullptr;

  return components.back().get();
}
