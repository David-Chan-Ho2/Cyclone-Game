#include "button.h"

volatile bool buttonPressed = false;

void initButtonGPIO() {
  BUTTON_DIR_REG &= ~BUTTON_PIN;
  BUTTON_REN_REG |= BUTTON_PIN;
  BUTTON_OUT_REG |= BUTTON_PIN;

  BUTTON_IES_REG |= BUTTON_PIN;
  BUTTON_IE_REG |= BUTTON_PIN;
}

void setButton(bool enable) {
  if (enable) {
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR() {
  setButton(true);
  BUTTON_IFG_REG &= ~BUTTON_PIN;
}
