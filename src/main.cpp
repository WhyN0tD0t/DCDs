#include <iostream>
#include <vector>
#include <memory>

class Component {
protected:

    std::string component_label;

    struct Pin {
        std::string label;
        int pin_number;
        bool value = 0;  // 0 for LOW  and 1 for HIGH
        bool is_inverted = 0;

        Pin(int number, bool value) : pin_number(number), value(value) {} 

    };

    std::vector<Pin> inputs;
    std::vector<Pin> outputs;

    virtual void compute() = 0; //pure virtual method different for every component (their internal logic)
    
public:
    void set_label(std::string label) {
        component_label = label;
    }

    std::string get_label() {
        return component_label;
    }

    bool get_output_from(int pin_number) {
        return outputs[pin_number].value;
    }

    void set_pin_value(int n, bool value) {
        inputs[n] = Pin(n, value);
    }

};

class Gate : public Component {

protected:
    int n_of_inputs = 2;  // default number of inputs for a simple gate
    bool output = 0;      // default output for a gate (0)  -> can be changed in specific gate_class eg. NAND  

    void init_inputs(int n) {
        for(int i = 0; i < n; ++i) {
            inputs.emplace_back(Pin(i, 0)); // all created pins are 0 by default for gates
        }
    }
    
    void init_outputs() {
        outputs.emplace_back(Pin(0, output));
    }

    void set_n_of_inputs(int number) {
        n_of_inputs = number;
    }
    


public:
    void print_gate() {
        std::cout<<"\n"<<get_label()<<std::endl;
        for(auto p : inputs) {
            std::cout<<"x_"<<p.pin_number << " - " << p.value << std::endl;
        }
        std::cout<<"\noutput - "<< outputs[0].value << std::endl;
    }




// CODED VISUALS ----------------------------------

   // float width = 50.f;
   // float heihgt = 50.f;
    
    
};


class AND_gate : public Gate {
private:
    int inputs_number = 2;
    std::string AND_label = "AND gate";
public:
    AND_gate() {
        init_inputs(inputs_number);
        init_outputs();
        set_label(AND_label);
    }
    AND_gate(int n) {
        init_inputs(n);
        init_outputs();
        set_label(AND_label);
    }
    

    void compute() override {
        output = 1;

        for(auto p : inputs) {
            if (p.value == 0) {
                output = 0;
            }
        outputs[0].value = output;
        }
    }
};

class OR_gate : public Gate {
    int inputs_number = 2; // default
    std::string OR_label  = "OR gate";  // default, can be change using change_label(std::string label);
public:
    OR_gate() {
        init_inputs(inputs_number);
        init_outputs();
        set_label(OR_label);
    }

    OR_gate(int n) {
        init_inputs(n);
        init_outputs();
        set_label(OR_label); 
    }


    void compute() override {
        output = 0;

        for(auto p : inputs) {
            if(p.value == 1) {
                output = 1;
            }
        }
        outputs[0].value = output;
    }
};

class NOT_gate : public Gate {
    int inputs_number = 1; // default for the NOT gate
    std::string NOT_label = "NOT gate";
public:
    NOT_gate() {
        init_inputs(inputs_number);
        init_outputs();
        set_label(NOT_label);
    }

    void compute() override {
        auto p = inputs[0];
        if(p.value == 1) output = 0;
        if(p.value == 0) output = 1;

        outputs[0].value = output;
    }
};

class Wire {
    bool value;
    Component* wire_begin;
    int index_pin_begin;
    Component* wire_end;
    int index_pin_end;
public:
   

    void connect(Component* out_component, int output_pin, Component* in_component, int input_pin) {
        wire_begin = out_component;
        wire_end =  in_component;
        index_pin_begin = output_pin;
        index_pin_end = input_pin;

        std::cout<<"Connecting... "<<wire_begin->get_label()<<", pin: "<<index_pin_begin<<" -> "<<wire_end->get_label()<<", pin: "<<index_pin_end<<std::endl;
    }

    void compute() {    // propagates signals
        value = wire_begin->get_output_from(index_pin_begin);
        wire_end->set_pin_value(index_pin_end, value);
        
        //debugging
        //
        std::cout<<wire_begin->get_label()<<" <-----( "<< value << " )-----> " << wire_end->get_label()<<std::endl;
    }
};


// FLP FLOPS
class Flip_flop : public Component {
protected:
    
    int n_of_inputs = 2; // default for flip_flops  (set and reset) except clock and enable signal ( WILL BE ADDED LATER)
    bool output = 0;
    bool output_not = 1;

    void init_inputs(int n) {
        for(int i = 0; i < n; ++i)
            inputs.emplace_back(Pin(i, 0));
    }

    void init_outputs() {
        outputs.emplace_back(Pin(0, output));       // Q
        outputs.emplace_back(Pin(1, output_not));   // !Q (not Q)
    }
    

    
    

};

int main()  {

    AND_gate gate1(3);
    gate1.set_pin_value(0, 0);
    gate1.set_pin_value(1, 1);
    gate1.set_pin_value(2, 1);
    gate1.set_label("and_gate_1");
    gate1.compute();
    std::cout<<( gate1.get_label());
    gate1.print_gate();

    OR_gate gate2(3);
    gate2.set_pin_value(0,0);
    gate2.compute();
    gate2.print_gate();

    Wire wire;
    
    wire.connect(&gate1, 0, &gate2, 1);
    wire.connect(&gate1, 0, &gate2, 0);

    wire.compute();

    gate2.compute();
    gate2.print_gate();

    NOT_gate gate3;
    Wire wire2;
    wire2.connect(&gate2, 0, &gate3, 0);
    wire2.compute();
    gate3.compute();
    gate3.print_gate();

    return 0;
}


// TO DO:
//
// stared: creating Flip_flop -> finish the ff class
//
// DONE - CHECK Wires, if they work, if need change 
// DONE - probably introduce pointers to WIRE class to make it easier;
// DONE I think the wires work now. check them on different examples. MAKE sure the gates work as intended
// - Create more gates ( XOR, NOR, NAND, XNOR)
// - Try to introduce FLI[-FLOPS] begin with sr latch,
// - Try to introduce clock and ticking system.
// - Start building interface for users (maybe raylib?) not sure yet
