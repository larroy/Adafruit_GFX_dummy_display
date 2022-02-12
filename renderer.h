#pragma once
#include <cstdint>
#include "emenu_base.h"

namespace emenu {



class Renderer {
public:
  Renderer() {}
  virtual ~Renderer() {}
  /*
  Renderer(Renderer&&) = default;
  Renderer(const Renderer&) = default;
  Renderer(Renderer&&) = default;
  Renderer& operator=(const Renderer&) = default;
  Renderer& operator=(Renderer&&) = default;
  */

  virtual void render(Screen&) = 0;
  virtual void render(Menu&) = 0;
  virtual void render(MenuItem&) = 0;
  virtual void render(Label&) = 0;
  virtual void render(DValue&) = 0;
  virtual void render(IValueInput<uint16_t>&) = 0;
  virtual void draw_focus(dim_t x, dim_t y, dim_t w, dim_t h, color_t color) = 0;
};

} // end ns emenu
