#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/GameSettings.h>
#include <LaggyDx/IObject.h>
#include <LaggyDx/Renderer2dGuard.h>

#include <LaggySdk/UniformIntGenerator.h>


namespace
{
  constexpr double FovLength = 200;

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

  Sdk::Vector2D getSpeed(const double i_rotation)
  {
    Sdk::Vector2D speed { 0, -400 };
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
    bird.setSpeed(getSpeed(bird.getRotation()));
    bird.setColor(Dx::Colors::DeepSkyBlue);
  }

  createActions();

  getInputDevice().showCursor();
}


void BirdsGame::update(const double i_dt)
{
  if (d_pause)
    return;

  Game::update(i_dt);
}

void BirdsGame::updateObject(Dx::IObject& i_obj, const double i_dt)
{
  perception(i_obj);
  Game::updateObject(i_obj, i_dt);
  teleportObject(i_obj);
}

void BirdsGame::teleportObject(Dx::IObject& i_obj)
{
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

void BirdsGame::perception(Dx::IObject& i_obj)
{
  if (!d_avoidMode)
    return;

  const auto& bird0 = *getObjectCollection().getObjects().front();
  if (&i_obj == &bird0)
    d_adjs.clear();

  std::optional<double> closestDistance;
  std::optional<double> closestAngle;

  for (const auto birdPtr : getObjectCollection().getObjects())
  {
    const auto& bird = *birdPtr;
    if (&i_obj == &bird)
      continue;

    const auto direction = bird.getPosition() - i_obj.getPosition();
    const double distance = direction.length();
    if (distance < FovLength)
    {
      const double angle = i_obj.getSpeed().angle(direction);
      if (std::abs(angle) < d_fieldOfView / 2)
      {
        if (&i_obj == &bird0)
          d_adjs.push_back({ bird0.getPositionF(), bird.getPositionF(), (float)distance });

        if (!closestDistance || distance < *closestDistance)
        {
          closestDistance = distance;
          closestAngle = angle;
        }
      }
    }
  }

  if (closestAngle)
  {
    const double Force = 0.1;
    const double ampl = (1 - (FovLength - *closestDistance) / FovLength) * Force;
    const double angleDiff = *closestAngle > 0 ? -ampl : ampl;
    const double newRotation = i_obj.getRotation() + angleDiff;
    i_obj.setRotation(newRotation);
    i_obj.setSpeed(getSpeed(newRotation));
  }
}


void BirdsGame::renderObjects()
{
  renderFov();
  Game::renderObjects();
  renderAvoid();
}

void BirdsGame::renderFov()
{
  if (!d_fovShape)
    return;

  auto& bird = *getObjectCollection().getObjects().front();
  Dx::Renderer2dGuard rendererGuard(getRenderer2d(), bird.getPositionF(), { 0.0f, 0.0f }, (float)bird.getRotation());
  getRenderer2d().renderShape(*d_fovShape);
}

void BirdsGame::renderAvoid()
{
  if (!d_avoidMode || d_adjs.empty())
    return;

  Dx::Renderer2dGuard rendererGuard(getRenderer2d());
  for (const auto& adj : d_adjs)
    getRenderer2d().renderLine(adj.p0, adj.p1, Dx::Colors::Red);
}
