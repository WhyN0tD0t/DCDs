#ifndef NAND_GATE_H
#define NAND_GATE_H

#include "Gate.h"
#include <cstdint>

class NAND_gate : public Gate {
private:
  const std::string type = "NAND_gate";

  int inputs_number = 2;
  std::string NAND_label = "NAND gate";

public:
  NAND_gate(uint32_t id);        // default constructor
  NAND_gate(uint32_t id, int n); // parametrized constructor (number of inputs)

  void compute() override;
  std::string get_type() override;
};

#endif
