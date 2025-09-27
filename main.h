#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define TIMER 5
#define DELAY 200
#define DEFAULT_CLK 1000000.0f

typedef enum { IDLE, RIGHT, LEFT, LOST, WON, MAX_STATES } state_t;

void init();

static inline state_t winCheck(int currentLED);

static inline state_t runIdleState();
static inline state_t runRightState();
static inline state_t runLeftState();
static inline state_t runWonState();
static inline state_t runLostState();

void delay_ms(uint64_t ms);

#endif