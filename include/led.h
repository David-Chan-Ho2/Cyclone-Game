#ifndef LED_H
#define LED_H

#include <msp430.h>
#include <stdbool.h>

#define LED_MASK_ALL (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6)
#define LED_MASK_ODD (BIT1 | BIT5)
#define LED_MASK_EVEN (LED_MASK_ALL & ~LED_MASK_ODD)
#define LED_MASK_WIN BIT3
#define LED_MASK_NO_WIN (LED_MASK_ALL & ~LED_MASK_WIN)

#define LED_LAST 6
#define LED_FIRST 0
#define LED_WIN 3
#define LED_DIR_REG P2DIR
#define LED_OUT_REG P2OUT
#define LED_SEL_REG P2SEL
#define LED_SEL2_REG P2SEL2

void initLEDGPIO();

void updateLED(int LED);
void setLED(int LED, bool enable);
void toggleLED(int LED);

#endif
