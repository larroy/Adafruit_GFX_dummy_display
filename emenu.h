#pragma once
#include <vector>
#include <memory>
#include <limits>
#include <cassert>
#include <algorithm>
#include <functional>
#include "renderer.h"
#include "emenu_util.h"
#include "emenu_base.h"

namespace emenu {


// function to get color value
typedef std::function<color_t()> fn_color_t;
typedef std::function<bool()> fn_visible_t;
typedef std::function<bool()> _focus_idx;

extern fn_color_t default_fg_color;
extern fn_color_t default_bg_color;

enum class Key : uint16_t {
  UP,
  DOWN,
  ENTER,
  BACK
};


enum class Focus : uint8_t {
  FOCUS,
  NO_FOCUS
};

class Widget {
public:
  Widget() {}
  /**
   * @brief Construct a new Widget object
   *
   * @param parent parent widget
   * @param can_focus true if component can receive focus
   * @param color foreground color
   * @param bg_color background color
   * @param visible if not set, componet is visible, otherwise return if it's visible or not
   */
  Widget(Widget* parent, dim_t x=0, dim_t y=0, dim_t w=0, dim_t h=0, Focus can_focus=Focus::NO_FOCUS, fn_color_t color=nullptr, fn_color_t bg_color=nullptr, fn_visible_t visible=nullptr) :
    _parent(parent),
    _x(x),
    _y(y),
    _w(w),
    _h(h),
    _can_focus(can_focus),
    _color(color),
    _bg_color(bg_color),
    _visible(visible),
    _focus_idx(-1),
    _active{nullptr},
    _widgets{},
    _widgets_nf{}
  {}

  virtual ~Widget() {}
  /*
  Widget(Widget&&) = default;
  Widget(const Widget&) = default;
  Widget(Widget&&) = default;
  Widget& operator=(const Widget&) = default;
  Widget& operator=(Widget&&) = default;
  */


  virtual void render(Renderer& r) = 0;

  virtual void renderChildren(Renderer& r) {
    for (auto&& x: _widgets)
      x->render(r);
    for (auto&& x: _widgets_nf)
      x->render(r);
  }

  void activateFocused() {
    if (_focus_idx >= 0 && _focus_idx < _widgets.size()) {
      _active = _widgets[_focus_idx];
      _widgets[_focus_idx]->setActive();
    }
  }

  virtual void handleKey(Key key) {
    assert(_active); // inactive components should not receieve input
    if (_active == this) {
      switch(key) {
        case Key::ENTER:
          activateFocused();
          break;
        case Key::UP:
          focusNext();
          break;
        case Key::DOWN:
          focusPrev();
          break;
        case Key::BACK:
          setInactive();
          break;
      }
    } else if (_active) {
        _active->handleKey(key);
    } else {
    }
  }

