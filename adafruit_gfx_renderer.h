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
  virtual void draw_focus(dim_t x, dim_t y, dim_t w, dim_t h, color_t color) override;



  Adafruit_GFX* _gfx;
};


} // end ns

