#include "adafruit_gfx_renderer.h"
#include "Adafruit_GFX.h"
#include "emenu.h"


namespace emenu {

AdaGFXRenderer::AdaGFXRenderer(Adafruit_GFX* ada_gfx) :
  _gfx(ada_gfx)
{
  _gfx->cp437(true);
  _gfx->setTextSize(1);
  _gfx->setTextColor(1);
  _gfx->setCursor(0,0);
}

void AdaGFXRenderer::render(Screen& s) {
  // background
  if (s._bg_color)
    _gfx->fillRect(s._x, s._y, s._w, s._h, s._bg_color());
  // children components
  s.renderChildren(*this);
}

void AdaGFXRenderer::render(Menu&) {}
void AdaGFXRenderer::render(MenuItem&) {}
void AdaGFXRenderer::render(Label& x) {
  if (x._bg_color)
    _gfx->fillRect(x._x, x._y, x._w, x._h, x._bg_color());
  if (x._refresh) {
    const char* txt = x._refresh();
    _gfx->setCursor(x._x, x._y);
    _gfx->print(txt);
  }
}
void AdaGFXRenderer::render(DValue& x) {
  if (x._bg_color)
    _gfx->fillRect(x._x, x._y, x._w, x._h, x._bg_color());
  if (x._refresh) {
    double val = x._refresh();
    _gfx->setCursor(x._x, x._y);
    _gfx->print(val, x._num_fractional_digits);
    /*
    char buff[64] = {};
    dtostrf(val, 16, x._num_fractional_digits, buff);
    // FIXME improve formatting
    _gfx->print(buff);
    */
  }
}

void AdaGFXRenderer::render(IValueInput<uint16_t>& x) {
  if (x._bg_color)
    _gfx->fillRect(x._x, x._y, x._w, x._h, x._bg_color());
  uint16_t val = {};
  if (! x.active() && x._refresh) {
    val = x._refresh();
  } else
    val = x._edit_val;
  _gfx->setCursor(x._x, x._y);
  _gfx->print(val);

}

}; // end ns emenu

