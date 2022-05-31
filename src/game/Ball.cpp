#include "Ball.hpp"

bool Ball::Init()
{
  return sprite->GetSprite()->loadTexture("data/images/ball.png");
}

std::unique_ptr<Sprites>& Ball::GetSprite()
{
  return sprite;
}
