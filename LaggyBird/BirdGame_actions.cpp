#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/IObject.h>
#include <LaggyDx/IShape.h>


void BirdsGame::createActions()
{
  getActionsMap().setAction(Dx::KeyboardKey::Space,
                            Dx::Action(std::bind(&BirdsGame::switchPause, this)),
                            Dx::ActionType::OnPress);

  getActionsMap().setAction(Dx::KeyboardKey::OemTilde,
                            Dx::Action(std::bind(&BirdsGame::switchBirdSelection, this)),
                            Dx::ActionType::OnPress);

  getActionsMap().setAction(Dx::KeyboardKey::D1,
                            Dx::Action(std::bind(&BirdsGame::switchAvoidMode, this)),
                            Dx::ActionType::OnPress);
  getActionsMap().setAction(Dx::KeyboardKey::D2,
                            Dx::Action(std::bind(&BirdsGame::switchMatchMode, this)),
                            Dx::ActionType::OnPress);

  getActionsMap().setAction(Dx::KeyboardKey::OemPlus,
                            Dx::Action(std::bind(&BirdsGame::increaseBirdFov, this)),
                            Dx::ActionType::Continuous);
  getActionsMap().setAction(Dx::KeyboardKey::OemMinus,
                            Dx::Action(std::bind(&BirdsGame::decreaseBirdFov, this)),
                            Dx::ActionType::Continuous);
  getActionsMap().setAction(Dx::KeyboardKey::D0,
                            Dx::Action(std::bind(&BirdsGame::resetBirdFov, this)),
                            Dx::ActionType::OnPress);
}


void BirdsGame::switchPause()
{
  d_pause = !d_pause;
}


void BirdsGame::switchBirdSelection()
{
  d_birdIsSelected = !d_birdIsSelected;

  auto& firstBird = *getObjectCollection().getObjects().front();
  firstBird.setColor(d_birdIsSelected ? Dx::Colors::DeepPink : Dx::Colors::DeepSkyBlue);

  switchBirdFov();
}


void BirdsGame::switchBirdFov()
{
  if (d_fovShape)
    d_fovShape.reset();
  else
    recreateBirdFov();
}

void BirdsGame::recreateBirdFov()
{
  const float fov2 = d_fieldOfView / 2;
  d_fovShape = Dx::IShape::createCircle(100, 40, -fov2, fov2);
  d_fovShape->setColor(Dx::colorWithAlpha(Dx::Colors::White, 0.5f));
}

void BirdsGame::increaseBirdFov()
{
  d_fieldOfView = std::min(d_fieldOfView + Sdk::degToRad<float>(5), Sdk::degToRad<float>(360));
  if (d_fovShape)
    recreateBirdFov();
}

void BirdsGame::decreaseBirdFov()
{
  d_fieldOfView = std::max(d_fieldOfView - Sdk::degToRad<float>(5), Sdk::degToRad<float>(5));
  if (d_fovShape)
    recreateBirdFov();
}

void BirdsGame::resetBirdFov()
{
  d_fieldOfView = DefaultFieldOfView;
  if (d_fovShape)
    recreateBirdFov();
}


void BirdsGame::switchAvoidMode()
{
  d_avoidMode = !d_avoidMode;
}


void BirdsGame::switchMatchMode()
{
  d_matchMode = !d_matchMode;
}
