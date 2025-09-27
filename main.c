#include "intrinsics.h"
#include <msp430.h>
#include <stdbool.h>

#include "main.h"
#include "led.h"
#include "button.h"

volatile bool buttonPressed = false;

state_t (*state_table[MAX_STATES])() = {
    runIdleState, runRightState, runLeftState, runLostState, runWonState};

void main() {
  state_t currentState = IDLE;
  state_t nextState = currentState;

  init();

  while (1) {
    if (currentState < MAX_STATES)
      nextState = state_table[currentState]();

    currentState = nextState;

    delay_ms(DELAY);
  }
}

void init() {
  WDTCTL = WDTPW + WDTHOLD;

  BUTTON_DIR_REG &= ~BUTTON_PIN;
  BUTTON_REN_REG |= BUTTON_PIN;
  BUTTON_OUT_REG |= BUTTON_PIN;

  BUTTON_IES_REG |= BUTTON_PIN;
  BUTTON_IE_REG |= BUTTON_PIN;

  LED_SEL_REG &= ~BIT6;
  LED_SEL2_REG &= ~BIT6;
  LED_DIR_REG |= LED_MASK_ALL;

  __enable_interrupt();
}

static inline void allLedOff() { LED_OUT_REG &= ~(LED_MASK_ALL); }

static inline void updateLED(int currentLED, int previousLED) {
  LED_OUT_REG |= (1 << currentLED);
  LED_OUT_REG &= ~(1 << previousLED);
}

static inline state_t winCheck(int currentLED) {
  return currentLED == LED_WIN ? WON : LOST;
}

static inline state_t runIdleState() {
  allLedOff();

  if (buttonPressed) {
    buttonPressed = false;
    return RIGHT;
  }

  return IDLE;
}

static inline state_t runRightState() {
  volatile int currentLED = LED_FIRST;

  for (currentLED = LED_FIRST; currentLED <= LED_LAST; currentLED++) {
    updateLED(currentLED, (currentLED - 1));

    if (buttonPressed)
      return winCheck(currentLED);

    delay_ms(DELAY);
  }

  return LEFT;
}

static inline state_t runLeftState() {
  volatile int currentLED = LED_LAST;

  for (currentLED = LED_LAST; currentLED >= LED_FIRST; currentLED--) {
    updateLED(currentLED, (currentLED + 1));

    if (buttonPressed)
      return winCheck(currentLED);

    delay_ms(DELAY);
  }

  return RIGHT;
}

static inline state_t runWonState() {
  LED_OUT_REG &= ~LED_MASK_EVEN;
  LED_OUT_REG |= LED_MASK_ODD;

  volatile int i = 0;
  for (i = 0; i < TIMER; i++) {
    LED_OUT_REG ^= LED_MASK_NO_WIN;
    LED_OUT_REG |= LED_MASK_WIN;

    delay_ms(DELAY);
  }

  buttonPressed = false;
  return IDLE;
}

static inline state_t runLostState() {
  delay_ms(DELAY);

  buttonPressed = false;
  return IDLE;
}

void delay_ms(uint64_t milliseconds) {
  static const uint64_t CYCLES_PER_MS = DEFAULT_CLK / 1000;
  uint16_t i;

  for (i = 0; i < milliseconds; i++) {
    __delay_cycles(CYCLES_PER_MS);
  }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR() {
  buttonPressed = true;
  BUTTON_IFG_REG &= ~BUTTON_PIN;
}
