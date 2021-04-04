#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Label.h>


void BirdsGame::createGui()
{
  auto label = std::make_shared<Dx::Label>();
  label->setFont("play.spritefont");
  label->setText("Welcome to the Bird Flock Simulation!\n" \
                 "\n" \
                 "Controls:\n" \
                 "'Esc' - exit\n" \
                 "'Space' - pause\n" \
                 "\n" \
                 "Modes:\n" \
                 "'~' - show metadata\n" \
                 "'1' - enable collision avoidance\n" \
                 "'2' - enable direction matching\n" \
                 "'3' - enable flocking\n" \
                 "\n" \
                 "Field of view:\n" \
                 "'-' - decrease FOV\n" \
                 "'+' - increase FOV\n" \
                 "'0' - reset FOV");
  label->setPosition({ 32, 32 });
  getForm().addChild(label);
}
