#ifndef OR_GATE_H
#define OR_GATE_H

#include "Gate.h"
#include <cstdint>

class OR_gate : public Gate {
  const std::string type = "OR_gate";

  int inputs_number = 2;            // default
  std::string OR_label = "OR gate"; // default, can be change using
                                    // change_label(std::string label);
public:
  OR_gate(uint32_t id); // uses default inputs_number

  OR_gate(uint32_t id, int n); // sets inputs_number

  void compute() override;

  std::string get_type() override;
};

#endif
