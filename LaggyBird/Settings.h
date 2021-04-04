#pragma once

#include <LaggySdk/Math.h>


class Settings
{
public:
  static constexpr int ClientWidth = 1600;
  static constexpr int ClientHeight = 900;

  static constexpr int OffsetToTeleport = 32;
  static constexpr int PointsForCircle = 40;

  static constexpr int BirdsCount = 200;
  static constexpr double SpeedValue = 500;

  static constexpr double FovLength = 100;
  static constexpr float FovAlpha = 0.5f;
  static constexpr float DefaultFieldOfView = Sdk::degToRad<float>(240.0f);
  static constexpr float MinFovDeg = 5;
  static constexpr float MinFovRad = Sdk::degToRad<float>(MinFovDeg);
  static constexpr float MaxFovDeg = 360;
  static constexpr float MaxFovRad = Sdk::degToRad<float>(MaxFovDeg);
  static constexpr float FovIncrementDeg = 5;
  static constexpr float FovIncrementRad = Sdk::degToRad<float>(FovIncrementDeg);

  static constexpr double AvoidForce = 0.3;
  static constexpr double MatchForce = 0.02;
  static constexpr double FlockForce = 0.05;

  static constexpr float DirectionVectorLength = 50;
};
