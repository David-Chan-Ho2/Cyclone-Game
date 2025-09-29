#include "state.h"

extern volatile bool buttonPressed;

static inline state_t runSweepState(int start, int end, int step,
                                    state_t nextState) {
  volatile int currentLED;
  for (currentLED = start; currentLED != end + step; currentLED += step) {
    setLED(LED_MASK_ALL, false);
    updateLED(currentLED);

    if (buttonPressed) {
      return winCheck(currentLED);
    }

    delay_ms(DELAY);
  }

  return nextState;
}

static inline state_t winCheck(int currentLED) {
  return (currentLED == LED_WIN) ? WON : LOST;
}

state_t (*state_table[MAX_STATES])() = {
    runIdleState, runRightState, runLeftState, runLostState, runWonState};

state_t runState(state_t currentState) {
  if (currentState < MAX_STATES) {
    return state_table[currentState]();
  }

  return IDLE;
}

state_t runIdleState() {
  setLED(LED_MASK_ALL, false);

  if (buttonPressed) {
    setButton(false);
    return RIGHT;
  }

  return IDLE;
}

state_t runRightState() {
  return runSweepState(LED_FIRST, LED_LAST, 1, LEFT); //
}

state_t runLeftState() {
  return runSweepState(LED_LAST, LED_FIRST, -1, RIGHT); //
}

state_t runWonState() {
  setLED(LED_MASK_EVEN, false);
  setLED(LED_MASK_ODD, true);

  volatile int i = 0;
  for (i = 0; i < TIMER; i++) {
    toggleLED(LED_MASK_NO_WIN);
    setLED(LED_MASK_WIN, true);

    delay_ms(DELAY);
  }

  setButton(false);
  return IDLE;
}

state_t runLostState() {
  delay_ms(DELAY);

  setButton(false);
  return IDLE;
}
