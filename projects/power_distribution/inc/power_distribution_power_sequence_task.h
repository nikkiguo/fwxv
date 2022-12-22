#pragma once

#include "delay.h"
#include "fsm.h"
#include "gpio.h"
#include "log.h"
#include "notify.h"
#include "task.h"

#define NUM_POWER_SEQUENCE_STATES 4
#define NUM_POWER_SEQUENCE_TRANSITIONS 7

DECLARE_FSM(power_sequence);

typedef enum PowerSequenceStateId {
  POWER_SEQUENCE_INIT_STATE = 0,
  POWER_SEQUENCE_DRIVER_CONTROLS_STATE,
  POWER_SEQUENCE_POWER_MAIN_EVERYTHING_STATE,
  POWER_SEQUENCE_MAIN_OPERATION_STATE,
} PowerSequenceStateId;

// Notifications to send to FSM 2
typedef enum Fsm1OutputEvent {
  FSM1_STATE_0_CMPL = 0,
  FSM1_STATE_1_CMPL,
  FSM1_STATE_2_CMPL,
} Fsm1OutputEvent;

typedef struct Fsm1Storage {
  GpioAddress start_btn_address;
} Fsm1Storage;

StatusCode init_power_sequence(void);
