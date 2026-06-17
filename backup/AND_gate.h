#ifndef AND_GATE_H
#define AND_GATE_H

#include "Gate.h"
#include <cstdint>

class AND_gate : public Gate {
private:
  const std::string type = "AND_gate";

  int inputs_number = 2;
  std::string AND_label = "AND gate";

public:
  AND_gate(uint32_t id);        // default constructor
  AND_gate(uint32_t id, int n); // parametrized constructor (number of inputs)

  void compute() override;
  std::string get_type() override;
};

#endif
