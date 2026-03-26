#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "Gate.h"
#include <cstdint>

class NOT_gate : public Gate {
  const std::string type = "NOT_gate";

  int inputs_number = 1; // default for the NOT gate
  std::string NOT_label = "NOT gate";

public:
  NOT_gate(uint32_t id);

  void compute() override;

  std::string get_type() override;
};

#endif
