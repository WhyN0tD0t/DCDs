#ifndef BULB_H
#define BULB_H

#include "Component.h"
#include <cstdint>

class Bulb : public Component {
private:
  const std::string type = "Bulb";
  int inputs_number = 1;
  bool output = 0;
  std::string Bulb_label = "Bulb";

public:
  Bulb(uint32_t id);

  void compute() override;
  std::string get_type() override;
};

#endif
