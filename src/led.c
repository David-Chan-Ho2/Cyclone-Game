#include "led.h"

void initLEDGPIO() {
  LED_SEL_REG &= ~BIT6;
  LED_SEL2_REG &= ~BIT6;
  LED_DIR_REG |= LED_MASK_ALL;
}

void updateLED(int LED) {
  LED_OUT_REG |= (1 << LED); //
}

void setLED(int LED, bool enable) {
  if (enable) {
    LED_OUT_REG |= LED;
  } else {
    LED_OUT_REG &= ~LED;
  }
}

void toggleLED(int LED) {
  LED_OUT_REG ^= LED; //
}
