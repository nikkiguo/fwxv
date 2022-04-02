#include <stdbool.h>
#include <stddef.h>

#include "delay.h"
#include "log.h"
#include "tasks.h"
#include "task_test_helpers.h"
#include "unity.h"

static void *s_passed_context;

void setup_test(void) {
  s_passed_context = NULL;
}

void teardown_test(void) {}

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
