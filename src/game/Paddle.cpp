//
// Created by William on 22/05/2022.
//

#include "Paddle.hpp"
bool Paddle::Init()
{
  return sprite->GetSprite()->loadTexture("data/images/paddle.png");
}
std::unique_ptr<Sprites>& Paddle::GetSprite()
{
  return sprite;
}
