#pragma once

#include <cstdint>
#include <stdint.h>
#include "renderer.h"

class Adafruit_GFX;

namespace emenu {

class AdaGFXRenderer : public Renderer {
public:
  AdaGFXRenderer(Adafruit_GFX* ada_gfx);

  virtual void render(Screen&) override;
  virtual void render(Menu&) override;
  virtual void render(MenuItem&) override;
  virtual void render(Label&) override;
  virtual void render(DValue&) override;
  virtual void render(IValueInput<uint16_t>&) override;




  Adafruit_GFX* _gfx;
};


} // end ns

