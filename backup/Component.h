#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <string>
#include <vector>

struct HitBox {
  float startX;
  float startY;
  float endX;
  float endY;
};

class Component {
protected:
  uint32_t id;
  std::string component_label;

  struct Pin {
    std::string label;
    int pin_number;
    bool value = 0; // 0 for LOW  and 1 for HIGH
    bool is_inverted = 0;
    // for the GUI
    float offsetX = 0.0f; // Distance from the component's base X
    float offsetY = 0.0f; // Distance from the component's base Y
    bool isHovered = 0;

    Pin(int number, bool value) : pin_number(number), value(value) {}
  };

  std::vector<Pin> inputs;
  std::vector<Pin> outputs;

public:
  Component(uint32_t assigned_id) : id(assigned_id) {}
  virtual ~Component() = default;

  void set_label(std::string label);

  std::string get_label();

  bool get_output_from(int pin_number);

  std::vector<Pin> &get_input_pins() { return inputs; }
  std::vector<Pin> &get_output_pins() { return outputs; }

  void set_pin_value(int n, bool value);

  virtual void compute() = 0; // pure virtual method different for every
                              // component (their internal logic)
                              //

  virtual std::string
  get_type() = 0; // method to be over written for gates and every other
                  // component, so that the type of the object is known
  uint32_t getID() const { return id; }

  // position GUI
  float x;
  float y;

  HitBox hitbox;

  void set_hitbox(float startX, float startY, float endX, float endY);
};

#endif
