// (c) Nikolai Baschinski

#include "LPC55S06.h"
#include "FPU.h"
#include "PMC.h"
#include "FLASH.h"
#include "CLOCK.h"
#include "SPI.h"
#include "GPIO.h"
#include "TIMER.h"
#include "NVIC.h"
#include "lcd.h"
#include "bme.h"

struct ProcessImage pi={0};

int main(void)
{
  init_FPU();
  init_PCM();
  init_FLASH();
  init_CLOCK();
  init_SPI();
  init_GPIO();
  init_TIMER();
  init_LCD();
  init_BME();
  init_NVIC();

  while(1){
    cyclic_BME(&pi.bme280);
    cyclic_LCD(&pi);
    delay(1000);
  }
  return 0;
}

void CTIMER0_IRQHandler(void)
{
  CTIMER0->IR = CTIMER_IR_MR0INT_MASK;

  if(pi.cntr_10ms%100 == 0) {
    GPIO->NOT[0] = (1UL << 22);
  }
  pi.cntr_10ms++;
  GPIO->NOT[0] = (1UL << 9);
}
