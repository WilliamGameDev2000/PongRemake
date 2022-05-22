#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "game.h"
/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
Pong::Pong()
{
  game_name = "ASGE Game";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
Pong::~Pong()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));

  //@feedback, good to see memory managed correctly
  if (ball)
  {
    delete (ball);
    ball = nullptr;
  }
  if (paddle1)
  {
    delete (paddle1);
    paddle1 = nullptr;
  }
  if (paddle2)
  {
    delete (paddle2);
    paddle2 = nullptr;
  }
  if (arrow)
  {
    delete (arrow);
    arrow = nullptr;
  }
}

bool Pong::initBall()
{
  ball = renderer->createRawSprite();

  if (!ball->loadTexture("/data//images/ball.png"))
  {
    ASGE::DebugPrinter{} << "init::failed to load ball" << std::endl;
    return false;
  }

  ball->height(64);
  ball->width(64);
  ball->opacity(1);
  return true;
}

bool Pong::initPaddle1()
{
  paddle1 = renderer->createRawSprite();

  if (!paddle1->loadTexture("/data//images/paddle.png"))
  {
    ASGE::DebugPrinter{} << "init::failed to load paddle" << std::endl;
    return false;
  }

  paddle1->height(150);
  paddle1->width(60);
  paddle1->yPos((game_height / 2) - (game_height / 6));
  paddle1->xPos(35);
  return true;
}

bool Pong::initPaddle2()
{
  paddle2 = renderer->createRawSprite();

  if (!paddle2->loadTexture("/data//images/paddle.png"))
  {
    ASGE::DebugPrinter{} << "init::failed to load paddle" << std::endl;
    return false;
  }

  paddle2->height(150);
  paddle2->width(60);
  paddle2->yPos((game_height / 2) - (game_height / 6));
  paddle2->xPos(game_width - (paddle2->width() + 35));
  return true;
}
bool Pong::initArrow()
{
  arrow = renderer->createRawSprite();

  if (!arrow->loadTexture("data/images/Arrow.png"))
  {
    ASGE::DebugPrinter{} << "init::failed to load arrow" << std::endl;
    return false;
  }

  arrow->height(45);
  arrow->width(66);
  arrow->yPos((game_height / 2) + (ball->yPos() + ball->height()));
  arrow->xPos((game_width / 2) + (ball->xPos() + ball->width()));

  arrow->opacity(1);
  return true;
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool Pong::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }
  if (!initBall())
  {
    return false;
  }
  if (!initPaddle1())
  {
    return false;
  }
  if (!initPaddle2())
  {
    return false;
  }
  if (!initArrow())
  {
    return false;
  }

  toggleFPS();

  spawn();

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Pong::keyHandler, this);

  mouse_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Pong::clickHandler, this);

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void Pong::setupResolution()
{
  game_width = 1280;
  game_height = 720;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void Pong::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  //@feedback, shouldn't this be in the menu state?
  if (key->key == ASGE::KEYS::KEY_SPACE)
  {
    in_menu = false;
    spawn();
  }

  if (in_menu)
  {
    handleMenu(key);
  }
  else if (!in_menu)
  {
    handleGame(key);
  }
}

