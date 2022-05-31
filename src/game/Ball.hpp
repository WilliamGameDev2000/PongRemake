#pragma once
#include "Sprites.hpp"

class Ball
{
 public:
  Ball() = default;
  ~Ball() = default;

  bool Init();
  std::unique_ptr<Sprites>& GetSprite();

 private:
  std::unique_ptr<Sprites> sprite;
};
