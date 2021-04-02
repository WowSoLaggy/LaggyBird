#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/IObject.h>
#include <LaggyDx/IShape.h>


void BirdsGame::createActions()
{
  getActionsMap().setAction(Dx::KeyboardKey::Space,
                            Dx::Action(std::bind(&BirdsGame::switchBirdSelection, this)),
                            Dx::ActionType::OnPress);

  getActionsMap().setAction(Dx::KeyboardKey::OemTilde,
                            Dx::Action(std::bind(&BirdsGame::selectMode0, this)),
                            Dx::ActionType::OnPress);
  getActionsMap().setAction(Dx::KeyboardKey::D1,
                            Dx::Action(std::bind(&BirdsGame::selectMode1, this)),
                            Dx::ActionType::OnPress);
  getActionsMap().setAction(Dx::KeyboardKey::D2,
                            Dx::Action(std::bind(&BirdsGame::selectMode2, this)),
                            Dx::ActionType::OnPress);
  getActionsMap().setAction(Dx::KeyboardKey::D3,
                            Dx::Action(std::bind(&BirdsGame::selectMode3, this)),
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


void BirdsGame::selectMode0()
{
  d_mode = Mode::Default;

  if (d_birdIsSelected)
    switchBirdSelection();
  if (d_fovShape)
    d_fovShape.reset();
}

void BirdsGame::selectMode1()
{
  d_mode = Mode::ShowPov;

  if (!d_birdIsSelected)
    switchBirdSelection();
  if (!d_fovShape)
    recreateBirdFov();
}

void BirdsGame::selectMode2()
{
}

void BirdsGame::selectMode3()
{
}


void BirdsGame::switchBirdSelection()
{
  d_birdIsSelected = !d_birdIsSelected;

  auto& firstBird = *getObjectCollection().getObjects().front();
  firstBird.setColor(d_birdIsSelected ? Dx::Colors::DeepPink : Dx::Colors::DeepSkyBlue);

  if (!d_birdIsSelected)
    d_fovShape.reset();
  else if (d_mode == Mode::ShowPov)
    recreateBirdFov();
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
  d_fieldOfView = Sdk::degToRad<float>(120);
  if (d_fovShape)
    recreateBirdFov();
}
