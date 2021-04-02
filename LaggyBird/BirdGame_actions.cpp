#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/IShape.h>


void BirdsGame::shorBirdFov()
{
  if (d_shape)
    d_shape.reset();
  else
  {
    d_shape = Dx::IShape::createCircle(100, 40);
    d_shape->setColor(Dx::colorWithAlpha(Dx::Colors::White, 0.5f));
  }
}
