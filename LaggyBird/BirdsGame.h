#pragma once

#include <LaggyDx/Game.h>
#include <LaggySdk/Math.h>


class BirdsGame : public Dx::Game
{
public:
  BirdsGame();

  virtual void onGameStart() override;

  virtual void updateObject(Dx::IObject& i_obj, double i_dt) override;

  virtual void renderObjects() override;

private:
  void createActions();
  
  enum class Mode
  {
    Default = 0,
    ShowPov,
    Avoid,
  };
  Mode d_mode = Mode::Default;
  void selectMode0();
  void selectMode1();
  void selectMode2();
  void selectMode3();

  bool d_birdIsSelected = false;
  void switchBirdSelection();

  float d_fieldOfView = Sdk::degToRad<float>(120.0f);
  std::shared_ptr<Dx::IShape> d_fovShape;
  void switchBirdFov();
  void recreateBirdFov();
  void increaseBirdFov();
  void decreaseBirdFov();
  void resetBirdFov();
};
