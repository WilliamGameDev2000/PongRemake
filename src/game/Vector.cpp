//
// Created by hwg2-lawrence on 06/12/2019.
//

#include "Vector.h"
#include <cmath>

Vector::Vector(float x, float y)
{
  setX(x);
  setY(y);

  normalise();
}

void Vector::setX(float x_dir)
{
  x_move = x_dir;
}

float Vector::getX()
{
  return x_move;
}

void Vector::setY(float y_dir)
{
  y_move = y_dir;
}

float Vector::getY()
{
  return y_move;
}

void Vector::normalise()
{
  float magnitude = 1;
  magnitude = (sqrt(x_move * x_move + y_move * y_move));

  x_move /= magnitude;
  y_move /= magnitude;
}

void Vector::multiplyScalar(float scalar)
{
  x_move * +scalar;
  y_move * +scalar;
}