  bool active() const {
    return _active != nullptr;
  }
  virtual void setActive() {
    _active = this;
    _focus_idx = 0;
  }
  virtual void setInactive() {
    _active = nullptr;
    if (_parent)
      _parent->_active = _parent;
    _focus_idx = -1;
  }
  void focusNext() {
    if (! _widgets.empty()) {
      if (_focus_idx < 0)
        _focus_idx = 0;
      size_t i = 0;
      while (true) {
        _focus_idx = (_focus_idx + 1) % _widgets.size();
        ++i;
        if (_widgets[_focus_idx]->canFocus() || i >= _widgets.size())
          return;
      }
    }
  }
  void focusPrev() {
    if (! _widgets.empty()) {
      if (_focus_idx < 0)
        _focus_idx = _widgets.size() - 1;
      size_t i = 0;
      while (true) {
        _focus_idx = (_focus_idx - 1) % _widgets.size();
        ++i;
        if (_widgets[_focus_idx]->canFocus() || i >= _widgets.size())
          return;
      }
    }

  }
  void setNoFocus() {
    _focus_idx = -1;
  }
  bool hasFocus() {
    if (_parent && _parent->_focus_idx >= 0)
        return _parent->_widgets[_parent->_focus_idx] == this;
    return false;
  }
  Widget* focusedChild() {
    if (_focus_idx >= 0 && ! _widgets.empty()) 
        return _widgets[_focus_idx];
    return nullptr;
  }
  bool canFocus() {
    return _can_focus == Focus::FOCUS;

  }
  void addChild(Widget* widget) {
    if (widget->canFocus())
      _widgets.emplace_back(widget);
    else
      _widgets_nf.emplace_back(widget);
  }
  Widget* _parent = nullptr;
  dim_t _x = 0;
  dim_t _y = 0;
  dim_t _w = 0;
  dim_t _h = 0;
  Focus _can_focus = Focus::NO_FOCUS;
  fn_color_t _color = nullptr;
  fn_color_t _bg_color = nullptr;
  fn_visible_t _visible = nullptr;
protected:
  int32_t _focus_idx = -1;
  // active, points to this or an active children or grandchildren
  Widget* _active = nullptr;
  /// child widgets
  std::vector<Widget*> _widgets;
  std::vector<Widget*> _widgets_nf;
};

template<class T>
class WidgetRender : public Widget {
public:
  WidgetRender() : Widget() {}
  WidgetRender(Widget* parent, dim_t x=0, dim_t y=0, dim_t w=0, dim_t h=0, Focus can_focus=Focus::NO_FOCUS, fn_color_t color=nullptr, fn_color_t bg_color=nullptr, fn_visible_t visible=nullptr) :
    Widget(parent, x, y, w, h, can_focus, color, _bg_color, visible)
  {}
  virtual void render(Renderer& r) {
    r.render(*static_cast<T*>(this));
  }
};


class Screen : public WidgetRender<Screen> {
public:
  Screen() :
    WidgetRender()
  {}
  Screen(Widget* parent, dim_t x=0, dim_t y=0, dim_t w=std::numeric_limits<dim_t>::max(), dim_t h=std::numeric_limits<dim_t>::max()) :
    WidgetRender(parent, x, y, w, h, Focus::NO_FOCUS)
  {}

};


class GUI : public Screen {
public:
  GUI(Renderer* renderer, dim_t x, dim_t y, dim_t w, dim_t h):
    Screen(this),
    _renderer(renderer)
  {
    setActive();
  }
  void redraw() {
    render(*_renderer);
  }
  Renderer* _renderer;
};



class Menu : public WidgetRender<Menu> {
public:
  Menu(Widget* parent, dim_t x, dim_t y, dim_t w, dim_t h) :
    WidgetRender(parent, x, y, w, h, Focus::FOCUS)
  {}
};

class MenuItem : public WidgetRender<MenuItem> {
public:
  MenuItem(Widget* parent) : WidgetRender(parent) {
    this->_can_focus = Focus::FOCUS;
  }
};

class Label : public WidgetRender<Label> {
public:
  typedef std::function<char const*()> fn_refresh_t;
  Label(Widget* parent, dim_t x, dim_t y, dim_t w, dim_t h, fn_refresh_t refresh) :
    WidgetRender(parent, x, y, w, h),
    _refresh(refresh)
  {}
  fn_refresh_t _refresh = nullptr;
};

class DValue : public WidgetRender<DValue> {
public:
  typedef std::function<double()> fn_refresh_t;
  DValue(Widget* parent, dim_t x, dim_t y, dim_t w, dim_t h, fn_refresh_t refresh, uint8_t num_integral_digits, uint8_t num_fractional_digits) :
    WidgetRender(parent, x, y, w, h),
    _refresh(refresh),
    _num_integral_digits(num_integral_digits),
    _num_fractional_digits(num_fractional_digits)
  {}
  fn_refresh_t _refresh = nullptr;
  uint8_t _num_integral_digits=0;
  uint8_t _num_fractional_digits=0;
};

template<typename T>
class IValueInput : public WidgetRender<IValueInput<T> > {
public:
  typedef std::function<T()> fn_refresh_t;
  typedef std::function<void(T)> fn_set_t;
  IValueInput(Widget* parent, dim_t x, dim_t y, dim_t w, dim_t h, fn_refresh_t refresh, fn_set_t set, uint8_t num_digits) :
    WidgetRender<IValueInput<T> >(parent, x, y , w, h, Focus::FOCUS),
    _refresh(refresh),
    _set(set),
    _edit_val{},
    _edit_digit{0},
    _num_digits{num_digits}
  {}

  /**
   * @brief increase or decrease on up and down respectively, enter edits next significant digit and then validates and exist.
   *
   * @param key
   */
  virtual void handleKey(Key key) override {
    assert(Widget::active());
    switch(key) {
      case Key::BACK:
        Widget::setInactive();
        break;
      case Key::DOWN:
        _edit_val -= ipow(10, _edit_digit);
        break;
      case Key::UP:
        _edit_val += ipow(10, _edit_digit);
        break;
      case Key::ENTER:
        if (_edit_digit < 0 || _edit_digit == _num_digits) {
          if (_set)
            _set(_edit_val);
          Widget::setInactive();
        } else
          ++_edit_digit;
        break;
    }
  }

  virtual void setActive() override {
    if (_refresh)
      _edit_val = _refresh();
    _edit_digit = 0;
    Widget::setActive();
  }

  fn_refresh_t _refresh;
  fn_set_t _set;
  T _edit_val;
  int8_t _edit_digit;
  uint8_t _num_digits;
};

/*
template<typename T>
class ValueInput : public Value<T> {
  typedef std::function<void(T)> fn_set_t;
  ValueInput(Widget* parent, dim_t x, dim_t y, dim_t w, dim_t h, fn_refresh_t refresh, uint8_t num_integral_digits, uint8_t num_fractional_digits, fn_set_t set) :
    Value(parent, x, y, w, h, refresh, num_integral_digits, num_fractional_digits),
    _set(set)
  {}
  fn_set_t _set;
};
*/


}; // end namespace emenu
