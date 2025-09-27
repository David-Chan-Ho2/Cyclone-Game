#include "main.h"
#include "button.h"
#include "led.h"
#include "state.h"
#include "utils.h"

void main() {
  state_t currentState = IDLE;
  state_t nextState = currentState;

  initButtonGPIO();
  initLEDGPIO();

  while (1) {
    if (currentState < MAX_STATES)
      nextState = runState(currentState);

    currentState = nextState;
    delay_ms(DELAY);
  }
}
