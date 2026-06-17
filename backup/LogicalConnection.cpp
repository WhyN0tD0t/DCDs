#include "LogicalConnection.h"
#include <iostream>

void LogicalConnection::connect(Component *out_component, int output_pin,
                                Component *in_component, int input_pin) {
  source_component = out_component;
  target_component = in_component;
  source_pin = output_pin;
  target_pin = input_pin;

  // Debugging
  // std::cout << "Logical Link created... " << source_component->get_label()
  //          << ", pin: " << source_pin << " -> "
  //          << target_component->get_label() << ", pin: " << target_pin
  //          << std::endl;
}

void LogicalConnection::compute() {
  // Propagates signals instantly
  if (source_component && target_component) {
    current_value = source_component->get_output_from(source_pin);
    target_component->set_pin_value(target_pin, current_value);

    // Debugging
    //  std::cout << source_component->get_label() << " <-----( " <<
    //  current_value << " )-----> " << target_component->get_label() <<
    //  std::endl;
  }
}
