module;
#include <cstdint>
#include <string>
#include <vector>

export module Component;

export struct HitBox {
  float startX = 0.0f;
  float startY = 0.0f;
  float endX = 0.0f;
  float endY = 0.0f;
};

export class Component {
protected:
  uint32_t id;
  std::string component_label;

public: // Pin must be public so it can be accessed outside the class
  struct Pin {
    std::string label;
    int pin_number;
    bool value = 0;
    bool is_inverted = 0;

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool isHovered = 0;

    Pin(int number, bool value) : pin_number(number), value(value) {}
  };

protected:
  std::vector<Pin> inputs;
  std::vector<Pin> outputs;

public:
  Component(uint32_t assigned_id) : id(assigned_id), x(0.0f), y(0.0f) {}
  virtual ~Component() = default;

  void set_label(std::string label) { component_label = label; }
  std::string get_label() { return component_label; }

  bool get_output_from(int pin_number) { return outputs[pin_number].value; }

  std::vector<Pin> &get_input_pins() { return inputs; }
  std::vector<Pin> &get_output_pins() { return outputs; }

  // FIXED: modifies the value without destroying your offsets!
  void set_pin_value(int n, bool value) { inputs[n].value = value; }

  virtual void compute() = 0;
  virtual std::string get_type() = 0;

  uint32_t getID() const { return id; }

  float x;
  float y;
  HitBox hitbox;

  void set_hitbox(float startX, float startY, float endX, float endY) {
    hitbox.startX = startX;
    hitbox.startY = startY;
    hitbox.endX = endX;
    hitbox.endY = endY;
  }
};
