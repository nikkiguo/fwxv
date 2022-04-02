#include <stdbool.h>
#include <stddef.h>

#include "delay.h"
#include "log.h"
#include "tasks.h"
#include "task_test_helpers.h"
#include "unity.h"

static bool s_task_1_started;
static bool s_task_2_started;

void setup_test(void) {
  s_task_1_started = false;
  s_task_2_started = false;
}

void teardown_test(void) {}

// A simple task to test.
TASK(my_task_1, TASK_STACK_512) {
  LOG_DEBUG("my_task_1 started\n");
  s_task_1_started = true;

  // Loop forever, because tasks should not exit.
  while (true) {
  }
}

TASK(my_task_2, TASK_STACK_512) {
  LOG_DEBUG("my_task_2 started\n");
  s_task_2_started = true;

  while (true) {
  }
}

TASK_TEST(test_time_slicing, TASK_STACK_512) {
  LOG_DEBUG("time slicing starting\n");
  // Run two tasks, at the same priority: both should run because of time slicing.
  tasks_init_task(my_task_1, TASK_PRIORITY(1), NULL);
  tasks_init_task(my_task_2, TASK_PRIORITY(1), NULL);

  // Run the tasks.
  delay_ms(50);

  // Both should have run.
  TEST_ASSERT_TRUE(s_task_1_started);
  TEST_ASSERT_TRUE(s_task_2_started);
  LOG_DEBUG("time slicing ending\n");
}
