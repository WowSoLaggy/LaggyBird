#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/GameSettings.h>
#include <LaggyDx/IObject.h>
#include <LaggyDx/Renderer2dGuard.h>

#include <LaggySdk/UniformIntGenerator.h>


namespace
{
  const Dx::GameSettings& getGameSettings()
  {
    static Dx::GameSettings gameSettings{ 1600, 900, "Birds", "Data/Assets" };
    return gameSettings;
  }

  int getScreenWidth()
  {
    return getGameSettings().screenWidth;
  }
  int getScreenHeight()
  {
    return getGameSettings().screenHeight;
  }

  Sdk::Vector2D getRandomPointOnScreen()
  {
    const auto& gameSettings = getGameSettings();
    
    return {
      (double)Sdk::UniformIntGenerator<int>(0, 1600).getNextValue(),
      (double)Sdk::UniformIntGenerator<int>(0, 900).getNextValue() };
  }

  double getRandomDirection()
  {
    const int randomInt = Sdk::UniformIntGenerator<int>(0, 2 * 314).getNextValue();
    return (double)randomInt / 100;
  }

  Sdk::Vector2D getSpeed(const double i_value, const double i_rotation)
  {
    Sdk::Vector2D speed { 0, -i_value };
    speed.rotate(i_rotation, { 0, 0 });
    return speed;
  }

} // anon NS

BirdsGame::BirdsGame()
  : Dx::Game(getGameSettings())
{
}


void BirdsGame::onGameStart()
{
  getRenderDevice().setClearColor({ 0, 0, 0, 1 });

  constexpr int numBirds = 50;
  for (int i = 0; i < numBirds; ++i)
  {
    auto& bird = getObjectCollection().createObject();
    bird.setTexture("Bird.png");
    bird.setPosition(getRandomPointOnScreen());
    bird.setRotation(getRandomDirection());
    bird.setSpeed(getSpeed(500, bird.getRotation()));
    bird.setColor(Dx::Colors::DeepSkyBlue);
  }

  createActions();

  getInputDevice().showCursor();
}


void BirdsGame::updateObject(Dx::IObject& i_obj, const double i_dt)
{
  Game::updateObject(i_obj, i_dt);

  auto pos = i_obj.getPosition();
  const auto speed = i_obj.getSpeed();
  const int screenWidth = getScreenWidth();
  const int screenHeight = getScreenHeight();

  constexpr int OffsetToTeleport = 64;
  if (pos.x < -OffsetToTeleport && speed.x < 0)
  {
    pos.x = screenWidth + OffsetToTeleport;
    i_obj.setPosition(pos);
  }
  else if (pos.x > screenWidth + OffsetToTeleport && speed.x > 0)
  {
    pos.x = -OffsetToTeleport;
    i_obj.setPosition(pos);
  }

  if (pos.y < -OffsetToTeleport && speed.y < 0)
  {
    pos.y = screenHeight + OffsetToTeleport;
    i_obj.setPosition(pos);
  }
  else if (pos.y > screenHeight + OffsetToTeleport && speed.y > 0)
  {
    pos.y = -OffsetToTeleport;
    i_obj.setPosition(pos);
  }
}


void BirdsGame::renderObjects()
{
  if (d_fovShape)
  {
    auto& bird = getObjectCollection().getObjects().front();
    Dx::Renderer2dGuard rendererGuard(getRenderer2d(), bird->getPosition(), { 0, 0 }, bird->getRotation());
    getRenderer2d().renderShape(*d_fovShape);
  }

  Game::renderObjects();
}
