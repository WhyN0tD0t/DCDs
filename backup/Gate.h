#ifndef GATE_H
#define GATE_H

#include "Component.h"

class Gate : public Component {

protected:
  int n_of_inputs = 2; // default number of inputs for a simple gate
  bool output = 0;     // default output for a gate (0)  -> can be changed in
                       // specific gate_class eg. NAND
  Gate(uint32_t id);

  void init_inputs(int n); // all the pins are set to 0 by default

  void init_outputs(); // only one output is initialized for gates

  void set_n_of_inputs(int number); // sets the number of inputs to n

public:
  void print_gate(); // prints the gate to the console
};

#endif
