#include <stdbool.h>

#include "delay.h"
#include "log.h"
#include "task_test_helpers.h"
#include "tasks.h"
#include "FreeRTOS.h"
#include "unity.h"
#include "mutex.h"
#include "spi.h"
#include "spi_mcu.h"

// These are tests for tasks.h, but also an example of using the task_test_helpers.h library.
#define MUTEX_WAIT_MS BLOCK_INDEFINITELY
#define LONG_BUFFER_LEN 100000
#define SHORT_BUFFER_LEN 16

static Mutex test_mutex = {0};
static bool s_spi_rx_long_started;
static bool s_spi_rx_short_started;

static uint8_t s_number;
static uint8_t s_long_buffer[LONG_BUFFER_LEN] = {0};
static uint8_t s_short_buffer[SHORT_BUFFER_LEN] = {0};

static const SpiSettings spi_settings = { .mode= SPI_PORT_1 };
static bool s_in_long = false;

void setup_test(void) {
  s_spi_rx_long_started= false;
  s_spi_rx_short_started = false;
}

void teardown_test(void) {}

TASK(spi_rx_long, TASK_STACK_512){
  s_spi_rx_long_started=true;
  s_in_long = true;
  while (true) {
    TEST_ASSERT_TRUE(s_in_long);
    LOG_DEBUG("LONG SPI TX\n");
    spi_tx(SPI_PORT_1, s_long_buffer, LONG_BUFFER_LEN);
    TEST_ASSERT_TRUE(s_in_long);
    LOG_DEBUG("LONG SPI RX\n");
    spi_rx(SPI_PORT_1, s_long_buffer, LONG_BUFFER_LEN, 0);
    s_in_long = false;
    vTaskDelay(500);
    TEST_ASSERT_TRUE(s_in_long);
  }
}

TASK(spi_rx_short, TASK_STACK_512){
  s_spi_rx_short_started=true;
  s_in_long = false;
  while (true) {
    TEST_ASSERT_FALSE(s_in_long);
    LOG_DEBUG("SHORT SPI TX\n");
    spi_tx(SPI_PORT_1, s_short_buffer, SHORT_BUFFER_LEN);
    TEST_ASSERT_FALSE(s_in_long);
    LOG_DEBUG("SHORT SPI RX\n");
    spi_rx(SPI_PORT_1, s_short_buffer, SHORT_BUFFER_LEN, 0);
    s_in_long = true;
    vTaskDelay(5000);
    TEST_ASSERT_FALSE(s_in_long);
    
  }
}

TASK_TEST(test_running_task, TASK_STACK_512) {
  
  spi_init(SPI_PORT_1, &spi_settings);
  tasks_init_task(spi_rx_long, TASK_PRIORITY(3), NULL);
  tasks_init_task(spi_rx_short, TASK_PRIORITY(3), NULL);


  // The task doesn't start immediately because the test task has the highest priority.
  TEST_ASSERT_FALSE(s_spi_rx_long_started);
  TEST_ASSERT_FALSE(s_spi_rx_short_started);

  // To let it run, use a delay.
  delay_ms(20000);

  // The task should have run.
  TEST_ASSERT_TRUE(s_spi_rx_long_started);
  TEST_ASSERT_TRUE(s_spi_rx_short_started);
}
