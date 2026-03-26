#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <string>
#include <vector>

class Component {
protected:
  uint32_t id;
  std::string component_label;

  struct Pin {
    std::string label;
    int pin_number;
    bool value = 0; // 0 for LOW  and 1 for HIGH
    bool is_inverted = 0;

    Pin(int number, bool value) : pin_number(number), value(value) {}
  };

  std::vector<Pin> inputs;
  std::vector<Pin> outputs;

  virtual void compute() = 0; // pure virtual method different for every
                              // component (their internal logic)

public:
  Component(uint32_t assigned_id) : id(assigned_id) {}
  virtual ~Component() = default;

  void set_label(std::string label);

  std::string get_label();

  bool get_output_from(int pin_number);

  void set_pin_value(int n, bool value);

  virtual std::string
  get_type() = 0; // method to be over written for gates and every other
                  // component, so that the type of the object is known
  uint32_t getID() const { return id; }

  struct position {
    float x;
    float y;
  };
};

#endif
