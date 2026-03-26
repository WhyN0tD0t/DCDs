#include "Wire.h"
#include <iostream>

// PUBLIC

void Wire::connect(Component* out_component, int output_pin, Component* in_component, int input_pin) {
    wire_begin = out_component;
    wire_end =  in_component;
    index_pin_begin = output_pin;
    index_pin_end = input_pin;

    std::cout<<"Connecting... "<<wire_begin->get_label()<<", pin: "<<index_pin_begin<<" -> "<<wire_end->get_label()<<", pin: "<<index_pin_end<<std::endl;
}

void Wire::compute() {    // propagates signals
    value = wire_begin->get_output_from(index_pin_begin);
    wire_end->set_pin_value(index_pin_end, value);
    
    //debugging
    //
    std::cout<<wire_begin->get_label()<<" <-----( "<< value << " )-----> " << wire_end->get_label()<<std::endl;
}
