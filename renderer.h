#pragma once
#include <cstdint>

namespace emenu {

class Screen;
class Menu;
class MenuItem;
class Label;
class DValue;
template<typename T>
class IValueInput;


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
};

} // end ns emenu
