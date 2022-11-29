#pragma once
// Common definitions for BTS7XXX-series load switches.
// Most of the functions in this file allow for a common API to easily interact with the
// EN pin(s) on the load switches, abstracting the specific method of accessing the pin.

#include "FreeRTOS.h"
#include "pca9539r_gpio_expander.h"
#include "soft_timer.h"

// Fault restart delay is the same across both the BTS7040 and the BTS7200
#define BTS7XXX_FAULT_RESTART_DELAY_MS 110 

#define STM32_GPIO_STATE_SELECT_OUT_0 GPIO_STATE_LOW
#define STM32_GPIO_STATE_SELECT_OUT_1 GPIO_STATE_HIGH
#define PCA9539R_GPIO_STATE_SELECT_OUT_0 PCA9539R_GPIO_STATE_LOW
#define PCA9539R_GPIO_STATE_SELECT_OUT_1 PCA9539R_GPIO_STATE_HIGH

// Represents whether SEL/EN pins are accessed through STM32 or a Pca9539R
typedef enum {
  BTS7XXX_PIN_STM32 = 0,
  BTS7XXX_PIN_PCA9539R,
  NUM_BTS7XXX_PIN_TYPES,
} Bts7xxxPinType;

// Holds pin-specific info for EN pins
typedef struct {
  GpioAddress *enable_pin_stm32;
  Pca9539rGpioAddress *enable_pin_pca9539r;
  bool fault_in_progress;
  TickType_t fault_delay;
  Bts7xxxPinType pin_type;
} Bts7xxxEnablePin;

// Holds pin-specific info for SEL pins
typedef struct {
  GpioAddress *select_pin_stm32;
  Pca9539rGpioAddress *select_pin_pca9539r;
  Bts7xxxPinType pin_type;
} Bts7xxxSelectPin;

// Broad function to initialize the pin passed in.
// If using with PCA9539R, pca9539r_gpio_init must be called on the i2c port and address of this pin
// prior to calling this function.
StatusCode bts7xxx_init_pin(Bts7xxxEnablePin *pin);

// Broad function to enable the pin passed in.
StatusCode bts7xxx_enable_pin(Bts7xxxEnablePin *pin);

// Broad function to disable the pin passed in.
StatusCode bts7xxx_disable_pin(Bts7xxxEnablePin *pin);

// Broad function to get whether the pin passed in is enabled.
StatusCode bts7xxx_get_pin_enabled(Bts7xxxEnablePin *pin);

// Called on a pin if a fault is detected
// Disables pin and sets a time to delay until, then enables again
// Must be called repeatedly
StatusCode bts7xxx_handle_fault_pin(Bts7xxxEnablePin *pin);
