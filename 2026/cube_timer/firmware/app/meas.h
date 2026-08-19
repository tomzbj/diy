#ifndef _MEAS_H
#define _MEAS_H

void MEAS_Config(void);
void MEAS_Poll(void);

// Main loop: blink 0:00 + LVGL while alarming (LVGL-safe)
void ALARM_Task(void);

// SysTick 1ms: Morse H beeps only (no LVGL)
void ALARM_Systick(void);

int MEAS_IsAlarming(void);

#endif
