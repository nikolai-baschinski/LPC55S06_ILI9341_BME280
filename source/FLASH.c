#include "LPC55S06.h"
#include "FLASH.h"

void set_flash_wait_states_96MHz(void)
{
  const uint32_t num_wait_states = 7U;

  // Save prefetch state
  uint32_t prefetch_enable_mask = SYSCON->FMCCR & SYSCON_FMCCR_PREFEN_MASK;

  // disable prefetch before flash commands
  SYSCON->FMCCR &= ~SYSCON_FMCCR_PREFEN_MASK;

  // Delete all status flags
  FLASH->INT_CLR_STATUS = 0x1FU;

  // Set new wait value
  FLASH->DATAW[0] = (FLASH->DATAW[0] & 0xFFFFFFF0UL) | (num_wait_states & 0xFU);

  // CMD_SET_READ_MODE
  FLASH->CMD = 0x2U;

  // wait
  while ((FLASH->INT_STATUS & FLASH_INT_STATUS_DONE_MASK) == 0U);

  // Set FMC
  SYSCON->FMCCR = (SYSCON->FMCCR & ~SYSCON_FMCCR_FLASHTIM_MASK) | ((num_wait_states << SYSCON_FMCCR_FLASHTIM_SHIFT) & SYSCON_FMCCR_FLASHTIM_MASK);

  // Restore prefetch state
  SYSCON->FMCCR |= prefetch_enable_mask;
}

void init_FLASH()
{
  set_flash_wait_states_96MHz();
}
