// (c) Nikolai Baschinski

#include "LPC55S06.h"

void set_flash_wait_states_96MHz();
void set_power_profile_96MHz();

void init_CLOCK()
{
  set_flash_wait_states_96MHz();

  set_power_profile_96MHz();

  // Enable 96 MHz free running oscillator
  ANACTRL->FRO192M_CTRL |= ANACTRL_FRO192M_CTRL_ENA_96MHZCLK(1);

  SYSCON->AHBCLKDIV = 0;   // divide by one
  SYSCON->MAINCLKSELA = 3; // Set clock source to FRO 96 MHz
  SYSCON->MAINCLKSELB = 0; // Set clock source to main clock A
}

void init_GPIO()
{
  SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_GPIO0_MASK | SYSCON_AHBCLKCTRL0_IOCON_MASK;

  // GPIO Port 0, Pin 22 (LED D4)
  IOCON->PIO[0][22] = 0;
  GPIO->DIRSET[0] = (1UL << 22);

  // GPIO Port 0, Pin 9 (test pin)
  IOCON->PIO[0][9] = 0;
  GPIO->DIRSET[0] = (1UL << 9);
  IOCON->PIO[0][9] = IOCON_PIO_DIGIMODE(1);

  // UM: Once the pins are configured, the IOCON clock can be disabled in order to conserve power.
  SYSCON->AHBCLKCTRLCLR[0] = SYSCON_AHBCLKCTRL0_IOCON_MASK;
}

void init_TIMER()
{
  SYSCON->CTIMERCLKSELX[0] = SYSCON_CTIMERCLKSEL0_SEL(3); // select FRO 96 MHz as source for timer 0
  SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_TIMER0_MASK; // enable clock for timer 0
  SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST(1); // reset peripheral

  CTIMER0->PR = 95; // prescaler
  CTIMER0->MR[0] = 10000; // set match value for 10 ms
  CTIMER0->MCR = CTIMER_MCR_MR0I_MASK | CTIMER_MCR_MR0R_MASK; // interrupt is generated when MR0 matches the value in the TC
  CTIMER0->TC = 0; // set timer counter to 0
  CTIMER0->PC = 0; // set prescale counter to 0
  CTIMER0->IR = CTIMER_IR_MR0INT_MASK;

  NVIC->ISER[0] = (1UL << (uint32_t)CTIMER0_IRQn); // Enable CTIMER0 interrupt

  CTIMER0->TCR = 1; // enable timer 0
}

uint32_t cntr = 0;

void init_SPI()
{
  SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_FC3_MASK; // enable clock for Flexcomm
  SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_FC3_RST(1); // set reset
  while (0u == (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_FC3_RST_MASK));
  SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_FC3_RST(1); // clear reset
  while (SYSCON_PRESETCTRL1_FC3_RST_MASK == (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_FC3_RST_MASK));

  FLEXCOMM3->PSELID = FLEXCOMM_PSELID_PERSEL(2); // choose function SPI
  SYSCON->FCCLKSELX[3] = SYSCON_FCCLKSEL3_SEL(2); // clock source for SPI

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_FUNC8 0x08u         /*!<@brief Selects pin function 8 */
#define IOCON_PIO_INV_DISABLED 0x00u  /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */
#define IOCON_PIO_SLEW_FAST 0x40u
#define IOCON_PIO_OPEN_DRAIN_ENABLED 0x200u

  // MISO
  IOCON->PIO[0][2] = IOCON_PIO_FUNC1 | IOCON_PIO_SLEW_FAST | IOCON_PIO_DIGITAL_EN;
  // MOSI
  IOCON->PIO[0][3] = IOCON_PIO_FUNC1 | IOCON_PIO_SLEW_FAST | IOCON_PIO_DIGITAL_EN;
  // SS0
  IOCON->PIO[0][4] = IOCON_PIO_FUNC8 | IOCON_PIO_SLEW_STANDARD | IOCON_PIO_DIGITAL_EN;
  // SS1
  IOCON->PIO[0][5] = IOCON_PIO_FUNC8 | IOCON_PIO_SLEW_STANDARD | IOCON_PIO_DIGITAL_EN;
  // CLK
  IOCON->PIO[0][6] = IOCON_PIO_FUNC1 | IOCON_PIO_SLEW_FAST | IOCON_PIO_DIGITAL_EN;

  SPI3->FIFOCFG |= SPI_FIFOCFG_ENABLETX(0) | SPI_FIFOCFG_ENABLERX(0); // Disable FIFO TX/RX
  SPI3->DIV = 11; // SPI prescalor
  SPI3->FIFOCFG |= SPI_FIFOCFG_EMPTYTX_MASK | SPI_FIFOCFG_EMPTYRX_MASK; // Empty the buffers
  SPI3->FIFOCFG |= SPI_FIFOCFG_ENABLETX(1) | SPI_FIFOCFG_ENABLERX(1); // Enable FIFO TX/RX
  SPI3->CFG = SPI_CFG_MASTER(1) | SPI_CFG_ENABLE(1);
}

