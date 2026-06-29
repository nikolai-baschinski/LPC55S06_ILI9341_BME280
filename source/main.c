// (c) Nikolai Baschinski

#include "LPC55S06.h"
#include "PMC.h"
#include "FLASH.h"
#include "CLOCK.h"
#include "SPI.h"
#include "GPIO.h"
#include "TIMER.h"
#include "lcd.h"

uint32_t cntr = 0;

unsigned char data[3] = {0x02, 0x32, 0xF6};

int main(void)
{
  init_PCM();
  init_FLASH();
  init_CLOCK();
  init_SPI();
  init_GPIO();
  init_TIMER();
  init_LCD();
  while(1);
  return 0;
}

void CTIMER0_IRQHandler(void)
{
  CTIMER0->IR = CTIMER_IR_MR0INT_MASK;

  if(cntr%100 == 0) {
    GPIO->NOT[0] = (1UL << 22);
  }
  cntr++;
  GPIO->NOT[0] = (1UL << 9);

  // GPIO_set_P0_4(0);
  // SPI_Transmit(data, 3);
  // GPIO_set_P0_4(1);
}
