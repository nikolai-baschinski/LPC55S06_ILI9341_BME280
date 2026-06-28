#include "LPC55S06.h"
#include "CLOCK.h"

void init_CLOCK()
{
  // Enable 96 MHz free running oscillator
  ANACTRL->FRO192M_CTRL |= ANACTRL_FRO192M_CTRL_ENA_96MHZCLK(1);

  SYSCON->AHBCLKDIV = 0;   // divide by one
  SYSCON->MAINCLKSELA = 3; // Set clock source to FRO 96 MHz
  SYSCON->MAINCLKSELB = 0; // Set clock source to main clock A
}