unsigned char data[3] = {0x02, 0x32, 0xF6};

void SPI_Transmit(unsigned char* data, int size)
{
  // Empty the buffers
  SPI3->FIFOCFG |= SPI_FIFOCFG_EMPTYTX_MASK | SPI_FIFOCFG_EMPTYRX_MASK;
  // Reset possible errors
  SPI3->FIFOSTAT = SPI_FIFOSTAT_TXERR_MASK | SPI_FIFOSTAT_RXERR_MASK;

  while (size > 0) {

    // wait till transmit FIFO is not empty
    while ((SPI3->FIFOSTAT & SPI_FIFOSTAT_TXNOTFULL_MASK) == 0);

    uint32_t ctrl = SPI_FIFOWR_LEN(7)        // 8 Bit
                  | SPI_FIFOWR_TXSSEL0_N(0)  // SS0 YES
                  | SPI_FIFOWR_TXSSEL1_N(1); // SS1 NO

    // last byte?
    if (size == 1) {
      ctrl |= SPI_FIFOWR_EOT_MASK; // set "End Of Transfer
    }

    SPI3->FIFOWR = ctrl | *data;

    data++;
    size--;
  }

  // wait will everything is sent
  while ((SPI3->FIFOSTAT & SPI_FIFOSTAT_TXEMPTY_MASK) == 0);
}

int main(void)
{
  init_CLOCK();
  init_SPI();
  init_GPIO();
  init_TIMER();
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
  SPI_Transmit(data, 3);
}

void set_power_profile_96MHz()
{
  // NXP recommended power profiles for 96 MHz

  // dcdc power profile
  #define FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDRS (FLASH_NMPA_BASE + 0xE8U)
  #define FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDRS (FLASH_NMPA_BASE + 0xECU)
  uint32_t dcdcTrimValue0 = (*((volatile unsigned int *)(FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDRS)));
  uint32_t dcdcTrimValue1 = (*((volatile unsigned int *)(FLASH_NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDRS)));

  if (0UL != (dcdcTrimValue0 & 0x1UL)) {
    dcdcTrimValue0 = dcdcTrimValue0 >> 1;
    PMC->DCDC0 = dcdcTrimValue0;
    PMC->DCDC1 = dcdcTrimValue1;
  }

  // set voltage, values see user manual
  uint32_t lv_dcdc         = 5;
  uint32_t lv_ldo_ao       = 22;
  uint32_t lv_ldo_ao_boost = 27;

  // Set up LDO Always-On voltages
  PMC->LDOPMU = (PMC->LDOPMU & (~PMC_LDOPMU_VADJ_MASK) & (~PMC_LDOPMU_VADJ_BOOST_MASK)) | PMC_LDOPMU_VADJ(lv_ldo_ao) |
                PMC_LDOPMU_VADJ_BOOST(lv_ldo_ao_boost);

  // Set up DCDC voltage
  PMC->DCDC0 = (PMC->DCDC0 & (~PMC_DCDC0_VOUT_MASK)) | PMC_DCDC0_VOUT(lv_dcdc);
}

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
