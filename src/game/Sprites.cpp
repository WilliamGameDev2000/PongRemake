#include "Sprites.hpp"
#include "Engine/Renderer.h"

void Sprites::SetSprite(std::unique_ptr<ASGE::Sprite>&& sprite)
{
  spritePtr = std::move(sprite);
}
const std::unique_ptr<ASGE::Sprite>& Sprites::GetSprite() const
{
  return spritePtr;
}
void Sprites::Render(ASGE::Renderer* renderer) const
{
  renderer->renderSprite(*spritePtr);
}
void Sprites::SetWidth(float _width)
{
  spritePtr->width(_width);
}
void Sprites::SetHeight(float _height)
{
  spritePtr->height(_height);
}
void Sprites::SetPosition(Point2D pos)
{
  position = pos;
}
float Sprites::GetHeight()
{
  return spritePtr->height();
}
float Sprites::GetWidth()
{
  return spritePtr->width();
}
Point2D Sprites::GetPos()
{
  return position;
}
