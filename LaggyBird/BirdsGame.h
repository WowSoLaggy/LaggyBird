#pragma once

#include "Adjacency.h"

#include <LaggyDx/Game.h>
#include <LaggySdk/Math.h>


class BirdsGame : public Dx::Game
{
public:
  BirdsGame();

  virtual void onGameStart() override;

  virtual void update(double i_dt) override;
  virtual void updateObject(Dx::IObject& i_obj, double i_dt) override;

  virtual void renderObjects() override;

private:
  void createActions();
  void teleportObject(Dx::IObject& i_obj);
  void perception(Dx::IObject& i_obj);
  
  void renderFov();
  void renderAvoid();

  bool d_pause = false;
  void switchPause();

  bool d_birdIsSelected = false;
  void switchBirdSelection();

  static constexpr float DefaultFieldOfView = Sdk::degToRad<float>(180.0f);
  float d_fieldOfView = DefaultFieldOfView;
  std::shared_ptr<Dx::IShape> d_fovShape;
  void switchBirdFov();
  void recreateBirdFov();
  void increaseBirdFov();
  void decreaseBirdFov();
  void resetBirdFov();

  bool d_avoidMode = false;
  std::vector<Adjacency> d_adjs;
  void switchAvoidMode();

  bool d_matchMode = false;
  void switchMatchMode();
};
