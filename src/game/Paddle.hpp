#pragma once
#include "Sprites.hpp"

class Paddle
{
 public:
  Paddle() = default;
  ~Paddle() = default;

  bool Init();
  std::unique_ptr<Sprites>& GetSprite();

 private:
  std::unique_ptr<Sprites> sprite;
};