void Pong::handleGame(const ASGE::KeyEvent* key)
{
  //@feedback, if key pressed or released should be own scopes
  if (key->action == ASGE::KEYS::KEY_PRESSED)
  {
    if (key->key == ASGE::KEYS::KEY_W)
    {
      paddle_up = true;
    }
  }
  else if (key->action == ASGE::KEYS::KEY_RELEASED)
  {
    if (key->key == ASGE::KEYS::KEY_W)
    {
      paddle_up = false;
    }
  }

  if (key->key == ASGE::KEYS::KEY_S)
  {
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      paddle_down = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      paddle_down = false;
    }
  }
  if (key->key == ASGE::KEYS::KEY_UP)
  {
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      paddle_increase = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      paddle_increase = false;
    }
  }
  else if (key->key == ASGE::KEYS::KEY_DOWN)
  {
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      paddle_decrease = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      paddle_decrease = false;
    }
  }
}
void Pong::handleMenu(const ASGE::KeyEvent* key)
{
  if (key->key == ASGE::KEYS::KEY_LEFT &&
      key->action == ASGE::KEYS::KEY_RELEASED)
  {
    menu_option = 1 - menu_option;
  }
  else if (key->key == ASGE::KEYS::KEY_RIGHT &&
           key->action == ASGE::KEYS::KEY_RELEASED)
  {
    menu_option = 1 - menu_option;
  }
  if (menu_option == 1)
  {
    game_mode = 2;
  }
  else
  {
    game_mode = 1;
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void Pong::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());
  double x_pos = click->xpos;
  double y_pos = click->ypos;

  if (x_pos > 0 && x_pos < game_width && y_pos > 0 && y_pos < game_height)
  {
    in_menu = true;
  }

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}
void Pong::spawn()
{
  ball->xPos(game_width / 2);
  ball->yPos(game_height / 2);
  ball->opacity(1);
  arrow->opacity(1);
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::update(const ASGE::GameTime& game_time)
{
  srand((unsigned)time(NULL));

  auto dt_sec = game_time.delta.count() / 1000.0;
  float velocity = 500 * (game_time.delta.count() / 1000.f);

  float y_pos = ball->yPos();
  float x_pos = ball->xPos();

  if (!in_menu)
  {
    updateGame(dt_sec, velocity, y_pos, x_pos);
  }
}

void Pong::updateGame(double dt_sec, float velocity, float y_pos, float x_pos)
{
  //@feedback, this function is far too complex and difficult to read
  ball->yPos(y_pos);
  ball->xPos(x_pos);

  if (ball->xPos() >= (game_width - ball->width()))
  {
    spawn();
    player1_score++;
  }
  else if (ball->xPos() <= 0)
  {
    spawn();
    player2_score++;
  }
  else
  {
    if (ball->yPos() >= (game_height - ball->height()))
    {
      directionY = false;
    }
    else if (ball->yPos() <= 0)
    {
      directionY = true;
    }

    if (directionY)
    {
      y_pos += velocity * vector.getY();
      arrow->yPos(y_pos + ball->height());
      if (game_mode == 2)
      {
        ball->opacity(ball->opacity() - 0.00005);
        arrow->opacity(arrow->opacity() - 0.00001);
      }
    }
    else
    {
      y_pos -= velocity * vector.getY();
      arrow->yPos(y_pos - ball->height());
      if (game_mode == 2)
      {
        ball->opacity(ball->opacity() - 0.00005);
        arrow->opacity(arrow->opacity() - 0.00001);
      }
    }
    ball->yPos(y_pos);

    if (paddle_up)
    {
      paddle1->yPos(paddle1->yPos() - (400 * dt_sec));
    }
    if (paddle_down)
    {
      paddle1->yPos(paddle1->yPos() + (400 * dt_sec));
    }
    if (paddle_increase)
    {
      paddle2->yPos(paddle2->yPos() - (400 * dt_sec));
    }
    if (paddle_decrease)
    {
      paddle2->yPos(paddle2->yPos() + (400 * dt_sec));
    }

    if (directionX && !directionY)
    {
      arrow->rotationInRadians(angle1);
    }
    else if (directionX && directionY)
    {
      arrow->rotationInRadians(angle2);
    }
    else if (!directionX && directionY)
    {
      arrow->rotationInRadians(angle3);
    }
    else
    {
      arrow->rotationInRadians(angle4);
    }

    if (inBoundsPaddleSection1(ball, paddle1) ||
        inBoundsPaddleSection1(ball, paddle2))
    {
      directionX = !directionX;
      directionY = false;
      ball->opacity(1);
      arrow->opacity(1);
    }
    else if (inBoundsPaddleSection2(ball, paddle1) ||
             inBoundsPaddleSection2(ball, paddle2))
    {
      directionX = !directionX;
      directionY = true;
      ball->opacity(1);
      arrow->opacity(1);
    }

    if (directionX)
    {
      x_pos += velocity * vector1.getX();
      arrow->xPos(x_pos + ball->width());
      if (game_mode == 2)
      {
        ball->opacity(ball->opacity() - 0.0005);
        arrow->opacity(arrow->opacity() - 0.0003);
      }
    }
    else
    {
      x_pos -= velocity * vector1.getX();
      arrow->xPos(x_pos - ball->width());
      if (game_mode == 2)
      {
        ball->opacity(ball->opacity() - 0.0005);
        arrow->opacity(arrow->opacity() - 0.0003);
      }
    }
    ball->xPos(x_pos);
  }

  if ((paddle1->yPos() + paddle1->height()) < 0)
  {
    paddle1->yPos(game_height);
  }
  else if (paddle1->yPos() > game_height)
  {
    paddle1->yPos(0);
  }
  if ((paddle2->yPos() + paddle2->height()) < 0)
  {
    paddle2->yPos(game_height);
  }
  else if (paddle2->yPos() > game_height)
  {
    paddle2->yPos(0);
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::render(const ASGE::GameTime& game_time)
{
  renderer->setFont(0);

  if (in_menu)
  {
    renderMenu();
  }
  else
  {
    renderer->renderText("Left click to return to the menu",
                         game_width - 850,
                         30,
                         1.2,
                         ASGE::COLOURS::GREEN);

    std::string score1_str = "|>SCORE: " + std::to_string(player1_score) + "<|";
    renderer->renderText(
      score1_str.c_str(), 250, 35, 1.0, ASGE::COLOURS::GREEN);

    std::string score2_str = "|>SCORE: " + std::to_string(player2_score) + "<|";
    renderer->renderText(
      score2_str.c_str(), 950, 35, 1.0, ASGE::COLOURS::GREEN);

    renderer->renderSprite(*ball);

    renderer->renderSprite(*paddle1);

    renderer->renderSprite(*paddle2);

    if (game_mode == 2)
    {
      renderer->renderSprite(*arrow);
    }
  }
}
void Pong::renderMenu() const
{
  renderer->renderText(" PLAY PONG IN ASGE \n PRESS THE SPACE KEY TO BEGIN "
                       "THE GAME",
                       150,
                       150,
                       1.35,
                       ASGE::COLOURS::GREEN);
  renderer->renderText(menu_option == 0 ? "|>GAME MODE 1<|" : "GAME MODE 1",
                       250,
                       650,
                       1.0,
                       ASGE::COLOURS::GREEN);

  renderer->renderText(menu_option == 1 ? "|>GAME MODE 2<|" : "GAME MODE 2",
                       450,
                       650,
                       1.0,
                       ASGE::COLOURS::GREEN);
  renderer->renderText(" PRESS ESC TO BAIL OUT AT ANY TIME",
                       100,
                       (game_height - 100),
                       1.25,
                       ASGE::COLOURS::GREEN);

  if (game_mode == 1)
  {
    renderer->renderText("NORMAL PONG", 150, 450, 1.35, ASGE::COLOURS::GREEN);
  }
  else if (game_mode == 2)
  {
    renderer->renderText("GHOST PONG", 150, 450, 1.35, ASGE::COLOURS::GREEN);
  }
}

//@feedback, this could be reduced to a single function as sprites as axis
// aligned
bool Pong::inBoundsPaddleSection1(const ASGE::Sprite* sprite,
                                  const ASGE::Sprite* sprite1)
{
  return (sprite->xPos() < sprite1->xPos() + sprite1->width() &&
          sprite->xPos() + sprite->width() > sprite1->xPos() &&
          sprite->yPos() < sprite1->yPos() + sprite1->height() / 2 &&
          sprite->yPos() + sprite->height() > sprite1->yPos());
}
bool Pong::inBoundsPaddleSection2(const ASGE::Sprite* sprite,
                                  const ASGE::Sprite* sprite1)
{
  return (sprite->xPos() < sprite1->xPos() + sprite1->width() &&
          sprite->xPos() + sprite->width() > sprite1->xPos() &&
          sprite->yPos() < (sprite1->yPos() + sprite1->height()) &&
          sprite->yPos() + sprite->height() >
            sprite1->yPos() + sprite->height() / 2);
}
