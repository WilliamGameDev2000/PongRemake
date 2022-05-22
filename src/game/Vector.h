//
// Created by hwg2-lawrence on 06/12/2019.
//

#ifndef PONG_WILLIAMUWE1_VECTOR_H
#define PONG_WILLIAMUWE1_VECTOR_H

//@feedback: good attempt at a vector class
class Vector
{
 public:
  Vector(float x, float y);

  void setX(float x_dir);
  float getX();
  void setY(float y_dir);
  float getY();
  void normalise();
  void multiplyScalar(float scalar);

 private:
  int x_move = 0;
  int y_move = 0;
};

#endif // PONG_WILLIAMUWE1_VECTOR_H
