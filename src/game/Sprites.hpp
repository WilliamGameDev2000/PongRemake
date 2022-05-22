#pragma once
#include <Engine/Sprite.h>
#include <memory>

class Sprites
{
 public:
  // constructor destructor
  Sprites() = default;
  virtual ~Sprites() = default;

  // sprite loading setters and getters
  void SetSprite(std::unique_ptr<ASGE::Sprite>&& sprite);
  [[nodiscard]] const std::unique_ptr<ASGE::Sprite>& GetSprite() const;

  // rendering
  void Render(ASGE::Renderer* renderer) const;
  void SetWidth(float _width);
  void SetHeight(float _height);
  void SetPosition(Point2D pos);

  // getters
  [[nodiscard]] float GetHeight();
  [[nodiscard]] float GetWidth();
  [[nodiscard]] Point2D GetPos();

 private:
  std::unique_ptr<ASGE::Sprite> spritePtr{ nullptr };
  Point2D position{ 0, 0 };
};
