#include "precharge_control.h"

#include "can.h"
#include "gpio.h"
#include "gpio_it.h"
#include "interrupt.h"
#include "log.h"
#include "motor_controller_getters.h"
#include "motor_controller_setters.h"
#include "status.h"

static PrechargeControlStorage s_precharge_storage = { 0 };

PrechargeControlStorage *test_get_storage(void) {
  return &s_precharge_storage;
}

StatusCode prv_set_precharge_control(PrechargeControlStorage *storage, const GpioState state) {
  gpio_set_state(&storage->precharge_control, state);
  return STATUS_CODE_OK;
}

TASK(PRECHARGE_INTERRUPT, TASK_MIN_STACK_SIZE) {
  uint32_t notification;
  while (true) {
    notify_wait(&notification, BLOCK_INDEFINITELY);

    if (notify_check_event(&notification, 0)) {
      if (get_drive_output_precharge() == MCI_PRECHARGE_DISCHARGED) {
        // inconsistent until second precharge result
        set_mc_status_precharge_status(MCI_PRECHARGE_INCONSISTENT);
      } else {
        set_mc_status_precharge_status(MCI_PRECHARGE_CHARGED);
      }
    }
    if (notify_check_event(&notification, 1)) {
      if (get_drive_output_precharge() == MCI_PRECHARGE_DISCHARGED) {
        // inconsistent until second precharge result
        set_mc_status_precharge_status(MCI_PRECHARGE_INCONSISTENT);
      } else {
        set_mc_status_precharge_status(MCI_PRECHARGE_CHARGED);
      }
    }
  }
}

StatusCode run_precharge_rx_cycle() {
  if (get_drive_output_precharge() == MCI_PRECHARGE_CHARGED) {
    set_mc_status_precharge_status(MCI_PRECHARGE_CHARGED);
    return prv_set_precharge_control(&s_precharge_storage, GPIO_STATE_HIGH);
  } else {
    set_mc_status_precharge_status(MCI_PRECHARGE_DISCHARGED);
    return prv_set_precharge_control(&s_precharge_storage, GPIO_STATE_LOW);
  }
}

void prv_populate_storage(PrechargeControlStorage *storage,
                          const PrechargeControlSettings *settings) {
  storage->precharge_control = settings->precharge_control;
}

StatusCode precharge_control_init(const PrechargeControlSettings *settings) {
  interrupt_init();
  prv_populate_storage(&s_precharge_storage, settings);
  InterruptSettings monitor_it_settings = { .type = INTERRUPT_TYPE_INTERRUPT,
                                            .priority = INTERRUPT_PRIORITY_NORMAL,
                                            .edge = INTERRUPT_EDGE_RISING };
  status_ok_or_return(gpio_it_register_interrupt(&settings->precharge_monitor, &monitor_it_settings,
                                                 0, PRECHARGE_INTERRUPT));
  status_ok_or_return(gpio_it_register_interrupt(&settings->precharge_monitor2,
                                                 &monitor_it_settings, 1, PRECHARGE_INTERRUPT));
  status_ok_or_return(tasks_init_task(PRECHARGE_INTERRUPT, TASK_PRIORITY(2), NULL));
  return STATUS_CODE_OK;
}
