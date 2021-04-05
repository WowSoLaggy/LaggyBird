#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/Label.h>


void BirdsGame::createTooltip()
{
  CONTRACT_EXPECT(!d_tooltip);

  d_tooltip = std::make_shared<Dx::Label>();
  d_tooltip->setFont("play.spritefont");
  d_tooltip->setText("Welcome to the Bird Flock Simulation!\n" \
                 "\n" \
                 "Controls:\n" \
                 "'Esc' - exit\n" \
                 "'Space' - pause\n" \
                 "'F1' - show / hide this tooltip\n" \
                 "\n" \
                 "Modes:\n" \
                 "'~' - show / hide metadata\n" \
                 "'1' - switch collision avoidance\n" \
                 "'2' - switch direction matching\n" \
                 "'3' - switch flocking\n" \
                 "\n" \
                 "Field of view:\n" \
                 "'-' - decrease FOV\n" \
                 "'+' - increase FOV\n" \
                 "'0' - reset FOV");
  d_tooltip->setPosition({ 32, 32 });

  getForm().addChild(d_tooltip);
}

void BirdsGame::deleteTooltip()
{
  CONTRACT_EXPECT(d_tooltip);

  getForm().removeChild(*d_tooltip);
  d_tooltip.reset();
}
