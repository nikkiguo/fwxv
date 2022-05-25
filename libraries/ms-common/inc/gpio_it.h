#pragma once
// GPIO Interrupt Handlers
// Requires GPIO and interrupts to be initialized.
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "gpio.h"
#include "interrupt_def.h"
#include "notify.h"
#include "status.h"
#include "task.h"

// Initializes the interrupt handler for GPIO.
void gpio_it_init(void);

// Gets the interrupt edge the interrupt is set to trigger on
StatusCode gpio_it_get_edge(const GpioAddress *address, InterruptEdge *edge);

// Registers a new callback on a given port pin combination with the desired
// settings. Set the notify value bit of the task when a gpio_it occurs.
// The task to notify needs to be initialized before calling register interrupt.
// Register interrupt needs to be called before scheduler starts.
StatusCode gpio_it_register_interrupt(const GpioAddress *address, const InterruptSettings *settings,
                                      const Event event, const TaskHandle_t task);

// Triggers an interrupt in software.
StatusCode gpio_it_trigger_interrupt(const GpioAddress *address);

// Masks the interrupt for the given address if masked is True.
// Enables the interrupt if masked is false.
StatusCode gpio_it_mask_interrupt(const GpioAddress *address, bool masked);
