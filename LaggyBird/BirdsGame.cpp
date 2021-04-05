#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Colors.h>
#include <LaggyDx/GameSettings.h>
#include <LaggyDx/IObject.h>
#include <LaggyDx/Renderer2dGuard.h>
#include <LaggyDx/IResourceController.h>

#include <LaggySdk/UniformIntGenerator.h>


namespace
{
const Dx::GameSettings& getGameSettings()
  {
    static Dx::GameSettings gameSettings{
      Settings::ClientWidth, Settings::ClientHeight,
      "Birds", "Data/Assets" };
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
      (double)Sdk::UniformIntGenerator<int>(0, Settings::ClientWidth).getNextValue(),
      (double)Sdk::UniformIntGenerator<int>(0, Settings::ClientHeight).getNextValue() };
  }

  double getRandomDirection()
  {
    const int randomInt = Sdk::UniformIntGenerator<int>(0, 2 * 314).getNextValue();
    return (double)randomInt / 100;
  }

  Sdk::Vector2D getSpeed(const double i_rotation)
  {
    Sdk::Vector2D speed { 0, -Settings::SpeedValue };
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

  for (int i = 0; i < Settings::BirdsCount; ++i)
  {
    auto& bird = getObjectCollection().createObject();
    bird.setTexture("Bird.png");
    bird.setPosition(getRandomPointOnScreen());
    bird.setRotation(getRandomDirection());
    bird.setSpeed(getSpeed(bird.getRotation()));
    bird.setColor(Dx::Colors::DeepSkyBlue);
  }

  d_flockCenter.setTexture(getResourceController().getTextureResource("Mark.png"));
  d_flockCenter.resetSizeToTexture();
  d_flockCenterSize = { (float)d_flockCenter.getSize().x, (float)d_flockCenter.getSize().y };

  createActions();
  createTooltip();

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

  if (pos.x < -Settings::OffsetToTeleport && speed.x < 0)
  {
    pos.x = screenWidth + Settings::OffsetToTeleport;
    i_obj.setPosition(pos);
  }
  else if (pos.x > screenWidth + Settings::OffsetToTeleport && speed.x > 0)
  {
    pos.x = -Settings::OffsetToTeleport;
    i_obj.setPosition(pos);
  }

  if (pos.y < -Settings::OffsetToTeleport && speed.y < 0)
  {
    pos.y = screenHeight + Settings::OffsetToTeleport;
    i_obj.setPosition(pos);
  }
  else if (pos.y > screenHeight + Settings::OffsetToTeleport && speed.y > 0)
  {
    pos.y = -Settings::OffsetToTeleport;
    i_obj.setPosition(pos);
  }
}

