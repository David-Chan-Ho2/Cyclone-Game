#ifndef STATE_H
#define STATE_H

#include "button.h"
#include "led.h"
#include "main.h"
#include "utils.h"

typedef enum { IDLE, RIGHT, LEFT, LOST, WON, MAX_STATES } state_t;

static inline state_t winCheck(int currentLED);
static inline state_t runSweepState(int start, int end, int step,
                                    state_t nextState);
state_t runState(state_t current);
state_t runIdleState();
state_t runRightState();
state_t runLeftState();
state_t runWonState();
state_t runLostState();

#endif
