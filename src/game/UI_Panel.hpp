#pragma once
#include "Sprites.hpp"

class UI_Panel
{
 public:
  explicit UI_Panel(ASGE::Renderer* renderer);
  ~UI_Panel() = default;

  enum class style
  {
    BOX,
    BANNER,
    CIRCLE
  };

 private:
  ASGE::Renderer* renderer_ = nullptr;
};
