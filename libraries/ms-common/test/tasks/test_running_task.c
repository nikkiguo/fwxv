#include <stdbool.h>
#include <stddef.h>

#include "delay.h"
#include "log.h"
#include "tasks.h"
#include "task_test_helpers.h"
#include "unity.h"

// This is a task test for tasks.h.
// We can only have one task test per file due to FreeRTOS scheduler nonsense.
// We also can't mix task tests and normal tests in one file.

static bool s_task_started;

void setup_test(void) {
  s_task_started = false;
}

void teardown_test(void) {}

// A simple task to test.
TASK(my_task, TASK_STACK_512) {
  LOG_DEBUG("my_task started\n");
  s_task_started = true;

  // Loop forever, because tasks should not exit.
  while (true) {
  }
}

TASK_TEST(test_running_task, TASK_STACK_512) {
  LOG_DEBUG("running starting\n");
  // Start the task: note no need to call tasks_start() because we're inside a test task and
  // FreeRTOS is already running.
  tasks_init_task(my_task, TASK_PRIORITY(1), NULL);

  // The task doesn't start immediately because the test task has the highest priority.
  TEST_ASSERT_FALSE(s_task_started);

  // To let it run, use a delay.
  delay_ms(50);

  // The task should have run.
  TEST_ASSERT_TRUE(s_task_started);
  LOG_DEBUG("running ending\n");
}
