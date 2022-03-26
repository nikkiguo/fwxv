#include <stdbool.h>

#include "delay.h"
#include "log.h"
#include "task_test_helpers.h"
#include "tasks.h"
#include "unity.h"

// These are tests for tasks.h, but also an example of using the task_test_helpers.h library.

static bool s_task_1_started;
static bool s_task_2_started;
static void *s_passed_context;

void setup_test(void) {
  s_task_1_started = false;
  s_task_2_started = false;
  s_passed_context = NULL;
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

TASK_TEST(test_running_task, TASK_STACK_512) {
  LOG_DEBUG("running starting\n");
  // Start the task: note no need to call tasks_start() because we're inside a test task and
  // FreeRTOS is already running.
  tasks_init_task(my_task_1, TASK_PRIORITY(1), NULL);

  // The task doesn't start immediately because the test task has the highest priority.
  TEST_ASSERT_FALSE(s_task_1_started);

  // To let it run, use a delay.
  delay_ms(50);

  // The task should have run.
  TEST_ASSERT_TRUE(s_task_1_started);
  LOG_DEBUG("running ending\n");
}

TASK_TEST(test_strict_priority_scheduling, TASK_STACK_512) {
  LOG_DEBUG("strict priority scheduling starting\n");

  // Run two tasks, one at a higher priority: only that one should run.
  tasks_init_task(my_task_1, TASK_PRIORITY(1), NULL);
  tasks_init_task(my_task_2, TASK_PRIORITY(2), NULL);

  // Run the tasks.
  delay_ms(50);

  // Only task 2 should have run.
  TEST_ASSERT_FALSE(s_task_1_started);
  TEST_ASSERT_TRUE(s_task_2_started);
  LOG_DEBUG("strict priority scheduling ending\n");
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

// A task to test context passing.
TASK(context_task, TASK_STACK_512) {
  LOG_DEBUG("context_task starting\n");
  s_passed_context = context;

  while (true) {
  }
}

TASK_TEST(test_context, TASK_STACK_512) {
  LOG_DEBUG("context starting\n");
  // Pass a context, make sure we get it back.
  uint8_t arbitrary = 0; // just used for a pointer
  tasks_init_task(context_task, TASK_PRIORITY(1), &arbitrary);

  // Run it, context should be as passed.
  delay_ms(50);
  TEST_ASSERT_EQUAL(&arbitrary, s_passed_context);
  LOG_DEBUG("context ending\n");
}
