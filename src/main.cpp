// #include <iostream>
#include <vector>

#include "App.h"

import Component;

// FLP FLOPS
class Flip_flop : public Component {
protected:
  int n_of_inputs = 2; // default for flip_flops  (set and reset) except clock
                       // and enable signal ( WILL BE ADDED LATER)
  bool output = 0;
  bool output_not = 1;

  void init_inputs(int n) {
    for (int i = 0; i < n; ++i)
      inputs.emplace_back(Pin(i, 0));
  }

  void init_outputs() {
    outputs.emplace_back(Pin(0, output));     // Q
    outputs.emplace_back(Pin(1, output_not)); // !Q (not Q)
  }
};

int main() {

  // gui side of application
  App app;
  app.run();

  return 0;
}

// TO DO:
//
// stared: creating Flip_flop -> finish the ff class
//
// DONE - CHECK Wires, if they work, if need change
// DONE - probably introduce pointers to WIRE class to make it easier;
// DONE I think the wires work now. check them on different examples. MAKE sure
// the gates work as intended
// - Create more gates ( XOR, NOR, NAND, XNOR)
// - Try to introduce FLI[-FLOPS] begin with sr latch,
// - Try to introduce clock and ticking system.
