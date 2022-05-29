#ifndef PUSHBUTTON_H_INCLUDED
#define PUSHBUTTON_H_INCLUDED

#include <Arduino.h>
#include "common.h"
#include "eeprom-handler.h"

#define BTN_PIN 13
#define BTN_PRESS_LENGTH 2000 //ms
#define BTN_SAMPLE_INTERVAL 25
#define BTN_RESET_CYCLES 80    // 80 * 25 = 2000 ms
#define BTN_PRESSED_STATE LOW
#define BTN_RELEASED_STATE HIGH

bool checkButtonPressed();
void setupButton();

#endif