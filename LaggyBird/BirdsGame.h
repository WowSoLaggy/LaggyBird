#pragma once

#include <LaggyDx/Game.h>


class BirdsGame : public Dx::Game
{
public:
  BirdsGame();

  virtual void onGameStart() override;

  virtual void updateObject(Dx::IObject& i_obj, double i_dt) override;

  virtual void renderObjects() override;

private:
  std::shared_ptr<Dx::IShape> d_shape;

  void shorBirdFov();
};
