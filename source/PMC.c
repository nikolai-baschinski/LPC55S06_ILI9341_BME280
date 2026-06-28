#include "LPC55S06.h"
#include "PMC.h"

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

void init_PCM() {
  set_power_profile_96MHz();
}

