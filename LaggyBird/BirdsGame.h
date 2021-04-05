#pragma once

#include "Adjacency.h"
#include "Settings.h"

#include <LaggyDx/Game.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Math.h>


class BirdsGame : public Dx::Game
{
public:
  BirdsGame();

  virtual void onGameStart() override;

  virtual void update(double i_dt) override;
  virtual void updateObject(Dx::IObject& i_obj, double i_dt) override;

  virtual void renderObjects() override;
  virtual void renderObject(const Dx::IObject& i_obj) override;

private:
  void createActions();
  void teleportObject(Dx::IObject& i_obj);
  void perception(Dx::IObject& i_obj);
  
  void renderFov();
  void renderAvoid();
  void renderMatch();
  void renderVelocity(const Dx::IObject& i_obj);
  void renderFlockCenter();

  bool d_pause = false;
  void switchPause();

  void switchTooltip();
  void createTooltip();
  void deleteTooltip();
  std::shared_ptr<Dx::Label> d_tooltip;

  bool d_birdIsSelected = false;
  void switchBirdSelection();

  float d_fieldOfView = Settings::DefaultFieldOfView;
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

  bool d_flockMode = false;
  Dx::Sprite d_flockCenter;
  Sdk::Vector2F d_flockCenterSize;
  void switchFlockMode();
};
