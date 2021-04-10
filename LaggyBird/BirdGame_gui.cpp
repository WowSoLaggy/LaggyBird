#include "stdafx.h"
#include "BirdsGame.h"

#include <LaggyDx/FadeOutEffect.h>
#include <LaggyDx/Label.h>


void BirdsGame::createTooltip(const bool i_withEffect)
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

  if (i_withEffect)
  {
    auto effect = std::make_shared<Dx::FadeOutEffect>(5, 1);
    effect->setOnFinishHandler(std::bind(&BirdsGame::deleteTooltip, this));
    d_tooltip->addEffect(effect);
  }

  getForm().addChild(d_tooltip);
}

void BirdsGame::deleteTooltip()
{
  CONTRACT_EXPECT(d_tooltip);

  getForm().removeChild(*d_tooltip);
  d_tooltip.reset();
}
