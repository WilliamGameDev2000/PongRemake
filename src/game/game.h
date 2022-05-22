#pragma once
#include "Vector.h"
#include <Engine/OGLGame.h>
#include <string>
#include <time.h>

/**
 *  An OpenGL Game based on ASGE.
 */
class Pong : public ASGE::OGLGame
{
 public:
  Pong();
  ~Pong() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void setupResolution();
  bool initBall();
  bool initPaddle1();
  bool initPaddle2();
  bool initArrow();

  bool inBoundsPaddleSection1(const ASGE::Sprite* sprite,
                              const ASGE::Sprite* sprite1);
  bool inBoundsPaddleSection2(const ASGE::Sprite* sprite,
                              const ASGE::Sprite* sprite1);
  void spawn();
  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;

  Vector vector = { 0, 2 };
  Vector vector1 = { 2, 0 };

  //@feedback, what happened to your player class?
  int player1_score = 0;
  int player2_score = 0;
  bool paddle_up = false;
  bool paddle_down = false;
  bool paddle_increase = false;
  bool paddle_decrease = false;

  //@feedback, just use a vector instead of these two booleans
  bool directionY = true;
  bool directionX = true;

  //@feedback, consider the use of constants or enums
  static constexpr int MAIN_MENU = 0;
  static constexpr int IN_GAME = 0;
  int game_mode = MAIN_MENU;

  float angle1 = 0.785398;
  float angle2 = 2.35619;
  float angle3 = 3.92699;
  float angle4 = 5.49779;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  int menu_option;
  bool in_menu = true;

  ASGE::Sprite* ball = nullptr;
  ASGE::Sprite* paddle1 = nullptr;
  ASGE::Sprite* paddle2 = nullptr;
  ASGE::Sprite* arrow = nullptr;
  void handleMenu(const ASGE::KeyEvent* key);
  void handleGame(const ASGE::KeyEvent* key);
  void updateGame(double dt_sec, float velocity, float y_pos, float x_pos);
  void renderMenu() const;
};