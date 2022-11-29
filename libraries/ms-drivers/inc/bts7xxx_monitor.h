#pragma once

// Module used to check for errors in sets of bts7xxx load switches


#include "adc.h"
#include "bts7xxx_common.h"

typedef struct BtsMonitorStorage {
  GpioAddress *sense_pin;
  :q