void BirdsGame::perception(Dx::IObject& i_obj)
{
  if (!d_avoidMode && !d_matchMode && !d_flockMode)
    return;

  const auto& bird0 = *getObjectCollection().getObjects().front();
  if (&i_obj == &bird0)
    d_adjs.clear();

  std::optional<double> closestDistance;
  std::optional<double> closestAngle;

  double avgNeighRotation = 0;
  Sdk::Vector2F avgNeighPosition;
  int neighCount = 0;

  for (const auto birdPtr : getObjectCollection().getObjects())
  {
    const auto& bird = *birdPtr;
    if (&i_obj == &bird)
      continue;

    const auto direction = bird.getPosition() - i_obj.getPosition();
    const double distance = direction.length();
    if (distance < Settings::FovLength)
    {
      const double angle = i_obj.getSpeed().angle(direction);
      if (std::abs(angle) < d_fieldOfView / 2)
      {
        if (&i_obj == &bird0)
        {
          const float relativeDistance = (float)(distance / Settings::FovLength);
          d_adjs.push_back({ bird0.getPositionF(), bird.getPositionF(), relativeDistance });
        }

        if (!closestDistance || distance < *closestDistance)
        {
          closestDistance = distance;
          closestAngle = angle;
        }

        avgNeighRotation += bird.getRotation();
        avgNeighPosition += bird.getPositionF();
        ++neighCount;
      }
    }
  }

  if (d_avoidMode && closestAngle)
  {
    double ampl = *closestDistance / Settings::FovLength; // 0 - close, 1 - far
    ampl = 1 - ampl; // 0 - far, 1 - close
    ampl *= ampl * ampl * Settings::AvoidForce;
    const double angleDiff = *closestAngle > 0 ? -ampl : ampl;
    const double newRotation = i_obj.getRotation() + angleDiff;
    i_obj.setRotation(newRotation);
    i_obj.setSpeed(getSpeed(newRotation));
  }

  if (d_matchMode && neighCount > 0)
  {
    avgNeighRotation /= (float)neighCount;
    const double diff = avgNeighRotation - i_obj.getRotation();
    const double diffToAdd = diff > 0 ? Settings::MatchForce : -Settings::MatchForce;
    const double newRotation = i_obj.getRotation() + diffToAdd;
    i_obj.setRotation(newRotation);
    i_obj.setSpeed(getSpeed(newRotation));
  }

  if (d_flockMode && neighCount > 0)
  {
    avgNeighPosition /= (float)neighCount;
    const auto dirToFlockCenter = avgNeighPosition - i_obj.getPositionF();
    const double angle = i_obj.getSpeedF().angle(dirToFlockCenter);
    const double angleToAdd = angle > 0 ? Settings::FlockForce : -Settings::FlockForce;
    const double newRotation = i_obj.getRotation() + angleToAdd;
    i_obj.setRotation(newRotation);
    i_obj.setSpeed(getSpeed(newRotation));
  }
}


void BirdsGame::renderObjects()
{
  renderFov();
  Game::renderObjects();
  renderAvoid();
  renderMatch();
  renderFlockCenter();
}

void BirdsGame::renderObject(const Dx::IObject& i_obj)
{
  Game::renderObject(i_obj);
  renderVelocity(i_obj);
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
  if (!d_avoidMode || d_adjs.empty() || !d_birdIsSelected)
    return;

  for (const auto& adj : d_adjs)
  {
    Dx::Renderer2dGuard rendererGuard(getRenderer2d());
    getRenderer2d().renderLine(adj.p0, adj.p1,
                               Dx::colorWithAlpha(Dx::Colors::Red, std::sqrt(1.0f - (float)adj.distance)));
  }
}

void BirdsGame::renderMatch()
{
  if (!d_matchMode || d_adjs.empty() || !d_birdIsSelected)
    return;

  Dx::Renderer2dGuard rendererGuard(getRenderer2d());
  for (const auto& adj : d_adjs)
    getRenderer2d().renderLine(adj.p0, adj.p1, Dx::Colors::AliceBlue);
}

void BirdsGame::renderVelocity(const Dx::IObject& i_obj)
{
  if (!d_matchMode || !d_birdIsSelected)
    return;

  auto endPoint = i_obj.getSpeedF();
  endPoint.normalize();
  endPoint = i_obj.getPositionF() + endPoint * Settings::DirectionVectorLength;

  Dx::Renderer2dGuard rendererGuard(getRenderer2d());
  getRenderer2d().renderLine(i_obj.getPositionF(), endPoint, Dx::Colors::DarkSeaGreen1);
}

void BirdsGame::renderFlockCenter()
{
  if (!d_flockMode || d_adjs.empty() || !d_birdIsSelected)
    return;

  Sdk::Vector2F flockCenter;
  for (const auto& adj : d_adjs)
    flockCenter += adj.p1;
  flockCenter /= (float)d_adjs.size();

  Dx::Renderer2dGuard rendererGuard(getRenderer2d(), flockCenter - d_flockCenterSize / 2.0f);
  getRenderer2d().renderSprite(d_flockCenter);
}
