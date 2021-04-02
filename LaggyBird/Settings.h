#pragma once

#include <LaggySdk/Math.h>


class Settings
{
public:
  static constexpr int ClientWidth = 1600;
  static constexpr int ClientHeight = 900;

  static constexpr int OffsetToTeleport = 64;
  static constexpr int PointsForCircle = 40;

  static constexpr int BirdsCount = 100;
  static constexpr double SpeedValue = 300;

  static constexpr double FovLength = 100;
  static constexpr float FovAlpha = 0.5f;
  static constexpr float DefaultFieldOfView = Sdk::degToRad<float>(240.0f);
  static constexpr float MinFovDeg = 5;
  static constexpr float MinFovRad = Sdk::degToRad<float>(MinFovDeg);
  static constexpr float MaxFovDeg = 360;
  static constexpr float MaxFovRad = Sdk::degToRad<float>(MaxFovDeg);
  static constexpr float FovIncrementDeg = 5;
  static constexpr float FovIncrementRad = Sdk::degToRad<float>(FovIncrementDeg);

  static constexpr double AvoidForce = 0.2;
  static constexpr double MatchForce = 0.02;

  static constexpr float DirectionVectorLength = 50;
};
