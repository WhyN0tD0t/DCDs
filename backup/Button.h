#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include <cstdint>

class Button : public Component {
private:
  const std::string type = "Button";
  int outputs_number = 1;
  bool state = 0;
  std::string Button_label = "Button";

public:
  Button(uint32_t id);

  void compute() override;
  std::string get_type() override;
  void switch_state();
  void set_state(bool value);
};

#endif
